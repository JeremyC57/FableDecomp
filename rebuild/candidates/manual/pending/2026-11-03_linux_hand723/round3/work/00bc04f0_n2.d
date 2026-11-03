00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(3780v3751) flags= []
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
 + 00bc04fd  895c2404               mov dword ptr [esp + 4], ebx
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
@@ retail 40-44 built 39-43
   00bc0570  eb02                   jmp 0xbc0574
   00bc0572  33c9                   xor ecx, ecx
 - 00bc0574  57                     push edi
 - 00bc0575  57                     push edi
 - 00bc0576  57                     push edi
 - 00bc0577  8d54241c               lea edx, [esp + 0x1c]
 + 00bc056e  6a00                   push 0
 + 00bc0570  6a00                   push 0
 + 00bc0572  6a00                   push 0
 + 00bc0574  8d542420               lea edx, [esp + 0x20]
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
@@ retail 46-47 built 45-46
   00bc057b  52                     push edx
   00bc057c  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00bc0580  897c2420               mov dword ptr [esp + 0x20], edi
 + 00bc057d  c744242400000000       mov dword ptr [esp + 0x24], 0
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
@@ retail 49-49 built 48-49
   00bc0584  e847f3e3ff             call 0x9ff8d0
   00bc0589  84c0                   test al, al
 + 00bc058c  bf01000000             mov edi, 1
   00bc058b  0f84b00d0000           je 0xbc1341
   00bc0591  a108b53c01             mov eax, dword ptr [0x13cb508]
@@ retail 61-62 built 61-62
   00bc05b7  89b014280000           mov dword ptr [eax + 0x2814], esi
   00bc05bd  8911                   mov dword ptr [ecx], edx
 - 00bc05bf  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc05c5  8b442414               mov eax, dword ptr [esp + 0x14]
   00bc05c3  8b4808                 mov ecx, dword ptr [eax + 8]
   00bc05c6  3bcf                   cmp ecx, edi
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
@@ retail 80-83 built 80-81
   00bc05f0  ff9204010000           call dword ptr [edx + 0x104]
   00bc05f6  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc05fb  8b8eb83c0000           mov ecx, dword ptr [esi + 0x3cb8]
 - 00bc0601  bf01000000             mov edi, 1
 - 00bc0606  3bcf                   cmp ecx, edi
 + 00bc0602  39beb83c0000           cmp dword ptr [esi + 0x3cb8], edi
   00bc0608  7d0b                   jge 0xbc0615
   00bc060a  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
@@ retail 121-122 built 119-120
   00bc069a  ff820c280000           inc dword ptr [edx + 0x280c]
   00bc06a0  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc06a5  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc06a5  8b3510000000           mov esi, dword ptr [0x10]
   00bc06ab  39b020290000           cmp dword ptr [eax + 0x2920], esi
   00bc06b1  8d901c290000           lea edx, [eax + 0x291c]
@@ retail 148-149 built 146-147
   00bc0707  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc070d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0712  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0712  8b3514000000           mov esi, dword ptr [0x14]
   00bc0718  39b034290000           cmp dword ptr [eax + 0x2934], esi
   00bc071e  8d9030290000           lea edx, [eax + 0x2930]
@@ retail 178-178 built 176-177
   00bc0785  f6c104                 test cl, 4
   00bc0788  0f8499010000           je 0xbc0927
 + 00bc078a  6a00                   push 0
   00bc078e  8d4b40                 lea ecx, [ebx + 0x40]
   00bc0791  6a00                   push 0
