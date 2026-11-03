00b6edf0 CEngineWaterRenderer::CEngineWaterRenderer::GenerateWaterMesh DIFFER(3114v3144) flags= []
@@ retail 0-1 built 0-1
 - 00b6edf0  83ec58                 sub esp, 0x58
 + 00b6edf0  83ec54                 sub esp, 0x54
   00b6edf3  53                     push ebx
   00b6edf4  55                     push ebp
@@ retail 5-5 built 5-6
   00b6edf5  56                     push esi
   00b6edf6  57                     push edi
 + 00b6edf7  33ff                   xor edi, edi
   00b6edf7  894c242c               mov dword ptr [esp + 0x2c], ecx
   00b6edfb  33c9                   xor ecx, ecx
@@ retail 8-14 built 9-15
   00b6edfb  33c9                   xor ecx, ecx
   00b6edfd  33c0                   xor eax, eax
 - 00b6edff  33ff                   xor edi, edi
 - 00b6ee01  33f6                   xor esi, esi
 - 00b6ee03  894c2444               mov dword ptr [esp + 0x44], ecx
 - 00b6ee07  89442448               mov dword ptr [esp + 0x48], eax
 - 00b6ee0b  897c244c               mov dword ptr [esp + 0x4c], edi
 - 00b6ee0f  89742420               mov dword ptr [esp + 0x20], esi
 + 00b6ee01  894c2440               mov dword ptr [esp + 0x40], ecx
 + 00b6ee05  89442444               mov dword ptr [esp + 0x44], eax
 + 00b6ee09  897c2448               mov dword ptr [esp + 0x48], edi
 + 00b6ee0d  897c2420               mov dword ptr [esp + 0x20], edi
 + 00b6ee11  8944241c               mov dword ptr [esp + 0x1c], eax
 + 00b6ee15  89442418               mov dword ptr [esp + 0x18], eax
   00b6ee13  eb0b                   jmp 0xb6ee20
   00b6ee15  8b742420               mov esi, dword ptr [esp + 0x20]
@@ retail 15-21 built 16-17
   00b6ee0f  89742420               mov dword ptr [esp + 0x20], esi
   00b6ee13  eb0b                   jmp 0xb6ee20
 - 00b6ee15  8b742420               mov esi, dword ptr [esp + 0x20]
 - 00b6ee19  8b7c244c               mov edi, dword ptr [esp + 0x4c]
 - 00b6ee1d  8d4900                 lea ecx, [ecx]
 - 00b6ee20  8bd6                   mov edx, esi
 - 00b6ee22  c1e204                 shl edx, 4
 - 00b6ee25  03d6                   add edx, esi
 + 00b6ee1b  8b7c2448               mov edi, dword ptr [esp + 0x48]
   00b6ee27  c744241c00000000       mov dword ptr [esp + 0x1c], 0
   00b6ee2f  89542430               mov dword ptr [esp + 0x30], edx
@@ retail 22-30 built 18-18
   00b6ee25  03d6                   add edx, esi
   00b6ee27  c744241c00000000       mov dword ptr [esp + 0x1c], 0
 - 00b6ee2f  89542430               mov dword ptr [esp + 0x30], edx
 - 00b6ee33  eb0b                   jmp 0xb6ee40
 - 00b6ee35  8b7c244c               mov edi, dword ptr [esp + 0x4c]
 - 00b6ee39  8da42400000000         lea esp, [esp]
 - 00b6ee40  8b54241c               mov edx, dword ptr [esp + 0x1c]
 - 00b6ee44  8b742430               mov esi, dword ptr [esp + 0x30]
 - 00b6ee48  03d6                   add edx, esi
 - 00b6ee4a  89542410               mov dword ptr [esp + 0x10], edx
   00b6ee4e  c744241800000000       mov dword ptr [esp + 0x18], 0
   00b6ee56  eb08                   jmp 0xb6ee60
@@ retail 32-38 built 20-33
   00b6ee4e  c744241800000000       mov dword ptr [esp + 0x18], 0
   00b6ee56  eb08                   jmp 0xb6ee60
 - 00b6ee58  8b7c244c               mov edi, dword ptr [esp + 0x4c]
 - 00b6ee5c  8d642400               lea esp, [esp]
 - 00b6ee60  8b542410               mov edx, dword ptr [esp + 0x10]
 - 00b6ee64  89542440               mov dword ptr [esp + 0x40], edx
 - 00b6ee68  8b542418               mov edx, dword ptr [esp + 0x18]
 - 00b6ee6c  83e201                 and edx, 1
 + 00b6ee31  8b7c2448               mov edi, dword ptr [esp + 0x48]
 + 00b6ee35  c744241800000000       mov dword ptr [esp + 0x18], 0
 + 00b6ee3d  eb04                   jmp 0xb6ee43
 + 00b6ee3f  8b7c2448               mov edi, dword ptr [esp + 0x48]
 + 00b6ee43  8b742418               mov esi, dword ptr [esp + 0x18]
 + 00b6ee47  8b542420               mov edx, dword ptr [esp + 0x20]
 + 00b6ee4b  8b6c241c               mov ebp, dword ptr [esp + 0x1c]
 + 00b6ee4f  03d6                   add edx, esi
 + 00b6ee51  8bda                   mov ebx, edx
 + 00b6ee53  c1e304                 shl ebx, 4
 + 00b6ee56  03dd                   add ebx, ebp
 + 00b6ee58  03d3                   add edx, ebx
 + 00b6ee5a  83e601                 and esi, 1
   00b6ee6f  c744241400000000       mov dword ptr [esp + 0x14], 0
   00b6ee77  89542434               mov dword ptr [esp + 0x34], edx
@@ retail 39-40 built 34-36
   00b6ee6c  83e201                 and edx, 1
   00b6ee6f  c744241400000000       mov dword ptr [esp + 0x14], 0
 - 00b6ee77  89542434               mov dword ptr [esp + 0x34], edx
 + 00b6ee65  89542430               mov dword ptr [esp + 0x30], edx
 + 00b6ee69  89742434               mov dword ptr [esp + 0x34], esi
   00b6ee7b  eb04                   jmp 0xb6ee81
   00b6ee7d  8b7c244c               mov edi, dword ptr [esp + 0x4c]
