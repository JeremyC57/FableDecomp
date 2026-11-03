00bc04f0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite DIFFER(3764v3751) flags= []
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
@@ retail 182-182 built 182-198
   00bc0793  51                     push ecx
   00bc0794  eb06                   jmp 0xbc079c
 + 00bc0792  f6c110                 test cl, 0x10
 + 00bc0795  7439                   je 0xbc07d0
 + 00bc0797  8b3508000000           mov esi, dword ptr [8]
 + 00bc079d  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
 + 00bc07a3  3bee                   cmp ebp, esi
 + 00bc07a5  8d8890280000           lea ecx, [eax + 0x2890]
 + 00bc07ab  8bd0                   mov edx, eax
 + 00bc07ad  8bfe                   mov edi, esi
 + 00bc07af  0f841f020000           je 0xbc09d4
 + 00bc07b5  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 + 00bc07bb  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00bc07be  85f0                   test eax, esi
 + 00bc07c0  0f85ed010000           jne 0xbc09b3
 + 00bc07c6  0bf0                   or esi, eax
 + 00bc07c8  897108                 mov dword ptr [ecx + 8], esi
 + 00bc07cb  e9c6010000             jmp 0xbc0996
   00bc0796  6a00                   push 0
   00bc0798  8d5330                 lea edx, [ebx + 0x30]
@@ retail 204-205 built 220-221
   00bc07e3  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc07e9  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc07ec  85f0                   test eax, esi
 + 00bc0826  85c6                   test esi, eax
   00bc07ee  7522                   jne 0xbc0812
   00bc07f0  0bf0                   or esi, eax
@@ retail 206-208 built 222-224
   00bc07ec  85f0                   test eax, esi
   00bc07ee  7522                   jne 0xbc0812
 - 00bc07f0  0bf0                   or esi, eax
 - 00bc07f2  897208                 mov dword ptr [edx + 8], esi
 + 00bc082a  0bc6                   or eax, esi
 + 00bc082c  894208                 mov dword ptr [edx + 8], eax
   00bc07f5  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc07fb  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 225-226 built 241-242
   00bc0838  8bb0e4280000           mov esi, dword ptr [eax + 0x28e4]
   00bc083e  8d90e0280000           lea edx, [eax + 0x28e0]
 - 00bc0844  bf10000000             mov edi, 0x10
 + 00bc087e  bf11000000             mov edi, 0x11
   00bc0849  3bf7                   cmp esi, edi
   00bc084b  8bc8                   mov ecx, eax
@@ retail 231-232 built 247-248
   00bc084f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0855  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0858  85f0                   test eax, esi
 + 00bc0892  85c6                   test esi, eax
   00bc085a  7522                   jne 0xbc087e
   00bc085c  0bf0                   or esi, eax
@@ retail 233-235 built 249-251
   00bc0858  85f0                   test eax, esi
   00bc085a  7522                   jne 0xbc087e
 - 00bc085c  0bf0                   or esi, eax
 - 00bc085e  897208                 mov dword ptr [edx + 8], esi
 + 00bc0896  0bc6                   or eax, esi
 + 00bc0898  894208                 mov dword ptr [edx + 8], eax
   00bc0861  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0867  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 250-251 built 266-267
   00bc0899  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc089f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc08a4  8b35e86d3901           mov esi, dword ptr [0x1396de8]
 + 00bc08de  8b3510000000           mov esi, dword ptr [0x10]
   00bc08aa  39b0a8280000           cmp dword ptr [eax + 0x28a8], esi
   00bc08b0  8d90a4280000           lea edx, [eax + 0x28a4]
@@ retail 258-259 built 274-275
   00bc08bc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc08c2  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc08c5  85f0                   test eax, esi
 + 00bc08ff  85c6                   test esi, eax
   00bc08c7  7522                   jne 0xbc08eb
   00bc08c9  0bf0                   or esi, eax
@@ retail 260-262 built 276-278
   00bc08c5  85f0                   test eax, esi
   00bc08c7  7522                   jne 0xbc08eb
 - 00bc08c9  0bf0                   or esi, eax
 - 00bc08cb  897208                 mov dword ptr [edx + 8], esi
 + 00bc0903  0bc6                   or eax, esi
 + 00bc0905  894208                 mov dword ptr [edx + 8], eax
   00bc08ce  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc08d4  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 286-287 built 302-303
   00bc0927  f6c110                 test cl, 0x10
   00bc092a  0f8466feffff           je 0xbc0796
 - 00bc0930  8b35b86f3901           mov esi, dword ptr [0x1396fb8]
 + 00bc096a  8b3508000000           mov esi, dword ptr [8]
   00bc0936  8ba894280000           mov ebp, dword ptr [eax + 0x2894]
   00bc093c  3bee                   cmp ebp, esi
