00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(3655v3751) flags= []
@@ retail 2-2 built 2-3
   00bc04f0  83ec30                 sub esp, 0x30
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
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
@@ retail 7-8 built 5-6
   00bc04fc  3bc7                   cmp eax, edi
   00bc04fe  8bd9                   mov ebx, ecx
 - 00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
 + 00bc04fd  895c2408               mov dword ptr [esp + 8], ebx
   00bc0504  0f84850e0000           je 0xbc138f
   00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
@@ retail 9-12 built 7-9
   00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
   00bc0504  0f84850e0000           je 0xbc138f
 - 00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc050e  8b0d986e4301           mov ecx, dword ptr [0x1436e98]
 - 00bc0514  8b8954090000           mov ecx, dword ptr [ecx + 0x954]
 + 00bc0507  8b442438               mov eax, dword ptr [esp + 0x38]
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
@@ retail 24-25 built 22-22
   00bc053b  56                     push esi
   00bc053c  8bb054090000           mov esi, dword ptr [eax + 0x954]
 - 00bc0542  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc0548  8b96e4010000           mov edx, dword ptr [esi + 0x1e4]
   00bc054e  2bd0                   sub edx, eax
@@ retail 26-27 built 23-26
   00bc0542  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc0548  8b96e4010000           mov edx, dword ptr [esi + 0x1e4]
 - 00bc054e  2bd0                   sub edx, eax
 + 00bc0545  57                     push edi
 + 00bc0546  8bbee0010000           mov edi, dword ptr [esi + 0x1e0]
 + 00bc054c  2bd7                   sub edx, edi
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
 + 00bc0566  8bc7                   mov eax, edi
   00bc056e  03c8                   add ecx, eax
   00bc0570  eb02                   jmp 0xbc0574
@@ retail 40-43 built 39-42
   00bc0570  eb02                   jmp 0xbc0574
   00bc0572  33c9                   xor ecx, ecx
 - 00bc0574  57                     push edi
 - 00bc0575  57                     push edi
 - 00bc0576  57                     push edi
 + 00bc056e  6a00                   push 0
 + 00bc0570  6a00                   push 0
 + 00bc0572  6a00                   push 0
   00bc0577  8d54241c               lea edx, [esp + 0x1c]
   00bc057b  52                     push edx
@@ retail 46-47 built 45-46
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00bc0580  897c2420               mov dword ptr [esp + 0x20], edi
 + 00bc057d  c744242000000000       mov dword ptr [esp + 0x20], 0
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
@@ retail 49-49 built 48-49
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
 + 00bc058c  bf10000000             mov edi, 0x10
   00bc058b  0f84b00d0000           je 0xbc1341
   00bc0591  a108b53c01             mov eax, dword ptr [0x13cb508]
@@ retail 63-64 built 63-64
   00bc05bf  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc05c3  8b4808                 mov ecx, dword ptr [eax + 8]
 - 00bc05c6  3bcf                   cmp ecx, edi
 + 00bc05cc  85c9                   test ecx, ecx
   00bc05c8  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc05cd  8bf0                   mov esi, eax
@@ retail 76-77 built 76-77
   00bc05e6  8b86f03c0000           mov eax, dword ptr [esi + 0x3cf0]
   00bc05ec  8b10                   mov edx, dword ptr [eax]
 - 00bc05ee  57                     push edi
 + 00bc05f4  6a00                   push 0
   00bc05ef  50                     push eax
   00bc05f0  ff9204010000           call dword ptr [edx + 0x104]