@@ retail 41-44 built 37-43
   00b6ee77  89542434               mov dword ptr [esp + 0x34], edx
   00b6ee7b  eb04                   jmp 0xb6ee81
 - 00b6ee7d  8b7c244c               mov edi, dword ptr [esp + 0x4c]
 - 00b6ee81  8b742414               mov esi, dword ptr [esp + 0x14]
 - 00b6ee85  8b5c2434               mov ebx, dword ptr [esp + 0x34]
 + 00b6ee6f  8b7c2448               mov edi, dword ptr [esp + 0x48]
 + 00b6ee73  8b542414               mov edx, dword ptr [esp + 0x14]
 + 00b6ee77  8b742430               mov esi, dword ptr [esp + 0x30]
 + 00b6ee7b  8b6c2434               mov ebp, dword ptr [esp + 0x34]
 + 00b6ee7f  03f2                   add esi, edx
 + 00b6ee81  8bda                   mov ebx, edx
   00b6ee89  ba01000000             mov edx, 1
   00b6ee8e  23f2                   and esi, edx
@@ retail 45-47 built 44-47
   00b6ee85  8b5c2434               mov ebx, dword ptr [esp + 0x34]
   00b6ee89  ba01000000             mov edx, 1
 - 00b6ee8e  23f2                   and esi, edx
 - 00b6ee90  3bde                   cmp ebx, esi
 + 00b6ee88  23da                   and ebx, edx
 + 00b6ee8a  3beb                   cmp ebp, ebx
 + 00b6ee8c  89742410               mov dword ptr [esp + 0x10], esi
   00b6ee92  0f85a4030000           jne 0xb6f23c
   00b6ee98  3bc7                   cmp eax, edi
@@ retail 52-56 built 52-55
   00b6ee9c  85c0                   test eax, eax
   00b6ee9e  7410                   je 0xb6eeb0
 - 00b6eea0  668b4c2410             mov cx, word ptr [esp + 0x10]
 - 00b6eea5  668908                 mov word ptr [eax], cx
 - 00b6eea8  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6eeac  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6ee9e  668930                 mov word ptr [eax], si
 + 00b6eea1  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6eea5  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6eeb0  83c002                 add eax, 2
   00b6eeb3  eb78                   jmp 0xb6ef2d
@@ retail 70-72 built 69-71
   00b6eed1  03d8                   add ebx, eax
   00b6eed3  7414                   je 0xb6eee9
 - 00b6eed5  8d141b                 lea edx, [ebx + ebx]
 - 00b6eed8  52                     push edx
 + 00b6eed1  8d041b                 lea eax, [ebx + ebx]
 + 00b6eed4  50                     push eax
   00b6eed9  e830fb0800             call 0xbfea0e
   00b6eede  8b4c2448               mov ecx, dword ptr [esp + 0x48]
@@ retail 73-74 built 72-73
   00b6eed8  52                     push edx
   00b6eed9  e830fb0800             call 0xbfea0e
 - 00b6eede  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6eeda  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6eee2  83c404                 add esp, 4
   00b6eee5  8bf8                   mov edi, eax
@@ retail 92-93 built 91-92
   00b6ef07  668908                 mov word ptr [eax], cx
   00b6ef0a  8d7002                 lea esi, [eax + 2]
 - 00b6ef0d  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6ef09  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6ef11  85c0                   test eax, eax
   00b6ef13  7409                   je 0xb6ef1e
@@ retail 98-98 built 97-98
   00b6ef16  e8f9fa0800             call 0xbfea14
   00b6ef1b  83c404                 add esp, 4
 + 00b6ef1a  8d145f                 lea edx, [edi + ebx*2]
   00b6ef1e  8bcf                   mov ecx, edi
   00b6ef20  8d145f                 lea edx, [edi + ebx*2]
@@ retail 99-101 built 99-99
   00b6ef1b  83c404                 add esp, 4
   00b6ef1e  8bcf                   mov ecx, edi
 - 00b6ef20  8d145f                 lea edx, [edi + ebx*2]
 - 00b6ef23  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6ef27  8bc6                   mov eax, esi
   00b6ef29  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 102-108 built 100-107
   00b6ef23  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6ef27  8bc6                   mov eax, esi
 - 00b6ef29  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b6ef2d  8b6c2410               mov ebp, dword ptr [esp + 0x10]
 - 00b6ef31  8b54244c               mov edx, dword ptr [esp + 0x4c]
 - 00b6ef35  83c512                 add ebp, 0x12
 - 00b6ef38  3bc2                   cmp eax, edx
 - 00b6ef3a  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6ef21  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00b6ef25  89542448               mov dword ptr [esp + 0x48], edx
 + 00b6ef29  894c2440               mov dword ptr [esp + 0x40], ecx
 + 00b6ef2d  ba01000000             mov edx, 1
 + 00b6ef32  8d6e12                 lea ebp, [esi + 0x12]
 + 00b6ef35  3b442448               cmp eax, dword ptr [esp + 0x48]
 + 00b6ef39  89442444               mov dword ptr [esp + 0x44], eax
   00b6ef3e  896c2428               mov dword ptr [esp + 0x28], ebp
   00b6ef42  7414                   je 0xb6ef58
@@ retail 113-115 built 112-114
   00b6ef46  740b                   je 0xb6ef53
   00b6ef48  668928                 mov word ptr [eax], bp
 - 00b6ef4b  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6ef4f  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6ef4a  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6ef4e  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6ef53  83c002                 add eax, 2
   00b6ef56  eb78                   jmp 0xb6efd0
@@ retail 120-122 built 119-121
   00b6ef5a  2bc1                   sub eax, ecx
   00b6ef5c  d1f8                   sar eax, 1
 - 00b6ef5e  83f801                 cmp eax, 1
 - 00b6ef61  c744243801000000       mov dword ptr [esp + 0x38], 1
 + 00b6ef5d  89542438               mov dword ptr [esp + 0x38], edx
 + 00b6ef61  3bc2                   cmp eax, edx
   00b6ef69  8944243c               mov dword ptr [esp + 0x3c], eax
   00b6ef6d  8d542438               lea edx, [esp + 0x38]
@@ retail 132-133 built 131-132
   00b6ef80  50                     push eax
   00b6ef81  e888fa0800             call 0xbfea0e
 - 00b6ef86  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6ef80  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6ef8a  83c404                 add esp, 4
   00b6ef8d  8bf8                   mov edi, eax
