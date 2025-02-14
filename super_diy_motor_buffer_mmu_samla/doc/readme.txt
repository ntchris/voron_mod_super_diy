What is this

This is a buffer system for different multi material systems. It uses motor, to automatically do both filament rewind and release the filament from spool.  

It works for different Voron printers (Trident, V2.4 etc) with ERCF and Prusa MK3(S) and MK4(S) with MMU3.

Also it's designed to put 3 units in IKEA Samla Box (22L) to keep filament from room air, dry,  and with potential silica gel box within, we can make filament even drier.

No electronics signal is exchanged between this motor buffer system and the multi material system (PRUSA MMU3 or ERCF etc), therefore, it should be compatible with most of those.

Main Functions:

This is a motor buffer system to works with Engraged Rabbit Carrot Feeder (ERCF V2, V1), PRUSA MMU3 (or MMU2), TradRack, 3D Chameleon, SMUFF etc, any multi material system without a box or container.
This system by itself is NOT a multi material system. It's just a motorized buffer.
Help to easily store and manage filaments using IKEA Samla box (22L) and use them (printing) at the same time.



Benefits of this motor buffer System:

No buffer wheels, saving time and effort when changing filament. Routing filament multiple rounds on the buffer wheel is not very easy and require some time to do.  Loading and changing different filament spools is much easier and straight forward.
No buffer wheel, much easier for MMUs to pull filament from the buffer system, since buffer wheel creates certain resistance/friction. With this motor buffer system, motor help to rotate the filament spool to release and loose the filament, this makes mmu very easy to pull filament out of the spool.
Rewind released filament (from MMU or printer) back into the spool
Very low cost for hardware/electrical parts. a few bearing and motor, Arduino, driver board, magnet, screws. Around 7$ one unit and 3 units in a Samla Box (22L). (YMMV)
Wide compatibility, because no electronic signal is exchanged between mmu and this buffer system, no connection between them, it works with most mmu in the open source area or market. This buffer system can sense the filament movement and automatically release or rewind filament accordingly, and stop doing so when it should.   It works with Enraged Rabbit Carrot Feeder (ERCF) , PRUSA MMU3, Trad Rack SMUFF and so on. 

Not Compatible list

Not working with Bambu AMS, Creality CFS , or not needed. They have their own filament rotating management system and box/container.
Not working with any open source or close source multi material system with its own filament container/rotation system.
Single filament printer system, there is not much value using this motor buffer without a multi material system, you still can use it, it should help to rewind the filament.

How it works

This motor buffer system has a sensor can sense filament movement in two directions.
When printer /mmu unload, filament tries to get in buffer system, it triggers the filament movement sensor, motor is turned on and rotate the roller, spool rotates and rewind the filament. After filament rewind enough, the sensor is not triggered and motor is turned off.
When filament is being pulled out of the buffer system (slowly printing, or higher speed initial loading), the filament movement sensor is triggered in another position, motor is turned on and rotate the spool to release filament, it makes the MMU system very easy to pull the filament, MMU small gear no need to rotate the heavy spool.
The motor buffer system also uses a Arduino module to help to better manage the motor turn on/off time, and to add a few different necessary safety features….ie even if the filament is stuck/sensor stuck, the motor won't get crazy and keep releasing the whole spool.



BOM, parts needed

For one unit (runs one filament spool):

N30 motor with gear, 12V, 666RPM,  X 1 PCS
Bearing 686ZZ,  size 6X13X5( ID 6mm, OD 13mm, thick 5mm),  X 4Pcs
Optical endstop sensor (Infrared sensor),  X 2pcs.  About 90 cents each



