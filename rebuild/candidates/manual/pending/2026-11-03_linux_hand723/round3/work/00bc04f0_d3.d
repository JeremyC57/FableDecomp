00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(3547v3751) flags= []
@@ retail 0-1 built 0-1
 - 00bc04f0  83ec30                 sub esp, 0x30
 + 00bc04f0  83ec20                 sub esp, 0x20
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
   00bc04f8  53                     push ebx
@@ retail 2-3 built 2-2
   00bc04f0  83ec30                 sub esp, 0x30
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
 - 00bc04f8  53                     push ebx
   00bc04f9  57                     push edi
   00bc04fa  33ff                   xor edi, edi
@@ retail 6-8 built 5-6
   00bc04fa  33ff                   xor edi, edi
   00bc04fc  3bc7                   cmp eax, edi
 - 00bc04fe  8bd9                   mov ebx, ecx
 - 00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
 + 00bc04fd  894c2404               mov dword ptr [esp + 4], ecx
   00bc0504  0f84850e0000           je 0xbc138f
   00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
@@ retail 9-12 built 7-9
   00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
   00bc0504  0f84850e0000           je 0xbc138f
 - 00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc050e  8b0d986e4301           mov ecx, dword ptr [0x1436e98]
 - 00bc0514  8b8954090000           mov ecx, dword ptr [ecx + 0x954]
 + 00bc0507  8b442428               mov eax, dword ptr [esp + 0x28]
 + 00bc050b  8b1500000000           mov edx, dword ptr [0]
   00bc051a  55                     push ebp
   00bc051b  8b680c                 mov ebp, dword ptr [eax + 0xc]
@@ retail 14-17 built 11-15
   00bc051a  55                     push ebp
   00bc051b  8b680c                 mov ebp, dword ptr [eax + 0xc]
 - 00bc051e  8b5554                 mov edx, dword ptr [ebp + 0x54]
 - 00bc0521  52                     push edx
 - 00bc0522  896c2418               mov dword ptr [esp + 0x18], ebp
 + 00bc0515  8b4d54                 mov ecx, dword ptr [ebp + 0x54]
 + 00bc0518  51                     push ecx
 + 00bc0519  8b8a54090000           mov ecx, dword ptr [edx + 0x954]
 + 00bc051f  896c2414               mov dword ptr [esp + 0x14], ebp
   00bc0526  e865bce0ff             call 0x9cc190
   00bc052b  84c0                   test al, al
@@ retail 22-22 built 20-21
   00bc0533  a1986e4301             mov eax, dword ptr [0x1436e98]
   00bc0538  8b4d54                 mov ecx, dword ptr [ebp + 0x54]
 + 00bc0538  53                     push ebx
   00bc053b  56                     push esi
   00bc053c  8bb054090000           mov esi, dword ptr [eax + 0x954]
@@ retail 24-25 built 23-23
   00bc053b  56                     push esi
   00bc053c  8bb054090000           mov esi, dword ptr [eax + 0x954]
 - 00bc0542  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc0548  8b96e4010000           mov edx, dword ptr [esi + 0x1e4]
   00bc054e  2bd0                   sub edx, eax
@@ retail 26-27 built 24-26
   00bc0542  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc0548  8b96e4010000           mov edx, dword ptr [esi + 0x1e4]
 - 00bc054e  2bd0                   sub edx, eax
 + 00bc0546  8b9ee0010000           mov ebx, dword ptr [esi + 0x1e0]
 + 00bc054c  2bd3                   sub edx, ebx
   00bc0550  b8e9a28b2e             mov eax, 0x2e8ba2e9
   00bc0555  f7ea                   imul edx
@@ retail 35-36 built 34-34
   00bc0561  3bc8                   cmp ecx, eax
   00bc0563  730d                   jae 0xbc0572
 - 00bc0565  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc056b  6bc92c                 imul ecx, ecx, 0x2c
   00bc056e  03c8                   add ecx, eax
@@ retail 37-37 built 35-36
   00bc0565  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc056b  6bc92c                 imul ecx, ecx, 0x2c
 + 00bc0566  8bc3                   mov eax, ebx
   00bc056e  03c8                   add ecx, eax
   00bc0570  eb02                   jmp 0xbc0574
@@ retail 43-44 built 42-43
   00bc0575  57                     push edi
   00bc0576  57                     push edi
 - 00bc0577  8d54241c               lea edx, [esp + 0x1c]
 + 00bc0571  8d542420               lea edx, [esp + 0x20]
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
@@ retail 46-47 built 45-46
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00bc0580  897c2420               mov dword ptr [esp + 0x20], edi
 + 00bc057a  897c2424               mov dword ptr [esp + 0x24], edi
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
@@ retail 49-49 built 48-49
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
 + 00bc0585  bb10000000             mov ebx, 0x10
   00bc058b  0f84b00d0000           je 0xbc1341
   00bc0591  a108b53c01             mov eax, dword ptr [0x13cb508]
@@ retail 61-62 built 61-62
   00bc05b7  89b014280000           mov dword ptr [eax + 0x2814], esi
   00bc05bd  8911                   mov dword ptr [ecx], edx
 - 00bc05bf  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc05be  8b442414               mov eax, dword ptr [esp + 0x14]
   00bc05c3  8b4808                 mov ecx, dword ptr [eax + 8]
   00bc05c6  3bcf                   cmp ecx, edi
@@ retail 86-87 built 86-87
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
   00bc0610  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0615  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00bc0614  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00bc0619  83f906                 cmp ecx, 6
   00bc061c  0f85a1010000           jne 0xbc07c3
@@ retail 94-95 built 94-96
   00bc062d  0f8472030000           je 0xbc09a5
   00bc0633  8b35d06e3901           mov esi, dword ptr [0x1396ed0]
 - 00bc0639  39b0402c0000           cmp dword ptr [eax + 0x2c40], esi
 + 00bc0638  8b98402c0000           mov ebx, dword ptr [eax + 0x2c40]
 + 00bc063e  3bde                   cmp ebx, esi
   00bc063f  8d883c2c0000           lea ecx, [eax + 0x2c3c]
   00bc0645  c644244400             mov byte ptr [esp + 0x44], 0
@@ retail 96-97 built 97-98
   00bc0639  39b0402c0000           cmp dword ptr [eax + 0x2c40], esi
   00bc063f  8d883c2c0000           lea ecx, [eax + 0x2c3c]
 - 00bc0645  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc0646  c644243400             mov byte ptr [esp + 0x34], 0
   00bc064a  8bd0                   mov edx, eax
   00bc064c  8bfe                   mov edi, esi
@@ retail 121-123 built 122-125
   00bc069a  ff820c280000           inc dword ptr [edx + 0x280c]
   00bc06a0  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc06a5  8b35786f3901           mov esi, dword ptr [0x1396f78]
 - 00bc06ab  39b020290000           cmp dword ptr [eax + 0x2920], esi
 + 00bc06a6  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc06ac  8b9820290000           mov ebx, dword ptr [eax + 0x2920]
 + 00bc06b2  3bde                   cmp ebx, esi
   00bc06b1  8d901c290000           lea edx, [eax + 0x291c]
   00bc06b7  8bc8                   mov ecx, eax
@@ retail 148-150 built 150-153
   00bc0707  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc070d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0712  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 - 00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
 + 00bc0715  8b3514000000           mov esi, dword ptr [0x14]
 + 00bc071b  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 + 00bc0721  3bde                   cmp ebx, esi
   00bc071e  8d9030290000           lea edx, [eax + 0x2930]
   00bc0724  8bc8                   mov ecx, eax
@@ retail 178-179 built 181-183
   00bc0785  f6c104                 test cl, 4
   00bc0788  0f8499010000           je 0xbc0927
 - 00bc078e  8d4b40                 lea ecx, [ebx + 0x40]
 + 00bc078f  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc0793  83c140                 add ecx, 0x40
   00bc0791  6a00                   push 0
   00bc0793  51                     push ecx