@@ retail 179-179 built 178-209
   00bc0788  0f8499010000           je 0xbc0927
   00bc078e  8d4b40                 lea ecx, [ebx + 0x40]
 + 00bc078f  51                     push ecx
 + 00bc0790  eb7d                   jmp 0xbc080f
 + 00bc0792  f6c110                 test cl, 0x10
 + 00bc0795  7472                   je 0xbc0809
 + 00bc0797  8b3508000000           mov esi, dword ptr [8]
 + 00bc079d  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
 + 00bc07a3  3bee                   cmp ebp, esi
 + 00bc07a5  8d8890280000           lea ecx, [eax + 0x2890]
 + 00bc07ab  8bd0                   mov edx, eax
 + 00bc07ad  8bfe                   mov edi, esi
 + 00bc07af  7450                   je 0xbc0801
 + 00bc07b1  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc07b7  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc07ba  85f0                   test eax, esi
 + 00bc07bc  7522                   jne 0xbc07e0
 + 00bc07be  0bf0                   or esi, eax
 + 00bc07c0  897108                 mov dword ptr [ecx + 8], esi
 + 00bc07c3  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc07c9  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
 + 00bc07cd  8b8208280000           mov eax, dword ptr [edx + 0x2808]
 + 00bc07d3  8b7104                 mov esi, dword ptr [ecx + 4]
 + 00bc07d6  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
 + 00bc07da  ff8208280000           inc dword ptr [edx + 0x2808]
 + 00bc07e0  8a4110                 mov al, byte ptr [ecx + 0x10]
 + 00bc07e3  84c0                   test al, al
 + 00bc07e5  897904                 mov dword ptr [ecx + 4], edi
 + 00bc07e8  7517                   jne 0xbc0801
 + 00bc07ea  c6411001               mov byte ptr [ecx + 0x10], 1
 + 00bc07ee  8b820c280000           mov eax, dword ptr [edx + 0x280c]
 + 00bc07f4  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
 + 00bc07fb  ff820c280000           inc dword ptr [edx + 0x280c]
   00bc0791  6a00                   push 0
   00bc0793  51                     push ecx
@@ retail 180-180 built 210-211
   00bc078e  8d4b40                 lea ecx, [ebx + 0x40]
   00bc0791  6a00                   push 0
 + 00bc0803  8d4b38                 lea ecx, [ebx + 0x38]
   00bc0793  51                     push ecx
   00bc0794  eb06                   jmp 0xbc079c
@@ retail 187-188 built 218-218
   00bc079c  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc07a2  e87978dcff             call 0x988020
 - 00bc07a7  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc07ad  83c358                 add ebx, 0x58
   00bc07b0  53                     push ebx
@@ retail 190-190 built 220-221
   00bc07ad  83c358                 add ebx, 0x58
   00bc07b0  53                     push ebx
 + 00bc081e  8b0d00000000           mov ecx, dword ptr [0]
   00bc07b1  e88a79dcff             call 0x988140
   00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
@@ retail 191-192 built 222-223
   00bc07b0  53                     push ebx
   00bc07b1  e88a79dcff             call 0x988140
 - 00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0829  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc07ba  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc07be  e9be060000             jmp 0xbc0e81
@@ retail 225-227 built 256-258
   00bc0838  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
   00bc083e  8d90e0280000           lea edx, [eax + 0x28e0]
 - 00bc0844  bf10000000             mov edi, 0x10
 - 00bc0849  3bf7                   cmp esi, edi
 + 00bc08b7  bb10000000             mov ebx, 0x10
 + 00bc08bc  3bf3                   cmp esi, ebx
   00bc084b  8bc8                   mov ecx, eax
   00bc084d  7455                   je 0xbc08a4
@@ retail 243-244 built 274-275
   00bc087e  8a4210                 mov al, byte ptr [edx + 0x10]
   00bc0881  84c0                   test al, al
 - 00bc0883  897a04                 mov dword ptr [edx + 4], edi
 + 00bc08f6  895a04                 mov dword ptr [edx + 4], ebx
   00bc0886  7517                   jne 0xbc089f
   00bc0888  c6421001               mov byte ptr [edx + 0x10], 1
@@ retail 250-251 built 281-282
   00bc0899  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc089f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc08a4  8b35e86d3901           mov esi, dword ptr [0x1396de8]
 + 00bc0917  8b3510000000           mov esi, dword ptr [0x10]
   00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
   00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