@@ retail 150-151 built 149-150
   00b6efaa  668928                 mov word ptr [eax], bp
   00b6efad  8d7002                 lea esi, [eax + 2]
 - 00b6efb0  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6efaa  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6efb4  85c0                   test eax, eax
   00b6efb6  7409                   je 0xb6efc1
@@ retail 158-159 built 157-158
   00b6efc1  8bcf                   mov ecx, edi
   00b6efc3  8d145f                 lea edx, [edi + ebx*2]
 - 00b6efc6  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b6efc0  894c2440               mov dword ptr [esp + 0x40], ecx
   00b6efca  8bc6                   mov eax, esi
   00b6efcc  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 160-161 built 159-160
   00b6efc6  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6efca  8bc6                   mov eax, esi
 - 00b6efcc  8954244c               mov dword ptr [esp + 0x4c], edx
 + 00b6efc6  89542448               mov dword ptr [esp + 0x48], edx
   00b6efd0  8b6c2410               mov ebp, dword ptr [esp + 0x10]
   00b6efd4  8b54244c               mov edx, dword ptr [esp + 0x4c]
@@ retail 162-163 built 161-162
   00b6efcc  8954244c               mov dword ptr [esp + 0x4c], edx
   00b6efd0  8b6c2410               mov ebp, dword ptr [esp + 0x10]
 - 00b6efd4  8b54244c               mov edx, dword ptr [esp + 0x4c]
 + 00b6efce  8b542448               mov edx, dword ptr [esp + 0x48]
   00b6efd8  83c511                 add ebp, 0x11
   00b6efdb  3bc2                   cmp eax, edx
@@ retail 165-166 built 164-165
   00b6efd8  83c511                 add ebp, 0x11
   00b6efdb  3bc2                   cmp eax, edx
 - 00b6efdd  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6efd7  89442444               mov dword ptr [esp + 0x44], eax
   00b6efe1  7414                   je 0xb6eff7
   00b6efe3  85c0                   test eax, eax
@@ retail 170-172 built 169-171
   00b6efe5  740b                   je 0xb6eff2
   00b6efe7  668928                 mov word ptr [eax], bp
 - 00b6efea  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6efee  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6efe4  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6efe8  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6eff2  83c002                 add eax, 2
   00b6eff5  eb78                   jmp 0xb6f06f
@@ retail 189-190 built 188-189
   00b6f01f  50                     push eax
   00b6f020  e8e9f90800             call 0xbfea0e
 - 00b6f025  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f01f  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f029  83c404                 add esp, 4
   00b6f02c  8bf8                   mov edi, eax
@@ retail 207-208 built 206-207
   00b6f049  668928                 mov word ptr [eax], bp
   00b6f04c  8d7002                 lea esi, [eax + 2]
 - 00b6f04f  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f049  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f053  85c0                   test eax, eax
   00b6f055  7409                   je 0xb6f060
@@ retail 215-216 built 214-215
   00b6f060  8bcf                   mov ecx, edi
   00b6f062  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f065  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b6f05f  894c2440               mov dword ptr [esp + 0x40], ecx
   00b6f069  8bc6                   mov eax, esi
   00b6f06b  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 217-220 built 216-219
   00b6f065  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f069  8bc6                   mov eax, esi
 - 00b6f06b  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b6f06f  3b44244c               cmp eax, dword ptr [esp + 0x4c]
 - 00b6f073  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6f065  89542448               mov dword ptr [esp + 0x48], edx
 + 00b6f069  3b442448               cmp eax, dword ptr [esp + 0x48]
 + 00b6f06d  89442444               mov dword ptr [esp + 0x44], eax
   00b6f077  7419                   je 0xb6f092
   00b6f079  85c0                   test eax, eax
@@ retail 225-227 built 224-226
   00b6f07d  668b4c2410             mov cx, word ptr [esp + 0x10]
   00b6f082  668908                 mov word ptr [eax], cx
 - 00b6f085  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f089  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f07f  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f083  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f08d  83c002                 add eax, 2
   00b6f090  eb7d                   jmp 0xb6f10f
@@ retail 244-245 built 243-244
   00b6f0ba  52                     push edx
   00b6f0bb  e84ef90800             call 0xbfea0e
 - 00b6f0c0  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f0ba  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f0c4  83c404                 add esp, 4
   00b6f0c7  8bf8                   mov edi, eax
@@ retail 263-264 built 262-263
   00b6f0e9  668908                 mov word ptr [eax], cx
   00b6f0ec  8d7002                 lea esi, [eax + 2]
 - 00b6f0ef  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f0e9  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f0f3  85c0                   test eax, eax
   00b6f0f5  7409                   je 0xb6f100
@@ retail 271-272 built 270-271
   00b6f100  8bcf                   mov ecx, edi
   00b6f102  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f105  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b6f0ff  894c2440               mov dword ptr [esp + 0x40], ecx
   00b6f109  8bc6                   mov eax, esi
   00b6f10b  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 273-274 built 272-273
   00b6f105  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f109  8bc6                   mov eax, esi
 - 00b6f10b  8954244c               mov dword ptr [esp + 0x4c], edx
 + 00b6f105  89542448               mov dword ptr [esp + 0x48], edx
   00b6f10f  8b6c2410               mov ebp, dword ptr [esp + 0x10]
   00b6f113  8b54244c               mov edx, dword ptr [esp + 0x4c]
@@ retail 275-276 built 274-275
   00b6f10b  8954244c               mov dword ptr [esp + 0x4c], edx
   00b6f10f  8b6c2410               mov ebp, dword ptr [esp + 0x10]
 - 00b6f113  8b54244c               mov edx, dword ptr [esp + 0x4c]
 + 00b6f10d  8b542448               mov edx, dword ptr [esp + 0x48]
   00b6f117  45                     inc ebp
   00b6f118  3bc2                   cmp eax, edx
@@ retail 278-279 built 277-278
   00b6f117  45                     inc ebp
   00b6f118  3bc2                   cmp eax, edx
 - 00b6f11a  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6f114  89442444               mov dword ptr [esp + 0x44], eax
   00b6f11e  7414                   je 0xb6f134
   00b6f120  85c0                   test eax, eax
@@ retail 283-285 built 282-284
   00b6f122  740b                   je 0xb6f12f
   00b6f124  668928                 mov word ptr [eax], bp
 - 00b6f127  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f12b  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f121  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f125  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f12f  83c002                 add eax, 2
   00b6f132  eb78                   jmp 0xb6f1ac
