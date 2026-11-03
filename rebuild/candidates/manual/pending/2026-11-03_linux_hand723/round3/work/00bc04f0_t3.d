00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(3508v3751) flags= []
@@ retail 0-0 built 0-2
 + 00bc04f0  55                     push ebp
 + 00bc04f1  8bec                   mov ebp, esp
   00bc04f0  83ec30                 sub esp, 0x30
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
@@ retail 1-1 built 3-9
   00bc04f0  83ec30                 sub esp, 0x30
 + 00bc04f6  833d0000000000         cmp dword ptr [0], 0
 + 00bc04fd  894dfc                 mov dword ptr [ebp - 4], ecx
 + 00bc0500  0f849a0d0000           je 0xbc12a0
 + 00bc0506  8b4508                 mov eax, dword ptr [ebp + 8]
 + 00bc0509  53                     push ebx
 + 00bc050a  8b580c                 mov ebx, dword ptr [eax + 0xc]
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
   00bc04f8  53                     push ebx
@@ retail 2-17 built 10-13
   00bc04f0  83ec30                 sub esp, 0x30
   00bc04f3  a1e4b63901             mov eax, dword ptr [0x139b6e4]
 - 00bc04f8  53                     push ebx
 - 00bc04f9  57                     push edi
 - 00bc04fa  33ff                   xor edi, edi
 - 00bc04fc  3bc7                   cmp eax, edi
 - 00bc04fe  8bd9                   mov ebx, ecx
 - 00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
 - 00bc0504  0f84850e0000           je 0xbc138f
 - 00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc050e  8b0d986e4301           mov ecx, dword ptr [0x1436e98]
 - 00bc0514  8b8954090000           mov ecx, dword ptr [ecx + 0x954]
 - 00bc051a  55                     push ebp
 - 00bc051b  8b680c                 mov ebp, dword ptr [eax + 0xc]
 - 00bc051e  8b5554                 mov edx, dword ptr [ebp + 0x54]
 - 00bc0521  52                     push edx
 - 00bc0522  896c2418               mov dword ptr [esp + 0x18], ebp
 + 00bc0512  ff7354                 push dword ptr [ebx + 0x54]
 + 00bc0515  8b8854090000           mov ecx, dword ptr [eax + 0x954]
 + 00bc051b  895df8                 mov dword ptr [ebp - 8], ebx
   00bc0526  e865bce0ff             call 0x9cc190
   00bc052b  84c0                   test al, al
@@ retail 21-22 built 17-17
   00bc052d  0f845b0e0000           je 0xbc138e
   00bc0533  a1986e4301             mov eax, dword ptr [0x1436e98]
 - 00bc0538  8b4d54                 mov ecx, dword ptr [ebp + 0x54]
   00bc053b  56                     push esi
   00bc053c  8bb054090000           mov esi, dword ptr [eax + 0x954]
@@ retail 24-33 built 19-27
   00bc053b  56                     push esi
   00bc053c  8bb054090000           mov esi, dword ptr [eax + 0x954]
 - 00bc0542  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
 - 00bc0548  8b96e4010000           mov edx, dword ptr [esi + 0x1e4]
 - 00bc054e  2bd0                   sub edx, eax
 - 00bc0550  b8e9a28b2e             mov eax, 0x2e8ba2e9
 - 00bc0555  f7ea                   imul edx
 - 00bc0557  c1fa03                 sar edx, 3
 - 00bc055a  8bc2                   mov eax, edx
 - 00bc055c  c1e81f                 shr eax, 0x1f
 - 00bc055f  03c2                   add eax, edx
 + 00bc0537  8b86e4010000           mov eax, dword ptr [esi + 0x1e4]
 + 00bc053d  2b86e0010000           sub eax, dword ptr [esi + 0x1e0]
 + 00bc0543  57                     push edi
 + 00bc0544  6a2c                   push 0x2c
 + 00bc0546  99                     cdq 
 + 00bc0547  5f                     pop edi
 + 00bc0548  f7ff                   idiv edi
 + 00bc054a  8b4b54                 mov ecx, dword ptr [ebx + 0x54]
   00bc0561  3bc8                   cmp ecx, eax
   00bc0563  730d                   jae 0xbc0572
@@ retail 35-36 built 29-29
   00bc0561  3bc8                   cmp ecx, eax
   00bc0563  730d                   jae 0xbc0572
 - 00bc0565  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc056b  6bc92c                 imul ecx, ecx, 0x2c
   00bc056e  03c8                   add ecx, eax
@@ retail 37-38 built 30-33
   00bc0565  8b86e0010000           mov eax, dword ptr [esi + 0x1e0]
   00bc056b  6bc92c                 imul ecx, ecx, 0x2c
 - 00bc056e  03c8                   add ecx, eax
 + 00bc0554  038ee0010000           add ecx, dword ptr [esi + 0x1e0]
 + 00bc055a  33ff                   xor edi, edi
 + 00bc055c  894df0                 mov dword ptr [ebp - 0x10], ecx
   00bc0570  eb02                   jmp 0xbc0574
   00bc0572  33c9                   xor ecx, ecx
@@ retail 39-40 built 34-37
   00bc056e  03c8                   add ecx, eax
   00bc0570  eb02                   jmp 0xbc0574
 - 00bc0572  33c9                   xor ecx, ecx
 + 00bc0561  33ff                   xor edi, edi
 + 00bc0563  897df0                 mov dword ptr [ebp - 0x10], edi
 + 00bc0566  8b4df0                 mov ecx, dword ptr [ebp - 0x10]
   00bc0574  57                     push edi
   00bc0575  57                     push edi
@@ retail 43-47 built 40-43
   00bc0575  57                     push edi
   00bc0576  57                     push edi
 - 00bc0577  8d54241c               lea edx, [esp + 0x1c]
 - 00bc057b  52                     push edx
 - 00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00bc0580  897c2420               mov dword ptr [esp + 0x20], edi
 + 00bc056c  8d4508                 lea eax, [ebp + 8]
 + 00bc056f  50                     push eax
 + 00bc0570  897d08                 mov dword ptr [ebp + 8], edi
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
@@ retail 51-52 built 47-47
   00bc058b  0f84b00d0000           je 0xbc1341
   00bc0591  a108b53c01             mov eax, dword ptr [0x13cb508]
 - 00bc0596  8bb008280000           mov esi, dword ptr [eax + 0x2808]
   00bc059c  8d8808280000           lea ecx, [eax + 0x2808]
   00bc05a2  8d90243a0000           lea edx, [eax + 0x3a24]
@@ retail 53-53 built 48-49
   00bc0596  8bb008280000           mov esi, dword ptr [eax + 0x2808]
   00bc059c  8d8808280000           lea ecx, [eax + 0x2808]
 + 00bc058b  8b31                   mov esi, dword ptr [ecx]
   00bc05a2  8d90243a0000           lea edx, [eax + 0x3a24]
   00bc05a8  8954f008               mov dword ptr [eax + esi*8 + 8], edx
@@ retail 55-62 built 51-54
   00bc05a2  8d90243a0000           lea edx, [eax + 0x3a24]
   00bc05a8  8954f008               mov dword ptr [eax + esi*8 + 8], edx
 - 00bc05ac  8bb014280000           mov esi, dword ptr [eax + 0x2814]
 - 00bc05b2  8b11                   mov edx, dword ptr [ecx]
 - 00bc05b4  d1e6                   shl esi, 1
 - 00bc05b6  42                     inc edx
 - 00bc05b7  89b014280000           mov dword ptr [eax + 0x2814], esi
 - 00bc05bd  8911                   mov dword ptr [ecx], edx
 - 00bc05bf  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc0597  ff01                   inc dword ptr [ecx]
 + 00bc0599  d1a014280000           shl dword ptr [eax + 0x2814], 1
 + 00bc059f  8b4508                 mov eax, dword ptr [ebp + 8]
   00bc05c3  8b4808                 mov ecx, dword ptr [eax + 8]
   00bc05c6  3bcf                   cmp ecx, edi
@@ retail 70-71 built 62-64
   00bc05d3  eb02                   jmp 0xbc05d7
   00bc05d5  33c9                   xor ecx, ecx
 - 00bc05d7  3988003d0000           cmp dword ptr [eax + 0x3d00], ecx
 + 00bc05b6  8d90003d0000           lea edx, [eax + 0x3d00]
 + 00bc05bc  390a                   cmp dword ptr [edx], ecx
   00bc05dd  741c                   je 0xbc05fb
   00bc05df  51                     push ecx
@@ retail 73-74 built 66-67
   00bc05dd  741c                   je 0xbc05fb
   00bc05df  51                     push ecx
 - 00bc05e0  8988003d0000           mov dword ptr [eax + 0x3d00], ecx
 + 00bc05c1  890a                   mov dword ptr [edx], ecx
   00bc05e6  8b86f03c0000           mov eax, dword ptr [esi + 0x3cf0]
   00bc05ec  8b10                   mov edx, dword ptr [eax]
@@ retail 80-83 built 73-77
   00bc05f0  ff9204010000           call dword ptr [edx + 0x104]
   00bc05f6  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc05fb  8b8eb83c0000           mov ecx, dword ptr [esi + 0x3cb8]
 - 00bc0601  bf01000000             mov edi, 1
 - 00bc0606  3bcf                   cmp ecx, edi
 + 00bc05d8  33ff                   xor edi, edi
 + 00bc05da  8d8eb83c0000           lea ecx, [esi + 0x3cb8]
 + 00bc05e0  47                     inc edi
 + 00bc05e1  3939                   cmp dword ptr [ecx], edi
   00bc0608  7d0b                   jge 0xbc0615
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
@@ retail 84-85 built 78-79
   00bc0606  3bcf                   cmp ecx, edi
   00bc0608  7d0b                   jge 0xbc0615
 - 00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
 + 00bc05e5  8939                   mov dword ptr [ecx], edi
   00bc0610  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0615  8b4c2448               mov ecx, dword ptr [esp + 0x48]
@@ retail 86-88 built 80-81
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
   00bc0610  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0615  8b4c2448               mov ecx, dword ptr [esp + 0x48]
 - 00bc0619  83f906                 cmp ecx, 6
 + 00bc05ec  837d0c06               cmp dword ptr [ebp + 0xc], 6
   00bc061c  0f85a1010000           jne 0xbc07c3
   00bc0622  8b0d386e4301           mov ecx, dword ptr [0x1436e38]
@@ retail 90-92 built 83-84
   00bc061c  0f85a1010000           jne 0xbc07c3
   00bc0622  8b0d386e4301           mov ecx, dword ptr [0x1436e38]
 - 00bc0628  8a5108                 mov dl, byte ptr [ecx + 8]
 - 00bc062b  84d2                   test dl, dl
 + 00bc05fc  80790800               cmp byte ptr [ecx + 8], 0
   00bc062d  0f8472030000           je 0xbc09a5
   00bc0633  8b35d06e3901           mov esi, dword ptr [0x1396ed0]
@@ retail 94-95 built 86-86
   00bc062d  0f8472030000           je 0xbc09a5
   00bc0633  8b35d06e3901           mov esi, dword ptr [0x1396ed0]
 - 00bc0639  39b0402c0000           cmp dword ptr [eax + 0x2c40], esi
   00bc063f  8d883c2c0000           lea ecx, [eax + 0x2c3c]
   00bc0645  c644244400             mov byte ptr [esp + 0x44], 0
@@ retail 96-97 built 87-89
   00bc0639  39b0402c0000           cmp dword ptr [eax + 0x2c40], esi
   00bc063f  8d883c2c0000           lea ecx, [eax + 0x2c3c]
 - 00bc0645  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc0612  397104                 cmp dword ptr [ecx + 4], esi
 + 00bc0615  c6450f00               mov byte ptr [ebp + 0xf], 0
   00bc064a  8bd0                   mov edx, eax
   00bc064c  8bfe                   mov edi, esi
@@ retail 106-114 built 98-107
   00bc065d  0bf0                   or esi, eax
   00bc065f  897108                 mov dword ptr [ecx + 8], esi
 - 00bc0662  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0668  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 - 00bc066c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0672  8b7104                 mov esi, dword ptr [ecx + 4]
 - 00bc0675  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 - 00bc0679  ff8208280000           inc dword ptr [edx + 0x2808]
 - 00bc067f  8a4110                 mov al, byte ptr [ecx + 0x10]
 - 00bc0682  84c0                   test al, al
 + 00bc0631  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc0637  8b30                   mov esi, dword ptr [eax]
 + 00bc0639  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc063d  8b5904                 mov ebx, dword ptr [ecx + 4]
 + 00bc0640  8b30                   mov esi, dword ptr [eax]
 + 00bc0642  895cf20c               mov dword ptr [edx + esi*8 + 0xc], ebx
 + 00bc0646  ff00                   inc dword ptr [eax]
 + 00bc0648  8b5df8                 mov ebx, dword ptr [ebp - 8]
 + 00bc064b  80791000               cmp byte ptr [ecx + 0x10], 0
   00bc0684  897904                 mov dword ptr [ecx + 4], edi
   00bc0687  7517                   jne 0xbc06a0
