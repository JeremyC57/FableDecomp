00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(2476v3751) flags= []
@@ retail 0-1 built 0-0
 - 00bc04f0  83ec30                 sub esp, 0x30
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
   00bc04f8  53                     push ebx
@@ retail 2-2 built 1-3
   00bc04f0  83ec30                 sub esp, 0x30
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
 + 00bc04f5  83ec28                 sub esp, 0x28
 + 00bc04f8  85c0                   test eax, eax
   00bc04f8  53                     push ebx
   00bc04f9  57                     push edi
@@ retail 3-6 built 4-4
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
   00bc04f8  53                     push ebx
 - 00bc04f9  57                     push edi
 - 00bc04fa  33ff                   xor edi, edi
 - 00bc04fc  3bc7                   cmp eax, edi
   00bc04fe  8bd9                   mov ebx, ecx
   00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
@@ retail 7-8 built 5-5
   00bc04fc  3bc7                   cmp eax, edi
   00bc04fe  8bd9                   mov ebx, ecx
 - 00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
   00bc0504  0f84850e0000           je 0xbc138f
   00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
@@ retail 9-12 built 6-8
   00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
   00bc0504  0f84850e0000           je 0xbc138f
 - 00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc050e  8b0d986e4301           mov ecx, dword ptr [0x1436e98]
 - 00bc0514  8b8954090000           mov ecx, dword ptr [ecx + 0x954]
 + 00bc0503  8b442430               mov eax, dword ptr [esp + 0x30]
 + 00bc0507  8b1500000000           mov edx, dword ptr [0]
   00bc051a  55                     push ebp
   00bc051b  8b680c                 mov ebp, dword ptr [eax + 0xc]
@@ retail 14-17 built 10-13
   00bc051a  55                     push ebp
   00bc051b  8b680c                 mov ebp, dword ptr [eax + 0xc]
 - 00bc051e  8b5554                 mov edx, dword ptr [ebp + 0x54]
 - 00bc0521  52                     push edx
 - 00bc0522  896c2418               mov dword ptr [esp + 0x18], ebp
 + 00bc0511  8b4d54                 mov ecx, dword ptr [ebp + 0x54]
 + 00bc0514  51                     push ecx
 + 00bc0515  8b8a54090000           mov ecx, dword ptr [edx + 0x954]
   00bc0526  e865bce0ff             call 0x9cc190
   00bc052b  84c0                   test al, al
@@ retail 24-25 built 20-20
   00bc053b  56                     push esi
   00bc053c  8bb054090000           mov esi, dword ptr [eax + 0x954]
 - 00bc0542  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc0548  8b96e4010000           mov edx, dword ptr [esi + 0x1e4]
   00bc054e  2bd0                   sub edx, eax
@@ retail 26-27 built 21-24
   00bc0542  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc0548  8b96e4010000           mov edx, dword ptr [esi + 0x1e4]
 - 00bc054e  2bd0                   sub edx, eax
 + 00bc053d  57                     push edi
 + 00bc053e  8bbee0010000           mov edi, dword ptr [esi + 0x1e0]
 + 00bc0544  2bd7                   sub edx, edi
   00bc0550  b8e9a28b2e             mov eax, 0x2e8ba2e9
   00bc0555  f7ea                   imul edx
@@ retail 35-36 built 32-32
   00bc0561  3bc8                   cmp ecx, eax
   00bc0563  730d                   jae 0xbc0572
 - 00bc0565  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc056b  6bc92c                 imul ecx, ecx, 0x2c
   00bc056e  03c8                   add ecx, eax
@@ retail 37-37 built 33-34
   00bc0565  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc056b  6bc92c                 imul ecx, ecx, 0x2c
 + 00bc055e  8bc7                   mov eax, edi
   00bc056e  03c8                   add ecx, eax
   00bc0570  eb02                   jmp 0xbc0574
@@ retail 38-38 built 35-36
   00bc056b  6bc92c                 imul ecx, ecx, 0x2c
   00bc056e  03c8                   add ecx, eax
 + 00bc0562  8bf9                   mov edi, ecx
   00bc0570  eb02                   jmp 0xbc0574
   00bc0572  33c9                   xor ecx, ecx
@@ retail 39-47 built 37-46
   00bc056e  03c8                   add ecx, eax
   00bc0570  eb02                   jmp 0xbc0574
 - 00bc0572  33c9                   xor ecx, ecx
 - 00bc0574  57                     push edi
 - 00bc0575  57                     push edi
 - 00bc0576  57                     push edi
 - 00bc0577  8d54241c               lea edx, [esp + 0x1c]
 - 00bc057b  52                     push edx
 - 00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00bc0580  897c2420               mov dword ptr [esp + 0x20], edi
 + 00bc0566  33ff                   xor edi, edi
 + 00bc0568  6a00                   push 0
 + 00bc056a  6a00                   push 0
 + 00bc056c  6a00                   push 0
 + 00bc056e  8d4c241c               lea ecx, [esp + 0x1c]
 + 00bc0572  51                     push ecx
 + 00bc0573  8bcf                   mov ecx, edi
 + 00bc0575  897c2424               mov dword ptr [esp + 0x24], edi
 + 00bc0579  c744242000000000       mov dword ptr [esp + 0x20], 0
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
@@ retail 63-64 built 62-63
   00bc05bf  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc05c3  8b4808                 mov ecx, dword ptr [eax + 8]
 - 00bc05c6  3bcf                   cmp ecx, edi
 + 00bc05c3  85c9                   test ecx, ecx
   00bc05c8  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc05cd  8bf0                   mov esi, eax
@@ retail 76-77 built 75-76
   00bc05e6  8b86f03c0000           mov eax, dword ptr [esi + 0x3cf0]
   00bc05ec  8b10                   mov edx, dword ptr [eax]
 - 00bc05ee  57                     push edi
 + 00bc05eb  6a00                   push 0
   00bc05ef  50                     push eax
   00bc05f0  ff9204010000           call dword ptr [edx + 0x104]
