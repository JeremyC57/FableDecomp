00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(3827v3751) flags= []
@@ retail 7-8 built 7-8
   00bc04fc  3bc7                   cmp eax, edi
   00bc04fe  8bd9                   mov ebx, ecx
 - 00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
 + 00bc0500  895c2408               mov dword ptr [esp + 8], ebx
   00bc0504  0f84850e0000           je 0xbc138f
   00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
@@ retail 10-12 built 10-11
   00bc0504  0f84850e0000           je 0xbc138f
   00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
 - 00bc050e  8b0d986e4301           mov ecx, dword ptr [0x1436e98]
 - 00bc0514  8b8954090000           mov ecx, dword ptr [ecx + 0x954]
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
@@ retail 43-44 built 43-44
   00bc0575  57                     push edi
   00bc0576  57                     push edi
 - 00bc0577  8d54241c               lea edx, [esp + 0x1c]
 + 00bc0577  8d542420               lea edx, [esp + 0x20]
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
@@ retail 46-47 built 46-47
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00bc0580  897c2420               mov dword ptr [esp + 0x20], edi
 + 00bc0580  897c2424               mov dword ptr [esp + 0x24], edi
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
@@ retail 61-62 built 61-62
   00bc05b7  89b014280000           mov dword ptr [eax + 0x2814], esi
   00bc05bd  8911                   mov dword ptr [ecx], edx
 - 00bc05bf  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc05bf  8b442414               mov eax, dword ptr [esp + 0x14]
   00bc05c3  8b4808                 mov ecx, dword ptr [eax + 8]
   00bc05c6  3bcf                   cmp ecx, edi
@@ retail 121-122 built 121-122
   00bc069a  ff820c280000           inc dword ptr [edx + 0x280c]
   00bc06a0  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc06a5  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc06a5  8b3510000000           mov esi, dword ptr [0x10]
   00bc06ab  39b020290000           cmp dword ptr [eax + 0x2920], esi
   00bc06b1  8d901c290000           lea edx, [eax + 0x291c]
@@ retail 148-149 built 148-149
   00bc0707  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc070d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0712  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0712  8b3514000000           mov esi, dword ptr [0x14]
   00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
   00bc071e  8d9030290000           lea edx, [eax + 0x2930]
@@ retail 151-253 built 151-151
   00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
   00bc071e  8d9030290000           lea edx, [eax + 0x2930]
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
   00bc08b6  8bc8                   mov ecx, eax
   00bc08b8  8bfe                   mov edi, esi
@@ retail 286-287 built 184-185
   00bc0927  f6c110                 test cl, 0x10
   00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 + 00bc0797  8b3508000000           mov esi, dword ptr [8]
   00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
   00bc093c  3bee                   cmp ebp, esi
@@ retail 293-293 built 191-192
   00bc0946  8bfe                   mov edi, esi
   00bc0948  7450                   je 0xbc099a
 + 00bc07b1  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
   00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 294-295 built 193-193
   00bc0948  7450                   je 0xbc099a
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0953  85f0                   test eax, esi
   00bc0955  7522                   jne 0xbc0979
@@ retail 305-306 built 203-203
   00bc096f  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
   00bc0973  ff8208280000           inc dword ptr [edx + 0x2808]
 - 00bc0979  897904                 mov dword ptr [ecx + 4], edi
   00bc097c  8a4110                 mov al, byte ptr [ecx + 0x10]
   00bc097f  84c0                   test al, al
@@ retail 308-308 built 205-206
   00bc097c  8a4110                 mov al, byte ptr [ecx + 0x10]
   00bc097f  84c0                   test al, al
 + 00bc07e5  897904                 mov dword ptr [ecx + 4], edi
   00bc0981  7517                   jne 0xbc099a
   00bc0983  c6411001               mov byte ptr [ecx + 0x10], 1
@@ retail 313-313 built 211-212
   00bc098d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
   00bc0994  ff820c280000           inc dword ptr [edx + 0x280c]
 + 00bc0801  6a00                   push 0
   00bc099a  8d4b38                 lea ecx, [ebx + 0x38]
   00bc099d  6a00                   push 0
