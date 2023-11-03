# ERCF Hardware and Software Config and Setting Notes for BTT BigTreeTech ERCF CAN BUS board


BTT calls this board 
"BIGTREETECH MMB CAN V1.0 Board Enraged Rabbit Carrot Feeder ERCF Control Board CAN Bus" 
btt_ercf_canbus.jpg
![BTT ercf](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/btt_ERCF_Canbus/btt_ercf_canbus.jpg)

## What is it
I assume you all know what is this and what this for?
If not, this is a expansion board for Voron printer or alike to let it work with Enraged Rabbit multi material system (ERCF)
Similar product in market is Mellow's CanBus ERCF board.
Fysetc's ERCF board only supports USB connection ( unless now they have newer product to support CanBus, I didn't check)

## How good, why good
What's special about this BTT ercf board is, and what make me buy it:
* It use BTT's EZ stepper motor driver, which is like a mini PCIE graphic card, looks cool
* It supports CanBus for ERCF, which makes wiring easier.
* It has a lot of expansion socket, 4 motor slots (ERCF only need 2), and tons of gpio. potentially we can make some new crazy features.


## Product and accessories purchase recommendation
* obviously you need the BTT ERCF Canbus main board, it contains all necessary power/canbus cable and jumpers
* Stepper motor driver, I suggest EZ2209 driver,  2 or more, it's cheap and powerful, supports sensorless homing (mandatory for ERCF in my opinion)
* EZ Driver to regular stepper dirver adapters, so that we can use the EZ driver on other boards.


## Setup Steps
This board uses STM32, not rp2040.
can refer to https://github.com/bigtreetech/MMB to get more info but a lot of things are missing in their own guide

### flash CanBoot bootloader

* Prepare a RaspberryPi mini computer. If we don't want to mess with the running voron/kliper printer, or cannot easily connect a USB cable to the working Pi4 under the heavy Voron printer, we can use any other random Raspberry Pi, or, even use Windows10, Windows10 can run a Ubuntu20 as alternative OS, we can do that in the Ubuntu envronment inside a Windows10 system.
* ssh login the Pi or in that Windows/Ubuntu environment, setup CanBoot git repo. run this
```
git clone https://github.com/Arksine/CanBoot
cd CanBoot
make menuconfig
```
choose below options
* micro controller -- STM32
* Processor -- STM32G0B1 ( they may change it in future, can read it from the actual IC on board)
* Build Kakapult -- Do Not Build
* Clock --  8MHz
* Comm Interface -- CanBus on PB0/PB1
* App Start offset -- 8KB offset
CanBus speed -- 1000000 (six zero, this is my speed, some people use 500000(5 zero).  It must be the same exact speed as the Pi4 in Voron printer/Tool Head CAN bus is running, must must be the same. )
* Support bootload entry....
Press Q and Save

![canboot make menu](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/btt_ERCF_Canbus/canboot_makemenu.jpg)


run make to build the CanBoot firmware.
the CanBoot firmware file shall be built and put inside home/pi(or your user name)/CanBoot/out/canboot.bin
* Prepare the dfu-util tool, we need that to flash STM32 chip. 
```
sudo apt install dfu-util
```

* Check the BTT ERCF board, remove the Jumper on VUSB (near the 24V power socket), don't connect anything else on the BTT board yet. No power cable is needed yet.
* Connect a USB-C cable to BTT ERCF and to PI( or the PC above)
Since we just removed the power jumper VUSB, therefore the whole BTT ERCF is not powred on. Now we will put the STM32 in DFU flash mode,
hold the 2nd tiny button (BOOT), connect the VUSB jumper (while holding BOOT), we will see the BTT ERCF is now powre on. Release finger for Boot button.
* Find out if it's indeed in DFU/flash mode and get it's ID, run lsusb command. we will see something like this
 ```
 lsusb
 DEVICE=0483:df11 STM device in DFU mode.
 ```
 if we don't see something like this, must redo above step: remove VUSB Jumper to power off the board, hold BOOT button and connect Jumper again.