@@ retail 80-83 built 80-83
   00bc05f0  ff9204010000           call dword ptr [edx + 0x104]
   00bc05f6  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc05fb  8b8eb83c0000           mov ecx, dword ptr [esi + 0x3cb8]
 - 00bc0601  bf01000000             mov edi, 1
 - 00bc0606  3bcf                   cmp ecx, edi
 + 00bc0602  8b96b83c0000           mov edx, dword ptr [esi + 0x3cb8]
 + 00bc0608  b901000000             mov ecx, 1
 + 00bc060d  3bd1                   cmp edx, ecx
   00bc0608  7d0b                   jge 0xbc0615
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
@@ retail 84-85 built 84-85
   00bc0606  3bcf                   cmp ecx, edi
   00bc0608  7d0b                   jge 0xbc0615
 - 00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
 + 00bc0611  898eb83c0000           mov dword ptr [esi + 0x3cb8], ecx
   00bc0610  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0615  8b4c2448               mov ecx, dword ptr [esp + 0x48]
@@ retail 121-122 built 121-122
   00bc069a  ff820c280000           inc dword ptr [edx + 0x280c]
   00bc06a0  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc06a5  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc06ac  8b3510000000           mov esi, dword ptr [0x10]
   00bc06ab  39b020290000           cmp dword ptr [eax + 0x2920], esi
   00bc06b1  8d901c290000           lea edx, [eax + 0x291c]
@@ retail 148-149 built 148-149
   00bc0707  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc070d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0712  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0719  8b3514000000           mov esi, dword ptr [0x14]
   00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
   00bc071e  8d9030290000           lea edx, [eax + 0x2930]
@@ retail 182-182 built 182-198
   00bc0793  51                     push ecx
   00bc0794  eb06                   jmp 0xbc079c
 + 00bc0799  f6c110                 test cl, 0x10
 + 00bc079c  7439                   je 0xbc07d7
 + 00bc079e  8b3508000000           mov esi, dword ptr [8]
 + 00bc07a4  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
 + 00bc07aa  3bee                   cmp ebp, esi
 + 00bc07ac  8d8890280000           lea ecx, [eax + 0x2890]
 + 00bc07b2  8bd0                   mov edx, eax
 + 00bc07b4  8bfe                   mov edi, esi
 + 00bc07b6  0f84b3010000           je 0xbc096f
 + 00bc07bc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc07c2  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc07c5  85f0                   test eax, esi
 + 00bc07c7  0f8581010000           jne 0xbc094e
 + 00bc07cd  0bf0                   or esi, eax
 + 00bc07cf  897108                 mov dword ptr [ecx + 8], esi
 + 00bc07d2  e95a010000             jmp 0xbc0931
   00bc0796  6a00                   push 0
   00bc0798  8d5330                 lea edx, [ebx + 0x30]
@@ retail 196-197 built 212-213
   00bc07c3  83f907                 cmp ecx, 7
   00bc07c6  0f85d9010000           jne 0xbc09a5
 - 00bc07cc  8bb0d0280000           mov esi, dword ptr [eax + 0x28d0]
 + 00bc080d  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
   00bc07d2  3bf7                   cmp esi, edi
   00bc07d4  8d90cc280000           lea edx, [eax + 0x28cc]
@@ retail 198-199 built 214-215
   00bc07cc  8bb0d0280000           mov esi, dword ptr [eax + 0x28d0]
   00bc07d2  3bf7                   cmp esi, edi
 - 00bc07d4  8d90cc280000           lea edx, [eax + 0x28cc]
 + 00bc0815  8d90e0280000           lea edx, [eax + 0x28e0]
   00bc07da  c644244400             mov byte ptr [esp + 0x44], 0
   00bc07df  8bc8                   mov ecx, eax