@@ retail 182-182 built 186-203
   00bc0793  51                     push ecx
   00bc0794  eb06                   jmp 0xbc079c
 + 00bc079b  f6c110                 test cl, 0x10
 + 00bc079e  7439                   je 0xbc07d9
 + 00bc07a0  8b3508000000           mov esi, dword ptr [8]
 + 00bc07a6  8b9894280000           mov ebx, dword ptr [eax + 0x2894]
 + 00bc07ac  3bde                   cmp ebx, esi
 + 00bc07ae  8d8890280000           lea ecx, [eax + 0x2890]
 + 00bc07b4  8bd0                   mov edx, eax
 + 00bc07b6  8bfe                   mov edi, esi
 + 00bc07b8  0f8428020000           je 0xbc09e6
 + 00bc07be  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc07c4  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc07c7  85f0                   test eax, esi
 + 00bc07c9  0f85f6010000           jne 0xbc09c5
 + 00bc07cf  0bf0                   or esi, eax
 + 00bc07d1  897108                 mov dword ptr [ecx + 8], esi
 + 00bc07d4  e9cf010000             jmp 0xbc09a8
 + 00bc07d9  8b542410               mov edx, dword ptr [esp + 0x10]
   00bc0796  6a00                   push 0
   00bc0798  8d5330                 lea edx, [ebx + 0x30]
@@ retail 183-184 built 204-205
   00bc0794  eb06                   jmp 0xbc079c
   00bc0796  6a00                   push 0
 - 00bc0798  8d5330                 lea edx, [ebx + 0x30]
 + 00bc07df  83c230                 add edx, 0x30
   00bc079b  52                     push edx
   00bc079c  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 187-187 built 208-209
   00bc079c  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc07a2  e87978dcff             call 0x988020
 + 00bc07ee  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc07a7  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc07ad  83c358                 add ebx, 0x58
@@ retail 188-190 built 210-212
   00bc07a2  e87978dcff             call 0x988020
   00bc07a7  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc07ad  83c358                 add ebx, 0x58
 - 00bc07b0  53                     push ebx
 + 00bc07f8  83c058                 add eax, 0x58
 + 00bc07fb  50                     push eax
   00bc07b1  e88a79dcff             call 0x988140
   00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
@@ retail 191-193 built 213-215
   00bc07b0  53                     push ebx
   00bc07b1  e88a79dcff             call 0x988140
 - 00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc07ba  8b742418               mov esi, dword ptr [esp + 0x18]
 + 00bc0801  8b7c2410               mov edi, dword ptr [esp + 0x10]
 + 00bc0805  8b6c2418               mov ebp, dword ptr [esp + 0x18]
   00bc07be  e9be060000             jmp 0xbc0e81
   00bc07c3  83f907                 cmp ecx, 7
@@ retail 199-200 built 221-222
   00bc07d2  3bf7                   cmp esi, edi
   00bc07d4  8d90cc280000           lea edx, [eax + 0x28cc]
 - 00bc07da  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc0825  c644243400             mov byte ptr [esp + 0x34], 0
   00bc07df  8bc8                   mov ecx, eax
   00bc07e1  7455                   je 0xbc0838
@@ retail 224-224 built 246-247
   00bc0833  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0838  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
 + 00bc0889  3bf3                   cmp esi, ebx
   00bc083e  8d90e0280000           lea edx, [eax + 0x28e0]
   00bc0844  bf10000000             mov edi, 0x10
@@ retail 225-227 built 248-248
   00bc0838  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
   00bc083e  8d90e0280000           lea edx, [eax + 0x28e0]
 - 00bc0844  bf10000000             mov edi, 0x10
 - 00bc0849  3bf7                   cmp esi, edi
   00bc084b  8bc8                   mov ecx, eax
   00bc084d  7455                   je 0xbc08a4
@@ retail 243-244 built 264-265
   00bc087e  8a4210                 mov al, byte ptr [edx + 0x10]
   00bc0881  84c0                   test al, al
 - 00bc0883  897a04                 mov dword ptr [edx + 4], edi
 + 00bc08c9  895a04                 mov dword ptr [edx + 4], ebx
   00bc0886  7517                   jne 0xbc089f
   00bc0888  c6421001               mov byte ptr [edx + 0x10], 1
@@ retail 250-252 built 271-274
   00bc0899  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc089f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc08a4  8b35e86d3901           mov esi, dword ptr [0x1396de8]
 - 00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
 + 00bc08ea  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc08f0  8b98a8280000           mov ebx, dword ptr [eax + 0x28a8]
 + 00bc08f6  3bde                   cmp ebx, esi
   00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
   00bc08b6  8bc8                   mov ecx, eax