@@ retail 302-303 built 301-302
   00b6f15c  50                     push eax
   00b6f15d  e8acf80800             call 0xbfea0e
 - 00b6f162  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f15c  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f166  83c404                 add esp, 4
   00b6f169  8bf8                   mov edi, eax
@@ retail 320-321 built 319-320
   00b6f186  668928                 mov word ptr [eax], bp
   00b6f189  8d7002                 lea esi, [eax + 2]
 - 00b6f18c  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f186  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f190  85c0                   test eax, eax
   00b6f192  7409                   je 0xb6f19d
@@ retail 328-329 built 327-328
   00b6f19d  8bcf                   mov ecx, edi
   00b6f19f  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f1a2  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b6f19c  894c2440               mov dword ptr [esp + 0x40], ecx
   00b6f1a6  8bc6                   mov eax, esi
   00b6f1a8  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 330-333 built 329-332
   00b6f1a2  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f1a6  8bc6                   mov eax, esi
 - 00b6f1a8  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b6f1ac  3b44244c               cmp eax, dword ptr [esp + 0x4c]
 - 00b6f1b0  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6f1a2  89542448               mov dword ptr [esp + 0x48], edx
 + 00b6f1a6  3b442448               cmp eax, dword ptr [esp + 0x48]
 + 00b6f1aa  89442444               mov dword ptr [esp + 0x44], eax
   00b6f1b4  741c                   je 0xb6f1d2
   00b6f1b6  85c0                   test eax, eax
@@ retail 338-340 built 337-339
   00b6f1ba  668b4c2428             mov cx, word ptr [esp + 0x28]
   00b6f1bf  668908                 mov word ptr [eax], cx
 - 00b6f1c2  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f1c6  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f1bc  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f1c0  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f1ca  83c002                 add eax, 2
   00b6f1cd  e914040000             jmp 0xb6f5e6
@@ retail 357-358 built 356-357
   00b6f1fa  52                     push edx
   00b6f1fb  e80ef80800             call 0xbfea0e
 - 00b6f200  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f1fa  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f204  83c404                 add esp, 4
   00b6f207  8bf8                   mov edi, eax
@@ retail 382-386 built 381-384
   00b6f240  85c0                   test eax, eax
   00b6f242  7410                   je 0xb6f254
 - 00b6f244  668b4c2410             mov cx, word ptr [esp + 0x10]
 - 00b6f249  668908                 mov word ptr [eax], cx
 - 00b6f24c  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f250  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f23e  668930                 mov word ptr [eax], si
 + 00b6f241  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f245  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f254  83c002                 add eax, 2
   00b6f257  eb78                   jmp 0xb6f2d1
@@ retail 400-402 built 398-400
   00b6f275  03d8                   add ebx, eax
   00b6f277  7414                   je 0xb6f28d
 - 00b6f279  8d141b                 lea edx, [ebx + ebx]
 - 00b6f27c  52                     push edx
 + 00b6f271  8d041b                 lea eax, [ebx + ebx]
 + 00b6f274  50                     push eax
   00b6f27d  e88cf70800             call 0xbfea0e
   00b6f282  8b4c2448               mov ecx, dword ptr [esp + 0x48]
@@ retail 403-404 built 401-402
   00b6f27c  52                     push edx
   00b6f27d  e88cf70800             call 0xbfea0e
 - 00b6f282  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f27a  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f286  83c404                 add esp, 4
   00b6f289  8bf8                   mov edi, eax
@@ retail 422-423 built 420-421
   00b6f2ab  668908                 mov word ptr [eax], cx
   00b6f2ae  8d7002                 lea esi, [eax + 2]
 - 00b6f2b1  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f2a9  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f2b5  85c0                   test eax, eax
   00b6f2b7  7409                   je 0xb6f2c2
@@ retail 428-428 built 426-427
   00b6f2ba  e855f70800             call 0xbfea14
   00b6f2bf  83c404                 add esp, 4
 + 00b6f2ba  8d145f                 lea edx, [edi + ebx*2]
   00b6f2c2  8bcf                   mov ecx, edi
   00b6f2c4  8d145f                 lea edx, [edi + ebx*2]
@@ retail 429-431 built 428-428
   00b6f2bf  83c404                 add esp, 4
   00b6f2c2  8bcf                   mov ecx, edi
 - 00b6f2c4  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f2c7  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f2cb  8bc6                   mov eax, esi
   00b6f2cd  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 432-438 built 429-436
   00b6f2c7  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f2cb  8bc6                   mov eax, esi
 - 00b6f2cd  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b6f2d1  8b6c2410               mov ebp, dword ptr [esp + 0x10]
 - 00b6f2d5  8b54244c               mov edx, dword ptr [esp + 0x4c]
 - 00b6f2d9  45                     inc ebp
 - 00b6f2da  3bc2                   cmp eax, edx
 - 00b6f2dc  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6f2c1  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00b6f2c5  89542448               mov dword ptr [esp + 0x48], edx
 + 00b6f2c9  894c2440               mov dword ptr [esp + 0x40], ecx
 + 00b6f2cd  ba01000000             mov edx, 1
 + 00b6f2d2  3b442448               cmp eax, dword ptr [esp + 0x48]
 + 00b6f2d6  8d6e01                 lea ebp, [esi + 1]
 + 00b6f2d9  89442444               mov dword ptr [esp + 0x44], eax
   00b6f2e0  896c2428               mov dword ptr [esp + 0x28], ebp
   00b6f2e4  7414                   je 0xb6f2fa
@@ retail 443-445 built 441-443
   00b6f2e8  740b                   je 0xb6f2f5
   00b6f2ea  668928                 mov word ptr [eax], bp
 - 00b6f2ed  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f2f1  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f2ea  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f2ee  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f2f5  83c002                 add eax, 2
   00b6f2f8  eb78                   jmp 0xb6f372
@@ retail 450-452 built 448-450
   00b6f2fc  2bc1                   sub eax, ecx
   00b6f2fe  d1f8                   sar eax, 1
 - 00b6f300  83f801                 cmp eax, 1
 - 00b6f303  c744243c01000000       mov dword ptr [esp + 0x3c], 1
 + 00b6f2fd  8954243c               mov dword ptr [esp + 0x3c], edx
 + 00b6f301  3bc2                   cmp eax, edx
   00b6f30b  89442438               mov dword ptr [esp + 0x38], eax
   00b6f30f  8d54243c               lea edx, [esp + 0x3c]