@@ retail 314-314 built 213-215
   00bc0994  ff820c280000           inc dword ptr [edx + 0x280c]
   00bc099a  8d4b38                 lea ecx, [ebx + 0x38]
 + 00bc0806  51                     push ecx
 + 00bc0807  eb06                   jmp 0xbc080f
   00bc099d  6a00                   push 0
   00bc099f  51                     push ecx
@@ retail 315-315 built 216-316
   00bc099a  8d4b38                 lea ecx, [ebx + 0x38]
   00bc099d  6a00                   push 0
 + 00bc080b  8d5330                 lea edx, [ebx + 0x30]
 + 00bc080e  52                     push edx
 + 00bc080f  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc0815  e800000000             call 0xbc081a
 + 00bc081a  83c358                 add ebx, 0x58
 + 00bc081d  53                     push ebx
 + 00bc081e  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc0824  e800000000             call 0xbc0829
 + 00bc0829  8b7c2410               mov edi, dword ptr [esp + 0x10]
 + 00bc082d  8b742418               mov esi, dword ptr [esp + 0x18]
 + 00bc0831  e9af060000             jmp 0xbc0ee5
 + 00bc0836  83f907                 cmp ecx, 7
 + 00bc0839  0f85fb010000           jne 0xbc0a3a
 + 00bc083f  8bb0d0280000           mov esi, dword ptr [eax + 0x28d0]
 + 00bc0845  3bf7                   cmp esi, edi
 + 00bc0847  8d90cc280000           lea edx, [eax + 0x28cc]
 + 00bc084d  c644244400             mov byte ptr [esp + 0x44], 0
 + 00bc0852  8bc8                   mov ecx, eax
 + 00bc0854  7455                   je 0xbc08ab
 + 00bc0856  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc085c  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc085f  85c6                   test esi, eax
 + 00bc0861  7522                   jne 0xbc0885
 + 00bc0863  0bc6                   or eax, esi
 + 00bc0865  894208                 mov dword ptr [edx + 8], eax
 + 00bc0868  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc086e  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc0872  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0878  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc087b  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc087f  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc0885  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc0888  84c0                   test al, al
 + 00bc088a  897a04                 mov dword ptr [edx + 4], edi
 + 00bc088d  7517                   jne 0xbc08a6
 + 00bc088f  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc0893  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0899  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc08a0  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc08a6  a100000000             mov eax, dword ptr [0]
 + 00bc08ab  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
 + 00bc08b1  8d90e0280000           lea edx, [eax + 0x28e0]
 + 00bc08b7  bb10000000             mov ebx, 0x10
 + 00bc08bc  3bf3                   cmp esi, ebx
 + 00bc08be  8bc8                   mov ecx, eax
 + 00bc08c0  7455                   je 0xbc0917
 + 00bc08c2  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc08c8  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc08cb  85c6                   test esi, eax
 + 00bc08cd  7522                   jne 0xbc08f1
 + 00bc08cf  0bc6                   or eax, esi
 + 00bc08d1  894208                 mov dword ptr [edx + 8], eax
 + 00bc08d4  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc08da  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc08de  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc08e4  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc08e7  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc08eb  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc08f1  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc08f4  84c0                   test al, al
 + 00bc08f6  895a04                 mov dword ptr [edx + 4], ebx
 + 00bc08f9  7517                   jne 0xbc0912
 + 00bc08fb  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc08ff  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0905  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc090c  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc0912  a100000000             mov eax, dword ptr [0]
 + 00bc0917  8b3510000000           mov esi, dword ptr [0x10]
 + 00bc091d  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
 + 00bc0923  8d90a4280000           lea edx, [eax + 0x28a4]
 + 00bc0929  8bc8                   mov ecx, eax
 + 00bc092b  8bfe                   mov edi, esi
 + 00bc092d  7455                   je 0xbc0984
 + 00bc092f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc0935  8b7208                 mov esi, dword ptr [edx + 8]
 + 00bc0938  85c6                   test esi, eax
 + 00bc093a  7522                   jne 0xbc095e
 + 00bc093c  0bc6                   or eax, esi
 + 00bc093e  894208                 mov dword ptr [edx + 8], eax
 + 00bc0941  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0947  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
 + 00bc094b  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
 + 00bc0951  8b7204                 mov esi, dword ptr [edx + 4]
 + 00bc0954  8974c10c               mov dword ptr [ecx + eax*8 + 0xc], esi
 + 00bc0958  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00bc095e  8a4210                 mov al, byte ptr [edx + 0x10]
 + 00bc0961  84c0                   test al, al
 + 00bc0963  897a04                 mov dword ptr [edx + 4], edi
 + 00bc0966  7517                   jne 0xbc097f
 + 00bc0968  c6421001               mov byte ptr [edx + 0x10], 1
 + 00bc096c  8b810c280000           mov eax, dword ptr [ecx + 0x280c]
 + 00bc0972  89948108200000         mov dword ptr [ecx + eax*4 + 0x2008], edx
 + 00bc0979  ff810c280000           inc dword ptr [ecx + 0x280c]
 + 00bc097f  a100000000             mov eax, dword ptr [0]
 + 00bc0984  8a8d84000000           mov cl, byte ptr [ebp + 0x84]
 + 00bc098a  f6c104                 test cl, 4
 + 00bc098d  740f                   je 0xbc099e
 + 00bc098f  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc0993  6a00                   push 0
 + 00bc0995  83c140                 add ecx, 0x40
   00bc099f  51                     push ecx
   00bc09a0  e9f7fdffff             jmp 0xbc079c