@@ retail 253-457 built 275-275
   00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
   00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
 - 00bc08b6  8bc8                   mov ecx, eax
 - 00bc08b8  8bfe                   mov edi, esi
 - 00bc08ba  7455                   je 0xbc0911
 - 00bc08bc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc08c2  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc08c5  85f0                   test eax, esi
 - 00bc08c7  7522                   jne 0xbc08eb
 - 00bc08c9  0bf0                   or esi, eax
 - 00bc08cb  897208                 mov dword ptr [edx + 8], esi
 - 00bc08ce  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc08d4  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc08d8  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc08de  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc08e1  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc08e5  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc08eb  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc08ee  84c0                   test al, al
 - 00bc08f0  897a04                 mov dword ptr [edx + 4], edi
 - 00bc08f3  7517                   jne 0xbc090c
 - 00bc08f5  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc08f9  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc08ff  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0906  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc090c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0911  8a8d84000000           mov cl, byte ptr [ebp + 0x84]
 - 00bc0917  f6c104                 test cl, 4
 - 00bc091a  740b                   je 0xbc0927
 - 00bc091c  6a00                   push 0
 - 00bc091e  8d4b40                 lea ecx, [ebx + 0x40]
 - 00bc0921  51                     push ecx
 - 00bc0922  e975feffff             jmp 0xbc079c
 - 00bc0927  f6c110                 test cl, 0x10
 - 00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 - 00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
 - 00bc093c  3bee                   cmp ebp, esi
 - 00bc093e  8d8890280000           lea ecx, [eax + 0x2890]
 - 00bc0944  8bd0                   mov edx, eax
 - 00bc0946  8bfe                   mov edi, esi
 - 00bc0948  7450                   je 0xbc099a
 - 00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0953  85f0                   test eax, esi
 - 00bc0955  7522                   jne 0xbc0979
 - 00bc0957  0bf0                   or esi, eax
 - 00bc0959  897108                 mov dword ptr [ecx + 8], esi
 - 00bc095c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0962  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 - 00bc0966  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc096c  8b7104                 mov esi, dword ptr [ecx + 4]
 - 00bc096f  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 - 00bc0973  ff8208280000           inc dword ptr [edx + 0x2808]
 - 00bc0979  897904                 mov dword ptr [ecx + 4], edi
 - 00bc097c  8a4110                 mov al, byte ptr [ecx + 0x10]
 - 00bc097f  84c0                   test al, al
 - 00bc0981  7517                   jne 0xbc099a
 - 00bc0983  c6411001               mov byte ptr [ecx + 0x10], 1
 - 00bc0987  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 - 00bc098d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 - 00bc0994  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc099a  8d4b38                 lea ecx, [ebx + 0x38]
 - 00bc099d  6a00                   push 0
 - 00bc099f  51                     push ecx
 - 00bc09a0  e9f7fdffff             jmp 0xbc079c
 - 00bc09a5  8b4d7c                 mov ecx, dword ptr [ebp + 0x7c]
 - 00bc09a8  49                     dec ecx
 - 00bc09a9  8b98402c0000           mov ebx, dword ptr [eax + 0x2c40]
 - 00bc09af  8d903c2c0000           lea edx, [eax + 0x2c3c]
 - 00bc09b5  7413                   je 0xbc09ca
 - 00bc09b7  8b35d06e3901           mov esi, dword ptr [0x1396ed0]
 - 00bc09bd  49                     dec ecx
 - 00bc09be  8bc8                   mov ecx, eax
 - 00bc09c0  7510                   jne 0xbc09d2
 - 00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
 - 00bc09c8  eb08                   jmp 0xbc09d2
 - 00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
 - 00bc09d0  8bc8                   mov ecx, eax
 - 00bc09d2  3bde                   cmp ebx, esi
 - 00bc09d4  8bfe                   mov edi, esi
 - 00bc09d6  7455                   je 0xbc0a2d
 - 00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc09e1  85f0                   test eax, esi
 - 00bc09e3  7522                   jne 0xbc0a07
 - 00bc09e5  0bf0                   or esi, eax
 - 00bc09e7  897208                 mov dword ptr [edx + 8], esi
 - 00bc09ea  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc09f0  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc09f4  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc09fa  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc09fd  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc0a01  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc0a07  897a04                 mov dword ptr [edx + 4], edi
 - 00bc0a0a  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc0a0d  84c0                   test al, al
 - 00bc0a0f  7517                   jne 0xbc0a28
 - 00bc0a11  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc0a15  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0a1b  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0a22  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0a28  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0a2d  8b4d78                 mov ecx, dword ptr [ebp + 0x78]
 - 00bc0a30  83e903                 sub ecx, 3
 - 00bc0a33  8b9820290000           mov ebx, dword ptr [eax + 0x2920]
 - 00bc0a39  8d901c290000           lea edx, [eax + 0x291c]
 - 00bc0a3f  0f8478010000           je 0xbc0bbd
 - 00bc0a45  49                     dec ecx
 - 00bc0a46  8bc8                   mov ecx, eax
 - 00bc0a48  0f84d5000000           je 0xbc0b23
 - 00bc0a4e  8b35786f3901           mov esi, dword ptr [0x1396f78]
 - 00bc0a54  3bde                   cmp ebx, esi
 - 00bc0a56  8bfe                   mov edi, esi
 - 00bc0a58  7455                   je 0xbc0aaf
 - 00bc0a5a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0a60  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0a63  85f0                   test eax, esi
 - 00bc0a65  7522                   jne 0xbc0a89
 - 00bc0a67  0bf0                   or esi, eax
 - 00bc0a69  897208                 mov dword ptr [edx + 8], esi
 - 00bc0a6c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0a72  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc0a76  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0a7c  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc0a7f  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc0a83  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc0a89  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc0a8c  84c0                   test al, al
 - 00bc0a8e  897a04                 mov dword ptr [edx + 4], edi
 - 00bc0a91  7517                   jne 0xbc0aaa
 - 00bc0a93  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc0a97  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0a9d  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0aa4  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0aaa  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0aaf  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 - 00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 - 00bc0abb  3bde                   cmp ebx, esi
 - 00bc0abd  8d8830290000           lea ecx, [eax + 0x2930]
 - 00bc0ac3  8bd0                   mov edx, eax
 - 00bc0ac5  8bfe                   mov edi, esi
 - 00bc0ac7  7450                   je 0xbc0b19
 - 00bc0ac9  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0acf  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0ad2  85f0                   test eax, esi
 - 00bc0ad4  7522                   jne 0xbc0af8
 - 00bc0ad6  0bf0                   or esi, eax
 - 00bc0ad8  897108                 mov dword ptr [ecx + 8], esi
 - 00bc0adb  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0ae1  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 - 00bc0ae5  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0aeb  8b7104                 mov esi, dword ptr [ecx + 4]
 - 00bc0aee  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 - 00bc0af2  ff8208280000           inc dword ptr [edx + 0x2808]
 - 00bc0af8  8a4110                 mov al, byte ptr [ecx + 0x10]
 - 00bc0afb  84c0                   test al, al
 - 00bc0afd  897904                 mov dword ptr [ecx + 4], edi
 - 00bc0b00  7517                   jne 0xbc0b19
 - 00bc0b02  c6411001               mov byte ptr [ecx + 0x10], 1
 - 00bc0b06  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 - 00bc0b0c  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 - 00bc0b13  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc0b19  c644244400             mov byte ptr [esp + 0x44], 0
 - 00bc0b1e  e96c010000             jmp 0xbc0c8f
 - 00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 - 00bc0b29  3bde                   cmp ebx, esi
 - 00bc0b2b  8bfe                   mov edi, esi
 - 00bc0b2d  7455                   je 0xbc0b84
 - 00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0b35  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0b38  85f0                   test eax, esi
 - 00bc0b3a  7522                   jne 0xbc0b5e
 - 00bc0b3c  0bf0                   or esi, eax
 - 00bc0b3e  897208                 mov dword ptr [edx + 8], esi
 - 00bc0b41  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0b47  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc0b4b  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0b51  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc0b54  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc0b58  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc0b5e  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc0b61  84c0                   test al, al
 - 00bc0b63  897a04                 mov dword ptr [edx + 4], edi
 - 00bc0b66  7517                   jne 0xbc0b7f
 - 00bc0b68  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc0b6c  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0b72  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0b79  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0b7f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0b84  8b35746f3901           mov esi, dword ptr [0x1396f74]
 - 00bc0b8a  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 - 00bc0b90  3bde                   cmp ebx, esi
 - 00bc0b92  8d8830290000           lea ecx, [eax + 0x2930]
 - 00bc0b98  8bd0                   mov edx, eax
 - 00bc0b9a  8bfe                   mov edi, esi
 - 00bc0b9c  0f84e8000000           je 0xbc0c8a
 - 00bc0ba2  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0ba8  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0bab  85f0                   test eax, esi
 - 00bc0bad  0f85b6000000           jne 0xbc0c69
 - 00bc0bb3  0bf0                   or esi, eax
 - 00bc0bb5  897108                 mov dword ptr [ecx + 8], esi
 - 00bc0bb8  e98f000000             jmp 0xbc0c4c
 - 00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 - 00bc0bc3  3bde                   cmp ebx, esi
   00bc0bc5  8bc8                   mov ecx, eax
   00bc0bc7  8bfe                   mov edi, esi
@@ retail 481-481 built 299-346
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 + 00bc0959  8a8d84000000           mov cl, byte ptr [ebp + 0x84]
 + 00bc095f  f6c104                 test cl, 4
 + 00bc0962  740f                   je 0xbc0973
 + 00bc0964  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc0968  6a00                   push 0
 + 00bc096a  83c140                 add ecx, 0x40
 + 00bc096d  51                     push ecx
 + 00bc096e  e970feffff             jmp 0xbc07e3
 + 00bc0973  f6c110                 test cl, 0x10
 + 00bc0976  0f845dfeffff           je 0xbc07d9
 + 00bc097c  8b3508000000           mov esi, dword ptr [8]
 + 00bc0982  8b9894280000           mov ebx, dword ptr [eax + 0x2894]
 + 00bc0988  3bde                   cmp ebx, esi
 + 00bc098a  8d8890280000           lea ecx, [eax + 0x2890]
 + 00bc0990  8bd0                   mov edx, eax
 + 00bc0992  8bfe                   mov edi, esi
 + 00bc0994  7450                   je 0xbc09e6
 + 00bc0996  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc099c  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc099f  85c6                   test esi, eax
 + 00bc09a1  7522                   jne 0xbc09c5
 + 00bc09a3  0bc6                   or eax, esi
 + 00bc09a5  894108                 mov dword ptr [ecx + 8], eax
 + 00bc09a8  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc09ae  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 + 00bc09b2  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc09b8  8b7104                 mov esi, dword ptr [ecx + 4]
 + 00bc09bb  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 + 00bc09bf  ff8208280000           inc dword ptr [edx + 0x2808]
 + 00bc09c5  897904                 mov dword ptr [ecx + 4], edi
 + 00bc09c8  8a4110                 mov al, byte ptr [ecx + 0x10]
 + 00bc09cb  84c0                   test al, al
 + 00bc09cd  7517                   jne 0xbc09e6
 + 00bc09cf  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc09d3  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 + 00bc09d9  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 + 00bc09e0  ff820c280000           inc dword ptr [edx + 0x280c]
 + 00bc09e6  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc09ea  83c138                 add ecx, 0x38
 + 00bc09ed  6a00                   push 0
 + 00bc09ef  51                     push ecx
 + 00bc09f0  e9eefdffff             jmp 0xbc07e3
 + 00bc09f5  8b4d7c                 mov ecx, dword ptr [ebp + 0x7c]
 + 00bc09f8  49                     dec ecx
 + 00bc09f9  8b98402c0000           mov ebx, dword ptr [eax + 0x2c40]
 + 00bc09ff  8d903c2c0000           lea edx, [eax + 0x2c3c]
 + 00bc0a05  7413                   je 0xbc0a1a
   00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