@@ retail 117-120 built 110-114
   00bc0687  7517                   jne 0xbc06a0
   00bc0689  c6411001               mov byte ptr [ecx + 0x10], 1
 - 00bc068d  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 - 00bc0693  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 - 00bc069a  ff820c280000           inc dword ptr [edx + 0x280c]
 + 00bc0658  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc065e  8b30                   mov esi, dword ptr [eax]
 + 00bc0660  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc0667  ff00                   inc dword ptr [eax]
   00bc06a0  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc06a5  8b35786f3901           mov esi, dword ptr [0x1396f78]
@@ retail 121-290 built 115-118
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
 - 00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
 - 00bc071e  8d9030290000           lea edx, [eax + 0x2930]
 - 00bc0724  8bc8                   mov ecx, eax
 - 00bc0726  8bfe                   mov edi, esi
 - 00bc0728  7455                   je 0xbc077f
 - 00bc072a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0730  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0733  85f0                   test eax, esi
 - 00bc0735  7522                   jne 0xbc0759
 - 00bc0737  0bf0                   or esi, eax
 - 00bc0739  897208                 mov dword ptr [edx + 8], esi
 - 00bc073c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0742  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc0746  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc074c  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc074f  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc0753  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc0759  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc075c  84c0                   test al, al
 - 00bc075e  897a04                 mov dword ptr [edx + 4], edi
 - 00bc0761  7517                   jne 0xbc077a
 - 00bc0763  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc0767  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc076d  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0774  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc077a  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc077f  8a8d84000000           mov cl, byte ptr [ebp + 0x84]
 - 00bc0785  f6c104                 test cl, 4
 - 00bc0788  0f8499010000           je 0xbc0927
 - 00bc078e  8d4b40                 lea ecx, [ebx + 0x40]
 - 00bc0791  6a00                   push 0
 - 00bc0793  51                     push ecx
 - 00bc0794  eb06                   jmp 0xbc079c
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
 - 00bc07c3  83f907                 cmp ecx, 7
 - 00bc07c6  0f85d9010000           jne 0xbc09a5
 - 00bc07cc  8bb0d0280000           mov esi, dword ptr [eax + 0x28d0]
 - 00bc07d2  3bf7                   cmp esi, edi
 - 00bc07d4  8d90cc280000           lea edx, [eax + 0x28cc]
 - 00bc07da  c644244400             mov byte ptr [esp + 0x44], 0
 - 00bc07df  8bc8                   mov ecx, eax
 - 00bc07e1  7455                   je 0xbc0838
 - 00bc07e3  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc07e9  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc07ec  85f0                   test eax, esi
 - 00bc07ee  7522                   jne 0xbc0812
 - 00bc07f0  0bf0                   or esi, eax
 - 00bc07f2  897208                 mov dword ptr [edx + 8], esi
 - 00bc07f5  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc07fb  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 - 00bc07ff  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 - 00bc0805  8b7204                 mov esi, dword ptr [edx + 4]
 - 00bc0808  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 - 00bc080c  ff8108280000           inc dword ptr [ecx + 0x2808]
 - 00bc0812  8a4210                 mov al, byte ptr [edx + 0x10]
 - 00bc0815  84c0                   test al, al
 - 00bc0817  897a04                 mov dword ptr [edx + 4], edi
 - 00bc081a  7517                   jne 0xbc0833
 - 00bc081c  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc0820  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0826  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc082d  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0833  a1186e4301             mov eax, dword ptr [0x1436e18]
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
 - 00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
 - 00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
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
 + 00bc066e  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc0674  8d881c290000           lea ecx, [eax + 0x291c]
 + 00bc067a  397104                 cmp dword ptr [ecx + 4], esi
   00bc0944  8bd0                   mov edx, eax
   00bc0946  8bfe                   mov edi, esi
@@ retail 291-393 built 119-120
   00bc093e  8d8890280000           lea ecx, [eax + 0x2890]
   00bc0944  8bd0                   mov edx, eax
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
 + 00bc067f  8975f4                 mov dword ptr [ebp - 0xc], esi
   00bc0ac7  7450                   je 0xbc0b19
   00bc0ac9  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 400-409 built 127-137
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
 + 00bc0696  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc069c  8b30                   mov esi, dword ptr [eax]
 + 00bc069e  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc06a2  8b30                   mov esi, dword ptr [eax]
 + 00bc06a4  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc06a7  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc06ab  ff00                   inc dword ptr [eax]
 + 00bc06ad  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc06b1  8b45f4                 mov eax, dword ptr [ebp - 0xc]
 + 00bc06b4  894104                 mov dword ptr [ecx + 4], eax
   00bc0b00  7517                   jne 0xbc0b19
   00bc0b02  c6411001               mov byte ptr [ecx + 0x10], 1
@@ retail 411-419 built 139-149
   00bc0b00  7517                   jne 0xbc0b19
   00bc0b02  c6411001               mov byte ptr [ecx + 0x10], 1
 - 00bc0b06  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 - 00bc0b0c  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 - 00bc0b13  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc0b19  c644244400             mov byte ptr [esp + 0x44], 0
 - 00bc0b1e  e96c010000             jmp 0xbc0c8f
 - 00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 - 00bc0b29  3bde                   cmp ebx, esi
 - 00bc0b2b  8bfe                   mov edi, esi
 + 00bc06bd  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc06c3  8b30                   mov esi, dword ptr [eax]
 + 00bc06c5  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc06cc  ff00                   inc dword ptr [eax]
 + 00bc06ce  a100000000             mov eax, dword ptr [0]
 + 00bc06d3  8b3514000000           mov esi, dword ptr [0x14]
 + 00bc06d9  8d8830290000           lea ecx, [eax + 0x2930]
 + 00bc06df  397104                 cmp dword ptr [ecx + 4], esi
 + 00bc06e2  8bd0                   mov edx, eax
 + 00bc06e4  8975f4                 mov dword ptr [ebp - 0xc], esi
   00bc0b2d  7455                   je 0xbc0b84
   00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 421-422 built 151-152
   00bc0b2d  7455                   je 0xbc0b84
   00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0b35  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc06ef  8b7108                 mov esi, dword ptr [ecx + 8]
   00bc0b38  85f0                   test eax, esi
   00bc0b3a  7522                   jne 0xbc0b5e
@@ retail 425-435 built 155-166
   00bc0b3a  7522                   jne 0xbc0b5e
   00bc0b3c  0bf0                   or esi, eax
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
 + 00bc06f8  897108                 mov dword ptr [ecx + 8], esi
 + 00bc06fb  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc0701  8b30                   mov esi, dword ptr [eax]
 + 00bc0703  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc0707  8b30                   mov esi, dword ptr [eax]
 + 00bc0709  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc070c  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc0710  ff00                   inc dword ptr [eax]
 + 00bc0712  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc0716  8b45f4                 mov eax, dword ptr [ebp - 0xc]
 + 00bc0719  894104                 mov dword ptr [ecx + 4], eax
   00bc0b66  7517                   jne 0xbc0b7f
   00bc0b68  c6421001               mov byte ptr [edx + 0x10], 1
@@ retail 436-440 built 167-172
   00bc0b63  897a04                 mov dword ptr [edx + 4], edi
   00bc0b66  7517                   jne 0xbc0b7f
 - 00bc0b68  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc0b6c  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0b72  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0b79  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc071e  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0722  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc0728  8b30                   mov esi, dword ptr [eax]
 + 00bc072a  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc0731  ff00                   inc dword ptr [eax]
   00bc0b7f  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0b84  8b35746f3901           mov esi, dword ptr [0x1396f74]
@@ retail 441-445 built 173-181
   00bc0b79  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0b7f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0b84  8b35746f3901           mov esi, dword ptr [0x1396f74]
 - 00bc0b8a  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 - 00bc0b90  3bde                   cmp ebx, esi
 - 00bc0b92  8d8830290000           lea ecx, [eax + 0x2930]
 + 00bc0738  8a8b84000000           mov cl, byte ptr [ebx + 0x84]
 + 00bc073e  f6c104                 test cl, 4
 + 00bc0741  0f859d010000           jne 0xbc08e4
 + 00bc0747  f6c110                 test cl, 0x10
 + 00bc074a  7435                   je 0xbc0781
 + 00bc074c  8b3508000000           mov esi, dword ptr [8]
 + 00bc0752  8d8890280000           lea ecx, [eax + 0x2890]
 + 00bc0758  397104                 cmp dword ptr [ecx + 4], esi
   00bc0b98  8bd0                   mov edx, eax
   00bc0b9a  8bfe                   mov edi, esi
@@ retail 446-447 built 182-183
   00bc0b92  8d8830290000           lea ecx, [eax + 0x2930]
   00bc0b98  8bd0                   mov edx, eax
 - 00bc0b9a  8bfe                   mov edi, esi
 + 00bc075d  8975f4                 mov dword ptr [ebp - 0xc], esi
   00bc0b9c  0f84e8000000           je 0xbc0c8a
   00bc0ba2  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 455-459 built 191-209
   00bc0bb5  897108                 mov dword ptr [ecx + 8], esi
   00bc0bb8  e98f000000             jmp 0xbc0c4c
 - 00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 - 00bc0bc3  3bde                   cmp ebx, esi
 - 00bc0bc5  8bc8                   mov ecx, eax
 - 00bc0bc7  8bfe                   mov edi, esi
 + 00bc0781  8b75fc                 mov esi, dword ptr [ebp - 4]
 + 00bc0784  8d4630                 lea eax, [esi + 0x30]
 + 00bc0787  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc078d  6a00                   push 0
 + 00bc078f  50                     push eax
 + 00bc0790  e800000000             call 0xbc0795
 + 00bc0795  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc079b  83c658                 add esi, 0x58
 + 00bc079e  56                     push esi
 + 00bc079f  e800000000             call 0xbc07a4
 + 00bc07a4  8b75fc                 mov esi, dword ptr [ebp - 4]
 + 00bc07a7  e971060000             jmp 0xbc0e1d
 + 00bc07ac  837d0c07               cmp dword ptr [ebp + 0xc], 7
 + 00bc07b0  0f85ad010000           jne 0xbc0963
 + 00bc07b6  8d88cc280000           lea ecx, [eax + 0x28cc]
 + 00bc07bc  397904                 cmp dword ptr [ecx + 4], edi
 + 00bc07bf  c6450f00               mov byte ptr [ebp + 0xf], 0
 + 00bc07c3  8bd0                   mov edx, eax
   00bc0bc9  745b                   je 0xbc0c26
   00bc0bcb  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 461-462 built 211-240
   00bc0bc9  745b                   je 0xbc0c26
   00bc0bcb  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0bd1  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc07cd  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc07d0  85f0                   test eax, esi
 + 00bc07d2  751f                   jne 0xbc07f3
 + 00bc07d4  0bf0                   or esi, eax
 + 00bc07d6  897108                 mov dword ptr [ecx + 8], esi
 + 00bc07d9  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc07df  8b30                   mov esi, dword ptr [eax]
 + 00bc07e1  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc07e5  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc07e8  8b30                   mov esi, dword ptr [eax]
 + 00bc07ea  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc07ee  ff00                   inc dword ptr [eax]
 + 00bc07f0  33ff                   xor edi, edi
 + 00bc07f2  47                     inc edi
 + 00bc07f3  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc07f7  897904                 mov dword ptr [ecx + 4], edi
 + 00bc07fa  7515                   jne 0xbc0811
 + 00bc07fc  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0800  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc0806  8b30                   mov esi, dword ptr [eax]
 + 00bc0808  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc080f  ff00                   inc dword ptr [eax]
 + 00bc0811  a100000000             mov eax, dword ptr [0]
 + 00bc0816  8d88e0280000           lea ecx, [eax + 0x28e0]
 + 00bc081c  83790410               cmp dword ptr [ecx + 4], 0x10
 + 00bc0820  8bd0                   mov edx, eax
 + 00bc0822  7450                   je 0xbc0874
 + 00bc0824  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc082a  8b7108                 mov esi, dword ptr [ecx + 8]
   00bc0bd4  85c6                   test esi, eax
   00bc0bd6  7522                   jne 0xbc0bfa