@@ retail 316-316 built 317-361
   00bc099d  6a00                   push 0
   00bc099f  51                     push ecx
 + 00bc0999  e984000000             jmp 0xbc0a22
 + 00bc099e  84cb                   test bl, cl
 + 00bc09a0  7476                   je 0xbc0a18
 + 00bc09a2  8b3508000000           mov esi, dword ptr [8]
 + 00bc09a8  8b9894280000           mov ebx, dword ptr [eax + 0x2894]
 + 00bc09ae  3bde                   cmp ebx, esi
 + 00bc09b0  8d8890280000           lea ecx, [eax + 0x2890]
 + 00bc09b6  8bd0                   mov edx, eax
 + 00bc09b8  8bfe                   mov edi, esi
 + 00bc09ba  7450                   je 0xbc0a0c
 + 00bc09bc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc09c2  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc09c5  85c6                   test esi, eax
 + 00bc09c7  7522                   jne 0xbc09eb
 + 00bc09c9  0bc6                   or eax, esi
 + 00bc09cb  894108                 mov dword ptr [ecx + 8], eax
 + 00bc09ce  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc09d4  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 + 00bc09d8  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc09de  8b7104                 mov esi, dword ptr [ecx + 4]
 + 00bc09e1  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 + 00bc09e5  ff8208280000           inc dword ptr [edx + 0x2808]
 + 00bc09eb  8a4110                 mov al, byte ptr [ecx + 0x10]
 + 00bc09ee  84c0                   test al, al
 + 00bc09f0  897904                 mov dword ptr [ecx + 4], edi
 + 00bc09f3  7517                   jne 0xbc0a0c
 + 00bc09f5  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc09f9  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 + 00bc09ff  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 + 00bc0a06  ff820c280000           inc dword ptr [edx + 0x280c]
 + 00bc0a0c  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc0a10  6a00                   push 0
 + 00bc0a12  83c138                 add ecx, 0x38
 + 00bc0a15  51                     push ecx
 + 00bc0a16  eb0a                   jmp 0xbc0a22
 + 00bc0a18  8b542410               mov edx, dword ptr [esp + 0x10]
 + 00bc0a1c  6a00                   push 0
 + 00bc0a1e  83c230                 add edx, 0x30
 + 00bc0a21  52                     push edx
 + 00bc0a22  8b0d00000000           mov ecx, dword ptr [0]
 + 00bc0a28  e800000000             call 0xbc0a2d
 + 00bc0a2d  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc0a31  83c058                 add eax, 0x58
 + 00bc0a34  50                     push eax
   00bc09a0  e9f7fdffff             jmp 0xbc079c
   00bc09a5  8b4d7c                 mov ecx, dword ptr [ebp + 0x7c]
@@ retail 326-327 built 371-372
   00bc09be  8bc8                   mov ecx, eax
   00bc09c0  7510                   jne 0xbc09d2
 - 00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
 + 00bc0a57  8b3508000000           mov esi, dword ptr [8]
   00bc09c8  eb08                   jmp 0xbc09d2
   00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