@@ retail 80-83 built 79-82
   00bc05f0  ff9204010000           call dword ptr [edx + 0x104]
   00bc05f6  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc05fb  8b8eb83c0000           mov ecx, dword ptr [esi + 0x3cb8]
 - 00bc0601  bf01000000             mov edi, 1
 - 00bc0606  3bcf                   cmp ecx, edi
 + 00bc05f9  8b96b83c0000           mov edx, dword ptr [esi + 0x3cb8]
 + 00bc05ff  b901000000             mov ecx, 1
 + 00bc0604  3bd1                   cmp edx, ecx
   00bc0608  7d0b                   jge 0xbc0615
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
@@ retail 84-85 built 83-84
   00bc0606  3bcf                   cmp ecx, edi
   00bc0608  7d0b                   jge 0xbc0615
 - 00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
 + 00bc0608  898eb83c0000           mov dword ptr [esi + 0x3cb8], ecx
   00bc0610  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0615  8b4c2448               mov ecx, dword ptr [esp + 0x48]
@@ retail 86-87 built 85-86
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
   00bc0610  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0615  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00bc0613  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00bc0619  83f906                 cmp ecx, 6
   00bc061c  0f85a1010000           jne 0xbc07c3
@@ retail 96-97 built 95-96
   00bc0639  39b0402c0000           cmp dword ptr [eax + 0x2c40], esi
   00bc063f  8d883c2c0000           lea ecx, [eax + 0x2c3c]
 - 00bc0645  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc0643  c644243c00             mov byte ptr [esp + 0x3c], 0
   00bc064a  8bd0                   mov edx, eax
   00bc064c  8bfe                   mov edi, esi
@@ retail 121-122 built 120-121
   00bc069a  ff820c280000           inc dword ptr [edx + 0x280c]
   00bc06a0  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc06a5  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc06a3  8b3510000000           mov esi, dword ptr [0x10]
   00bc06ab  39b020290000           cmp dword ptr [eax + 0x2920], esi
   00bc06b1  8d901c290000           lea edx, [eax + 0x291c]
@@ retail 148-149 built 147-148
   00bc0707  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc070d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0712  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0710  8b3514000000           mov esi, dword ptr [0x14]
   00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
   00bc071e  8d9030290000           lea edx, [eax + 0x2930]
@@ retail 182-194 built 181-181
   00bc0793  51                     push ecx
   00bc0794  eb06                   jmp 0xbc079c
 - 00bc0796  6a00                   push 0
 - 00bc0798  8d5330                 lea edx, [ebx + 0x30]
 - 00bc079b  52                     push edx
 - 00bc079c  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc07a2  e87978dcff             call 0x988020
 - 00bc07a7  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc07ad  83c358                 add ebx, 0x58
 - 00bc07b0  53                     push ebx
 - 00bc07b1  e88a79dcff             call 0x988140
 - 00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc07ba  8b742418               mov esi, dword ptr [esp + 0x18]
 - 00bc07be  e9be060000             jmp 0xbc0e81
   00bc07c3  83f907                 cmp ecx, 7
   00bc07c6  0f85d9010000           jne 0xbc09a5
@@ retail 197-197 built 184-186
   00bc07c6  0f85d9010000           jne 0xbc09a5
   00bc07cc  8bb0d0280000           mov esi, dword ptr [eax + 0x28d0]
 + 00bc07a6  8d90cc280000           lea edx, [eax + 0x28cc]
 + 00bc07ac  bf01000000             mov edi, 1
   00bc07d2  3bf7                   cmp esi, edi
   00bc07d4  8d90cc280000           lea edx, [eax + 0x28cc]
@@ retail 198-200 built 187-188
   00bc07cc  8bb0d0280000           mov esi, dword ptr [eax + 0x28d0]
   00bc07d2  3bf7                   cmp esi, edi
 - 00bc07d4  8d90cc280000           lea edx, [eax + 0x28cc]
 - 00bc07da  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc07b3  c644243c00             mov byte ptr [esp + 0x3c], 0
   00bc07df  8bc8                   mov ecx, eax
   00bc07e1  7455                   je 0xbc0838
@@ retail 250-251 built 238-239
   00bc0899  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc089f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc08a4  8b35e86d3901           mov esi, dword ptr [0x1396de8]
 + 00bc087d  8b3510000000           mov esi, dword ptr [0x10]
   00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
   00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
@@ retail 286-289 built 274-276
   00bc0927  f6c110                 test cl, 0x10
   00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 - 00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
 - 00bc093c  3bee                   cmp ebp, esi
 + 00bc0902  8b3508000000           mov esi, dword ptr [8]
 + 00bc0908  39b094280000           cmp dword ptr [eax + 0x2894], esi
   00bc093e  8d8890280000           lea ecx, [eax + 0x2890]
   00bc0944  8bd0                   mov edx, eax
@@ retail 317-510 built 304-307
   00bc099f  51                     push ecx
   00bc09a0  e9f7fdffff             jmp 0xbc079c
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
 - 00bc0bc5  8bc8                   mov ecx, eax
 - 00bc0bc7  8bfe                   mov edi, esi
 - 00bc0bc9  745b                   je 0xbc0c26
 - 00bc0bcb  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0bd1  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0bd4  85c6                   test esi, eax
 - 00bc0bd6  7522                   jne 0xbc0bfa
 - 00bc0bd8  0bc6                   or eax, esi
 - 00bc0bda  894208                 mov dword ptr [edx + 8], eax
 - 00bc0bdd  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0be3  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc0be7  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0bed  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc0bf0  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc0bf4  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc0bfa  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc0bfd  84c0                   test al, al
 - 00bc0bff  897a04                 mov dword ptr [edx + 4], edi
 - 00bc0c02  7517                   jne 0xbc0c1b
 - 00bc0c04  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc0c08  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0c0e  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 - 00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 - 00bc0c2c  3bde                   cmp ebx, esi
 - 00bc0c2e  8d8830290000           lea ecx, [eax + 0x2930]
 - 00bc0c34  8bd0                   mov edx, eax
 - 00bc0c36  8bfe                   mov edi, esi
 - 00bc0c38  7450                   je 0xbc0c8a
 - 00bc0c3a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0c40  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0c43  85c6                   test esi, eax
 - 00bc0c45  7522                   jne 0xbc0c69
 - 00bc0c47  0bc6                   or eax, esi
 - 00bc0c49  894108                 mov dword ptr [ecx + 8], eax
 - 00bc0c4c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0c52  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 - 00bc0c56  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0c5c  8b7104                 mov esi, dword ptr [ecx + 4]
 - 00bc0c5f  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 - 00bc0c63  ff8208280000           inc dword ptr [edx + 0x2808]
 - 00bc0c69  8a4110                 mov al, byte ptr [ecx + 0x10]
 - 00bc0c6c  84c0                   test al, al
 - 00bc0c6e  897904                 mov dword ptr [ecx + 4], edi
 - 00bc0c71  7517                   jne 0xbc0c8a
 - 00bc0c73  c6411001               mov byte ptr [ecx + 0x10], 1
 - 00bc0c77  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 - 00bc0c7d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 - 00bc0c84  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc0c8a  c644244401             mov byte ptr [esp + 0x44], 1
 - 00bc0c8f  8a8584000000           mov al, byte ptr [ebp + 0x84]
 + 00bc0972  8d5330                 lea edx, [ebx + 0x30]
 + 00bc0975  6a00                   push 0
 + 00bc0977  52                     push edx
   00bc0c95  8b0d846e4301           mov ecx, dword ptr [0x1436e84]
   00bc0c9b  8b5970                 mov ebx, dword ptr [ecx + 0x70]
