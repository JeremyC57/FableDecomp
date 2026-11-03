006aabe0 CThingPlayerCreature::SetActionForMelee DIFFER(1273v1278) flags= []
@@ retail 30-31 built 30-31
   006aac32  8be8                   mov ebp, eax
   006aac34  8b86c0010000           mov eax, dword ptr [esi + 0x1c0]
 - 006aac3a  894c2444               mov dword ptr [esp + 0x44], ecx
 + 006aac3a  894c2438               mov dword ptr [esp + 0x38], ecx
   006aac3e  8b4e60                 mov ecx, dword ptr [esi + 0x60]
   006aac41  89542448               mov dword ptr [esp + 0x48], edx
@@ retail 32-33 built 32-33
   006aac3a  894c2444               mov dword ptr [esp + 0x44], ecx
   006aac3e  8b4e60                 mov ecx, dword ptr [esi + 0x60]
 - 006aac41  89542448               mov dword ptr [esp + 0x48], edx
 + 006aac41  8954243c               mov dword ptr [esp + 0x3c], edx
   006aac45  8d54244c               lea edx, [esp + 0x4c]
   006aac49  89442440               mov dword ptr [esp + 0x40], eax
@@ retail 34-35 built 34-35
   006aac41  89542448               mov dword ptr [esp + 0x48], edx
   006aac45  8d54244c               lea edx, [esp + 0x4c]
 - 006aac49  89442440               mov dword ptr [esp + 0x40], eax
 + 006aac49  89442434               mov dword ptr [esp + 0x34], eax
   006aac4d  8b01                   mov eax, dword ptr [ecx]
   006aac4f  52                     push edx
@@ retail 37-39 built 37-38
   006aac4d  8b01                   mov eax, dword ptr [ecx]
   006aac4f  52                     push edx
 - 006aac50  c744241802000000       mov dword ptr [esp + 0x18], 2
 - 006aac58  ff9018010000           call dword ptr [eax + 0x118]
 + 006aac50  ff9014010000           call dword ptr [eax + 0x114]
   006aac5e  8d44244c               lea eax, [esp + 0x4c]
   006aac62  50                     push eax
@@ retail 43-44 built 42-43
   006aac63  56                     push esi
   006aac64  8bce                   mov ecx, esi
 - 006aac66  32db                   xor bl, bl
 + 006aac5e  c744241c02000000       mov dword ptr [esp + 0x1c], 2
   006aac68  e86372fbff             call 0x661ed0
   006aac6d  8bc8                   mov ecx, eax
@@ retail 52-52 built 51-53
   006aac7f  7502                   jne 0x6aac83
   006aac81  b301                   mov bl, 1
 + 006aac81  eb02                   jmp 0x6aac85
 + 006aac83  32db                   xor bl, bl
   006aac83  8b442454               mov eax, dword ptr [esp + 0x54]
   006aac87  8b4c244c               mov ecx, dword ptr [esp + 0x4c]
@@ retail 71-71 built 72-73
   006aacbe  0f8487000000           je 0x6aad4b
   006aacc4  8b442418               mov eax, dword ptr [esp + 0x18]
 + 006aacca  d94020                 fld dword ptr [eax + 0x20]
   006aacc8  8b481c                 mov ecx, dword ptr [eax + 0x1c]
   006aaccb  894c2434               mov dword ptr [esp + 0x34], ecx