@@ retail 328-329 built 373-374
   00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
   00bc09c8  eb08                   jmp 0xbc09d2
 - 00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
 + 00bc0a5f  8b3504000000           mov esi, dword ptr [4]
   00bc09d0  8bc8                   mov ecx, eax
   00bc09d2  3bde                   cmp ebx, esi
@@ retail 333-333 built 378-379
   00bc09d4  8bfe                   mov edi, esi
   00bc09d6  7455                   je 0xbc0a2d
 + 00bc0a6d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
   00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 334-336 built 380-381
   00bc09d6  7455                   je 0xbc0a2d
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc09e1  85f0                   test eax, esi
 + 00bc0a76  85c6                   test esi, eax
   00bc09e3  7522                   jne 0xbc0a07
   00bc09e5  0bf0                   or esi, eax
@@ retail 337-339 built 382-384
   00bc09e1  85f0                   test eax, esi
   00bc09e3  7522                   jne 0xbc0a07
 - 00bc09e5  0bf0                   or esi, eax
 - 00bc09e7  897208                 mov dword ptr [edx + 8], esi
 + 00bc0a7a  0bc6                   or eax, esi
 + 00bc0a7c  894208                 mov dword ptr [edx + 8], eax
   00bc09ea  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc09f0  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 362-363 built 407-408
   00bc0a46  8bc8                   mov ecx, eax
   00bc0a48  0f84d5000000           je 0xbc0b23
 - 00bc0a4e  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc0ae3  8b3510000000           mov esi, dword ptr [0x10]
   00bc0a54  3bde                   cmp ebx, esi
   00bc0a56  8bfe                   mov edi, esi
@@ retail 368-369 built 413-414
   00bc0a5a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0a60  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0a63  85f0                   test eax, esi
 + 00bc0af8  85c6                   test esi, eax
   00bc0a65  7522                   jne 0xbc0a89
   00bc0a67  0bf0                   or esi, eax
@@ retail 370-372 built 415-417
   00bc0a63  85f0                   test eax, esi
   00bc0a65  7522                   jne 0xbc0a89
 - 00bc0a67  0bf0                   or esi, eax
 - 00bc0a69  897208                 mov dword ptr [edx + 8], esi
 + 00bc0afc  0bc6                   or eax, esi
 + 00bc0afe  894208                 mov dword ptr [edx + 8], eax
   00bc0a6c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0a72  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 387-388 built 432-433
   00bc0aa4  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0aaa  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0aaf  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0b44  8b3514000000           mov esi, dword ptr [0x14]
   00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0abb  3bde                   cmp ebx, esi
@@ retail 396-397 built 441-442
   00bc0ac9  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0acf  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0ad2  85f0                   test eax, esi
 + 00bc0b67  85c6                   test esi, eax
   00bc0ad4  7522                   jne 0xbc0af8
   00bc0ad6  0bf0                   or esi, eax
@@ retail 398-400 built 443-445
   00bc0ad2  85f0                   test eax, esi
   00bc0ad4  7522                   jne 0xbc0af8
 - 00bc0ad6  0bf0                   or esi, eax
 - 00bc0ad8  897108                 mov dword ptr [ecx + 8], esi
 + 00bc0b6b  0bc6                   or eax, esi
 + 00bc0b6d  894108                 mov dword ptr [ecx + 8], eax
   00bc0adb  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0ae1  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 416-417 built 461-462
   00bc0b19  c644244400             mov byte ptr [esp + 0x44], 0
   00bc0b1e  e96c010000             jmp 0xbc0c8f
 - 00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0bb8  8b3504000000           mov esi, dword ptr [4]
   00bc0b29  3bde                   cmp ebx, esi
   00bc0b2b  8bfe                   mov edi, esi
@@ retail 422-423 built 467-468
   00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0b35  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0b38  85f0                   test eax, esi
 + 00bc0bcd  85c6                   test esi, eax
   00bc0b3a  7522                   jne 0xbc0b5e
   00bc0b3c  0bf0                   or esi, eax
