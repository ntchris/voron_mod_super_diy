#define IGNORE_FILA_EXIST 1

//=========================  PIN DEFINITION 1 ============================
//#define FILAMENT_EXIST_PIN_A 2
#define INPUT_SENSOR_PIN_A 2
#define OUTPUT_SENSOR_PIN_A 4   
#define MOTOR_INPUT_EN_PIN_A 3   // motor PCB A1  PWM
#define MOTOR_OUTPUT_EN_PIN_A 5  // motor PCB A2  PWM must.  arduino NANO: 3,5,6,9,10,11 for analog output/PWM



//=========================  PIN DEFINITION 2 ============================
//#define FILAMENT_EXIST_PIN_B 7
#define INPUT_SENSOR_PIN_B 7
#define OUTPUT_SENSOR_PIN_B 8
#define MOTOR_INPUT_EN_PIN_B 6    // motor PCB C1
#define MOTOR_OUTPUT_EN_PIN_B 9  //  motor PCB C2 PWM must. arduino NANO: 3,5,6,9,10,11 for analog output/PWM



//=========================  PIN DEFINITION 3 ============================
//#define FILAMENT_EXIST_PIN_C 11
#define INPUT_SENSOR_PIN_C A0
#define OUTPUT_SENSOR_PIN_C A1
#define MOTOR_INPUT_EN_PIN_C 10   //  motor PCB D1
#define MOTOR_OUTPUT_EN_PIN_C 11  //  motor PCB D2   arduino NANO: 3,5,6,9,10,11 for analog output/PWM



//============================ performance and functional definition ==============================
#define SerialSpeed 115200

// could be high enable or low enable
#define MOTOR_EN_IS_HIGH 1  //  some board use LOW to enable, others use HI to enable

#define MAX_PWM 255 // for arduino
#define OUTPUT_FULL_SPEED_PWM 220   // when fast load, need to run faster
#define OUTPUT_SLOW_SPEED_PWM 98   // 80.  cannot be too low, cannot move heavy spool; also cannot be too high : when printing, to reduce noise.
#define INPUT_SPEED_PWM 220   // if too fast may pull filament away out of ercf gear box

#define MAX_INPUT_TIME_MS 15000
#define MIN_OUTPUT_TIME_MS 300              //1000    output slow output for at least this time.
#define MAX_HIGH_SPEED_OUTPUT_TIME_MS 100  // even output triggered is stuck , do not run motor more than this time

#define OUTPUT_SPEED_UP_TIME 3000  // if output triggerred for more than this time (ie 500ms, 0.5s), speed up the motor



class Motor_Controller {

private:
  int _pin_input, _pin_output, _pin_motor_in, _pin_motor_out;
  String _name;

  unsigned long int output_start_ts = 0;
  unsigned long int output_high_speed_start_ts = 0;  //MAX_HIGH_SPEED_OUTPUT_TIME_MS
  unsigned long int input_start_ts = 0;
  bool force_rollback_stopped = 0;
  bool force_output_stopped = 0;
  bool debug_print = 0;

  void debugPrint(String text) {
    if (this->debug_print) {
      Serial.println(text +" : " + _name);
    }
  };

public:
  Motor_Controller(int pin_input, int pin_output, int pin_motor_in, int pin_motor_out, String name) {

    _pin_input = pin_input;
    _pin_output = pin_output;
    _pin_motor_in = pin_motor_in;
    _pin_motor_out = pin_motor_out;
    _name = name;

    output_start_ts = 0;
    output_high_speed_start_ts = 0;  //MAX_HIGH_SPEED_OUTPUT_TIME_MS
    input_start_ts = 0;
    force_output_stopped = 0;

    // after power on, set it force stopped state so it doesn't run crazy
    force_rollback_stopped = 1;

    debug_print = 0;
  };


  void init_pins() {


    pinMode(_pin_input, INPUT_PULLUP);
    pinMode(_pin_output, INPUT_PULLUP);
    pinMode(_pin_motor_in, OUTPUT);
    pinMode(_pin_motor_out, OUTPUT);

    this->debugPrint("init done " + this->_name);
  }

  void enable_debug_print(bool en) {
    this->debug_print = en;
  }

  bool has_filament() {
#ifdef IGNORE_FILA_EXIST
    return 1;
#else
    return digitalRead(_pin_exist);
#endif
  }