@@ retail 465-475 built 243-253
   00bc0bd6  7522                   jne 0xbc0bfa
   00bc0bd8  0bc6                   or eax, esi
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
 + 00bc0833  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0836  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc083c  8b30                   mov esi, dword ptr [eax]
 + 00bc083e  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc0842  8b30                   mov esi, dword ptr [eax]
 + 00bc0844  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc0847  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc084b  ff00                   inc dword ptr [eax]
 + 00bc084d  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc0851  c7410410000000         mov dword ptr [ecx + 4], 0x10
   00bc0c02  7517                   jne 0xbc0c1b
   00bc0c04  c6421001               mov byte ptr [edx + 0x10], 1
@@ retail 476-480 built 254-259
   00bc0bff  897a04                 mov dword ptr [edx + 4], edi
   00bc0c02  7517                   jne 0xbc0c1b
 - 00bc0c04  c6421001               mov byte ptr [edx + 0x10], 1
 - 00bc0c08  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 - 00bc0c0e  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 - 00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc085a  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc085e  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc0864  8b30                   mov esi, dword ptr [eax]
 + 00bc0866  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc086d  ff00                   inc dword ptr [eax]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
@@ retail 481-481 built 260-332
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 + 00bc0874  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc087a  8d88a4280000           lea ecx, [eax + 0x28a4]
 + 00bc0880  397104                 cmp dword ptr [ecx + 4], esi
 + 00bc0883  8bd0                   mov edx, eax
 + 00bc0885  8975f4                 mov dword ptr [ebp - 0xc], esi
 + 00bc0888  744f                   je 0xbc08d9
 + 00bc088a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0890  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc0893  85c6                   test esi, eax
 + 00bc0895  751c                   jne 0xbc08b3
 + 00bc0897  0bc6                   or eax, esi
 + 00bc0899  894108                 mov dword ptr [ecx + 8], eax
 + 00bc089c  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc08a2  8b30                   mov esi, dword ptr [eax]
 + 00bc08a4  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc08a8  8b30                   mov esi, dword ptr [eax]
 + 00bc08aa  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc08ad  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc08b1  ff00                   inc dword ptr [eax]
 + 00bc08b3  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc08b7  8b45f4                 mov eax, dword ptr [ebp - 0xc]
 + 00bc08ba  894104                 mov dword ptr [ecx + 4], eax
 + 00bc08bd  7515                   jne 0xbc08d4
 + 00bc08bf  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc08c3  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc08c9  8b30                   mov esi, dword ptr [eax]
 + 00bc08cb  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc08d2  ff00                   inc dword ptr [eax]
 + 00bc08d4  a100000000             mov eax, dword ptr [0]
 + 00bc08d9  8a8b84000000           mov cl, byte ptr [ebx + 0x84]
 + 00bc08df  f6c104                 test cl, 4
 + 00bc08e2  740b                   je 0xbc08ef
 + 00bc08e4  8b75fc                 mov esi, dword ptr [ebp - 4]
 + 00bc08e7  8d4640                 lea eax, [esi + 0x40]
 + 00bc08ea  e998feffff             jmp 0xbc0787
 + 00bc08ef  f6c110                 test cl, 0x10
 + 00bc08f2  0f8489feffff           je 0xbc0781
 + 00bc08f8  8b3508000000           mov esi, dword ptr [8]
 + 00bc08fe  8d8890280000           lea ecx, [eax + 0x2890]
 + 00bc0904  397104                 cmp dword ptr [ecx + 4], esi
 + 00bc0907  8bd0                   mov edx, eax
 + 00bc0909  8975f4                 mov dword ptr [ebp - 0xc], esi
 + 00bc090c  744a                   je 0xbc0958
 + 00bc090e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0914  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc0917  85c6                   test esi, eax
 + 00bc0919  751c                   jne 0xbc0937
 + 00bc091b  0bc6                   or eax, esi
 + 00bc091d  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0920  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc0926  8b30                   mov esi, dword ptr [eax]
 + 00bc0928  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc092c  8b30                   mov esi, dword ptr [eax]
 + 00bc092e  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc0931  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc0935  ff00                   inc dword ptr [eax]
 + 00bc0937  8b45f4                 mov eax, dword ptr [ebp - 0xc]
 + 00bc093a  894104                 mov dword ptr [ecx + 4], eax
 + 00bc093d  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc0941  7515                   jne 0xbc0958
 + 00bc0943  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0947  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc094d  8b30                   mov esi, dword ptr [eax]
 + 00bc094f  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc0956  ff00                   inc dword ptr [eax]
 + 00bc0958  8b75fc                 mov esi, dword ptr [ebp - 4]
 + 00bc095b  8d4638                 lea eax, [esi + 0x38]
 + 00bc095e  e924feffff             jmp 0xbc0787
 + 00bc0963  8b4b7c                 mov ecx, dword ptr [ebx + 0x7c]
 + 00bc0966  49                     dec ecx
 + 00bc0967  8bd0                   mov edx, eax
 + 00bc0969  7417                   je 0xbc0982
   00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
@@ retail 482-484 built 333-401
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
   00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 - 00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
 - 00bc0c2c  3bde                   cmp ebx, esi
 + 00bc0971  49                     dec ecx
 + 00bc0972  8d883c2c0000           lea ecx, [eax + 0x2c3c]
 + 00bc0978  7514                   jne 0xbc098e
 + 00bc097a  8b3508000000           mov esi, dword ptr [8]
 + 00bc0980  eb0c                   jmp 0xbc098e
 + 00bc0982  8b3504000000           mov esi, dword ptr [4]
 + 00bc0988  8d883c2c0000           lea ecx, [eax + 0x2c3c]
 + 00bc098e  397104                 cmp dword ptr [ecx + 4], esi
 + 00bc0991  89750c                 mov dword ptr [ebp + 0xc], esi
 + 00bc0994  744f                   je 0xbc09e5
 + 00bc0996  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc099c  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc099f  85c6                   test esi, eax
 + 00bc09a1  751c                   jne 0xbc09bf
 + 00bc09a3  0bc6                   or eax, esi
 + 00bc09a5  894108                 mov dword ptr [ecx + 8], eax
 + 00bc09a8  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc09ae  8b30                   mov esi, dword ptr [eax]
 + 00bc09b0  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc09b4  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc09b7  8b30                   mov esi, dword ptr [eax]
 + 00bc09b9  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc09bd  ff00                   inc dword ptr [eax]
 + 00bc09bf  8b450c                 mov eax, dword ptr [ebp + 0xc]
 + 00bc09c2  894104                 mov dword ptr [ecx + 4], eax
 + 00bc09c5  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc09c9  7515                   jne 0xbc09e0
 + 00bc09cb  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc09cf  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc09d5  8b30                   mov esi, dword ptr [eax]
 + 00bc09d7  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc09de  ff00                   inc dword ptr [eax]
 + 00bc09e0  a100000000             mov eax, dword ptr [0]
 + 00bc09e5  8b4b78                 mov ecx, dword ptr [ebx + 0x78]
 + 00bc09e8  83e903                 sub ecx, 3
 + 00bc09eb  0f849c010000           je 0xbc0b8d
 + 00bc09f1  49                     dec ecx
 + 00bc09f2  8bd0                   mov edx, eax
 + 00bc09f4  8d881c290000           lea ecx, [eax + 0x291c]
 + 00bc09fa  0f84c6000000           je 0xbc0ac6
 + 00bc0a00  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc0a06  397104                 cmp dword ptr [ecx + 4], esi
 + 00bc0a09  89750c                 mov dword ptr [ebp + 0xc], esi
 + 00bc0a0c  744f                   je 0xbc0a5d
 + 00bc0a0e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0a14  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc0a17  85c6                   test esi, eax
 + 00bc0a19  751c                   jne 0xbc0a37
 + 00bc0a1b  0bc6                   or eax, esi
 + 00bc0a1d  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0a20  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc0a26  8b30                   mov esi, dword ptr [eax]
 + 00bc0a28  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc0a2c  8b30                   mov esi, dword ptr [eax]
 + 00bc0a2e  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc0a31  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc0a35  ff00                   inc dword ptr [eax]
 + 00bc0a37  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc0a3b  8b450c                 mov eax, dword ptr [ebp + 0xc]
 + 00bc0a3e  894104                 mov dword ptr [ecx + 4], eax
 + 00bc0a41  7515                   jne 0xbc0a58
 + 00bc0a43  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0a47  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc0a4d  8b30                   mov esi, dword ptr [eax]
 + 00bc0a4f  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc0a56  ff00                   inc dword ptr [eax]
 + 00bc0a58  a100000000             mov eax, dword ptr [0]
 + 00bc0a5d  8b3514000000           mov esi, dword ptr [0x14]
   00bc0c2e  8d8830290000           lea ecx, [eax + 0x2930]
   00bc0c34  8bd0                   mov edx, eax
@@ retail 485-485 built 402-460
   00bc0c2c  3bde                   cmp ebx, esi
   00bc0c2e  8d8830290000           lea ecx, [eax + 0x2930]
 + 00bc0a69  397104                 cmp dword ptr [ecx + 4], esi
 + 00bc0a6c  8bd0                   mov edx, eax
 + 00bc0a6e  89750c                 mov dword ptr [ebp + 0xc], esi
 + 00bc0a71  744a                   je 0xbc0abd
 + 00bc0a73  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0a79  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc0a7c  85c6                   test esi, eax
 + 00bc0a7e  751c                   jne 0xbc0a9c
 + 00bc0a80  0bc6                   or eax, esi
 + 00bc0a82  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0a85  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc0a8b  8b30                   mov esi, dword ptr [eax]
 + 00bc0a8d  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc0a91  8b30                   mov esi, dword ptr [eax]
 + 00bc0a93  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc0a96  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc0a9a  ff00                   inc dword ptr [eax]
 + 00bc0a9c  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc0aa0  8b450c                 mov eax, dword ptr [ebp + 0xc]
 + 00bc0aa3  894104                 mov dword ptr [ecx + 4], eax
 + 00bc0aa6  7515                   jne 0xbc0abd
 + 00bc0aa8  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0aac  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc0ab2  8b30                   mov esi, dword ptr [eax]
 + 00bc0ab4  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc0abb  ff00                   inc dword ptr [eax]
 + 00bc0abd  c6450f00               mov byte ptr [ebp + 0xf], 0
 + 00bc0ac1  e98e010000             jmp 0xbc0c54
 + 00bc0ac6  8b3504000000           mov esi, dword ptr [4]
 + 00bc0acc  397104                 cmp dword ptr [ecx + 4], esi
 + 00bc0acf  89750c                 mov dword ptr [ebp + 0xc], esi
 + 00bc0ad2  744f                   je 0xbc0b23
 + 00bc0ad4  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0ada  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc0add  85c6                   test esi, eax
 + 00bc0adf  751c                   jne 0xbc0afd
 + 00bc0ae1  0bc6                   or eax, esi
 + 00bc0ae3  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0ae6  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc0aec  8b30                   mov esi, dword ptr [eax]
 + 00bc0aee  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc0af2  8b30                   mov esi, dword ptr [eax]
 + 00bc0af4  8b7904                 mov edi, dword ptr [ecx + 4]
 + 00bc0af7  897cf20c               mov dword ptr [edx + esi*8 + 0xc], edi
 + 00bc0afb  ff00                   inc dword ptr [eax]
 + 00bc0afd  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc0b01  8b450c                 mov eax, dword ptr [ebp + 0xc]
 + 00bc0b04  894104                 mov dword ptr [ecx + 4], eax
 + 00bc0b07  7515                   jne 0xbc0b1e
 + 00bc0b09  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0b0d  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc0b13  8b30                   mov esi, dword ptr [eax]
 + 00bc0b15  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc0b1c  ff00                   inc dword ptr [eax]
 + 00bc0b1e  a100000000             mov eax, dword ptr [0]
 + 00bc0b23  8b350c000000           mov esi, dword ptr [0xc]
 + 00bc0b29  8d8830290000           lea ecx, [eax + 0x2930]
 + 00bc0b2f  397104                 cmp dword ptr [ecx + 4], esi
   00bc0c34  8bd0                   mov edx, eax
   00bc0c36  8bfe                   mov edi, esi