@@ retail 482-482 built 347-412
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0a0d  49                     dec ecx
 + 00bc0a0e  8bc8                   mov ecx, eax
 + 00bc0a10  7510                   jne 0xbc0a22
 + 00bc0a12  8b3508000000           mov esi, dword ptr [8]
 + 00bc0a18  eb08                   jmp 0xbc0a22
 + 00bc0a1a  8b3504000000           mov esi, dword ptr [4]
 + 00bc0a20  8bc8                   mov ecx, eax
 + 00bc0a22  3bde                   cmp ebx, esi
 + 00bc0a24  8bfe                   mov edi, esi
 + 00bc0a26  7455                   je 0xbc0a7d
 + 00bc0a28  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0a2e  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc0a31  85c6                   test esi, eax
 + 00bc0a33  7522                   jne 0xbc0a57
 + 00bc0a35  0bc6                   or eax, esi
 + 00bc0a37  894208                 mov dword ptr [edx + 8], eax
 + 00bc0a3a  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0a40  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc0a44  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0a4a  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc0a4d  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc0a51  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc0a57  897a04                 mov dword ptr [edx + 4], edi
 + 00bc0a5a  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc0a5d  84c0                   test al, al
 + 00bc0a5f  7517                   jne 0xbc0a78
 + 00bc0a61  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc0a65  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0a6b  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc0a72  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc0a78  a100000000             mov eax, dword ptr [0]
 + 00bc0a7d  8b4d78                 mov ecx, dword ptr [ebp + 0x78]
 + 00bc0a80  83e903                 sub ecx, 3
 + 00bc0a83  8b9820290000           mov ebx, dword ptr [eax + 0x2920]
 + 00bc0a89  8d901c290000           lea edx, [eax + 0x291c]
 + 00bc0a8f  0f8447010000           je 0xbc0bdc
 + 00bc0a95  49                     dec ecx
 + 00bc0a96  8bc8                   mov ecx, eax
 + 00bc0a98  0f84d5000000           je 0xbc0b73
 + 00bc0a9e  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc0aa4  3bde                   cmp ebx, esi
 + 00bc0aa6  8bfe                   mov edi, esi
 + 00bc0aa8  7455                   je 0xbc0aff
 + 00bc0aaa  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0ab0  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc0ab3  85c6                   test esi, eax
 + 00bc0ab5  7522                   jne 0xbc0ad9
 + 00bc0ab7  0bc6                   or eax, esi
 + 00bc0ab9  894208                 mov dword ptr [edx + 8], eax
 + 00bc0abc  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0ac2  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc0ac6  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0acc  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc0acf  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc0ad3  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc0ad9  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc0adc  84c0                   test al, al
 + 00bc0ade  897a04                 mov dword ptr [edx + 4], edi
 + 00bc0ae1  7517                   jne 0xbc0afa
 + 00bc0ae3  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc0ae7  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0aed  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc0af4  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc0afa  a100000000             mov eax, dword ptr [0]
 + 00bc0aff  8b3514000000           mov esi, dword ptr [0x14]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0c2c  3bde                   cmp ebx, esi
@@ retail 508-509 built 438-521
   00bc0c7d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
   00bc0c84  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc0c8a  c644244401             mov byte ptr [esp + 0x44], 1
 + 00bc0b69  c644243400             mov byte ptr [esp + 0x34], 0
 + 00bc0b6e  e93b010000             jmp 0xbc0cae
 + 00bc0b73  8b3504000000           mov esi, dword ptr [4]
 + 00bc0b79  3bde                   cmp ebx, esi
 + 00bc0b7b  8bfe                   mov edi, esi
 + 00bc0b7d  7455                   je 0xbc0bd4
 + 00bc0b7f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0b85  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc0b88  85c6                   test esi, eax
 + 00bc0b8a  7522                   jne 0xbc0bae
 + 00bc0b8c  0bc6                   or eax, esi
 + 00bc0b8e  894208                 mov dword ptr [edx + 8], eax
 + 00bc0b91  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0b97  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc0b9b  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0ba1  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc0ba4  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc0ba8  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc0bae  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc0bb1  84c0                   test al, al
 + 00bc0bb3  897a04                 mov dword ptr [edx + 4], edi
 + 00bc0bb6  7517                   jne 0xbc0bcf
 + 00bc0bb8  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc0bbc  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0bc2  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc0bc9  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc0bcf  a100000000             mov eax, dword ptr [0]
 + 00bc0bd4  8b350c000000           mov esi, dword ptr [0xc]
 + 00bc0bda  eb69                   jmp 0xbc0c45
 + 00bc0bdc  8b3504000000           mov esi, dword ptr [4]
 + 00bc0be2  3bde                   cmp ebx, esi
 + 00bc0be4  8bc8                   mov ecx, eax
 + 00bc0be6  8bfe                   mov edi, esi
 + 00bc0be8  745b                   je 0xbc0c45
 + 00bc0bea  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0bf0  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc0bf3  85c6                   test esi, eax
 + 00bc0bf5  7522                   jne 0xbc0c19
 + 00bc0bf7  0bc6                   or eax, esi
 + 00bc0bf9  894208                 mov dword ptr [edx + 8], eax
 + 00bc0bfc  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0c02  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc0c06  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0c0c  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc0c0f  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc0c13  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc0c19  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc0c1c  84c0                   test al, al
 + 00bc0c1e  897a04                 mov dword ptr [edx + 4], edi
 + 00bc0c21  7517                   jne 0xbc0c3a
 + 00bc0c23  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc0c27  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0c2d  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc0c34  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc0c3a  a100000000             mov eax, dword ptr [0]
 + 00bc0c3f  8b3504000000           mov esi, dword ptr [4]
 + 00bc0c45  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 + 00bc0c4b  3bde                   cmp ebx, esi
 + 00bc0c4d  8d8830290000           lea ecx, [eax + 0x2930]
 + 00bc0c53  8bd0                   mov edx, eax
 + 00bc0c55  8bfe                   mov edi, esi
 + 00bc0c57  7450                   je 0xbc0ca9
 + 00bc0c59  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0c5f  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc0c62  85c6                   test esi, eax
 + 00bc0c64  7522                   jne 0xbc0c88
 + 00bc0c66  0bc6                   or eax, esi
 + 00bc0c68  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0c6b  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc0c71  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 + 00bc0c75  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc0c7b  8b7104                 mov esi, dword ptr [ecx + 4]
 + 00bc0c7e  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 + 00bc0c82  ff8208280000           inc dword ptr [edx + 0x2808]
 + 00bc0c88  8a4110                 mov al, byte ptr [ecx + 0x10]
 + 00bc0c8b  84c0                   test al, al
 + 00bc0c8d  897904                 mov dword ptr [ecx + 4], edi
 + 00bc0c90  7517                   jne 0xbc0ca9
 + 00bc0c92  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0c96  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 + 00bc0c9c  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 + 00bc0ca3  ff820c280000           inc dword ptr [edx + 0x280c]
 + 00bc0ca9  c644243401             mov byte ptr [esp + 0x34], 1
   00bc0c8f  8a8584000000           mov al, byte ptr [ebp + 0x84]
   00bc0c95  8b0d846e4301           mov ecx, dword ptr [0x1436e84]
