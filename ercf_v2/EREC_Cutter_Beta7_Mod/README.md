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
* the attached servo base part works with my 28mm servo thicker selector body mod

https://www.printables.com/model/1006480-ercf-v2-selector-mod-for-larger-28mm-17g-servo-ver

if use regular selector body, just use the regular servo mount part from beta 7

too complicated, that's why we really should decouple different parts to reduce the complexity.





Print settings

no support needed.



Assembly difference from original Beta7

use M2.5 heat insert for middle screw for knife/blade #11. and M2.5 screw. ( instead of M2) .  because M2.5 is stronger,  the blade's middle hole can hold M2.5,  M2.5 heat insert stays better in plastic.
use M2.5 heat insert in adapter filament output hole. refer to beta7's doc , it suggest put a M3 heat insert in encoder case, but in this mod, we are using adaptor,  encoder case stays as is, don't put heat insert in encoder.  instead, put a M2.5 heat insert in the adapter part.   obviously M2.5 hole fits filament better.
use M2.5 heat insert on the side hole of the servo base so we don't need M2.