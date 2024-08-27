Description

Inspired by Annex-Engineering's Belay sensor module

 https://github.com/Annex-Engineering/Belay

 

Inspired by jmillerfo's Voron ERCF Filament Stress Sensor

 https://www.printables.com/model/803180-voron-ercf-filament-stress-sensor

 
Major difference from above modules

    two way sensors module, using hall effect sensor
    base on standard PCB 2CMX8CM
    with pcb, we can add 2X color LEDs to indicate sensor states
    added filament tension indicator/window for user to watch
    larger moving distance, it's about 12mm movable.

 
 
Work with Happy hare MMU software and ERCF V2 

https://github.com/Enraged-Rabbit-Community/ERCF_v2

https://github.com/moggieuk/Happy-Hare

 

 
What:

to help MMU ERCF V2 (V1) output correct amount of filament to keep filament sync tension remain in neutral state.

 
Why:

 MMU like ERCF V2 or V1 works with controller software like Happy Hare output/extrude certain amount of filament while printer printing, currently ERCF V2 has no idea the filament it output is too much or too little.

There is a rotation_distance value 22.7316868  as default value in HappyHare default config, this value may work to certain level, but it depends on your actual gear on each gate.

If the value is 1% off the actual size, then after print 10meter of filament (maybe 2-3 hours of printing ), there is 0.1 meter of filament too much or too little output by the ERCF.

if too little, the print is ruined.

if too much, the PTEF tube will be forced got out of the connector

either way grinding and stress stepper motor etc.

 
Solution:

Use this small device to help detect the filament tension state, there is a small mover block inside the device, it has a 3mmX2mm small magnet.

If filament is getting expanded (too little), it triggers the hall effect sensor in one side, it notifies the software to output a bit more.

If filament is getting compressed(too much), it triggers the hall effect sensor in the other side, it notifies the software to output a bit less.

 

Connect two hall effect sensor 's output to BTT MMB stop connectors.

Set HappyHare  config for two pins

with the correct software support, the filament shall forever remain in neutral.

 
BOM

    2CMX8CM prototype double side PCB .  for easier electronics part of the work, use this standard size one, can be bought from a lot of places, cheap.
    round magnet : 3mm diameter 2mm thick X 1pcs
    hall effect sensor X 2pcs, SMD SOT23 , I think through hole type works too( I use 44E (full name HAL3144E SMD SOT23,  TH type can use  A3144 ), the names are similar but they are from different manufacturers.
    optional but highly recommended , two LEDs with different colors.  SMD 1206 size RED BLUE or GREEN or other colors,  or through hole type 3mm LEDs ( red blue green etc choose two different colors)
    SMD or TH resistors for LED 3K resistance X 2  (value can be more or less)
    XH254  1X 4pin  XH254 socket  on PCB 
    2X   3PIN XH254 plug to BTT MMB
    ECAS ptfe tube connectors X 2
    ptfe tube 2.5mm ID  10cm to 20cm
    M2 screws 6mm long multiple
    thin fly wire (ie 30AWG )

 
Skill Required

    solder TH or SMD part on double side PCB
    knowledge about basic electrical stuffs.
    make XH254 connectors, or solder wires if using pre-made 

  
Print setting and material 

Material any

20%  to 40% infill

no support needed

if you want to print in multi color, drag and drop case.stl and sign.stl together at the same time to slicer.

suggest use different color to print mover.stl file, so user can easily see the current state from the windows of the case

 
Files

fusion360 f3z file provided.

stl provided.

step files provided.

 
Assembly

1

some hall effect sensor can only be triggered by one side of magnet.

so before putting magnet into the mover block, check the bottom side of the magnet can trigger the sensor. therefore, should make PCB first, make mover block later

2 check schematics and have some idea about the pcb.

3 print bottom.stl part, use it to align the hall effect sensor position. make the pcb. also check the pcb picture as reference for position of two sensors

4 no wire should be solder on top side of the pcb, only two sensor on top. otherwise it cannot be installed under the “bottom.stl” part. that's why use choose double side pcb. we can fly the wires easily on bottom side.   solder 2X LEDs in the hole position. 

note, after turn pcb up side down, the sensor pin position is reversed.

5 after pcb is made, use lab power supply to test it with magnet to see if LED can turn on when trigger.  If not trigger, turn magnet to other side, mark the side, install magnet in mover block.

6 install ECAS connect to mover block and case.

7 the inner ECAS must have a locking ring, but need to cut the tab on ring. without the ring, ptef tube can be easily put out, so must have the locker ring.

 
Software Config

happy hare mmu_hardware.cfg
sync_feedback_tension_pin: !mmu:PB12

sync_feedback_compression_pin: !mmu:PB11

happy hare mmu_parameter.cfg

sync_feedback_enable: 1   # 0 = Turn off (even with fitted sensor), 1 = Turn on when printing
sync_multiplier_high: 1.05  # Maximum factor to apply to gear stepper rotation_distance
sync_multipler_low: 0.95  # Minimum factor to apply

 
Pros and Cons about using hall effect sensors

Pro 

smaller, looks cool, reliable than mechanical /contact micro switch

Con

need to make PCB ( not making pcb itself, just solder part on pcb)

if want to add led light, need pcb even more so

hall effect sensor's position is not easy to set. when magnet gets closer to the sensor, it could be 4mm away, 3mm away, or 2mm away, the sensor is triggered, it's not super easy to set the accurate end position.
 