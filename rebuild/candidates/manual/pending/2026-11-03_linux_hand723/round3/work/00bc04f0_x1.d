00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(2903v3751) flags= []
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
 + 00bc051f  896c2410               mov dword ptr [esp + 0x10], ebp
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
 + 00bc0571  8d542424               lea edx, [esp + 0x24]
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
@@ retail 46-47 built 45-46
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00bc0580  897c2420               mov dword ptr [esp + 0x20], edi
 + 00bc057a  897c2428               mov dword ptr [esp + 0x28], edi
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
 + 00bc05be  8b442418               mov eax, dword ptr [esp + 0x18]
   00bc05c3  8b4808                 mov ecx, dword ptr [eax + 8]
   00bc05c6  3bcf                   cmp ecx, edi
@@ retail 86-87 built 86-87
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
   00bc0610  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0615  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00bc0614  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00bc0619  83f906                 cmp ecx, 6
   00bc061c  0f85a1010000           jne 0xbc07c3
@@ retail 94-124 built 94-97
   00bc062d  0f8472030000           je 0xbc09a5
   00bc0633  8b35d06e3901           mov esi, dword ptr [0x1396ed0]
 - 00bc0639  39b0402c0000           cmp dword ptr [eax + 0x2c40], esi
 - 00bc063f  8d883c2c0000           lea ecx, [eax + 0x2c3c]
 - 00bc0645  c644244400             mov byte ptr [esp + 0x44], 0
 - 00bc064a  8bd0                   mov edx, eax
 - 00bc064c  8bfe                   mov edi, esi
 - 00bc064e  7455                   je 0xbc06a5
 - 00bc0650  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0656  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0659  85f0                   test eax, esi
 - 00bc065b  7522                   jne 0xbc067f
 - 00bc065d  0bf0                   or esi, eax
 - 00bc065f  897108                 mov dword ptr [ecx + 8], esi
 - 00bc0662  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0668  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 - 00bc066c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0672  8b7104                 mov esi, dword ptr [ecx + 4]
 - 00bc0675  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 - 00bc0679  ff8208280000           inc dword ptr [edx + 0x2808]
 - 00bc067f  8a4110                 mov al, byte ptr [ecx + 0x10]
 - 00bc0682  84c0                   test al, al
 - 00bc0684  897904                 mov dword ptr [ecx + 4], edi
 - 00bc0687  7517                   jne 0xbc06a0
 - 00bc0689  c6411001               mov byte ptr [ecx + 0x10], 1
 - 00bc068d  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 - 00bc0693  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 - 00bc069a  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc06a0  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc06a5  8b35786f3901           mov esi, dword ptr [0x1396f78]
 - 00bc06ab  39b020290000           cmp dword ptr [eax + 0x2920], esi
 - 00bc06b1  8d901c290000           lea edx, [eax + 0x291c]
 + 00bc0638  8b98402c0000           mov ebx, dword ptr [eax + 0x2c40]
 + 00bc063e  3bde                   cmp ebx, esi
 + 00bc0640  8d903c2c0000           lea edx, [eax + 0x2c3c]
   00bc06b7  8bc8                   mov ecx, eax
   00bc06b9  8bfe                   mov edi, esi
@@ retail 129-130 built 102-103
   00bc06bd  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc06c3  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc06c6  85f0                   test eax, esi
 + 00bc0655  85c6                   test esi, eax
   00bc06c8  7522                   jne 0xbc06ec
   00bc06ca  0bf0                   or esi, eax
@@ retail 131-133 built 104-106
   00bc06c6  85f0                   test eax, esi
   00bc06c8  7522                   jne 0xbc06ec
 - 00bc06ca  0bf0                   or esi, eax
 - 00bc06cc  897208                 mov dword ptr [edx + 8], esi
 + 00bc0659  0bc6                   or eax, esi
 + 00bc065b  894208                 mov dword ptr [edx + 8], eax
   00bc06cf  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc06d5  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 148-150 built 121-152
   00bc0707  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc070d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0712  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 - 00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
 + 00bc06a1  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc06a7  8b9820290000           mov ebx, dword ptr [eax + 0x2920]
 + 00bc06ad  3bde                   cmp ebx, esi
 + 00bc06af  8d901c290000           lea edx, [eax + 0x291c]
 + 00bc06b5  8bc8                   mov ecx, eax
 + 00bc06b7  8bfe                   mov edi, esi
 + 00bc06b9  7455                   je 0xbc0710
 + 00bc06bb  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc06c1  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc06c4  85c6                   test esi, eax
 + 00bc06c6  7522                   jne 0xbc06ea
 + 00bc06c8  0bc6                   or eax, esi
 + 00bc06ca  894208                 mov dword ptr [edx + 8], eax
 + 00bc06cd  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc06d3  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc06d7  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc06dd  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc06e0  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc06e4  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc06ea  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc06ed  84c0                   test al, al
 + 00bc06ef  897a04                 mov dword ptr [edx + 4], edi
 + 00bc06f2  7517                   jne 0xbc070b
 + 00bc06f4  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc06f8  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc06fe  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc0705  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc070b  a100000000             mov eax, dword ptr [0]
 + 00bc0710  8b3514000000           mov esi, dword ptr [0x14]
 + 00bc0716  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 + 00bc071c  3bde                   cmp ebx, esi
   00bc071e  8d9030290000           lea edx, [eax + 0x2930]
   00bc0724  8bc8                   mov ecx, eax