@@ retail 494-502 built 469-478
   00bc0c47  0bc6                   or eax, esi
   00bc0c49  894108                 mov dword ptr [ecx + 8], eax
 - 00bc0c4c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0c52  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 - 00bc0c56  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 - 00bc0c5c  8b7104                 mov esi, dword ptr [ecx + 4]
 - 00bc0c5f  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 - 00bc0c63  ff8208280000           inc dword ptr [edx + 0x2808]
 - 00bc0c69  8a4110                 mov al, byte ptr [ecx + 0x10]
 - 00bc0c6c  84c0                   test al, al
 + 00bc0b4e  8d8208280000           lea eax, [edx + 0x2808]
 + 00bc0b54  8b30                   mov esi, dword ptr [eax]
 + 00bc0b56  894cf208               mov dword ptr [edx + esi*8 + 8], ecx
 + 00bc0b5a  8b5904                 mov ebx, dword ptr [ecx + 4]
 + 00bc0b5d  8b30                   mov esi, dword ptr [eax]
 + 00bc0b5f  895cf20c               mov dword ptr [edx + esi*8 + 0xc], ebx
 + 00bc0b63  ff00                   inc dword ptr [eax]
 + 00bc0b65  8b5df8                 mov ebx, dword ptr [ebp - 8]
 + 00bc0b68  80791000               cmp byte ptr [ecx + 0x10], 0
   00bc0c6e  897904                 mov dword ptr [ecx + 4], edi
   00bc0c71  7517                   jne 0xbc0c8a
@@ retail 505-513 built 481-547
   00bc0c71  7517                   jne 0xbc0c8a
   00bc0c73  c6411001               mov byte ptr [ecx + 0x10], 1
 - 00bc0c77  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 - 00bc0c7d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 - 00bc0c84  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc0c8a  c644244401             mov byte ptr [esp + 0x44], 1
 - 00bc0c8f  8a8584000000           mov al, byte ptr [ebp + 0x84]
 - 00bc0c95  8b0d846e4301           mov ecx, dword ptr [0x1436e84]
 - 00bc0c9b  8b5970                 mov ebx, dword ptr [ecx + 0x70]
 - 00bc0c9e  33f6                   xor esi, esi
 + 00bc0b79  8d820c280000           lea eax, [edx + 0x280c]
 + 00bc0b7f  8b30                   mov esi, dword ptr [eax]
 + 00bc0b81  898cb208200000         mov dword ptr [edx + esi*4 + 0x2008], ecx
 + 00bc0b88  e9c1000000             jmp 0xbc0c4e
 + 00bc0b8d  8b1504000000           mov edx, dword ptr [4]
 + 00bc0b93  8d881c290000           lea ecx, [eax + 0x291c]
 + 00bc0b99  395104                 cmp dword ptr [ecx + 4], edx
 + 00bc0b9c  8bf0                   mov esi, eax
 + 00bc0b9e  8bfa                   mov edi, edx
 + 00bc0ba0  7455                   je 0xbc0bf7
 + 00bc0ba2  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0ba8  8b5108                 mov edx, dword ptr [ecx + 8]
 + 00bc0bab  85c2                   test edx, eax
 + 00bc0bad  751f                   jne 0xbc0bce
 + 00bc0baf  0bc2                   or eax, edx
 + 00bc0bb1  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0bb4  8d8608280000           lea eax, [esi + 0x2808]
 + 00bc0bba  8b10                   mov edx, dword ptr [eax]
 + 00bc0bbc  894cd608               mov dword ptr [esi + edx*8 + 8], ecx
 + 00bc0bc0  8b5904                 mov ebx, dword ptr [ecx + 4]
 + 00bc0bc3  8b10                   mov edx, dword ptr [eax]
 + 00bc0bc5  895cd60c               mov dword ptr [esi + edx*8 + 0xc], ebx
 + 00bc0bc9  ff00                   inc dword ptr [eax]
 + 00bc0bcb  8b5df8                 mov ebx, dword ptr [ebp - 8]
 + 00bc0bce  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc0bd2  897904                 mov dword ptr [ecx + 4], edi
 + 00bc0bd5  7515                   jne 0xbc0bec
 + 00bc0bd7  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0bdb  8d860c280000           lea eax, [esi + 0x280c]
 + 00bc0be1  8b10                   mov edx, dword ptr [eax]
 + 00bc0be3  898c9608200000         mov dword ptr [esi + edx*4 + 0x2008], ecx
 + 00bc0bea  ff00                   inc dword ptr [eax]
 + 00bc0bec  a100000000             mov eax, dword ptr [0]
 + 00bc0bf1  8b1504000000           mov edx, dword ptr [4]
 + 00bc0bf7  8d8830290000           lea ecx, [eax + 0x2930]
 + 00bc0bfd  395104                 cmp dword ptr [ecx + 4], edx
 + 00bc0c00  8bf0                   mov esi, eax
 + 00bc0c02  8bfa                   mov edi, edx
 + 00bc0c04  744a                   je 0xbc0c50
 + 00bc0c06  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0c0c  8b5108                 mov edx, dword ptr [ecx + 8]
 + 00bc0c0f  85c2                   test edx, eax
 + 00bc0c11  751f                   jne 0xbc0c32
 + 00bc0c13  0bc2                   or eax, edx
 + 00bc0c15  894108                 mov dword ptr [ecx + 8], eax
 + 00bc0c18  8d8608280000           lea eax, [esi + 0x2808]
 + 00bc0c1e  8b10                   mov edx, dword ptr [eax]
 + 00bc0c20  894cd608               mov dword ptr [esi + edx*8 + 8], ecx
 + 00bc0c24  8b5904                 mov ebx, dword ptr [ecx + 4]
 + 00bc0c27  8b10                   mov edx, dword ptr [eax]
 + 00bc0c29  895cd60c               mov dword ptr [esi + edx*8 + 0xc], ebx
 + 00bc0c2d  ff00                   inc dword ptr [eax]
 + 00bc0c2f  8b5df8                 mov ebx, dword ptr [ebp - 8]
 + 00bc0c32  80791000               cmp byte ptr [ecx + 0x10], 0
 + 00bc0c36  897904                 mov dword ptr [ecx + 4], edi
 + 00bc0c39  7515                   jne 0xbc0c50
 + 00bc0c3b  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc0c3f  8d860c280000           lea eax, [esi + 0x280c]
 + 00bc0c45  8b10                   mov edx, dword ptr [eax]
 + 00bc0c47  898c9608200000         mov dword ptr [esi + edx*4 + 0x2008], ecx
 + 00bc0c4e  ff00                   inc dword ptr [eax]
 + 00bc0c50  c6450f01               mov byte ptr [ebp + 0xf], 1
 + 00bc0c54  a100000000             mov eax, dword ptr [0]
 + 00bc0c59  8b4870                 mov ecx, dword ptr [eax + 0x70]
 + 00bc0c5c  8a8384000000           mov al, byte ptr [ebx + 0x84]
 + 00bc0c62  33ff                   xor edi, edi
   00bc0ca0  a802                   test al, 2
   00bc0ca2  7469                   je 0xbc0d0d
@@ retail 514-514 built 548-549
   00bc0c9e  33f6                   xor esi, esi
   00bc0ca0  a802                   test al, 2
 + 00bc0c66  894dec                 mov dword ptr [ebp - 0x14], ecx
   00bc0ca2  7469                   je 0xbc0d0d
   00bc0ca4  a820                   test al, 0x20
@@ retail 517-518 built 552-552
   00bc0ca4  a820                   test al, 0x20
   00bc0ca6  740b                   je 0xbc0cb3
 - 00bc0ca8  8bcb                   mov ecx, ebx
   00bc0caa  e801b4ffff             call 0xbbc0b0
   00bc0caf  8bf0                   mov esi, eax
@@ retail 519-520 built 553-554
   00bc0ca8  8bcb                   mov ecx, ebx
   00bc0caa  e801b4ffff             call 0xbbc0b0
 - 00bc0caf  8bf0                   mov esi, eax
 + 00bc0c74  8bf8                   mov edi, eax
   00bc0cb1  eb7b                   jmp 0xbc0d2e
   00bc0cb3  8b4d4c                 mov ecx, dword ptr [ebp + 0x4c]
@@ retail 521-532 built 555-555
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
 - 00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
   00bc0ce2  8954242c               mov dword ptr [esp + 0x2c], edx
@@ retail 533-536 built 556-571
   00bc0cdb  51                     push ecx
   00bc0cdc  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0ce2  8954242c               mov dword ptr [esp + 0x2c], edx
 - 00bc0ce6  89442424               mov dword ptr [esp + 0x24], eax
 - 00bc0cea  8b4570                 mov eax, dword ptr [ebp + 0x70]
 + 00bc0c7e  c645f400               mov byte ptr [ebp - 0xc], 0
 + 00bc0c82  c645f500               mov byte ptr [ebp - 0xb], 0
 + 00bc0c86  c645f600               mov byte ptr [ebp - 0xa], 0
 + 00bc0c8a  8b45f4                 mov eax, dword ptr [ebp - 0xc]
 + 00bc0c8d  8945f4                 mov dword ptr [ebp - 0xc], eax
 + 00bc0c90  8b4348                 mov eax, dword ptr [ebx + 0x48]
 + 00bc0c93  8945d0                 mov dword ptr [ebp - 0x30], eax
 + 00bc0c96  8b434c                 mov eax, dword ptr [ebx + 0x4c]
 + 00bc0c99  8945d4                 mov dword ptr [ebp - 0x2c], eax
 + 00bc0c9c  8b4350                 mov eax, dword ptr [ebx + 0x50]
 + 00bc0c9f  8945d8                 mov dword ptr [ebp - 0x28], eax
 + 00bc0ca2  8b4370                 mov eax, dword ptr [ebx + 0x70]
 + 00bc0ca5  8945dc                 mov dword ptr [ebp - 0x24], eax
 + 00bc0ca8  8d45f4                 lea eax, [ebp - 0xc]
 + 00bc0cab  50                     push eax
   00bc0ced  6a05                   push 5
   00bc0cef  8d542428               lea edx, [esp + 0x28]
@@ retail 537-540 built 572-574
   00bc0cea  8b4570                 mov eax, dword ptr [ebp + 0x70]
   00bc0ced  6a05                   push 5
 - 00bc0cef  8d542428               lea edx, [esp + 0x28]
 - 00bc0cf3  52                     push edx
 - 00bc0cf4  89442438               mov dword ptr [esp + 0x38], eax
 + 00bc0cae  8d45d0                 lea eax, [ebp - 0x30]
 + 00bc0cb1  50                     push eax
   00bc0cf8  e8935ef8ff             call 0xb46b90
   00bc0cfd  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
@@ retail 542-543 built 576-577
   00bc0cf8  e8935ef8ff             call 0xb46b90
   00bc0cfd  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
 - 00bc0d03  8b30                   mov esi, dword ptr [eax]
 + 00bc0cbd  8b38                   mov edi, dword ptr [eax]
   00bc0d05  50                     push eax
   00bc0d06  e81542f8ff             call 0xb44f20
@@ retail 547-550 built 581-586
   00bc0d0b  eb21                   jmp 0xbc0d2e
   00bc0d0d  a19c6e4301             mov eax, dword ptr [0x1436e9c]
 - 00bc0d12  8b90e8000000           mov edx, dword ptr [eax + 0xe8]
 - 00bc0d18  b964000000             mov ecx, 0x64
 - 00bc0d1d  3bd1                   cmp edx, ecx
 + 00bc0ccc  6a64                   push 0x64
 + 00bc0cce  8bc8                   mov ecx, eax
 + 00bc0cd0  05e8000000             add eax, 0xe8
 + 00bc0cd5  5a                     pop edx
 + 00bc0cd6  3910                   cmp dword ptr [eax], edx
   00bc0d1f  740d                   je 0xbc0d2e
   00bc0d21  8988e8000000           mov dword ptr [eax + 0xe8], ecx
@@ retail 551-553 built 587-589
   00bc0d1d  3bd1                   cmp edx, ecx
   00bc0d1f  740d                   je 0xbc0d2e
 - 00bc0d21  8988e8000000           mov dword ptr [eax + 0xe8], ecx
 - 00bc0d27  8388e400000002         or dword ptr [eax + 0xe4], 2
 + 00bc0cda  8389e400000002         or dword ptr [ecx + 0xe4], 2
 + 00bc0ce1  8910                   mov dword ptr [eax], edx
   00bc0d2e  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
   00bc0d34  e8475ff8ff             call 0xb46c80
@@ retail 555-556 built 591-592
   00bc0d2e  8b0d9c6e4301           mov ecx, dword ptr [0x1436e9c]
   00bc0d34  e8475ff8ff             call 0xb46c80
 - 00bc0d39  8a8584000000           mov al, byte ptr [ebp + 0x84]
 + 00bc0cee  8a8384000000           mov al, byte ptr [ebx + 0x84]
   00bc0d3f  a804                   test al, 4
   00bc0d41  7429                   je 0xbc0d6c