@@ retail 223-251 built 239-240
   00bc082d  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0833  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0838  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
 - 00bc083e  8d90e0280000           lea edx, [eax + 0x28e0]
 - 00bc0844  bf10000000             mov edi, 0x10
 - 00bc0849  3bf7                   cmp esi, edi
 - 00bc084b  8bc8                   mov ecx, eax
 - 00bc084d  7455                   je 0xbc08a4
 - 00bc084f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0855  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0858  85f0                   test eax, esi
 - 00bc085a  7522                   jne 0xbc087e
 - 00bc085c  0bf0                   or esi, eax
 - 00bc085e  897208                 mov dword ptr [edx + 8], esi
 - 00bc0861  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0867  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc086b  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0871  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc0874  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc0878  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc087e  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc0881  84c0                   test al, al
 - 00bc0883  897a04                 mov dword ptr [edx + 4], edi
 - 00bc0886  7517                   jne 0xbc089f
 - 00bc0888  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc088c  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0892  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0899  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc089f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc08a4  8b35e86d3901           mov esi, dword ptr [0x1396de8]
 + 00bc0879  8b3510000000           mov esi, dword ptr [0x10]
   00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
   00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
@@ retail 286-287 built 275-276
   00bc0927  f6c110                 test cl, 0x10
   00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 + 00bc0905  8b3508000000           mov esi, dword ptr [8]
   00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
   00bc093c  3bee                   cmp ebp, esi
@@ retail 293-293 built 282-283
   00bc0946  8bfe                   mov edi, esi
   00bc0948  7450                   je 0xbc099a
 + 00bc091f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
   00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 294-296 built 284-285
   00bc0948  7450                   je 0xbc099a
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0953  85f0                   test eax, esi
 + 00bc0928  85c6                   test esi, eax
   00bc0955  7522                   jne 0xbc0979
   00bc0957  0bf0                   or esi, eax
@@ retail 297-299 built 286-288
   00bc0953  85f0                   test eax, esi
   00bc0955  7522                   jne 0xbc0979
 - 00bc0957  0bf0                   or esi, eax
 - 00bc0959  897108                 mov dword ptr [ecx + 8], esi
 + 00bc092c  0bc6                   or eax, esi
 + 00bc092e  894108                 mov dword ptr [ecx + 8], eax
   00bc095c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0962  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 326-327 built 315-316
   00bc09be  8bc8                   mov ecx, eax
   00bc09c0  7510                   jne 0xbc09d2
 - 00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
 + 00bc0997  8b3508000000           mov esi, dword ptr [8]
   00bc09c8  eb08                   jmp 0xbc09d2
   00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
@@ retail 328-329 built 317-318
   00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
   00bc09c8  eb08                   jmp 0xbc09d2
 - 00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
 + 00bc099f  8b3504000000           mov esi, dword ptr [4]
   00bc09d0  8bc8                   mov ecx, eax
   00bc09d2  3bde                   cmp ebx, esi
@@ retail 333-333 built 322-323
   00bc09d4  8bfe                   mov edi, esi
   00bc09d6  7455                   je 0xbc0a2d
 + 00bc09ad  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
   00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 334-336 built 324-325
   00bc09d6  7455                   je 0xbc0a2d
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc09e1  85f0                   test eax, esi
 + 00bc09b6  85c6                   test esi, eax
   00bc09e3  7522                   jne 0xbc0a07
   00bc09e5  0bf0                   or esi, eax
@@ retail 337-339 built 326-328
   00bc09e1  85f0                   test eax, esi
   00bc09e3  7522                   jne 0xbc0a07
 - 00bc09e5  0bf0                   or esi, eax
 - 00bc09e7  897208                 mov dword ptr [edx + 8], esi
 + 00bc09ba  0bc6                   or eax, esi
 + 00bc09bc  894208                 mov dword ptr [edx + 8], eax
   00bc09ea  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc09f0  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 362-363 built 351-352
   00bc0a46  8bc8                   mov ecx, eax
   00bc0a48  0f84d5000000           je 0xbc0b23
 - 00bc0a4e  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc0a23  8b3510000000           mov esi, dword ptr [0x10]
   00bc0a54  3bde                   cmp ebx, esi
   00bc0a56  8bfe                   mov edi, esi