@@ retail 178-179 built 180-182
   00bc0785  f6c104                 test cl, 4
   00bc0788  0f8499010000           je 0xbc0927
 - 00bc078e  8d4b40                 lea ecx, [ebx + 0x40]
 + 00bc078e  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc0792  83c140                 add ecx, 0x40
   00bc0791  6a00                   push 0
   00bc0793  51                     push ecx
@@ retail 182-182 built 185-186
   00bc0793  51                     push ecx
   00bc0794  eb06                   jmp 0xbc079c
 + 00bc079a  8b542410               mov edx, dword ptr [esp + 0x10]
   00bc0796  6a00                   push 0
   00bc0798  8d5330                 lea edx, [ebx + 0x30]
@@ retail 183-184 built 187-188
   00bc0794  eb06                   jmp 0xbc079c
   00bc0796  6a00                   push 0
 - 00bc0798  8d5330                 lea edx, [ebx + 0x30]
 + 00bc07a0  83c230                 add edx, 0x30
   00bc079b  52                     push edx
   00bc079c  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 187-187 built 191-192
   00bc079c  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc07a2  e87978dcff             call 0x988020
 + 00bc07af  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc07a7  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc07ad  83c358                 add ebx, 0x58
@@ retail 188-190 built 193-195
   00bc07a2  e87978dcff             call 0x988020
   00bc07a7  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc07ad  83c358                 add ebx, 0x58
 - 00bc07b0  53                     push ebx
 + 00bc07b9  83c058                 add eax, 0x58
 + 00bc07bc  50                     push eax
   00bc07b1  e88a79dcff             call 0x988140
   00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
@@ retail 191-193 built 196-198
   00bc07b0  53                     push ebx
   00bc07b1  e88a79dcff             call 0x988140
 - 00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc07ba  8b742418               mov esi, dword ptr [esp + 0x18]
 + 00bc07c2  8b742414               mov esi, dword ptr [esp + 0x14]
 + 00bc07c6  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc07be  e9be060000             jmp 0xbc0e81
   00bc07c3  83f907                 cmp ecx, 7
@@ retail 199-200 built 204-204
   00bc07d2  3bf7                   cmp esi, edi
   00bc07d4  8d90cc280000           lea edx, [eax + 0x28cc]
 - 00bc07da  c644244400             mov byte ptr [esp + 0x44], 0
   00bc07df  8bc8                   mov ecx, eax
   00bc07e1  7455                   je 0xbc0838
@@ retail 224-224 built 228-229
   00bc0833  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0838  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
 + 00bc0845  3bf3                   cmp esi, ebx
   00bc083e  8d90e0280000           lea edx, [eax + 0x28e0]
   00bc0844  bf10000000             mov edi, 0x10
@@ retail 225-227 built 230-230
   00bc0838  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
   00bc083e  8d90e0280000           lea edx, [eax + 0x28e0]
 - 00bc0844  bf10000000             mov edi, 0x10
 - 00bc0849  3bf7                   cmp esi, edi
   00bc084b  8bc8                   mov ecx, eax
   00bc084d  7455                   je 0xbc08a4
@@ retail 243-244 built 246-247
   00bc087e  8a4210                 mov al, byte ptr [edx + 0x10]
   00bc0881  84c0                   test al, al
 - 00bc0883  897a04                 mov dword ptr [edx + 4], edi
 + 00bc0885  895a04                 mov dword ptr [edx + 4], ebx
   00bc0886  7517                   jne 0xbc089f
   00bc0888  c6421001               mov byte ptr [edx + 0x10], 1
@@ retail 250-252 built 253-256
   00bc0899  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc089f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc08a4  8b35e86d3901           mov esi, dword ptr [0x1396de8]
 - 00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
 + 00bc08a6  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc08ac  8b98a8280000           mov ebx, dword ptr [eax + 0x28a8]
 + 00bc08b2  3bde                   cmp ebx, esi
   00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
   00bc08b6  8bc8                   mov ecx, eax
@@ retail 280-280 built 284-285
   00bc0917  f6c104                 test cl, 4
   00bc091a  740b                   je 0xbc0927
 + 00bc0920  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc091c  6a00                   push 0
   00bc091e  8d4b40                 lea ecx, [ebx + 0x40]