Arduino Nano ( small arduino board). X 1Pcs (One arduino can drive 3units in a Samla box). I think other MCU module works too. 
L9110S Motor Driver Controller Board. X 1Pcs ( one board can drive 3 motors or 3 units in one Samla box)
Small PCB with holes prototype board. X1 PCS ( good for 3 units).  There are different types of holes pcb in the market, all should work.
DC Buck converter step down module 24V to 12V, preferred adjustable, so we can get about 11V. One for 3 units. Most 3D printer uses 24V (Voron, Prusa MK3,4 etc). Our motor need 12V.  One for 3 units. Most module works, our application doesn't need high current. About 1-2Amp is enough.
Smaller DC Buck converter step down module 12V to 5V preferred adjustable so we can get 5V. one for 3 units. for optical sensors only. Any small module should work.  One for 3 units. 
Optional , common diode like 1N4007 or 1n5819 etc, any type should work, SMD or TH, just to prevent back power. X 2 PCS
2.54mm pins header. To put on the hole PCB to connect optical sensor wires/connectors, solder free, and connect arduino Nano pins.
IKEA Samla Box 22L or 6 Gallon ( 801.029.76), size 39X28X28cm.  and top cover. One box can hold 3 buffer units or 3 spools.
DC Jack or any DC connector you choose. One for 3 units.
round magnet 6mmX3mm X 4PCS
M3 socket head machine screws 8mm X 14pcs and heat inserts.
M2.6 self tapping screws 8mm.  
M3 self tapping screw 8mm X 4.
PTFE tube ID 2mm (OD 4mm as always, in 3D printing world)
PTFE tube ID 3mm (or ID 2.5mm, but ID 3mm works better. ID 2.5mm has higher chance of stuck if filament is deformed )
ECAS04 ptfe tube connector(same used in ERCF and a lot of mods) 
nail dia 2mm (insert into PTFE to make it strong and straight) X 1PCS

For 3 units (runs 3 filament spools) with one Samla Box:

everything above X 3, but we only need one Arduino, one motor driver board, one DC jack connector set, one DC buck step down 12V and 5V, one holes PCB , one DC connector set,  and one Samla Box.   3 buffer units can share a lot of of common components.

Filament

To print this motor buffer system we need to print PLA or PETG, and TPU.

Most parts can be printed using PLA and PETG. Darker color for optical sensor light blocking part.

TPU is only for soft roller, so it can better drive spool to rotate. 

Skills and knowledge Required

very basic soldering skill is needed. need to make some 12V, 5V, GND power rail on the hole PCB, to connect some wires, pins and the DC buck converter.
Need to understand what is 24V 12V and 5V and Gnd and make correct connections.
Need to know how to deal with a Arduino and flash firmware.



Print Setting

PLA or PETG. 
print N copies for file name has XN
TPU for <main> TPU_roller_A and TPU_roller_B_X3
Darker color for <push_pull_sensor> dark_push_pull_sensor_simple.stl part.
infill 30% or up
wall/perimeters 3 or 4
External Perimeter First enable for outter and inner for better dimensional accuracy 
for example in super slicer , Print Settings, Perimeters and Shell, lower part of the page.
in Prusa Slicer, Print Settings, Layers and perimeters, lower part of the page, Check box for External Perimeter first.
No support for all parts, no brim needed
all parts are in correct orientation.

Print a small test part test_bearing_size.stl to test dimensional accuracy and print settings. Try to hand press in a bearing 686ZZ,  (size 6X13X5). The goal is to hand push the bearing in, not too hard to push it in, but also not too loose to easily get out.



Assembly

Assemble the sensor module