@@ retail 258-259 built 289-290
   00bc08bc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc08c2  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc08c5  85f0                   test eax, esi
 + 00bc0938  85c6                   test esi, eax
   00bc08c7  7522                   jne 0xbc08eb
   00bc08c9  0bf0                   or esi, eax
@@ retail 260-262 built 291-293
   00bc08c5  85f0                   test eax, esi
   00bc08c7  7522                   jne 0xbc08eb
 - 00bc08c9  0bf0                   or esi, eax
 - 00bc08cb  897208                 mov dword ptr [edx + 8], esi
 + 00bc093c  0bc6                   or eax, esi
 + 00bc093e  894208                 mov dword ptr [edx + 8], eax
   00bc08ce  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc08d4  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 280-280 built 311-312
   00bc0917  f6c104                 test cl, 4
   00bc091a  740b                   je 0xbc0927
 + 00bc098f  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc091c  6a00                   push 0
   00bc091e  8d4b40                 lea ecx, [ebx + 0x40]
@@ retail 281-282 built 313-314
   00bc091a  740b                   je 0xbc0927
   00bc091c  6a00                   push 0
 - 00bc091e  8d4b40                 lea ecx, [ebx + 0x40]
 + 00bc0995  83c140                 add ecx, 0x40
   00bc0921  51                     push ecx
   00bc0922  e975feffff             jmp 0xbc079c
@@ retail 284-285 built 316-317
   00bc0921  51                     push ecx
   00bc0922  e975feffff             jmp 0xbc079c
 - 00bc0927  f6c110                 test cl, 0x10
 + 00bc099e  84cb                   test bl, cl
   00bc092a  0f8466feffff           je 0xbc0796
   00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
@@ retail 286-289 built 318-321
   00bc0927  f6c110                 test cl, 0x10
   00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 - 00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
 - 00bc093c  3bee                   cmp ebp, esi
 + 00bc09a2  8b3508000000           mov esi, dword ptr [8]
 + 00bc09a8  8b9894280000           mov ebx, dword ptr [eax + 0x2894]
 + 00bc09ae  3bde                   cmp ebx, esi
   00bc093e  8d8890280000           lea ecx, [eax + 0x2890]
   00bc0944  8bd0                   mov edx, eax
@@ retail 293-293 built 325-326
   00bc0946  8bfe                   mov edi, esi
   00bc0948  7450                   je 0xbc099a
 + 00bc09bc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
   00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 294-296 built 327-328
   00bc0948  7450                   je 0xbc099a
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0953  85f0                   test eax, esi
 + 00bc09c5  85c6                   test esi, eax
   00bc0955  7522                   jne 0xbc0979
   00bc0957  0bf0                   or esi, eax
@@ retail 297-299 built 329-331
   00bc0953  85f0                   test eax, esi
   00bc0955  7522                   jne 0xbc0979
 - 00bc0957  0bf0                   or esi, eax
 - 00bc0959  897108                 mov dword ptr [ecx + 8], esi
 + 00bc09c9  0bc6                   or eax, esi
 + 00bc09cb  894108                 mov dword ptr [ecx + 8], eax
   00bc095c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0962  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 305-306 built 337-337
   00bc096f  8974c20c               mov dword ptr [edx + eax*8 + 0xc], esi
   00bc0973  ff8208280000           inc dword ptr [edx + 0x2808]
 - 00bc0979  897904                 mov dword ptr [ecx + 4], edi
   00bc097c  8a4110                 mov al, byte ptr [ecx + 0x10]
   00bc097f  84c0                   test al, al
@@ retail 308-308 built 339-340
   00bc097c  8a4110                 mov al, byte ptr [ecx + 0x10]
   00bc097f  84c0                   test al, al
 + 00bc09f0  897904                 mov dword ptr [ecx + 4], edi
   00bc0981  7517                   jne 0xbc099a
   00bc0983  c6411001               mov byte ptr [ecx + 0x10], 1