@@ retail 368-369 built 357-358
   00bc0a5a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0a60  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0a63  85f0                   test eax, esi
 + 00bc0a38  85c6                   test esi, eax
   00bc0a65  7522                   jne 0xbc0a89
   00bc0a67  0bf0                   or esi, eax
@@ retail 370-372 built 359-361
   00bc0a63  85f0                   test eax, esi
   00bc0a65  7522                   jne 0xbc0a89
 - 00bc0a67  0bf0                   or esi, eax
 - 00bc0a69  897208                 mov dword ptr [edx + 8], esi
 + 00bc0a3c  0bc6                   or eax, esi
 + 00bc0a3e  894208                 mov dword ptr [edx + 8], eax
   00bc0a6c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0a72  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 387-388 built 376-377
   00bc0aa4  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0aaa  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0aaf  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0a84  8b3514000000           mov esi, dword ptr [0x14]
   00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0abb  3bde                   cmp ebx, esi
@@ retail 396-397 built 385-386
   00bc0ac9  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0acf  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0ad2  85f0                   test eax, esi
 + 00bc0aa7  85c6                   test esi, eax
   00bc0ad4  7522                   jne 0xbc0af8
   00bc0ad6  0bf0                   or esi, eax
@@ retail 398-400 built 387-389
   00bc0ad2  85f0                   test eax, esi
   00bc0ad4  7522                   jne 0xbc0af8
 - 00bc0ad6  0bf0                   or esi, eax
 - 00bc0ad8  897108                 mov dword ptr [ecx + 8], esi
 + 00bc0aab  0bc6                   or eax, esi
 + 00bc0aad  894108                 mov dword ptr [ecx + 8], eax
   00bc0adb  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0ae1  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 416-417 built 405-406
   00bc0b19  c644244400             mov byte ptr [esp + 0x44], 0
   00bc0b1e  e96c010000             jmp 0xbc0c8f
 - 00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0af8  8b3504000000           mov esi, dword ptr [4]
   00bc0b29  3bde                   cmp ebx, esi
   00bc0b2b  8bfe                   mov edi, esi
@@ retail 422-423 built 411-412
   00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0b35  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0b38  85f0                   test eax, esi
 + 00bc0b0d  85c6                   test esi, eax
   00bc0b3a  7522                   jne 0xbc0b5e
   00bc0b3c  0bf0                   or esi, eax
@@ retail 424-426 built 413-415
   00bc0b38  85f0                   test eax, esi
   00bc0b3a  7522                   jne 0xbc0b5e
 - 00bc0b3c  0bf0                   or esi, eax
 - 00bc0b3e  897208                 mov dword ptr [edx + 8], esi
 + 00bc0b11  0bc6                   or eax, esi
 + 00bc0b13  894208                 mov dword ptr [edx + 8], eax
   00bc0b41  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0b47  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 441-454 built 430-431
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
 + 00bc0b59  8b350c000000           mov esi, dword ptr [0xc]
   00bc0bb8  e98f000000             jmp 0xbc0c4c
   00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
@@ retail 455-456 built 432-433
   00bc0bb5  897108                 mov dword ptr [ecx + 8], esi
   00bc0bb8  e98f000000             jmp 0xbc0c4c
 - 00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0b61  8b3504000000           mov esi, dword ptr [4]
   00bc0bc3  3bde                   cmp ebx, esi
   00bc0bc5  8bc8                   mov ecx, eax