print all sensor parts from the sensor folder. Use dark color for the dark_push_pull_sensor_simple.stl. This part will have to block the IR from the optical endstop. In my test, black is not a must, light blue also works, red seems not so good. 
Put 1 X magent spacer (printed part) in the magnet hole first, and M6X3mm(or 2.5mm in fact) magnet X 2Pcs into the dark_push_pull_sensor_simple.stl part.  (so we can use X3 magnet if it's needed later). If it's hard, use tool to help to push magnet. 
check the through hole besides the magnet hole, it should be smooth and clean, no string or bad print filament debris left. Use a round file tool to clean it and make it smooth if needed. Insert a PTFE tube to check it can move very very freely. It's Important. Otherwise sensor will be stuck and bad things will happen.
base, check how push pull sensor part will be put together with base first. Then put two M6X3 round magnet in the hole in the middle wall, but first check magnet polarity, base and push_pull sensor much repel each other and NOT attract. If magnet is wrong, use a nail from small hole on the back to easily get magnet out. 
check the rail gap in the base, it must be clean and smooth, if not , use a file to clean it a little bit. Put the push_pull sensor in place as picture shows, insert a PTFE tube (ID2mm).  from the outer wall hole, through the sensor part, and insert into the middle wall hole. Push it hard to make sure it's secure in the middle wall. Cut the PTFE tube from the outter wall. Insert a nail (dia about 1.5 to 1.9mm should work ) into the PTFE tube to make it harder, straight. 
Check sensor is pushed to the outer wall by the magnet, away from the middle wall. Hand push the moving part, make sure it can move back and forth and bounce very very freely.
use a M2.6 X8mm tapping screw and the block.stl part to cover the nail and ptfe tube so they don't move out.
use 2 M2.6X8mm tapping screw to install the optical sensor mout input , and do the same for output. They should face each other. 
use 3 M3X8mm tapping screw to install the optical endstop sensor on the mount.  Using M3 screw is because those endstop has M3 holes.   
Use M2.6  8mm screw to install PTFE holder part onto the base part.
Check again the push pull sensor can move freely and magnet is expel it to the outter wall.  

The sensor module is finished.



Main buffer module assembly

print all parts using the correct material.
check picture and insert M3 heat insert to the locations as below 
sensor_base: use solder iron to insert 2 X M3 heat insert on each side of the part.
motor_base:  use solder iron to insert 2 X M3 heat insert on each side of the part.
driver_pcb_mount and arduino_mountX2: use solder iron to insert a M3 heat insert on each side of the part.
solder wires for motor. if motor connector is too tiny too weak, use hot glue to glue wire so it wont easily break.
use 2X M2.6 screw to lock N30 motor in place. put the small gear in motor, check gear direction. use M3X8mm screw to lock motor module on motor base.   
frame A and frame B and motor base and sensor base and driver_pcb_mount  (in middle). Use M3X8mm screw to lock them together. For UnitA we use driver_pcb_mount. for UnitB and UnitC we use arduino_mountX2.stl in middle.  
install sensor module on sensor base. use two M3X8mm loosely tighten it for now. no need very tight, position need adjust later after put in box. 
assemble roller X 2, one is idler roller , one is power roller. use M2.6 len 8mm or 10mm (better) to put roller_axle_A and B  two halves together. 
for power roller, add large gear, check gear orientation, should align with motor gear. add TPU roller A(shorter) and B(longer).  for idler roller, add two TPU rollers B (longer) .  optional, add M2.6 screw len 6mm to lock TPU parts on roller. 
put bearing 686X4 on both roller, two sides. hand push the two rollers power roller and idler roll. power roller gear should align well and mesh well with motor gear. idler roller should rotate very freely.
install PTFE holder on motor base with M3 8mm.  
install two stand parts under motor base and sensor base using M2.6 8mmX2
install two box blocker part using M3 8mm screw, loosely tighten for now, will adjust after put in samla box. so the motor buffer module won't move around freely inside Samla box. 



main body assembly is finished.



Electronics Wiring

prepare Arduino Nano, flash the firmware



Optional - Optimization for Optical Sensor

The above optical endstop works, but they can work better if we mod it a little bit, by changing some of the resistors ( SMD 0805 size). By doing so we can make it draw less current, and last  longer, reduce load from the DC buck converter. But it's optional. Those sensor should work as is.

optional mod: replace IR resistor 220R to 600R. replace pull up resistor 2K to 10K. replace indicator resistor to 2k