@@ retail 313-314 built 345-346
   00bc098d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
   00bc0994  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc099a  8d4b38                 lea ecx, [ebx + 0x38]
 + 00bc0a0c  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc099d  6a00                   push 0
   00bc099f  51                     push ecx
@@ retail 315-315 built 347-348
   00bc099a  8d4b38                 lea ecx, [ebx + 0x38]
   00bc099d  6a00                   push 0
 + 00bc0a12  83c138                 add ecx, 0x38
   00bc099f  51                     push ecx
   00bc09a0  e9f7fdffff             jmp 0xbc079c
@@ retail 316-316 built 349-359
   00bc099d  6a00                   push 0
   00bc099f  51                     push ecx
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
@@ retail 326-327 built 369-370
   00bc09be  8bc8                   mov ecx, eax
   00bc09c0  7510                   jne 0xbc09d2
 - 00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
 + 00bc0a57  8b3508000000           mov esi, dword ptr [8]
   00bc09c8  eb08                   jmp 0xbc09d2
   00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
@@ retail 328-329 built 371-372
   00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
   00bc09c8  eb08                   jmp 0xbc09d2
 - 00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
 + 00bc0a5f  8b3504000000           mov esi, dword ptr [4]
   00bc09d0  8bc8                   mov ecx, eax
   00bc09d2  3bde                   cmp ebx, esi
@@ retail 333-333 built 376-377
   00bc09d4  8bfe                   mov edi, esi
   00bc09d6  7455                   je 0xbc0a2d
 + 00bc0a6d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
   00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 334-336 built 378-379
   00bc09d6  7455                   je 0xbc0a2d
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc09e1  85f0                   test eax, esi
 + 00bc0a76  85c6                   test esi, eax
   00bc09e3  7522                   jne 0xbc0a07
   00bc09e5  0bf0                   or esi, eax
@@ retail 337-339 built 380-382
   00bc09e1  85f0                   test eax, esi
   00bc09e3  7522                   jne 0xbc0a07
 - 00bc09e5  0bf0                   or esi, eax
 - 00bc09e7  897208                 mov dword ptr [edx + 8], esi
 + 00bc0a7a  0bc6                   or eax, esi
 + 00bc0a7c  894208                 mov dword ptr [edx + 8], eax
   00bc09ea  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc09f0  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 362-363 built 405-406
   00bc0a46  8bc8                   mov ecx, eax
   00bc0a48  0f84d5000000           je 0xbc0b23
 - 00bc0a4e  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc0ae3  8b3510000000           mov esi, dword ptr [0x10]
   00bc0a54  3bde                   cmp ebx, esi
   00bc0a56  8bfe                   mov edi, esi
@@ retail 368-369 built 411-412
   00bc0a5a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0a60  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0a63  85f0                   test eax, esi
 + 00bc0af8  85c6                   test esi, eax
   00bc0a65  7522                   jne 0xbc0a89
   00bc0a67  0bf0                   or esi, eax
@@ retail 370-372 built 413-415
   00bc0a63  85f0                   test eax, esi
   00bc0a65  7522                   jne 0xbc0a89
 - 00bc0a67  0bf0                   or esi, eax
 - 00bc0a69  897208                 mov dword ptr [edx + 8], esi
 + 00bc0afc  0bc6                   or eax, esi
 + 00bc0afe  894208                 mov dword ptr [edx + 8], eax
   00bc0a6c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0a72  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 387-388 built 430-431
   00bc0aa4  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0aaa  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0aaf  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0b44  8b3514000000           mov esi, dword ptr [0x14]
   00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0abb  3bde                   cmp ebx, esi
@@ retail 396-397 built 439-440
   00bc0ac9  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0acf  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0ad2  85f0                   test eax, esi
 + 00bc0b67  85c6                   test esi, eax
   00bc0ad4  7522                   jne 0xbc0af8
   00bc0ad6  0bf0                   or esi, eax
