00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(3549v3751) flags= []
@@ retail 0-1 built 0-1
 - 00bc04f0  83ec30                 sub esp, 0x30
 + 00bc04f0  83ec20                 sub esp, 0x20
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
   00bc04f8  53                     push ebx
@@ retail 9-12 built 9-11
   00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
   00bc0504  0f84850e0000           je 0xbc138f
 - 00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc050e  8b0d986e4301           mov ecx, dword ptr [0x1436e98]
 - 00bc0514  8b8954090000           mov ecx, dword ptr [ecx + 0x954]
 + 00bc050a  8b44242c               mov eax, dword ptr [esp + 0x2c]
 + 00bc050e  8b1500000000           mov edx, dword ptr [0]
   00bc051a  55                     push ebp
   00bc051b  8b680c                 mov ebp, dword ptr [eax + 0xc]
@@ retail 14-16 built 13-16
   00bc051a  55                     push ebp
   00bc051b  8b680c                 mov ebp, dword ptr [eax + 0xc]
 - 00bc051e  8b5554                 mov edx, dword ptr [ebp + 0x54]
 - 00bc0521  52                     push edx
 + 00bc0518  8b4d54                 mov ecx, dword ptr [ebp + 0x54]
 + 00bc051b  51                     push ecx
 + 00bc051c  8b8a54090000           mov ecx, dword ptr [edx + 0x954]
   00bc0522  896c2418               mov dword ptr [esp + 0x18], ebp
   00bc0526  e865bce0ff             call 0x9cc190
@@ retail 49-49 built 49-52
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
 + 00bc058b  755e                   jne 0xbc05eb
 + 00bc058d  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00bc0591  3bf7                   cmp esi, edi
   00bc058b  0f84b00d0000           je 0xbc1341
   00bc0591  a108b53c01             mov eax, dword ptr [0x13cb508]
@@ retail 50-50 built 53-84
   00bc0589  84c0                   test al, al
   00bc058b  0f84b00d0000           je 0xbc1341
 + 00bc0599  ff0e                   dec dword ptr [esi]
 + 00bc059b  393e                   cmp dword ptr [esi], edi
 + 00bc059d  0f85200d0000           jne 0xbc12c3
 + 00bc05a3  f6461a10               test byte ptr [esi + 0x1a], 0x10
 + 00bc05a7  7407                   je 0xbc05b0
 + 00bc05a9  8bce                   mov ecx, esi
 + 00bc05ab  e800000000             call 0xbc05b0
 + 00bc05b0  8b460c                 mov eax, dword ptr [esi + 0xc]
 + 00bc05b3  3bc7                   cmp eax, edi
 + 00bc05b5  741b                   je 0xbc05d2
 + 00bc05b7  ff08                   dec dword ptr [eax]
 + 00bc05b9  8b460c                 mov eax, dword ptr [esi + 0xc]
 + 00bc05bc  3938                   cmp dword ptr [eax], edi
 + 00bc05be  7512                   jne 0xbc05d2
 + 00bc05c0  8b4808                 mov ecx, dword ptr [eax + 8]
 + 00bc05c3  ff5004                 call dword ptr [eax + 4]
 + 00bc05c6  8b460c                 mov eax, dword ptr [esi + 0xc]
 + 00bc05c9  50                     push eax
 + 00bc05ca  e800000000             call 0xbc05cf
 + 00bc05cf  83c404                 add esp, 4
 + 00bc05d2  56                     push esi
 + 00bc05d3  897e08                 mov dword ptr [esi + 8], edi
 + 00bc05d6  897e0c                 mov dword ptr [esi + 0xc], edi
 + 00bc05d9  e800000000             call 0xbc05de
 + 00bc05de  83c404                 add esp, 4
 + 00bc05e1  5e                     pop esi
 + 00bc05e2  5d                     pop ebp
 + 00bc05e3  5f                     pop edi
 + 00bc05e4  5b                     pop ebx
 + 00bc05e5  83c420                 add esp, 0x20
 + 00bc05e8  c20800                 ret 8
   00bc0591  a108b53c01             mov eax, dword ptr [0x13cb508]
   00bc0596  8bb008280000           mov esi, dword ptr [eax + 0x2808]
@@ retail 86-87 built 120-121
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
   00bc0610  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0615  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 + 00bc066f  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00bc0619  83f906                 cmp ecx, 6
   00bc061c  0f85a1010000           jne 0xbc07c3
@@ retail 95-97 built 129-158
   00bc0633  8b35d06e3901           mov esi, dword ptr [0x1396ed0]
   00bc0639  39b0402c0000           cmp dword ptr [eax + 0x2c40], esi
 - 00bc063f  8d883c2c0000           lea ecx, [eax + 0x2c3c]
 - 00bc0645  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc0699  8d903c2c0000           lea edx, [eax + 0x2c3c]
 + 00bc069f  8bc8                   mov ecx, eax
 + 00bc06a1  8bfe                   mov edi, esi
 + 00bc06a3  7455                   je 0xbc06fa
 + 00bc06a5  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc06ab  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc06ae  85f0                   test eax, esi
 + 00bc06b0  7522                   jne 0xbc06d4
 + 00bc06b2  0bf0                   or esi, eax
 + 00bc06b4  897208                 mov dword ptr [edx + 8], esi
 + 00bc06b7  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc06bd  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc06c1  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc06c7  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc06ca  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc06ce  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc06d4  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc06d7  84c0                   test al, al
 + 00bc06d9  897a04                 mov dword ptr [edx + 4], edi
 + 00bc06dc  7517                   jne 0xbc06f5
 + 00bc06de  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc06e2  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc06e8  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc06ef  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc06f5  a100000000             mov eax, dword ptr [0]
 + 00bc06fa  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc0700  39b020290000           cmp dword ptr [eax + 0x2920], esi
 + 00bc0706  8d881c290000           lea ecx, [eax + 0x291c]
 + 00bc070c  c644243400             mov byte ptr [esp + 0x34], 0
   00bc064a  8bd0                   mov edx, eax
   00bc064c  8bfe                   mov edi, esi