@@ retail 281-282 built 286-287
   00bc091a  740b                   je 0xbc0927
   00bc091c  6a00                   push 0
 - 00bc091e  8d4b40                 lea ecx, [ebx + 0x40]
 + 00bc0926  83c140                 add ecx, 0x40
   00bc0921  51                     push ecx
   00bc0922  e975feffff             jmp 0xbc079c
@@ retail 286-289 built 291-294
   00bc0927  f6c110                 test cl, 0x10
   00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 - 00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
 - 00bc093c  3bee                   cmp ebp, esi
 + 00bc0938  8b3508000000           mov esi, dword ptr [8]
 + 00bc093e  8b9894280000           mov ebx, dword ptr [eax + 0x2894]
 + 00bc0944  3bde                   cmp ebx, esi
   00bc093e  8d8890280000           lea ecx, [eax + 0x2890]
   00bc0944  8bd0                   mov edx, eax
@@ retail 313-314 built 318-320
   00bc098d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
   00bc0994  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc099a  8d4b38                 lea ecx, [ebx + 0x38]
 + 00bc09a2  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc09a6  83c138                 add ecx, 0x38
   00bc099d  6a00                   push 0
   00bc099f  51                     push ecx
@@ retail 326-327 built 332-333
   00bc09be  8bc8                   mov ecx, eax
   00bc09c0  7510                   jne 0xbc09d2
 - 00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
 + 00bc09ce  8b3508000000           mov esi, dword ptr [8]
   00bc09c8  eb08                   jmp 0xbc09d2
   00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
@@ retail 328-329 built 334-335
   00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
   00bc09c8  eb08                   jmp 0xbc09d2
 - 00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
 + 00bc09d6  8b3504000000           mov esi, dword ptr [4]
   00bc09d0  8bc8                   mov ecx, eax
   00bc09d2  3bde                   cmp ebx, esi
@@ retail 362-363 built 368-369
   00bc0a46  8bc8                   mov ecx, eax
   00bc0a48  0f84d5000000           je 0xbc0b23
 - 00bc0a4e  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc0a5a  8b3510000000           mov esi, dword ptr [0x10]
   00bc0a54  3bde                   cmp ebx, esi
   00bc0a56  8bfe                   mov edi, esi
@@ retail 387-388 built 393-394
   00bc0aa4  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0aaa  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0aaf  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0abb  8b3514000000           mov esi, dword ptr [0x14]
   00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0abb  3bde                   cmp ebx, esi
@@ retail 400-415 built 406-406
   00bc0ad6  0bf0                   or esi, eax
   00bc0ad8  897108                 mov dword ptr [ecx + 8], esi
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
   00bc0b1e  e96c010000             jmp 0xbc0c8f
   00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
@@ retail 416-417 built 407-408
   00bc0b19  c644244400             mov byte ptr [esp + 0x44], 0
   00bc0b1e  e96c010000             jmp 0xbc0c8f
 - 00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0af4  8b3504000000           mov esi, dword ptr [4]
   00bc0b29  3bde                   cmp ebx, esi
   00bc0b2b  8bfe                   mov edi, esi
@@ retail 422-423 built 413-414
   00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0b35  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0b38  85f0                   test eax, esi
 + 00bc0b09  85c6                   test esi, eax
   00bc0b3a  7522                   jne 0xbc0b5e
   00bc0b3c  0bf0                   or esi, eax
@@ retail 424-426 built 415-417
   00bc0b38  85f0                   test eax, esi
   00bc0b3a  7522                   jne 0xbc0b5e
 - 00bc0b3c  0bf0                   or esi, eax
 - 00bc0b3e  897208                 mov dword ptr [edx + 8], esi
 + 00bc0b0d  0bc6                   or eax, esi
 + 00bc0b0f  894208                 mov dword ptr [edx + 8], eax
   00bc0b41  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0b47  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 441-454 built 432-433
   00bc0b79  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0b7f  a1186e4301             mov eax, dword ptr [0x1436e18]
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
 + 00bc0b55  8b350c000000           mov esi, dword ptr [0xc]
   00bc0bb8  e98f000000             jmp 0xbc0c4c
   00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
@@ retail 455-456 built 434-435
   00bc0bb5  897108                 mov dword ptr [ecx + 8], esi
   00bc0bb8  e98f000000             jmp 0xbc0c4c
 - 00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0b5d  8b3504000000           mov esi, dword ptr [4]
   00bc0bc3  3bde                   cmp ebx, esi
   00bc0bc5  8bc8                   mov ecx, eax