@@ retail 398-400 built 441-443
   00bc0ad2  85f0                   test eax, esi
   00bc0ad4  7522                   jne 0xbc0af8
 - 00bc0ad6  0bf0                   or esi, eax
 - 00bc0ad8  897108                 mov dword ptr [ecx + 8], esi
 + 00bc0b6b  0bc6                   or eax, esi
 + 00bc0b6d  894108                 mov dword ptr [ecx + 8], eax
   00bc0adb  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0ae1  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 416-417 built 459-460
   00bc0b19  c644244400             mov byte ptr [esp + 0x44], 0
   00bc0b1e  e96c010000             jmp 0xbc0c8f
 - 00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0bb8  8b3504000000           mov esi, dword ptr [4]
   00bc0b29  3bde                   cmp ebx, esi
   00bc0b2b  8bfe                   mov edi, esi
@@ retail 422-423 built 465-466
   00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0b35  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0b38  85f0                   test eax, esi
 + 00bc0bcd  85c6                   test esi, eax
   00bc0b3a  7522                   jne 0xbc0b5e
   00bc0b3c  0bf0                   or esi, eax
@@ retail 424-426 built 467-469
   00bc0b38  85f0                   test eax, esi
   00bc0b3a  7522                   jne 0xbc0b5e
 - 00bc0b3c  0bf0                   or esi, eax
 - 00bc0b3e  897208                 mov dword ptr [edx + 8], esi
 + 00bc0bd1  0bc6                   or eax, esi
 + 00bc0bd3  894208                 mov dword ptr [edx + 8], eax
   00bc0b41  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0b47  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 441-454 built 484-485
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
@@ retail 455-456 built 486-487
   00bc0bb5  897108                 mov dword ptr [ecx + 8], esi
   00bc0bb8  e98f000000             jmp 0xbc0c4c
 - 00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0c21  8b3504000000           mov esi, dword ptr [4]
   00bc0bc3  3bde                   cmp ebx, esi
   00bc0bc5  8bc8                   mov ecx, eax
@@ retail 481-482 built 512-513
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0c84  8b3504000000           mov esi, dword ptr [4]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0c2c  3bde                   cmp ebx, esi
@@ retail 558-559 built 589-590
   00bc0d3f  a804                   test al, 4
   00bc0d41  7429                   je 0xbc0d6c
 - 00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0da7  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
   00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
@@ retail 577-578 built 608-609
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 + 00bc0dde  8b1508000000           mov edx, dword ptr [8]
   00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
   00bc0d86  3bfa                   cmp edi, edx
@@ retail 583-583 built 614-615
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 + 00bc0df6  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
   00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
@@ retail 584-585 built 616-616
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
@@ retail 603-604 built 634-635
   00bc0dd5  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00bc0ddc  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0de2  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0e46  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc0de6  8b4f1c                 mov ecx, dword ptr [edi + 0x1c]
   00bc0de9  8b6f18                 mov ebp, dword ptr [edi + 0x18]
@@ retail 624-625 built 655-656
   00bc0e12  e80972dcff             call 0x988020
   00bc0e17  eb39                   jmp 0xbc0e52
 - 00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0e7d  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
   00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
@@ retail 644-645 built 675-676
   00bc0e48  52                     push edx
   00bc0e49  e8d271dcff             call 0x988020
 - 00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0eb2  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc0e52  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
@@ retail 662-663 built 693-695
   00bc0e81  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc0e87  8d4760                 lea eax, [edi + 0x60]
 - 00bc0e8a  85c0                   test eax, eax
 + 00bc0eee  33ed                   xor ebp, ebp
 + 00bc0ef0  3bc5                   cmp eax, ebp
   00bc0e8c  7418                   je 0xbc0ea6
   00bc0e8e  0fb65810               movzx ebx, byte ptr [eax + 0x10]