@@ retail 121-149 built 182-183
   00bc069a  ff820c280000           inc dword ptr [edx + 0x280c]
   00bc06a0  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc06a5  8b35786f3901           mov esi, dword ptr [0x1396f78]
 - 00bc06ab  39b020290000           cmp dword ptr [eax + 0x2920], esi
 - 00bc06b1  8d901c290000           lea edx, [eax + 0x291c]
 - 00bc06b7  8bc8                   mov ecx, eax
 - 00bc06b9  8bfe                   mov edi, esi
 - 00bc06bb  7455                   je 0xbc0712
 - 00bc06bd  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc06c3  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc06c6  85f0                   test eax, esi
 - 00bc06c8  7522                   jne 0xbc06ec
 - 00bc06ca  0bf0                   or esi, eax
 - 00bc06cc  897208                 mov dword ptr [edx + 8], esi
 - 00bc06cf  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc06d5  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc06d9  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc06df  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc06e2  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc06e6  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc06ec  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc06ef  84c0                   test al, al
 - 00bc06f1  897a04                 mov dword ptr [edx + 4], edi
 - 00bc06f4  7517                   jne 0xbc070d
 - 00bc06f6  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc06fa  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0700  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0707  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc070d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0712  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc076c  8b3514000000           mov esi, dword ptr [0x14]
   00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
   00bc071e  8d9030290000           lea edx, [eax + 0x2930]
@@ retail 192-193 built 226-227
   00bc07b1  e88a79dcff             call 0x988140
   00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc07ba  8b742418               mov esi, dword ptr [esp + 0x18]
 + 00bc0814  8b6c2418               mov ebp, dword ptr [esp + 0x18]
   00bc07be  e9be060000             jmp 0xbc0e81
   00bc07c3  83f907                 cmp ecx, 7
@@ retail 199-200 built 233-233
   00bc07d2  3bf7                   cmp esi, edi
   00bc07d4  8d90cc280000           lea edx, [eax + 0x28cc]
 - 00bc07da  c644244400             mov byte ptr [esp + 0x44], 0
   00bc07df  8bc8                   mov ecx, eax
   00bc07e1  7455                   je 0xbc0838
@@ retail 227-227 built 260-261
   00bc0844  bf10000000             mov edi, 0x10
   00bc0849  3bf7                   cmp esi, edi
 + 00bc08a0  c644243400             mov byte ptr [esp + 0x34], 0
   00bc084b  8bc8                   mov ecx, eax
   00bc084d  7455                   je 0xbc08a4
@@ retail 250-251 built 284-285
   00bc0899  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc089f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc08a4  8b35e86d3901           mov esi, dword ptr [0x1396de8]
 + 00bc08fe  8b3510000000           mov esi, dword ptr [0x10]
   00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
   00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
@@ retail 286-287 built 320-321
   00bc0927  f6c110                 test cl, 0x10
   00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 + 00bc098a  8b3508000000           mov esi, dword ptr [8]
   00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
   00bc093c  3bee                   cmp ebp, esi
@@ retail 319-358 built 353-353
   00bc09a5  8b4d7c                 mov ecx, dword ptr [ebp + 0x7c]
   00bc09a8  49                     dec ecx
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
   00bc0a3f  0f8478010000           je 0xbc0bbd
   00bc0a45  49                     dec ecx
@@ retail 360-360 built 355-365
   00bc0a3f  0f8478010000           je 0xbc0bbd
   00bc0a45  49                     dec ecx
 + 00bc0a06  7404                   je 0xbc0a0c
 + 00bc0a08  33f6                   xor esi, esi
 + 00bc0a0a  eb09                   jmp 0xbc0a15
 + 00bc0a0c  be02000000             mov esi, 2
 + 00bc0a11  eb02                   jmp 0xbc0a15
 + 00bc0a13  8bf7                   mov esi, edi
 + 00bc0a15  8b3cb500000000         mov edi, dword ptr [esi*4]
 + 00bc0a1c  8bb0402c0000           mov esi, dword ptr [eax + 0x2c40]
 + 00bc0a22  3bf7                   cmp esi, edi
 + 00bc0a24  8d903c2c0000           lea edx, [eax + 0x2c3c]
   00bc0a46  8bc8                   mov ecx, eax
   00bc0a48  0f84d5000000           je 0xbc0b23
@@ retail 361-365 built 366-366
   00bc0a45  49                     dec ecx
   00bc0a46  8bc8                   mov ecx, eax
 - 00bc0a48  0f84d5000000           je 0xbc0b23
 - 00bc0a4e  8b35786f3901           mov esi, dword ptr [0x1396f78]
 - 00bc0a54  3bde                   cmp ebx, esi
 - 00bc0a56  8bfe                   mov edi, esi
   00bc0a58  7455                   je 0xbc0aaf
   00bc0a5a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 368-369 built 369-370
   00bc0a5a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0a60  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0a63  85f0                   test eax, esi
 + 00bc0a37  85c6                   test esi, eax
   00bc0a65  7522                   jne 0xbc0a89
   00bc0a67  0bf0                   or esi, eax
@@ retail 370-372 built 371-373
   00bc0a63  85f0                   test eax, esi
   00bc0a65  7522                   jne 0xbc0a89
 - 00bc0a67  0bf0                   or esi, eax
 - 00bc0a69  897208                 mov dword ptr [edx + 8], esi
 + 00bc0a3b  0bc6                   or eax, esi
 + 00bc0a3d  894208                 mov dword ptr [edx + 8], eax
   00bc0a6c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0a72  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 387-389 built 388-397
   00bc0aa4  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0aaa  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0aaf  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 - 00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 + 00bc0a83  8b4d78                 mov ecx, dword ptr [ebp + 0x78]
 + 00bc0a86  83e903                 sub ecx, 3
 + 00bc0a89  8b9820290000           mov ebx, dword ptr [eax + 0x2920]
 + 00bc0a8f  8d901c290000           lea edx, [eax + 0x291c]
 + 00bc0a95  0f8447010000           je 0xbc0be2
 + 00bc0a9b  49                     dec ecx
 + 00bc0a9c  8bc8                   mov ecx, eax
 + 00bc0a9e  0f84d5000000           je 0xbc0b79
 + 00bc0aa4  8b3510000000           mov esi, dword ptr [0x10]
   00bc0abb  3bde                   cmp ebx, esi
   00bc0abd  8d8830290000           lea ecx, [eax + 0x2930]