@@ retail 511-540 built 308-308
   00bc0c8f  8a8584000000           mov al, byte ptr [ebp + 0x84]
   00bc0c95  8b0d846e4301           mov ecx, dword ptr [0x1436e84]
 - 00bc0c9b  8b5970                 mov ebx, dword ptr [ecx + 0x70]
 - 00bc0c9e  33f6                   xor esi, esi
 - 00bc0ca0  a802                   test al, 2
 - 00bc0ca2  7469                   je 0xbc0d0d
 - 00bc0ca4  a820                   test al, 0x20
 - 00bc0ca6  740b                   je 0xbc0cb3
 - 00bc0ca8  8bcb                   mov ecx, ebx
 - 00bc0caa  e801b4ffff             call 0xbbc0b0
 - 00bc0caf  8bf0                   mov esi, eax
 - 00bc0cb1  eb7b                   jmp 0xbc0d2e
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
 - 00bc0cdb  51                     push ecx
 - 00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0ce2  8954242c               mov dword ptr [esp + 0x2c], edx
 - 00bc0ce6  89442424               mov dword ptr [esp + 0x24], eax
 - 00bc0cea  8b4570                 mov eax, dword ptr [ebp + 0x70]
 - 00bc0ced  6a05                   push 5
 - 00bc0cef  8d542428               lea edx, [esp + 0x28]
 - 00bc0cf3  52                     push edx
 - 00bc0cf4  89442438               mov dword ptr [esp + 0x38], eax
   00bc0cf8  e8935ef8ff             call 0xb46b90
   00bc0cfd  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 542-543 built 310-311
   00bc0cf8  e8935ef8ff             call 0xb46b90
   00bc0cfd  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0d03  8b30                   mov esi, dword ptr [eax]
 + 00bc0989  8d4358                 lea eax, [ebx + 0x58]
   00bc0d05  50                     push eax
   00bc0d06  e81542f8ff             call 0xb44f20
@@ retail 545-546 built 313-313
   00bc0d05  50                     push eax
   00bc0d06  e81542f8ff             call 0xb44f20
 - 00bc0d0b  eb21                   jmp 0xbc0d2e
   00bc0d0d  a19c6e4301             mov eax, dword ptr [0x1436e9c]
   00bc0d12  8b90e8000000           mov edx, dword ptr [eax + 0xe8]
@@ retail 547-550 built 314-319
   00bc0d0b  eb21                   jmp 0xbc0d2e
   00bc0d0d  a19c6e4301             mov eax, dword ptr [0x1436e9c]
 - 00bc0d12  8b90e8000000           mov edx, dword ptr [eax + 0xe8]
 - 00bc0d18  b964000000             mov ecx, 0x64
 - 00bc0d1d  3bd1                   cmp edx, ecx
 + 00bc0997  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc099b  8b80f03c0000           mov eax, dword ptr [eax + 0x3cf0]
 + 00bc09a1  8b10                   mov edx, dword ptr [eax]
 + 00bc09a3  8d4b60                 lea ecx, [ebx + 0x60]
 + 00bc09a6  85c9                   test ecx, ecx
   00bc0d1f  740d                   je 0xbc0d2e
   00bc0d21  8988e8000000           mov dword ptr [eax + 0xe8], ecx