@@ retail 521-531 built 533-544
   00bc0caf  8bf0                   mov esi, eax
   00bc0cb1  eb7b                   jmp 0xbc0d2e
 - 00bc0cb3  8b4d4c                 mov ecx, dword ptr [ebp + 0x4c]
 - 00bc0cb6  8b4548                 mov eax, dword ptr [ebp + 0x48]
 - 00bc0cb9  c644244800             mov byte ptr [esp + 0x48], 0
 - 00bc0cbe  c644244900             mov byte ptr [esp + 0x49], 0
 - 00bc0cc3  c644244a00             mov byte ptr [esp + 0x4a], 0
 - 00bc0cc8  8b542448               mov edx, dword ptr [esp + 0x48]
 - 00bc0ccc  894c2424               mov dword ptr [esp + 0x24], ecx
 - 00bc0cd0  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0cd4  8b5550                 mov edx, dword ptr [ebp + 0x50]
 - 00bc0cd7  8d4c2448               lea ecx, [esp + 0x48]
 + 00bc0cd2  8b454c                 mov eax, dword ptr [ebp + 0x4c]
 + 00bc0cd5  8b4d50                 mov ecx, dword ptr [ebp + 0x50]
 + 00bc0cd8  8b5548                 mov edx, dword ptr [ebp + 0x48]
 + 00bc0cdb  89442424               mov dword ptr [esp + 0x24], eax
 + 00bc0cdf  8d442438               lea eax, [esp + 0x38]
 + 00bc0ce3  50                     push eax
 + 00bc0ce4  894c242c               mov dword ptr [esp + 0x2c], ecx
 + 00bc0ce8  6a05                   push 5
 + 00bc0cea  8d4c2428               lea ecx, [esp + 0x28]
 + 00bc0cee  89542428               mov dword ptr [esp + 0x28], edx
 + 00bc0cf2  8b5570                 mov edx, dword ptr [ebp + 0x70]
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 533-540 built 546-550
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0ce2  8954242c               mov dword ptr [esp + 0x2c], edx
 - 00bc0ce6  89442424               mov dword ptr [esp + 0x24], eax
 - 00bc0cea  8b4570                 mov eax, dword ptr [ebp + 0x70]
 - 00bc0ced  6a05                   push 5
 - 00bc0cef  8d542428               lea edx, [esp + 0x28]
 - 00bc0cf3  52                     push edx
 - 00bc0cf4  89442438               mov dword ptr [esp + 0x38], eax
 + 00bc0cfc  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc0d01  c644244500             mov byte ptr [esp + 0x45], 0
 + 00bc0d06  c644244600             mov byte ptr [esp + 0x46], 0
 + 00bc0d0b  89542438               mov dword ptr [esp + 0x38], edx
   00bc0cf8  e8935ef8ff             call 0xb46b90
   00bc0cfd  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 546-553 built 556-559
   00bc0d06  e81542f8ff             call 0xb44f20
   00bc0d0b  eb21                   jmp 0xbc0d2e
 - 00bc0d0d  a19c6e4301             mov eax, dword ptr [0x1436e9c]
 - 00bc0d12  8b90e8000000           mov edx, dword ptr [eax + 0xe8]
 - 00bc0d18  b964000000             mov ecx, 0x64
 - 00bc0d1d  3bd1                   cmp edx, ecx
 - 00bc0d1f  740d                   je 0xbc0d2e
 - 00bc0d21  8988e8000000           mov dword ptr [eax + 0xe8], ecx
 - 00bc0d27  8388e400000002         or dword ptr [eax + 0xe4], 2
 + 00bc0d24  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc0d2a  6a64                   push 0x64
 + 00bc0d2c  e800000000             call 0xbc0d31
   00bc0d2e  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
   00bc0d34  e8475ff8ff             call 0xb46c80
@@ retail 558-559 built 564-565
   00bc0d3f  a804                   test al, 4
   00bc0d41  7429                   je 0xbc0d6c
 - 00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0d46  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
   00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
@@ retail 560-562 built 566-567
   00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
 - 00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
 - 00bc0d4d  2bd5                   sub edx, ebp
 + 00bc0d4d  2b5124                 sub edx, dword ptr [ecx + 0x24]
   00bc0d4f  c1fa03                 sar edx, 3
   00bc0d52  4a                     dec edx
@@ retail 565-566 built 570-570
   00bc0d52  4a                     dec edx
   00bc0d53  3bf2                   cmp esi, edx
 - 00bc0d55  8bc6                   mov eax, esi
   00bc0d57  760b                   jbe 0xbc0d64
   00bc0d59  8b4128                 mov eax, dword ptr [ecx + 0x28]
@@ retail 568-569 built 572-573
   00bc0d57  760b                   jbe 0xbc0d64
   00bc0d59  8b4128                 mov eax, dword ptr [ecx + 0x28]
 - 00bc0d5c  8bfd                   mov edi, ebp
 + 00bc0d5b  8b7924                 mov edi, dword ptr [ecx + 0x24]
   00bc0d5e  2bc7                   sub eax, edi
   00bc0d60  c1f803                 sar eax, 3
@@ retail 572-572 built 576-578
   00bc0d60  c1f803                 sar eax, 3
   00bc0d63  48                     dec eax
 + 00bc0d64  8bcf                   mov ecx, edi
 + 00bc0d66  e9df000000             jmp 0xbc0e4a
   00bc0d64  8b4924                 mov ecx, dword ptr [ecx + 0x24]
   00bc0d67  e9d1000000             jmp 0xbc0e3d
@@ retail 573-573 built 579-580
   00bc0d63  48                     dec eax
   00bc0d64  8b4924                 mov ecx, dword ptr [ecx + 0x24]
 + 00bc0d6e  8bc6                   mov eax, esi
   00bc0d67  e9d1000000             jmp 0xbc0e3d
   00bc0d6c  a810                   test al, 0x10
@@ retail 577-578 built 584-585
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 + 00bc0d83  8b1508000000           mov edx, dword ptr [8]
   00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
   00bc0d86  3bfa                   cmp edi, edx
@@ retail 583-583 built 590-591
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 + 00bc0d9b  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
   00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
@@ retail 584-585 built 592-592
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
@@ retail 603-611 built 610-617
   00bc0dd5  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00bc0ddc  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0de2  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0de6  8b4f1c                 mov ecx, dword ptr [edi + 0x1c]
 - 00bc0de9  8b6f18                 mov ebp, dword ptr [edi + 0x18]
 - 00bc0dec  2bcd                   sub ecx, ebp
 - 00bc0dee  c1f903                 sar ecx, 3
 - 00bc0df1  49                     dec ecx
 - 00bc0df2  3bf1                   cmp esi, ecx
 - 00bc0df4  8bc6                   mov eax, esi
 + 00bc0deb  8b7c2410               mov edi, dword ptr [esp + 0x10]
 + 00bc0def  8b471c                 mov eax, dword ptr [edi + 0x1c]
 + 00bc0df2  8b4f18                 mov ecx, dword ptr [edi + 0x18]
 + 00bc0df5  2bc1                   sub eax, ecx
 + 00bc0df7  c1f803                 sar eax, 3
 + 00bc0dfa  48                     dec eax
 + 00bc0dfb  3bf0                   cmp esi, eax
   00bc0df6  760b                   jbe 0xbc0e03
   00bc0df8  8b471c                 mov eax, dword ptr [edi + 0x1c]
@@ retail 613-614 built 619-620
   00bc0df6  760b                   jbe 0xbc0e03
   00bc0df8  8b471c                 mov eax, dword ptr [edi + 0x1c]
 - 00bc0dfb  8bf5                   mov esi, ebp
 + 00bc0e02  8bf1                   mov esi, ecx
   00bc0dfd  2bc6                   sub eax, esi
   00bc0dff  c1f803                 sar eax, 3
@@ retail 617-618 built 623-627
   00bc0dff  c1f803                 sar eax, 3
   00bc0e02  48                     dec eax
 - 00bc0e03  8b5718                 mov edx, dword ptr [edi + 0x18]
 + 00bc0e0a  eb02                   jmp 0xbc0e0e
 + 00bc0e0c  8bc6                   mov eax, esi
 + 00bc0e0e  8b4f18                 mov ecx, dword ptr [edi + 0x18]
 + 00bc0e11  8d14c1                 lea edx, [ecx + eax*8]
   00bc0e06  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0e0c  6a00                   push 0
@@ retail 620-622 built 629-630
   00bc0e06  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0e0c  6a00                   push 0
 - 00bc0e0e  8d04c2                 lea eax, [edx + eax*8]
 - 00bc0e11  50                     push eax
 + 00bc0e1c  52                     push edx
   00bc0e12  e80972dcff             call 0x988020
   00bc0e17  eb39                   jmp 0xbc0e52