  void process_neutral() {


    unsigned long int now = millis();
    force_output_stopped = 0;
    force_rollback_stopped = 0;
    // not input triggered, not output triggered, nothing triggered, neutral position
    if (output_start_ts) {
      unsigned long int time_passed = now - output_start_ts;
      //debugPrint("process_neutral already running slow ");

      // already outputting, let it output for min time
      if (time_passed < MIN_OUTPUT_TIME_MS) {
        //debugPrint("motor already running slow, now nuetral, don't stop:time NOT reaches MIN_OUTPUT_TIME_MS " + String(MIN_OUTPUT_TIME_MS) + "ms ");
        return;
      }else{
        debugPrint("motor already running slow, now nuetral, passed min output time MIN_OUTPUT_TIME_MS " + String(time_passed)+">="+ String(MIN_OUTPUT_TIME_MS) + "ms ");
        stop_motor();
        return;
      }
    }else if(input_start_ts)
    {
        debugPrint("motor was inputting, now stop!");
        stop_motor();
    }

    //debugPrint("");
    //stop_motor();
  }




  void process_output() {

    unsigned long int now = millis();
    if (!is_output_triggered()) {
      return;
    }

    if (force_output_stopped) {
      static bool printed = false;
      if (!printed)
      {
         this->debugPrint("motor output force stopped, so ignore the output trigger. ");
         printed = true;
      }
      return;
    }


    if (!is_motor_outputting()) {

      // was idle, now need to output
      debugPrint("starting motor for output ");
      motor_output_slow();
      // let it output at least....some time
      //delay(200);
      debugPrint("start motor delay done ");

      return;
    }


    // motor is already running for output

    // already outputting!
    if (output_start_ts) {
      //debugPrint("motor for output is already running slow ");

      unsigned long slow_output_time_passed = now - output_start_ts;
      if (slow_output_time_passed > OUTPUT_SPEED_UP_TIME) {
        debugPrint("output need speed up!! ");
        motor_output_fast();

      } else if (slow_output_time_passed < MIN_OUTPUT_TIME_MS) {
        // do nothing, let it output for at least the min output time
      }

    } else if (output_high_speed_start_ts) {
      unsigned long fast_output_time_passed = now - output_high_speed_start_ts;
      debugPrint("motor running high speed! ");

      if (fast_output_time_passed > MAX_HIGH_SPEED_OUTPUT_TIME_MS) {
        debugPrint("high speed output too long, stop! ");
        force_output_stopped = 1;
        stop_motor();
      }
    }
  }

  bool is_motor_stopped() {
    if (output_start_ts == 0 && output_high_speed_start_ts==0 && input_start_ts==0) {
      return true;
    } else {
      return false;
    }
  }

  void stop_motor() {


    if (!is_motor_stopped()) {
      // don't print too much
      debugPrint("motor controller STOP motor! ");
    }


    if (MOTOR_EN_IS_HIGH) {
      //Serial.println("MOTOR_EN_IS_HIGH: stop motor pwm is 0 " + _name);

      analogWrite(_pin_motor_out, 0);
      digitalWrite(_pin_motor_out, LOW);

      digitalWrite(_pin_motor_in, LOW);
    } else {
      // en is low, so use HIGH to stop to
      //Serial.println("MOTOR_EN_IS_LOW: stop motor pwm is 255 " + _name);
      //analogWrite(_pin_motor_out, 255);
      digitalWrite(_pin_motor_out, HIGH);
      digitalWrite(_pin_motor_in, HIGH);
    }
    output_start_ts = 0;
    input_start_ts = 0;
    output_high_speed_start_ts = 0;
  }

  // motor is already running for output
  bool is_motor_outputting() {
    return output_high_speed_start_ts || output_start_ts;
  }

  bool is_motor_inputting() {
    return input_start_ts;
  }



  bool is_output_triggered() {
    return digitalRead(_pin_output);
  }

  bool is_input_triggered() {
    return digitalRead(_pin_input);
  }


  void motor_roll_back() {
    input_start_ts = millis();
    output_start_ts = 0;

    output_high_speed_start_ts = 0;

    if (MOTOR_EN_IS_HIGH) {
      analogWrite(_pin_motor_in, INPUT_SPEED_PWM);
    } else {
      analogWrite(_pin_motor_in, MAX_PWM - INPUT_SPEED_PWM);
    }
  }