@@ retail 72-80 built 74-81
   006aacc4  8b442418               mov eax, dword ptr [esp + 0x18]
   006aacc8  8b481c                 mov ecx, dword ptr [eax + 0x1c]
 - 006aaccb  894c2434               mov dword ptr [esp + 0x34], ecx
 - 006aaccf  d9442434               fld dword ptr [esp + 0x34]
 - 006aacd3  d84c2434               fmul dword ptr [esp + 0x34]
 - 006aacd7  8b5020                 mov edx, dword ptr [eax + 0x20]
 - 006aacda  89542438               mov dword ptr [esp + 0x38], edx
 - 006aacde  d9442438               fld dword ptr [esp + 0x38]
 - 006aace2  d84c2438               fmul dword ptr [esp + 0x38]
 - 006aace6  c744243c00000000       mov dword ptr [esp + 0x3c], 0
 + 006aacd0  d9542444               fst dword ptr [esp + 0x44]
 + 006aacd4  894c2440               mov dword ptr [esp + 0x40], ecx
 + 006aacd8  d9442440               fld dword ptr [esp + 0x40]
 + 006aacdc  c744244800000000       mov dword ptr [esp + 0x48], 0
 + 006aace4  d84c2440               fmul dword ptr [esp + 0x40]
 + 006aace8  d9c1                   fld st(1)
 + 006aacea  d8ca                   fmul st(2)
   006aacee  dec1                   faddp st(1)
   006aacf0  d9053cba2901           fld dword ptr [0x129ba3c]
@@ retail 85-85 built 86-87
   006aacfc  ded9                   fcompp 
   006aacfe  dfe0                   fnstsw ax
 + 006aacfe  ddd8                   fstp st(0)
   006aad00  f6c405                 test ah, 5
   006aad03  7a46                   jp 0x6aad4b
@@ retail 87-88 built 89-90
   006aad00  f6c405                 test ah, 5
   006aad03  7a46                   jp 0x6aad4b
 - 006aad05  8d4c2434               lea ecx, [esp + 0x34]
 + 006aad05  8d4c2440               lea ecx, [esp + 0x40]
   006aad09  e832973600             call 0xa14440
   006aad0e  8d442434               lea eax, [esp + 0x34]
@@ retail 89-91 built 91-93
   006aad05  8d4c2434               lea ecx, [esp + 0x34]
   006aad09  e832973600             call 0xa14440
 - 006aad0e  8d442434               lea eax, [esp + 0x34]
 - 006aad12  50                     push eax
 + 006aad0e  8d542440               lea edx, [esp + 0x40]
 + 006aad12  52                     push edx
   006aad13  56                     push esi
   006aad14  8bce                   mov ecx, esi
@@ retail 102-108 built 104-110
   006aad2f  84db                   test bl, bl
   006aad31  7518                   jne 0x6aad4b
 - 006aad33  8b4c2434               mov ecx, dword ptr [esp + 0x34]
 - 006aad37  8b542438               mov edx, dword ptr [esp + 0x38]
 - 006aad3b  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 006aad3f  894c241c               mov dword ptr [esp + 0x1c], ecx
 - 006aad43  89542420               mov dword ptr [esp + 0x20], edx
 - 006aad47  89442424               mov dword ptr [esp + 0x24], eax
 + 006aad33  8b442440               mov eax, dword ptr [esp + 0x40]
 + 006aad37  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 006aad3b  8b542448               mov edx, dword ptr [esp + 0x48]
 + 006aad3f  8944241c               mov dword ptr [esp + 0x1c], eax
 + 006aad43  894c2420               mov dword ptr [esp + 0x20], ecx
 + 006aad47  89542424               mov dword ptr [esp + 0x24], edx
   006aad4b  8bce                   mov ecx, esi
   006aad4d  e8decce1ff             call 0x4c7a30
@@ retail 125-126 built 127-127
   006aad83  d827                   fsub dword ptr [edi]
   006aad85  d95c2440               fstp dword ptr [esp + 0x40]
 - 006aad89  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   006aad8d  d94004                 fld dword ptr [eax + 4]
   006aad90  d86704                 fsub dword ptr [edi + 4]
@@ retail 129-130 built 130-131
   006aad90  d86704                 fsub dword ptr [edi + 4]
   006aad93  d95c2444               fstp dword ptr [esp + 0x44]
 - 006aad97  8b542444               mov edx, dword ptr [esp + 0x44]
 + 006aad93  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   006aad9b  d94008                 fld dword ptr [eax + 8]
   006aad9e  d86708                 fsub dword ptr [edi + 8]