@@ retail 462-463 built 460-461
   00b6f322  50                     push eax
   00b6f323  e8e6f60800             call 0xbfea0e
 - 00b6f328  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f320  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f32c  83c404                 add esp, 4
   00b6f32f  8bf8                   mov edi, eax
@@ retail 480-481 built 478-479
   00b6f34c  668928                 mov word ptr [eax], bp
   00b6f34f  8d7002                 lea esi, [eax + 2]
 - 00b6f352  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f34a  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f356  85c0                   test eax, eax
   00b6f358  7409                   je 0xb6f363
@@ retail 486-486 built 484-485
   00b6f35b  e8b4f60800             call 0xbfea14
   00b6f360  83c404                 add esp, 4
 + 00b6f35b  8d145f                 lea edx, [edi + ebx*2]
   00b6f363  8bcf                   mov ecx, edi
   00b6f365  8d145f                 lea edx, [edi + ebx*2]
@@ retail 487-489 built 486-486
   00b6f360  83c404                 add esp, 4
   00b6f363  8bcf                   mov ecx, edi
 - 00b6f365  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f368  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f36c  8bc6                   mov eax, esi
   00b6f36e  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 490-496 built 487-494
   00b6f368  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f36c  8bc6                   mov eax, esi
 - 00b6f36e  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b6f372  8b6c2410               mov ebp, dword ptr [esp + 0x10]
 - 00b6f376  8b54244c               mov edx, dword ptr [esp + 0x4c]
 - 00b6f37a  83c511                 add ebp, 0x11
 - 00b6f37d  3bc2                   cmp eax, edx
 - 00b6f37f  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6f362  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00b6f366  89542448               mov dword ptr [esp + 0x48], edx
 + 00b6f36a  894c2440               mov dword ptr [esp + 0x40], ecx
 + 00b6f36e  ba01000000             mov edx, 1
 + 00b6f373  8d6e11                 lea ebp, [esi + 0x11]
 + 00b6f376  3b442448               cmp eax, dword ptr [esp + 0x48]
 + 00b6f37a  89442444               mov dword ptr [esp + 0x44], eax
   00b6f383  7414                   je 0xb6f399
   00b6f385  85c0                   test eax, eax
@@ retail 500-502 built 498-500
   00b6f387  740b                   je 0xb6f394
   00b6f389  668928                 mov word ptr [eax], bp
 - 00b6f38c  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f390  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f387  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f38b  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f394  83c002                 add eax, 2
   00b6f397  eb78                   jmp 0xb6f411
@@ retail 507-509 built 505-507
   00b6f39b  2bc1                   sub eax, ecx
   00b6f39d  d1f8                   sar eax, 1
 - 00b6f39f  83f801                 cmp eax, 1
 - 00b6f3a2  c744243c01000000       mov dword ptr [esp + 0x3c], 1
 + 00b6f39a  8954243c               mov dword ptr [esp + 0x3c], edx
 + 00b6f39e  3bc2                   cmp eax, edx
   00b6f3aa  89442438               mov dword ptr [esp + 0x38], eax
   00b6f3ae  8d54243c               lea edx, [esp + 0x3c]
@@ retail 519-520 built 517-518
   00b6f3c1  50                     push eax
   00b6f3c2  e847f60800             call 0xbfea0e
 - 00b6f3c7  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f3bd  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f3cb  83c404                 add esp, 4
   00b6f3ce  8bf8                   mov edi, eax
@@ retail 537-538 built 535-536
   00b6f3eb  668928                 mov word ptr [eax], bp
   00b6f3ee  8d7002                 lea esi, [eax + 2]
 - 00b6f3f1  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f3e7  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f3f5  85c0                   test eax, eax
   00b6f3f7  7409                   je 0xb6f402
@@ retail 543-543 built 541-542
   00b6f3fa  e815f60800             call 0xbfea14
   00b6f3ff  83c404                 add esp, 4
 + 00b6f3f8  8d145f                 lea edx, [edi + ebx*2]
   00b6f402  8bcf                   mov ecx, edi
   00b6f404  8d145f                 lea edx, [edi + ebx*2]
@@ retail 544-546 built 543-545
   00b6f3ff  83c404                 add esp, 4
   00b6f402  8bcf                   mov ecx, edi
 - 00b6f404  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f407  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b6f3fd  89542448               mov dword ptr [esp + 0x48], edx
 + 00b6f401  894c2440               mov dword ptr [esp + 0x40], ecx
   00b6f40b  8bc6                   mov eax, esi
   00b6f40d  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 547-550 built 546-549
   00b6f407  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f40b  8bc6                   mov eax, esi
 - 00b6f40d  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b6f411  3b44244c               cmp eax, dword ptr [esp + 0x4c]
 - 00b6f415  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6f407  ba01000000             mov edx, 1
 + 00b6f40c  3b442448               cmp eax, dword ptr [esp + 0x48]
 + 00b6f410  89442444               mov dword ptr [esp + 0x44], eax
   00b6f419  7414                   je 0xb6f42f
   00b6f41b  85c0                   test eax, eax
@@ retail 554-556 built 553-555
   00b6f41d  740b                   je 0xb6f42a
   00b6f41f  668928                 mov word ptr [eax], bp
 - 00b6f422  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f426  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f41d  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f421  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f42a  83c002                 add eax, 2
   00b6f42d  eb78                   jmp 0xb6f4a7
@@ retail 561-563 built 560-562
   00b6f431  2bc1                   sub eax, ecx
   00b6f433  d1f8                   sar eax, 1
 - 00b6f435  83f801                 cmp eax, 1
 - 00b6f438  c744243c01000000       mov dword ptr [esp + 0x3c], 1
 + 00b6f430  8954243c               mov dword ptr [esp + 0x3c], edx
 + 00b6f434  3bc2                   cmp eax, edx
   00b6f440  89442438               mov dword ptr [esp + 0x38], eax
   00b6f444  8d54243c               lea edx, [esp + 0x3c]
@@ retail 573-574 built 572-573
   00b6f457  50                     push eax
   00b6f458  e8b1f50800             call 0xbfea0e
 - 00b6f45d  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f453  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f461  83c404                 add esp, 4
   00b6f464  8bf8                   mov edi, eax