@@ retail 390-458 built 398-398
   00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0abb  3bde                   cmp ebx, esi
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
   00bc0bc7  8bfe                   mov edi, esi
   00bc0bc9  745b                   je 0xbc0c26
@@ retail 481-482 built 421-422
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0b05  8b3514000000           mov esi, dword ptr [0x14]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0c2c  3bde                   cmp ebx, esi
@@ retail 508-509 built 448-531
   00bc0c7d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
   00bc0c84  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc0c8a  c644244401             mov byte ptr [esp + 0x44], 1
 + 00bc0b6f  c644243400             mov byte ptr [esp + 0x34], 0
 + 00bc0b74  e93b010000             jmp 0xbc0cb4
 + 00bc0b79  8b3504000000           mov esi, dword ptr [4]
 + 00bc0b7f  3bde                   cmp ebx, esi
 + 00bc0b81  8bfe                   mov edi, esi
 + 00bc0b83  7455                   je 0xbc0bda
 + 00bc0b85  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0b8b  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc0b8e  85c6                   test esi, eax
 + 00bc0b90  7522                   jne 0xbc0bb4
 + 00bc0b92  0bc6                   or eax, esi
 + 00bc0b94  894208                 mov dword ptr [edx + 8], eax
 + 00bc0b97  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0b9d  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc0ba1  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0ba7  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc0baa  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc0bae  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc0bb4  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc0bb7  84c0                   test al, al
 + 00bc0bb9  897a04                 mov dword ptr [edx + 4], edi
 + 00bc0bbc  7517                   jne 0xbc0bd5
 + 00bc0bbe  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc0bc2  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0bc8  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc0bcf  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc0bd5  a100000000             mov eax, dword ptr [0]
 + 00bc0bda  8b350c000000           mov esi, dword ptr [0xc]
 + 00bc0be0  eb69                   jmp 0xbc0c4b
 + 00bc0be2  8b3504000000           mov esi, dword ptr [4]
 + 00bc0be8  3bde                   cmp ebx, esi
 + 00bc0bea  8bc8                   mov ecx, eax
 + 00bc0bec  8bfe                   mov edi, esi
 + 00bc0bee  745b                   je 0xbc0c4b
 + 00bc0bf0  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0bf6  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc0bf9  85c6                   test esi, eax
 + 00bc0bfb  7522                   jne 0xbc0c1f
 + 00bc0bfd  0bc6                   or eax, esi
 + 00bc0bff  894208                 mov dword ptr [edx + 8], eax
 + 00bc0c02  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0c08  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc0c0c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0c12  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc0c15  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc0c19  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc0c1f  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc0c22  84c0                   test al, al
 + 00bc0c24  897a04                 mov dword ptr [edx + 4], edi
 + 00bc0c27  7517                   jne 0xbc0c40
 + 00bc0c29  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc0c2d  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0c33  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc0c3a  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc0c40  a100000000             mov eax, dword ptr [0]
 + 00bc0c45  8b3504000000           mov esi, dword ptr [4]
 + 00bc0c4b  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 + 00bc0c51  3bde                   cmp ebx, esi
 + 00bc0c53  8d8830290000           lea ecx, [eax + 0x2930]
 + 00bc0c59  8bd0                   mov edx, eax
 + 00bc0c5b  8bfe                   mov edi, esi
 + 00bc0c5d  7450                   je 0xbc0caf
 + 00bc0c5f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0c65  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc0c68  85c6                   test esi, eax
 + 00bc0c6a  7522                   jne 0xbc0c8e
 + 00bc0c6c  0bc6                   or eax, esi
 + 00bc0c6e  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0c71  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc0c77  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 + 00bc0c7b  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc0c81  8b7104                 mov esi, dword ptr [ecx + 4]
 + 00bc0c84  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 + 00bc0c88  ff8208280000           inc dword ptr [edx + 0x2808]
 + 00bc0c8e  8a4110                 mov al, byte ptr [ecx + 0x10]
 + 00bc0c91  84c0                   test al, al
 + 00bc0c93  897904                 mov dword ptr [ecx + 4], edi
 + 00bc0c96  7517                   jne 0xbc0caf
 + 00bc0c98  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0c9c  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 + 00bc0ca2  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 + 00bc0ca9  ff820c280000           inc dword ptr [edx + 0x280c]
 + 00bc0caf  c644243401             mov byte ptr [esp + 0x34], 1
   00bc0c8f  8a8584000000           mov al, byte ptr [ebp + 0x84]
   00bc0c95  8b0d846e4301           mov ecx, dword ptr [0x1436e84]