* In BTT's manual it says now it's time to run make flash, but I have error for make flash. so we can use dfu-util to flash.
* I think most people's ID would be the same? is it ? is it not ?  Run below command to flash.
```
  sudo dfu-util -a 0 -D ~/CanBoot/out/canboot.bin --dfuse-address 0x08000000:force:mass-erase:leave -d 0483:df11
```
Check the output, it should flash the STM32 with CanBoot. If see there is such error in last line, just ignore.
```
  dfu-util: Error during download get_status
```

Remove USB cable, remove the VUSB Jumper
Now  the BTT ERCF has CanBoot, we can connect it to the Voron/Pi4 in next step.

### Connect BTT ERCF to Voron using CanBus
1. For all 4 stepper motor driver slots, there is a Vin or HiV choose jumper, I believe most people just use Vin(standard 24V), so connect a jumper on left (VIN)-Middle to use 24V for EACH driver slot. no need to connect the EZ driver board yet.
1. There is a 120R resistor jumper on BTT ERCF CanBus, I believe most people shouldn't need it, so don't connect the jumper. The reason is, you probably don't try the CanBus on ERCF if you have NOT setup CANBus for printer Tool Head.  In a CanBus system, only two 120R is needed, one is in mainboard/Klipper, the other should be in ToolHead already, so this ERCF must not has 120R connected. 
1. no need to connect anything else yet, just connect the BTT supplied cable for power+Can cable to supply 24V and Can lines, Can line is connected to Voron mainboard Canbus.
1. Now the BTT ERCF is powered on and connected CanBus, we should find the CAN ID now.
```
cd ~/CanBoot/scripts
~/CanBoot/scripts $ python3 flash_can.py -i can0 -q
Checking for Katapult nodes...
Detected UUID: e8d2d1c60b3f, Application: Katapult  <==== BTT CanBus ERCF
Detected UUID: 6220af9ec579, Application: Klipper
Query Complete
```
copy the can id e8d2d1c60b3f, we will use it to flash the klipper firmware later.
If don't see it, check the CAN SPEED, speed must be same in the whole system, mainboard, Pi, toolhead, ERCF. And check Can wire (High/Low wrong ??)  Only two 120R resistors shall be in the whole Can system, Not 120R in each Can node.

### Flash  BTT ERCF with Klipper firmware

