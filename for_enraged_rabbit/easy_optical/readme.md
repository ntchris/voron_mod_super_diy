# ERCF Easy Optical Encoder Module
For ERCF: to make it easier for everyone to use a transmissive optical sensor

in one simple sentence, a DIY friendly, a much cheaper and easier blinky sensor module for ERCF.


![ercf easy optical](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/easy_optical/3d_view2.jpg)

## What is this:

for ERCF (Enraged Rabbit Carrot Feeder Multiple Material System for Voron or alike 3D printers)
replace the original reflective optical sensor which is quite not reliable.
so mneuhaus made a transmissive optical sensor encoder called Binky, which should provide a much more reliable encoder functions/counter.
 https://github.com/mneuhaus/EnragedRabbitProject/tree/main/usermods/Binky
 
* What is a transmissive optical sensor
an optical sensor, or photo sensor, slot type, like SX1103, or ITR9608.
It has a infrared diode and a receiver, if an object blocks the infrared light, the receiver detects it and triggers/send a signal.

However, there are still some problems remains with Binky module, 
* blinky PCB is quite expensive, plus shipping (international!) and tax, I'm afraid we are looking at ~~30$ or more. 
* Also the PCB is quite difficult to make via DIY methods. 
* And some components, in my opinion, is too expensive or useless.
So I made this Easy Optical module for ERCF.
The goal is to save a ton of money not to buy binky PCB or wait for shipping, or buy some expensive and /or unncessary components. 
and we can make it using some easy home DIY methods, CNC milling or chemical etching.

## Easy Optical PCB module feature list:
* single side PCB design, reduce PCB DIY complicity
* 0.5mm thick traces and large clearance, good precision/error tolerance, so we can easily make it at home with a cheap CNC or chemical method. I made mine using a DIY CNC made by myself.
* Use a much cheaper slotted optical sensor - ITR20403, it's small enough and very cheap, about 2$ for 10pcs. 
https://everlighteurope.com/index.php?controller=attachment?id_attachment=5394 comparing with the SX1103, it's about 1$ each.
* removed a useless component small inverter, to reduce cost and efforts for making the pcb/soldering.
* using a 3mm through hole LED, it fits the rabbit eyes much better and brighter.
* Kicad 7 is used to create the schematics and PCB. full schematics/PCB project/source are provided

![ercf easy optical](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/easy_optical/3d_view1.jpg)

![ercf easy optical pcb a](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/easy_optical/pcb_3D_a.jpg)

![ercf easy optical pcb a](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/easy_optical/pcb_3D_b.jpg)


## Things to pay attention while making the PCB at home.
* Only front side of the PCB is needed, back side is not needed, just optional.
* for single side PCB, the 3pins connector must be soldered on the front side of the PCB, because the back side has no copper. 
* For single side PCB, the 3pins connect drill hole better be 0.8mm, so it's tight enough. If the holes are too large, the pin will be easily pulled out since the copper is not strong enough to hold it against some large pull up force.
* If don't care about flashing rabbit eyes, some components are not needed, so to make it even easier to make: R3 (D1 3mm LED, R3 1k)for LED, Q1 ( AO3402 Nchannel Mosfet), C2 (10u)
* when installing the ITR20403, the 45 degree edge is facing outside of PCB, longer pin is facing 3mm led. there are two plastic mounting pin under it, one is larger, it's closer to the 3mm led. hold it tight and close on the PCB when solder.


## Component list
* C1	1	1u	C      0805 SMD Capacitor, filtering signal.
* C2	1	10u	C      0805 SMD Capacitor, optional
* D1	1	LED	LED    3mm red LED, A on right, K on left. optional
* H1	1	MountingHole	2mm mounting hole.
* J1	1	Conn_01x03	Conn_01x03,  
* Q1	1	AO3402_N_Mosfet	, Nchannel mosfet, for red LED only, optional.
* R1    1   1K  R   0805 SMD Resistor,  for optical sensor infrared
* R3	1	1k	R   0805 SMD Resistor    Optional
* R2	1	10k	R      0805 SMD Resistor , pull up for optical sensor
* R4	1	0	R     1206 0R resistor, act as a jumper wire, if don't have it, just fly a wire.
* U1	1	ITR20403  optical sensor

