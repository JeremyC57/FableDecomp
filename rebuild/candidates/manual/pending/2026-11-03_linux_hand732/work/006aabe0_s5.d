006aabe0 CThingPlayerCreature::SetActionForMelee DIFFER(1278v1278) flags= []
@@ retail 77-77 built 77-78
   006aacd7  8b5020                 mov edx, dword ptr [eax + 0x20]
   006aacda  89542438               mov dword ptr [esp + 0x38], edx
 + 006aacde  c744243c00000000       mov dword ptr [esp + 0x3c], 0
   006aacde  d9442438               fld dword ptr [esp + 0x38]
   006aace2  d84c2438               fmul dword ptr [esp + 0x38]
@@ retail 79-80 built 80-80
   006aacde  d9442438               fld dword ptr [esp + 0x38]
   006aace2  d84c2438               fmul dword ptr [esp + 0x38]
 - 006aace6  c744243c00000000       mov dword ptr [esp + 0x3c], 0
   006aacee  dec1                   faddp st(1)
   006aacf0  d9053cba2901           fld dword ptr [0x129ba3c]
DIFFLINES 2 retail_instr 380 built_instr 380