@@ retail 521-531 built 543-554
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
 + 00bc0cd8  8b454c                 mov eax, dword ptr [ebp + 0x4c]
 + 00bc0cdb  8b4d50                 mov ecx, dword ptr [ebp + 0x50]
 + 00bc0cde  8b5548                 mov edx, dword ptr [ebp + 0x48]
 + 00bc0ce1  89442424               mov dword ptr [esp + 0x24], eax
 + 00bc0ce5  8d442438               lea eax, [esp + 0x38]
 + 00bc0ce9  50                     push eax
 + 00bc0cea  894c242c               mov dword ptr [esp + 0x2c], ecx
 + 00bc0cee  6a05                   push 5
 + 00bc0cf0  8d4c2428               lea ecx, [esp + 0x28]
 + 00bc0cf4  89542428               mov dword ptr [esp + 0x28], edx
 + 00bc0cf8  8b5570                 mov edx, dword ptr [ebp + 0x70]
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 533-540 built 556-560
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0ce2  8954242c               mov dword ptr [esp + 0x2c], edx
 - 00bc0ce6  89442424               mov dword ptr [esp + 0x24], eax
 - 00bc0cea  8b4570                 mov eax, dword ptr [ebp + 0x70]
 - 00bc0ced  6a05                   push 5
 - 00bc0cef  8d542428               lea edx, [esp + 0x28]
 - 00bc0cf3  52                     push edx
 - 00bc0cf4  89442438               mov dword ptr [esp + 0x38], eax
 + 00bc0d02  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc0d07  c644244500             mov byte ptr [esp + 0x45], 0
 + 00bc0d0c  c644244600             mov byte ptr [esp + 0x46], 0
 + 00bc0d11  89542438               mov dword ptr [esp + 0x38], edx
   00bc0cf8  e8935ef8ff             call 0xb46b90
   00bc0cfd  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 546-553 built 566-569
   00bc0d06  e81542f8ff             call 0xb44f20
   00bc0d0b  eb21                   jmp 0xbc0d2e
 - 00bc0d0d  a19c6e4301             mov eax, dword ptr [0x1436e9c]
 - 00bc0d12  8b90e8000000           mov edx, dword ptr [eax + 0xe8]
 - 00bc0d18  b964000000             mov ecx, 0x64
 - 00bc0d1d  3bd1                   cmp edx, ecx
 - 00bc0d1f  740d                   je 0xbc0d2e
 - 00bc0d21  8988e8000000           mov dword ptr [eax + 0xe8], ecx
 - 00bc0d27  8388e400000002         or dword ptr [eax + 0xe4], 2
 + 00bc0d2a  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc0d30  6a64                   push 0x64
 + 00bc0d32  e800000000             call 0xbc0d37
   00bc0d2e  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
   00bc0d34  e8475ff8ff             call 0xb46c80
@@ retail 560-562 built 576-577
   00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
 - 00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
 - 00bc0d4d  2bd5                   sub edx, ebp
 + 00bc0d53  2b5124                 sub edx, dword ptr [ecx + 0x24]
   00bc0d4f  c1fa03                 sar edx, 3
   00bc0d52  4a                     dec edx
@@ retail 565-566 built 580-580
   00bc0d52  4a                     dec edx
   00bc0d53  3bf2                   cmp esi, edx
 - 00bc0d55  8bc6                   mov eax, esi
   00bc0d57  760b                   jbe 0xbc0d64
   00bc0d59  8b4128                 mov eax, dword ptr [ecx + 0x28]
@@ retail 568-569 built 582-583
   00bc0d57  760b                   jbe 0xbc0d64
   00bc0d59  8b4128                 mov eax, dword ptr [ecx + 0x28]
 - 00bc0d5c  8bfd                   mov edi, ebp
 + 00bc0d61  8b7924                 mov edi, dword ptr [ecx + 0x24]
   00bc0d5e  2bc7                   sub eax, edi
   00bc0d60  c1f803                 sar eax, 3
@@ retail 572-572 built 586-588
   00bc0d60  c1f803                 sar eax, 3
   00bc0d63  48                     dec eax
 + 00bc0d6a  8bcf                   mov ecx, edi
 + 00bc0d6c  e9df000000             jmp 0xbc0e50
   00bc0d64  8b4924                 mov ecx, dword ptr [ecx + 0x24]
   00bc0d67  e9d1000000             jmp 0xbc0e3d
@@ retail 573-573 built 589-590
   00bc0d63  48                     dec eax
   00bc0d64  8b4924                 mov ecx, dword ptr [ecx + 0x24]
 + 00bc0d74  8bc6                   mov eax, esi
   00bc0d67  e9d1000000             jmp 0xbc0e3d
   00bc0d6c  a810                   test al, 0x10
@@ retail 577-578 built 594-595
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 + 00bc0d89  8b1508000000           mov edx, dword ptr [8]
   00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
   00bc0d86  3bfa                   cmp edi, edx
@@ retail 583-583 built 600-601
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 + 00bc0da1  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
   00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
@@ retail 584-585 built 602-602
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
@@ retail 604-611 built 621-627
   00bc0ddc  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0de2  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0de6  8b4f1c                 mov ecx, dword ptr [edi + 0x1c]
 - 00bc0de9  8b6f18                 mov ebp, dword ptr [edi + 0x18]
 - 00bc0dec  2bcd                   sub ecx, ebp
 - 00bc0dee  c1f903                 sar ecx, 3
 - 00bc0df1  49                     dec ecx
 - 00bc0df2  3bf1                   cmp esi, ecx
 - 00bc0df4  8bc6                   mov eax, esi
 + 00bc0df5  8b471c                 mov eax, dword ptr [edi + 0x1c]
 + 00bc0df8  8b4f18                 mov ecx, dword ptr [edi + 0x18]
 + 00bc0dfb  2bc1                   sub eax, ecx
 + 00bc0dfd  c1f803                 sar eax, 3
 + 00bc0e00  48                     dec eax
 + 00bc0e01  3bf0                   cmp esi, eax
   00bc0df6  760b                   jbe 0xbc0e03
   00bc0df8  8b471c                 mov eax, dword ptr [edi + 0x1c]
@@ retail 613-614 built 629-630
   00bc0df6  760b                   jbe 0xbc0e03
   00bc0df8  8b471c                 mov eax, dword ptr [edi + 0x1c]
 - 00bc0dfb  8bf5                   mov esi, ebp
 + 00bc0e08  8bf1                   mov esi, ecx
   00bc0dfd  2bc6                   sub eax, esi
   00bc0dff  c1f803                 sar eax, 3