@@ retail 558-559 built 594-595
   00bc0d3f  a804                   test al, 4
   00bc0d41  7429                   je 0xbc0d6c
 - 00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0cf8  8b4dfc                 mov ecx, dword ptr [ebp - 4]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
   00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
@@ retail 560-562 built 596-598
   00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
 - 00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
 - 00bc0d4d  2bd5                   sub edx, ebp
 + 00bc0cfe  2b5124                 sub edx, dword ptr [ecx + 0x24]
 + 00bc0d01  8bc7                   mov eax, edi
   00bc0d4f  c1fa03                 sar edx, 3
   00bc0d52  4a                     dec edx
@@ retail 564-566 built 600-601
   00bc0d4f  c1fa03                 sar edx, 3
   00bc0d52  4a                     dec edx
 - 00bc0d53  3bf2                   cmp esi, edx
 - 00bc0d55  8bc6                   mov eax, esi
 + 00bc0d07  3bfa                   cmp edi, edx
   00bc0d57  760b                   jbe 0xbc0d64
   00bc0d59  8b4128                 mov eax, dword ptr [ecx + 0x28]
@@ retail 568-570 built 603-604
   00bc0d57  760b                   jbe 0xbc0d64
   00bc0d59  8b4128                 mov eax, dword ptr [ecx + 0x28]
 - 00bc0d5c  8bfd                   mov edi, ebp
 - 00bc0d5e  2bc7                   sub eax, edi
 + 00bc0d0e  2b4124                 sub eax, dword ptr [ecx + 0x24]
   00bc0d60  c1f803                 sar eax, 3
   00bc0d63  48                     dec eax
@@ retail 577-580 built 611-612
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 - 00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
 - 00bc0d86  3bfa                   cmp edi, edx
 + 00bc0d2b  8b3508000000           mov esi, dword ptr [8]
   00bc0d88  8d8190280000           lea eax, [ecx + 0x2890]
   00bc0d8e  8bea                   mov ebp, edx
@@ retail 581-582 built 613-616
   00bc0d86  3bfa                   cmp edi, edx
   00bc0d88  8d8190280000           lea eax, [ecx + 0x2890]
 - 00bc0d8e  8bea                   mov ebp, edx
 + 00bc0d37  397004                 cmp dword ptr [eax + 4], esi
 + 00bc0d3a  8bd1                   mov edx, ecx
 + 00bc0d3c  8975f4                 mov dword ptr [ebp - 0xc], esi
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
@@ retail 583-586 built 617-620
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 - 00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
 - 00bc0d9b  85d7                   test edi, edx
 + 00bc0d41  8b8914280000           mov ecx, dword ptr [ecx + 0x2814]
 + 00bc0d47  8b7008                 mov esi, dword ptr [eax + 8]
 + 00bc0d4a  85ce                   test esi, ecx
   00bc0d9d  7522                   jne 0xbc0dc1
   00bc0d9f  0bd7                   or edx, edi
@@ retail 587-598 built 621-633
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
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
 + 00bc0d4e  0bce                   or ecx, esi
 + 00bc0d50  894808                 mov dword ptr [eax + 8], ecx
 + 00bc0d53  8d8a08280000           lea ecx, [edx + 0x2808]
 + 00bc0d59  8b31                   mov esi, dword ptr [ecx]
 + 00bc0d5b  8944f208               mov dword ptr [edx + esi*8 + 8], eax
 + 00bc0d5f  8b31                   mov esi, dword ptr [ecx]
 + 00bc0d61  8b5804                 mov ebx, dword ptr [eax + 4]
 + 00bc0d64  895cf20c               mov dword ptr [edx + esi*8 + 0xc], ebx
 + 00bc0d68  ff01                   inc dword ptr [ecx]
 + 00bc0d6a  80781000               cmp byte ptr [eax + 0x10], 0
 + 00bc0d6e  8b4df4                 mov ecx, dword ptr [ebp - 0xc]
 + 00bc0d71  894804                 mov dword ptr [eax + 4], ecx
   00bc0dc9  7517                   jne 0xbc0de2
   00bc0dcb  c6401001               mov byte ptr [eax + 0x10], 1
@@ retail 600-607 built 635-643
   00bc0dc9  7517                   jne 0xbc0de2
   00bc0dcb  c6401001               mov byte ptr [eax + 0x10], 1
 - 00bc0dcf  8b910c280000           mov edx, dword ptr [ecx + 0x280c]
 - 00bc0dd5  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
 - 00bc0ddc  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0de2  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0de6  8b4f1c                 mov ecx, dword ptr [edi + 0x1c]
 - 00bc0de9  8b6f18                 mov ebp, dword ptr [edi + 0x18]
 - 00bc0dec  2bcd                   sub ecx, ebp
 + 00bc0d7a  8d8a0c280000           lea ecx, [edx + 0x280c]
 + 00bc0d80  8b31                   mov esi, dword ptr [ecx]
 + 00bc0d82  8984b208200000         mov dword ptr [edx + esi*4 + 0x2008], eax
 + 00bc0d89  ff01                   inc dword ptr [ecx]
 + 00bc0d8b  8b75fc                 mov esi, dword ptr [ebp - 4]
 + 00bc0d8e  8b4e1c                 mov ecx, dword ptr [esi + 0x1c]
 + 00bc0d91  2b4e18                 sub ecx, dword ptr [esi + 0x18]
 + 00bc0d94  8bc7                   mov eax, edi
   00bc0dee  c1f903                 sar ecx, 3
   00bc0df1  49                     dec ecx
@@ retail 609-611 built 645-646
   00bc0dee  c1f903                 sar ecx, 3
   00bc0df1  49                     dec ecx
 - 00bc0df2  3bf1                   cmp esi, ecx
 - 00bc0df4  8bc6                   mov eax, esi
 + 00bc0d9a  3bf9                   cmp edi, ecx
   00bc0df6  760b                   jbe 0xbc0e03
   00bc0df8  8b471c                 mov eax, dword ptr [edi + 0x1c]
@@ retail 612-615 built 647-649
   00bc0df4  8bc6                   mov eax, esi
   00bc0df6  760b                   jbe 0xbc0e03
 - 00bc0df8  8b471c                 mov eax, dword ptr [edi + 0x1c]
 - 00bc0dfb  8bf5                   mov esi, ebp
 - 00bc0dfd  2bc6                   sub eax, esi
 + 00bc0d9e  8b461c                 mov eax, dword ptr [esi + 0x1c]
 + 00bc0da1  2b4618                 sub eax, dword ptr [esi + 0x18]
   00bc0dff  c1f803                 sar eax, 3
   00bc0e02  48                     dec eax
@@ retail 617-618 built 651-653
   00bc0dff  c1f803                 sar eax, 3
   00bc0e02  48                     dec eax
 - 00bc0e03  8b5718                 mov edx, dword ptr [edi + 0x18]
 + 00bc0da8  8b4e18                 mov ecx, dword ptr [esi + 0x18]
 + 00bc0dab  8d04c1                 lea eax, [ecx + eax*8]
   00bc0e06  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0e0c  6a00                   push 0
@@ retail 620-621 built 655-655
   00bc0e06  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0e0c  6a00                   push 0
 - 00bc0e0e  8d04c2                 lea eax, [edx + eax*8]
   00bc0e11  50                     push eax
   00bc0e12  e80972dcff             call 0x988020
@@ retail 623-623 built 657-658
   00bc0e11  50                     push eax
   00bc0e12  e80972dcff             call 0x988020
 + 00bc0dbc  8b5df8                 mov ebx, dword ptr [ebp - 8]
   00bc0e17  eb39                   jmp 0xbc0e52
   00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
@@ retail 624-625 built 659-660
   00bc0e12  e80972dcff             call 0x988020
   00bc0e17  eb39                   jmp 0xbc0e52
 - 00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0dc1  8b4dfc                 mov ecx, dword ptr [ebp - 4]
   00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
   00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
@@ retail 626-628 built 661-663
   00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
   00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
 - 00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
 - 00bc0e23  2bd5                   sub edx, ebp
 + 00bc0dc7  2b510c                 sub edx, dword ptr [ecx + 0xc]
 + 00bc0dca  8bc7                   mov eax, edi
   00bc0e25  c1fa03                 sar edx, 3
   00bc0e28  4a                     dec edx
@@ retail 630-632 built 665-666
   00bc0e25  c1fa03                 sar edx, 3
   00bc0e28  4a                     dec edx
 - 00bc0e29  3bf2                   cmp esi, edx
 - 00bc0e2b  8bc6                   mov eax, esi
 + 00bc0dd0  3bfa                   cmp edi, edx
   00bc0e2d  760b                   jbe 0xbc0e3a
   00bc0e2f  8b4110                 mov eax, dword ptr [ecx + 0x10]
@@ retail 634-636 built 668-669
   00bc0e2d  760b                   jbe 0xbc0e3a
   00bc0e2f  8b4110                 mov eax, dword ptr [ecx + 0x10]
 - 00bc0e32  8bfd                   mov edi, ebp
 - 00bc0e34  2bc7                   sub eax, edi
 + 00bc0dd7  2b410c                 sub eax, dword ptr [ecx + 0xc]
   00bc0e36  c1f803                 sar eax, 3
   00bc0e39  48                     dec eax
@@ retail 639-640 built 672-673
   00bc0e39  48                     dec eax
   00bc0e3a  8b490c                 mov ecx, dword ptr [ecx + 0xc]
 - 00bc0e3d  8d14c1                 lea edx, [ecx + eax*8]
 + 00bc0de1  8d04c1                 lea eax, [ecx + eax*8]
   00bc0e40  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0e46  6a00                   push 0
@@ retail 642-643 built 675-676
   00bc0e40  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0e46  6a00                   push 0
 - 00bc0e48  52                     push edx
 + 00bc0dec  50                     push eax
   00bc0e49  e8d271dcff             call 0x988020
   00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
@@ retail 644-647 built 677-681
   00bc0e48  52                     push edx
   00bc0e49  e8d271dcff             call 0x988020
 - 00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
 - 00bc0e52  8b742418               mov esi, dword ptr [esp + 0x18]
 - 00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
 + 00bc0df2  8b75fc                 mov esi, dword ptr [ebp - 4]
 + 00bc0df5  f6838400000020         test byte ptr [ebx + 0x84], 0x20
 + 00bc0dfc  8d4648                 lea eax, [esi + 0x48]
 + 00bc0dff  50                     push eax
   00bc0e5d  7413                   je 0xbc0e72
   00bc0e5f  8d4748                 lea eax, [edi + 0x48]
@@ retail 648-649 built 682-684
   00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
   00bc0e5d  7413                   je 0xbc0e72
 - 00bc0e5f  8d4748                 lea eax, [edi + 0x48]
 + 00bc0e02  8b4dec                 mov ecx, dword ptr [ebp - 0x14]
 + 00bc0e05  8d4650                 lea eax, [esi + 0x50]
   00bc0e62  50                     push eax
   00bc0e63  8d4f50                 lea ecx, [edi + 0x50]
@@ retail 650-652 built 685-685
   00bc0e5f  8d4748                 lea eax, [edi + 0x48]
   00bc0e62  50                     push eax
 - 00bc0e63  8d4f50                 lea ecx, [edi + 0x50]
 - 00bc0e66  51                     push ecx
   00bc0e67  6a00                   push 0
   00bc0e69  8bcb                   mov ecx, ebx
@@ retail 653-654 built 686-686
   00bc0e66  51                     push ecx
   00bc0e67  6a00                   push 0
 - 00bc0e69  8bcb                   mov ecx, ebx
   00bc0e6b  e800beffff             call 0xbbcc70
   00bc0e70  eb0f                   jmp 0xbc0e81
@@ retail 657-659 built 689-689
   00bc0e70  eb0f                   jmp 0xbc0e81
   00bc0e72  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc0e78  8d5748                 lea edx, [edi + 0x48]
 - 00bc0e7b  52                     push edx
   00bc0e7c  e8bf72dcff             call 0x988140
   00bc0e81  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
@@ retail 660-660 built 690-695
   00bc0e7b  52                     push edx
   00bc0e7c  e8bf72dcff             call 0x988140
 + 00bc0e1d  8d4660                 lea eax, [esi + 0x60]
 + 00bc0e20  33c9                   xor ecx, ecx
 + 00bc0e22  3bc1                   cmp eax, ecx
 + 00bc0e24  741d                   je 0xbc0e43
 + 00bc0e26  0fb67810               movzx edi, byte ptr [eax + 0x10]
   00bc0e81  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc0e87  8d4760                 lea eax, [edi + 0x60]