  void motor_output_fast() {
    input_start_ts = 0;
    output_start_ts = 0;
    output_high_speed_start_ts = millis();
    uint8_t pwm;
    if (MOTOR_EN_IS_HIGH) {
      pwm = OUTPUT_FULL_SPEED_PWM;
    } else {
      pwm = MAX_PWM - OUTPUT_FULL_SPEED_PWM;
    }
    analogWrite(_pin_motor_out, pwm);
  }

  void motor_output_slow() {
    output_start_ts = millis();
    input_start_ts = 0;
    output_high_speed_start_ts = 0;
    uint8_t pwm;

    if (MOTOR_EN_IS_HIGH) {
      pwm = OUTPUT_SLOW_SPEED_PWM;
    } else {
      pwm = MAX_PWM - OUTPUT_SLOW_SPEED_PWM;
    }
    debugPrint("slow output pwm is " + String(pwm));

    analogWrite(_pin_motor_out, pwm);
  }



  void process_input() {
    unsigned long int now = millis();

    // input , roll back
    if (!is_input_triggered()) {
      return;
    }
    if (force_rollback_stopped) {
      static bool printed=false;
      if (!printed)
      {
         this->debugPrint("input is triggered, but forced input stopped, ignore the input trigger. return " + _name);
         printed = true;
      }
      return;
    }

    if (is_motor_inputting()) {
      unsigned long int time_passed = now - input_start_ts;

      Serial.println("motor input start_ts is " + String(input_start_ts) + " time_passed is " + String(time_passed) + " MAX_INPUT_TIME_MS:" + String(MAX_INPUT_TIME_MS) + " " + _name);
      if (time_passed > MAX_INPUT_TIME_MS) {
        // inputting for too long, even it's still triggered, stop the motor
        debugPrint("motor input too long,  stop! ");
        force_rollback_stopped = 1;
        stop_motor();
      }

    } else {
      debugPrint("motor roll back started ");

      motor_roll_back();
    }

    return;
  }

  // put this in arduino main loop
  void do_work() {

    if (!this->has_filament()) {

      stop_motor();
      //Serial.println("no filament " + _name);


    } else {
      //Serial.println("yes filament " + _name);

      this->process_output();
      this->process_input();
      if (!this->is_input_triggered() && !this->is_output_triggered()) {
        this->process_neutral();
      }
    };
  }
};


Motor_Controller controller_a(INPUT_SENSOR_PIN_A, OUTPUT_SENSOR_PIN_A,
                              MOTOR_INPUT_EN_PIN_A, MOTOR_OUTPUT_EN_PIN_A, "motor A");

Motor_Controller controller_b(INPUT_SENSOR_PIN_B, OUTPUT_SENSOR_PIN_B,
                              MOTOR_INPUT_EN_PIN_B, MOTOR_OUTPUT_EN_PIN_B, "motor B");

Motor_Controller controller_c(INPUT_SENSOR_PIN_C, OUTPUT_SENSOR_PIN_C,
                              MOTOR_INPUT_EN_PIN_C, MOTOR_OUTPUT_EN_PIN_C, "motor C");



void test_pwm() {
  int i = 10;
  for (i = 10; i < 255; i += 2) {
    Serial.println("pwm is " + String(i));

    analogWrite(MOTOR_OUTPUT_EN_PIN_A, i);

    delay(200);
  }

  static int en = HIGH;
  en = !en;
  Serial.println("enable is " + String(en));
  digitalWrite(MOTOR_OUTPUT_EN_PIN_A, en);
  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
#ifdef DebugPrint
  Serial.begin(SerialSpeed);
  Serial.println("motor controller started");
#endif


  controller_a.init_pins();
//  controller_a.enable_debug_print(1);

  controller_b.init_pins();
//  controller_b.enable_debug_print(1);

  controller_c.init_pins();

#ifdef DebugPrint
  controller_a.enable_debug_print(1);
  //controller_c.enable_debug_print(1);
#endif


// debug only
//   pinMode(11, OUTPUT);
      //pinMode(10, OUTPUT);
      //analogWrite(10, 0);
}


void loop() {

  controller_a.do_work();
  controller_b.do_work();
  controller_c.do_work();



//   analogWrite(11, 254);
 

}