@@ retail 617-618 built 633-637
   00bc0dff  c1f803                 sar eax, 3
   00bc0e02  48                     dec eax
 - 00bc0e03  8b5718                 mov edx, dword ptr [edi + 0x18]
 + 00bc0e10  eb02                   jmp 0xbc0e14
 + 00bc0e12  8bc6                   mov eax, esi
 + 00bc0e14  8b4f18                 mov ecx, dword ptr [edi + 0x18]
 + 00bc0e17  8d14c1                 lea edx, [ecx + eax*8]
   00bc0e06  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0e0c  6a00                   push 0
@@ retail 620-622 built 639-640
   00bc0e06  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0e0c  6a00                   push 0
 - 00bc0e0e  8d04c2                 lea eax, [edx + eax*8]
 - 00bc0e11  50                     push eax
 + 00bc0e22  52                     push edx
   00bc0e12  e80972dcff             call 0x988020
   00bc0e17  eb39                   jmp 0xbc0e52
@@ retail 625-632 built 643-649
   00bc0e17  eb39                   jmp 0xbc0e52
   00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 - 00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
 - 00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
 - 00bc0e23  2bd5                   sub edx, ebp
 - 00bc0e25  c1fa03                 sar edx, 3
 - 00bc0e28  4a                     dec edx
 - 00bc0e29  3bf2                   cmp esi, edx
 - 00bc0e2b  8bc6                   mov eax, esi
 + 00bc0e2e  8b4110                 mov eax, dword ptr [ecx + 0x10]
 + 00bc0e31  8b510c                 mov edx, dword ptr [ecx + 0xc]
 + 00bc0e34  2bc2                   sub eax, edx
 + 00bc0e36  c1f803                 sar eax, 3
 + 00bc0e39  48                     dec eax
 + 00bc0e3a  3bf0                   cmp esi, eax
   00bc0e2d  760b                   jbe 0xbc0e3a
   00bc0e2f  8b4110                 mov eax, dword ptr [ecx + 0x10]
@@ retail 634-635 built 651-652
   00bc0e2d  760b                   jbe 0xbc0e3a
   00bc0e2f  8b4110                 mov eax, dword ptr [ecx + 0x10]
 - 00bc0e32  8bfd                   mov edi, ebp
 + 00bc0e41  8bfa                   mov edi, edx
   00bc0e34  2bc7                   sub eax, edi
   00bc0e36  c1f803                 sar eax, 3
@@ retail 638-638 built 655-657
   00bc0e36  c1f803                 sar eax, 3
   00bc0e39  48                     dec eax
 + 00bc0e49  eb02                   jmp 0xbc0e4d
 + 00bc0e4b  8bc6                   mov eax, esi
   00bc0e3a  8b490c                 mov ecx, dword ptr [ecx + 0xc]
   00bc0e3d  8d14c1                 lea edx, [ecx + eax*8]
@@ retail 645-647 built 664-666
   00bc0e49  e8d271dcff             call 0x988020
   00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0e52  8b742418               mov esi, dword ptr [esp + 0x18]
 - 00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
 + 00bc0e65  8b6c2418               mov ebp, dword ptr [esp + 0x18]
 + 00bc0e69  f6858400000020         test byte ptr [ebp + 0x84], 0x20
   00bc0e5d  7413                   je 0xbc0e72
   00bc0e5f  8d4748                 lea eax, [edi + 0x48]
@@ retail 664-665 built 683-684
   00bc0e8a  85c0                   test eax, eax
   00bc0e8c  7418                   je 0xbc0ea6
 - 00bc0e8e  0fb65810               movzx ebx, byte ptr [eax + 0x10]
 + 00bc0ea1  0fb67010               movzx esi, byte ptr [eax + 0x10]
   00bc0e92  8b4004                 mov eax, dword ptr [eax + 4]
   00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
@@ retail 668-669 built 687-688
   00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
   00bc0e9b  8b11                   mov edx, dword ptr [ecx]
 - 00bc0e9d  53                     push ebx
 + 00bc0eb0  56                     push esi
   00bc0e9e  6a00                   push 0
   00bc0ea0  50                     push eax