@@ retail 669-670 built 701-702
   00bc0e9b  8b11                   mov edx, dword ptr [ecx]
   00bc0e9d  53                     push ebx
 - 00bc0e9e  6a00                   push 0
 + 00bc0f04  55                     push ebp
   00bc0ea0  50                     push eax
   00bc0ea1  6a00                   push 0
@@ retail 671-672 built 703-704
   00bc0e9e  6a00                   push 0
   00bc0ea0  50                     push eax
 - 00bc0ea1  6a00                   push 0
 + 00bc0f06  55                     push ebp
   00bc0ea3  51                     push ecx
   00bc0ea4  eb11                   jmp 0xbc0eb7
@@ retail 676-680 built 708-712
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
@@ retail 684-685 built 716-717
   00bc0ebd  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc0ec3  8d8794000000           lea eax, [edi + 0x94]
 - 00bc0ec9  85c0                   test eax, eax
 + 00bc0f29  3bc5                   cmp eax, ebp
   00bc0ecb  8981f43c0000           mov dword ptr [ecx + 0x3cf4], eax
   00bc0ed1  8b89f03c0000           mov ecx, dword ptr [ecx + 0x3cf0]
@@ retail 692-693 built 724-725
   00bc0ede  50                     push eax
   00bc0edf  eb02                   jmp 0xbc0ee3
 - 00bc0ee1  6a00                   push 0
 + 00bc0f41  55                     push ebp
   00bc0ee3  51                     push ecx
   00bc0ee4  ff92a0010000           call dword ptr [edx + 0x1a0]
