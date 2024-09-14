# ERCF filament Cutter beta7 mod

## What:
for ERCF V2 / ERCF Filament Cutter Beta7 https://github.com/kevinakasam/ERCF_Filament_Cutter

* it fixed or improved a few problem in the beta7 design
* it works with my ERCF V2  28mm servo mod, since my 28mm servo mod selector has a thicker body, also stronger screw mount holes on top.
 

## New changes comparing to ERCF Cutter Beta7
 one major idea is to decouple cutter unit from the ercf encoder case.
 * the cutter beta7 need to change the encoder case once a few months, in different releases, I think this should be avoided since ERCF encoder case may have other mods,  and it's not easy to print and put in heat insert M3 multiple, and endstop screw need to be precisely adjusted everytime.   the idea is to make the encoder case stable and don't change unless really really necessary.
 * in order to support new features in beta7, also avoid touching/changing the encoder case, I made a adapter part, so encoder case stays as is.

 * in order to make it works with my 28mm servo MOD thicker selector body , I made a different servo base part.
 * servo GEAR: to remove the built in support in the beta7 design, which is quite difficult to remove and dangerous, the modified gear part now can be just printed, without any support. 
 * in beta7 , the cutter blade/knife need to be cut/removed or break the front tip, which is very dangerous and not convenient. knife does get dull over time, we need to change knife once a few months. This mod removed this requirement, not needed.  I moved the guide rail 2-3mm upper, so knife tip can stay.  because I am using an adapter, so space is not not limited as it was in encoder case, I have the space to move the rail away from knife tip.
 
 * no change for servo mount part.