@@ retail 424-426 built 469-471
   00bc0b38  85f0                   test eax, esi
   00bc0b3a  7522                   jne 0xbc0b5e
 - 00bc0b3c  0bf0                   or esi, eax
 - 00bc0b3e  897208                 mov dword ptr [edx + 8], esi
 + 00bc0bd1  0bc6                   or eax, esi
 + 00bc0bd3  894208                 mov dword ptr [edx + 8], eax
   00bc0b41  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0b47  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 441-454 built 486-487
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
 + 00bc0c19  8b350c000000           mov esi, dword ptr [0xc]
   00bc0bb8  e98f000000             jmp 0xbc0c4c
   00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
@@ retail 455-456 built 488-489
   00bc0bb5  897108                 mov dword ptr [ecx + 8], esi
   00bc0bb8  e98f000000             jmp 0xbc0c4c
 - 00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0c21  8b3504000000           mov esi, dword ptr [4]
   00bc0bc3  3bde                   cmp ebx, esi
   00bc0bc5  8bc8                   mov ecx, eax
@@ retail 481-482 built 514-515
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0c84  8b3504000000           mov esi, dword ptr [4]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0c2c  3bde                   cmp ebx, esi
@@ retail 558-559 built 591-592
   00bc0d3f  a804                   test al, 4
   00bc0d41  7429                   je 0xbc0d6c
 - 00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0da7  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
   00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
@@ retail 577-578 built 610-611
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 + 00bc0dde  8b1508000000           mov edx, dword ptr [8]
   00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
   00bc0d86  3bfa                   cmp edi, edx
@@ retail 583-583 built 616-617
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 + 00bc0df6  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
   00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
@@ retail 584-585 built 618-618
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
@@ retail 603-604 built 636-637
   00bc0dd5  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00bc0ddc  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0de2  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0e46  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc0de6  8b4f1c                 mov ecx, dword ptr [edi + 0x1c]
   00bc0de9  8b6f18                 mov ebp, dword ptr [edi + 0x18]
@@ retail 624-625 built 657-658
   00bc0e12  e80972dcff             call 0x988020
   00bc0e17  eb39                   jmp 0xbc0e52
 - 00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0e7d  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
   00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
@@ retail 644-645 built 677-678
   00bc0e48  52                     push edx
   00bc0e49  e8d271dcff             call 0x988020
 - 00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0eb2  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc0e52  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
@@ retail 662-663 built 695-697
   00bc0e81  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc0e87  8d4760                 lea eax, [edi + 0x60]
 - 00bc0e8a  85c0                   test eax, eax
 + 00bc0eee  33ed                   xor ebp, ebp
 + 00bc0ef0  3bc5                   cmp eax, ebp
   00bc0e8c  7418                   je 0xbc0ea6
   00bc0e8e  0fb65810               movzx ebx, byte ptr [eax + 0x10]
@@ retail 669-670 built 703-704
   00bc0e9b  8b11                   mov edx, dword ptr [ecx]
   00bc0e9d  53                     push ebx
 - 00bc0e9e  6a00                   push 0
 + 00bc0f04  55                     push ebp
   00bc0ea0  50                     push eax
   00bc0ea1  6a00                   push 0
@@ retail 671-672 built 705-706
   00bc0e9e  6a00                   push 0
   00bc0ea0  50                     push eax
 - 00bc0ea1  6a00                   push 0
 + 00bc0f06  55                     push ebp
   00bc0ea3  51                     push ecx
   00bc0ea4  eb11                   jmp 0xbc0eb7
@@ retail 676-680 built 710-714
   00bc0ea6  8b81f03c0000           mov eax, dword ptr [ecx + 0x3cf0]
   00bc0eac  8b10                   mov edx, dword ptr [eax]
 - 00bc0eae  6a00                   push 0
 - 00bc0eb0  6a00                   push 0
 - 00bc0eb2  6a00                   push 0
 - 00bc0eb4  6a00                   push 0
 + 00bc0f12  55                     push ebp
 + 00bc0f13  55                     push ebp
 + 00bc0f14  55                     push ebp
 + 00bc0f15  55                     push ebp
   00bc0eb6  50                     push eax
   00bc0eb7  ff9290010000           call dword ptr [edx + 0x190]