@@ retail 591-592 built 590-591
   00b6f481  668928                 mov word ptr [eax], bp
   00b6f484  8d7002                 lea esi, [eax + 2]
 - 00b6f487  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f47d  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f48b  85c0                   test eax, eax
   00b6f48d  7409                   je 0xb6f498
@@ retail 597-597 built 596-597
   00b6f490  e87ff50800             call 0xbfea14
   00b6f495  83c404                 add esp, 4
 + 00b6f48e  8d145f                 lea edx, [edi + ebx*2]
   00b6f498  8bcf                   mov ecx, edi
   00b6f49a  8d145f                 lea edx, [edi + ebx*2]
@@ retail 598-600 built 598-600
   00b6f495  83c404                 add esp, 4
   00b6f498  8bcf                   mov ecx, edi
 - 00b6f49a  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f49d  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b6f493  89542448               mov dword ptr [esp + 0x48], edx
 + 00b6f497  894c2440               mov dword ptr [esp + 0x40], ecx
   00b6f4a1  8bc6                   mov eax, esi
   00b6f4a3  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 601-604 built 601-604
   00b6f49d  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f4a1  8bc6                   mov eax, esi
 - 00b6f4a3  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b6f4a7  3b44244c               cmp eax, dword ptr [esp + 0x4c]
 - 00b6f4ab  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6f49d  ba01000000             mov edx, 1
 + 00b6f4a2  3b442448               cmp eax, dword ptr [esp + 0x48]
 + 00b6f4a6  89442444               mov dword ptr [esp + 0x44], eax
   00b6f4af  7419                   je 0xb6f4ca
   00b6f4b1  85c0                   test eax, eax
@@ retail 609-611 built 609-611
   00b6f4b5  668b4c2428             mov cx, word ptr [esp + 0x28]
   00b6f4ba  668908                 mov word ptr [eax], cx
 - 00b6f4bd  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f4c1  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f4b8  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f4bc  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f4c5  83c002                 add eax, 2
   00b6f4c8  eb7d                   jmp 0xb6f547
@@ retail 616-618 built 616-618
   00b6f4cc  2bc1                   sub eax, ecx
   00b6f4ce  d1f8                   sar eax, 1
 - 00b6f4d0  83f801                 cmp eax, 1
 - 00b6f4d3  c744243c01000000       mov dword ptr [esp + 0x3c], 1
 + 00b6f4cb  8954243c               mov dword ptr [esp + 0x3c], edx
 + 00b6f4cf  3bc2                   cmp eax, edx
   00b6f4db  89442438               mov dword ptr [esp + 0x38], eax
   00b6f4df  8d54243c               lea edx, [esp + 0x3c]
@@ retail 628-629 built 628-629
   00b6f4f2  52                     push edx
   00b6f4f3  e816f50800             call 0xbfea0e
 - 00b6f4f8  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f4ee  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f4fc  83c404                 add esp, 4
   00b6f4ff  8bf8                   mov edi, eax
@@ retail 647-648 built 647-648
   00b6f521  668908                 mov word ptr [eax], cx
   00b6f524  8d7002                 lea esi, [eax + 2]
 - 00b6f527  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f51d  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f52b  85c0                   test eax, eax
   00b6f52d  7409                   je 0xb6f538
@@ retail 655-656 built 655-656
   00b6f538  8bcf                   mov ecx, edi
   00b6f53a  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f53d  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b6f533  894c2440               mov dword ptr [esp + 0x40], ecx
   00b6f541  8bc6                   mov eax, esi
   00b6f543  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 657-658 built 657-658
   00b6f53d  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f541  8bc6                   mov eax, esi
 - 00b6f543  8954244c               mov dword ptr [esp + 0x4c], edx
 + 00b6f539  89542448               mov dword ptr [esp + 0x48], edx
   00b6f547  8b6c2410               mov ebp, dword ptr [esp + 0x10]
   00b6f54b  8b54244c               mov edx, dword ptr [esp + 0x4c]
@@ retail 659-660 built 659-660
   00b6f543  8954244c               mov dword ptr [esp + 0x4c], edx
   00b6f547  8b6c2410               mov ebp, dword ptr [esp + 0x10]
 - 00b6f54b  8b54244c               mov edx, dword ptr [esp + 0x4c]
 + 00b6f541  8b542448               mov edx, dword ptr [esp + 0x48]
   00b6f54f  83c512                 add ebp, 0x12
   00b6f552  3bc2                   cmp eax, edx
@@ retail 662-663 built 662-663
   00b6f54f  83c512                 add ebp, 0x12
   00b6f552  3bc2                   cmp eax, edx
 - 00b6f554  89442448               mov dword ptr [esp + 0x48], eax
 + 00b6f54a  89442444               mov dword ptr [esp + 0x44], eax
   00b6f558  7414                   je 0xb6f56e
   00b6f55a  85c0                   test eax, eax
@@ retail 667-669 built 667-669
   00b6f55c  740b                   je 0xb6f569
   00b6f55e  668928                 mov word ptr [eax], bp
 - 00b6f561  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00b6f565  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00b6f557  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f55b  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b6f569  83c002                 add eax, 2
   00b6f56c  eb78                   jmp 0xb6f5e6
@@ retail 686-687 built 686-687
   00b6f596  50                     push eax
   00b6f597  e872f40800             call 0xbfea0e
 - 00b6f59c  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00b6f592  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b6f5a0  83c404                 add esp, 4
   00b6f5a3  8bf8                   mov edi, eax
@@ retail 704-705 built 704-705
   00b6f5c0  668928                 mov word ptr [eax], bp
   00b6f5c3  8d7002                 lea esi, [eax + 2]
 - 00b6f5c6  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b6f5bc  8b442440               mov eax, dword ptr [esp + 0x40]
   00b6f5ca  85c0                   test eax, eax
   00b6f5cc  7409                   je 0xb6f5d7
@@ retail 712-713 built 712-713
   00b6f5d7  8bcf                   mov ecx, edi
   00b6f5d9  8d145f                 lea edx, [edi + ebx*2]
 - 00b6f5dc  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b6f5d2  894c2440               mov dword ptr [esp + 0x40], ecx
   00b6f5e0  8bc6                   mov eax, esi
   00b6f5e2  8954244c               mov dword ptr [esp + 0x4c], edx