@@ retail 131-131 built 132-133
   006aad97  8b542444               mov edx, dword ptr [esp + 0x44]
   006aad9b  d94008                 fld dword ptr [eax + 8]
 + 006aad9a  8b442440               mov eax, dword ptr [esp + 0x40]
   006aad9e  d86708                 fsub dword ptr [edi + 8]
   006aada1  894c241c               mov dword ptr [esp + 0x1c], ecx
@@ retail 132-134 built 134-136
   006aad9b  d94008                 fld dword ptr [eax + 8]
   006aad9e  d86708                 fsub dword ptr [edi + 8]
 - 006aada1  894c241c               mov dword ptr [esp + 0x1c], ecx
 - 006aada5  89542420               mov dword ptr [esp + 0x20], edx
 + 006aada1  894c2420               mov dword ptr [esp + 0x20], ecx
 + 006aada5  8944241c               mov dword ptr [esp + 0x1c], eax
   006aada9  d95c2448               fstp dword ptr [esp + 0x48]
   006aadad  8b442448               mov eax, dword ptr [esp + 0x48]
@@ retail 135-136 built 137-142
   006aada5  89542420               mov dword ptr [esp + 0x20], edx
   006aada9  d95c2448               fstp dword ptr [esp + 0x48]
 - 006aadad  8b442448               mov eax, dword ptr [esp + 0x48]
 + 006aadad  8b542448               mov edx, dword ptr [esp + 0x48]
 + 006aadb1  d9442420               fld dword ptr [esp + 0x20]
 + 006aadb5  89542424               mov dword ptr [esp + 0x24], edx
 + 006aadb9  d84c2420               fmul dword ptr [esp + 0x20]
 + 006aadbd  c744242400000000       mov dword ptr [esp + 0x24], 0
   006aadb1  d944241c               fld dword ptr [esp + 0x1c]
   006aadb5  89442424               mov dword ptr [esp + 0x24], eax
@@ retail 137-138 built 143-143
   006aadad  8b442448               mov eax, dword ptr [esp + 0x48]
   006aadb1  d944241c               fld dword ptr [esp + 0x1c]
 - 006aadb5  89442424               mov dword ptr [esp + 0x24], eax
   006aadb9  d84c241c               fmul dword ptr [esp + 0x1c]
   006aadbd  c744242400000000       mov dword ptr [esp + 0x24], 0
@@ retail 139-142 built 144-144
   006aadb5  89442424               mov dword ptr [esp + 0x24], eax
   006aadb9  d84c241c               fmul dword ptr [esp + 0x1c]
 - 006aadbd  c744242400000000       mov dword ptr [esp + 0x24], 0
 - 006aadc5  d9442420               fld dword ptr [esp + 0x20]
 - 006aadc9  d84c2420               fmul dword ptr [esp + 0x20]
   006aadcd  dec1                   faddp st(1)
   006aadcf  d9053cba2901           fld dword ptr [0x129ba3c]
@@ retail 157-161 built 159-163
   006aadfc  84c9                   test cl, cl
   006aadfe  7562                   jne 0x6aae62
 - 006aae00  d9442440               fld dword ptr [esp + 0x40]
 - 006aae04  d84c2440               fmul dword ptr [esp + 0x40]
 - 006aae08  d9442448               fld dword ptr [esp + 0x48]
 - 006aae0c  d84c2448               fmul dword ptr [esp + 0x48]
 + 006aae00  d944243c               fld dword ptr [esp + 0x3c]
 + 006aae04  d84c243c               fmul dword ptr [esp + 0x3c]
 + 006aae08  d9442438               fld dword ptr [esp + 0x38]
 + 006aae0c  d84c2438               fmul dword ptr [esp + 0x38]
   006aae10  dec1                   faddp st(1)
   006aae12  d9442444               fld dword ptr [esp + 0x44]