1. ssh login the Voron's Raspberry Pi.
1. make klipper firmware for BTT ERCF
```
cd klipper`
make menuconfig
```
choose Enable Extra Low level
choose STM32 
choose STM32G0B1
Bootloader Offset 8KiB
Clock 8Mhz
Comm Interface CanBus PB0/PB1
1000000 CAN speed
run make, we will have klipper.bin in out dir.

![klipper make menu](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/btt_ERCF_Canbus/klipper_makemenu.jpg)

1. use CanBoot to flash the klipper firmware, replace the can id with the actual one from query output.
```
python3 flash_can.py -i can0 -f ~/klipper/out/klipper.bin -u e8d2d1c60b3f
Sending bootloader jump command...
Resetting all bootloader node IDs...
Checking for Katapult nodes...
Detected UUID: e8d2d1c60b3f, Application: Katapult  <==== BTT CanBus ERCF
Detected UUID: 6220af9ec579, Application: Klipper   
...
Application Start: 0x8002000
MCU type: stm32g0b1xx
Verifying canbus connection
Flashing '/home/chris/klipper/out/klipper.bin'
...
Flash Success
```

1. After flash, run query again, it shall show klipper for the Can Id.
~/CanBoot/scripts $ python3 flash_can.py -i can0 -q
Resetting all bootloader node IDs...
Checking for Katapult nodes...
Detected UUID: e8d2d1c60b3f, Application: Klipper   <==== BTT CanBus ERCF
Detected UUID: 6220af9xxxxx, Application: Klipper
Query Complete

Klipper is now read in the BTT ERCF board.
Next step is to config software config for ERCF klipper.


### BTT ERCF jumpers
Power off the BTT ERCF, unplug the power + Can cable.
1. check again VIN jumper for each motor is installed,
2. I choose 2nd motor slot to work as Selector Stepper so connect the jumper for Diag2. It will do sensorless selector moving.

### for ERCF we sure will use happy hare V3. 
https://github.com/ntchris/ERCF-Software-V3
this is my fork from the original https://github.com/moggieuk/ERCF-Software-V3
I added quite a few new features I believe very useful.

### BTT ERCF software config file

BTT supplied a sample-bigtreetech-mmb-canbus.cfg file, this is useless in our ERCF config.
we can refer to bigtreetech-mmb-ercf.cfg if needed.
we can modify from the bigtreetech-mmb-ercf.cfg as a starting point.
Modify the main Klipper config file printer.cfg (in Voron Pi4 ~/printer_data/config/ )
```
nano ~/printer_data/config/printer.cfg
[include btt_mmb_ercf_canbus.cfg]
```

#### set can id in the config file
use the CAN ID we copied from previous step, set it in the config file
```
[mcu ercf]
canbus_uuid=e8d2d1c60b3f
```
#### Stepper motor
![motor pin btt ercf](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/btt_ERCF_Canbus/motor_pins.jpg))

1. btt ercf has 4 stepper motor slots, each one can work as selector motor or gear_stepper motor. I choose 2nd as selector and 4th as gear stepper.
1. check motor pins fo pin names. 
1. gear motor part 1(4th slot)
CS pin is the uart pin.
```
[tmc2209 manual_extruder_stepper gear_stepper]
# 4th motor slot
uart_pin: ercf:PA9  # CS pin
# uart_address: 0  is useless
interpolate: True
run_current: 0.56			# NEMA14 motor
hold_current: 0.1			# Can be small
sense_resistor: 0.110
stealthchop_threshold: 0	# 999999 to enable stealthChop
```
we can comment out every driver_#### items unless you know what they do.
1. gear motor part 2 (still, the same 4th slot)
```
[manual_extruder_stepper gear_stepper]
# using motor 4 (4th)
step_pin: ercf:PB6
dir_pin: !ercf:PB7   # use or remove the !
enable_pin: !ercf:PB5
rotation_distance: 22.7316868		# Bondtech 5mm Drive Gears. Must be manually verified and tuned.
gear_ratio: 80:20
microsteps: 8 				# Recommend 8 or 16. Try both to avoid Klipper 'Timer too close' error (also try adjusting num_moves)
full_steps_per_rotation: 200		# 200 for 1.8 degree, 400 for 0.9 degree
velocity: 50
accel: 400
endstop_pin: ercf:PA3	# mandatory for sensorless selector, PA3 is STP1 in the BTT ERCF.

```
For dir pin, it depends on the motor wires, there is no guarentee everyone's motor has the exact same wiring, so the dir can be opposite, If later we find that the rotation dir is wrong, we can easily change it by adding or removing the !  symbole in the dir_pin.
If later we find the motor is hot but no moving, check the enable_pin, make sure there is a !.

1. selector motor part 1
it's the 2nd motor slot I choose to use.
Use CS pin for Uart pin, PC7.
Use below run and hold current for sensorless selector.
```
[tmc2209 manual_stepper selector_stepper]
# using Motor slot 2 (2nd)
uart_pin: ercf:PC7
# uart_address: 1  # useless
run_current: 0.55		# NEMA14 motor
hold_current: 0.1		# Can be small if not using sensorless selector
interpolate: True
sense_resistor: 0.110
stealthchop_threshold: 0	# 999999 to enable stealthChop
diag_pin: ercf:PA4		# Set to MCU pin connected to TMC DIAG pin, Jumper.  pull up is optional, not mandatory
driver_SGTHRS: 80		#  YMMV 255 is most sensitive value, 0 is least sensitive
```
1. selector motor part 2
```
[manual_stepper selector_stepper]
step_pin: ercf:PD2
dir_pin: ercf:PB13
enable_pin: !ercf:PD1
rotation_distance: 40
microsteps: 8 			# Don't need high fidelity
full_steps_per_rotation: 200	# 200 for 1.8 degree, 400 for 0.9 degree
velocity: 200
accel: 600

