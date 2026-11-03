00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(3786v3751) flags= []
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
@@ retail 9-10 built 7-8
   00bc0500  895c240c               mov dword ptr [esp + 0xc], ebx
   00bc0504  0f84850e0000           je 0xbc138f
 - 00bc050a  8b44243c               mov eax, dword ptr [esp + 0x3c]
 + 00bc0507  8b442438               mov eax, dword ptr [esp + 0x38]
   00bc050e  8b0d986e4301           mov ecx, dword ptr [0x1436e98]
   00bc0514  8b8954090000           mov ecx, dword ptr [ecx + 0x954]
@@ retail 16-17 built 14-15
   00bc051e  8b5554                 mov edx, dword ptr [ebp + 0x54]
   00bc0521  52                     push edx
 - 00bc0522  896c2418               mov dword ptr [esp + 0x18], ebp
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
@@ retail 94-95 built 94-96
   00bc062d  0f8472030000           je 0xbc09a5
   00bc0633  8b35d06e3901           mov esi, dword ptr [0x1396ed0]
 - 00bc0639  39b0402c0000           cmp dword ptr [eax + 0x2c40], esi
 + 00bc0638  8b98402c0000           mov ebx, dword ptr [eax + 0x2c40]
 + 00bc063e  3bde                   cmp ebx, esi
   00bc063f  8d883c2c0000           lea ecx, [eax + 0x2c3c]
   00bc0645  c644244400             mov byte ptr [esp + 0x44], 0
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
@@ retail 191-192 built 213-214
   00bc07b0  53                     push ebx
   00bc07b1  e88a79dcff             call 0x988140
 - 00bc07b6  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0801  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc07ba  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc07be  e9be060000             jmp 0xbc0e81
@@ retail 204-205 built 226-227
   00bc07e3  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc07e9  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc07ec  85f0                   test eax, esi
 + 00bc0837  85c6                   test esi, eax
   00bc07ee  7522                   jne 0xbc0812
   00bc07f0  0bf0                   or esi, eax
@@ retail 206-208 built 228-230
   00bc07ec  85f0                   test eax, esi
   00bc07ee  7522                   jne 0xbc0812
 - 00bc07f0  0bf0                   or esi, eax
 - 00bc07f2  897208                 mov dword ptr [edx + 8], esi
 + 00bc083b  0bc6                   or eax, esi
 + 00bc083d  894208                 mov dword ptr [edx + 8], eax
   00bc07f5  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc07fb  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
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
@@ retail 231-232 built 252-253
   00bc084f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0855  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0858  85f0                   test eax, esi
 + 00bc089e  85c6                   test esi, eax
   00bc085a  7522                   jne 0xbc087e
   00bc085c  0bf0                   or esi, eax
@@ retail 233-235 built 254-256
   00bc0858  85f0                   test eax, esi
   00bc085a  7522                   jne 0xbc087e
 - 00bc085c  0bf0                   or esi, eax
 - 00bc085e  897208                 mov dword ptr [edx + 8], esi
 + 00bc08a2  0bc6                   or eax, esi
 + 00bc08a4  894208                 mov dword ptr [edx + 8], eax
   00bc0861  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0867  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
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
@@ retail 258-259 built 280-281
   00bc08bc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc08c2  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc08c5  85f0                   test eax, esi
 + 00bc090d  85c6                   test esi, eax
   00bc08c7  7522                   jne 0xbc08eb
   00bc08c9  0bf0                   or esi, eax
@@ retail 260-262 built 282-284
   00bc08c5  85f0                   test eax, esi
   00bc08c7  7522                   jne 0xbc08eb
 - 00bc08c9  0bf0                   or esi, eax
 - 00bc08cb  897208                 mov dword ptr [edx + 8], esi
 + 00bc0911  0bc6                   or eax, esi
 + 00bc0913  894208                 mov dword ptr [edx + 8], eax
   00bc08ce  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc08d4  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 280-280 built 302-303
   00bc0917  f6c104                 test cl, 4
   00bc091a  740b                   je 0xbc0927
 + 00bc0964  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc091c  6a00                   push 0
   00bc091e  8d4b40                 lea ecx, [ebx + 0x40]