@@ retail 684-685 built 718-719
   00bc0ebd  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc0ec3  8d8794000000           lea eax, [edi + 0x94]
 - 00bc0ec9  85c0                   test eax, eax
 + 00bc0f29  3bc5                   cmp eax, ebp
   00bc0ecb  8981f43c0000           mov dword ptr [ecx + 0x3cf4], eax
   00bc0ed1  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
@@ retail 692-693 built 726-727
   00bc0ede  50                     push eax
   00bc0edf  eb02                   jmp 0xbc0ee3
 - 00bc0ee1  6a00                   push 0
 + 00bc0f41  55                     push ebp
   00bc0ee3  51                     push ecx
   00bc0ee4  ff92a0010000           call dword ptr [edx + 0x1a0]
@@ retail 698-698 built 732-733
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 + 00bc0f58  0544010000             add eax, 0x144
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
@@ retail 699-700 built 734-735
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 + 00bc0f63  8b500c                 mov edx, dword ptr [eax + 0xc]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
@@ retail 702-703 built 737-737
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
@@ retail 706-707 built 740-741
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0f7e  8997b4000000           mov dword ptr [edi + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 737-738 built 771-772
   00bc0f9c  d99fd0000000           fstp dword ptr [edi + 0xd0]
   00bc0fa2  6a03                   push 3
 - 00bc0fa4  c787d400000000000000   mov dword ptr [edi + 0xd4], 0
 + 00bc1003  89afd4000000           mov dword ptr [edi + 0xd4], ebp
   00bc0fae  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0fb4  6a01                   push 1
@@ retail 755-757 built 789-791
   00bc0feb  0f85a5020000           jne 0xbc1296
   00bc0ff1  8b4e5c                 mov ecx, dword ptr [esi + 0x5c]
 - 00bc0ff4  6a00                   push 0
 - 00bc0ff6  6a00                   push 0
 + 00bc104f  55                     push ebp
 + 00bc1050  55                     push ebp
   00bc0ff8  51                     push ecx
   00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
@@ retail 758-759 built 792-793
   00bc0ff6  6a00                   push 0
   00bc0ff8  51                     push ecx
 - 00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
 + 00bc1052  8d542420               lea edx, [esp + 0x20]
   00bc0ffd  52                     push edx
   00bc0ffe  8bcb                   mov ecx, ebx
@@ retail 788-789 built 822-822
   00bc1075  eb06                   jmp 0xbc107d
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1081  d9ca                   fxch st(2)
   00bc1083  d8ca                   fmul st(2)
@@ retail 790-790 built 823-824
   00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1081  d9ca                   fxch st(2)
 + 00bc10d8  55                     push ebp
   00bc1083  d8ca                   fmul st(2)
   00bc1085  6a00                   push 0
@@ retail 791-792 built 825-825
   00bc1081  d9ca                   fxch st(2)
   00bc1083  d8ca                   fmul st(2)
 - 00bc1085  6a00                   push 0
   00bc1087  8d4e48                 lea ecx, [esi + 0x48]
   00bc108a  51                     push ecx
@@ retail 796-797 built 829-829
   00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc1091  d95c2428               fstp dword ptr [esp + 0x28]
 - 00bc1095  d944243c               fld dword ptr [esp + 0x3c]
   00bc1099  6a04                   push 4
   00bc109b  d8ca                   fmul st(2)
@@ retail 798-798 built 830-831
   00bc1095  d944243c               fld dword ptr [esp + 0x3c]
   00bc1099  6a04                   push 4
 + 00bc10eb  d9442440               fld dword ptr [esp + 0x40]
   00bc109b  d8ca                   fmul st(2)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
@@ retail 818-819 built 851-852
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc113f  d80c8500000000         fmul dword ptr [eax*4]
   00bc10f2  d9c9                   fxch st(1)
   00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 820-821 built 853-854
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc1148  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 844-845 built 877-878
   00bc1146  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc11a1  d80c8500000000         fmul dword ptr [eax*4]
   00bc1154  d9c9                   fxch st(1)
   00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 846-847 built 879-880
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc11aa  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 862-863 built 895-896
   00bc1183  6a06                   push 6
   00bc1185  e87689dcff             call 0x989b00
 - 00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc11de  8b442414               mov eax, dword ptr [esp + 0x14]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
   00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
@@ retail 864-868 built 897-902
   00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
 - 00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
 - 00bc1196  6a00                   push 0
 - 00bc1198  db442448               fild dword ptr [esp + 0x48]
 - 00bc119c  6a00                   push 0
 + 00bc11e6  55                     push ebp
 + 00bc11e7  894c2448               mov dword ptr [esp + 0x48], ecx
 + 00bc11eb  55                     push ebp
 + 00bc11ec  db44244c               fild dword ptr [esp + 0x4c]
 + 00bc11f0  83ec08                 sub esp, 8
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
@@ retail 870-870 built 904-905
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
 + 00bc11fd  d95c2404               fstp dword ptr [esp + 4]
   00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc11ae  83ec08                 sub esp, 8
@@ retail 871-872 built 906-906
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
   00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc11ae  83ec08                 sub esp, 8
   00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
   00bc11b5  d95c2404               fstp dword ptr [esp + 4]
@@ retail 873-874 built 907-907
   00bc11ae  83ec08                 sub esp, 8
   00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
 - 00bc11b5  d95c2404               fstp dword ptr [esp + 4]
   00bc11b9  db442454               fild dword ptr [esp + 0x54]
   00bc11bd  d80d1cc82901           fmul dword ptr [0x129c81c]
@@ retail 895-896 built 928-929
   00bc1202  8bd0                   mov edx, eax
   00bc1204  8b5804                 mov ebx, dword ptr [eax + 4]
 - 00bc1207  8b28                   mov ebp, dword ptr [eax]
 + 00bc1259  2b18                   sub ebx, dword ptr [eax]
   00bc1209  8b4104                 mov eax, dword ptr [ecx + 4]
   00bc120c  2bdd                   sub ebx, ebp
@@ retail 897-898 built 930-930
   00bc1207  8b28                   mov ebp, dword ptr [eax]
   00bc1209  8b4104                 mov eax, dword ptr [ecx + 4]
 - 00bc120c  2bdd                   sub ebx, ebp
   00bc120e  c1fb04                 sar ebx, 4
   00bc1211  53                     push ebx
@@ retail 940-941 built 972-973
   00bc1283  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc1289  6a02                   push 2
 - 00bc128b  6a00                   push 0
 + 00bc12db  55                     push ebp
   00bc128d  6a06                   push 6
   00bc128f  6a00                   push 0
@@ retail 942-943 built 974-975
   00bc128b  6a00                   push 0
   00bc128d  6a06                   push 6
 - 00bc128f  6a00                   push 0
 + 00bc12de  55                     push ebp
   00bc1291  e8ca99e4ff             call 0xa0ac60
   00bc1296  a19c6e4301             mov eax, dword ptr [0x1436e9c]
@@ retail 972-975 built 1004-1005
   00bc1307  c680e200000000         mov byte ptr [eax + 0xe2], 0
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
 - 00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 + 00bc1363  39a8e8000000           cmp dword ptr [eax + 0xe8], ebp
   00bc131f  7415                   je 0xbc1336
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 978-979 built 1008-1009
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
   00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 + 00bc1374  89a8e8000000           mov dword ptr [eax + 0xe8], ebp
   00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
@@ retail 982-983 built 1012-1014
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
 - 00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00bc138b  33ff                   xor edi, edi
 + 00bc138d  8b742414               mov esi, dword ptr [esp + 0x14]
   00bc1345  3bf7                   cmp esi, edi
   00bc1347  7444                   je 0xbc138d
@@ retail 988-989 built 1019-1020
   00bc134b  393e                   cmp dword ptr [esi], edi
   00bc134d  753e                   jne 0xbc138d
 - 00bc134f  f6461a10               test byte ptr [esi + 0x1a], 0x10
 + 00bc139b  f6461a20               test byte ptr [esi + 0x1a], 0x20
   00bc1353  7407                   je 0xbc135c
   00bc1355  8bce                   mov ecx, esi
@@ retail 1005-1006 built 1036-1036
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
@@ retail 1008-1008 built 1038-1039
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
 + 00bc13ce  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
DIFFLINES 355 retail_instr 1016 built_instr 1047