endstop_pin: tmc2209_selector_stepper:virtual_endstop	# Uncomment for sensorless selector homing
```

#### ERCF Servo
PA0 is the servo signal pin, in BTT's connector/diagram it says "MOT"
```
# SELECTOR SERVO -----------------------------------------------------------------------------------------------------------
[ercf_servo ercf_servo]
pin: ercf:PA0
```

#### selector end stop pin
Even thought we use sensorless homing for selector movement but we still use a endstop, so that when selector couldn't move, we can know if it's a true home, or it's a filament blockage.
We have added those in above gear stepper and selector definition.
so double check to make sure they are all correct.
```
in [manual_extruder_stepper gear_stepper]
endstop_pin: ercf:PA3	# mandatory for sensorless selector.  This is in gear_stepper section.

in [manual_stepper selector_stepper]
endstop_pin: tmc2209_selector_stepper:virtual_endstop
```

#### Encoder Pin
encoder is the optical device to help ERCF to findout if filament is moving or not.
encoder pin is PA1.
It's the 2nd vertical socket, has name "sensor" on the PCB.
```
[ercf_encoder ercf_encoder]
encoder_pin: ercf:PA1			# BTT CanBus  PA1 ,  Mellow Canbus ^ercf:gpio15
encoder_resolution: 1.341082
extruder: extruder		# The extruder to track with for runout/clog detection
# These are advanced but settings for Automatic clog detection mode. Make sure you understand or ask questions on Discord
desired_headroom: 15.0		# The runout headroom that ERCF will attempt to maintain (closest ERCF comes to triggering runout)
average_samples: 5		# The "damping" effect of last measurement. Higher value means clog_length will be reduced more slowly
```

now the hardware config file is fully ready.
upload this file to Voron/PI, restart firmware, it should apply.

### ERCF connection
Still, power off the ERCF, now power cable at all while connecting things.
1. use some thick paper/tissue to hold the EZ driver board, straight edge facing up, a 45Degree bump edge (with a screw) facing down to the slot ( the slot has a cut out to hold that small 45Degree bump edge). Push it very hard in to the EZ driver slot. We use paper to hold so fingers won't get hurt by the PCB/glass fiber.
1. double check driver's voltage jumper, VIN, diag2  (or other slot's Diag pin)
1. install two EZ driver to 2nd and 4th slot. (If you choose to use other slot, must change all the driver pins in the config file)
1. connect servo connector, it's the 1st vertial socket, check the servo wire/pin definition, order is 5V/GND/Signal...basically all the connectors in BTT ERCF have the same order (except RGB, since it's not covered by this note, ignore it for now)
1. conenct optical sensor to "Sensor" port. 5/G/S.
1. connect endstop, We can choose the STP1 ( PA3 pin) connector.
if using a standard mechanical microswitch, only connect GND and GPIO. If using a optical endstop like mine
https://www.printables.com/model/598672-ercf-enraged-rabbit-button-style-optical-endstop-f
https://www.printables.com/model/596286-ercf-enraged-rabbit-optical-endstop-for-selector
connect all 5V G Signal.
All connection is now finished.

### Power on Voron and Run some ERCF test
1. Connect the supplied power cable with Can.
1. No USB cable is needed.
Do a ERCF home and other motor and sensor action, we can verify each item.
1. run servo up and down command to test servo  ERCF_SERVO_UP
1. while servo is up, pull/run a filament in ERCF, optical sensor or Rabbit's eyes should flash, it means the sensor connection is good.
1. test endstop
```
Go to mainsail -> machine -> Endstop query
when selector not touching endstop
below two should be OPEN:
Endstop MANUAL_EXTRUDER_STEPPER GEAR_STEPPER OPEN
Endstop MANUAL_STEPPER SELECTOR_STEPPER OPEN