@@ retail 661-666 built 696-696
   00bc0e7c  e8bf72dcff             call 0x988140
   00bc0e81  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0e87  8d4760                 lea eax, [edi + 0x60]
 - 00bc0e8a  85c0                   test eax, eax
 - 00bc0e8c  7418                   je 0xbc0ea6
 - 00bc0e8e  0fb65810               movzx ebx, byte ptr [eax + 0x10]
 - 00bc0e92  8b4004                 mov eax, dword ptr [eax + 4]
   00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
   00bc0e9b  8b11                   mov edx, dword ptr [ecx]
@@ retail 668-669 built 698-699
   00bc0e95  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
   00bc0e9b  8b11                   mov edx, dword ptr [ecx]
 - 00bc0e9d  53                     push ebx
 + 00bc0e38  57                     push edi
   00bc0e9e  6a00                   push 0
   00bc0ea0  50                     push eax
@@ retail 670-671 built 700-701
   00bc0e9d  53                     push ebx
   00bc0e9e  6a00                   push 0
 - 00bc0ea0  50                     push eax
 + 00bc0e3b  ff7004                 push dword ptr [eax + 4]
   00bc0ea1  6a00                   push 0
   00bc0ea3  51                     push ecx
@@ retail 674-675 built 704-706
   00bc0ea3  51                     push ecx
   00bc0ea4  eb11                   jmp 0xbc0eb7
 - 00bc0ea6  8b81f03c0000           mov eax, dword ptr [ecx + 0x3cf0]
 + 00bc0e43  a100000000             mov eax, dword ptr [0]
 + 00bc0e48  8b80f03c0000           mov eax, dword ptr [eax + 0x3cf0]
   00bc0eac  8b10                   mov edx, dword ptr [eax]
   00bc0eae  6a00                   push 0
@@ retail 676-679 built 707-725
   00bc0ea6  8b81f03c0000           mov eax, dword ptr [ecx + 0x3cf0]
   00bc0eac  8b10                   mov edx, dword ptr [eax]
 - 00bc0eae  6a00                   push 0
 - 00bc0eb0  6a00                   push 0
 - 00bc0eb2  6a00                   push 0
 + 00bc0e50  51                     push ecx
 + 00bc0e51  51                     push ecx
 + 00bc0e52  51                     push ecx
 + 00bc0e53  51                     push ecx
 + 00bc0e54  50                     push eax
 + 00bc0e55  ff9290010000           call dword ptr [edx + 0x190]
 + 00bc0e5b  a100000000             mov eax, dword ptr [0]
 + 00bc0e60  8d8e94000000           lea ecx, [esi + 0x94]
 + 00bc0e66  85c9                   test ecx, ecx
 + 00bc0e68  8988f43c0000           mov dword ptr [eax + 0x3cf4], ecx
 + 00bc0e6e  8b80f03c0000           mov eax, dword ptr [eax + 0x3cf0]
 + 00bc0e74  740e                   je 0xbc0e84
 + 00bc0e76  ff7104                 push dword ptr [ecx + 4]
 + 00bc0e79  8b10                   mov edx, dword ptr [eax]
 + 00bc0e7b  50                     push eax
 + 00bc0e7c  ff92a0010000           call dword ptr [edx + 0x1a0]
 + 00bc0e82  eb0b                   jmp 0xbc0e8f
 + 00bc0e84  8b08                   mov ecx, dword ptr [eax]
   00bc0eb4  6a00                   push 0
   00bc0eb6  50                     push eax
@@ retail 681-682 built 727-773
   00bc0eb4  6a00                   push 0
   00bc0eb6  50                     push eax
 - 00bc0eb7  ff9290010000           call dword ptr [edx + 0x190]
 + 00bc0e89  ff91a0010000           call dword ptr [ecx + 0x1a0]
 + 00bc0e8f  a100000000             mov eax, dword ptr [0]
 + 00bc0e94  8b7df0                 mov edi, dword ptr [ebp - 0x10]
 + 00bc0e97  0544010000             add eax, 0x144
 + 00bc0e9c  8b10                   mov edx, dword ptr [eax]
 + 00bc0e9e  8d8ea8000000           lea ecx, [esi + 0xa8]
 + 00bc0ea4  8911                   mov dword ptr [ecx], edx
 + 00bc0ea6  8b500c                 mov edx, dword ptr [eax + 0xc]
 + 00bc0ea9  8996ac000000           mov dword ptr [esi + 0xac], edx
 + 00bc0eaf  8b5368                 mov edx, dword ptr [ebx + 0x68]
 + 00bc0eb2  8996b0000000           mov dword ptr [esi + 0xb0], edx
 + 00bc0eb8  8b536c                 mov edx, dword ptr [ebx + 0x6c]
 + 00bc0ebb  8996b4000000           mov dword ptr [esi + 0xb4], edx
 + 00bc0ec1  8b5004                 mov edx, dword ptr [eax + 4]
 + 00bc0ec4  8996b8000000           mov dword ptr [esi + 0xb8], edx
 + 00bc0eca  8b5010                 mov edx, dword ptr [eax + 0x10]
 + 00bc0ecd  8996bc000000           mov dword ptr [esi + 0xbc], edx
 + 00bc0ed3  0fb75706               movzx edx, word ptr [edi + 6]
 + 00bc0ed7  8955ec                 mov dword ptr [ebp - 0x14], edx
 + 00bc0eda  0fb717                 movzx edx, word ptr [edi]
 + 00bc0edd  db45ec                 fild dword ptr [ebp - 0x14]
 + 00bc0ee0  8955ec                 mov dword ptr [ebp - 0x14], edx
 + 00bc0ee3  da75ec                 fidiv dword ptr [ebp - 0x14]
 + 00bc0ee6  d99ec0000000           fstp dword ptr [esi + 0xc0]
 + 00bc0eec  0fb75708               movzx edx, word ptr [edi + 8]
 + 00bc0ef0  8955ec                 mov dword ptr [ebp - 0x14], edx
 + 00bc0ef3  0fb75702               movzx edx, word ptr [edi + 2]
 + 00bc0ef7  db45ec                 fild dword ptr [ebp - 0x14]
 + 00bc0efa  8955ec                 mov dword ptr [ebp - 0x14], edx
 + 00bc0efd  da75ec                 fidiv dword ptr [ebp - 0x14]
 + 00bc0f00  d99ec4000000           fstp dword ptr [esi + 0xc4]
 + 00bc0f06  8b5008                 mov edx, dword ptr [eax + 8]
 + 00bc0f09  8996c8000000           mov dword ptr [esi + 0xc8], edx
 + 00bc0f0f  8b4014                 mov eax, dword ptr [eax + 0x14]
 + 00bc0f12  8986cc000000           mov dword ptr [esi + 0xcc], eax
 + 00bc0f18  f6838400000008         test byte ptr [ebx + 0x84], 8
 + 00bc0f1f  7404                   je 0xbc0f25
 + 00bc0f21  d9e8                   fld1 
 + 00bc0f23  eb02                   jmp 0xbc0f27
 + 00bc0f25  d9ee                   fldz 
 + 00bc0f27  d99ed0000000           fstp dword ptr [esi + 0xd0]
 + 00bc0f2d  51                     push ecx
 + 00bc0f2e  d9ee                   fldz 
 + 00bc0f30  6a03                   push 3
 + 00bc0f32  d99ed4000000           fstp dword ptr [esi + 0xd4]
 + 00bc0f38  6a01                   push 1
   00bc0ebd  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc0ec3  8d8794000000           lea eax, [edi + 0x94]
@@ retail 683-740 built 774-774
   00bc0eb7  ff9290010000           call dword ptr [edx + 0x190]
   00bc0ebd  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0ec3  8d8794000000           lea eax, [edi + 0x94]
 - 00bc0ec9  85c0                   test eax, eax
 - 00bc0ecb  8981f43c0000           mov dword ptr [ecx + 0x3cf4], eax
 - 00bc0ed1  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
 - 00bc0ed7  8b11                   mov edx, dword ptr [ecx]
 - 00bc0ed9  7406                   je 0xbc0ee1
 - 00bc0edb  8b4004                 mov eax, dword ptr [eax + 4]
 - 00bc0ede  50                     push eax
 - 00bc0edf  eb02                   jmp 0xbc0ee3
 - 00bc0ee1  6a00                   push 0
 - 00bc0ee3  51                     push ecx
 - 00bc0ee4  ff92a0010000           call dword ptr [edx + 0x1a0]
 - 00bc0eea  a1a06e4301             mov eax, dword ptr [0x1436ea0]
 - 00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
 - 00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 - 00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 - 00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
 - 00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
 - 00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
 - 00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
 - 00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 - 00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
 - 00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
 - 00bc0f2e  8b5010                 mov edx, dword ptr [eax + 0x10]
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
 - 00bc0f72  8b5008                 mov edx, dword ptr [eax + 8]
 - 00bc0f75  8997c8000000           mov dword ptr [edi + 0xc8], edx
 - 00bc0f7b  8b4014                 mov eax, dword ptr [eax + 0x14]
 - 00bc0f7e  8987cc000000           mov dword ptr [edi + 0xcc], eax
 - 00bc0f84  f6868400000008         test byte ptr [esi + 0x84], 8
 - 00bc0f8b  7408                   je 0xbc0f95
 - 00bc0f8d  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc0f93  eb06                   jmp 0xbc0f9b
 - 00bc0f95  d905dcde2201           fld dword ptr [0x122dedc]
 - 00bc0f9b  51                     push ecx
 - 00bc0f9c  d99fd0000000           fstp dword ptr [edi + 0xd0]
 - 00bc0fa2  6a03                   push 3
 - 00bc0fa4  c787d400000000000000   mov dword ptr [edi + 0xd4], 0
 - 00bc0fae  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc0fb4  6a01                   push 1
   00bc0fb6  e81587dcff             call 0x9896d0
   00bc0fbb  8a8683000000           mov al, byte ptr [esi + 0x83]
@@ retail 741-743 built 775-776
   00bc0fb4  6a01                   push 1
   00bc0fb6  e81587dcff             call 0x9896d0
 - 00bc0fbb  8a8683000000           mov al, byte ptr [esi + 0x83]
 - 00bc0fc1  84c0                   test al, al
 + 00bc0f45  80bb8300000000         cmp byte ptr [ebx + 0x83], 0
   00bc0fc3  0f86cd020000           jbe 0xbc1296
   00bc0fc9  d94660                 fld dword ptr [esi + 0x60]
@@ retail 744-745 built 777-778
   00bc0fc1  84c0                   test al, al
   00bc0fc3  0f86cd020000           jbe 0xbc1296
 - 00bc0fc9  d94660                 fld dword ptr [esi + 0x60]
 + 00bc0f52  d94360                 fld dword ptr [ebx + 0x60]
   00bc0fcc  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00bc0fd2  dfe0                   fnstsw ax
@@ retail 749-750 built 782-783
   00bc0fd4  f6c441                 test ah, 0x41
   00bc0fd7  0f85b9020000           jne 0xbc1296
 - 00bc0fdd  d94664                 fld dword ptr [esi + 0x64]
 + 00bc0f66  d94364                 fld dword ptr [ebx + 0x64]
   00bc0fe0  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00bc0fe6  dfe0                   fnstsw ax
@@ retail 754-755 built 787-787
   00bc0fe8  f6c441                 test ah, 0x41
   00bc0feb  0f85a5020000           jne 0xbc1296
 - 00bc0ff1  8b4e5c                 mov ecx, dword ptr [esi + 0x5c]
   00bc0ff4  6a00                   push 0
   00bc0ff6  6a00                   push 0
@@ retail 757-761 built 789-793
   00bc0ff4  6a00                   push 0
   00bc0ff6  6a00                   push 0
 - 00bc0ff8  51                     push ecx
 - 00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
 - 00bc0ffd  52                     push edx
 - 00bc0ffe  8bcb                   mov ecx, ebx
 + 00bc0f7e  ff735c                 push dword ptr [ebx + 0x5c]
 + 00bc0f81  8d4508                 lea eax, [ebp + 8]
 + 00bc0f84  50                     push eax
 + 00bc0f85  8bcf                   mov ecx, edi
   00bc1000  e8cbe8e3ff             call 0x9ff8d0
   00bc1005  84c0                   test al, al