@@ retail 281-282 built 304-305
   00bc091a  740b                   je 0xbc0927
   00bc091c  6a00                   push 0
 - 00bc091e  8d4b40                 lea ecx, [ebx + 0x40]
 + 00bc096a  83c140                 add ecx, 0x40
   00bc0921  51                     push ecx
   00bc0922  e975feffff             jmp 0xbc079c
@@ retail 286-289 built 309-312
   00bc0927  f6c110                 test cl, 0x10
   00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 - 00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
 - 00bc093c  3bee                   cmp ebp, esi
 + 00bc097c  8b3508000000           mov esi, dword ptr [8]
 + 00bc0982  8b9894280000           mov ebx, dword ptr [eax + 0x2894]
 + 00bc0988  3bde                   cmp ebx, esi
   00bc093e  8d8890280000           lea ecx, [eax + 0x2890]
   00bc0944  8bd0                   mov edx, eax
@@ retail 293-293 built 316-317
   00bc0946  8bfe                   mov edi, esi
   00bc0948  7450                   je 0xbc099a
 + 00bc0996  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
   00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 294-296 built 318-319
   00bc0948  7450                   je 0xbc099a
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0953  85f0                   test eax, esi
 + 00bc099f  85c6                   test esi, eax
   00bc0955  7522                   jne 0xbc0979
   00bc0957  0bf0                   or esi, eax
@@ retail 297-299 built 320-322
   00bc0953  85f0                   test eax, esi
   00bc0955  7522                   jne 0xbc0979
 - 00bc0957  0bf0                   or esi, eax
 - 00bc0959  897108                 mov dword ptr [ecx + 8], esi
 + 00bc09a3  0bc6                   or eax, esi
 + 00bc09a5  894108                 mov dword ptr [ecx + 8], eax
   00bc095c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0962  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 313-314 built 336-338
   00bc098d  898c8208200000         mov dword ptr [edx + eax*4 + 0x2008], ecx
   00bc0994  ff820c280000           inc dword ptr [edx + 0x280c]
 - 00bc099a  8d4b38                 lea ecx, [ebx + 0x38]
 + 00bc09e6  8b4c2410               mov ecx, dword ptr [esp + 0x10]
 + 00bc09ea  83c138                 add ecx, 0x38
   00bc099d  6a00                   push 0
   00bc099f  51                     push ecx
@@ retail 326-327 built 350-351
   00bc09be  8bc8                   mov ecx, eax
   00bc09c0  7510                   jne 0xbc09d2
 - 00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
 + 00bc0a12  8b3508000000           mov esi, dword ptr [8]
   00bc09c8  eb08                   jmp 0xbc09d2
   00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
@@ retail 328-329 built 352-353
   00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
   00bc09c8  eb08                   jmp 0xbc09d2
 - 00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
 + 00bc0a1a  8b3504000000           mov esi, dword ptr [4]
   00bc09d0  8bc8                   mov ecx, eax
   00bc09d2  3bde                   cmp ebx, esi
@@ retail 333-333 built 357-358
   00bc09d4  8bfe                   mov edi, esi
   00bc09d6  7455                   je 0xbc0a2d
 + 00bc0a28  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
   00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 334-336 built 359-360
   00bc09d6  7455                   je 0xbc0a2d
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc09e1  85f0                   test eax, esi
 + 00bc0a31  85c6                   test esi, eax
   00bc09e3  7522                   jne 0xbc0a07
   00bc09e5  0bf0                   or esi, eax
@@ retail 337-339 built 361-363
   00bc09e1  85f0                   test eax, esi
   00bc09e3  7522                   jne 0xbc0a07
 - 00bc09e5  0bf0                   or esi, eax
 - 00bc09e7  897208                 mov dword ptr [edx + 8], esi
 + 00bc0a35  0bc6                   or eax, esi
 + 00bc0a37  894208                 mov dword ptr [edx + 8], eax
   00bc09ea  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc09f0  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 362-363 built 386-387
   00bc0a46  8bc8                   mov ecx, eax
   00bc0a48  0f84d5000000           je 0xbc0b23
 - 00bc0a4e  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc0a9e  8b3510000000           mov esi, dword ptr [0x10]
   00bc0a54  3bde                   cmp ebx, esi
   00bc0a56  8bfe                   mov edi, esi
