006aabe0 CThingPlayerCreature::SetActionForMelee DIFFER(1278v1278) flags= []
@@ retail 157-157 built 157-159
   006aadfc  84c9                   test cl, cl
   006aadfe  7562                   jne 0x6aae62
 + 006aae00  d9442448               fld dword ptr [esp + 0x48]
 + 006aae04  d84c2448               fmul dword ptr [esp + 0x48]
   006aae00  d9442440               fld dword ptr [esp + 0x40]
   006aae04  d84c2440               fmul dword ptr [esp + 0x40]
@@ retail 159-161 built 161-161
   006aae00  d9442440               fld dword ptr [esp + 0x40]
   006aae04  d84c2440               fmul dword ptr [esp + 0x40]
 - 006aae08  d9442448               fld dword ptr [esp + 0x48]
 - 006aae0c  d84c2448               fmul dword ptr [esp + 0x48]
   006aae10  dec1                   faddp st(1)
   006aae12  d9442444               fld dword ptr [esp + 0x44]
DIFFLINES 4 retail_instr 380 built_instr 380