@@ retail 714-716 built 714-715
   00b6f5dc  894c2444               mov dword ptr [esp + 0x44], ecx
   00b6f5e0  8bc6                   mov eax, esi
 - 00b6f5e2  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b6f5e6  8b5c2410               mov ebx, dword ptr [esp + 0x10]
 + 00b6f5d8  89542448               mov dword ptr [esp + 0x48], edx
   00b6f5ea  8b542414               mov edx, dword ptr [esp + 0x14]
   00b6f5ee  43                     inc ebx
@@ retail 717-718 built 716-716
   00b6f5e6  8b5c2410               mov ebx, dword ptr [esp + 0x10]
   00b6f5ea  8b542414               mov edx, dword ptr [esp + 0x14]
 - 00b6f5ee  43                     inc ebx
   00b6f5ef  42                     inc edx
   00b6f5f0  83fa02                 cmp edx, 2
@@ retail 720-722 built 718-719
   00b6f5ef  42                     inc edx
   00b6f5f0  83fa02                 cmp edx, 2
 - 00b6f5f3  89442448               mov dword ptr [esp + 0x48], eax
 - 00b6f5f7  895c2410               mov dword ptr [esp + 0x10], ebx
 + 00b6f5e4  89442444               mov dword ptr [esp + 0x44], eax
   00b6f5fb  89542414               mov dword ptr [esp + 0x14], edx
   00b6f5ff  0f8c78f8ffff           jl 0xb6ee7d
@@ retail 724-727 built 721-721
   00b6f5fb  89542414               mov dword ptr [esp + 0x14], edx
   00b6f5ff  0f8c78f8ffff           jl 0xb6ee7d
 - 00b6f605  8b542440               mov edx, dword ptr [esp + 0x40]
 - 00b6f609  83c211                 add edx, 0x11
 - 00b6f60c  89542410               mov dword ptr [esp + 0x10], edx
   00b6f610  8b542418               mov edx, dword ptr [esp + 0x18]
   00b6f614  42                     inc edx
@@ retail 744-744 built 738-739
   00b6f64a  33ff                   xor edi, edi
   00b6f64c  8d4c2424               lea ecx, [esp + 0x24]
 + 00b6f632  897c2458               mov dword ptr [esp + 0x58], edi
   00b6f650  897c245c               mov dword ptr [esp + 0x5c], edi
   00b6f654  897c2460               mov dword ptr [esp + 0x60], edi
@@ retail 746-747 built 741-742
   00b6f650  897c245c               mov dword ptr [esp + 0x5c], edi
   00b6f654  897c2460               mov dword ptr [esp + 0x60], edi
 - 00b6f658  897c2464               mov dword ptr [esp + 0x64], edi
 + 00b6f63e  897c244c               mov dword ptr [esp + 0x4c], edi
   00b6f65c  897c2450               mov dword ptr [esp + 0x50], edi
   00b6f660  897c2454               mov dword ptr [esp + 0x54], edi
@@ retail 749-750 built 744-744
   00b6f65c  897c2450               mov dword ptr [esp + 0x50], edi
   00b6f660  897c2454               mov dword ptr [esp + 0x54], edi
 - 00b6f664  897c2458               mov dword ptr [esp + 0x58], edi
   00b6f668  e883ace2ff             call 0x99a2f0
   00b6f66d  8d442450               lea eax, [esp + 0x50]
@@ retail 751-752 built 745-746
   00b6f664  897c2458               mov dword ptr [esp + 0x58], edi
   00b6f668  e883ace2ff             call 0x99a2f0
 - 00b6f66d  8d442450               lea eax, [esp + 0x50]
 + 00b6f64f  8d44244c               lea eax, [esp + 0x4c]
   00b6f671  50                     push eax
   00b6f672  8d4c2460               lea ecx, [esp + 0x60]
@@ retail 753-754 built 747-748
   00b6f66d  8d442450               lea eax, [esp + 0x50]
   00b6f671  50                     push eax
 - 00b6f672  8d4c2460               lea ecx, [esp + 0x60]
 + 00b6f654  8d4c245c               lea ecx, [esp + 0x5c]
   00b6f676  51                     push ecx
   00b6f677  8d54244c               lea edx, [esp + 0x4c]
@@ retail 755-756 built 749-750
   00b6f672  8d4c2460               lea ecx, [esp + 0x60]
   00b6f676  51                     push ecx
 - 00b6f677  8d54244c               lea edx, [esp + 0x4c]
 + 00b6f659  8d542448               lea edx, [esp + 0x48]
   00b6f67b  52                     push edx
   00b6f67c  8d4c2430               lea ecx, [esp + 0x30]
@@ retail 760-762 built 754-756
   00b6f680  c74424301c332a01       mov dword ptr [esp + 0x30], 0x12a331c
   00b6f688  e8c3f33c00             call 0xf3ea50
 - 00b6f68d  8b6c2450               mov ebp, dword ptr [esp + 0x50]
 - 00b6f691  8b5c2454               mov ebx, dword ptr [esp + 0x54]
 + 00b6f66f  8b6c244c               mov ebp, dword ptr [esp + 0x4c]
 + 00b6f673  8b5c2450               mov ebx, dword ptr [esp + 0x50]
   00b6f695  2bdd                   sub ebx, ebp
   00b6f697  8b6c242c               mov ebp, dword ptr [esp + 0x2c]
@@ retail 785-787 built 779-781
   00b6f6d1  8bcd                   mov ecx, ebp
   00b6f6d3  e8d828f5ff             call 0xac1fb0
 - 00b6f6d8  8b4c2454               mov ecx, dword ptr [esp + 0x54]
 - 00b6f6dc  8b542450               mov edx, dword ptr [esp + 0x50]
 + 00b6f6ba  8b4c2450               mov ecx, dword ptr [esp + 0x50]
 + 00b6f6be  8b54244c               mov edx, dword ptr [esp + 0x4c]
   00b6f6e0  2bca                   sub ecx, edx
   00b6f6e2  d1f9                   sar ecx, 1
@@ retail 791-792 built 785-786
   00b6f6e4  85c9                   test ecx, ecx
   00b6f6e6  7704                   ja 0xb6f6ec
 - 00b6f6e8  8d542450               lea edx, [esp + 0x50]
 + 00b6f6ca  8d54244c               lea edx, [esp + 0x4c]
   00b6f6ec  8bce                   mov ecx, esi
   00b6f6ee  8bf2                   mov esi, edx