@@ retail 624-632 built 632-639
   00bc0e12  e80972dcff             call 0x988020
   00bc0e17  eb39                   jmp 0xbc0e52
 - 00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 - 00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
 - 00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
 - 00bc0e23  2bd5                   sub edx, ebp
 - 00bc0e25  c1fa03                 sar edx, 3
 - 00bc0e28  4a                     dec edx
 - 00bc0e29  3bf2                   cmp esi, edx
 - 00bc0e2b  8bc6                   mov eax, esi
 + 00bc0e24  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc0e28  8b4110                 mov eax, dword ptr [ecx + 0x10]
 + 00bc0e2b  8b510c                 mov edx, dword ptr [ecx + 0xc]
 + 00bc0e2e  2bc2                   sub eax, edx
 + 00bc0e30  c1f803                 sar eax, 3
 + 00bc0e33  48                     dec eax
 + 00bc0e34  3bf0                   cmp esi, eax
   00bc0e2d  760b                   jbe 0xbc0e3a
   00bc0e2f  8b4110                 mov eax, dword ptr [ecx + 0x10]
@@ retail 634-635 built 641-642
   00bc0e2d  760b                   jbe 0xbc0e3a
   00bc0e2f  8b4110                 mov eax, dword ptr [ecx + 0x10]
 - 00bc0e32  8bfd                   mov edi, ebp
 + 00bc0e3b  8bfa                   mov edi, edx
   00bc0e34  2bc7                   sub eax, edi
   00bc0e36  c1f803                 sar eax, 3
@@ retail 638-638 built 645-647
   00bc0e36  c1f803                 sar eax, 3
   00bc0e39  48                     dec eax
 + 00bc0e43  eb02                   jmp 0xbc0e47
 + 00bc0e45  8bc6                   mov eax, esi
   00bc0e3a  8b490c                 mov ecx, dword ptr [ecx + 0xc]
   00bc0e3d  8d14c1                 lea edx, [ecx + eax*8]
@@ retail 644-647 built 653-656
   00bc0e48  52                     push edx
   00bc0e49  e8d271dcff             call 0x988020
 - 00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0e52  8b742418               mov esi, dword ptr [esp + 0x18]
 - 00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
 + 00bc0e5b  8b7c2410               mov edi, dword ptr [esp + 0x10]
 + 00bc0e5f  8b6c2418               mov ebp, dword ptr [esp + 0x18]
 + 00bc0e63  f6858400000020         test byte ptr [ebp + 0x84], 0x20
   00bc0e5d  7413                   je 0xbc0e72
   00bc0e5f  8d4748                 lea eax, [edi + 0x48]
@@ retail 664-665 built 673-674
   00bc0e8a  85c0                   test eax, eax
   00bc0e8c  7418                   je 0xbc0ea6
 - 00bc0e8e  0fb65810               movzx ebx, byte ptr [eax + 0x10]
 + 00bc0e9b  0fb67010               movzx esi, byte ptr [eax + 0x10]
   00bc0e92  8b4004                 mov eax, dword ptr [eax + 4]
   00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
@@ retail 668-669 built 677-678
   00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
   00bc0e9b  8b11                   mov edx, dword ptr [ecx]
 - 00bc0e9d  53                     push ebx
 + 00bc0eaa  56                     push esi
   00bc0e9e  6a00                   push 0
   00bc0ea0  50                     push eax