@@ retail 368-369 built 392-393
   00bc0a5a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0a60  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0a63  85f0                   test eax, esi
 + 00bc0ab3  85c6                   test esi, eax
   00bc0a65  7522                   jne 0xbc0a89
   00bc0a67  0bf0                   or esi, eax
@@ retail 370-372 built 394-396
   00bc0a63  85f0                   test eax, esi
   00bc0a65  7522                   jne 0xbc0a89
 - 00bc0a67  0bf0                   or esi, eax
 - 00bc0a69  897208                 mov dword ptr [edx + 8], esi
 + 00bc0ab7  0bc6                   or eax, esi
 + 00bc0ab9  894208                 mov dword ptr [edx + 8], eax
   00bc0a6c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0a72  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 387-388 built 411-412
   00bc0aa4  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0aaa  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0aaf  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0aff  8b3514000000           mov esi, dword ptr [0x14]
   00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0abb  3bde                   cmp ebx, esi
@@ retail 396-397 built 420-421
   00bc0ac9  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0acf  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0ad2  85f0                   test eax, esi
 + 00bc0b22  85c6                   test esi, eax
   00bc0ad4  7522                   jne 0xbc0af8
   00bc0ad6  0bf0                   or esi, eax
@@ retail 398-400 built 422-424
   00bc0ad2  85f0                   test eax, esi
   00bc0ad4  7522                   jne 0xbc0af8
 - 00bc0ad6  0bf0                   or esi, eax
 - 00bc0ad8  897108                 mov dword ptr [ecx + 8], esi
 + 00bc0b26  0bc6                   or eax, esi
 + 00bc0b28  894108                 mov dword ptr [ecx + 8], eax
   00bc0adb  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0ae1  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 416-417 built 440-441
   00bc0b19  c644244400             mov byte ptr [esp + 0x44], 0
   00bc0b1e  e96c010000             jmp 0xbc0c8f
 - 00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0b73  8b3504000000           mov esi, dword ptr [4]
   00bc0b29  3bde                   cmp ebx, esi
   00bc0b2b  8bfe                   mov edi, esi
@@ retail 422-423 built 446-447
   00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0b35  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0b38  85f0                   test eax, esi
 + 00bc0b88  85c6                   test esi, eax
   00bc0b3a  7522                   jne 0xbc0b5e
   00bc0b3c  0bf0                   or esi, eax
@@ retail 424-426 built 448-450
   00bc0b38  85f0                   test eax, esi
   00bc0b3a  7522                   jne 0xbc0b5e
 - 00bc0b3c  0bf0                   or esi, eax
 - 00bc0b3e  897208                 mov dword ptr [edx + 8], esi
 + 00bc0b8c  0bc6                   or eax, esi
 + 00bc0b8e  894208                 mov dword ptr [edx + 8], eax
   00bc0b41  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0b47  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 441-454 built 465-466
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
 + 00bc0bd4  8b350c000000           mov esi, dword ptr [0xc]
   00bc0bb8  e98f000000             jmp 0xbc0c4c
   00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
@@ retail 455-456 built 467-468
   00bc0bb5  897108                 mov dword ptr [ecx + 8], esi
   00bc0bb8  e98f000000             jmp 0xbc0c4c
 - 00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0bdc  8b3504000000           mov esi, dword ptr [4]
   00bc0bc3  3bde                   cmp ebx, esi
   00bc0bc5  8bc8                   mov ecx, eax
@@ retail 481-482 built 493-494
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0c3f  8b3504000000           mov esi, dword ptr [4]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0c2c  3bde                   cmp ebx, esi
@@ retail 558-559 built 570-571
   00bc0d3f  a804                   test al, 4
   00bc0d41  7429                   je 0xbc0d6c
 - 00bc0d43  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0d62  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0d47  8b5128                 mov edx, dword ptr [ecx + 0x28]
   00bc0d4a  8b6924                 mov ebp, dword ptr [ecx + 0x24]
@@ retail 577-578 built 589-590
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 + 00bc0d99  8b1508000000           mov edx, dword ptr [8]
   00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
   00bc0d86  3bfa                   cmp edi, edx
@@ retail 583-583 built 595-596
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 + 00bc0db1  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
   00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