@@ retail 894-894 built 888-889
   00b6f84f  83caff                 or edx, 0xffffffff
   00b6f852  89500c                 mov dword ptr [eax + 0xc], edx
 + 00b6f837  894818                 mov dword ptr [eax + 0x18], ecx
   00b6f855  89481c                 mov dword ptr [eax + 0x1c], ecx
   00b6f858  894818                 mov dword ptr [eax + 0x18], ecx
@@ retail 895-896 built 890-890
   00b6f852  89500c                 mov dword ptr [eax + 0xc], edx
   00b6f855  89481c                 mov dword ptr [eax + 0x1c], ecx
 - 00b6f858  894818                 mov dword ptr [eax + 0x18], ecx
   00b6f85b  894820                 mov dword ptr [eax + 0x20], ecx
   00b6f85e  894828                 mov dword ptr [eax + 0x28], ecx
@@ retail 927-927 built 921-923
   00b6f8c0  898890000000           mov dword ptr [eax + 0x90], ecx
   00b6f8c6  898898000000           mov dword ptr [eax + 0x98], ecx
 + 00b6f8ae  bdcdccccbd             mov ebp, 0xbdcccccd
 + 00b6f8b3  89a894000000           mov dword ptr [eax + 0x94], ebp
   00b6f8cc  8988a0000000           mov dword ptr [eax + 0xa0], ecx
   00b6f8d2  c780a4000000cdcccc3d   mov dword ptr [eax + 0xa4], 0x3dcccccd
@@ retail 930-932 built 926-926
   00b6f8d2  c780a4000000cdcccc3d   mov dword ptr [eax + 0xa4], 0x3dcccccd
   00b6f8dc  89909c000000           mov dword ptr [eax + 0x9c], edx
 - 00b6f8e2  bdcdccccbd             mov ebp, 0xbdcccccd
 - 00b6f8e7  89a894000000           mov dword ptr [eax + 0x94], ebp
   00b6f8ed  89b8a8000000           mov dword ptr [eax + 0xa8], edi
   00b6f8f3  89a8ac000000           mov dword ptr [eax + 0xac], ebp
@@ retail 964-964 built 958-959
   00b6f9a5  898820010000           mov dword ptr [eax + 0x120], ecx
   00b6f9ab  898828010000           mov dword ptr [eax + 0x128], ecx
 + 00b6f993  89a824010000           mov dword ptr [eax + 0x124], ebp
   00b6f9b1  898830010000           mov dword ptr [eax + 0x130], ecx
   00b6f9b7  898834010000           mov dword ptr [eax + 0x134], ecx
@@ retail 966-967 built 961-961
   00b6f9b1  898830010000           mov dword ptr [eax + 0x130], ecx
   00b6f9b7  898834010000           mov dword ptr [eax + 0x134], ecx
 - 00b6f9bd  89a824010000           mov dword ptr [eax + 0x124], ebp
   00b6f9c3  89902c010000           mov dword ptr [eax + 0x12c], edx
   00b6f9c9  898840010000           mov dword ptr [eax + 0x140], ecx
@@ retail 969-971 built 963-963
   00b6f9c3  89902c010000           mov dword ptr [eax + 0x12c], edx
   00b6f9c9  898840010000           mov dword ptr [eax + 0x140], ecx
 - 00b6f9cf  89884c010000           mov dword ptr [eax + 0x14c], ecx
 - 00b6f9d5  8bce                   mov ecx, esi
   00b6f9d7  89b838010000           mov dword ptr [eax + 0x138], edi
   00b6f9dd  89a83c010000           mov dword ptr [eax + 0x13c], ebp
@@ retail 973-973 built 965-966
   00b6f9d7  89b838010000           mov dword ptr [eax + 0x138], edi
   00b6f9dd  89a83c010000           mov dword ptr [eax + 0x13c], ebp
 + 00b6f9bd  89884c010000           mov dword ptr [eax + 0x14c], ecx
   00b6f9e3  899848010000           mov dword ptr [eax + 0x148], ebx
   00b6f9e9  899044010000           mov dword ptr [eax + 0x144], edx
@@ retail 974-974 built 967-968
   00b6f9dd  89a83c010000           mov dword ptr [eax + 0x13c], ebp
   00b6f9e3  899848010000           mov dword ptr [eax + 0x148], ebx
 + 00b6f9c9  8bce                   mov ecx, esi
   00b6f9e9  899044010000           mov dword ptr [eax + 0x144], edx
   00b6f9ef  e8bc26f5ff             call 0xac20b0
@@ retail 978-979 built 972-973
   00b6f9f4  8d4c2424               lea ecx, [esp + 0x24]
   00b6f9f8  e803a9e2ff             call 0x99a300
 - 00b6f9fd  8b442450               mov eax, dword ptr [esp + 0x50]
 + 00b6f9df  8b44244c               mov eax, dword ptr [esp + 0x4c]
   00b6fa01  5f                     pop edi
   00b6fa02  5e                     pop esi
@@ retail 988-989 built 982-983
   00b6fa0a  e805f00800             call 0xbfea14
   00b6fa0f  83c404                 add esp, 4
 - 00b6fa12  8b44244c               mov eax, dword ptr [esp + 0x4c]
 + 00b6f9f4  8b442448               mov eax, dword ptr [esp + 0x48]
   00b6fa16  85c0                   test eax, eax
   00b6fa18  7409                   je 0xb6fa23
@@ retail 994-995 built 988-989
   00b6fa1b  e8f4ef0800             call 0xbfea14
   00b6fa20  83c404                 add esp, 4
 - 00b6fa23  8b442434               mov eax, dword ptr [esp + 0x34]
 + 00b6fa05  8b442430               mov eax, dword ptr [esp + 0x30]
   00b6fa27  85c0                   test eax, eax
   00b6fa29  7409                   je 0xb6fa34
@@ retail 1000-1001 built 994-995
   00b6fa2c  e8e3ef0800             call 0xbfea14
   00b6fa31  83c404                 add esp, 4
 - 00b6fa34  83c458                 add esp, 0x58
 + 00b6fa16  83c454                 add esp, 0x54
   00b6fa37  c3                     ret 
DIFFLINES 209 retail_instr 1002 built_instr 996
