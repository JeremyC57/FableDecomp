006aabe0 CThingPlayerCreature::SetActionForMelee DIFFER(1278v1278) flags= []
@@ retail 157-157 built 157-162
   006aadfc  84c9                   test cl, cl
   006aadfe  7562                   jne 0x6aae62
 + 006aae00  d9442448               fld dword ptr [esp + 0x48]
 + 006aae04  d84c2448               fmul dword ptr [esp + 0x48]
 + 006aae08  d9442444               fld dword ptr [esp + 0x44]
 + 006aae0c  d84c2444               fmul dword ptr [esp + 0x44]
 + 006aae10  dec1                   faddp st(1)
   006aae00  d9442440               fld dword ptr [esp + 0x40]
   006aae04  d84c2440               fmul dword ptr [esp + 0x40]
@@ retail 159-164 built 164-164
   006aae00  d9442440               fld dword ptr [esp + 0x40]
   006aae04  d84c2440               fmul dword ptr [esp + 0x40]
 - 006aae08  d9442448               fld dword ptr [esp + 0x48]
 - 006aae0c  d84c2448               fmul dword ptr [esp + 0x48]
 - 006aae10  dec1                   faddp st(1)
 - 006aae12  d9442444               fld dword ptr [esp + 0x44]
 - 006aae16  d84c2444               fmul dword ptr [esp + 0x44]
   006aae1a  dec1                   faddp st(1)
   006aae1c  d9053cba2901           fld dword ptr [0x129ba3c]
DIFFLINES 10 retail_instr 380 built_instr 380