@@ retail 697-698 built 716-718
   00bc0eea  a1a06e4301             mov eax, dword ptr [0x1436ea0]
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
 - 00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 + 00bc0f08  8b74241c               mov esi, dword ptr [esp + 0x1c]
 + 00bc0f0c  0544010000             add eax, 0x144
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
@@ retail 699-700 built 719-720
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 + 00bc0f17  8b500c                 mov edx, dword ptr [eax + 0xc]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
@@ retail 701-703 built 721-722
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
 - 00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
 + 00bc0f20  8b5568                 mov edx, dword ptr [ebp + 0x68]
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
@@ retail 704-705 built 723-724
   00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
 - 00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
 + 00bc0f29  8b556c                 mov edx, dword ptr [ebp + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
   00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
@@ retail 706-707 built 725-726
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0f32  8997b4000000           mov dword ptr [edi + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 711-717 built 730-736
   00bc0f2e  8b5010                 mov edx, dword ptr [eax + 0x10]
   00bc0f31  8997bc000000           mov dword ptr [edi + 0xbc], edx
 - 00bc0f37  0fb75306               movzx edx, word ptr [ebx + 6]
 - 00bc0f3b  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f3f  0fb713                 movzx edx, word ptr [ebx]
 - 00bc0f42  db442448               fild dword ptr [esp + 0x48]
 - 00bc0f46  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f4a  da742448               fidiv dword ptr [esp + 0x48]
 + 00bc0f4a  0fb75606               movzx edx, word ptr [esi + 6]
 + 00bc0f4e  89542438               mov dword ptr [esp + 0x38], edx
 + 00bc0f52  0fb716                 movzx edx, word ptr [esi]
 + 00bc0f55  db442438               fild dword ptr [esp + 0x38]
 + 00bc0f59  89542438               mov dword ptr [esp + 0x38], edx
 + 00bc0f5d  da742438               fidiv dword ptr [esp + 0x38]
   00bc0f4e  d99fc0000000           fstp dword ptr [edi + 0xc0]
   00bc0f54  0fb75308               movzx edx, word ptr [ebx + 8]
@@ retail 718-724 built 737-743
   00bc0f4a  da742448               fidiv dword ptr [esp + 0x48]
   00bc0f4e  d99fc0000000           fstp dword ptr [edi + 0xc0]
 - 00bc0f54  0fb75308               movzx edx, word ptr [ebx + 8]
 - 00bc0f58  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f5c  0fb75302               movzx edx, word ptr [ebx + 2]
 - 00bc0f60  db442448               fild dword ptr [esp + 0x48]
 - 00bc0f64  89542448               mov dword ptr [esp + 0x48], edx
 - 00bc0f68  da742448               fidiv dword ptr [esp + 0x48]
 + 00bc0f67  0fb75608               movzx edx, word ptr [esi + 8]
 + 00bc0f6b  89542438               mov dword ptr [esp + 0x38], edx
 + 00bc0f6f  0fb75602               movzx edx, word ptr [esi + 2]
 + 00bc0f73  db442438               fild dword ptr [esp + 0x38]
 + 00bc0f77  89542438               mov dword ptr [esp + 0x38], edx
 + 00bc0f7b  da742438               fidiv dword ptr [esp + 0x38]
   00bc0f6c  d99fc4000000           fstp dword ptr [edi + 0xc4]
   00bc0f72  8b5008                 mov edx, dword ptr [eax + 8]
@@ retail 729-730 built 748-749
   00bc0f7b  8b4014                 mov eax, dword ptr [eax + 0x14]
   00bc0f7e  8987cc000000           mov dword ptr [edi + 0xcc], eax
 - 00bc0f84  f6868400000008         test byte ptr [esi + 0x84], 8
 + 00bc0f97  f6858400000008         test byte ptr [ebp + 0x84], 8
   00bc0f8b  7408                   je 0xbc0f95
   00bc0f8d  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 736-736 built 755-756
   00bc0f9b  51                     push ecx
   00bc0f9c  d99fd0000000           fstp dword ptr [edi + 0xd0]
 + 00bc0fb5  33db                   xor ebx, ebx
   00bc0fa2  6a03                   push 3
   00bc0fa4  c787d400000000000000   mov dword ptr [edi + 0xd4], 0
@@ retail 737-738 built 757-758
   00bc0f9c  d99fd0000000           fstp dword ptr [edi + 0xd0]
   00bc0fa2  6a03                   push 3
 - 00bc0fa4  c787d400000000000000   mov dword ptr [edi + 0xd4], 0
 + 00bc0fb9  899fd4000000           mov dword ptr [edi + 0xd4], ebx
   00bc0fae  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0fb4  6a01                   push 1
@@ retail 741-742 built 761-762
   00bc0fb4  6a01                   push 1
   00bc0fb6  e81587dcff             call 0x9896d0
 - 00bc0fbb  8a8683000000           mov al, byte ptr [esi + 0x83]
 + 00bc0fcc  8a8583000000           mov al, byte ptr [ebp + 0x83]
   00bc0fc1  84c0                   test al, al
   00bc0fc3  0f86cd020000           jbe 0xbc1296
@@ retail 744-745 built 764-765
   00bc0fc1  84c0                   test al, al
   00bc0fc3  0f86cd020000           jbe 0xbc1296
 - 00bc0fc9  d94660                 fld dword ptr [esi + 0x60]
 + 00bc0fda  d94560                 fld dword ptr [ebp + 0x60]
   00bc0fcc  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00bc0fd2  dfe0                   fnstsw ax
@@ retail 749-750 built 769-770
   00bc0fd4  f6c441                 test ah, 0x41
   00bc0fd7  0f85b9020000           jne 0xbc1296
 - 00bc0fdd  d94664                 fld dword ptr [esi + 0x64]
 + 00bc0fee  d94564                 fld dword ptr [ebp + 0x64]
   00bc0fe0  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00bc0fe6  dfe0                   fnstsw ax
@@ retail 754-757 built 774-777
   00bc0fe8  f6c441                 test ah, 0x41
   00bc0feb  0f85a5020000           jne 0xbc1296
 - 00bc0ff1  8b4e5c                 mov ecx, dword ptr [esi + 0x5c]
 - 00bc0ff4  6a00                   push 0
 - 00bc0ff6  6a00                   push 0
 + 00bc1002  8b4d5c                 mov ecx, dword ptr [ebp + 0x5c]
 + 00bc1005  53                     push ebx
 + 00bc1006  53                     push ebx
   00bc0ff8  51                     push ecx
   00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
@@ retail 760-761 built 780-781
   00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
   00bc0ffd  52                     push edx
 - 00bc0ffe  8bcb                   mov ecx, ebx
 + 00bc100d  8bce                   mov ecx, esi
   00bc1000  e8cbe8e3ff             call 0x9ff8d0
   00bc1005  84c0                   test al, al
@@ retail 764-779 built 784-789
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
 + 00bc101c  81c580000000           add ebp, 0x80
 + 00bc1022  55                     push ebp
 + 00bc1023  8d4c2424               lea ecx, [esp + 0x24]
 + 00bc1027  e800000000             call 0xbc102c
 + 00bc102c  8a442434               mov al, byte ptr [esp + 0x34]
   00bc1059  84c0                   test al, al
   00bc105b  d80d24172301           fmul dword ptr [0x1231724]
@@ retail 780-784 built 790-790
   00bc1055  8a442444               mov al, byte ptr [esp + 0x44]
   00bc1059  84c0                   test al, al
 - 00bc105b  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc1061  db442448               fild dword ptr [esp + 0x48]
 - 00bc1065  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc106b  d95c243c               fstp dword ptr [esp + 0x3c]
   00bc106f  7406                   je 0xbc1077
   00bc1071  d944243c               fld dword ptr [esp + 0x3c]
@@ retail 785-786 built 791-792
   00bc106b  d95c243c               fstp dword ptr [esp + 0x3c]
   00bc106f  7406                   je 0xbc1077
 - 00bc1071  d944243c               fld dword ptr [esp + 0x3c]
 + 00bc1034  d944242c               fld dword ptr [esp + 0x2c]
   00bc1075  eb06                   jmp 0xbc107d
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 788-788 built 794-795
   00bc1075  eb06                   jmp 0xbc107d
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
 + 00bc1040  d9442420               fld dword ptr [esp + 0x20]
   00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1081  d9ca                   fxch st(2)
@@ retail 789-791 built 796-798
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
   00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
 - 00bc1081  d9ca                   fxch st(2)
 - 00bc1083  d8ca                   fmul st(2)
 + 00bc1048  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc104e  d8c9                   fmul st(1)
   00bc1085  6a00                   push 0
   00bc1087  8d4e48                 lea ecx, [esi + 0x48]
@@ retail 792-797 built 799-803
   00bc1083  d8ca                   fmul st(2)
   00bc1085  6a00                   push 0
 - 00bc1087  8d4e48                 lea ecx, [esi + 0x48]
 - 00bc108a  51                     push ecx
 - 00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc1091  d95c2428               fstp dword ptr [esp + 0x28]
 - 00bc1095  d944243c               fld dword ptr [esp + 0x3c]
 + 00bc1052  8d4648                 lea eax, [esi + 0x48]
 + 00bc1055  d95c2424               fstp dword ptr [esp + 0x24]
 + 00bc1059  50                     push eax
 + 00bc105a  d944242c               fld dword ptr [esp + 0x2c]
   00bc1099  6a04                   push 4
   00bc109b  d8ca                   fmul st(2)
@@ retail 798-799 built 804-805
   00bc1095  d944243c               fld dword ptr [esp + 0x3c]
   00bc1099  6a04                   push 4
 - 00bc109b  d8ca                   fmul st(2)
 + 00bc1060  d8c9                   fmul st(1)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
   00bc10a1  d8c9                   fmul st(1)
@@ retail 800-800 built 806-807
   00bc109b  d8ca                   fmul st(2)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
 + 00bc1066  d9442434               fld dword ptr [esp + 0x34]
   00bc10a1  d8c9                   fmul st(1)
   00bc10a3  d95c2434               fstp dword ptr [esp + 0x34]
@@ retail 806-807 built 813-814
   00bc10ae  d94674                 fld dword ptr [esi + 0x74]
   00bc10b1  d80d10002301           fmul dword ptr [0x1230010]
 - 00bc10b7  d9542444               fst dword ptr [esp + 0x44]
 + 00bc1080  d9542434               fst dword ptr [esp + 0x34]
   00bc10bb  d8259cf52201           fsub dword ptr [0x122f59c]
   00bc10c1  d95c241c               fstp dword ptr [esp + 0x1c]
@@ retail 810-813 built 817-820
   00bc10c1  d95c241c               fstp dword ptr [esp + 0x1c]
   00bc10c5  d944241c               fld dword ptr [esp + 0x1c]
 - 00bc10c9  db5c2448               fistp dword ptr [esp + 0x48]
 - 00bc10cd  db442448               fild dword ptr [esp + 0x48]
 - 00bc10d1  8b442448               mov eax, dword ptr [esp + 0x48]
 + 00bc1092  db5c2438               fistp dword ptr [esp + 0x38]
 + 00bc1096  db442438               fild dword ptr [esp + 0x38]
 + 00bc109a  8b442438               mov eax, dword ptr [esp + 0x38]
   00bc10d5  0500ffffff             add eax, 0xffffff00
   00bc10da  25ff030000             and eax, 0x3ff
@@ retail 815-816 built 822-823
   00bc10d5  0500ffffff             add eax, 0xffffff00
   00bc10da  25ff030000             and eax, 0x3ff
 - 00bc10df  d86c2444               fsubr dword ptr [esp + 0x44]
 + 00bc10a8  d86c2434               fsubr dword ptr [esp + 0x34]
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
@@ retail 818-819 built 825-826
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc10b4  d80c8500000000         fmul dword ptr [eax*4]
   00bc10f2  d9c9                   fxch st(1)
   00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 820-821 built 827-828
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc10bd  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 825-826 built 832-833
   00bc1101  d94674                 fld dword ptr [esi + 0x74]
   00bc1104  d80d10002301           fmul dword ptr [0x1230010]
 - 00bc110a  d9542444               fst dword ptr [esp + 0x44]
 + 00bc10d3  d9542434               fst dword ptr [esp + 0x34]
   00bc110e  d8259cf52201           fsub dword ptr [0x122f59c]
   00bc1114  d95c241c               fstp dword ptr [esp + 0x1c]
@@ retail 829-835 built 836-842
   00bc1114  d95c241c               fstp dword ptr [esp + 0x1c]
   00bc1118  d944241c               fld dword ptr [esp + 0x1c]
 - 00bc111c  db5c2448               fistp dword ptr [esp + 0x48]
 - 00bc1120  db442448               fild dword ptr [esp + 0x48]
 - 00bc1124  8b442448               mov eax, dword ptr [esp + 0x48]
 - 00bc1128  8b5664                 mov edx, dword ptr [esi + 0x64]
 - 00bc112b  8b4e60                 mov ecx, dword ptr [esi + 0x60]
 - 00bc112e  d86c2444               fsubr dword ptr [esp + 0x44]
 + 00bc10e5  db5c2438               fistp dword ptr [esp + 0x38]
 + 00bc10e9  db442438               fild dword ptr [esp + 0x38]
 + 00bc10ed  8b442438               mov eax, dword ptr [esp + 0x38]
 + 00bc10f1  8b4e64                 mov ecx, dword ptr [esi + 0x64]
 + 00bc10f4  8b5660                 mov edx, dword ptr [esi + 0x60]
 + 00bc10f7  d86c2434               fsubr dword ptr [esp + 0x34]
   00bc1132  25ff030000             and eax, 0x3ff
   00bc1137  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 837-839 built 844-846
   00bc1132  25ff030000             and eax, 0x3ff
   00bc1137  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc113d  52                     push edx
 - 00bc113e  8b54241c               mov edx, dword ptr [esp + 0x1c]
 + 00bc1106  51                     push ecx
 + 00bc1107  8b4c241c               mov ecx, dword ptr [esp + 0x1c]
   00bc1142  d8e1                   fsub st(1)
   00bc1144  51                     push ecx
@@ retail 840-841 built 847-847
   00bc113e  8b54241c               mov edx, dword ptr [esp + 0x1c]
   00bc1142  d8e1                   fsub st(1)
 - 00bc1144  51                     push ecx
   00bc1145  52                     push edx
   00bc1146  51                     push ecx
@@ retail 843-843 built 849-850
   00bc1145  52                     push edx
   00bc1146  51                     push ecx
 + 00bc110f  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
@@ retail 844-845 built 851-852
   00bc1146  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc1116  d80c8500000000         fmul dword ptr [eax*4]
   00bc1154  d9c9                   fxch st(1)
   00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 846-847 built 853-854
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc111f  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 851-853 built 858-862
   00bc1162  6a05                   push 5
   00bc1164  e89789dcff             call 0x989b00
 - 00bc1169  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc116d  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 + 00bc1132  8b54242c               mov edx, dword ptr [esp + 0x2c]
 + 00bc1136  8b442428               mov eax, dword ptr [esp + 0x28]
 + 00bc113a  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 + 00bc113e  52                     push edx
   00bc1171  8b542424               mov edx, dword ptr [esp + 0x24]
   00bc1175  50                     push eax
@@ retail 855-856 built 864-864
   00bc1171  8b542424               mov edx, dword ptr [esp + 0x24]
   00bc1175  50                     push eax
 - 00bc1176  8b442424               mov eax, dword ptr [esp + 0x24]
   00bc117a  51                     push ecx
   00bc117b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 859-860 built 867-867
   00bc117b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc1181  52                     push edx
 - 00bc1182  50                     push eax
   00bc1183  6a06                   push 6
   00bc1185  e87689dcff             call 0x989b00
@@ retail 864-865 built 871-871
   00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
 - 00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
   00bc1196  6a00                   push 0
   00bc1198  db442448               fild dword ptr [esp + 0x48]
@@ retail 866-867 built 872-873
   00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
   00bc1196  6a00                   push 0
 - 00bc1198  db442448               fild dword ptr [esp + 0x48]
 + 00bc115d  894c2438               mov dword ptr [esp + 0x38], ecx
   00bc119c  6a00                   push 0
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
@@ retail 868-868 built 874-875
   00bc1198  db442448               fild dword ptr [esp + 0x48]
   00bc119c  6a00                   push 0
 + 00bc1163  db44243c               fild dword ptr [esp + 0x3c]
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
@@ retail 872-873 built 879-880
   00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc11ae  83ec08                 sub esp, 8
 - 00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
 + 00bc117a  89542444               mov dword ptr [esp + 0x44], edx
   00bc11b5  d95c2404               fstp dword ptr [esp + 4]
   00bc11b9  db442454               fild dword ptr [esp + 0x54]
@@ retail 874-875 built 881-882
   00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
   00bc11b5  d95c2404               fstp dword ptr [esp + 4]
 - 00bc11b9  db442454               fild dword ptr [esp + 0x54]
 + 00bc1182  db442444               fild dword ptr [esp + 0x44]
   00bc11bd  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11c3  d91c24                 fstp dword ptr [esp]
@@ retail 944-980 built 951-954
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
 + 00bc125f  33db                   xor ebx, ebx
 + 00bc1261  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc1267  e800000000             call 0xbc126c
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
@@ retail 983-984 built 957-958
   00bc133c  e8ff44e4ff             call 0xa05840
   00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 - 00bc1345  3bf7                   cmp esi, edi
 + 00bc127b  3bf3                   cmp esi, ebx
   00bc1347  7444                   je 0xbc138d
   00bc1349  ff0e                   dec dword ptr [esi]
@@ retail 986-987 built 960-961
   00bc1347  7444                   je 0xbc138d
   00bc1349  ff0e                   dec dword ptr [esi]
 - 00bc134b  393e                   cmp dword ptr [esi], edi
 + 00bc1281  391e                   cmp dword ptr [esi], ebx
   00bc134d  753e                   jne 0xbc138d
   00bc134f  f6461a10               test byte ptr [esi + 0x1a], 0x10
@@ retail 993-994 built 967-968
   00bc1357  e844b8e3ff             call 0x9fcba0
   00bc135c  8b460c                 mov eax, dword ptr [esi + 0xc]
 - 00bc135f  3bc7                   cmp eax, edi
 + 00bc1295  3bc3                   cmp eax, ebx
   00bc1361  741b                   je 0xbc137e
   00bc1363  ff08                   dec dword ptr [eax]
@@ retail 997-998 built 971-972
   00bc1363  ff08                   dec dword ptr [eax]
   00bc1365  8b460c                 mov eax, dword ptr [esi + 0xc]
 - 00bc1368  3938                   cmp dword ptr [eax], edi
 + 00bc129e  3918                   cmp dword ptr [eax], ebx
   00bc136a  7512                   jne 0xbc137e
   00bc136c  8b4808                 mov ecx, dword ptr [eax + 8]
@@ retail 1005-1006 built 979-979
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
@@ retail 1007-1008 built 980-982
   00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
 - 00bc1382  897e08                 mov dword ptr [esi + 8], edi
 + 00bc12b5  895e08                 mov dword ptr [esi + 8], ebx
 + 00bc12b8  895e0c                 mov dword ptr [esi + 0xc], ebx
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
@@ retail 1014-1015 built 988-989
   00bc138f  5f                     pop edi
   00bc1390  5b                     pop ebx
 - 00bc1391  83c430                 add esp, 0x30
 + 00bc12c7  83c420                 add esp, 0x20
   00bc1394  c20800                 ret 8
DIFFLINES 518 retail_instr 1016 built_instr 990
