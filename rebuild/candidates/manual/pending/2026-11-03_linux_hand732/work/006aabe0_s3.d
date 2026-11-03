006aabe0 CThingPlayerCreature::SetActionForMelee DIFFER(1273v1278) flags= []
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
@@ retail 179-181 built 179-179
   006aae47  89442424               mov dword ptr [esp + 0x24], eax
   006aae4b  e8f0e7ffff             call 0x6a9640
 - 006aae50  d95c2418               fstp dword ptr [esp + 0x18]
 - 006aae54  8b4c2418               mov ecx, dword ptr [esp + 0x18]
   006aae58  51                     push ecx
   006aae59  8d4c2420               lea ecx, [esp + 0x20]
@@ retail 183-183 built 181-182
   006aae58  51                     push ecx
   006aae59  8d4c2420               lea ecx, [esp + 0x20]
 + 006aae55  d91c24                 fstp dword ptr [esp]
   006aae5d  e8de913600             call 0xa14040
   006aae62  8d54241c               lea edx, [esp + 0x1c]
@@ retail 184-186 built 183-185
   006aae59  8d4c2420               lea ecx, [esp + 0x20]
   006aae5d  e8de913600             call 0xa14040
 - 006aae62  8d54241c               lea edx, [esp + 0x1c]
 - 006aae66  52                     push edx
 + 006aae5d  8d4c241c               lea ecx, [esp + 0x1c]
 + 006aae61  51                     push ecx
   006aae67  56                     push esi
   006aae68  8bce                   mov ecx, esi
@@ retail 200-202 built 199-201
   006aae89  f6859100000001         test byte ptr [ebp + 0x91], 1
   006aae90  7551                   jne 0x6aaee3
 - 006aae92  8d44241c               lea eax, [esp + 0x1c]
 - 006aae96  50                     push eax
 + 006aae8d  8d54241c               lea edx, [esp + 0x1c]
 + 006aae91  52                     push edx
   006aae97  55                     push ebp
   006aae98  56                     push esi
@@ retail 213-214 built 212-213
   006aaeae  f6c402                 test ah, 2
   006aaeb1  7430                   je 0x6aaee3
 - 006aaeb3  8d4c2418               lea ecx, [esp + 0x18]
 + 006aaeae  8d442418               lea eax, [esp + 0x18]
   006aaeb7  8d7d44                 lea edi, [ebp + 0x44]
   006aaeba  51                     push ecx
@@ retail 215-216 built 214-215
   006aaeb3  8d4c2418               lea ecx, [esp + 0x18]
   006aaeb7  8d7d44                 lea edi, [ebp + 0x44]
 - 006aaeba  51                     push ecx
 + 006aaeb5  50                     push eax
   006aaebb  8bcf                   mov ecx, edi
   006aaebd  c744241c09000000       mov dword ptr [esp + 0x1c], 9
@@ retail 231-232 built 230-231
   006aaee3  f6462840               test byte ptr [esi + 0x28], 0x40
   006aaee7  742a                   je 0x6aaf13
 - 006aaee9  8d542418               lea edx, [esp + 0x18]
 + 006aaee4  8d4c2418               lea ecx, [esp + 0x18]
   006aaeed  8d7e44                 lea edi, [esi + 0x44]
   006aaef0  52                     push edx
@@ retail 233-234 built 232-233
   006aaee9  8d542418               lea edx, [esp + 0x18]
   006aaeed  8d7e44                 lea edi, [esi + 0x44]
 - 006aaef0  52                     push edx
 + 006aaeeb  51                     push ecx
   006aaef1  8bcf                   mov ecx, edi
   006aaef3  c744241c46000000       mov dword ptr [esp + 0x1c], 0x46
@@ retail 256-258 built 255-257
   006aaf39  0f8499000000           je 0x6aafd8
   006aaf3f  8d7844                 lea edi, [eax + 0x44]
 - 006aaf42  8d442418               lea eax, [esp + 0x18]
 - 006aaf46  50                     push eax
 + 006aaf3d  8d542418               lea edx, [esp + 0x18]
 + 006aaf41  52                     push edx
   006aaf47  8bcf                   mov ecx, edi
   006aaf49  c744241c48000000       mov dword ptr [esp + 0x1c], 0x48
@@ retail 274-278 built 273-277
   006aaf71  7565                   jne 0x6aafd8
   006aaf73  8b4e60                 mov ecx, dword ptr [esi + 0x60]
 - 006aaf76  8b11                   mov edx, dword ptr [ecx]
 - 006aaf78  8d442440               lea eax, [esp + 0x40]
 - 006aaf7c  50                     push eax
 - 006aaf7d  ff9218010000           call dword ptr [edx + 0x118]
 + 006aaf71  8b01                   mov eax, dword ptr [ecx]
 + 006aaf73  8d542440               lea edx, [esp + 0x40]
 + 006aaf77  52                     push edx
 + 006aaf78  ff9018010000           call dword ptr [eax + 0x118]
   006aaf83  8b08                   mov ecx, dword ptr [eax]
   006aaf85  894c2428               mov dword ptr [esp + 0x28], ecx
@@ retail 284-284 built 283-286
   006aaf90  8b4008                 mov eax, dword ptr [eax + 8]
   006aaf93  89442430               mov dword ptr [esp + 0x30], eax
 + 006aaf92  d944242c               fld dword ptr [esp + 0x2c]
 + 006aaf96  c744243000000000       mov dword ptr [esp + 0x30], 0
 + 006aaf9e  d84c242c               fmul dword ptr [esp + 0x2c]
   006aaf97  d9442428               fld dword ptr [esp + 0x28]
   006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
@@ retail 285-286 built 287-287
   006aaf93  89442430               mov dword ptr [esp + 0x30], eax
   006aaf97  d9442428               fld dword ptr [esp + 0x28]
 - 006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
   006aafa3  d84c2428               fmul dword ptr [esp + 0x28]
   006aafa7  d944242c               fld dword ptr [esp + 0x2c]
@@ retail 287-289 built 288-288
   006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
   006aafa3  d84c2428               fmul dword ptr [esp + 0x28]
 - 006aafa7  d944242c               fld dword ptr [esp + 0x2c]
 - 006aafab  d84c242c               fmul dword ptr [esp + 0x2c]
   006aafaf  dec1                   faddp st(1)
   006aafb1  d9053cba2901           fld dword ptr [0x129ba3c]
DIFFLINES 47 retail_instr 380 built_instr 379