@@ retail 764-771 built 796-841
   00bc1005  84c0                   test al, al
   00bc1007  0f8489020000           je 0xbc1296
 - 00bc100d  0fb68682000000         movzx eax, byte ptr [esi + 0x82]
 - 00bc1014  0fb68e81000000         movzx ecx, byte ptr [esi + 0x81]
 - 00bc101b  0fb69680000000         movzx edx, byte ptr [esi + 0x80]
 - 00bc1022  89442448               mov dword ptr [esp + 0x48], eax
 - 00bc1026  0fb68683000000         movzx eax, byte ptr [esi + 0x83]
 - 00bc102d  db442448               fild dword ptr [esp + 0x48]
 - 00bc1031  894c2448               mov dword ptr [esp + 0x48], ecx
 + 00bc0f94  807d0f00               cmp byte ptr [ebp + 0xf], 0
 + 00bc0f98  d90500000000           fld dword ptr [0]
 + 00bc0f9e  0fb68382000000         movzx eax, byte ptr [ebx + 0x82]
 + 00bc0fa5  8945ec                 mov dword ptr [ebp - 0x14], eax
 + 00bc0fa8  0fb68381000000         movzx eax, byte ptr [ebx + 0x81]
 + 00bc0faf  db45ec                 fild dword ptr [ebp - 0x14]
 + 00bc0fb2  8945ec                 mov dword ptr [ebp - 0x14], eax
 + 00bc0fb5  0fb68380000000         movzx eax, byte ptr [ebx + 0x80]
 + 00bc0fbc  d8c9                   fmul st(1)
 + 00bc0fbe  d95dd0                 fstp dword ptr [ebp - 0x30]
 + 00bc0fc1  db45ec                 fild dword ptr [ebp - 0x14]
 + 00bc0fc4  8945ec                 mov dword ptr [ebp - 0x14], eax
 + 00bc0fc7  0fb68383000000         movzx eax, byte ptr [ebx + 0x83]
 + 00bc0fce  d8c9                   fmul st(1)
 + 00bc0fd0  d95dd4                 fstp dword ptr [ebp - 0x2c]
 + 00bc0fd3  db45ec                 fild dword ptr [ebp - 0x14]
 + 00bc0fd6  8945ec                 mov dword ptr [ebp - 0x14], eax
 + 00bc0fd9  d8c9                   fmul st(1)
 + 00bc0fdb  d95dd8                 fstp dword ptr [ebp - 0x28]
 + 00bc0fde  db45ec                 fild dword ptr [ebp - 0x14]
 + 00bc0fe1  d8c9                   fmul st(1)
 + 00bc0fe3  d955dc                 fst dword ptr [ebp - 0x24]
 + 00bc0fe6  ddd9                   fstp st(1)
 + 00bc0fe8  7504                   jne 0xbc0fee
 + 00bc0fea  ddd8                   fstp st(0)
 + 00bc0fec  d9e8                   fld1 
 + 00bc0fee  d945d0                 fld dword ptr [ebp - 0x30]
 + 00bc0ff1  51                     push ecx
 + 00bc0ff2  d8c9                   fmul st(1)
 + 00bc0ff4  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc0ffa  8d4348                 lea eax, [ebx + 0x48]
 + 00bc0ffd  d95de0                 fstp dword ptr [ebp - 0x20]
 + 00bc1000  d945d4                 fld dword ptr [ebp - 0x2c]
 + 00bc1003  d8c9                   fmul st(1)
 + 00bc1005  d95de4                 fstp dword ptr [ebp - 0x1c]
 + 00bc1008  d945d8                 fld dword ptr [ebp - 0x28]
 + 00bc100b  d8c9                   fmul st(1)
 + 00bc100d  d95de8                 fstp dword ptr [ebp - 0x18]
 + 00bc1010  ddd8                   fstp st(0)
 + 00bc1012  d9ee                   fldz 
 + 00bc1014  d91c24                 fstp dword ptr [esp]
 + 00bc1017  50                     push eax
 + 00bc1018  6a04                   push 4
 + 00bc101a  e800000000             call 0xbc101f
 + 00bc101f  d94374                 fld dword ptr [ebx + 0x74]
   00bc1035  d80d24172301           fmul dword ptr [0x1231724]
   00bc103b  db442448               fild dword ptr [esp + 0x48]
@@ retail 772-774 built 842-863
   00bc1031  894c2448               mov dword ptr [esp + 0x48], ecx
   00bc1035  d80d24172301           fmul dword ptr [0x1231724]
 - 00bc103b  db442448               fild dword ptr [esp + 0x48]
 - 00bc103f  89542448               mov dword ptr [esp + 0x48], edx
 + 00bc1028  d9550c                 fst dword ptr [ebp + 0xc]
 + 00bc102b  d82500000000           fsub dword ptr [0]
 + 00bc1031  d95dec                 fstp dword ptr [ebp - 0x14]
 + 00bc1034  d945ec                 fld dword ptr [ebp - 0x14]
 + 00bc1037  db5df0                 fistp dword ptr [ebp - 0x10]
 + 00bc103a  db45f0                 fild dword ptr [ebp - 0x10]
 + 00bc103d  8b45f0                 mov eax, dword ptr [ebp - 0x10]
 + 00bc1040  0500ffffff             add eax, 0xffffff00
 + 00bc1045  b9ff030000             mov ecx, 0x3ff
 + 00bc104a  d86d0c                 fsubr dword ptr [ebp + 0xc]
 + 00bc104d  23c1                   and eax, ecx
 + 00bc104f  d9e8                   fld1 
 + 00bc1051  c1e002                 shl eax, 2
 + 00bc1054  d8e1                   fsub st(1)
 + 00bc1056  d88800000000           fmul dword ptr [eax]
 + 00bc105c  d9c1                   fld st(1)
 + 00bc105e  d88804000000           fmul dword ptr [eax + 4]
 + 00bc1064  dec1                   faddp st(1)
 + 00bc1066  d95df4                 fstp dword ptr [ebp - 0xc]
 + 00bc1069  ddd8                   fstp st(0)
 + 00bc106b  d94374                 fld dword ptr [ebx + 0x74]
   00bc1043  d80d24172301           fmul dword ptr [0x1231724]
   00bc1049  d95c2434               fstp dword ptr [esp + 0x34]
@@ retail 775-794 built 864-874
   00bc103f  89542448               mov dword ptr [esp + 0x48], edx
   00bc1043  d80d24172301           fmul dword ptr [0x1231724]
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
 + 00bc1074  d9550c                 fst dword ptr [ebp + 0xc]
 + 00bc1077  d82500000000           fsub dword ptr [0]
 + 00bc107d  d95dec                 fstp dword ptr [ebp - 0x14]
 + 00bc1080  d945ec                 fld dword ptr [ebp - 0x14]
 + 00bc1083  db5df0                 fistp dword ptr [ebp - 0x10]
 + 00bc1086  db45f0                 fild dword ptr [ebp - 0x10]
 + 00bc1089  8b45f0                 mov eax, dword ptr [ebp - 0x10]
 + 00bc108c  83ec10                 sub esp, 0x10
 + 00bc108f  23c1                   and eax, ecx
 + 00bc1091  d86d0c                 fsubr dword ptr [ebp + 0xc]
   00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc1091  d95c2428               fstp dword ptr [esp + 0x28]