@@ retail 697-698 built 706-708
   00bc0eea  a1a06e4301             mov eax, dword ptr [0x1436ea0]
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
 - 00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 + 00bc0f02  8b74241c               mov esi, dword ptr [esp + 0x1c]
 + 00bc0f06  0544010000             add eax, 0x144
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
@@ retail 699-700 built 709-710
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 + 00bc0f11  8b500c                 mov edx, dword ptr [eax + 0xc]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
@@ retail 701-703 built 711-712
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
 - 00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
 + 00bc0f1a  8b5568                 mov edx, dword ptr [ebp + 0x68]
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
@@ retail 704-705 built 713-714
   00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
 - 00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
 + 00bc0f23  8b556c                 mov edx, dword ptr [ebp + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
   00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
@@ retail 706-707 built 715-716
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0f2c  8997b4000000           mov dword ptr [edi + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 711-717 built 720-726
   00bc0f2e  8b5010                 mov edx, dword ptr [eax + 0x10]
   00bc0f31  8997bc000000           mov dword ptr [edi + 0xbc], edx
 - 00bc0f37  0fb75306               movzx edx, word ptr [ebx + 6]
 - 00bc0f3b  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f3f  0fb713                 movzx edx, word ptr [ebx]
 - 00bc0f42  db442448               fild dword ptr [esp + 0x48]
 - 00bc0f46  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f4a  da742448               fidiv dword ptr [esp + 0x48]
 + 00bc0f44  0fb75606               movzx edx, word ptr [esi + 6]
 + 00bc0f48  89542438               mov dword ptr [esp + 0x38], edx
 + 00bc0f4c  0fb716                 movzx edx, word ptr [esi]
 + 00bc0f4f  db442438               fild dword ptr [esp + 0x38]
 + 00bc0f53  89542438               mov dword ptr [esp + 0x38], edx
 + 00bc0f57  da742438               fidiv dword ptr [esp + 0x38]
   00bc0f4e  d99fc0000000           fstp dword ptr [edi + 0xc0]
   00bc0f54  0fb75308               movzx edx, word ptr [ebx + 8]
@@ retail 718-724 built 727-733
   00bc0f4a  da742448               fidiv dword ptr [esp + 0x48]
   00bc0f4e  d99fc0000000           fstp dword ptr [edi + 0xc0]
 - 00bc0f54  0fb75308               movzx edx, word ptr [ebx + 8]
 - 00bc0f58  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f5c  0fb75302               movzx edx, word ptr [ebx + 2]
 - 00bc0f60  db442448               fild dword ptr [esp + 0x48]
 - 00bc0f64  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f68  da742448               fidiv dword ptr [esp + 0x48]
 + 00bc0f61  0fb75608               movzx edx, word ptr [esi + 8]
 + 00bc0f65  89542438               mov dword ptr [esp + 0x38], edx
 + 00bc0f69  0fb75602               movzx edx, word ptr [esi + 2]
 + 00bc0f6d  db442438               fild dword ptr [esp + 0x38]
 + 00bc0f71  89542438               mov dword ptr [esp + 0x38], edx
 + 00bc0f75  da742438               fidiv dword ptr [esp + 0x38]
   00bc0f6c  d99fc4000000           fstp dword ptr [edi + 0xc4]
   00bc0f72  8b5008                 mov edx, dword ptr [eax + 8]
@@ retail 729-730 built 738-739
   00bc0f7b  8b4014                 mov eax, dword ptr [eax + 0x14]
   00bc0f7e  8987cc000000           mov dword ptr [edi + 0xcc], eax
 - 00bc0f84  f6868400000008         test byte ptr [esi + 0x84], 8
 + 00bc0f91  f6858400000008         test byte ptr [ebp + 0x84], 8
   00bc0f8b  7408                   je 0xbc0f95
   00bc0f8d  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 741-742 built 750-751
   00bc0fb4  6a01                   push 1
   00bc0fb6  e81587dcff             call 0x9896d0
 - 00bc0fbb  8a8683000000           mov al, byte ptr [esi + 0x83]
 + 00bc0fc8  8a8583000000           mov al, byte ptr [ebp + 0x83]
   00bc0fc1  84c0                   test al, al
   00bc0fc3  0f86cd020000           jbe 0xbc1296
@@ retail 744-745 built 753-754
   00bc0fc1  84c0                   test al, al
   00bc0fc3  0f86cd020000           jbe 0xbc1296
 - 00bc0fc9  d94660                 fld dword ptr [esi + 0x60]
 + 00bc0fd6  d94560                 fld dword ptr [ebp + 0x60]
   00bc0fcc  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00bc0fd2  dfe0                   fnstsw ax
@@ retail 749-750 built 758-759
   00bc0fd4  f6c441                 test ah, 0x41
   00bc0fd7  0f85b9020000           jne 0xbc1296
 - 00bc0fdd  d94664                 fld dword ptr [esi + 0x64]
 + 00bc0fea  d94564                 fld dword ptr [ebp + 0x64]
   00bc0fe0  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00bc0fe6  dfe0                   fnstsw ax
@@ retail 754-755 built 763-764
   00bc0fe8  f6c441                 test ah, 0x41
   00bc0feb  0f85a5020000           jne 0xbc1296
 - 00bc0ff1  8b4e5c                 mov ecx, dword ptr [esi + 0x5c]
 + 00bc0ffe  8b4d5c                 mov ecx, dword ptr [ebp + 0x5c]
   00bc0ff4  6a00                   push 0
   00bc0ff6  6a00                   push 0
@@ retail 758-759 built 767-768
   00bc0ff6  6a00                   push 0
   00bc0ff8  51                     push ecx
 - 00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
 + 00bc1006  8d542420               lea edx, [esp + 0x20]
   00bc0ffd  52                     push edx
   00bc0ffe  8bcb                   mov ecx, ebx
@@ retail 760-761 built 769-770
   00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
   00bc0ffd  52                     push edx
 - 00bc0ffe  8bcb                   mov ecx, ebx
 + 00bc100b  8bce                   mov ecx, esi
   00bc1000  e8cbe8e3ff             call 0x9ff8d0
   00bc1005  84c0                   test al, al
@@ retail 764-779 built 773-778
   00bc1005  84c0                   test al, al
   00bc1007  0f8489020000           je 0xbc1296
 - 00bc100d  0fb68682000000         movzx eax, byte ptr [esi + 0x82]
 - 00bc1014  0fb68e81000000         movzx ecx, byte ptr [esi + 0x81]
 - 00bc101b  0fb69680000000         movzx edx, byte ptr [esi + 0x80]
 - 00bc1022  89442448               mov dword ptr [esp + 0x48], eax
 - 00bc1026  0fb68683000000         movzx eax, byte ptr [esi + 0x83]
 - 00bc102d  db442448               fild dword ptr [esp + 0x48]
 - 00bc1031  894c2448               mov dword ptr [esp + 0x48], ecx
 - 00bc1035  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc103b  db442448               fild dword ptr [esp + 0x48]
 - 00bc103f  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc1043  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc1049  d95c2434               fstp dword ptr [esp + 0x34]
 - 00bc104d  db442448               fild dword ptr [esp + 0x48]
 - 00bc1051  89442448               mov dword ptr [esp + 0x48], eax
 - 00bc1055  8a442444               mov al, byte ptr [esp + 0x44]
 + 00bc101a  81c580000000           add ebp, 0x80
 + 00bc1020  55                     push ebp
 + 00bc1021  8d4c2424               lea ecx, [esp + 0x24]
 + 00bc1025  e800000000             call 0xbc102a
 + 00bc102a  8a442434               mov al, byte ptr [esp + 0x34]
   00bc1059  84c0                   test al, al
   00bc105b  d80d24172301           fmul dword ptr [0x1231724]
@@ retail 780-784 built 779-779
   00bc1055  8a442444               mov al, byte ptr [esp + 0x44]
   00bc1059  84c0                   test al, al
 - 00bc105b  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc1061  db442448               fild dword ptr [esp + 0x48]
 - 00bc1065  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc106b  d95c243c               fstp dword ptr [esp + 0x3c]
   00bc106f  7406                   je 0xbc1077
   00bc1071  d944243c               fld dword ptr [esp + 0x3c]
@@ retail 785-786 built 780-781
   00bc106b  d95c243c               fstp dword ptr [esp + 0x3c]
   00bc106f  7406                   je 0xbc1077
 - 00bc1071  d944243c               fld dword ptr [esp + 0x3c]
 + 00bc1032  d944242c               fld dword ptr [esp + 0x2c]
   00bc1075  eb06                   jmp 0xbc107d
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 788-788 built 783-784
   00bc1075  eb06                   jmp 0xbc107d
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
 + 00bc103e  d9442420               fld dword ptr [esp + 0x20]
   00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1081  d9ca                   fxch st(2)
@@ retail 789-791 built 785-787
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
   00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
 - 00bc1081  d9ca                   fxch st(2)
 - 00bc1083  d8ca                   fmul st(2)
 + 00bc1046  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc104c  d8c9                   fmul st(1)
   00bc1085  6a00                   push 0
   00bc1087  8d4e48                 lea ecx, [esi + 0x48]
@@ retail 792-797 built 788-792
   00bc1083  d8ca                   fmul st(2)
   00bc1085  6a00                   push 0
 - 00bc1087  8d4e48                 lea ecx, [esi + 0x48]
 - 00bc108a  51                     push ecx
 - 00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc1091  d95c2428               fstp dword ptr [esp + 0x28]
 - 00bc1095  d944243c               fld dword ptr [esp + 0x3c]
 + 00bc1050  8d4648                 lea eax, [esi + 0x48]
 + 00bc1053  d95c2424               fstp dword ptr [esp + 0x24]
 + 00bc1057  50                     push eax
 + 00bc1058  d944242c               fld dword ptr [esp + 0x2c]
   00bc1099  6a04                   push 4
   00bc109b  d8ca                   fmul st(2)
@@ retail 798-799 built 793-794
   00bc1095  d944243c               fld dword ptr [esp + 0x3c]
   00bc1099  6a04                   push 4
 - 00bc109b  d8ca                   fmul st(2)
 + 00bc105e  d8c9                   fmul st(1)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
   00bc10a1  d8c9                   fmul st(1)
@@ retail 800-800 built 795-796
   00bc109b  d8ca                   fmul st(2)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
 + 00bc1064  d9442434               fld dword ptr [esp + 0x34]
   00bc10a1  d8c9                   fmul st(1)
   00bc10a3  d95c2434               fstp dword ptr [esp + 0x34]
@@ retail 806-807 built 802-803
   00bc10ae  d94674                 fld dword ptr [esi + 0x74]
   00bc10b1  d80d10002301           fmul dword ptr [0x1230010]
 - 00bc10b7  d9542444               fst dword ptr [esp + 0x44]
 + 00bc107e  d9542434               fst dword ptr [esp + 0x34]
   00bc10bb  d8259cf52201           fsub dword ptr [0x122f59c]
   00bc10c1  d95c241c               fstp dword ptr [esp + 0x1c]
@@ retail 810-813 built 806-809
   00bc10c1  d95c241c               fstp dword ptr [esp + 0x1c]
   00bc10c5  d944241c               fld dword ptr [esp + 0x1c]
 - 00bc10c9  db5c2448               fistp dword ptr [esp + 0x48]
 - 00bc10cd  db442448               fild dword ptr [esp + 0x48]
 - 00bc10d1  8b442448               mov eax, dword ptr [esp + 0x48]
 + 00bc1090  db5c2438               fistp dword ptr [esp + 0x38]
 + 00bc1094  db442438               fild dword ptr [esp + 0x38]
 + 00bc1098  8b442438               mov eax, dword ptr [esp + 0x38]
   00bc10d5  0500ffffff             add eax, 0xffffff00
   00bc10da  25ff030000             and eax, 0x3ff
@@ retail 815-816 built 811-812
   00bc10d5  0500ffffff             add eax, 0xffffff00
   00bc10da  25ff030000             and eax, 0x3ff
 - 00bc10df  d86c2444               fsubr dword ptr [esp + 0x44]
 + 00bc10a6  d86c2434               fsubr dword ptr [esp + 0x34]
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
@@ retail 818-819 built 814-815
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc10b2  d80c8500000000         fmul dword ptr [eax*4]
   00bc10f2  d9c9                   fxch st(1)
   00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 820-821 built 816-817
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc10bb  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 825-826 built 821-822
   00bc1101  d94674                 fld dword ptr [esi + 0x74]
   00bc1104  d80d10002301           fmul dword ptr [0x1230010]
 - 00bc110a  d9542444               fst dword ptr [esp + 0x44]
 + 00bc10d1  d9542434               fst dword ptr [esp + 0x34]
   00bc110e  d8259cf52201           fsub dword ptr [0x122f59c]
   00bc1114  d95c241c               fstp dword ptr [esp + 0x1c]
@@ retail 829-835 built 825-831
   00bc1114  d95c241c               fstp dword ptr [esp + 0x1c]
   00bc1118  d944241c               fld dword ptr [esp + 0x1c]
 - 00bc111c  db5c2448               fistp dword ptr [esp + 0x48]
 - 00bc1120  db442448               fild dword ptr [esp + 0x48]
 - 00bc1124  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00bc1128  8b5664                 mov edx, dword ptr [esi + 0x64]
 - 00bc112b  8b4e60                 mov ecx, dword ptr [esi + 0x60]
 - 00bc112e  d86c2444               fsubr dword ptr [esp + 0x44]
 + 00bc10e3  db5c2438               fistp dword ptr [esp + 0x38]
 + 00bc10e7  db442438               fild dword ptr [esp + 0x38]
 + 00bc10eb  8b442438               mov eax, dword ptr [esp + 0x38]
 + 00bc10ef  8b4e64                 mov ecx, dword ptr [esi + 0x64]
 + 00bc10f2  8b5660                 mov edx, dword ptr [esi + 0x60]
 + 00bc10f5  d86c2434               fsubr dword ptr [esp + 0x34]
   00bc1132  25ff030000             and eax, 0x3ff
   00bc1137  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 837-839 built 833-835
   00bc1132  25ff030000             and eax, 0x3ff
   00bc1137  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc113d  52                     push edx
 - 00bc113e  8b54241c               mov edx, dword ptr [esp + 0x1c]
 + 00bc1104  51                     push ecx
 + 00bc1105  8b4c241c               mov ecx, dword ptr [esp + 0x1c]
   00bc1142  d8e1                   fsub st(1)
   00bc1144  51                     push ecx
@@ retail 840-841 built 836-836
   00bc113e  8b54241c               mov edx, dword ptr [esp + 0x1c]
   00bc1142  d8e1                   fsub st(1)
 - 00bc1144  51                     push ecx
   00bc1145  52                     push edx
   00bc1146  51                     push ecx
@@ retail 843-843 built 838-839
   00bc1145  52                     push edx
   00bc1146  51                     push ecx
 + 00bc110d  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
@@ retail 844-845 built 840-841
   00bc1146  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc1114  d80c8500000000         fmul dword ptr [eax*4]
   00bc1154  d9c9                   fxch st(1)
   00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 846-847 built 842-843
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc111d  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 851-853 built 847-851
   00bc1162  6a05                   push 5
   00bc1164  e89789dcff             call 0x989b00
 - 00bc1169  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc116d  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 + 00bc1130  8b54242c               mov edx, dword ptr [esp + 0x2c]
 + 00bc1134  8b442428               mov eax, dword ptr [esp + 0x28]
 + 00bc1138  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 + 00bc113c  52                     push edx
   00bc1171  8b542424               mov edx, dword ptr [esp + 0x24]
   00bc1175  50                     push eax
@@ retail 855-856 built 853-853
   00bc1171  8b542424               mov edx, dword ptr [esp + 0x24]
   00bc1175  50                     push eax
 - 00bc1176  8b442424               mov eax, dword ptr [esp + 0x24]
   00bc117a  51                     push ecx
   00bc117b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 859-860 built 856-856
   00bc117b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc1181  52                     push edx
 - 00bc1182  50                     push eax
   00bc1183  6a06                   push 6
   00bc1185  e87689dcff             call 0x989b00
@@ retail 862-863 built 858-859
   00bc1183  6a06                   push 6
   00bc1185  e87689dcff             call 0x989b00
 - 00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc1151  8b442414               mov eax, dword ptr [esp + 0x14]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
   00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
@@ retail 864-865 built 860-860
   00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
 - 00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
   00bc1196  6a00                   push 0
   00bc1198  db442448               fild dword ptr [esp + 0x48]
@@ retail 866-867 built 861-862
   00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
   00bc1196  6a00                   push 0
 - 00bc1198  db442448               fild dword ptr [esp + 0x48]
 + 00bc115b  894c2438               mov dword ptr [esp + 0x38], ecx
   00bc119c  6a00                   push 0
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
@@ retail 868-868 built 863-864
   00bc1198  db442448               fild dword ptr [esp + 0x48]
   00bc119c  6a00                   push 0
 + 00bc1161  db44243c               fild dword ptr [esp + 0x3c]
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
@@ retail 872-873 built 868-869
   00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc11ae  83ec08                 sub esp, 8
 - 00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
 + 00bc1178  89542444               mov dword ptr [esp + 0x44], edx
   00bc11b5  d95c2404               fstp dword ptr [esp + 4]
   00bc11b9  db442454               fild dword ptr [esp + 0x54]
@@ retail 874-875 built 870-871
   00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
   00bc11b5  d95c2404               fstp dword ptr [esp + 4]
 - 00bc11b9  db442454               fild dword ptr [esp + 0x54]
 + 00bc1180  db442444               fild dword ptr [esp + 0x44]
   00bc11bd  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11c3  d91c24                 fstp dword ptr [esp]
@@ retail 944-980 built 940-940
   00bc128f  6a00                   push 0
   00bc1291  e8ca99e4ff             call 0xa0ac60
 - 00bc1296  a19c6e4301             mov eax, dword ptr [0x1436e9c]
 - 00bc129b  8a88ce000000           mov cl, byte ptr [eax + 0xce]
 - 00bc12a1  84c9                   test cl, cl
 - 00bc12a3  7416                   je 0xbc12bb
 - 00bc12a5  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
 - 00bc12ab  83c901                 or ecx, 1
 - 00bc12ae  c680ce00000000         mov byte ptr [eax + 0xce], 0
 - 00bc12b5  8988e4000000           mov dword ptr [eax + 0xe4], ecx
 - 00bc12bb  8a88e0000000           mov cl, byte ptr [eax + 0xe0]
 - 00bc12c1  84c9                   test cl, cl
 - 00bc12c3  c644244400             mov byte ptr [esp + 0x44], 0
 - 00bc12c8  c644244500             mov byte ptr [esp + 0x45], 0
 - 00bc12cd  c644244600             mov byte ptr [esp + 0x46], 0
 - 00bc12d2  7514                   jne 0xbc12e8
 - 00bc12d4  8a88e1000000           mov cl, byte ptr [eax + 0xe1]
 - 00bc12da  84c9                   test cl, cl
 - 00bc12dc  750a                   jne 0xbc12e8
 - 00bc12de  8a88e2000000           mov cl, byte ptr [eax + 0xe2]
 - 00bc12e4  84c9                   test cl, cl
 - 00bc12e6  742d                   je 0xbc1315
 - 00bc12e8  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 - 00bc12ec  8988e0000000           mov dword ptr [eax + 0xe0], ecx
 - 00bc12f2  80b8e2000000ff         cmp byte ptr [eax + 0xe2], 0xff
 - 00bc12f9  7513                   jne 0xbc130e
 - 00bc12fb  8a90e1000000           mov dl, byte ptr [eax + 0xe1]
 - 00bc1301  8890e0000000           mov byte ptr [eax + 0xe0], dl
 - 00bc1307  c680e200000000         mov byte ptr [eax + 0xe2], 0
 - 00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
 - 00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 - 00bc131f  7415                   je 0xbc1336
 - 00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
 - 00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 - 00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
@@ retail 982-983 built 942-946
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
 - 00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00bc1268  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc126e  e800000000             call 0xbc1273
 + 00bc1273  33ff                   xor edi, edi
 + 00bc1275  8b742414               mov esi, dword ptr [esp + 0x14]
   00bc1345  3bf7                   cmp esi, edi
   00bc1347  7444                   je 0xbc138d
@@ retail 1005-1006 built 968-968
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
@@ retail 1008-1008 built 970-971
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
 + 00bc12b6  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
@@ retail 1011-1011 built 974-975
   00bc138a  83c404                 add esp, 4
   00bc138d  5e                     pop esi
 + 00bc12c2  5b                     pop ebx
   00bc138e  5d                     pop ebp
   00bc138f  5f                     pop edi
@@ retail 1013-1015 built 977-978
   00bc138e  5d                     pop ebp
   00bc138f  5f                     pop edi
 - 00bc1390  5b                     pop ebx
 - 00bc1391  83c430                 add esp, 0x30
 + 00bc12c5  83c420                 add esp, 0x20
   00bc1394  c20800                 ret 8
DIFFLINES 663 retail_instr 1016 built_instr 979