@@ retail 481-482 built 460-461
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0bc0  8b3504000000           mov esi, dword ptr [4]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0c2c  3bde                   cmp ebx, esi
@@ retail 508-509 built 487-487
   00bc0c7d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
   00bc0c84  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc0c8a  c644244401             mov byte ptr [esp + 0x44], 1
   00bc0c8f  8a8584000000           mov al, byte ptr [ebp + 0x84]
   00bc0c95  8b0d846e4301           mov ecx, dword ptr [0x1436e84]
@@ retail 521-531 built 499-510
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
 + 00bc0c4e  8b454c                 mov eax, dword ptr [ebp + 0x4c]
 + 00bc0c51  8b4d50                 mov ecx, dword ptr [ebp + 0x50]
 + 00bc0c54  8b5548                 mov edx, dword ptr [ebp + 0x48]
 + 00bc0c57  89442424               mov dword ptr [esp + 0x24], eax
 + 00bc0c5b  8d442434               lea eax, [esp + 0x34]
 + 00bc0c5f  50                     push eax
 + 00bc0c60  894c242c               mov dword ptr [esp + 0x2c], ecx
 + 00bc0c64  6a05                   push 5
 + 00bc0c66  8d4c2428               lea ecx, [esp + 0x28]
 + 00bc0c6a  89542428               mov dword ptr [esp + 0x28], edx
 + 00bc0c6e  8b5570                 mov edx, dword ptr [ebp + 0x70]
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 533-540 built 512-516
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0ce2  8954242c               mov dword ptr [esp + 0x2c], edx
 - 00bc0ce6  89442424               mov dword ptr [esp + 0x24], eax
 - 00bc0cea  8b4570                 mov eax, dword ptr [ebp + 0x70]
 - 00bc0ced  6a05                   push 5
 - 00bc0cef  8d542428               lea edx, [esp + 0x28]
 - 00bc0cf3  52                     push edx
 - 00bc0cf4  89442438               mov dword ptr [esp + 0x38], eax
 + 00bc0c78  c644244000             mov byte ptr [esp + 0x40], 0
 + 00bc0c7d  c644244100             mov byte ptr [esp + 0x41], 0
 + 00bc0c82  c644244200             mov byte ptr [esp + 0x42], 0
 + 00bc0c87  89542438               mov dword ptr [esp + 0x38], edx
   00bc0cf8  e8935ef8ff             call 0xb46b90
   00bc0cfd  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 558-559 built 534-535
   00bc0d3f  a804                   test al, 4
   00bc0d41  7429                   je 0xbc0d6c
 - 00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0cd6  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
   00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
@@ retail 577-578 built 553-554
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 + 00bc0d0d  8b1508000000           mov edx, dword ptr [8]
   00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
   00bc0d86  3bfa                   cmp edi, edx
@@ retail 583-583 built 559-560
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 + 00bc0d25  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
   00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
@@ retail 584-585 built 561-561
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
@@ retail 603-604 built 579-580
   00bc0dd5  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00bc0ddc  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0de2  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0d75  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc0de6  8b4f1c                 mov ecx, dword ptr [edi + 0x1c]
   00bc0de9  8b6f18                 mov ebp, dword ptr [edi + 0x18]
@@ retail 624-625 built 600-601
   00bc0e12  e80972dcff             call 0x988020
   00bc0e17  eb39                   jmp 0xbc0e52
 - 00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0dac  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
   00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
@@ retail 644-646 built 620-622
   00bc0e48  52                     push edx
   00bc0e49  e8d271dcff             call 0x988020
 - 00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0e52  8b742418               mov esi, dword ptr [esp + 0x18]
 + 00bc0de1  8b7c2410               mov edi, dword ptr [esp + 0x10]
 + 00bc0de5  8b742414               mov esi, dword ptr [esp + 0x14]
   00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
   00bc0e5d  7413                   je 0xbc0e72
@@ retail 662-663 built 638-640
   00bc0e81  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc0e87  8d4760                 lea eax, [edi + 0x60]
 - 00bc0e8a  85c0                   test eax, eax
 + 00bc0e1d  33db                   xor ebx, ebx
 + 00bc0e1f  3bc3                   cmp eax, ebx
   00bc0e8c  7418                   je 0xbc0ea6
   00bc0e8e  0fb65810               movzx ebx, byte ptr [eax + 0x10]
@@ retail 664-665 built 641-642
   00bc0e8a  85c0                   test eax, eax
   00bc0e8c  7418                   je 0xbc0ea6
 - 00bc0e8e  0fb65810               movzx ebx, byte ptr [eax + 0x10]
 + 00bc0e23  0fb66810               movzx ebp, byte ptr [eax + 0x10]
   00bc0e92  8b4004                 mov eax, dword ptr [eax + 4]
   00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
@@ retail 668-668 built 645-646
   00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
   00bc0e9b  8b11                   mov edx, dword ptr [ecx]
 + 00bc0e32  55                     push ebp
   00bc0e9d  53                     push ebx
   00bc0e9e  6a00                   push 0