@@ retail 584-585 built 597-597
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
@@ retail 603-604 built 615-616
   00bc0dd5  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00bc0ddc  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00bc0de2  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0e01  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc0de6  8b4f1c                 mov ecx, dword ptr [edi + 0x1c]
   00bc0de9  8b6f18                 mov ebp, dword ptr [edi + 0x18]
@@ retail 624-625 built 636-637
   00bc0e12  e80972dcff             call 0x988020
   00bc0e17  eb39                   jmp 0xbc0e52
 - 00bc0e19  8b4c2414               mov ecx, dword ptr [esp + 0x14]
 + 00bc0e38  8b4c2410               mov ecx, dword ptr [esp + 0x10]
   00bc0e1d  8b5110                 mov edx, dword ptr [ecx + 0x10]
   00bc0e20  8b690c                 mov ebp, dword ptr [ecx + 0xc]
@@ retail 644-645 built 656-657
   00bc0e48  52                     push edx
   00bc0e49  e8d271dcff             call 0x988020
 - 00bc0e4e  8b7c2414               mov edi, dword ptr [esp + 0x14]
 + 00bc0e6d  8b7c2410               mov edi, dword ptr [esp + 0x10]
   00bc0e52  8b742418               mov esi, dword ptr [esp + 0x18]
   00bc0e56  f6868400000020         test byte ptr [esi + 0x84], 0x20
@@ retail 698-698 built 710-711
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 + 00bc0f18  0544010000             add eax, 0x144
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
@@ retail 699-700 built 712-713
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 + 00bc0f23  8b500c                 mov edx, dword ptr [eax + 0xc]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
@@ retail 702-703 built 715-715
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
@@ retail 706-707 built 718-719
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0f3e  8997b4000000           mov dword ptr [edi + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 758-759 built 770-771
   00bc0ff6  6a00                   push 0
   00bc0ff8  51                     push ecx
 - 00bc0ff9  8d54241c               lea edx, [esp + 0x1c]
 + 00bc1018  8d542420               lea edx, [esp + 0x20]
   00bc0ffd  52                     push edx
   00bc0ffe  8bcb                   mov ecx, ebx
@@ retail 818-819 built 830-831
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc110a  d80c8500000000         fmul dword ptr [eax*4]
   00bc10f2  d9c9                   fxch st(1)
   00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 820-821 built 832-833
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc1113  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 844-845 built 856-857
   00bc1146  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc116c  d80c8500000000         fmul dword ptr [eax*4]
   00bc1154  d9c9                   fxch st(1)
   00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 846-847 built 858-859
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc1175  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 862-863 built 874-875
   00bc1183  6a06                   push 6
   00bc1185  e87689dcff             call 0x989b00
 - 00bc118a  8b442410               mov eax, dword ptr [esp + 0x10]
 + 00bc11a9  8b442414               mov eax, dword ptr [esp + 0x14]
   00bc118e  0fb74812               movzx ecx, word ptr [eax + 0x12]
   00bc1192  894c2444               mov dword ptr [esp + 0x44], ecx
@@ retail 973-975 built 985-986
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
   00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 + 00bc133a  85c9                   test ecx, ecx
   00bc131f  7415                   je 0xbc1336
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 978-979 built 989-990
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
   00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 + 00bc1347  c780e800000000000000   mov dword ptr [eax + 0xe8], 0
   00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
@@ retail 982-983 built 993-995
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
 - 00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
 + 00bc1362  33ff                   xor edi, edi
 + 00bc1364  8b742414               mov esi, dword ptr [esp + 0x14]
   00bc1345  3bf7                   cmp esi, edi
   00bc1347  7444                   je 0xbc138d
@@ retail 1005-1006 built 1017-1017
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
@@ retail 1008-1008 built 1019-1020
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
 + 00bc13a5  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
@@ retail 1011-1011 built 1023-1024
   00bc138a  83c404                 add esp, 4
   00bc138d  5e                     pop esi
 + 00bc13b1  5b                     pop ebx
   00bc138e  5d                     pop ebp
   00bc138f  5f                     pop edi
@@ retail 1013-1014 built 1026-1026
   00bc138e  5d                     pop ebp
   00bc138f  5f                     pop edi
 - 00bc1390  5b                     pop ebx
   00bc1391  83c430                 add esp, 0x30
   00bc1394  c20800                 ret 8
DIFFLINES 135 retail_instr 1016 built_instr 1028