@@ retail 162-164 built 164-166
   006aae0c  d84c2448               fmul dword ptr [esp + 0x48]
   006aae10  dec1                   faddp st(1)
 - 006aae12  d9442444               fld dword ptr [esp + 0x44]
 - 006aae16  d84c2444               fmul dword ptr [esp + 0x44]
 + 006aae12  d9442434               fld dword ptr [esp + 0x34]
 + 006aae16  d84c2434               fmul dword ptr [esp + 0x34]
   006aae1a  dec1                   faddp st(1)
   006aae1c  d9053cba2901           fld dword ptr [0x129ba3c]
@@ retail 171-175 built 173-177
   006aae2c  f6c405                 test ah, 5
   006aae2f  7a31                   jp 0x6aae62
 - 006aae31  8b4c2440               mov ecx, dword ptr [esp + 0x40]
 - 006aae35  8b542444               mov edx, dword ptr [esp + 0x44]
 - 006aae39  8b442448               mov eax, dword ptr [esp + 0x48]
 - 006aae3d  894c241c               mov dword ptr [esp + 0x1c], ecx
 + 006aae31  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 + 006aae35  8b442434               mov eax, dword ptr [esp + 0x34]
 + 006aae39  8b54243c               mov edx, dword ptr [esp + 0x3c]
 + 006aae3d  894c2420               mov dword ptr [esp + 0x20], ecx
   006aae41  8bce                   mov ecx, esi
   006aae43  89542420               mov dword ptr [esp + 0x20], edx
@@ retail 176-178 built 178-180
   006aae3d  894c241c               mov dword ptr [esp + 0x1c], ecx
   006aae41  8bce                   mov ecx, esi
 - 006aae43  89542420               mov dword ptr [esp + 0x20], edx
 - 006aae47  89442424               mov dword ptr [esp + 0x24], eax
 + 006aae43  8944241c               mov dword ptr [esp + 0x1c], eax
 + 006aae47  89542424               mov dword ptr [esp + 0x24], edx
   006aae4b  e8f0e7ffff             call 0x6a9640
   006aae50  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 179-181 built 181-181
   006aae47  89442424               mov dword ptr [esp + 0x24], eax
   006aae4b  e8f0e7ffff             call 0x6a9640
 - 006aae50  d95c2418               fstp dword ptr [esp + 0x18]
 - 006aae54  8b4c2418               mov ecx, dword ptr [esp + 0x18]
   006aae58  51                     push ecx
   006aae59  8d4c2420               lea ecx, [esp + 0x20]
@@ retail 183-183 built 183-184
   006aae58  51                     push ecx
   006aae59  8d4c2420               lea ecx, [esp + 0x20]
 + 006aae55  d91c24                 fstp dword ptr [esp]
   006aae5d  e8de913600             call 0xa14040
   006aae62  8d54241c               lea edx, [esp + 0x1c]
@@ retail 184-186 built 185-187
   006aae59  8d4c2420               lea ecx, [esp + 0x20]
   006aae5d  e8de913600             call 0xa14040
 - 006aae62  8d54241c               lea edx, [esp + 0x1c]
 - 006aae66  52                     push edx
 + 006aae5d  8d44241c               lea eax, [esp + 0x1c]
 + 006aae61  50                     push eax
   006aae67  56                     push esi
   006aae68  8bce                   mov ecx, esi
@@ retail 200-202 built 201-203
   006aae89  f6859100000001         test byte ptr [ebp + 0x91], 1
   006aae90  7551                   jne 0x6aaee3
 - 006aae92  8d44241c               lea eax, [esp + 0x1c]
 - 006aae96  50                     push eax
 + 006aae8d  8d4c241c               lea ecx, [esp + 0x1c]
 + 006aae91  51                     push ecx
   006aae97  55                     push ebp
   006aae98  56                     push esi
@@ retail 213-214 built 214-215
   006aaeae  f6c402                 test ah, 2
   006aaeb1  7430                   je 0x6aaee3
 - 006aaeb3  8d4c2418               lea ecx, [esp + 0x18]
 + 006aaeae  8d542418               lea edx, [esp + 0x18]
   006aaeb7  8d7d44                 lea edi, [ebp + 0x44]
   006aaeba  51                     push ecx