@@ retail 669-670 built 647-647
   00bc0e9b  8b11                   mov edx, dword ptr [ecx]
   00bc0e9d  53                     push ebx
 - 00bc0e9e  6a00                   push 0
   00bc0ea0  50                     push eax
   00bc0ea1  6a00                   push 0
@@ retail 671-672 built 648-649
   00bc0e9e  6a00                   push 0
   00bc0ea0  50                     push eax
 - 00bc0ea1  6a00                   push 0
 + 00bc0e35  53                     push ebx
   00bc0ea3  51                     push ecx
   00bc0ea4  eb11                   jmp 0xbc0eb7
@@ retail 676-680 built 653-657
   00bc0ea6  8b81f03c0000           mov eax, dword ptr [ecx + 0x3cf0]
   00bc0eac  8b10                   mov edx, dword ptr [eax]
 - 00bc0eae  6a00                   push 0
 - 00bc0eb0  6a00                   push 0
 - 00bc0eb2  6a00                   push 0
 - 00bc0eb4  6a00                   push 0
 + 00bc0e41  53                     push ebx
 + 00bc0e42  53                     push ebx
 + 00bc0e43  53                     push ebx
 + 00bc0e44  53                     push ebx
   00bc0eb6  50                     push eax
   00bc0eb7  ff9290010000           call dword ptr [edx + 0x190]
@@ retail 684-685 built 661-662
   00bc0ebd  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc0ec3  8d8794000000           lea eax, [edi + 0x94]
 - 00bc0ec9  85c0                   test eax, eax
 + 00bc0e58  3bc3                   cmp eax, ebx
   00bc0ecb  8981f43c0000           mov dword ptr [ecx + 0x3cf4], eax
   00bc0ed1  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
@@ retail 692-693 built 669-670
   00bc0ede  50                     push eax
   00bc0edf  eb02                   jmp 0xbc0ee3
 - 00bc0ee1  6a00                   push 0
 + 00bc0e70  53                     push ebx
   00bc0ee3  51                     push ecx
   00bc0ee4  ff92a0010000           call dword ptr [edx + 0x1a0]
@@ retail 697-698 built 674-674
   00bc0eea  a1a06e4301             mov eax, dword ptr [0x1436ea0]
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
 - 00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