@@ retail 551-671 built 320-323
   00bc0d1d  3bd1                   cmp edx, ecx
   00bc0d1f  740d                   je 0xbc0d2e
 - 00bc0d21  8988e8000000           mov dword ptr [eax + 0xe8], ecx
 - 00bc0d27  8388e400000002         or dword ptr [eax + 0xe4], 2
 - 00bc0d2e  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0d34  e8475ff8ff             call 0xb46c80
 - 00bc0d39  8a8584000000           mov al, byte ptr [ebp + 0x84]
 - 00bc0d3f  a804                   test al, 4
 - 00bc0d41  7429                   je 0xbc0d6c
 - 00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 - 00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
 - 00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
 - 00bc0d4d  2bd5                   sub edx, ebp
 - 00bc0d4f  c1fa03                 sar edx, 3
 - 00bc0d52  4a                     dec edx
 - 00bc0d53  3bf2                   cmp esi, edx
 - 00bc0d55  8bc6                   mov eax, esi
 - 00bc0d57  760b                   jbe 0xbc0d64
 - 00bc0d59  8b4128                 mov eax, dword ptr [ecx + 0x28]
 - 00bc0d5c  8bfd                   mov edi, ebp
 - 00bc0d5e  2bc7                   sub eax, edi
 - 00bc0d60  c1f803                 sar eax, 3
 - 00bc0d63  48                     dec eax
 - 00bc0d64  8b4924                 mov ecx, dword ptr [ecx + 0x24]
 - 00bc0d67  e9d1000000             jmp 0xbc0e3d
 - 00bc0d6c  a810                   test al, 0x10
 - 00bc0d6e  0f84a5000000           je 0xbc0e19
 - 00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 - 00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
 - 00bc0d86  3bfa                   cmp edi, edx
 - 00bc0d88  8d8190280000           lea eax, [ecx + 0x2890]
 - 00bc0d8e  8bea                   mov ebp, edx
 - 00bc0d90  7450                   je 0xbc0de2
 - 00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
 - 00bc0d9b  85d7                   test edi, edx
 - 00bc0d9d  7522                   jne 0xbc0dc1
 - 00bc0d9f  0bd7                   or edx, edi
 - 00bc0da1  895008                 mov dword ptr [eax + 8], edx
 - 00bc0da4  8b9108280000           mov edx, dword ptr [ecx + 0x2808]
 - 00bc0daa  8944d108               mov dword ptr [ecx + edx*8 + 8], eax
 - 00bc0dae  8b9108280000           mov edx, dword ptr [ecx + 0x2808]
 - 00bc0db4  8b7804                 mov edi, dword ptr [eax + 4]
 - 00bc0db7  897cd10c               mov dword ptr [ecx + edx*8 + 0xc], edi
 - 00bc0dbb  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc0dc1  8a5010                 mov dl, byte ptr [eax + 0x10]
 - 00bc0dc4  84d2                   test dl, dl
 - 00bc0dc6  896804                 mov dword ptr [eax + 4], ebp
 - 00bc0dc9  7517                   jne 0xbc0de2
 - 00bc0dcb  c6401001               mov byte ptr [eax + 0x10], 1
 - 00bc0dcf  8b910c280000           mov edx, dword ptr [ecx + 0x280c]
 - 00bc0dd5  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
 - 00bc0ddc  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0de2  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0de6  8b4f1c                 mov ecx, dword ptr [edi + 0x1c]
 - 00bc0de9  8b6f18                 mov ebp, dword ptr [edi + 0x18]
 - 00bc0dec  2bcd                   sub ecx, ebp
 - 00bc0dee  c1f903                 sar ecx, 3
 - 00bc0df1  49                     dec ecx
 - 00bc0df2  3bf1                   cmp esi, ecx
 - 00bc0df4  8bc6                   mov eax, esi
 - 00bc0df6  760b                   jbe 0xbc0e03
 - 00bc0df8  8b471c                 mov eax, dword ptr [edi + 0x1c]
 - 00bc0dfb  8bf5                   mov esi, ebp
 - 00bc0dfd  2bc6                   sub eax, esi
 - 00bc0dff  c1f803                 sar eax, 3
 - 00bc0e02  48                     dec eax
 - 00bc0e03  8b5718                 mov edx, dword ptr [edi + 0x18]
 - 00bc0e06  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc0e0c  6a00                   push 0
 - 00bc0e0e  8d04c2                 lea eax, [edx + eax*8]
 - 00bc0e11  50                     push eax
 - 00bc0e12  e80972dcff             call 0x988020
 - 00bc0e17  eb39                   jmp 0xbc0e52
 - 00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 - 00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
 - 00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
 - 00bc0e23  2bd5                   sub edx, ebp
 - 00bc0e25  c1fa03                 sar edx, 3
 - 00bc0e28  4a                     dec edx
 - 00bc0e29  3bf2                   cmp esi, edx
 - 00bc0e2b  8bc6                   mov eax, esi
 - 00bc0e2d  760b                   jbe 0xbc0e3a
 - 00bc0e2f  8b4110                 mov eax, dword ptr [ecx + 0x10]
 - 00bc0e32  8bfd                   mov edi, ebp
 - 00bc0e34  2bc7                   sub eax, edi
 - 00bc0e36  c1f803                 sar eax, 3
 - 00bc0e39  48                     dec eax
 - 00bc0e3a  8b490c                 mov ecx, dword ptr [ecx + 0xc]
 - 00bc0e3d  8d14c1                 lea edx, [ecx + eax*8]
 - 00bc0e40  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc0e46  6a00                   push 0
 - 00bc0e48  52                     push edx
 - 00bc0e49  e8d271dcff             call 0x988020
 - 00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0e52  8b742418               mov esi, dword ptr [esp + 0x18]
 - 00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
 - 00bc0e5d  7413                   je 0xbc0e72
 - 00bc0e5f  8d4748                 lea eax, [edi + 0x48]
 - 00bc0e62  50                     push eax
 - 00bc0e63  8d4f50                 lea ecx, [edi + 0x50]
 - 00bc0e66  51                     push ecx
 - 00bc0e67  6a00                   push 0
 - 00bc0e69  8bcb                   mov ecx, ebx
 - 00bc0e6b  e800beffff             call 0xbbcc70
 - 00bc0e70  eb0f                   jmp 0xbc0e81
 - 00bc0e72  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc0e78  8d5748                 lea edx, [edi + 0x48]
 - 00bc0e7b  52                     push edx
 - 00bc0e7c  e8bf72dcff             call 0x988140
 - 00bc0e81  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0e87  8d4760                 lea eax, [edi + 0x60]
 - 00bc0e8a  85c0                   test eax, eax
 - 00bc0e8c  7418                   je 0xbc0ea6
 - 00bc0e8e  0fb65810               movzx ebx, byte ptr [eax + 0x10]
 - 00bc0e92  8b4004                 mov eax, dword ptr [eax + 4]
 - 00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
 - 00bc0e9b  8b11                   mov edx, dword ptr [ecx]
 - 00bc0e9d  53                     push ebx
 - 00bc0e9e  6a00                   push 0
 - 00bc0ea0  50                     push eax
 + 00bc09aa  0fb67110               movzx esi, byte ptr [ecx + 0x10]
 + 00bc09ae  8b4904                 mov ecx, dword ptr [ecx + 4]
 + 00bc09b1  56                     push esi
   00bc0ea1  6a00                   push 0
   00bc0ea3  51                     push ecx
@@ retail 674-676 built 326-326
   00bc0ea3  51                     push ecx
   00bc0ea4  eb11                   jmp 0xbc0eb7
 - 00bc0ea6  8b81f03c0000           mov eax, dword ptr [ecx + 0x3cf0]
 - 00bc0eac  8b10                   mov edx, dword ptr [eax]
   00bc0eae  6a00                   push 0
   00bc0eb0  6a00                   push 0
@@ retail 683-684 built 333-334
   00bc0eb7  ff9290010000           call dword ptr [edx + 0x190]
   00bc0ebd  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0ec3  8d8794000000           lea eax, [edi + 0x94]
 + 00bc09cc  8d8394000000           lea eax, [ebx + 0x94]
   00bc0ec9  85c0                   test eax, eax
   00bc0ecb  8981f43c0000           mov dword ptr [ecx + 0x3cf4], eax