@@ retail 795-817 built 875-883
   00bc108a  51                     push ecx
   00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
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
 + 00bc109a  d94364                 fld dword ptr [ebx + 0x64]
 + 00bc109d  c1e002                 shl eax, 2
 + 00bc10a0  d95c240c               fstp dword ptr [esp + 0xc]
 + 00bc10a4  d94360                 fld dword ptr [ebx + 0x60]
 + 00bc10a7  d95c2408               fstp dword ptr [esp + 8]
 + 00bc10ab  d945f4                 fld dword ptr [ebp - 0xc]
 + 00bc10ae  d95c2404               fstp dword ptr [esp + 4]
 + 00bc10b2  d9e8                   fld1 
   00bc10e9  d8e1                   fsub st(1)
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
@@ retail 818-847 built 884-887
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
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
 + 00bc10b6  d88800000000           fmul dword ptr [eax]
 + 00bc10bc  d9c1                   fld st(1)
 + 00bc10be  d88804000000           fmul dword ptr [eax + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 850-850 built 890-891
   00bc115f  d91c24                 fstp dword ptr [esp]
   00bc1162  6a05                   push 5
 + 00bc10cb  ddd8                   fstp st(0)
   00bc1164  e89789dcff             call 0x989b00
   00bc1169  8b44243c               mov eax, dword ptr [esp + 0x3c]
@@ retail 851-857 built 892-893
   00bc1162  6a05                   push 5
   00bc1164  e89789dcff             call 0x989b00
 - 00bc1169  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc116d  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 - 00bc1171  8b542424               mov edx, dword ptr [esp + 0x24]
 - 00bc1175  50                     push eax
 - 00bc1176  8b442424               mov eax, dword ptr [esp + 0x24]
 - 00bc117a  51                     push ecx
 + 00bc10d2  d945dc                 fld dword ptr [ebp - 0x24]
   00bc117b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc1181  52                     push edx
@@ retail 858-860 built 894-902
   00bc117a  51                     push ecx
   00bc117b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc1181  52                     push edx
 - 00bc1182  50                     push eax
 + 00bc10db  83ec10                 sub esp, 0x10
 + 00bc10de  d95c240c               fstp dword ptr [esp + 0xc]
 + 00bc10e2  d945e8                 fld dword ptr [ebp - 0x18]
 + 00bc10e5  d95c2408               fstp dword ptr [esp + 8]
 + 00bc10e9  d945e4                 fld dword ptr [ebp - 0x1c]
 + 00bc10ec  d95c2404               fstp dword ptr [esp + 4]
 + 00bc10f0  d945e0                 fld dword ptr [ebp - 0x20]
 + 00bc10f3  d91c24                 fstp dword ptr [esp]
   00bc1183  6a06                   push 6
   00bc1185  e87689dcff             call 0x989b00
@@ retail 862-863 built 904-906
   00bc1183  6a06                   push 6
   00bc1185  e87689dcff             call 0x989b00
 - 00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc10fd  d9ee                   fldz 
 + 00bc10ff  8b4508                 mov eax, dword ptr [ebp + 8]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
   00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
@@ retail 864-868 built 907-916
   00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
 - 00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
 - 00bc1196  6a00                   push 0
 - 00bc1198  db442448               fild dword ptr [esp + 0x48]
 - 00bc119c  6a00                   push 0
 + 00bc1106  0fb74010               movzx eax, word ptr [eax + 0x10]
 + 00bc110a  83ec10                 sub esp, 0x10
 + 00bc110d  894d0c                 mov dword ptr [ebp + 0xc], ecx
 + 00bc1110  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc1116  d95c240c               fstp dword ptr [esp + 0xc]
 + 00bc111a  d9ee                   fldz 
 + 00bc111c  d95c2408               fstp dword ptr [esp + 8]
 + 00bc1120  db450c                 fild dword ptr [ebp + 0xc]
 + 00bc1123  89450c                 mov dword ptr [ebp + 0xc], eax
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
@@ retail 869-873 built 917-917
   00bc119c  6a00                   push 0
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
 - 00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
 - 00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc11ae  83ec08                 sub esp, 8
 - 00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
   00bc11b5  d95c2404               fstp dword ptr [esp + 4]
   00bc11b9  db442454               fild dword ptr [esp + 0x54]
@@ retail 874-875 built 918-919
   00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
   00bc11b5  d95c2404               fstp dword ptr [esp + 4]
 - 00bc11b9  db442454               fild dword ptr [esp + 0x54]
 + 00bc1130  db450c                 fild dword ptr [ebp + 0xc]
   00bc11bd  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11c3  d91c24                 fstp dword ptr [esp]
@@ retail 880-882 built 924-924
   00bc11c8  e83389dcff             call 0x989b00
   00bc11cd  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc11d3  8a81d8030000           mov al, byte ptr [ecx + 0x3d8]
 - 00bc11d9  a804                   test al, 4
   00bc11db  8db1d8030000           lea esi, [ecx + 0x3d8]
   00bc11e1  8bf9                   mov edi, ecx
@@ retail 883-883 built 925-926
   00bc11d9  a804                   test al, 4
   00bc11db  8db1d8030000           lea esi, [ecx + 0x3d8]
 + 00bc114f  f60604                 test byte ptr [esi], 4
   00bc11e1  8bf9                   mov edi, ecx
   00bc11e3  7437                   je 0xbc121c
@@ retail 886-887 built 929-929
   00bc11e3  7437                   je 0xbc121c
   00bc11e5  8b4104                 mov eax, dword ptr [ecx + 4]
 - 00bc11e8  8b9040010000           mov edx, dword ptr [eax + 0x140]
   00bc11ee  8b9844010000           mov ebx, dword ptr [eax + 0x144]
   00bc11f4  0540010000             add eax, 0x140
@@ retail 889-889 built 931-932
   00bc11ee  8b9844010000           mov ebx, dword ptr [eax + 0x144]
   00bc11f4  0540010000             add eax, 0x140
 + 00bc1164  8b10                   mov edx, dword ptr [eax]
   00bc11f9  2bda                   sub ebx, edx
   00bc11fb  c1fb04                 sar ebx, 4
@@ retail 891-893 built 934-935
   00bc11f9  2bda                   sub ebx, edx
   00bc11fb  c1fb04                 sar ebx, 4
 - 00bc11fe  85db                   test ebx, ebx
 - 00bc1200  7702                   ja 0xbc1204
 + 00bc116b  7502                   jne 0xbc116f
   00bc1202  8bd0                   mov edx, eax
   00bc1204  8b5804                 mov ebx, dword ptr [eax + 4]
@@ retail 895-896 built 937-938
   00bc1202  8bd0                   mov edx, eax
   00bc1204  8b5804                 mov ebx, dword ptr [eax + 4]
 - 00bc1207  8b28                   mov ebp, dword ptr [eax]
 + 00bc1172  2b18                   sub ebx, dword ptr [eax]
   00bc1209  8b4104                 mov eax, dword ptr [ecx + 4]
   00bc120c  2bdd                   sub ebx, ebp
@@ retail 897-898 built 939-939
   00bc1207  8b28                   mov ebp, dword ptr [eax]
   00bc1209  8b4104                 mov eax, dword ptr [ecx + 4]
 - 00bc120c  2bdd                   sub ebx, ebp
   00bc120e  c1fb04                 sar ebx, 4
   00bc1211  53                     push ebx
@@ retail 901-903 built 942-943
   00bc1211  53                     push ebx
   00bc1212  52                     push edx
 - 00bc1213  8b5008                 mov edx, dword ptr [eax + 8]
 - 00bc1216  52                     push edx
 + 00bc117c  ff7008                 push dword ptr [eax + 8]
   00bc1217  e814a7dcff             call 0x98b930
   00bc121c  f60601                 test byte ptr [esi], 1
@@ retail 916-917 built 956-957
   00bc1239  8bcf                   mov ecx, edi
   00bc123b  e82085dcff             call 0x989760
 - 00bc1240  f70600000200           test dword ptr [esi], 0x20000
 + 00bc11a8  f6460202               test byte ptr [esi + 2], 2
   00bc1246  7407                   je 0xbc124f
   00bc1248  8bcf                   mov ecx, edi
@@ retail 920-922 built 960-961
   00bc1248  8bcf                   mov ecx, edi
   00bc124a  e87185dcff             call 0x9897c0
 - 00bc124f  8b06                   mov eax, dword ptr [esi]
 - 00bc1251  f6c404                 test ah, 4
 + 00bc11b5  f6460104               test byte ptr [esi + 1], 4
   00bc1254  7407                   je 0xbc125d
   00bc1256  8bcf                   mov ecx, edi
@@ retail 925-927 built 964-965
   00bc1256  8bcf                   mov ecx, edi
   00bc1258  e80395dcff             call 0x98a760
 - 00bc125d  8b06                   mov eax, dword ptr [esi]
 - 00bc125f  f6c402                 test ah, 2
 + 00bc11c2  f6460102               test byte ptr [esi + 1], 2
   00bc1262  7407                   je 0xbc126b
   00bc1264  8bcf                   mov ecx, edi
@@ retail 930-932 built 968-970
   00bc1264  8bcf                   mov ecx, edi
   00bc1266  e8d592dcff             call 0x98a540
 - 00bc126b  803e00                 cmp byte ptr [esi], 0
 - 00bc126e  7907                   jns 0xbc1277
 + 00bc11cf  f60680                 test byte ptr [esi], 0x80
 + 00bc11d2  7407                   je 0xbc11db
   00bc1270  8bcf                   mov ecx, edi
   00bc1272  e8d977dcff             call 0x988a50
@@ retail 945-947 built 983-985
   00bc1291  e8ca99e4ff             call 0xa0ac60
   00bc1296  a19c6e4301             mov eax, dword ptr [0x1436e9c]
 - 00bc129b  8a88ce000000           mov cl, byte ptr [eax + 0xce]
 - 00bc12a1  84c9                   test cl, cl
 + 00bc11ff  8d88ce000000           lea ecx, [eax + 0xce]
 + 00bc1205  803900                 cmp byte ptr [ecx], 0
   00bc12a3  7416                   je 0xbc12bb
   00bc12a5  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 948-957 built 986-992
   00bc12a1  84c9                   test cl, cl
   00bc12a3  7416                   je 0xbc12bb
 - 00bc12a5  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
 - 00bc12ab  83c901                 or ecx, 1
 - 00bc12ae  c680ce00000000         mov byte ptr [eax + 0xce], 0
 - 00bc12b5  8988e4000000           mov dword ptr [eax + 0xe4], ecx
 - 00bc12bb  8a88e0000000           mov cl, byte ptr [eax + 0xe0]
 - 00bc12c1  84c9                   test cl, cl
 - 00bc12c3  c644244400             mov byte ptr [esp + 0x44], 0
 - 00bc12c8  c644244500             mov byte ptr [esp + 0x45], 0
 - 00bc12cd  c644244600             mov byte ptr [esp + 0x46], 0
 + 00bc120a  8388e400000001         or dword ptr [eax + 0xe4], 1
 + 00bc1211  c60100                 mov byte ptr [ecx], 0
 + 00bc1214  80b8e000000000         cmp byte ptr [eax + 0xe0], 0
 + 00bc121b  c6450c00               mov byte ptr [ebp + 0xc], 0
 + 00bc121f  c6450d00               mov byte ptr [ebp + 0xd], 0
 + 00bc1223  c6450e00               mov byte ptr [ebp + 0xe], 0
   00bc12d2  7514                   jne 0xbc12e8
   00bc12d4  8a88e1000000           mov cl, byte ptr [eax + 0xe1]
@@ retail 958-960 built 993-994
   00bc12cd  c644244600             mov byte ptr [esp + 0x46], 0
   00bc12d2  7514                   jne 0xbc12e8
 - 00bc12d4  8a88e1000000           mov cl, byte ptr [eax + 0xe1]
 - 00bc12da  84c9                   test cl, cl
 + 00bc1229  80b8e100000000         cmp byte ptr [eax + 0xe1], 0
   00bc12dc  750a                   jne 0xbc12e8
   00bc12de  8a88e2000000           mov cl, byte ptr [eax + 0xe2]
@@ retail 961-963 built 995-996
   00bc12da  84c9                   test cl, cl
   00bc12dc  750a                   jne 0xbc12e8
 - 00bc12de  8a88e2000000           mov cl, byte ptr [eax + 0xe2]
 - 00bc12e4  84c9                   test cl, cl
 + 00bc1232  80b8e200000000         cmp byte ptr [eax + 0xe2], 0
   00bc12e6  742d                   je 0xbc1315
   00bc12e8  8b4c2444               mov ecx, dword ptr [esp + 0x44]
@@ retail 964-965 built 997-998
   00bc12e4  84c9                   test cl, cl
   00bc12e6  742d                   je 0xbc1315
 - 00bc12e8  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 + 00bc123b  8b4d0c                 mov ecx, dword ptr [ebp + 0xc]
   00bc12ec  8988e0000000           mov dword ptr [eax + 0xe0], ecx
   00bc12f2  80b8e2000000ff         cmp byte ptr [eax + 0xe2], 0xff
@@ retail 966-967 built 999-1001
   00bc12e8  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00bc12ec  8988e0000000           mov dword ptr [eax + 0xe0], ecx
 - 00bc12f2  80b8e2000000ff         cmp byte ptr [eax + 0xe2], 0xff
 + 00bc1244  8d88e2000000           lea ecx, [eax + 0xe2]
 + 00bc124a  8039ff                 cmp byte ptr [ecx], 0xff
   00bc12f9  7513                   jne 0xbc130e
   00bc12fb  8a90e1000000           mov dl, byte ptr [eax + 0xe1]
@@ retail 970-971 built 1004-1005
   00bc12fb  8a90e1000000           mov dl, byte ptr [eax + 0xe1]
   00bc1301  8890e0000000           mov byte ptr [eax + 0xe0], dl
 - 00bc1307  c680e200000000         mov byte ptr [eax + 0xe2], 0
 + 00bc125b  c60100                 mov byte ptr [ecx], 0
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
   00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
@@ retail 972-975 built 1006-1008
   00bc1307  c680e200000000         mov byte ptr [eax + 0xe2], 0
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
 - 00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 + 00bc1265  8d88e8000000           lea ecx, [eax + 0xe8]
 + 00bc126b  833900                 cmp dword ptr [ecx], 0
   00bc131f  7415                   je 0xbc1336
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 976-980 built 1009-1011
   00bc131d  3bcf                   cmp ecx, edi
   00bc131f  7415                   je 0xbc1336
 - 00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
 - 00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 - 00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
 + 00bc1270  832100                 and dword ptr [ecx], 0
 + 00bc1273  8388e400000002         or dword ptr [eax + 0xe4], 2
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
@@ retail 982-993 built 1013-1015
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
 - 00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 - 00bc1345  3bf7                   cmp esi, edi
 - 00bc1347  7444                   je 0xbc138d
 - 00bc1349  ff0e                   dec dword ptr [esi]
 - 00bc134b  393e                   cmp dword ptr [esi], edi
 - 00bc134d  753e                   jne 0xbc138d
 - 00bc134f  f6461a10               test byte ptr [esi + 0x1a], 0x10
 - 00bc1353  7407                   je 0xbc135c
 - 00bc1355  8bce                   mov ecx, esi
 - 00bc1357  e844b8e3ff             call 0x9fcba0
 - 00bc135c  8b460c                 mov eax, dword ptr [esi + 0xc]
 + 00bc1285  33ff                   xor edi, edi
 + 00bc1287  8b4508                 mov eax, dword ptr [ebp + 8]
   00bc135f  3bc7                   cmp eax, edi
   00bc1361  741b                   je 0xbc137e
@@ retail 996-998 built 1018-1020
   00bc1361  741b                   je 0xbc137e
   00bc1363  ff08                   dec dword ptr [eax]
 - 00bc1365  8b460c                 mov eax, dword ptr [esi + 0xc]
 - 00bc1368  3938                   cmp dword ptr [eax], edi
 + 00bc1290  8bc8                   mov ecx, eax
 + 00bc1292  3939                   cmp dword ptr [ecx], edi
   00bc136a  7512                   jne 0xbc137e
   00bc136c  8b4808                 mov ecx, dword ptr [eax + 8]
@@ retail 999-1003 built 1021-1022
   00bc1368  3938                   cmp dword ptr [eax], edi
   00bc136a  7512                   jne 0xbc137e
 - 00bc136c  8b4808                 mov ecx, dword ptr [eax + 8]
 - 00bc136f  ff5004                 call dword ptr [eax + 4]
 - 00bc1372  8b460c                 mov eax, dword ptr [esi + 0xc]
 - 00bc1375  50                     push eax
 + 00bc1296  6a01                   push 1
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
@@ retail 1004-1010 built 1023-1024
   00bc1375  50                     push eax
   00bc1376  e841d60300             call 0xbfe9bc
 - 00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
 - 00bc1381  56                     push esi
 - 00bc1382  897e08                 mov dword ptr [esi + 8], edi
 - 00bc1385  e832d60300             call 0xbfe9bc
 - 00bc138a  83c404                 add esp, 4
 + 00bc129d  5f                     pop edi
   00bc138d  5e                     pop esi
   00bc138e  5d                     pop ebp
@@ retail 1011-1013 built 1025-1025
   00bc138a  83c404                 add esp, 4
   00bc138d  5e                     pop esi
 - 00bc138e  5d                     pop ebp
 - 00bc138f  5f                     pop edi
   00bc1390  5b                     pop ebx
   00bc1391  83c430                 add esp, 0x30
@@ retail 1014-1015 built 1026-1027
   00bc138f  5f                     pop edi
   00bc1390  5b                     pop ebx
 - 00bc1391  83c430                 add esp, 0x30
 + 00bc12a0  c9                     leave 
   00bc1394  c20800                 ret 8
DIFFLINES 1220 retail_instr 1016 built_instr 1028