@@ retail 700-700 built 676-677
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 + 00bc0e8f  0544010000             add eax, 0x144
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
@@ retail 702-703 built 679-679
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
@@ retail 706-707 built 682-683
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0eac  8997b4000000           mov dword ptr [edi + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 711-717 built 687-694
   00bc0f2e  8b5010                 mov edx, dword ptr [eax + 0x10]
   00bc0f31  8997bc000000           mov dword ptr [edi + 0xbc], edx
 - 00bc0f37  0fb75306               movzx edx, word ptr [ebx + 6]
 - 00bc0f3b  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f3f  0fb713                 movzx edx, word ptr [ebx]
 - 00bc0f42  db442448               fild dword ptr [esp + 0x48]
 - 00bc0f46  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f4a  da742448               fidiv dword ptr [esp + 0x48]
 + 00bc0ec4  8b54241c               mov edx, dword ptr [esp + 0x1c]
 + 00bc0ec8  0fb76a06               movzx ebp, word ptr [edx + 6]
 + 00bc0ecc  896c2434               mov dword ptr [esp + 0x34], ebp
 + 00bc0ed0  0fb72a                 movzx ebp, word ptr [edx]
 + 00bc0ed3  db442434               fild dword ptr [esp + 0x34]
 + 00bc0ed7  896c2434               mov dword ptr [esp + 0x34], ebp
 + 00bc0edb  da742434               fidiv dword ptr [esp + 0x34]
   00bc0f4e  d99fc0000000           fstp dword ptr [edi + 0xc0]
   00bc0f54  0fb75308               movzx edx, word ptr [ebx + 8]
@@ retail 718-724 built 695-701
   00bc0f4a  da742448               fidiv dword ptr [esp + 0x48]
   00bc0f4e  d99fc0000000           fstp dword ptr [edi + 0xc0]
 - 00bc0f54  0fb75308               movzx edx, word ptr [ebx + 8]
 - 00bc0f58  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f5c  0fb75302               movzx edx, word ptr [ebx + 2]
 - 00bc0f60  db442448               fild dword ptr [esp + 0x48]
 - 00bc0f64  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f68  da742448               fidiv dword ptr [esp + 0x48]
 + 00bc0ee5  0fb76a08               movzx ebp, word ptr [edx + 8]
 + 00bc0ee9  0fb75202               movzx edx, word ptr [edx + 2]
 + 00bc0eed  896c2434               mov dword ptr [esp + 0x34], ebp
 + 00bc0ef1  db442434               fild dword ptr [esp + 0x34]
 + 00bc0ef5  89542434               mov dword ptr [esp + 0x34], edx
 + 00bc0ef9  da742434               fidiv dword ptr [esp + 0x34]
   00bc0f6c  d99fc4000000           fstp dword ptr [edi + 0xc4]
   00bc0f72  8b5008                 mov edx, dword ptr [eax + 8]
@@ retail 737-738 built 714-715
   00bc0f9c  d99fd0000000           fstp dword ptr [edi + 0xd0]
   00bc0fa2  6a03                   push 3
 - 00bc0fa4  c787d400000000000000   mov dword ptr [edi + 0xd4], 0
 + 00bc0f35  899fd4000000           mov dword ptr [edi + 0xd4], ebx
   00bc0fae  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0fb4  6a01                   push 1
@@ retail 740-943 built 717-717
   00bc0fae  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0fb4  6a01                   push 1
 - 00bc0fb6  e81587dcff             call 0x9896d0
 - 00bc0fbb  8a8683000000           mov al, byte ptr [esi + 0x83]
 - 00bc0fc1  84c0                   test al, al
 - 00bc0fc3  0f86cd020000           jbe 0xbc1296
 - 00bc0fc9  d94660                 fld dword ptr [esi + 0x60]
 - 00bc0fcc  d81ddcde2201           fcomp dword ptr [0x122dedc]
 - 00bc0fd2  dfe0                   fnstsw ax
 - 00bc0fd4  f6c441                 test ah, 0x41
 - 00bc0fd7  0f85b9020000           jne 0xbc1296
 - 00bc0fdd  d94664                 fld dword ptr [esi + 0x64]
 - 00bc0fe0  d81ddcde2201           fcomp dword ptr [0x122dedc]
 - 00bc0fe6  dfe0                   fnstsw ax
 - 00bc0fe8  f6c441                 test ah, 0x41
 - 00bc0feb  0f85a5020000           jne 0xbc1296
 - 00bc0ff1  8b4e5c                 mov ecx, dword ptr [esi + 0x5c]
 - 00bc0ff4  6a00                   push 0
 - 00bc0ff6  6a00                   push 0
 - 00bc0ff8  51                     push ecx
 - 00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
 - 00bc0ffd  52                     push edx
 - 00bc0ffe  8bcb                   mov ecx, ebx
 - 00bc1000  e8cbe8e3ff             call 0x9ff8d0
 - 00bc1005  84c0                   test al, al
 - 00bc1007  0f8489020000           je 0xbc1296
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
 - 00bc1059  84c0                   test al, al
 - 00bc105b  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc1061  db442448               fild dword ptr [esp + 0x48]
 - 00bc1065  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc106b  d95c243c               fstp dword ptr [esp + 0x3c]
 - 00bc106f  7406                   je 0xbc1077
 - 00bc1071  d944243c               fld dword ptr [esp + 0x3c]
 - 00bc1075  eb06                   jmp 0xbc107d
 - 00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
 - 00bc1081  d9ca                   fxch st(2)
 - 00bc1083  d8ca                   fmul st(2)
 - 00bc1085  6a00                   push 0
 - 00bc1087  8d4e48                 lea ecx, [esi + 0x48]
 - 00bc108a  51                     push ecx
 - 00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc1091  d95c2428               fstp dword ptr [esp + 0x28]
 - 00bc1095  d944243c               fld dword ptr [esp + 0x3c]
 - 00bc1099  6a04                   push 4
 - 00bc109b  d8ca                   fmul st(2)
 - 00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
 - 00bc10a1  d8c9                   fmul st(1)
 - 00bc10a3  d95c2434               fstp dword ptr [esp + 0x34]
 - 00bc10a7  ddd8                   fstp st(0)
 - 00bc10a9  e8b289dcff             call 0x989a60
 - 00bc10ae  d94674                 fld dword ptr [esi + 0x74]
 - 00bc10b1  d80d10002301           fmul dword ptr [0x1230010]
 - 00bc10b7  d9542444               fst dword ptr [esp + 0x44]
 - 00bc10bb  d8259cf52201           fsub dword ptr [0x122f59c]
 - 00bc10c1  d95c241c               fstp dword ptr [esp + 0x1c]
 - 00bc10c5  d944241c               fld dword ptr [esp + 0x1c]
 - 00bc10c9  db5c2448               fistp dword ptr [esp + 0x48]
 - 00bc10cd  db442448               fild dword ptr [esp + 0x48]
 - 00bc10d1  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00bc10d5  0500ffffff             add eax, 0xffffff00
 - 00bc10da  25ff030000             and eax, 0x3ff
 - 00bc10df  d86c2444               fsubr dword ptr [esp + 0x44]
 - 00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 - 00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 - 00bc10fb  dec1                   faddp st(1)
 - 00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
 - 00bc1101  d94674                 fld dword ptr [esi + 0x74]
 - 00bc1104  d80d10002301           fmul dword ptr [0x1230010]
 - 00bc110a  d9542444               fst dword ptr [esp + 0x44]
 - 00bc110e  d8259cf52201           fsub dword ptr [0x122f59c]
 - 00bc1114  d95c241c               fstp dword ptr [esp + 0x1c]
 - 00bc1118  d944241c               fld dword ptr [esp + 0x1c]
 - 00bc111c  db5c2448               fistp dword ptr [esp + 0x48]
 - 00bc1120  db442448               fild dword ptr [esp + 0x48]
 - 00bc1124  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00bc1128  8b5664                 mov edx, dword ptr [esi + 0x64]
 - 00bc112b  8b4e60                 mov ecx, dword ptr [esi + 0x60]
 - 00bc112e  d86c2444               fsubr dword ptr [esp + 0x44]
 - 00bc1132  25ff030000             and eax, 0x3ff
 - 00bc1137  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc113d  52                     push edx
 - 00bc113e  8b54241c               mov edx, dword ptr [esp + 0x1c]
 - 00bc1142  d8e1                   fsub st(1)
 - 00bc1144  51                     push ecx
 - 00bc1145  52                     push edx
 - 00bc1146  51                     push ecx
 - 00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 - 00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 - 00bc115d  dec1                   faddp st(1)
 - 00bc115f  d91c24                 fstp dword ptr [esp]
 - 00bc1162  6a05                   push 5
 - 00bc1164  e89789dcff             call 0x989b00
 - 00bc1169  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc116d  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 - 00bc1171  8b542424               mov edx, dword ptr [esp + 0x24]
 - 00bc1175  50                     push eax
 - 00bc1176  8b442424               mov eax, dword ptr [esp + 0x24]
 - 00bc117a  51                     push ecx
 - 00bc117b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc1181  52                     push edx
 - 00bc1182  50                     push eax
 - 00bc1183  6a06                   push 6
 - 00bc1185  e87689dcff             call 0x989b00
 - 00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
 - 00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
 - 00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
 - 00bc1196  6a00                   push 0
 - 00bc1198  db442448               fild dword ptr [esp + 0x48]
 - 00bc119c  6a00                   push 0
 - 00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
 - 00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
 - 00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc11ae  83ec08                 sub esp, 8
 - 00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
 - 00bc11b5  d95c2404               fstp dword ptr [esp + 4]
 - 00bc11b9  db442454               fild dword ptr [esp + 0x54]
 - 00bc11bd  d80d1cc82901           fmul dword ptr [0x129c81c]
 - 00bc11c3  d91c24                 fstp dword ptr [esp]
 - 00bc11c6  6a07                   push 7
 - 00bc11c8  e83389dcff             call 0x989b00
 - 00bc11cd  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc11d3  8a81d8030000           mov al, byte ptr [ecx + 0x3d8]
 - 00bc11d9  a804                   test al, 4
 - 00bc11db  8db1d8030000           lea esi, [ecx + 0x3d8]
 - 00bc11e1  8bf9                   mov edi, ecx
 - 00bc11e3  7437                   je 0xbc121c
 - 00bc11e5  8b4104                 mov eax, dword ptr [ecx + 4]
 - 00bc11e8  8b9040010000           mov edx, dword ptr [eax + 0x140]
 - 00bc11ee  8b9844010000           mov ebx, dword ptr [eax + 0x144]
 - 00bc11f4  0540010000             add eax, 0x140
 - 00bc11f9  2bda                   sub ebx, edx
 - 00bc11fb  c1fb04                 sar ebx, 4
 - 00bc11fe  85db                   test ebx, ebx
 - 00bc1200  7702                   ja 0xbc1204
 - 00bc1202  8bd0                   mov edx, eax
 - 00bc1204  8b5804                 mov ebx, dword ptr [eax + 4]
 - 00bc1207  8b28                   mov ebp, dword ptr [eax]
 - 00bc1209  8b4104                 mov eax, dword ptr [ecx + 4]
 - 00bc120c  2bdd                   sub ebx, ebp
 - 00bc120e  c1fb04                 sar ebx, 4
 - 00bc1211  53                     push ebx
 - 00bc1212  52                     push edx
 - 00bc1213  8b5008                 mov edx, dword ptr [eax + 8]
 - 00bc1216  52                     push edx
 - 00bc1217  e814a7dcff             call 0x98b930
 - 00bc121c  f60601                 test byte ptr [esi], 1
 - 00bc121f  7407                   je 0xbc1228
 - 00bc1221  8bcf                   mov ecx, edi
 - 00bc1223  e8f877dcff             call 0x988a20
 - 00bc1228  f60602                 test byte ptr [esi], 2
 - 00bc122b  7407                   je 0xbc1234
 - 00bc122d  8bcf                   mov ecx, edi
 - 00bc122f  e8bc89dcff             call 0x989bf0
 - 00bc1234  f60604                 test byte ptr [esi], 4
 - 00bc1237  7407                   je 0xbc1240
 - 00bc1239  8bcf                   mov ecx, edi
 - 00bc123b  e82085dcff             call 0x989760
 - 00bc1240  f70600000200           test dword ptr [esi], 0x20000
 - 00bc1246  7407                   je 0xbc124f
 - 00bc1248  8bcf                   mov ecx, edi
 - 00bc124a  e87185dcff             call 0x9897c0
 - 00bc124f  8b06                   mov eax, dword ptr [esi]
 - 00bc1251  f6c404                 test ah, 4
 - 00bc1254  7407                   je 0xbc125d
 - 00bc1256  8bcf                   mov ecx, edi
 - 00bc1258  e80395dcff             call 0x98a760
 - 00bc125d  8b06                   mov eax, dword ptr [esi]
 - 00bc125f  f6c402                 test ah, 2
 - 00bc1262  7407                   je 0xbc126b
 - 00bc1264  8bcf                   mov ecx, edi
 - 00bc1266  e8d592dcff             call 0x98a540
 - 00bc126b  803e00                 cmp byte ptr [esi], 0
 - 00bc126e  7907                   jns 0xbc1277
 - 00bc1270  8bcf                   mov ecx, edi
 - 00bc1272  e8d977dcff             call 0x988a50
 - 00bc1277  f60608                 test byte ptr [esi], 8
 - 00bc127a  7407                   je 0xbc1283
 - 00bc127c  8bcf                   mov ecx, edi
 - 00bc127e  e87d84dcff             call 0x989700
 - 00bc1283  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc1289  6a02                   push 2
 - 00bc128b  6a00                   push 0
 - 00bc128d  6a06                   push 6
 - 00bc128f  6a00                   push 0
   00bc1291  e8ca99e4ff             call 0xa0ac60
   00bc1296  a19c6e4301             mov eax, dword ptr [0x1436e9c]
@@ retail 954-957 built 728-731
   00bc12bb  8a88e0000000           mov cl, byte ptr [eax + 0xe0]
   00bc12c1  84c9                   test cl, cl
 - 00bc12c3  c644244400             mov byte ptr [esp + 0x44], 0
 - 00bc12c8  c644244500             mov byte ptr [esp + 0x45], 0
 - 00bc12cd  c644244600             mov byte ptr [esp + 0x46], 0
 + 00bc0f75  c644243400             mov byte ptr [esp + 0x34], 0
 + 00bc0f7a  c644243500             mov byte ptr [esp + 0x35], 0
 + 00bc0f7f  c644243600             mov byte ptr [esp + 0x36], 0
   00bc12d2  7514                   jne 0xbc12e8
   00bc12d4  8a88e1000000           mov cl, byte ptr [eax + 0xe1]
@@ retail 964-965 built 738-739
   00bc12e4  84c9                   test cl, cl
   00bc12e6  742d                   je 0xbc1315
 - 00bc12e8  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00bc0f9a  8b4c2434               mov ecx, dword ptr [esp + 0x34]
   00bc12ec  8988e0000000           mov dword ptr [eax + 0xe0], ecx
   00bc12f2  80b8e2000000ff         cmp byte ptr [eax + 0xe2], 0xff
@@ retail 972-975 built 746-747
   00bc1307  c680e200000000         mov byte ptr [eax + 0xe2], 0
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
 - 00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 + 00bc0fc7  3998e8000000           cmp dword ptr [eax + 0xe8], ebx
   00bc131f  7415                   je 0xbc1336
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 978-979 built 750-751
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
   00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 + 00bc0fd8  8998e8000000           mov dword ptr [eax + 0xe8], ebx
   00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
@@ retail 982-983 built 754-756
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
 - 00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00bc0fef  33ff                   xor edi, edi
 + 00bc0ff1  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1345  3bf7                   cmp esi, edi
   00bc1347  7444                   je 0xbc138d
@@ retail 1005-1006 built 778-778
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
@@ retail 1008-1008 built 780-781
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
 + 00bc1032  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
@@ retail 1011-1011 built 784-785
   00bc138a  83c404                 add esp, 4
   00bc138d  5e                     pop esi
 + 00bc103e  5b                     pop ebx
   00bc138e  5d                     pop ebp
   00bc138f  5f                     pop edi
@@ retail 1013-1015 built 787-788
   00bc138e  5d                     pop ebp
   00bc138f  5f                     pop edi
 - 00bc1390  5b                     pop ebx
 - 00bc1391  83c430                 add esp, 0x30
 + 00bc1041  83c420                 add esp, 0x20
   00bc1394  c20800                 ret 8
DIFFLINES 423 retail_instr 1016 built_instr 789