@@ retail 481-482 built 458-459
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0bc4  8b3504000000           mov esi, dword ptr [4]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0c2c  3bde                   cmp ebx, esi
@@ retail 521-531 built 498-509
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
 + 00bc0c57  8b454c                 mov eax, dword ptr [ebp + 0x4c]
 + 00bc0c5a  8b4d50                 mov ecx, dword ptr [ebp + 0x50]
 + 00bc0c5d  8b5548                 mov edx, dword ptr [ebp + 0x48]
 + 00bc0c60  89442424               mov dword ptr [esp + 0x24], eax
 + 00bc0c64  8d442448               lea eax, [esp + 0x48]
 + 00bc0c68  50                     push eax
 + 00bc0c69  894c242c               mov dword ptr [esp + 0x2c], ecx
 + 00bc0c6d  6a05                   push 5
 + 00bc0c6f  8d4c2428               lea ecx, [esp + 0x28]
 + 00bc0c73  89542428               mov dword ptr [esp + 0x28], edx
 + 00bc0c77  8b5570                 mov edx, dword ptr [ebp + 0x70]
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 533-540 built 511-515
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0ce2  8954242c               mov dword ptr [esp + 0x2c], edx
 - 00bc0ce6  89442424               mov dword ptr [esp + 0x24], eax
 - 00bc0cea  8b4570                 mov eax, dword ptr [ebp + 0x70]
 - 00bc0ced  6a05                   push 5
 - 00bc0cef  8d542428               lea edx, [esp + 0x28]
 - 00bc0cf3  52                     push edx
 - 00bc0cf4  89442438               mov dword ptr [esp + 0x38], eax
 + 00bc0c81  c644245400             mov byte ptr [esp + 0x54], 0
 + 00bc0c86  c644245500             mov byte ptr [esp + 0x55], 0
 + 00bc0c8b  c644245600             mov byte ptr [esp + 0x56], 0
 + 00bc0c90  89542438               mov dword ptr [esp + 0x38], edx
   00bc0cf8  e8935ef8ff             call 0xb46b90
   00bc0cfd  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 577-578 built 552-553
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 + 00bc0d16  8b1508000000           mov edx, dword ptr [8]
   00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
   00bc0d86  3bfa                   cmp edi, edx
@@ retail 583-583 built 558-559
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 + 00bc0d2e  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
   00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
@@ retail 584-585 built 560-560
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
@@ retail 698-698 built 673-674
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 + 00bc0e95  0544010000             add eax, 0x144
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
@@ retail 699-700 built 675-676
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 + 00bc0ea0  8b500c                 mov edx, dword ptr [eax + 0xc]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
@@ retail 702-703 built 678-678
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
@@ retail 706-707 built 681-682
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0ebb  8997b4000000           mov dword ptr [edi + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 818-819 built 793-794
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc1087  d80c8500000000         fmul dword ptr [eax*4]
   00bc10f2  d9c9                   fxch st(1)
   00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 820-821 built 795-796
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc1090  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 844-845 built 819-820
   00bc1146  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc10e9  d80c8500000000         fmul dword ptr [eax*4]
   00bc1154  d9c9                   fxch st(1)
   00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 846-847 built 821-822
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc10f2  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 973-975 built 948-949
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
   00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 + 00bc12b7  85c9                   test ecx, ecx
   00bc131f  7415                   je 0xbc1336
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 978-979 built 952-953
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
   00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 + 00bc12c4  c780e800000000000000   mov dword ptr [eax + 0xe8], 0
   00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
@@ retail 983-983 built 957-958
   00bc133c  e8ff44e4ff             call 0xa05840
   00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00bc12e3  33ff                   xor edi, edi
   00bc1345  3bf7                   cmp esi, edi
   00bc1347  7444                   je 0xbc138d
@@ retail 1005-1006 built 980-980
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
@@ retail 1008-1008 built 982-983
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
 + 00bc1322  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
@@ retail 1010-1010 built 985-986
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
 + 00bc132d  5f                     pop edi
   00bc138d  5e                     pop esi
   00bc138e  5d                     pop ebp
@@ retail 1012-1013 built 988-988
   00bc138d  5e                     pop esi
   00bc138e  5d                     pop ebp
 - 00bc138f  5f                     pop edi
   00bc1390  5b                     pop ebx
   00bc1391  83c430                 add esp, 0x30
DIFFLINES 154 retail_instr 1016 built_instr 991
