006aabe0 CThingPlayerCreature::SetActionForMelee DIFFER(1278v1278) flags= []
@@ retail 73-75 built 73-73
   006aacc8  8b481c                 mov ecx, dword ptr [eax + 0x1c]
   006aaccb  894c2434               mov dword ptr [esp + 0x34], ecx
 - 006aaccf  d9442434               fld dword ptr [esp + 0x34]
 - 006aacd3  d84c2434               fmul dword ptr [esp + 0x34]
   006aacd7  8b5020                 mov edx, dword ptr [eax + 0x20]
   006aacda  89542438               mov dword ptr [esp + 0x38], edx
@@ retail 80-80 built 78-80
   006aace2  d84c2438               fmul dword ptr [esp + 0x38]
   006aace6  c744243c00000000       mov dword ptr [esp + 0x3c], 0
 + 006aace6  d9442434               fld dword ptr [esp + 0x34]
 + 006aacea  d84c2434               fmul dword ptr [esp + 0x34]
   006aacee  dec1                   faddp st(1)
   006aacf0  d9053cba2901           fld dword ptr [0x129ba3c]
@@ retail 132-132 built 132-133
   006aad9b  d94008                 fld dword ptr [eax + 8]
   006aad9e  d86708                 fsub dword ptr [edi + 8]
 + 006aada1  89542420               mov dword ptr [esp + 0x20], edx
   006aada1  894c241c               mov dword ptr [esp + 0x1c], ecx
   006aada5  89542420               mov dword ptr [esp + 0x20], edx
@@ retail 133-134 built 134-134
   006aad9e  d86708                 fsub dword ptr [edi + 8]
   006aada1  894c241c               mov dword ptr [esp + 0x1c], ecx
 - 006aada5  89542420               mov dword ptr [esp + 0x20], edx
   006aada9  d95c2448               fstp dword ptr [esp + 0x48]
   006aadad  8b442448               mov eax, dword ptr [esp + 0x48]
@@ retail 136-136 built 136-140
   006aada9  d95c2448               fstp dword ptr [esp + 0x48]
   006aadad  8b442448               mov eax, dword ptr [esp + 0x48]
 + 006aadb1  d9442420               fld dword ptr [esp + 0x20]
 + 006aadb5  89442424               mov dword ptr [esp + 0x24], eax
 + 006aadb9  d84c2420               fmul dword ptr [esp + 0x20]
 + 006aadbd  c744242400000000       mov dword ptr [esp + 0x24], 0
   006aadb1  d944241c               fld dword ptr [esp + 0x1c]
   006aadb5  89442424               mov dword ptr [esp + 0x24], eax
@@ retail 137-138 built 141-141
   006aadad  8b442448               mov eax, dword ptr [esp + 0x48]
   006aadb1  d944241c               fld dword ptr [esp + 0x1c]
 - 006aadb5  89442424               mov dword ptr [esp + 0x24], eax
   006aadb9  d84c241c               fmul dword ptr [esp + 0x1c]
   006aadbd  c744242400000000       mov dword ptr [esp + 0x24], 0
@@ retail 139-142 built 142-142
   006aadb5  89442424               mov dword ptr [esp + 0x24], eax
   006aadb9  d84c241c               fmul dword ptr [esp + 0x1c]
 - 006aadbd  c744242400000000       mov dword ptr [esp + 0x24], 0
 - 006aadc5  d9442420               fld dword ptr [esp + 0x20]
 - 006aadc9  d84c2420               fmul dword ptr [esp + 0x20]
   006aadcd  dec1                   faddp st(1)
   006aadcf  d9053cba2901           fld dword ptr [0x129ba3c]
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
@@ retail 284-284 built 284-287
   006aaf90  8b4008                 mov eax, dword ptr [eax + 8]
   006aaf93  89442430               mov dword ptr [esp + 0x30], eax
 + 006aaf97  d944242c               fld dword ptr [esp + 0x2c]
 + 006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
 + 006aafa3  d84c242c               fmul dword ptr [esp + 0x2c]
   006aaf97  d9442428               fld dword ptr [esp + 0x28]
   006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
@@ retail 285-286 built 288-288
   006aaf93  89442430               mov dword ptr [esp + 0x30], eax
   006aaf97  d9442428               fld dword ptr [esp + 0x28]
 - 006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
   006aafa3  d84c2428               fmul dword ptr [esp + 0x28]
   006aafa7  d944242c               fld dword ptr [esp + 0x2c]
@@ retail 287-289 built 289-289
   006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
   006aafa3  d84c2428               fmul dword ptr [esp + 0x28]
 - 006aafa7  d944242c               fld dword ptr [esp + 0x2c]
 - 006aafab  d84c242c               fmul dword ptr [esp + 0x2c]
   006aafaf  dec1                   faddp st(1)
   006aafb1  d9053cba2901           fld dword ptr [0x129ba3c]
DIFFLINES 30 retail_instr 380 built_instr 380