@@ retail 293-293 built 309-310
   00bc0946  8bfe                   mov edi, esi
   00bc0948  7450                   je 0xbc099a
 + 00bc0984  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
   00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 294-296 built 311-312
   00bc0948  7450                   je 0xbc099a
   00bc094a  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc094d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc0953  85f0                   test eax, esi
 + 00bc098d  85c6                   test esi, eax
   00bc0955  7522                   jne 0xbc0979
   00bc0957  0bf0                   or esi, eax
@@ retail 297-299 built 313-315
   00bc0953  85f0                   test eax, esi
   00bc0955  7522                   jne 0xbc0979
 - 00bc0957  0bf0                   or esi, eax
 - 00bc0959  897108                 mov dword ptr [ecx + 8], esi
 + 00bc0991  0bc6                   or eax, esi
 + 00bc0993  894108                 mov dword ptr [ecx + 8], eax
   00bc095c  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0962  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 326-327 built 342-343
   00bc09be  8bc8                   mov ecx, eax
   00bc09c0  7510                   jne 0xbc09d2
 - 00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
 + 00bc09fc  8b3508000000           mov esi, dword ptr [8]
   00bc09c8  eb08                   jmp 0xbc09d2
   00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
@@ retail 328-329 built 344-345
   00bc09c2  8b35d86e3901           mov esi, dword ptr [0x1396ed8]
   00bc09c8  eb08                   jmp 0xbc09d2
 - 00bc09ca  8b35d46e3901           mov esi, dword ptr [0x1396ed4]
 + 00bc0a04  8b3504000000           mov esi, dword ptr [4]
   00bc09d0  8bc8                   mov ecx, eax
   00bc09d2  3bde                   cmp ebx, esi
@@ retail 333-333 built 349-350
   00bc09d4  8bfe                   mov edi, esi
   00bc09d6  7455                   je 0xbc0a2d
 + 00bc0a12  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
   00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 334-336 built 351-352
   00bc09d6  7455                   je 0xbc0a2d
   00bc09d8  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc09db  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00bc09e1  85f0                   test eax, esi
 + 00bc0a1b  85c6                   test esi, eax
   00bc09e3  7522                   jne 0xbc0a07
   00bc09e5  0bf0                   or esi, eax
@@ retail 337-339 built 353-355
   00bc09e1  85f0                   test eax, esi
   00bc09e3  7522                   jne 0xbc0a07
 - 00bc09e5  0bf0                   or esi, eax
 - 00bc09e7  897208                 mov dword ptr [edx + 8], esi
 + 00bc0a1f  0bc6                   or eax, esi
 + 00bc0a21  894208                 mov dword ptr [edx + 8], eax
   00bc09ea  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc09f0  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 362-363 built 378-379
   00bc0a46  8bc8                   mov ecx, eax
   00bc0a48  0f84d5000000           je 0xbc0b23
 - 00bc0a4e  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00bc0a88  8b3510000000           mov esi, dword ptr [0x10]
   00bc0a54  3bde                   cmp ebx, esi
   00bc0a56  8bfe                   mov edi, esi
@@ retail 368-369 built 384-385
   00bc0a5a  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0a60  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0a63  85f0                   test eax, esi
 + 00bc0a9d  85c6                   test esi, eax
   00bc0a65  7522                   jne 0xbc0a89
   00bc0a67  0bf0                   or esi, eax
@@ retail 370-372 built 386-388
   00bc0a63  85f0                   test eax, esi
   00bc0a65  7522                   jne 0xbc0a89
 - 00bc0a67  0bf0                   or esi, eax
 - 00bc0a69  897208                 mov dword ptr [edx + 8], esi
 + 00bc0aa1  0bc6                   or eax, esi
 + 00bc0aa3  894208                 mov dword ptr [edx + 8], eax
   00bc0a6c  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0a72  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 387-388 built 403-404
   00bc0aa4  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0aaa  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0aaf  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00bc0ae9  8b3514000000           mov esi, dword ptr [0x14]
   00bc0ab5  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0abb  3bde                   cmp ebx, esi