@@ retail 697-702 built 347-347
   00bc0eea  a1a06e4301             mov eax, dword ptr [0x1436ea0]
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
 - 00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 - 00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 - 00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
 - 00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
   00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
@@ retail 703-707 built 348-356
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
   00bc0f0e  0544010000             add eax, 0x144
 - 00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
 - 00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
 - 00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0a03  8993a8000000           mov dword ptr [ebx + 0xa8], edx
 + 00bc0a09  8b500c                 mov edx, dword ptr [eax + 0xc]
 + 00bc0a0c  8993ac000000           mov dword ptr [ebx + 0xac], edx
 + 00bc0a12  8b5568                 mov edx, dword ptr [ebp + 0x68]
 + 00bc0a15  8993b0000000           mov dword ptr [ebx + 0xb0], edx
 + 00bc0a1b  8b556c                 mov edx, dword ptr [ebp + 0x6c]
 + 00bc0a1e  8d8ba8000000           lea ecx, [ebx + 0xa8]
 + 00bc0a24  8993b4000000           mov dword ptr [ebx + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 708-709 built 357-358
   00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
 - 00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
 + 00bc0a2d  8993b8000000           mov dword ptr [ebx + 0xb8], edx
   00bc0f2e  8b5010                 mov edx, dword ptr [eax + 0x10]
   00bc0f31  8997bc000000           mov dword ptr [edi + 0xbc], edx
@@ retail 710-725 built 359-374
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
   00bc0f2e  8b5010                 mov edx, dword ptr [eax + 0x10]
 - 00bc0f31  8997bc000000           mov dword ptr [edi + 0xbc], edx
 - 00bc0f37  0fb75306               movzx edx, word ptr [ebx + 6]
 - 00bc0f3b  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f3f  0fb713                 movzx edx, word ptr [ebx]
 - 00bc0f42  db442448               fild dword ptr [esp + 0x48]
 - 00bc0f46  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f4a  da742448               fidiv dword ptr [esp + 0x48]
 - 00bc0f4e  d99fc0000000           fstp dword ptr [edi + 0xc0]
 - 00bc0f54  0fb75308               movzx edx, word ptr [ebx + 8]
 - 00bc0f58  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f5c  0fb75302               movzx edx, word ptr [ebx + 2]
 - 00bc0f60  db442448               fild dword ptr [esp + 0x48]
 - 00bc0f64  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f68  da742448               fidiv dword ptr [esp + 0x48]
 - 00bc0f6c  d99fc4000000           fstp dword ptr [edi + 0xc4]
 + 00bc0a36  8993bc000000           mov dword ptr [ebx + 0xbc], edx
 + 00bc0a3c  0fb75706               movzx edx, word ptr [edi + 6]
 + 00bc0a40  89542440               mov dword ptr [esp + 0x40], edx
 + 00bc0a44  0fb717                 movzx edx, word ptr [edi]
 + 00bc0a47  db442440               fild dword ptr [esp + 0x40]
 + 00bc0a4b  89542440               mov dword ptr [esp + 0x40], edx
 + 00bc0a4f  da742440               fidiv dword ptr [esp + 0x40]
 + 00bc0a53  d99bc0000000           fstp dword ptr [ebx + 0xc0]
 + 00bc0a59  0fb75708               movzx edx, word ptr [edi + 8]
 + 00bc0a5d  89542440               mov dword ptr [esp + 0x40], edx
 + 00bc0a61  0fb75702               movzx edx, word ptr [edi + 2]
 + 00bc0a65  db442440               fild dword ptr [esp + 0x40]
 + 00bc0a69  89542440               mov dword ptr [esp + 0x40], edx
 + 00bc0a6d  da742440               fidiv dword ptr [esp + 0x40]
 + 00bc0a71  d99bc4000000           fstp dword ptr [ebx + 0xc4]
   00bc0f72  8b5008                 mov edx, dword ptr [eax + 8]
   00bc0f75  8997c8000000           mov dword ptr [edi + 0xc8], edx
@@ retail 726-727 built 375-376
   00bc0f6c  d99fc4000000           fstp dword ptr [edi + 0xc4]
   00bc0f72  8b5008                 mov edx, dword ptr [eax + 8]
 - 00bc0f75  8997c8000000           mov dword ptr [edi + 0xc8], edx
 + 00bc0a7a  8993c8000000           mov dword ptr [ebx + 0xc8], edx
   00bc0f7b  8b4014                 mov eax, dword ptr [eax + 0x14]
   00bc0f7e  8987cc000000           mov dword ptr [edi + 0xcc], eax
@@ retail 728-730 built 377-379
   00bc0f75  8997c8000000           mov dword ptr [edi + 0xc8], edx
   00bc0f7b  8b4014                 mov eax, dword ptr [eax + 0x14]
 - 00bc0f7e  8987cc000000           mov dword ptr [edi + 0xcc], eax
 - 00bc0f84  f6868400000008         test byte ptr [esi + 0x84], 8
 + 00bc0a83  8983cc000000           mov dword ptr [ebx + 0xcc], eax
 + 00bc0a89  f6858400000008         test byte ptr [ebp + 0x84], 8
   00bc0f8b  7408                   je 0xbc0f95
   00bc0f8d  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 735-736 built 384-385
   00bc0f95  d905dcde2201           fld dword ptr [0x122dedc]
   00bc0f9b  51                     push ecx
 - 00bc0f9c  d99fd0000000           fstp dword ptr [edi + 0xd0]
 + 00bc0aa1  d99bd0000000           fstp dword ptr [ebx + 0xd0]
   00bc0fa2  6a03                   push 3
   00bc0fa4  c787d400000000000000   mov dword ptr [edi + 0xd4], 0
@@ retail 737-738 built 386-387
   00bc0f9c  d99fd0000000           fstp dword ptr [edi + 0xd0]
   00bc0fa2  6a03                   push 3
 - 00bc0fa4  c787d400000000000000   mov dword ptr [edi + 0xd4], 0
 + 00bc0aa9  c783d400000000000000   mov dword ptr [ebx + 0xd4], 0
   00bc0fae  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0fb4  6a01                   push 1
@@ retail 741-742 built 390-391
   00bc0fb4  6a01                   push 1
   00bc0fb6  e81587dcff             call 0x9896d0
 - 00bc0fbb  8a8683000000           mov al, byte ptr [esi + 0x83]
 + 00bc0ac0  8a8583000000           mov al, byte ptr [ebp + 0x83]
   00bc0fc1  84c0                   test al, al
   00bc0fc3  0f86cd020000           jbe 0xbc1296
@@ retail 744-745 built 393-394
   00bc0fc1  84c0                   test al, al
   00bc0fc3  0f86cd020000           jbe 0xbc1296
 - 00bc0fc9  d94660                 fld dword ptr [esi + 0x60]
 + 00bc0ace  d94560                 fld dword ptr [ebp + 0x60]
   00bc0fcc  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00bc0fd2  dfe0                   fnstsw ax
@@ retail 749-750 built 398-399
   00bc0fd4  f6c441                 test ah, 0x41
   00bc0fd7  0f85b9020000           jne 0xbc1296
 - 00bc0fdd  d94664                 fld dword ptr [esi + 0x64]
 + 00bc0ae2  d94564                 fld dword ptr [ebp + 0x64]
   00bc0fe0  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00bc0fe6  dfe0                   fnstsw ax
@@ retail 754-755 built 403-404
   00bc0fe8  f6c441                 test ah, 0x41
   00bc0feb  0f85a5020000           jne 0xbc1296
 - 00bc0ff1  8b4e5c                 mov ecx, dword ptr [esi + 0x5c]
 + 00bc0af6  8b4d5c                 mov ecx, dword ptr [ebp + 0x5c]
   00bc0ff4  6a00                   push 0
   00bc0ff6  6a00                   push 0
@@ retail 760-761 built 409-410
   00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
   00bc0ffd  52                     push edx
 - 00bc0ffe  8bcb                   mov ecx, ebx
 + 00bc0b03  8bcf                   mov ecx, edi
   00bc1000  e8cbe8e3ff             call 0x9ff8d0
   00bc1005  84c0                   test al, al
@@ retail 764-771 built 413-420
   00bc1005  84c0                   test al, al
   00bc1007  0f8489020000           je 0xbc1296
 - 00bc100d  0fb68682000000         movzx eax, byte ptr [esi + 0x82]
 - 00bc1014  0fb68e81000000         movzx ecx, byte ptr [esi + 0x81]
 - 00bc101b  0fb69680000000         movzx edx, byte ptr [esi + 0x80]
 - 00bc1022  89442448               mov dword ptr [esp + 0x48], eax
 - 00bc1026  0fb68683000000         movzx eax, byte ptr [esi + 0x83]
 - 00bc102d  db442448               fild dword ptr [esp + 0x48]
 - 00bc1031  894c2448               mov dword ptr [esp + 0x48], ecx
 + 00bc0b12  0fb68582000000         movzx eax, byte ptr [ebp + 0x82]
 + 00bc0b19  0fb68d81000000         movzx ecx, byte ptr [ebp + 0x81]
 + 00bc0b20  0fb69580000000         movzx edx, byte ptr [ebp + 0x80]
 + 00bc0b27  89442440               mov dword ptr [esp + 0x40], eax
 + 00bc0b2b  0fb68583000000         movzx eax, byte ptr [ebp + 0x83]
 + 00bc0b32  db442440               fild dword ptr [esp + 0x40]
 + 00bc0b36  894c2440               mov dword ptr [esp + 0x40], ecx
   00bc1035  d80d24172301           fmul dword ptr [0x1231724]
   00bc103b  db442448               fild dword ptr [esp + 0x48]
@@ retail 772-774 built 421-431
   00bc1031  894c2448               mov dword ptr [esp + 0x48], ecx
   00bc1035  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc103b  db442448               fild dword ptr [esp + 0x48]
 - 00bc103f  89542448               mov dword ptr [esp + 0x48], edx
 + 00bc0b40  db442440               fild dword ptr [esp + 0x40]
 + 00bc0b44  89542440               mov dword ptr [esp + 0x40], edx
 + 00bc0b48  d80d00000000           fmul dword ptr [0]
 + 00bc0b4e  d95c242c               fstp dword ptr [esp + 0x2c]
 + 00bc0b52  db442440               fild dword ptr [esp + 0x40]
 + 00bc0b56  89442440               mov dword ptr [esp + 0x40], eax
 + 00bc0b5a  8a44243c               mov al, byte ptr [esp + 0x3c]
 + 00bc0b5e  84c0                   test al, al
 + 00bc0b60  d80d00000000           fmul dword ptr [0]
 + 00bc0b66  db442440               fild dword ptr [esp + 0x40]
   00bc1043  d80d24172301           fmul dword ptr [0x1231724]
   00bc1049  d95c2434               fstp dword ptr [esp + 0x34]
@@ retail 776-784 built 433-433
   00bc1043  d80d24172301           fmul dword ptr [0x1231724]
   00bc1049  d95c2434               fstp dword ptr [esp + 0x34]
 - 00bc104d  db442448               fild dword ptr [esp + 0x48]
 - 00bc1051  89442448               mov dword ptr [esp + 0x48], eax
 - 00bc1055  8a442444               mov al, byte ptr [esp + 0x44]
 - 00bc1059  84c0                   test al, al
 - 00bc105b  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc1061  db442448               fild dword ptr [esp + 0x48]
 - 00bc1065  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc106b  d95c243c               fstp dword ptr [esp + 0x3c]
   00bc106f  7406                   je 0xbc1077
   00bc1071  d944243c               fld dword ptr [esp + 0x3c]
@@ retail 785-786 built 434-435
   00bc106b  d95c243c               fstp dword ptr [esp + 0x3c]
   00bc106f  7406                   je 0xbc1077
 - 00bc1071  d944243c               fld dword ptr [esp + 0x3c]
 + 00bc0b76  d9442434               fld dword ptr [esp + 0x34]
   00bc1075  eb06                   jmp 0xbc107d
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 788-789 built 437-437
   00bc1075  eb06                   jmp 0xbc107d
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1081  d9ca                   fxch st(2)
   00bc1083  d8ca                   fmul st(2)
@@ retail 790-790 built 438-439
   00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1081  d9ca                   fxch st(2)
 + 00bc0b84  6a00                   push 0
   00bc1083  d8ca                   fmul st(2)
   00bc1085  6a00                   push 0
@@ retail 791-793 built 440-441
   00bc1081  d9ca                   fxch st(2)
   00bc1083  d8ca                   fmul st(2)
 - 00bc1085  6a00                   push 0
 - 00bc1087  8d4e48                 lea ecx, [esi + 0x48]
 + 00bc0b88  8d4d48                 lea ecx, [ebp + 0x48]
   00bc108a  51                     push ecx
   00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 795-797 built 443-444
   00bc108a  51                     push ecx
   00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc1091  d95c2428               fstp dword ptr [esp + 0x28]
 - 00bc1095  d944243c               fld dword ptr [esp + 0x3c]
 + 00bc0b92  d95c2424               fstp dword ptr [esp + 0x24]
   00bc1099  6a04                   push 4
   00bc109b  d8ca                   fmul st(2)
@@ retail 798-798 built 445-446
   00bc1095  d944243c               fld dword ptr [esp + 0x3c]
   00bc1099  6a04                   push 4
 + 00bc0b98  d9442438               fld dword ptr [esp + 0x38]
   00bc109b  d8ca                   fmul st(2)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
@@ retail 799-799 built 447-449
   00bc1099  6a04                   push 4
   00bc109b  d8ca                   fmul st(2)
 + 00bc0b9e  d95c242c               fstp dword ptr [esp + 0x2c]
 + 00bc0ba2  d8c9                   fmul st(1)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
   00bc10a1  d8c9                   fmul st(1)
@@ retail 800-802 built 450-450
   00bc109b  d8ca                   fmul st(2)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
 - 00bc10a1  d8c9                   fmul st(1)
 - 00bc10a3  d95c2434               fstp dword ptr [esp + 0x34]
   00bc10a7  ddd8                   fstp st(0)
   00bc10a9  e8b289dcff             call 0x989a60
@@ retail 804-805 built 452-453
   00bc10a7  ddd8                   fstp st(0)
   00bc10a9  e8b289dcff             call 0x989a60
 - 00bc10ae  d94674                 fld dword ptr [esi + 0x74]
 + 00bc0baf  d94574                 fld dword ptr [ebp + 0x74]
   00bc10b1  d80d10002301           fmul dword ptr [0x1230010]
   00bc10b7  d9542444               fst dword ptr [esp + 0x44]
@@ retail 806-807 built 454-455
   00bc10ae  d94674                 fld dword ptr [esi + 0x74]
   00bc10b1  d80d10002301           fmul dword ptr [0x1230010]
 - 00bc10b7  d9542444               fst dword ptr [esp + 0x44]
 + 00bc0bb8  d954243c               fst dword ptr [esp + 0x3c]
   00bc10bb  d8259cf52201           fsub dword ptr [0x122f59c]
   00bc10c1  d95c241c               fstp dword ptr [esp + 0x1c]
@@ retail 808-813 built 456-461
   00bc10b7  d9542444               fst dword ptr [esp + 0x44]
   00bc10bb  d8259cf52201           fsub dword ptr [0x122f59c]
 - 00bc10c1  d95c241c               fstp dword ptr [esp + 0x1c]
 - 00bc10c5  d944241c               fld dword ptr [esp + 0x1c]
 - 00bc10c9  db5c2448               fistp dword ptr [esp + 0x48]
 - 00bc10cd  db442448               fild dword ptr [esp + 0x48]
 - 00bc10d1  8b442448               mov eax, dword ptr [esp + 0x48]
 + 00bc0bc2  d95c2414               fstp dword ptr [esp + 0x14]
 + 00bc0bc6  d9442414               fld dword ptr [esp + 0x14]
 + 00bc0bca  db5c2440               fistp dword ptr [esp + 0x40]
 + 00bc0bce  db442440               fild dword ptr [esp + 0x40]
 + 00bc0bd2  8b442440               mov eax, dword ptr [esp + 0x40]
   00bc10d5  0500ffffff             add eax, 0xffffff00
   00bc10da  25ff030000             and eax, 0x3ff
@@ retail 815-816 built 463-464
   00bc10d5  0500ffffff             add eax, 0xffffff00
   00bc10da  25ff030000             and eax, 0x3ff
 - 00bc10df  d86c2444               fsubr dword ptr [esp + 0x44]
 + 00bc0be0  d86c243c               fsubr dword ptr [esp + 0x3c]
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
@@ retail 818-819 built 466-467
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc0bec  d80c8500000000         fmul dword ptr [eax*4]
   00bc10f2  d9c9                   fxch st(1)
   00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 820-821 built 468-469
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc0bf5  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 823-824 built 471-472
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
 - 00bc1101  d94674                 fld dword ptr [esi + 0x74]
 + 00bc0c02  d94574                 fld dword ptr [ebp + 0x74]
   00bc1104  d80d10002301           fmul dword ptr [0x1230010]
   00bc110a  d9542444               fst dword ptr [esp + 0x44]
@@ retail 825-826 built 473-474
   00bc1101  d94674                 fld dword ptr [esi + 0x74]
   00bc1104  d80d10002301           fmul dword ptr [0x1230010]
 - 00bc110a  d9542444               fst dword ptr [esp + 0x44]
 + 00bc0c0b  d954243c               fst dword ptr [esp + 0x3c]
   00bc110e  d8259cf52201           fsub dword ptr [0x122f59c]
   00bc1114  d95c241c               fstp dword ptr [esp + 0x1c]
@@ retail 827-835 built 475-483
   00bc110a  d9542444               fst dword ptr [esp + 0x44]
   00bc110e  d8259cf52201           fsub dword ptr [0x122f59c]
 - 00bc1114  d95c241c               fstp dword ptr [esp + 0x1c]
 - 00bc1118  d944241c               fld dword ptr [esp + 0x1c]
 - 00bc111c  db5c2448               fistp dword ptr [esp + 0x48]
 - 00bc1120  db442448               fild dword ptr [esp + 0x48]
 - 00bc1124  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00bc1128  8b5664                 mov edx, dword ptr [esi + 0x64]
 - 00bc112b  8b4e60                 mov ecx, dword ptr [esi + 0x60]
 - 00bc112e  d86c2444               fsubr dword ptr [esp + 0x44]
 + 00bc0c15  d95c2414               fstp dword ptr [esp + 0x14]
 + 00bc0c19  d9442414               fld dword ptr [esp + 0x14]
 + 00bc0c1d  db5c2440               fistp dword ptr [esp + 0x40]
 + 00bc0c21  db442440               fild dword ptr [esp + 0x40]
 + 00bc0c25  8b442440               mov eax, dword ptr [esp + 0x40]
 + 00bc0c29  8b5564                 mov edx, dword ptr [ebp + 0x64]
 + 00bc0c2c  8b4d60                 mov ecx, dword ptr [ebp + 0x60]
 + 00bc0c2f  d86c243c               fsubr dword ptr [esp + 0x3c]
   00bc1132  25ff030000             and eax, 0x3ff
   00bc1137  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 844-845 built 492-493
   00bc1146  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc0c4e  d80c8500000000         fmul dword ptr [eax*4]
   00bc1154  d9c9                   fxch st(1)
   00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 846-847 built 494-495
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc0c57  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 851-854 built 499-502
   00bc1162  6a05                   push 5
   00bc1164  e89789dcff             call 0x989b00
 - 00bc1169  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc116d  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 - 00bc1171  8b542424               mov edx, dword ptr [esp + 0x24]
 + 00bc0c6a  8b442434               mov eax, dword ptr [esp + 0x34]
 + 00bc0c6e  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 + 00bc0c72  8b542420               mov edx, dword ptr [esp + 0x20]
   00bc1175  50                     push eax
   00bc1176  8b442424               mov eax, dword ptr [esp + 0x24]
@@ retail 855-856 built 503-504
   00bc1171  8b542424               mov edx, dword ptr [esp + 0x24]
   00bc1175  50                     push eax
 - 00bc1176  8b442424               mov eax, dword ptr [esp + 0x24]
 + 00bc0c77  8b442420               mov eax, dword ptr [esp + 0x20]
   00bc117a  51                     push ecx
   00bc117b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 864-865 built 512-512
   00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
 - 00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
   00bc1196  6a00                   push 0
   00bc1198  db442448               fild dword ptr [esp + 0x48]
@@ retail 866-867 built 513-514
   00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
   00bc1196  6a00                   push 0
 - 00bc1198  db442448               fild dword ptr [esp + 0x48]
 + 00bc0c95  894c2440               mov dword ptr [esp + 0x40], ecx
   00bc119c  6a00                   push 0
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
@@ retail 868-868 built 515-517
   00bc1198  db442448               fild dword ptr [esp + 0x48]
   00bc119c  6a00                   push 0
 + 00bc0c9b  db442444               fild dword ptr [esp + 0x44]
 + 00bc0c9f  83ec08                 sub esp, 8
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
@@ retail 870-870 built 519-520
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
 + 00bc0cac  d95c2404               fstp dword ptr [esp + 4]
   00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc11ae  83ec08                 sub esp, 8
@@ retail 871-875 built 521-523
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
   00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc11ae  83ec08                 sub esp, 8
 - 00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
 - 00bc11b5  d95c2404               fstp dword ptr [esp + 4]
 - 00bc11b9  db442454               fild dword ptr [esp + 0x54]
 + 00bc0cb6  8954244c               mov dword ptr [esp + 0x4c], edx
 + 00bc0cba  db44244c               fild dword ptr [esp + 0x4c]
   00bc11bd  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11c3  d91c24                 fstp dword ptr [esp]
@@ retail 954-957 built 602-605
   00bc12bb  8a88e0000000           mov cl, byte ptr [eax + 0xe0]
   00bc12c1  84c9                   test cl, cl
 - 00bc12c3  c644244400             mov byte ptr [esp + 0x44], 0
 - 00bc12c8  c644244500             mov byte ptr [esp + 0x45], 0
 - 00bc12cd  c644244600             mov byte ptr [esp + 0x46], 0
 + 00bc0dc4  c644243c00             mov byte ptr [esp + 0x3c], 0
 + 00bc0dc9  c644243d00             mov byte ptr [esp + 0x3d], 0
 + 00bc0dce  c644243e00             mov byte ptr [esp + 0x3e], 0
   00bc12d2  7514                   jne 0xbc12e8
   00bc12d4  8a88e1000000           mov cl, byte ptr [eax + 0xe1]
@@ retail 964-965 built 612-613
   00bc12e4  84c9                   test cl, cl
   00bc12e6  742d                   je 0xbc1315
 - 00bc12e8  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00bc0de9  8b4c243c               mov ecx, dword ptr [esp + 0x3c]
   00bc12ec  8988e0000000           mov dword ptr [eax + 0xe0], ecx
   00bc12f2  80b8e2000000ff         cmp byte ptr [eax + 0xe2], 0xff
@@ retail 973-975 built 621-622
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
   00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 + 00bc0e1c  85c9                   test ecx, ecx
   00bc131f  7415                   je 0xbc1336
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 978-979 built 625-626
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
   00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 + 00bc0e29  c780e800000000000000   mov dword ptr [eax + 0xe8], 0
   00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
@@ retail 983-983 built 630-631
   00bc133c  e8ff44e4ff             call 0xa05840
   00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00bc0e48  33ff                   xor edi, edi
   00bc1345  3bf7                   cmp esi, edi
   00bc1347  7444                   je 0xbc138d
@@ retail 1005-1006 built 653-653
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
@@ retail 1008-1008 built 655-656
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
 + 00bc0e87  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
@@ retail 1010-1010 built 658-659
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
 + 00bc0e92  5f                     pop edi
   00bc138d  5e                     pop esi
   00bc138e  5d                     pop ebp
@@ retail 1012-1013 built 661-661
   00bc138d  5e                     pop esi
   00bc138e  5d                     pop ebp
 - 00bc138f  5f                     pop edi
   00bc1390  5b                     pop ebx
   00bc1391  83c430                 add esp, 0x30
@@ retail 1014-1015 built 662-663
   00bc138f  5f                     pop edi
   00bc1390  5b                     pop ebx
 - 00bc1391  83c430                 add esp, 0x30
 + 00bc0e96  83c428                 add esp, 0x28
   00bc1394  c20800                 ret 8
DIFFLINES 535 retail_instr 1016 built_instr 664