## Operation theory
![easy optical schematics](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/easy_optical/schematics.jpg)

It couldn't be simpler and easier than this.
* When the ITR20403 sensor is power on, R1 (1K) resistor make 5V power supply to provide and reduce a suitable current for the sensor's infrared led, about (5-2)/1k = ~~3ma.
* When nothing blocking, the infrared light fire on the other side collector/emitter, so it's kind of connected, so our signal termial has about 0V, low.
* When our 3D printed gear blocking the infrared, the E/C is disconnected, so the signal point is pull high.
So our easy optical module can send low/high signal as the gear rotates, and it's super reliable, since the gear can always block the light and let the light pass.

* Optional for blinking rabbit eyes.
 N channel mosfet gate when pull high, it power on the 3mm LED.

( low or high doesn't matter in our application, as long as it can be triggered and change, it's good enough. also this is not a high speed application. )


## Encoder 3DPrint part 
The 3D pirnted part is modified based on binky's encoder however made quite a few changes as below:

### modification list:

1. larger cut for optical sensor, so it fits IT20403  ( 7mm for sensor 6.4mm width, 4.7mm for 4.2mm length.), it's slightly larger than the sx1103 (in binky pcb), but much much cheaper than SX1103. ITR20403 is only a bit wider than it (6.4mm VS 5mm).
1. larger LED hole so it fits for through hole 3mm led, and the led's bottom is even larger ( I think it's the smallest through hole cheap led in the market)
1. very very slightly larger (0.02mm) slot wheel, and tighter fit on bondtech gear. So it won't easily move on the bondtech gear. also it can block the light better.
1. 0.2mm lower pcb mount position so to let slot wheel fit deeper/better in the optical sensor
1. larger gap in the brake, so it doesn't get stuck with main body
1. thicker gear slot wheel (1.2mm now), so it can block light better.
  I don't think color matters, as long as it's not translucent. (I used grey color to print that on purpose to prove that. )

## Installation
* follow original ERCF's guide page 49, need 3 heat inserts, one bearing 623, a few 20mm M3 screws, 2 magnets 3x6mm
* it's better to make a new working encoder module first, and not to destroy the existing/working encoder, if you already have one running well, and if you have all the necessary parts, especially the bondtech gear...
* check 3 pins connector polaries before connect,  G V S, first one is Gnd, middle one is 5V, Sigal at right most. 
* modify ercf.py file ( in PI /home/yourname/ERCF-Software-V3/extras , about line 139 DEFAULT_ENCODER_RESOLUTION = 1.0   , as stated in binky's readme
* put the 3mm red led into the 3d printed part hole.
* use a 2mm screw to mount the PCB.
* add some suitable oil inside the bondtech gear idler.

## check again ercf is restored in proper working state after new encoder installation,  check points and potential problems are:  
* 2 X latches are locked again
* servo can move do up and down correctly, if it cannot move down correctly, it cannot push the bondtech gear to hold the filament tight, so filament cannot move correctly, making sensor cannot detect movement.
* selector can move freely. turn motor off ( ercf_motors_off), move selector by hand to feel if there is any resistance, if so, use a file tool to fix it.
* some filament can be grinded and wear out so bondtech gear cannot catch it well, observe, see if filament are actually moving out and in to the correct distance when running the calibrate command, we can easily tell if this problem happens. (It happened to my ABS, too soft and easily grinded out)

## Testing
* insert a filament in encoder, while pulling it in and out, check rabbit eyes are blinking , (only if all optional LED components are installed /soldered)
* use gcode command ERCF_CALIBRATE_ENCODER for a few times, it should always have a result of 2.0#, it shouldn't be too much difference than 2.0 something. If not, need to investigate.

![ercf test log](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/easy_optical/ercf_log.jpg)