@@ retail 396-397 built 412-413
   00bc0ac9  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0acf  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00bc0ad2  85f0                   test eax, esi
 + 00bc0b0c  85c6                   test esi, eax
   00bc0ad4  7522                   jne 0xbc0af8
   00bc0ad6  0bf0                   or esi, eax
@@ retail 398-400 built 414-416
   00bc0ad2  85f0                   test eax, esi
   00bc0ad4  7522                   jne 0xbc0af8
 - 00bc0ad6  0bf0                   or esi, eax
 - 00bc0ad8  897108                 mov dword ptr [ecx + 8], esi
 + 00bc0b10  0bc6                   or eax, esi
 + 00bc0b12  894108                 mov dword ptr [ecx + 8], eax
   00bc0adb  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00bc0ae1  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 416-417 built 432-433
   00bc0b19  c644244400             mov byte ptr [esp + 0x44], 0
   00bc0b1e  e96c010000             jmp 0xbc0c8f
 - 00bc0b23  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0b5d  8b3504000000           mov esi, dword ptr [4]
   00bc0b29  3bde                   cmp ebx, esi
   00bc0b2b  8bfe                   mov edi, esi
@@ retail 422-423 built 438-439
   00bc0b2f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00bc0b35  8b7208                 mov esi, dword ptr [edx + 8]
 - 00bc0b38  85f0                   test eax, esi
 + 00bc0b72  85c6                   test esi, eax
   00bc0b3a  7522                   jne 0xbc0b5e
   00bc0b3c  0bf0                   or esi, eax
@@ retail 424-426 built 440-442
   00bc0b38  85f0                   test eax, esi
   00bc0b3a  7522                   jne 0xbc0b5e
 - 00bc0b3c  0bf0                   or esi, eax
 - 00bc0b3e  897208                 mov dword ptr [edx + 8], esi
 + 00bc0b76  0bc6                   or eax, esi
 + 00bc0b78  894208                 mov dword ptr [edx + 8], eax
   00bc0b41  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00bc0b47  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 441-454 built 457-458
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
 + 00bc0bbe  8b350c000000           mov esi, dword ptr [0xc]
   00bc0bb8  e98f000000             jmp 0xbc0c4c
   00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
@@ retail 455-456 built 459-460
   00bc0bb5  897108                 mov dword ptr [ecx + 8], esi
   00bc0bb8  e98f000000             jmp 0xbc0c4c
 - 00bc0bbd  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0bc6  8b3504000000           mov esi, dword ptr [4]
   00bc0bc3  3bde                   cmp ebx, esi
   00bc0bc5  8bc8                   mov ecx, eax
@@ retail 481-482 built 485-486
   00bc0c15  ff810c280000           inc dword ptr [ecx + 0x280c]
   00bc0c1b  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00bc0c20  8b356c6f3901           mov esi, dword ptr [0x1396f6c]
 + 00bc0c29  8b3504000000           mov esi, dword ptr [4]
   00bc0c26  8b9834290000           mov ebx, dword ptr [eax + 0x2934]
   00bc0c2c  3bde                   cmp ebx, esi
@@ retail 577-578 built 581-582
   00bc0d6e  0f84a5000000           je 0xbc0e19
   00bc0d74  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00bc0d7a  8b15b86f3901           mov edx, dword ptr [0x1396fb8]
 + 00bc0d83  8b1508000000           mov edx, dword ptr [8]
   00bc0d80  8bb994280000           mov edi, dword ptr [ecx + 0x2894]
   00bc0d86  3bfa                   cmp edi, edx
@@ retail 583-583 built 587-588
   00bc0d8e  8bea                   mov ebp, edx
   00bc0d90  7450                   je 0xbc0de2
 + 00bc0d9b  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
   00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
@@ retail 584-585 built 589-589
   00bc0d90  7450                   je 0xbc0de2
   00bc0d92  8b7808                 mov edi, dword ptr [eax + 8]
 - 00bc0d95  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
   00bc0d9b  85d7                   test edi, edx
   00bc0d9d  7522                   jne 0xbc0dc1
@@ retail 698-698 built 702-703
   00bc0eef  8b9044010000           mov edx, dword ptr [eax + 0x144]
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
 + 00bc0f02  0544010000             add eax, 0x144
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
   00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
