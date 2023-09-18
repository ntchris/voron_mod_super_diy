# ERCF Setting Notes for Mellow ERCF CAN BUS board

This is the Mellow ERCF canbus board
![mellow ercf](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/mellow_ERCF_CanBus/mellow_ercf_canbus_pcb.png)

There are so many combination in the configuration when using ERCF, see below..., so I think a HW/SW config note is necessary.
### A few key points in the config:
* Using SensorLess Selector Homing
* Mellow ERCF Board supporting Canbus
* Using CANBUS (not USB) to Klipper mother board
* My Klipper mother board is Optopus but it shouldn't matter
* Using HappyHare ERCF Software V3 (https://github.com/moggieuk/ERCF-Software-V3). ( original ERCF fw has quite different config file)
* Tool head has no hallsensor or mechanical microswitch sensor, since not needed ( but PTFE Pneumatic fitting to lock the PTFE (2.5 ID mm/4mm OD) is needed since ptfe tube must be locked tight to the tool head. 

## Mellow ERCF Canbus Board preparation
### Hardware Jumper config
1
Do NOT plug in the CanBus 120 Resistor Jumper, because, Klipper board has it, Tool Head has it, so ERCF doesn't need it.
base on the fact that we are trying to use CANBus for ERCF, so I am guessing most likely a CanBus toolhead is already running in the system!
There are total TWO 120 ohm resistors should be in the CAN bus, at the beginning and the end of the CANBus.
(It's hard to tell if the Canbus toolhead board or Canbus ERCF is the end, so it shouldn't matter, both are equally far far away from the Klipper mother board)

2 
Selector Driver stick is on the left.
use only one Jumper for Uart under driver stick slot, 3rd row, jumper on right.
uart jumper 

![uart jumper setting](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/mellow_ERCF_CanBus/mellow_ercf_uartmode.jpg)

no other jumper is needed, diag doesn't need jumper.

3 mellow TMC2209 driver stick setting for DIAG PIN.
diag micro switch must move to ON position ( ON position, not 1, low )

![TMC2209 diag switch](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/mellow_ERCF_CanBus/mellow_tmc2209_sensorless.jpg)


4 Always double check all plug's wires polarity. 
  I am moving from Fysetc's ERCF (usb interface) to this, so quite a fiew wire need to mod.
  mechanical micro switch end stop.
  optical encoder
  no need for stepper motor

### Flashing CANBOOT for ERCF board

the Mellow ERCF board may already have CANBOOT flashed in factory, howerver, the speed may or may not be the same as our existing Klipper mother board (ie Octopus/ToolHead Canbus board etc).  My existing system is running 1000,000 (six zero), so the new mellow ercf board doesn't work with it ( so maybe it's running 500000 (5 zero) speed ) 
So USB is the only option to flash Canboot.

ssh login Pi4.

```
git clone https://github.com/Arksine/CanBoot
cd CanBoot
make menuconfig
```

Use below setting and save then make
```
RP2040
16K bootloader
4RX/ 5TX 
1000000 ( or 500000, must be the same as Klipper mother board)
Enable Status LED
GPIO17 for mellow LED
```
![make menuconfig for canboot](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/mellow_ERCF_CanBus/mellow_ercf_canboot_config.jpg)
(not to be confused by ERCF klipper firmware, this is for CANBOOT only)

run make to make the uf2 file in the out dir
```
make
```
get the ufs file in dir out/canboot.uf2

power off ERCF (the whole printer), 
connect USB cable to Pi4, hold boot botton in ERCF board, power on ERCF. (the whole printer, including 24V)

```
lsusb  
```
lsusb command should find the rp2040 id ( mostlikely 2e8a:0003), if not, must redo the hold botton / power on again.
run this to flash CANBOOT to the ERCF board
```
 make flash FLASH_DEVICE=2e8a:0003
```

### Flashing Klipper for Mellow ERCF
ssh login Pi4
```
cd ~/klipper
make menuconfig
```
choose every option mostly the same as previous canboot,
RP2040
16K bootloader
4RX/ 5TX 
1000000 ( or 500000, must be the same as Klipper mother board)
Enable Status LED
GPIO17 for mellow LED
![make menuconfig for ERCF Klipper](https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/mellow_ERCF_CanBus/mellow_ercf_klipper_make_config.jpg)

make and flash it using the CAN ID of the ERCF
```
make -j4
python3 ~/klipper/lib/canboot/flash_can.py -u efc740efc589 <==(the CAN id)
```

## printer config file for mellow ERCF Canbus using SensorLess Selector Homing
refer to this config file https://github.com/ntchris/voron_mod_super_diy/blob/main/for_enraged_rabbit/mellow_ERCF_CanBus/ercf_hardware_mellow_canbus.cfg

basic facts about Mellow ERCF board.
selector endstop pin is GPIO20
Left driver stick is driver0 in schematics, is for Selector, TMC driver diag pin is gpio22


## a few key points in the config file (again, for sensorless selector homing)
config file name ercf_hardware_mellow_canbus.cfg, include this file in the main printer.cfg file.
set the CAN id
```
[mcu ercf]
canbus_uuid:  efc74####c589 (your mileage may vary, your CAN ID is very different)
```

for physical selector endstop pin (even though we are using sensorless homing, physical selector is still needed, because Happy Hare can use that to detect a true home position VS blocked by filament.
```
[manual_extruder_stepper gear_stepper]
endstop_pin: ercf:gpio20
```

```
[tmc2209 manual_stepper selector_stepper]
diag_pin: ercf:gpio22	# Set to MCU pin connected to TMC DIAG pin, pullup is not needed

# no need to make selector stepper motor current as low as possible, since select motor rarely move
# if current is too low, 
run_current: 0.55
driver_SGTHRS: 70		# 255 is most sensitive value, 0 is least sensitive, adjust according to your setup
```


```
[manual_stepper selector_stepper]
endstop_pin: tmc2209_selector_stepper:virtual_endstop

```

When everything is correct, 
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
[ercf_servo ercf_servo]
pin: ercf:gpio21   # mellow ercf
```


```
## ENCODER -----------------------------------------------------------------------------------------------------------------
[ercf_encoder ercf_encoder]
encoder_pin: ^ercf:gpio15	  # mellow ercf
```

ercf_paramenters.cfg
```
sensorless_selector: 1		# 0 = use only a physical endstop, 1 = use advanced sensorless selector homing
```

Using HappyHare, should always sync to extruder, so to let ERCF help to pull the filament out from buffer, otherwise the printer's toolhead extruder may not have enough power to pull and the print will have big problem.
```
sync_to_extruder: 1		# Gear motor is synchronized to extruder during print  
```
To help to let ERCF always enabled after printer restart, can use this version of ERCF ercf.py, https://github.com/ntchris/ERCF-Software-V3/blob/master/extras/ercf.py

## Testing sensorless selector home
Use ERCF_home and ERCF__SELECT_TOOL TOOL=5 to test selector.
If selector cannot move and have red error inside mainsail
> An issue with the ERCF has been detected...Selector recovery failed. Path is probably internally blocked and unable to move filament to clear
must set the current higher (ie 0.55, 0.6 ) and driver_SGTHRS lower (ie 60, 70, 80)
If current is too low and driver is too sensitive, it cannot move because the resistance/friction is too high.

## Trouble shooting
### Mainsail cannot boot, firmware restart / restart error in webpage.
Probably the CAN wire is not properly connected in ERCF board. 
Consider replug in the 24V/Can Plug in the mellow ERCF board.

### when move, selector goes to wrong direction - left / right
depends on the stepper motor wiring, the direction could be reverted.
can be fixed by reverting the dir pin by adding or removing !
```
dir_pin: !ercf:gpio1
dir_pin: ercf:gpio1
```
### when gear motor rotate, it goes in wrong direction.
refer above

### selector cannot move and have red error inside mainsail
> An issue with the ERCF has been detected...Selector recovery failed. Path is probably internally blocked and unable to move filament to clear
must set the current higher (ie 0.55, 0.6 ) and driver_SGTHRS lower (ie 60, 70, 80)
If current is too low and driver is too sensitive, it cannot move because the resistance/friction is too high.

### can id cannot query, always zero can device returned

python3  ~/klipper/scripts/canbus_query.py can0
or 
python3 ~/klipper/lib/canboot/flash_can.py -i can0 -q
unfortunately, when everything is working correctly, can ID cannot show!
(true to klipper mother board, toolhead, ercf board)




## Other reference
Mellow TMC2209
https://mellow.klipper.cn/?spm=a2g0o.detail.1000023.3.13e16cccOvvDGr#/board/fly_tmc/2209?id=tmc2209

Mellow Toolhead Canbus
https://mellow.klipper.cn/?spm=a2g0o.detail.1000023.3.13e16cccOvvDGr#/board/fly_sb2040/README

Mellow ERCF Canbus board
https://mellow.klipper.cn/?spm=a2g0o.detail.1000023.3.13e16cccOvvDGr#/board/fly_ercf/README

happyhare
https://github.com/moggieuk/ERCF-Software-V3

Mellow CanBoot
https://mellow.klipper.cn/?spm=a2g0o.detail.1000023.3.13e16cccOvvDGr#/advanced/canboot