@@ retail 698-698 built 730-731
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 + 00bc0f58  0544010000             add eax, 0x144
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
@@ retail 699-700 built 732-733
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 + 00bc0f63  8b500c                 mov edx, dword ptr [eax + 0xc]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
@@ retail 702-703 built 735-735
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
@@ retail 706-707 built 738-739
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0f7e  8997b4000000           mov dword ptr [edi + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 737-738 built 769-770
   00bc0f9c  d99fd0000000           fstp dword ptr [edi + 0xd0]
   00bc0fa2  6a03                   push 3
 - 00bc0fa4  c787d400000000000000   mov dword ptr [edi + 0xd4], 0
 + 00bc1003  89afd4000000           mov dword ptr [edi + 0xd4], ebp
   00bc0fae  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc0fb4  6a01                   push 1
@@ retail 755-757 built 787-789
   00bc0feb  0f85a5020000           jne 0xbc1296
   00bc0ff1  8b4e5c                 mov ecx, dword ptr [esi + 0x5c]
 - 00bc0ff4  6a00                   push 0
 - 00bc0ff6  6a00                   push 0
 + 00bc104f  55                     push ebp
 + 00bc1050  55                     push ebp
   00bc0ff8  51                     push ecx
   00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
@@ retail 758-759 built 790-791
   00bc0ff6  6a00                   push 0
   00bc0ff8  51                     push ecx
 - 00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
 + 00bc1052  8d542420               lea edx, [esp + 0x20]
   00bc0ffd  52                     push edx
   00bc0ffe  8bcb                   mov ecx, ebx
@@ retail 788-789 built 820-820
   00bc1075  eb06                   jmp 0xbc107d
   00bc1077  d905d8de2201           fld dword ptr [0x122ded8]
 - 00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1081  d9ca                   fxch st(2)
   00bc1083  d8ca                   fmul st(2)
@@ retail 790-790 built 821-822
   00bc107d  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc1081  d9ca                   fxch st(2)
 + 00bc10d8  55                     push ebp
   00bc1083  d8ca                   fmul st(2)
   00bc1085  6a00                   push 0
@@ retail 791-792 built 823-823
   00bc1081  d9ca                   fxch st(2)
   00bc1083  d8ca                   fmul st(2)
 - 00bc1085  6a00                   push 0
   00bc1087  8d4e48                 lea ecx, [esi + 0x48]
   00bc108a  51                     push ecx
@@ retail 796-797 built 827-827
   00bc108b  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc1091  d95c2428               fstp dword ptr [esp + 0x28]
 - 00bc1095  d944243c               fld dword ptr [esp + 0x3c]
   00bc1099  6a04                   push 4
   00bc109b  d8ca                   fmul st(2)
@@ retail 798-798 built 828-829
   00bc1095  d944243c               fld dword ptr [esp + 0x3c]
   00bc1099  6a04                   push 4
 + 00bc10eb  d9442440               fld dword ptr [esp + 0x40]
   00bc109b  d8ca                   fmul st(2)
   00bc109d  d95c2430               fstp dword ptr [esp + 0x30]
@@ retail 818-819 built 849-850
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc113f  d80c8500000000         fmul dword ptr [eax*4]
   00bc10f2  d9c9                   fxch st(1)
   00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 820-821 built 851-852
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc1148  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 844-845 built 875-876
   00bc1146  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc11a1  d80c8500000000         fmul dword ptr [eax*4]
   00bc1154  d9c9                   fxch st(1)
   00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 846-847 built 877-878
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc11aa  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 862-863 built 893-894
   00bc1183  6a06                   push 6
   00bc1185  e87689dcff             call 0x989b00
 - 00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc11de  8b442414               mov eax, dword ptr [esp + 0x14]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
   00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
@@ retail 864-868 built 895-900
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
@@ retail 870-870 built 902-903
   00bc119e  d80d1cc82901           fmul dword ptr [0x129c81c]
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
 + 00bc11fd  d95c2404               fstp dword ptr [esp + 4]
   00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00bc11ae  83ec08                 sub esp, 8
@@ retail 871-872 built 904-904
   00bc11a4  0fb75010               movzx edx, word ptr [eax + 0x10]
   00bc11a8  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc11ae  83ec08                 sub esp, 8
   00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
   00bc11b5  d95c2404               fstp dword ptr [esp + 4]
@@ retail 873-874 built 905-905
   00bc11ae  83ec08                 sub esp, 8
   00bc11b1  89542454               mov dword ptr [esp + 0x54], edx
 - 00bc11b5  d95c2404               fstp dword ptr [esp + 4]
   00bc11b9  db442454               fild dword ptr [esp + 0x54]
   00bc11bd  d80d1cc82901           fmul dword ptr [0x129c81c]
@@ retail 895-896 built 926-927
   00bc1202  8bd0                   mov edx, eax
   00bc1204  8b5804                 mov ebx, dword ptr [eax + 4]
 - 00bc1207  8b28                   mov ebp, dword ptr [eax]
 + 00bc1259  2b18                   sub ebx, dword ptr [eax]
   00bc1209  8b4104                 mov eax, dword ptr [ecx + 4]
   00bc120c  2bdd                   sub ebx, ebp
@@ retail 897-898 built 928-928
   00bc1207  8b28                   mov ebp, dword ptr [eax]
   00bc1209  8b4104                 mov eax, dword ptr [ecx + 4]
 - 00bc120c  2bdd                   sub ebx, ebp
   00bc120e  c1fb04                 sar ebx, 4
   00bc1211  53                     push ebx
@@ retail 940-941 built 970-971
   00bc1283  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
   00bc1289  6a02                   push 2
 - 00bc128b  6a00                   push 0
 + 00bc12db  55                     push ebp
   00bc128d  6a06                   push 6
   00bc128f  6a00                   push 0
@@ retail 942-943 built 972-973
   00bc128b  6a00                   push 0
   00bc128d  6a06                   push 6
 - 00bc128f  6a00                   push 0
 + 00bc12de  55                     push ebp
   00bc1291  e8ca99e4ff             call 0xa0ac60
   00bc1296  a19c6e4301             mov eax, dword ptr [0x1436e9c]
@@ retail 972-975 built 1002-1003
   00bc1307  c680e200000000         mov byte ptr [eax + 0xe2], 0
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
 - 00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 + 00bc1363  39a8e8000000           cmp dword ptr [eax + 0xe8], ebp
   00bc131f  7415                   je 0xbc1336
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 978-979 built 1006-1007
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
   00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 + 00bc1374  89a8e8000000           mov dword ptr [eax + 0xe8], ebp
   00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
@@ retail 982-984 built 1010-1013
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
 - 00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 - 00bc1345  3bf7                   cmp esi, edi
 + 00bc138b  bf01000000             mov edi, 1
 + 00bc1390  8b442414               mov eax, dword ptr [esp + 0x14]
 + 00bc1394  85c0                   test eax, eax
   00bc1347  7444                   je 0xbc138d
   00bc1349  ff0e                   dec dword ptr [esi]
@@ retail 985-987 built 1014-1019
   00bc1345  3bf7                   cmp esi, edi
   00bc1347  7444                   je 0xbc138d
 - 00bc1349  ff0e                   dec dword ptr [esi]
 - 00bc134b  393e                   cmp dword ptr [esi], edi
 + 00bc1398  8b10                   mov edx, dword ptr [eax]
 + 00bc139a  4a                     dec edx
 + 00bc139b  8bc8                   mov ecx, eax
 + 00bc139d  8910                   mov dword ptr [eax], edx
 + 00bc139f  833900                 cmp dword ptr [ecx], 0
   00bc134d  753e                   jne 0xbc138d
   00bc134f  f6461a10               test byte ptr [esi + 0x1a], 0x10
@@ retail 988-991 built 1020-1021
   00bc134b  393e                   cmp dword ptr [esi], edi
   00bc134d  753e                   jne 0xbc138d
 - 00bc134f  f6461a10               test byte ptr [esi + 0x1a], 0x10
 - 00bc1353  7407                   je 0xbc135c
 - 00bc1355  8bce                   mov ecx, esi
 + 00bc13a4  57                     push edi
   00bc1357  e844b8e3ff             call 0x9fcba0
   00bc135c  8b460c                 mov eax, dword ptr [esi + 0xc]
@@ retail 992-1010 built 1022-1023
   00bc1355  8bce                   mov ecx, esi
   00bc1357  e844b8e3ff             call 0x9fcba0
 - 00bc135c  8b460c                 mov eax, dword ptr [esi + 0xc]
 - 00bc135f  3bc7                   cmp eax, edi
 - 00bc1361  741b                   je 0xbc137e
 - 00bc1363  ff08                   dec dword ptr [eax]
 - 00bc1365  8b460c                 mov eax, dword ptr [esi + 0xc]
 - 00bc1368  3938                   cmp dword ptr [eax], edi
 - 00bc136a  7512                   jne 0xbc137e
 - 00bc136c  8b4808                 mov ecx, dword ptr [eax + 8]
 - 00bc136f  ff5004                 call dword ptr [eax + 4]
 - 00bc1372  8b460c                 mov eax, dword ptr [esi + 0xc]
 - 00bc1375  50                     push eax
 - 00bc1376  e841d60300             call 0xbfe9bc
 - 00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
 - 00bc1381  56                     push esi
 - 00bc1382  897e08                 mov dword ptr [esi + 8], edi
 - 00bc1385  e832d60300             call 0xbfe9bc
 - 00bc138a  83c404                 add esp, 4
 + 00bc13aa  5f                     pop edi
   00bc138d  5e                     pop esi
   00bc138e  5d                     pop ebp
@@ retail 1012-1013 built 1025-1025
   00bc138d  5e                     pop esi
   00bc138e  5d                     pop ebp
 - 00bc138f  5f                     pop edi
   00bc1390  5b                     pop ebx
   00bc1391  83c430                 add esp, 0x30
DIFFLINES 215 retail_instr 1016 built_instr 1028