@@ retail 699-700 built 704-705
   00bc0ef5  8b5c241c               mov ebx, dword ptr [esp + 0x1c]
   00bc0ef9  8997a8000000           mov dword ptr [edi + 0xa8], edx
 - 00bc0eff  8b9050010000           mov edx, dword ptr [eax + 0x150]
 + 00bc0f0d  8b500c                 mov edx, dword ptr [eax + 0xc]
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
@@ retail 702-703 built 707-707
   00bc0f05  8997ac000000           mov dword ptr [edi + 0xac], edx
   00bc0f0b  8b5668                 mov edx, dword ptr [esi + 0x68]
 - 00bc0f0e  0544010000             add eax, 0x144
   00bc0f13  8997b0000000           mov dword ptr [edi + 0xb0], edx
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
@@ retail 706-707 built 710-711
   00bc0f19  8b566c                 mov edx, dword ptr [esi + 0x6c]
   00bc0f1c  8d8fa8000000           lea ecx, [edi + 0xa8]
 - 00bc0f22  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00bc0f28  8997b4000000           mov dword ptr [edi + 0xb4], edx
   00bc0f25  8b5004                 mov edx, dword ptr [eax + 4]
   00bc0f28  8997b8000000           mov dword ptr [edi + 0xb8], edx
@@ retail 818-819 built 822-823
   00bc10e3  d905d8de2201           fld dword ptr [0x122ded8]
   00bc10e9  d8e1                   fsub st(1)
 - 00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc10f4  d80c8500000000         fmul dword ptr [eax*4]
   00bc10f2  d9c9                   fxch st(1)
   00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 820-821 built 824-825
   00bc10eb  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc10f2  d9c9                   fxch st(1)
 - 00bc10f4  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc10fd  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc10fb  dec1                   faddp st(1)
   00bc10fd  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 844-845 built 848-849
   00bc1146  51                     push ecx
   00bc1147  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
 + 00bc1156  d80c8500000000         fmul dword ptr [eax*4]
   00bc1154  d9c9                   fxch st(1)
   00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
@@ retail 846-847 built 850-851
   00bc114d  d80c8550d53c01         fmul dword ptr [eax*4 + 0x13cd550]
   00bc1154  d9c9                   fxch st(1)
 - 00bc1156  d80c8554d53c01         fmul dword ptr [eax*4 + 0x13cd554]
 + 00bc115f  d80c8504000000         fmul dword ptr [eax*4 + 4]
   00bc115d  dec1                   faddp st(1)
   00bc115f  d91c24                 fstp dword ptr [esp]
@@ retail 973-975 built 977-978
   00bc130e  8388e40000000f         or dword ptr [eax + 0xe4], 0xf
   00bc1315  8b88e8000000           mov ecx, dword ptr [eax + 0xe8]
 - 00bc131b  33ff                   xor edi, edi
 - 00bc131d  3bcf                   cmp ecx, edi
 + 00bc1324  85c9                   test ecx, ecx
   00bc131f  7415                   je 0xbc1336
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
@@ retail 978-979 built 981-982
   00bc1321  8b88e4000000           mov ecx, dword ptr [eax + 0xe4]
   00bc1327  83c902                 or ecx, 2
 - 00bc132a  89b8e8000000           mov dword ptr [eax + 0xe8], edi
 + 00bc1331  c780e800000000000000   mov dword ptr [eax + 0xe8], 0
   00bc1330  8988e4000000           mov dword ptr [eax + 0xe4], ecx
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
@@ retail 982-982 built 985-986
   00bc1336  8b0d08b53c01           mov ecx, dword ptr [0x13cb508]
   00bc133c  e8ff44e4ff             call 0xa05840
 + 00bc134c  33ff                   xor edi, edi
   00bc1341  8b742410               mov esi, dword ptr [esp + 0x10]
   00bc1345  3bf7                   cmp esi, edi
@@ retail 1005-1006 built 1009-1009
   00bc1376  e841d60300             call 0xbfe9bc
   00bc137b  83c404                 add esp, 4
 - 00bc137e  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
@@ retail 1008-1008 built 1011-1012
   00bc1381  56                     push esi
   00bc1382  897e08                 mov dword ptr [esi + 8], edi
 + 00bc138f  897e0c                 mov dword ptr [esi + 0xc], edi
   00bc1385  e832d60300             call 0xbfe9bc
   00bc138a  83c404                 add esp, 4
DIFFLINES 91 retail_instr 1016 built_instr 1020