When selector pushing endstop, gear stepper should be triggered
(that's right, HappyHare use the endstop inside gear stepper, not selector stepper):
Endstop MANUAL_EXTRUDER_STEPPER GEAR_STEPPER Triggered
Endstop MANUAL_STEPPER SELECTOR_STEPPER OPEN
```


### Testing and Trouble shooting for all possible problems:

#### selector cannot move or can only move a little bit.

Use ERCF_home and ERCF__SELECT_TOOL TOOL=5 to test selector.
If selector cannot move and have red error inside mainsail
> An issue with the ERCF has been detected...Selector recovery failed. Path is probably internally blocked and unable to move filament to clear

Solution is to set the current higher (ie 0.55, 0.6 ) and driver_SGTHRS lower (ie 60, 70, 80)
If current is too low and driver is too sensitive, it cannot move because the resistance/friction is too high.
Warning, absolutely don't set the current value too high ( 1.0?? 1.2?? ) , it may damage the TMC2209 driver, and causes a horrible and expensive "Voltage too low" error.



#### Mainsail cannot boot, firmware restart / restart error in webpage.

Probably the CAN wire is not properly connected in ERCF board. 
Consider replug in /plug in hard the 24V/CAN connector in the mellow ERCF board.

#### when move, selector or gear motor goes to wrong direction - left / right / forward /backward
depends on the stepper motor wiring, the direction could be reverted.
can be fixed by reverting the dir pin by adding or removing !
```
dir_pin: !ercf:
dir_pin: ercf:
```
#### selector cannot move and have red error inside mainsail
> An issue with the ERCF has been detected...Selector recovery failed. Path is probably internally blocked and unable to move filament to clear

Solution: Must set the current higher (ie 0.55, 0.6 ) and driver_SGTHRS lower (ie 60, 70, 80)
If current is too low and driver is too sensitive, it cannot move because the resistance/friction is too high. 
It thinks it hits the end wall.

#### stepper motor (selector or gear ) does nothing but very hot.
Check the stepper motor enable pin in ercf hardware config file, may need to remove the ! (revert sign)
With the wrong revert sign, when doing nothing, the motor is enabled/coil(s) is(are) powered on. so it gets hot.

#### when use selector sensorless home, when it home and hit the end, always or very high chance has a TIME TOO CLSOE red error.
check manual_stepper selector_stepper in hardware config file, change microsteps to 8 from 16.
or, use a spacer on each of the M8 rod for selector rail, to adjust when it hits the wall.
The timer error seems to be caused by the hit wall timing and endstop switch trigger time.
I did not have the time too close error before I added two spacers(1mm) in the end of the M8 rod,
but after add (to make a solid wall), there is such error.
and the thickness is also important, when I use 0.8mm, no error, 1mm has error, so I have to use step=8 to get rid of the error.
I guess micro step=8 should be good enough for the selector motor accuracy...

#### ERCF has too large of a delta value when home (after home)
After home, selector will move from zero position to T0. after it's done, ERCF will check the attempt travel and actual travel distance by some means. (Planned travel is T0 position , "actual" travel is from MCU. This is not a close loop system)
If see this error sometimes:
>An issue with the ERCF has been detected whilst out of a print
Reason: Selector recovery failed. Path is probably internally blocked and unable to move filament to clear
> selector move delta is too large, most likely it's because selector driver_SGTHRS is too large (too sensitive) 2.5 > 1.1  ( this log is only available in my branch only)

It's because the selector motor drive has a too large driver_SGTHRS value(too sensitive). so most likely it stopped too early due to resistance, so the solution try to reduce the value (less sensitive).
In my setup and test, driver_SGTHRS = 86 causes sometimes 1.6 to 2.5mm delta. (Only allow 1mm)
After reduce driver_SGTHRS to 78, the delta gets smaller like 0.6mm.
However if we set the value too small, the selector hits the wall harder and a larger noise...so it need to be balanced.
Recommend value is 60-78.
YMMV, it all depends on your current setting, and how good are the 8mm rail smooth rod and your linear bearing selector accuracy etc.

## Other reference
BTT ERCF
https://github.com/bigtreetech/MMB/tree/master

CanBoot
https://github.com/Arksine/katapult

My branch of ERCF happy hare software 
https://github.com/ntchris/ERCF-Software-V3

original happyhare ERCF software
https://github.com/moggieuk/ERCF-Software-V3


how-to-use-can-toolhead-boards-connected-directly-to-octopus-octopus-pro-on-canboot
https://www.teamfdm.com/forums/topic/672-how-to-use-can-toolhead-boards-connected-directly-to-octopus-octopus-pro-on-canboot/

voron_canbus_octopus_sb2040
https://github.com/akhamar/voron_canbus_octopus_sb2040

optical light sensor endstop
button style
https://www.printables.com/model/598672-ercf-enraged-rabbit-button-style-optical-endstop-f

light blocker style
https://www.printables.com/model/596286-ercf-enraged-rabbit-optical-endstop-for-selector