@@ retail 215-216 built 216-217
   006aaeb3  8d4c2418               lea ecx, [esp + 0x18]
   006aaeb7  8d7d44                 lea edi, [ebp + 0x44]
 - 006aaeba  51                     push ecx
 + 006aaeb5  52                     push edx
   006aaebb  8bcf                   mov ecx, edi
   006aaebd  c744241c09000000       mov dword ptr [esp + 0x1c], 9
@@ retail 231-232 built 232-233
   006aaee3  f6462840               test byte ptr [esi + 0x28], 0x40
   006aaee7  742a                   je 0x6aaf13
 - 006aaee9  8d542418               lea edx, [esp + 0x18]
 + 006aaee4  8d442418               lea eax, [esp + 0x18]
   006aaeed  8d7e44                 lea edi, [esi + 0x44]
   006aaef0  52                     push edx
@@ retail 233-234 built 234-235
   006aaee9  8d542418               lea edx, [esp + 0x18]
   006aaeed  8d7e44                 lea edi, [esi + 0x44]
 - 006aaef0  52                     push edx
 + 006aaeeb  50                     push eax
   006aaef1  8bcf                   mov ecx, edi
   006aaef3  c744241c46000000       mov dword ptr [esp + 0x1c], 0x46
@@ retail 255-255 built 256-257
   006aaf36  f6c501                 test ch, 1
   006aaf39  0f8499000000           je 0x6aafd8
 + 006aaf3a  8d4c2418               lea ecx, [esp + 0x18]
   006aaf3f  8d7844                 lea edi, [eax + 0x44]
   006aaf42  8d442418               lea eax, [esp + 0x18]
@@ retail 256-258 built 258-259
   006aaf39  0f8499000000           je 0x6aafd8
   006aaf3f  8d7844                 lea edi, [eax + 0x44]
 - 006aaf42  8d442418               lea eax, [esp + 0x18]
 - 006aaf46  50                     push eax
 + 006aaf41  51                     push ecx
   006aaf47  8bcf                   mov ecx, edi
   006aaf49  c744241c48000000       mov dword ptr [esp + 0x1c], 0x48
@@ retail 277-278 built 278-279
   006aaf78  8d442440               lea eax, [esp + 0x40]
   006aaf7c  50                     push eax
 - 006aaf7d  ff9218010000           call dword ptr [edx + 0x118]
 + 006aaf78  ff9214010000           call dword ptr [edx + 0x114]
   006aaf83  8b08                   mov ecx, dword ptr [eax]
   006aaf85  894c2428               mov dword ptr [esp + 0x28], ecx
@@ retail 284-284 built 285-288
   006aaf90  8b4008                 mov eax, dword ptr [eax + 8]
   006aaf93  89442430               mov dword ptr [esp + 0x30], eax
 + 006aaf92  d944242c               fld dword ptr [esp + 0x2c]
 + 006aaf96  c744243000000000       mov dword ptr [esp + 0x30], 0
 + 006aaf9e  d84c242c               fmul dword ptr [esp + 0x2c]
   006aaf97  d9442428               fld dword ptr [esp + 0x28]
   006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
@@ retail 285-286 built 289-289
   006aaf93  89442430               mov dword ptr [esp + 0x30], eax
   006aaf97  d9442428               fld dword ptr [esp + 0x28]
 - 006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
   006aafa3  d84c2428               fmul dword ptr [esp + 0x28]
   006aafa7  d944242c               fld dword ptr [esp + 0x2c]
@@ retail 287-289 built 290-290
   006aaf9b  c744243000000000       mov dword ptr [esp + 0x30], 0
   006aafa3  d84c2428               fmul dword ptr [esp + 0x28]
 - 006aafa7  d944242c               fld dword ptr [esp + 0x2c]
 - 006aafab  d84c242c               fmul dword ptr [esp + 0x2c]
   006aafaf  dec1                   faddp st(1)
   006aafb1  d9053cba2901           fld dword ptr [0x129ba3c]
DIFFLINES 74 retail_instr 380 built_instr 381
