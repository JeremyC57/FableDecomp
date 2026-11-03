00b4d370 81ecd0030000         sub esp, 0x3d0
00b4d376 a19c6e4301           mov eax, dword ptr [0x1436e9c]
00b4d37b 53                   push ebx
00b4d37c 8bd9                 mov ebx, ecx
00b4d37e 8b4830               mov ecx, dword ptr [eax + 0x30]
00b4d381 898c24e8000000       mov dword ptr [esp + 0xe8], ecx
00b4d388 8b5034               mov edx, dword ptr [eax + 0x34]
00b4d38b 55                   push ebp
00b4d38c 56                   push esi
00b4d38d 899424f4000000       mov dword ptr [esp + 0xf4], edx
00b4d394 8b4038               mov eax, dword ptr [eax + 0x38]
00b4d397 57                   push edi
00b4d398 8d8c24f4000000       lea ecx, [esp + 0xf4]
00b4d39f 898424fc000000       mov dword ptr [esp + 0xfc], eax
00b4d3a6 e89570ecff           call 0xa14440
00b4d3ab d983bc020000         fld dword ptr [ebx + 0x2bc]
00b4d3b1 8b9424f4000000       mov edx, dword ptr [esp + 0xf4]
00b4d3b8 dcc0                 fadd st(0), st(0)
00b4d3ba 8b8424f8000000       mov eax, dword ptr [esp + 0xf8]
00b4d3c1 8d8b04010000         lea ecx, [ebx + 0x104]
00b4d3c7 8911                 mov dword ptr [ecx], edx
00b4d3c9 8b9424fc000000       mov edx, dword ptr [esp + 0xfc]
00b4d3d0 894104               mov dword ptr [ecx + 4], eax
00b4d3d3 6800000044           push 0x44000000
00b4d3d8 51                   push ecx
00b4d3d9 d91c24               fstp dword ptr [esp]
00b4d3dc 895108               mov dword ptr [ecx + 8], edx
00b4d3df 8d8424fc000000       lea eax, [esp + 0xfc]
00b4d3e6 50                   push eax
00b4d3e7 8d8c240c010000       lea ecx, [esp + 0x10c]
00b4d3ee 51                   push ecx
00b4d3ef 8bcb                 mov ecx, ebx
00b4d3f1 e8bae4ffff           call 0xb4b8b0
00b4d3f6 8db37c020000         lea esi, [ebx + 0x27c]
00b4d3fc b90c000000           mov ecx, 0xc
00b4d401 8dbc248c000000       lea edi, [esp + 0x8c]
00b4d408 f3a5                 rep movsd dword ptr es:[edi], dword ptr [esi]
00b4d40a d9842494000000       fld dword ptr [esp + 0x94]
00b4d411 d88c24a8000000       fmul dword ptr [esp + 0xa8]
00b4d418 d9842490000000       fld dword ptr [esp + 0x90]
00b4d41f d88c24ac000000       fmul dword ptr [esp + 0xac]
00b4d426 dee9                 fsubp st(1)
00b4d428 d9842490000000       fld dword ptr [esp + 0x90]
00b4d42f d88c24a0000000       fmul dword ptr [esp + 0xa0]
00b4d436 d9842494000000       fld dword ptr [esp + 0x94]
00b4d43d d88c249c000000       fmul dword ptr [esp + 0x9c]
00b4d444 dee9                 fsubp st(1)
00b4d446 d98424ac000000       fld dword ptr [esp + 0xac]
00b4d44d d88c249c000000       fmul dword ptr [esp + 0x9c]
00b4d454 d98424a0000000       fld dword ptr [esp + 0xa0]
00b4d45b d88c24a8000000       fmul dword ptr [esp + 0xa8]
00b4d462 dee9                 fsubp st(1)
00b4d464 d98424a4000000       fld dword ptr [esp + 0xa4]
00b4d46b d8ca                 fmul st(2)
00b4d46d d9842498000000       fld dword ptr [esp + 0x98]
00b4d474 d8cc                 fmul st(4)
00b4d476 dec1                 faddp st(1)
00b4d478 d9c1                 fld st(1)
00b4d47a d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4d481 dec1                 faddp st(1)
00b4d483 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4d489 d95c2410             fstp dword ptr [esp + 0x10]
00b4d48d d84c2410             fmul dword ptr [esp + 0x10]
00b4d491 d95c2434             fstp dword ptr [esp + 0x34]
00b4d495 d98424a4000000       fld dword ptr [esp + 0xa4]
00b4d49c d88c24a0000000       fmul dword ptr [esp + 0xa0]
00b4d4a3 d9842498000000       fld dword ptr [esp + 0x98]
00b4d4aa d88c24ac000000       fmul dword ptr [esp + 0xac]
00b4d4b1 dee9                 fsubp st(1)
00b4d4b3 d84c2410             fmul dword ptr [esp + 0x10]
00b4d4b7 d95c2440             fstp dword ptr [esp + 0x40]
00b4d4bb d9842498000000       fld dword ptr [esp + 0x98]
00b4d4c2 d88c24a8000000       fmul dword ptr [esp + 0xa8]
00b4d4c9 d98424a4000000       fld dword ptr [esp + 0xa4]
00b4d4d0 d88c249c000000       fmul dword ptr [esp + 0x9c]
00b4d4d7 dee9                 fsubp st(1)
00b4d4d9 d84c2410             fmul dword ptr [esp + 0x10]
00b4d4dd d95c244c             fstp dword ptr [esp + 0x4c]
00b4d4e1 d9c9                 fxch st(1)
00b4d4e3 d84c2410             fmul dword ptr [esp + 0x10]
00b4d4e7 8b93bc020000         mov edx, dword ptr [ebx + 0x2bc]
00b4d4ed 8bc2                 mov eax, edx
00b4d4ef 8bca                 mov ecx, edx
00b4d4f1 d95c2438             fstp dword ptr [esp + 0x38]
00b4d4f5 899424b4000000       mov dword ptr [esp + 0xb4], edx
00b4d4fc d98424ac000000       fld dword ptr [esp + 0xac]
00b4d503 898c24ac000000       mov dword ptr [esp + 0xac], ecx
00b4d50a d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4d511 898424b8000000       mov dword ptr [esp + 0xb8], eax
00b4d518 d98424a4000000       fld dword ptr [esp + 0xa4]
00b4d51f 898c24bc000000       mov dword ptr [esp + 0xbc], ecx
00b4d526 d88c2494000000       fmul dword ptr [esp + 0x94]
00b4d52d 899424c0000000       mov dword ptr [esp + 0xc0], edx
00b4d534 898424c4000000       mov dword ptr [esp + 0xc4], eax
00b4d53b dee9                 fsubp st(1)
00b4d53d d84c2410             fmul dword ptr [esp + 0x10]
00b4d541 d95c2444             fstp dword ptr [esp + 0x44]
00b4d545 d98424a4000000       fld dword ptr [esp + 0xa4]
00b4d54c 898424a4000000       mov dword ptr [esp + 0xa4], eax
00b4d553 d88c2490000000       fmul dword ptr [esp + 0x90]
00b4d55a d98424a8000000       fld dword ptr [esp + 0xa8]
00b4d561 d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4d568 dee9                 fsubp st(1)
00b4d56a d84c2410             fmul dword ptr [esp + 0x10]
00b4d56e d95c2450             fstp dword ptr [esp + 0x50]
00b4d572 d84c2410             fmul dword ptr [esp + 0x10]
00b4d576 d9842498000000       fld dword ptr [esp + 0x98]
00b4d57d d88c2494000000       fmul dword ptr [esp + 0x94]
00b4d584 d98424a0000000       fld dword ptr [esp + 0xa0]
00b4d58b 899424a0000000       mov dword ptr [esp + 0xa0], edx
00b4d592 d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4d599 dee9                 fsubp st(1)
00b4d59b d84c2410             fmul dword ptr [esp + 0x10]
00b4d59f d984249c000000       fld dword ptr [esp + 0x9c]
00b4d5a6 d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4d5ad d9842498000000       fld dword ptr [esp + 0x98]
00b4d5b4 d88c2490000000       fmul dword ptr [esp + 0x90]
00b4d5bb dee9                 fsubp st(1)
00b4d5bd d84c2410             fmul dword ptr [esp + 0x10]
00b4d5c1 d944244c             fld dword ptr [esp + 0x4c]
00b4d5c5 d8442440             fadd dword ptr [esp + 0x40]
00b4d5c9 d8442434             fadd dword ptr [esp + 0x34]
00b4d5cd d80ddcde2201         fmul dword ptr [0x122dedc]
00b4d5d3 d9c0                 fld st(0)
00b4d5d5 d9e0                 fchs 
00b4d5d7 d9442450             fld dword ptr [esp + 0x50]
00b4d5db d8442444             fadd dword ptr [esp + 0x44]
00b4d5df d8442438             fadd dword ptr [esp + 0x38]
00b4d5e3 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4d5e9 d9c0                 fld st(0)
00b4d5eb d9e0                 fchs 
00b4d5ed d95c245c             fstp dword ptr [esp + 0x5c]
00b4d5f1 d9c3                 fld st(3)
00b4d5f3 d8c5                 fadd st(5)
00b4d5f5 d8c6                 fadd st(6)
00b4d5f7 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4d5fd d9542414             fst dword ptr [esp + 0x14]
00b4d601 d9e0                 fchs 
00b4d603 d95c2460             fstp dword ptr [esp + 0x60]
00b4d607 d983bc020000         fld dword ptr [ebx + 0x2bc]
00b4d60d d9e0                 fchs 
00b4d60f d9942498000000       fst dword ptr [esp + 0x98]
00b4d616 d994249c000000       fst dword ptr [esp + 0x9c]
00b4d61d d99424a8000000       fst dword ptr [esp + 0xa8]
00b4d624 d99c24b0000000       fstp dword ptr [esp + 0xb0]
00b4d62b d9c1                 fld st(1)
00b4d62d d8c3                 fadd st(3)
00b4d62f d99c248c000000       fstp dword ptr [esp + 0x8c]
00b4d636 d944245c             fld dword ptr [esp + 0x5c]
00b4d63a d8c1                 fadd st(1)
00b4d63c d99c2490000000       fstp dword ptr [esp + 0x90]
00b4d643 ddd8                 fstp st(0)
00b4d645 d9442460             fld dword ptr [esp + 0x60]
00b4d649 d8442414             fadd dword ptr [esp + 0x14]
00b4d64d d99c2494000000       fstp dword ptr [esp + 0x94]
00b4d654 d9842498000000       fld dword ptr [esp + 0x98]
00b4d65b d9842498000000       fld dword ptr [esp + 0x98]
00b4d662 8b8c249c000000       mov ecx, dword ptr [esp + 0x9c]
00b4d669 d84c2434             fmul dword ptr [esp + 0x34]
00b4d66d 894c2418             mov dword ptr [esp + 0x18], ecx
00b4d671 d98424a0000000       fld dword ptr [esp + 0xa0]
00b4d678 8b9424a8000000       mov edx, dword ptr [esp + 0xa8]
00b4d67f d84c244c             fmul dword ptr [esp + 0x4c]
00b4d683 dec1                 faddp st(1)
00b4d685 d984249c000000       fld dword ptr [esp + 0x9c]
00b4d68c d84c2440             fmul dword ptr [esp + 0x40]
00b4d690 dec1                 faddp st(1)
00b4d692 d8c2                 fadd st(2)
00b4d694 d99c2498000000       fstp dword ptr [esp + 0x98]
00b4d69b d9442438             fld dword ptr [esp + 0x38]
00b4d69f d8c9                 fmul st(1)
00b4d6a1 d98424a0000000       fld dword ptr [esp + 0xa0]
00b4d6a8 d84c2450             fmul dword ptr [esp + 0x50]
00b4d6ac dec1                 faddp st(1)
00b4d6ae d984249c000000       fld dword ptr [esp + 0x9c]
00b4d6b5 d84c2444             fmul dword ptr [esp + 0x44]
00b4d6b9 dec1                 faddp st(1)
00b4d6bb d844245c             fadd dword ptr [esp + 0x5c]
00b4d6bf d99c249c000000       fstp dword ptr [esp + 0x9c]
00b4d6c6 d9c4                 fld st(4)
00b4d6c8 d84c2418             fmul dword ptr [esp + 0x18]
00b4d6cc 89542418             mov dword ptr [esp + 0x18], edx
00b4d6d0 d9c6                 fld st(6)
00b4d6d2 d8ca                 fmul st(2)
00b4d6d4 dec1                 faddp st(1)
00b4d6d6 d98424a0000000       fld dword ptr [esp + 0xa0]
00b4d6dd d8cd                 fmul st(5)
00b4d6df dec1                 faddp st(1)
00b4d6e1 d8442460             fadd dword ptr [esp + 0x60]
00b4d6e5 d99c24a0000000       fstp dword ptr [esp + 0xa0]
00b4d6ec ddd8                 fstp st(0)
00b4d6ee d98424a4000000       fld dword ptr [esp + 0xa4]
00b4d6f5 d98424ac000000       fld dword ptr [esp + 0xac]
00b4d6fc d84c244c             fmul dword ptr [esp + 0x4c]
00b4d700 d98424a8000000       fld dword ptr [esp + 0xa8]
00b4d707 d84c2440             fmul dword ptr [esp + 0x40]
00b4d70b dec1                 faddp st(1)
00b4d70d d98424a4000000       fld dword ptr [esp + 0xa4]
00b4d714 d84c2434             fmul dword ptr [esp + 0x34]
00b4d718 dec1                 faddp st(1)
00b4d71a d8c2                 fadd st(2)
00b4d71c d99c24a4000000       fstp dword ptr [esp + 0xa4]
00b4d723 d9442438             fld dword ptr [esp + 0x38]
00b4d727 d8c9                 fmul st(1)
00b4d729 d98424ac000000       fld dword ptr [esp + 0xac]
00b4d730 d84c2450             fmul dword ptr [esp + 0x50]
00b4d734 dec1                 faddp st(1)
00b4d736 d98424a8000000       fld dword ptr [esp + 0xa8]
00b4d73d d84c2444             fmul dword ptr [esp + 0x44]
00b4d741 dec1                 faddp st(1)
00b4d743 d844245c             fadd dword ptr [esp + 0x5c]
00b4d747 d99c24a8000000       fstp dword ptr [esp + 0xa8]
00b4d74e d9c4                 fld st(4)
00b4d750 d84c2418             fmul dword ptr [esp + 0x18]
00b4d754 89442418             mov dword ptr [esp + 0x18], eax
00b4d758 d9c6                 fld st(6)
00b4d75a d8ca                 fmul st(2)
00b4d75c dec1                 faddp st(1)
00b4d75e d98424ac000000       fld dword ptr [esp + 0xac]
00b4d765 d8cd                 fmul st(5)
00b4d767 dec1                 faddp st(1)
00b4d769 d8442460             fadd dword ptr [esp + 0x60]
00b4d76d d99c24ac000000       fstp dword ptr [esp + 0xac]
00b4d774 ddd8                 fstp st(0)
00b4d776 d98424b0000000       fld dword ptr [esp + 0xb0]
00b4d77d d98424b8000000       fld dword ptr [esp + 0xb8]
00b4d784 d84c244c             fmul dword ptr [esp + 0x4c]
00b4d788 d98424b4000000       fld dword ptr [esp + 0xb4]
00b4d78f d84c2440             fmul dword ptr [esp + 0x40]
00b4d793 dec1                 faddp st(1)
00b4d795 d98424b0000000       fld dword ptr [esp + 0xb0]
00b4d79c d84c2434             fmul dword ptr [esp + 0x34]
00b4d7a0 dec1                 faddp st(1)
00b4d7a2 d8c2                 fadd st(2)
00b4d7a4 d99c24b0000000       fstp dword ptr [esp + 0xb0]
00b4d7ab d9442438             fld dword ptr [esp + 0x38]
00b4d7af d8c9                 fmul st(1)
00b4d7b1 d98424b8000000       fld dword ptr [esp + 0xb8]
00b4d7b8 8bc8                 mov ecx, eax
00b4d7ba d84c2450             fmul dword ptr [esp + 0x50]
00b4d7be 894c2414             mov dword ptr [esp + 0x14], ecx
00b4d7c2 8b8c2490000000       mov ecx, dword ptr [esp + 0x90]
00b4d7c9 8bd0                 mov edx, eax
00b4d7cb dec1                 faddp st(1)
00b4d7cd 8b84248c000000       mov eax, dword ptr [esp + 0x8c]
00b4d7d4 d98424b4000000       fld dword ptr [esp + 0xb4]
00b4d7db 898424a8020000       mov dword ptr [esp + 0x2a8], eax
00b4d7e2 d84c2444             fmul dword ptr [esp + 0x44]
00b4d7e6 898c24ac020000       mov dword ptr [esp + 0x2ac], ecx
00b4d7ed c744243000000000     mov dword ptr [esp + 0x30], 0
00b4d7f5 8dac24b4020000       lea ebp, [esp + 0x2b4]
00b4d7fc dec1                 faddp st(1)
00b4d7fe d844245c             fadd dword ptr [esp + 0x5c]
00b4d802 d99c24b4000000       fstp dword ptr [esp + 0xb4]
00b4d809 d9c4                 fld st(4)
00b4d80b d84c2418             fmul dword ptr [esp + 0x18]
00b4d80f 89542418             mov dword ptr [esp + 0x18], edx
00b4d813 d9c6                 fld st(6)
00b4d815 8b942494000000       mov edx, dword ptr [esp + 0x94]
00b4d81c d8ca                 fmul st(2)
00b4d81e 899424b0020000       mov dword ptr [esp + 0x2b0], edx
00b4d825 dec1                 faddp st(1)
00b4d827 d98424b8000000       fld dword ptr [esp + 0xb8]
00b4d82e d8cd                 fmul st(5)
00b4d830 dec1                 faddp st(1)
00b4d832 d8442460             fadd dword ptr [esp + 0x60]
00b4d836 d99c24b8000000       fstp dword ptr [esp + 0xb8]
00b4d83d ddd8                 fstp st(0)
00b4d83f d98424c4000000       fld dword ptr [esp + 0xc4]
00b4d846 d84c244c             fmul dword ptr [esp + 0x4c]
00b4d84a d98424c0000000       fld dword ptr [esp + 0xc0]
00b4d851 d84c2440             fmul dword ptr [esp + 0x40]
00b4d855 dec1                 faddp st(1)
00b4d857 d98424bc000000       fld dword ptr [esp + 0xbc]
00b4d85e d84c2434             fmul dword ptr [esp + 0x34]
00b4d862 dec1                 faddp st(1)
00b4d864 d8c1                 fadd st(1)
00b4d866 d99c24bc000000       fstp dword ptr [esp + 0xbc]
00b4d86d ddd8                 fstp st(0)
00b4d86f ddd8                 fstp st(0)
00b4d871 d9442438             fld dword ptr [esp + 0x38]
00b4d875 d84c2414             fmul dword ptr [esp + 0x14]
00b4d879 d98424c4000000       fld dword ptr [esp + 0xc4]
00b4d880 d84c2450             fmul dword ptr [esp + 0x50]
00b4d884 dec1                 faddp st(1)
00b4d886 d98424c0000000       fld dword ptr [esp + 0xc0]
00b4d88d d84c2444             fmul dword ptr [esp + 0x44]
00b4d891 dec1                 faddp st(1)
00b4d893 d844245c             fadd dword ptr [esp + 0x5c]
00b4d897 d99c24c0000000       fstp dword ptr [esp + 0xc0]
00b4d89e d9c9                 fxch st(1)
00b4d8a0 d84c2418             fmul dword ptr [esp + 0x18]
00b4d8a4 d9ca                 fxch st(2)
00b4d8a6 d84c2414             fmul dword ptr [esp + 0x14]
00b4d8aa dec2                 faddp st(2)
00b4d8ac d98424c4000000       fld dword ptr [esp + 0xc4]
00b4d8b3 d8c9                 fmul st(1)
00b4d8b5 dec2                 faddp st(2)
00b4d8b7 d9c9                 fxch st(1)
00b4d8b9 d8442460             fadd dword ptr [esp + 0x60]
00b4d8bd d99c24c4000000       fstp dword ptr [esp + 0xc4]
00b4d8c4 ddd8                 fstp st(0)
00b4d8c6 db442430             fild dword ptr [esp + 0x30]
00b4d8ca 33ff                 xor edi, edi
00b4d8cc 8bf5                 mov esi, ebp
00b4d8ce 897c2418             mov dword ptr [esp + 0x18], edi
00b4d8d2 d80d8c072301         fmul dword ptr [0x123078c]
00b4d8d8 83c53c               add ebp, 0x3c
00b4d8db d95c2414             fstp dword ptr [esp + 0x14]
00b4d8df d905d8de2201         fld dword ptr [0x122ded8]
00b4d8e5 d8642414             fsub dword ptr [esp + 0x14]
00b4d8e9 d95c2474             fstp dword ptr [esp + 0x74]
00b4d8ed 8d4900               lea ecx, [ecx]
00b4d8f0 db442418             fild dword ptr [esp + 0x18]
00b4d8f4 d80d8c072301         fmul dword ptr [0x123078c]
00b4d8fa d905d8de2201         fld dword ptr [0x122ded8]
00b4d900 d8e1                 fsub st(1)
00b4d902 d9442414             fld dword ptr [esp + 0x14]
00b4d906 d8ca                 fmul st(2)
00b4d908 d95c2420             fstp dword ptr [esp + 0x20]
00b4d90c d9442414             fld dword ptr [esp + 0x14]
00b4d910 d8c9                 fmul st(1)
00b4d912 d95c2410             fstp dword ptr [esp + 0x10]
00b4d916 d9442474             fld dword ptr [esp + 0x74]
00b4d91a d8ca                 fmul st(2)
00b4d91c d95c2418             fstp dword ptr [esp + 0x18]
00b4d920 d9442474             fld dword ptr [esp + 0x74]
00b4d924 d8c9                 fmul st(1)
00b4d926 ddda                 fstp st(2)
00b4d928 ddd8                 fstp st(0)
00b4d92a d9442418             fld dword ptr [esp + 0x18]
00b4d92e d8c1                 fadd st(1)
00b4d930 d8442410             fadd dword ptr [esp + 0x10]
00b4d934 d8442420             fadd dword ptr [esp + 0x20]
00b4d938 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4d93e d9442420             fld dword ptr [esp + 0x20]
00b4d942 d8c9                 fmul st(1)
00b4d944 d95c2420             fstp dword ptr [esp + 0x20]
00b4d948 d9442410             fld dword ptr [esp + 0x10]
00b4d94c d8c9                 fmul st(1)
00b4d94e d95c2410             fstp dword ptr [esp + 0x10]
00b4d952 d9442418             fld dword ptr [esp + 0x18]
00b4d956 d8c9                 fmul st(1)
00b4d958 d95c2418             fstp dword ptr [esp + 0x18]
00b4d95c dec9                 fmulp st(1)
00b4d95e d98424bc000000       fld dword ptr [esp + 0xbc]
00b4d965 d8c9                 fmul st(1)
00b4d967 d98424c0000000       fld dword ptr [esp + 0xc0]
00b4d96e d8ca                 fmul st(2)
00b4d970 d99c2444010000       fstp dword ptr [esp + 0x144]
00b4d977 d98424c4000000       fld dword ptr [esp + 0xc4]
00b4d97e d8ca                 fmul st(2)
00b4d980 d99c2448010000       fstp dword ptr [esp + 0x148]
00b4d987 d98424b0000000       fld dword ptr [esp + 0xb0]
00b4d98e d84c2418             fmul dword ptr [esp + 0x18]
00b4d992 d98424b4000000       fld dword ptr [esp + 0xb4]
00b4d999 d84c2418             fmul dword ptr [esp + 0x18]
00b4d99d d98424b8000000       fld dword ptr [esp + 0xb8]
00b4d9a4 d84c2418             fmul dword ptr [esp + 0x18]
00b4d9a8 d99c2498020000       fstp dword ptr [esp + 0x298]
00b4d9af d98424a4000000       fld dword ptr [esp + 0xa4]
00b4d9b6 d84c2410             fmul dword ptr [esp + 0x10]
00b4d9ba d98424a8000000       fld dword ptr [esp + 0xa8]
00b4d9c1 d84c2410             fmul dword ptr [esp + 0x10]
00b4d9c5 d98424ac000000       fld dword ptr [esp + 0xac]
00b4d9cc d84c2410             fmul dword ptr [esp + 0x10]
00b4d9d0 d99c24a4020000       fstp dword ptr [esp + 0x2a4]
00b4d9d7 d9842498000000       fld dword ptr [esp + 0x98]
00b4d9de d84c2420             fmul dword ptr [esp + 0x20]
00b4d9e2 d984249c000000       fld dword ptr [esp + 0x9c]
00b4d9e9 d84c2420             fmul dword ptr [esp + 0x20]
00b4d9ed d95c247c             fstp dword ptr [esp + 0x7c]
00b4d9f1 d98424a0000000       fld dword ptr [esp + 0xa0]
00b4d9f8 d84c2420             fmul dword ptr [esp + 0x20]
00b4d9fc d99c2480000000       fstp dword ptr [esp + 0x80]
00b4da03 d8c2                 fadd st(2)
00b4da05 d99c244c010000       fstp dword ptr [esp + 0x14c]
00b4da0c d944247c             fld dword ptr [esp + 0x7c]
00b4da10 d8c1                 fadd st(1)
00b4da12 ddda                 fstp st(2)
00b4da14 ddd8                 fstp st(0)
00b4da16 d9842480000000       fld dword ptr [esp + 0x80]
00b4da1d d88424a4020000       fadd dword ptr [esp + 0x2a4]
00b4da24 d99c2454010000       fstp dword ptr [esp + 0x154]
00b4da2b d984244c010000       fld dword ptr [esp + 0x14c]
00b4da32 d8c3                 fadd st(3)
00b4da34 d99c24cc000000       fstp dword ptr [esp + 0xcc]
00b4da3b d8c1                 fadd st(1)
00b4da3d d99c24d0000000       fstp dword ptr [esp + 0xd0]
00b4da44 ddd8                 fstp st(0)
00b4da46 ddd8                 fstp st(0)
00b4da48 d9842454010000       fld dword ptr [esp + 0x154]
00b4da4f d8842498020000       fadd dword ptr [esp + 0x298]
00b4da56 d99c24d4000000       fstp dword ptr [esp + 0xd4]
00b4da5d d98424cc000000       fld dword ptr [esp + 0xcc]
00b4da64 8d4c2424             lea ecx, [esp + 0x24]
00b4da68 d8c1                 fadd st(1)
00b4da6a d95c2424             fstp dword ptr [esp + 0x24]
00b4da6e ddd8                 fstp st(0)
00b4da70 ddd8                 fstp st(0)
00b4da72 d98424d0000000       fld dword ptr [esp + 0xd0]
00b4da79 d8842444010000       fadd dword ptr [esp + 0x144]
00b4da80 d95c2428             fstp dword ptr [esp + 0x28]
00b4da84 d98424d4000000       fld dword ptr [esp + 0xd4]
00b4da8b d8842448010000       fadd dword ptr [esp + 0x148]
00b4da92 d95c242c             fstp dword ptr [esp + 0x2c]
00b4da96 e8a569ecff           call 0xa14440
00b4da9b d983bc020000         fld dword ptr [ebx + 0x2bc]
00b4daa1 8bc6                 mov eax, esi
00b4daa3 d9442424             fld dword ptr [esp + 0x24]
00b4daa7 83c60c               add esi, 0xc
00b4daaa d8c9                 fmul st(1)
00b4daac 47                   inc edi
00b4daad 83ff04               cmp edi, 4
00b4dab0 897c2418             mov dword ptr [esp + 0x18], edi
00b4dab4 d95c2424             fstp dword ptr [esp + 0x24]
00b4dab8 8b4c2424             mov ecx, dword ptr [esp + 0x24]
00b4dabc d9442428             fld dword ptr [esp + 0x28]
00b4dac0 8908                 mov dword ptr [eax], ecx
00b4dac2 d8c9                 fmul st(1)
00b4dac4 d95c2428             fstp dword ptr [esp + 0x28]
00b4dac8 8b542428             mov edx, dword ptr [esp + 0x28]
00b4dacc d944242c             fld dword ptr [esp + 0x2c]
00b4dad0 895004               mov dword ptr [eax + 4], edx
00b4dad3 d8c9                 fmul st(1)
00b4dad5 d95c242c             fstp dword ptr [esp + 0x2c]
00b4dad9 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00b4dadd 894808               mov dword ptr [eax + 8], ecx
00b4dae0 ddd8                 fstp st(0)
00b4dae2 0f8e08feffff         jle 0xb4d8f0
00b4dae8 8b442430             mov eax, dword ptr [esp + 0x30]
00b4daec 40                   inc eax
00b4daed 83f804               cmp eax, 4
00b4daf0 89442430             mov dword ptr [esp + 0x30], eax
00b4daf4 0f8eccfdffff         jle 0xb4d8c6
00b4dafa 33c0                 xor eax, eax
00b4dafc 8d642400             lea esp, [esp]
00b4db00 d98404a8020000       fld dword ptr [esp + eax + 0x2a8]
00b4db07 8b9404b0020000       mov edx, dword ptr [esp + eax + 0x2b0]
00b4db0e d98404ac020000       fld dword ptr [esp + eax + 0x2ac]
00b4db15 89942480000000       mov dword ptr [esp + 0x80], edx
00b4db1c d9c1                 fld st(1)
00b4db1e 8d8c0458010000       lea ecx, [esp + eax + 0x158]
00b4db25 d95c2418             fstp dword ptr [esp + 0x18]
00b4db29 d9542414             fst dword ptr [esp + 0x14]
00b4db2d d9842418010000       fld dword ptr [esp + 0x118]
00b4db34 d88c2480000000       fmul dword ptr [esp + 0x80]
00b4db3b d984240c010000       fld dword ptr [esp + 0x10c]
00b4db42 d8ca                 fmul st(2)
00b4db44 dec1                 faddp st(1)
00b4db46 d9842400010000       fld dword ptr [esp + 0x100]
00b4db4d d8cb                 fmul st(3)
00b4db4f dec1                 faddp st(1)
00b4db51 d8842424010000       fadd dword ptr [esp + 0x124]
00b4db58 d95c2478             fstp dword ptr [esp + 0x78]
00b4db5c 8b542478             mov edx, dword ptr [esp + 0x78]
00b4db60 d9842404010000       fld dword ptr [esp + 0x104]
00b4db67 8911                 mov dword ptr [ecx], edx
00b4db69 d84c2418             fmul dword ptr [esp + 0x18]
00b4db6d d984241c010000       fld dword ptr [esp + 0x11c]
00b4db74 d88c2480000000       fmul dword ptr [esp + 0x80]
00b4db7b dec1                 faddp st(1)
00b4db7d d9842410010000       fld dword ptr [esp + 0x110]
00b4db84 d8ca                 fmul st(2)
00b4db86 dec1                 faddp st(1)
00b4db88 d8842428010000       fadd dword ptr [esp + 0x128]
00b4db8f d95c247c             fstp dword ptr [esp + 0x7c]
00b4db93 8b54247c             mov edx, dword ptr [esp + 0x7c]
00b4db97 895104               mov dword ptr [ecx + 4], edx
00b4db9a ddd8                 fstp st(0)
00b4db9c ddd8                 fstp st(0)
00b4db9e d9842414010000       fld dword ptr [esp + 0x114]
00b4dba5 d84c2414             fmul dword ptr [esp + 0x14]
00b4dba9 d9842408010000       fld dword ptr [esp + 0x108]
00b4dbb0 d84c2418             fmul dword ptr [esp + 0x18]
00b4dbb4 dec1                 faddp st(1)
00b4dbb6 d9842420010000       fld dword ptr [esp + 0x120]
00b4dbbd d88c2480000000       fmul dword ptr [esp + 0x80]
00b4dbc4 dec1                 faddp st(1)
00b4dbc6 d884242c010000       fadd dword ptr [esp + 0x12c]
00b4dbcd d99c2480000000       fstp dword ptr [esp + 0x80]
00b4dbd4 8b942480000000       mov edx, dword ptr [esp + 0x80]
00b4dbdb d98404b4020000       fld dword ptr [esp + eax + 0x2b4]
00b4dbe2 895108               mov dword ptr [ecx + 8], edx
00b4dbe5 d98404b8020000       fld dword ptr [esp + eax + 0x2b8]
00b4dbec 8b8c04bc020000       mov ecx, dword ptr [esp + eax + 0x2bc]
00b4dbf3 d9c1                 fld st(1)
00b4dbf5 898c2480000000       mov dword ptr [esp + 0x80], ecx
00b4dbfc d95c2418             fstp dword ptr [esp + 0x18]
00b4dc00 d9542414             fst dword ptr [esp + 0x14]
00b4dc04 d9842418010000       fld dword ptr [esp + 0x118]
00b4dc0b d88c2480000000       fmul dword ptr [esp + 0x80]
00b4dc12 d984240c010000       fld dword ptr [esp + 0x10c]
00b4dc19 d8ca                 fmul st(2)
00b4dc1b dec1                 faddp st(1)
00b4dc1d d9842400010000       fld dword ptr [esp + 0x100]
00b4dc24 d8cb                 fmul st(3)
00b4dc26 dec1                 faddp st(1)
00b4dc28 d8842424010000       fadd dword ptr [esp + 0x124]
00b4dc2f d95c2478             fstp dword ptr [esp + 0x78]
00b4dc33 d9842404010000       fld dword ptr [esp + 0x104]
00b4dc3a d84c2418             fmul dword ptr [esp + 0x18]
00b4dc3e d984241c010000       fld dword ptr [esp + 0x11c]
00b4dc45 d88c2480000000       fmul dword ptr [esp + 0x80]
00b4dc4c dec1                 faddp st(1)
00b4dc4e d9842410010000       fld dword ptr [esp + 0x110]
00b4dc55 d8ca                 fmul st(2)
00b4dc57 dec1                 faddp st(1)
00b4dc59 d8842428010000       fadd dword ptr [esp + 0x128]
00b4dc60 d95c247c             fstp dword ptr [esp + 0x7c]
00b4dc64 ddd8                 fstp st(0)
00b4dc66 ddd8                 fstp st(0)
00b4dc68 d9842414010000       fld dword ptr [esp + 0x114]
00b4dc6f d84c2414             fmul dword ptr [esp + 0x14]
00b4dc73 d9842408010000       fld dword ptr [esp + 0x108]
00b4dc7a d84c2418             fmul dword ptr [esp + 0x18]
00b4dc7e dec1                 faddp st(1)
00b4dc80 d9842420010000       fld dword ptr [esp + 0x120]
00b4dc87 d88c2480000000       fmul dword ptr [esp + 0x80]
00b4dc8e 8b4c2478             mov ecx, dword ptr [esp + 0x78]
00b4dc92 8d940464010000       lea edx, [esp + eax + 0x164]
00b4dc99 890a                 mov dword ptr [edx], ecx
00b4dc9b 8b4c247c             mov ecx, dword ptr [esp + 0x7c]
00b4dc9f dec1                 faddp st(1)
00b4dca1 894a04               mov dword ptr [edx + 4], ecx
00b4dca4 83c018               add eax, 0x18
00b4dca7 3d38010000           cmp eax, 0x138
00b4dcac d884242c010000       fadd dword ptr [esp + 0x12c]
00b4dcb3 d99c2480000000       fstp dword ptr [esp + 0x80]
00b4dcba 8b8c2480000000       mov ecx, dword ptr [esp + 0x80]
00b4dcc1 894a08               mov dword ptr [edx + 8], ecx
00b4dcc4 0f8c36feffff         jl 0xb4db00
00b4dcca d98424f4010000       fld dword ptr [esp + 0x1f4]
00b4dcd1 8d8c2484000000       lea ecx, [esp + 0x84]
00b4dcd8 d8a42458010000       fsub dword ptr [esp + 0x158]
00b4dcdf d98424f8010000       fld dword ptr [esp + 0x1f8]
00b4dce6 d8a4245c010000       fsub dword ptr [esp + 0x15c]
00b4dced d9c9                 fxch st(1)
00b4dcef d99c2484000000       fstp dword ptr [esp + 0x84]
00b4dcf6 d99c2488000000       fstp dword ptr [esp + 0x88]
00b4dcfd e80e68ecff           call 0xa14510
00b4dd02 d9842484000000       fld dword ptr [esp + 0x84]
00b4dd09 8b942488000000       mov edx, dword ptr [esp + 0x88]
00b4dd10 d9e0                 fchs 
00b4dd12 8bc2                 mov eax, edx
00b4dd14 d95c247c             fstp dword ptr [esp + 0x7c]
00b4dd18 8b4c247c             mov ecx, dword ptr [esp + 0x7c]
00b4dd1c d905a8222a01         fld dword ptr [0x12a22a8]
00b4dd22 89442418             mov dword ptr [esp + 0x18], eax
00b4dd26 d984246c010000       fld dword ptr [esp + 0x16c]
00b4dd2d 8b842484000000       mov eax, dword ptr [esp + 0x84]
00b4dd34 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4dd3a 89442438             mov dword ptr [esp + 0x38], eax
00b4dd3e 89542478             mov dword ptr [esp + 0x78], edx
00b4dd42 894c241c             mov dword ptr [esp + 0x1c], ecx
00b4dd46 d95c2420             fstp dword ptr [esp + 0x20]
00b4dd4a 89542434             mov dword ptr [esp + 0x34], edx
00b4dd4e d9842464010000       fld dword ptr [esp + 0x164]
00b4dd55 c744243c00000000     mov dword ptr [esp + 0x3c], 0
00b4dd5d d80ddcde2201         fmul dword ptr [0x122dedc]
00b4dd63 c744244000000000     mov dword ptr [esp + 0x40], 0
00b4dd6b 894c2444             mov dword ptr [esp + 0x44], ecx
00b4dd6f 89542448             mov dword ptr [esp + 0x48], edx
00b4dd73 d99c24d8000000       fstp dword ptr [esp + 0xd8]
00b4dd7a c744244c00000000     mov dword ptr [esp + 0x4c], 0
00b4dd82 d9842468010000       fld dword ptr [esp + 0x168]
00b4dd89 c744245000000000     mov dword ptr [esp + 0x50], 0
00b4dd91 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4dd97 c744245400000000     mov dword ptr [esp + 0x54], 0
00b4dd9f c744245800000000     mov dword ptr [esp + 0x58], 0
00b4dda7 c744245c0000803f     mov dword ptr [esp + 0x5c], 0x3f800000
00b4ddaf d99c24ec000000       fstp dword ptr [esp + 0xec]
00b4ddb6 c744246000000000     mov dword ptr [esp + 0x60], 0
00b4ddbe d9842468010000       fld dword ptr [esp + 0x168]
00b4ddc5 c744246400000000     mov dword ptr [esp + 0x64], 0
00b4ddcd d88c2488000000       fmul dword ptr [esp + 0x88]
00b4ddd4 c744246800000000     mov dword ptr [esp + 0x68], 0
00b4dddc d9842464010000       fld dword ptr [esp + 0x164]
00b4dde3 c744246c00000000     mov dword ptr [esp + 0x6c], 0
00b4ddeb d88c2484000000       fmul dword ptr [esp + 0x84]
00b4ddf2 c74424700000803f     mov dword ptr [esp + 0x70], 0x3f800000
00b4ddfa c7442410caf249f1     mov dword ptr [esp + 0x10], 0xf149f2ca
00b4de02 dec1                 faddp st(1)
00b4de04 d8442420             fadd dword ptr [esp + 0x20]
00b4de08 d98424ec000000       fld dword ptr [esp + 0xec]
00b4de0f d88424d8000000       fadd dword ptr [esp + 0xd8]
00b4de16 d8442420             fadd dword ptr [esp + 0x20]
00b4de1a d805d8de2201         fadd dword ptr [0x122ded8]
00b4de20 def9                 fdivp st(1)
00b4de22 d95c2428             fstp dword ptr [esp + 0x28]
00b4de26 d905a8222a01         fld dword ptr [0x12a22a8]
00b4de2c d85c2428             fcomp dword ptr [esp + 0x28]
00b4de30 dfe0                 fnstsw ax
00b4de32 f6c441               test ah, 0x41
00b4de35 7506                 jne 0xb4de3d
00b4de37 ddd8                 fstp st(0)
00b4de39 d9442428             fld dword ptr [esp + 0x28]
00b4de3d d905a4dc2901         fld dword ptr [0x129dca4]
00b4de43 d85c2428             fcomp dword ptr [esp + 0x28]
00b4de47 dfe0                 fnstsw ax
00b4de49 f6c405               test ah, 5
00b4de4c 7a08                 jp 0xb4de56
00b4de4e 8b442428             mov eax, dword ptr [esp + 0x28]
00b4de52 89442410             mov dword ptr [esp + 0x10], eax
00b4de56 d984249c010000       fld dword ptr [esp + 0x19c]
00b4de5d d80ddcde2201         fmul dword ptr [0x122dedc]
00b4de63 d95c2474             fstp dword ptr [esp + 0x74]
00b4de67 d9842494010000       fld dword ptr [esp + 0x194]
00b4de6e d80ddcde2201         fmul dword ptr [0x122dedc]
00b4de74 d99c24e0000000       fstp dword ptr [esp + 0xe0]
00b4de7b d9842498010000       fld dword ptr [esp + 0x198]
00b4de82 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4de88 d99c24f0000000       fstp dword ptr [esp + 0xf0]
00b4de8f d9842498010000       fld dword ptr [esp + 0x198]
00b4de96 d88c2488000000       fmul dword ptr [esp + 0x88]
00b4de9d d9842494010000       fld dword ptr [esp + 0x194]
00b4dea4 d88c2484000000       fmul dword ptr [esp + 0x84]
00b4deab dec1                 faddp st(1)
00b4dead d8442474             fadd dword ptr [esp + 0x74]
00b4deb1 d98424f0000000       fld dword ptr [esp + 0xf0]
00b4deb8 d88424e0000000       fadd dword ptr [esp + 0xe0]
00b4debf d8442474             fadd dword ptr [esp + 0x74]
00b4dec3 d805d8de2201         fadd dword ptr [0x122ded8]
00b4dec9 def9                 fdivp st(1)
00b4decb d95c2428             fstp dword ptr [esp + 0x28]
00b4decf d8542428             fcom dword ptr [esp + 0x28]
00b4ded3 dfe0                 fnstsw ax
00b4ded5 f6c441               test ah, 0x41
00b4ded8 7506                 jne 0xb4dee0
00b4deda ddd8                 fstp st(0)
00b4dedc d9442428             fld dword ptr [esp + 0x28]
00b4dee0 d9442410             fld dword ptr [esp + 0x10]
00b4dee4 d85c2428             fcomp dword ptr [esp + 0x28]
00b4dee8 dfe0                 fnstsw ax
00b4deea f6c405               test ah, 5
00b4deed 7a08                 jp 0xb4def7
00b4deef 8b4c2428             mov ecx, dword ptr [esp + 0x28]
00b4def3 894c2410             mov dword ptr [esp + 0x10], ecx
00b4def7 d984245c020000       fld dword ptr [esp + 0x25c]
00b4defe d80ddcde2201         fmul dword ptr [0x122dedc]
00b4df04 d95c2430             fstp dword ptr [esp + 0x30]
00b4df08 d9842454020000       fld dword ptr [esp + 0x254]
00b4df0f d80ddcde2201         fmul dword ptr [0x122dedc]
00b4df15 d99c24dc000000       fstp dword ptr [esp + 0xdc]
00b4df1c d9842458020000       fld dword ptr [esp + 0x258]
00b4df23 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4df29 d99c24e4000000       fstp dword ptr [esp + 0xe4]
00b4df30 d9842458020000       fld dword ptr [esp + 0x258]
00b4df37 d88c2488000000       fmul dword ptr [esp + 0x88]
00b4df3e d9842454020000       fld dword ptr [esp + 0x254]
00b4df45 d88c2484000000       fmul dword ptr [esp + 0x84]
00b4df4c dec1                 faddp st(1)
00b4df4e d8442430             fadd dword ptr [esp + 0x30]
00b4df52 d98424e4000000       fld dword ptr [esp + 0xe4]
00b4df59 d88424dc000000       fadd dword ptr [esp + 0xdc]
00b4df60 d8442430             fadd dword ptr [esp + 0x30]
00b4df64 d805d8de2201         fadd dword ptr [0x122ded8]
00b4df6a def9                 fdivp st(1)
00b4df6c d95c2428             fstp dword ptr [esp + 0x28]
00b4df70 d8542428             fcom dword ptr [esp + 0x28]
00b4df74 dfe0                 fnstsw ax
00b4df76 f6c441               test ah, 0x41
00b4df79 7506                 jne 0xb4df81
00b4df7b ddd8                 fstp st(0)
00b4df7d d9442428             fld dword ptr [esp + 0x28]
00b4df81 d9442410             fld dword ptr [esp + 0x10]
00b4df85 d85c2428             fcomp dword ptr [esp + 0x28]
00b4df89 dfe0                 fnstsw ax
00b4df8b f6c405               test ah, 5
00b4df8e 7a08                 jp 0xb4df98
00b4df90 8b542428             mov edx, dword ptr [esp + 0x28]
00b4df94 89542410             mov dword ptr [esp + 0x10], edx
00b4df98 d984248c020000       fld dword ptr [esp + 0x28c]
00b4df9f d80ddcde2201         fmul dword ptr [0x122dedc]
00b4dfa5 d95c2414             fstp dword ptr [esp + 0x14]
00b4dfa9 d9842484020000       fld dword ptr [esp + 0x284]
00b4dfb0 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4dfb6 d95c2478             fstp dword ptr [esp + 0x78]
00b4dfba d9842488020000       fld dword ptr [esp + 0x288]
00b4dfc1 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4dfc7 d99c24e8000000       fstp dword ptr [esp + 0xe8]
00b4dfce d9842488020000       fld dword ptr [esp + 0x288]
00b4dfd5 d88c2488000000       fmul dword ptr [esp + 0x88]
00b4dfdc d9842484020000       fld dword ptr [esp + 0x284]
00b4dfe3 d88c2484000000       fmul dword ptr [esp + 0x84]
00b4dfea dec1                 faddp st(1)
00b4dfec d8442414             fadd dword ptr [esp + 0x14]
00b4dff0 d98424e8000000       fld dword ptr [esp + 0xe8]
00b4dff7 d8442478             fadd dword ptr [esp + 0x78]
00b4dffb d8442414             fadd dword ptr [esp + 0x14]
00b4dfff d805d8de2201         fadd dword ptr [0x122ded8]
00b4e005 def9                 fdivp st(1)
00b4e007 d95c2428             fstp dword ptr [esp + 0x28]
00b4e00b d8542428             fcom dword ptr [esp + 0x28]
00b4e00f dfe0                 fnstsw ax
00b4e011 f6c441               test ah, 0x41
00b4e014 7506                 jne 0xb4e01c
00b4e016 ddd8                 fstp st(0)
00b4e018 d9442428             fld dword ptr [esp + 0x28]
00b4e01c d9442410             fld dword ptr [esp + 0x10]
00b4e020 d85c2428             fcomp dword ptr [esp + 0x28]
00b4e024 dfe0                 fnstsw ax
00b4e026 f6c405               test ah, 5
00b4e029 7a08                 jp 0xb4e033
00b4e02b 8b442428             mov eax, dword ptr [esp + 0x28]
00b4e02f 89442410             mov dword ptr [esp + 0x10], eax
00b4e033 d815dcde2201         fcom dword ptr [0x122dedc]
00b4e039 dfe0                 fnstsw ax
00b4e03b f6c405               test ah, 5
00b4e03e 0f8aa2000000         jp 0xb4e0e6
00b4e044 ddd8                 fstp st(0)
00b4e046 d9442410             fld dword ptr [esp + 0x10]
00b4e04a d81ddcde2201         fcomp dword ptr [0x122dedc]
00b4e050 dfe0                 fnstsw ax
00b4e052 f6c441               test ah, 0x41
00b4e055 750b                 jne 0xb4e062
00b4e057 d905dcde2201         fld dword ptr [0x122dedc]
00b4e05d e984000000           jmp 0xb4e0e6
00b4e062 d9442418             fld dword ptr [esp + 0x18]
00b4e066 c744243c00000000     mov dword ptr [esp + 0x3c], 0
00b4e06e d80de0de2201         fmul dword ptr [0x122dee0]
00b4e074 c744244c00000000     mov dword ptr [esp + 0x4c], 0
00b4e07c c744245400000080     mov dword ptr [esp + 0x54], 0x80000000
00b4e084 c744245800000080     mov dword ptr [esp + 0x58], 0x80000000
00b4e08c d95c2434             fstp dword ptr [esp + 0x34]
00b4e090 c744245c0000803f     mov dword ptr [esp + 0x5c], 0x3f800000
00b4e098 d9842484000000       fld dword ptr [esp + 0x84]
00b4e09f c744246400000080     mov dword ptr [esp + 0x64], 0x80000000
00b4e0a7 d80de0de2201         fmul dword ptr [0x122dee0]
00b4e0ad c744246800000080     mov dword ptr [esp + 0x68], 0x80000000
00b4e0b5 c744246c00000000     mov dword ptr [esp + 0x6c], 0
00b4e0bd d95c2438             fstp dword ptr [esp + 0x38]
00b4e0c1 d944241c             fld dword ptr [esp + 0x1c]
00b4e0c5 d80de0de2201         fmul dword ptr [0x122dee0]
00b4e0cb d95c2444             fstp dword ptr [esp + 0x44]
00b4e0cf d9842488000000       fld dword ptr [esp + 0x88]
00b4e0d6 d80de0de2201         fmul dword ptr [0x122dee0]
00b4e0dc d95c2448             fstp dword ptr [esp + 0x48]
00b4e0e0 d9442410             fld dword ptr [esp + 0x10]
00b4e0e4 d9e0                 fchs 
00b4e0e6 8a8bac020000         mov cl, byte ptr [ebx + 0x2ac]
00b4e0ec 84c9                 test cl, cl
00b4e0ee 0f8493040000         je 0xb4e587
00b4e0f4 d983b0020000         fld dword ptr [ebx + 0x2b0]
00b4e0fa d9c9                 fxch st(1)
00b4e0fc d8e1                 fsub st(1)
00b4e0fe d983b4020000         fld dword ptr [ebx + 0x2b4]
00b4e104 d8e2                 fsub st(2)
00b4e106 def9                 fdivp st(1)
00b4e108 ddd9                 fstp st(1)
00b4e10a d815dcde2201         fcom dword ptr [0x122dedc]
00b4e110 dfe0                 fnstsw ax
00b4e112 f6c405               test ah, 5
00b4e115 0f8b6c040000         jnp 0xb4e587
00b4e11b d815d8de2201         fcom dword ptr [0x122ded8]
00b4e121 dfe0                 fnstsw ax
00b4e123 f6c441               test ah, 0x41
00b4e126 750a                 jne 0xb4e132
00b4e128 ddd8                 fstp st(0)
00b4e12a d905d8de2201         fld dword ptr [0x122ded8]
00b4e130 eb11                 jmp 0xb4e143
00b4e132 d815dcde2201         fcom dword ptr [0x122dedc]
00b4e138 dfe0                 fnstsw ax
00b4e13a f6c441               test ah, 0x41
00b4e13d 0f8544040000         jne 0xb4e587
00b4e143 d983b8020000         fld dword ptr [ebx + 0x2b8]
00b4e149 d8b3bc020000         fdiv dword ptr [ebx + 0x2bc]
00b4e14f d905d8de2201         fld dword ptr [0x122ded8]
00b4e155 d8e2                 fsub st(2)
00b4e157 d8c9                 fmul st(1)
00b4e159 d8c2                 fadd st(2)
00b4e15b d8e1                 fsub st(1)
00b4e15d d8f9                 fdivr st(1)
00b4e15f d95c2410             fstp dword ptr [esp + 0x10]
00b4e163 d8742410             fdiv dword ptr [esp + 0x10]
00b4e167 d95c2418             fstp dword ptr [esp + 0x18]
00b4e16b ddd8                 fstp st(0)
00b4e16d d905d8de2201         fld dword ptr [0x122ded8]
00b4e173 d9442448             fld dword ptr [esp + 0x48]
00b4e177 d88c2468010000       fmul dword ptr [esp + 0x168]
00b4e17e d9442438             fld dword ptr [esp + 0x38]
00b4e182 d88c2464010000       fmul dword ptr [esp + 0x164]
00b4e189 dec1                 faddp st(1)
00b4e18b d8442420             fadd dword ptr [esp + 0x20]
00b4e18f d98424ec000000       fld dword ptr [esp + 0xec]
00b4e196 d88424d8000000       fadd dword ptr [esp + 0xd8]
00b4e19d d8442420             fadd dword ptr [esp + 0x20]
00b4e1a1 d805d8de2201         fadd dword ptr [0x122ded8]
00b4e1a7 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4e1ad d9842464010000       fld dword ptr [esp + 0x164]
00b4e1b4 d84c2434             fmul dword ptr [esp + 0x34]
00b4e1b8 d9442444             fld dword ptr [esp + 0x44]
00b4e1bc d88c2468010000       fmul dword ptr [esp + 0x168]
00b4e1c3 dec1                 faddp st(1)
00b4e1c5 d8442420             fadd dword ptr [esp + 0x20]
00b4e1c9 d8c9                 fmul st(1)
00b4e1cb d95c2424             fstp dword ptr [esp + 0x24]
00b4e1cf dec9                 fmulp st(1)
00b4e1d1 d9442410             fld dword ptr [esp + 0x10]
00b4e1d5 d8c1                 fadd st(1)
00b4e1d7 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4e1dd d9442424             fld dword ptr [esp + 0x24]
00b4e1e1 d8c9                 fmul st(1)
00b4e1e3 d9c9                 fxch st(1)
00b4e1e5 d8ca                 fmul st(2)
00b4e1e7 d99c24d0000000       fstp dword ptr [esp + 0xd0]
00b4e1ee d9e1                 fabs 
00b4e1f0 d8542418             fcom dword ptr [esp + 0x18]
00b4e1f4 dfe0                 fnstsw ax
00b4e1f6 f6c441               test ah, 0x41
00b4e1f9 753e                 jne 0xb4e239
00b4e1fb d8bc24d0000000       fdivr dword ptr [esp + 0xd0]
00b4e202 d84c2418             fmul dword ptr [esp + 0x18]
00b4e206 d9442410             fld dword ptr [esp + 0x10]
00b4e20a d8c9                 fmul st(1)
00b4e20c d905d8de2201         fld dword ptr [0x122ded8]
00b4e212 d8e2                 fsub st(2)
00b4e214 def9                 fdivp st(1)
00b4e216 d8f2                 fdiv st(2)
00b4e218 d95c2420             fstp dword ptr [esp + 0x20]
00b4e21c ddd8                 fstp st(0)
00b4e21e ddd8                 fstp st(0)
00b4e220 d905d8de2201         fld dword ptr [0x122ded8]
00b4e226 d85c2420             fcomp dword ptr [esp + 0x20]
00b4e22a dfe0                 fnstsw ax
00b4e22c f6c441               test ah, 0x41
00b4e22f 750c                 jne 0xb4e23d
00b4e231 ddd8                 fstp st(0)
00b4e233 d9442420             fld dword ptr [esp + 0x20]
00b4e237 eb04                 jmp 0xb4e23d
00b4e239 ddd8                 fstp st(0)
00b4e23b ddd8                 fstp st(0)
00b4e23d d9442448             fld dword ptr [esp + 0x48]
00b4e241 d88c2498010000       fmul dword ptr [esp + 0x198]
00b4e248 d9442438             fld dword ptr [esp + 0x38]
00b4e24c d88c2494010000       fmul dword ptr [esp + 0x194]
00b4e253 dec1                 faddp st(1)
00b4e255 d8442474             fadd dword ptr [esp + 0x74]
00b4e259 d98424f0000000       fld dword ptr [esp + 0xf0]
00b4e260 d88424e0000000       fadd dword ptr [esp + 0xe0]
00b4e267 d8442474             fadd dword ptr [esp + 0x74]
00b4e26b d805d8de2201         fadd dword ptr [0x122ded8]
00b4e271 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4e277 d9842494010000       fld dword ptr [esp + 0x194]
00b4e27e d84c2434             fmul dword ptr [esp + 0x34]
00b4e282 d9442444             fld dword ptr [esp + 0x44]
00b4e286 d88c2498010000       fmul dword ptr [esp + 0x198]
00b4e28d dec1                 faddp st(1)
00b4e28f d8442474             fadd dword ptr [esp + 0x74]
00b4e293 d8c9                 fmul st(1)
00b4e295 d95c2424             fstp dword ptr [esp + 0x24]
00b4e299 dec9                 fmulp st(1)
00b4e29b d9442410             fld dword ptr [esp + 0x10]
00b4e29f d8c1                 fadd st(1)
00b4e2a1 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4e2a7 d9442424             fld dword ptr [esp + 0x24]
00b4e2ab d8c9                 fmul st(1)
00b4e2ad d9c9                 fxch st(1)
00b4e2af d8ca                 fmul st(2)
00b4e2b1 d99c24d0000000       fstp dword ptr [esp + 0xd0]
00b4e2b8 d9e1                 fabs 
00b4e2ba d8542418             fcom dword ptr [esp + 0x18]
00b4e2be dfe0                 fnstsw ax
00b4e2c0 f6c441               test ah, 0x41
00b4e2c3 7538                 jne 0xb4e2fd
00b4e2c5 d8bc24d0000000       fdivr dword ptr [esp + 0xd0]
00b4e2cc d84c2418             fmul dword ptr [esp + 0x18]
00b4e2d0 d9442410             fld dword ptr [esp + 0x10]
00b4e2d4 d8c9                 fmul st(1)
00b4e2d6 d905d8de2201         fld dword ptr [0x122ded8]
00b4e2dc d8e2                 fsub st(2)
00b4e2de def9                 fdivp st(1)
00b4e2e0 d8f2                 fdiv st(2)
00b4e2e2 d95c2420             fstp dword ptr [esp + 0x20]
00b4e2e6 ddd8                 fstp st(0)
00b4e2e8 ddd8                 fstp st(0)
00b4e2ea d8542420             fcom dword ptr [esp + 0x20]
00b4e2ee dfe0                 fnstsw ax
00b4e2f0 f6c441               test ah, 0x41
00b4e2f3 750c                 jne 0xb4e301
00b4e2f5 ddd8                 fstp st(0)
00b4e2f7 d9442420             fld dword ptr [esp + 0x20]
00b4e2fb eb04                 jmp 0xb4e301
00b4e2fd ddd8                 fstp st(0)
00b4e2ff ddd8                 fstp st(0)
00b4e301 d9442448             fld dword ptr [esp + 0x48]
00b4e305 d88c2458020000       fmul dword ptr [esp + 0x258]
00b4e30c d9442438             fld dword ptr [esp + 0x38]
00b4e310 d88c2454020000       fmul dword ptr [esp + 0x254]
00b4e317 dec1                 faddp st(1)
00b4e319 d8442430             fadd dword ptr [esp + 0x30]
00b4e31d d98424e4000000       fld dword ptr [esp + 0xe4]
00b4e324 d88424dc000000       fadd dword ptr [esp + 0xdc]
00b4e32b d8442430             fadd dword ptr [esp + 0x30]
00b4e32f d805d8de2201         fadd dword ptr [0x122ded8]
00b4e335 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4e33b d9842454020000       fld dword ptr [esp + 0x254]
00b4e342 d84c2434             fmul dword ptr [esp + 0x34]
00b4e346 d9442444             fld dword ptr [esp + 0x44]
00b4e34a d88c2458020000       fmul dword ptr [esp + 0x258]
00b4e351 dec1                 faddp st(1)
00b4e353 d8442430             fadd dword ptr [esp + 0x30]
00b4e357 d8c9                 fmul st(1)
00b4e359 d95c2424             fstp dword ptr [esp + 0x24]
00b4e35d dec9                 fmulp st(1)
00b4e35f d9442410             fld dword ptr [esp + 0x10]
00b4e363 d8c1                 fadd st(1)
00b4e365 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4e36b d9442424             fld dword ptr [esp + 0x24]
00b4e36f d8c9                 fmul st(1)
00b4e371 d9c9                 fxch st(1)
00b4e373 d8ca                 fmul st(2)
00b4e375 d99c24d0000000       fstp dword ptr [esp + 0xd0]
00b4e37c d9e1                 fabs 
00b4e37e d8542418             fcom dword ptr [esp + 0x18]
00b4e382 dfe0                 fnstsw ax
00b4e384 f6c441               test ah, 0x41
00b4e387 7538                 jne 0xb4e3c1
00b4e389 d8bc24d0000000       fdivr dword ptr [esp + 0xd0]
00b4e390 d84c2418             fmul dword ptr [esp + 0x18]
00b4e394 d9442410             fld dword ptr [esp + 0x10]
00b4e398 d8c9                 fmul st(1)
00b4e39a d905d8de2201         fld dword ptr [0x122ded8]
00b4e3a0 d8e2                 fsub st(2)
00b4e3a2 def9                 fdivp st(1)
00b4e3a4 d8f2                 fdiv st(2)
00b4e3a6 d95c2420             fstp dword ptr [esp + 0x20]
00b4e3aa ddd8                 fstp st(0)
00b4e3ac ddd8                 fstp st(0)
00b4e3ae d8542420             fcom dword ptr [esp + 0x20]
00b4e3b2 dfe0                 fnstsw ax
00b4e3b4 f6c441               test ah, 0x41
00b4e3b7 750c                 jne 0xb4e3c5
00b4e3b9 ddd8                 fstp st(0)
00b4e3bb d9442420             fld dword ptr [esp + 0x20]
00b4e3bf eb04                 jmp 0xb4e3c5
00b4e3c1 ddd8                 fstp st(0)
00b4e3c3 ddd8                 fstp st(0)
00b4e3c5 d9442448             fld dword ptr [esp + 0x48]
00b4e3c9 d88c2488020000       fmul dword ptr [esp + 0x288]
00b4e3d0 d9442438             fld dword ptr [esp + 0x38]
00b4e3d4 d88c2484020000       fmul dword ptr [esp + 0x284]
00b4e3db dec1                 faddp st(1)
00b4e3dd d8442414             fadd dword ptr [esp + 0x14]
00b4e3e1 d98424e8000000       fld dword ptr [esp + 0xe8]
00b4e3e8 d8442478             fadd dword ptr [esp + 0x78]
00b4e3ec d8442414             fadd dword ptr [esp + 0x14]
00b4e3f0 d805d8de2201         fadd dword ptr [0x122ded8]
00b4e3f6 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4e3fc d9842484020000       fld dword ptr [esp + 0x284]
00b4e403 d84c2434             fmul dword ptr [esp + 0x34]
00b4e407 d9442444             fld dword ptr [esp + 0x44]
00b4e40b d88c2488020000       fmul dword ptr [esp + 0x288]
00b4e412 dec1                 faddp st(1)
00b4e414 d8442414             fadd dword ptr [esp + 0x14]
00b4e418 d8c9                 fmul st(1)
00b4e41a d95c2424             fstp dword ptr [esp + 0x24]
00b4e41e dec9                 fmulp st(1)
00b4e420 d9442410             fld dword ptr [esp + 0x10]
00b4e424 d8c1                 fadd st(1)
00b4e426 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4e42c d9442424             fld dword ptr [esp + 0x24]
00b4e430 d8c9                 fmul st(1)
00b4e432 d9c9                 fxch st(1)
00b4e434 d8ca                 fmul st(2)
00b4e436 d99c24d0000000       fstp dword ptr [esp + 0xd0]
00b4e43d d9e1                 fabs 
00b4e43f d8542418             fcom dword ptr [esp + 0x18]
00b4e443 dfe0                 fnstsw ax
00b4e445 f6c441               test ah, 0x41
00b4e448 7538                 jne 0xb4e482
00b4e44a d8bc24d0000000       fdivr dword ptr [esp + 0xd0]
00b4e451 d84c2418             fmul dword ptr [esp + 0x18]
00b4e455 d9442410             fld dword ptr [esp + 0x10]
00b4e459 d8c9                 fmul st(1)
00b4e45b d905d8de2201         fld dword ptr [0x122ded8]
00b4e461 d8e2                 fsub st(2)
00b4e463 def9                 fdivp st(1)
00b4e465 d8f2                 fdiv st(2)
00b4e467 d95c2420             fstp dword ptr [esp + 0x20]
00b4e46b ddd8                 fstp st(0)
00b4e46d ddd8                 fstp st(0)
00b4e46f d8542420             fcom dword ptr [esp + 0x20]
00b4e473 dfe0                 fnstsw ax
00b4e475 f6c441               test ah, 0x41
00b4e478 750c                 jne 0xb4e486
00b4e47a ddd8                 fstp st(0)
00b4e47c d9442420             fld dword ptr [esp + 0x20]
00b4e480 eb04                 jmp 0xb4e486
00b4e482 ddd8                 fstp st(0)
00b4e484 ddd8                 fstp st(0)
00b4e486 8d842460010000       lea eax, [esp + 0x160]
00b4e48d b90d000000           mov ecx, 0xd
00b4e492 d9c0                 fld st(0)
00b4e494 83c018               add eax, 0x18
00b4e497 49                   dec ecx
00b4e498 d848e0               fmul dword ptr [eax - 0x20]
00b4e49b d958e0               fstp dword ptr [eax - 0x20]
00b4e49e d9c0                 fld st(0)
00b4e4a0 d848e4               fmul dword ptr [eax - 0x1c]
00b4e4a3 d958e4               fstp dword ptr [eax - 0x1c]
00b4e4a6 d9c0                 fld st(0)
00b4e4a8 d848e8               fmul dword ptr [eax - 0x18]
00b4e4ab d958e8               fstp dword ptr [eax - 0x18]
00b4e4ae d9c0                 fld st(0)
00b4e4b0 d848ec               fmul dword ptr [eax - 0x14]
00b4e4b3 d958ec               fstp dword ptr [eax - 0x14]
00b4e4b6 d9c0                 fld st(0)
00b4e4b8 d848f0               fmul dword ptr [eax - 0x10]
00b4e4bb d958f0               fstp dword ptr [eax - 0x10]
00b4e4be d9c0                 fld st(0)
00b4e4c0 d848f4               fmul dword ptr [eax - 0xc]
00b4e4c3 d958f4               fstp dword ptr [eax - 0xc]
00b4e4c6 75ca                 jne 0xb4e492
00b4e4c8 d88bbc020000         fmul dword ptr [ebx + 0x2bc]
00b4e4ce 8d44243c             lea eax, [esp + 0x3c]
00b4e4d2 8d8c2490000000       lea ecx, [esp + 0x90]
00b4e4d9 ba04000000           mov edx, 4
00b4e4de d99b00010000         fstp dword ptr [ebx + 0x100]
00b4e4e4 d9442410             fld dword ptr [esp + 0x10]
00b4e4e8 d805d8de2201         fadd dword ptr [0x122ded8]
00b4e4ee 8bff                 mov edi, edi
00b4e4f0 d94004               fld dword ptr [eax + 4]
00b4e4f3 83c010               add eax, 0x10
00b4e4f6 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4e4fc 83c110               add ecx, 0x10
00b4e4ff 4a                   dec edx
00b4e500 d940f0               fld dword ptr [eax - 0x10]
00b4e503 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4e509 d9c1                 fld st(1)
00b4e50b d8c1                 fadd st(1)
00b4e50d d940ec               fld dword ptr [eax - 0x14]
00b4e510 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4e516 d9c4                 fld st(4)
00b4e518 d848e8               fmul dword ptr [eax - 0x18]
00b4e51b d8c1                 fadd st(1)
00b4e51d d8c2                 fadd st(2)
00b4e51f d959ec               fstp dword ptr [ecx - 0x14]
00b4e522 d940e8               fld dword ptr [eax - 0x18]
00b4e525 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4e52b d95c2414             fstp dword ptr [esp + 0x14]
00b4e52f d9c4                 fld st(4)
00b4e531 d848ec               fmul dword ptr [eax - 0x14]
00b4e534 d8442414             fadd dword ptr [esp + 0x14]
00b4e538 d8c2                 fadd st(2)
00b4e53a d959f0               fstp dword ptr [ecx - 0x10]
00b4e53d d9c4                 fld st(4)
00b4e53f d848f0               fmul dword ptr [eax - 0x10]
00b4e542 d8442414             fadd dword ptr [esp + 0x14]
00b4e546 d8c1                 fadd st(1)
00b4e548 d8c4                 fadd st(4)
00b4e54a d959f4               fstp dword ptr [ecx - 0xc]
00b4e54d ddd8                 fstp st(0)
00b4e54f ddd8                 fstp st(0)
00b4e551 d9442410             fld dword ptr [esp + 0x10]
00b4e555 d848f4               fmul dword ptr [eax - 0xc]
00b4e558 d8442414             fadd dword ptr [esp + 0x14]
00b4e55c d8c1                 fadd st(1)
00b4e55e d840ec               fadd dword ptr [eax - 0x14]
00b4e561 d959f8               fstp dword ptr [ecx - 8]
00b4e564 ddd8                 fstp st(0)
00b4e566 ddd8                 fstp st(0)
00b4e568 7586                 jne 0xb4e4f0
00b4e56a b910000000           mov ecx, 0x10
00b4e56f ddd8                 fstp st(0)
00b4e571 8db4248c000000       lea esi, [esp + 0x8c]
00b4e578 8d7c2434             lea edi, [esp + 0x34]
00b4e57c f3a5                 rep movsd dword ptr es:[edi], dword ptr [esi]
00b4e57e d9442434             fld dword ptr [esp + 0x34]
00b4e582 e943020000           jmp 0xb4e7ca
00b4e587 ddd8                 fstp st(0)
00b4e589 d984240c020000       fld dword ptr [esp + 0x20c]
00b4e590 d8a424dc010000       fsub dword ptr [esp + 0x1dc]
00b4e597 d9842410020000       fld dword ptr [esp + 0x210]
00b4e59e d8a424e0010000       fsub dword ptr [esp + 0x1e0]
00b4e5a5 d9c1                 fld st(1)
00b4e5a7 d95c2418             fstp dword ptr [esp + 0x18]
00b4e5ab d954241c             fst dword ptr [esp + 0x1c]
00b4e5af d984246c020000       fld dword ptr [esp + 0x26c]
00b4e5b6 d8a4247c010000       fsub dword ptr [esp + 0x17c]
00b4e5bd d95c2424             fstp dword ptr [esp + 0x24]
00b4e5c1 8b542424             mov edx, dword ptr [esp + 0x24]
00b4e5c5 d9842470020000       fld dword ptr [esp + 0x270]
00b4e5cc 89942484000000       mov dword ptr [esp + 0x84], edx
00b4e5d3 d8a42480010000       fsub dword ptr [esp + 0x180]
00b4e5da d95c2428             fstp dword ptr [esp + 0x28]
00b4e5de 8b442428             mov eax, dword ptr [esp + 0x28]
00b4e5e2 89842488000000       mov dword ptr [esp + 0x88], eax
00b4e5e9 d9c0                 fld st(0)
00b4e5eb d8c9                 fmul st(1)
00b4e5ed d9c2                 fld st(2)
00b4e5ef d8cb                 fmul st(3)
00b4e5f1 dec1                 faddp st(1)
00b4e5f3 d9442428             fld dword ptr [esp + 0x28]
00b4e5f7 d84c2428             fmul dword ptr [esp + 0x28]
00b4e5fb d9442424             fld dword ptr [esp + 0x24]
00b4e5ff d84c2424             fmul dword ptr [esp + 0x24]
00b4e603 dec1                 faddp st(1)
00b4e605 ded9                 fcompp 
00b4e607 dfe0                 fnstsw ax
00b4e609 f6c405               test ah, 5
00b4e60c 7a22                 jp 0xb4e630
00b4e60e d9e0                 fchs 
00b4e610 d95c2478             fstp dword ptr [esp + 0x78]
00b4e614 8b542478             mov edx, dword ptr [esp + 0x78]
00b4e618 89942484000000       mov dword ptr [esp + 0x84], edx
00b4e61f d95c247c             fstp dword ptr [esp + 0x7c]
00b4e623 8b44247c             mov eax, dword ptr [esp + 0x7c]
00b4e627 89842488000000       mov dword ptr [esp + 0x88], eax
00b4e62e eb24                 jmp 0xb4e654
00b4e630 8b542428             mov edx, dword ptr [esp + 0x28]
00b4e634 ddd8                 fstp st(0)
00b4e636 ddd8                 fstp st(0)
00b4e638 8bc2                 mov eax, edx
00b4e63a d9442424             fld dword ptr [esp + 0x24]
00b4e63e 89542478             mov dword ptr [esp + 0x78], edx
00b4e642 d9e0                 fchs 
00b4e644 89442418             mov dword ptr [esp + 0x18], eax
00b4e648 d95c247c             fstp dword ptr [esp + 0x7c]
00b4e64c 8b54247c             mov edx, dword ptr [esp + 0x7c]
00b4e650 8954241c             mov dword ptr [esp + 0x1c], edx
00b4e654 84c9                 test cl, cl
00b4e656 d9442418             fld dword ptr [esp + 0x18]
00b4e65a 8b842484000000       mov eax, dword ptr [esp + 0x84]
00b4e661 8b54241c             mov edx, dword ptr [esp + 0x1c]
00b4e665 89442438             mov dword ptr [esp + 0x38], eax
00b4e669 8b842488000000       mov eax, dword ptr [esp + 0x88]
00b4e670 c744243c00000000     mov dword ptr [esp + 0x3c], 0
00b4e678 c744244000000000     mov dword ptr [esp + 0x40], 0
00b4e680 89542444             mov dword ptr [esp + 0x44], edx
00b4e684 89442448             mov dword ptr [esp + 0x48], eax
00b4e688 c744244c00000000     mov dword ptr [esp + 0x4c], 0
00b4e690 c744245000000000     mov dword ptr [esp + 0x50], 0
00b4e698 c744245400000000     mov dword ptr [esp + 0x54], 0
00b4e6a0 c744245800000000     mov dword ptr [esp + 0x58], 0
00b4e6a8 c744245c0000803f     mov dword ptr [esp + 0x5c], 0x3f800000
00b4e6b0 c744246000000000     mov dword ptr [esp + 0x60], 0
00b4e6b8 c744246400000000     mov dword ptr [esp + 0x64], 0
00b4e6c0 c744246800000000     mov dword ptr [esp + 0x68], 0
00b4e6c8 c744246c00000000     mov dword ptr [esp + 0x6c], 0
00b4e6d0 c74424700000803f     mov dword ptr [esp + 0x70], 0x3f800000
00b4e6d8 0f84e0000000         je 0xb4e7be
00b4e6de d98424a4000000       fld dword ptr [esp + 0xa4]
00b4e6e5 8d842460010000       lea eax, [esp + 0x160]
00b4e6ec d8a42498000000       fsub dword ptr [esp + 0x98]
00b4e6f3 b90d000000           mov ecx, 0xd
00b4e6f8 d98424a8000000       fld dword ptr [esp + 0xa8]
00b4e6ff d8a4249c000000       fsub dword ptr [esp + 0x9c]
00b4e706 d98424ac000000       fld dword ptr [esp + 0xac]
00b4e70d d8a424a0000000       fsub dword ptr [esp + 0xa0]
00b4e714 d9c2                 fld st(2)
00b4e716 decb                 fmulp st(3)
00b4e718 d9c0                 fld st(0)
00b4e71a d8c9                 fmul st(1)
00b4e71c dec3                 faddp st(3)
00b4e71e d9c1                 fld st(1)
00b4e720 d8ca                 fmul st(2)
00b4e722 dec3                 faddp st(3)
00b4e724 d9ca                 fxch st(2)
00b4e726 d9fa                 fsqrt 
00b4e728 ddda                 fstp st(2)
00b4e72a ddd8                 fstp st(0)
00b4e72c d9059cf52201         fld dword ptr [0x122f59c]
00b4e732 d8c9                 fmul st(1)
00b4e734 d983bc020000         fld dword ptr [ebx + 0x2bc]
00b4e73a d905d8de2201         fld dword ptr [0x122ded8]
00b4e740 d8b3bc020000         fdiv dword ptr [ebx + 0x2bc]
00b4e746 d95c2478             fstp dword ptr [esp + 0x78]
00b4e74a d9c0                 fld st(0)
00b4e74c d8c9                 fmul st(1)
00b4e74e d9c2                 fld st(2)
00b4e750 d8cb                 fmul st(3)
00b4e752 dec1                 faddp st(1)
00b4e754 d9fa                 fsqrt 
00b4e756 ddda                 fstp st(2)
00b4e758 ddd8                 fstp st(0)
00b4e75a d8bbbc020000         fdivr dword ptr [ebx + 0x2bc]
00b4e760 d88bbc020000         fmul dword ptr [ebx + 0x2bc]
00b4e766 d84c2478             fmul dword ptr [esp + 0x78]
00b4e76a dec9                 fmulp st(1)
00b4e76c d8bbb8020000         fdivr dword ptr [ebx + 0x2b8]
00b4e772 d88bbc020000         fmul dword ptr [ebx + 0x2bc]
00b4e778 dcc0                 fadd st(0), st(0)
00b4e77a d99300010000         fst dword ptr [ebx + 0x100]
00b4e780 d84c2478             fmul dword ptr [esp + 0x78]
00b4e784 d9c0                 fld st(0)
00b4e786 83c018               add eax, 0x18
00b4e789 49                   dec ecx
00b4e78a d848e0               fmul dword ptr [eax - 0x20]
00b4e78d d958e0               fstp dword ptr [eax - 0x20]
00b4e790 d9c0                 fld st(0)
00b4e792 d848e4               fmul dword ptr [eax - 0x1c]
00b4e795 d958e4               fstp dword ptr [eax - 0x1c]
00b4e798 d9c0                 fld st(0)
00b4e79a d848e8               fmul dword ptr [eax - 0x18]
00b4e79d d958e8               fstp dword ptr [eax - 0x18]
00b4e7a0 d9c0                 fld st(0)
00b4e7a2 d848ec               fmul dword ptr [eax - 0x14]
00b4e7a5 d958ec               fstp dword ptr [eax - 0x14]
00b4e7a8 d9c0                 fld st(0)
00b4e7aa d848f0               fmul dword ptr [eax - 0x10]
00b4e7ad d958f0               fstp dword ptr [eax - 0x10]
00b4e7b0 d9c0                 fld st(0)
00b4e7b2 d848f4               fmul dword ptr [eax - 0xc]
00b4e7b5 d958f4               fstp dword ptr [eax - 0xc]
00b4e7b8 75ca                 jne 0xb4e784
00b4e7ba ddd8                 fstp st(0)
00b4e7bc eb0c                 jmp 0xb4e7ca
00b4e7be 8b8bbc020000         mov ecx, dword ptr [ebx + 0x2bc]
00b4e7c4 898b00010000         mov dword ptr [ebx + 0x100], ecx
00b4e7ca d983fc000000         fld dword ptr [ebx + 0xfc]
00b4e7d0 d81ddcde2201         fcomp dword ptr [0x122dedc]
00b4e7d6 dfe0                 fnstsw ax
00b4e7d8 f6c405               test ah, 5
00b4e7db 7a0c                 jp 0xb4e7e9
00b4e7dd c783fc00000000000000 mov dword ptr [ebx + 0xfc], 0
00b4e7e7 eb25                 jmp 0xb4e80e
00b4e7e9 d98300010000         fld dword ptr [ebx + 0x100]
00b4e7ef dc0db8232a01         fmul qword ptr [0x12a23b8]
00b4e7f5 d983fc000000         fld dword ptr [ebx + 0xfc]
00b4e7fb d8d9                 fcomp st(1)
00b4e7fd dfe0                 fnstsw ax
00b4e7ff f6c441               test ah, 0x41
00b4e802 7508                 jne 0xb4e80c
00b4e804 d99bfc000000         fstp dword ptr [ebx + 0xfc]
00b4e80a eb02                 jmp 0xb4e80e
00b4e80c ddd8                 fstp st(0)
00b4e80e d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e814 c7442418caf24971     mov dword ptr [esp + 0x18], 0x7149f2ca
00b4e81c d9442438             fld dword ptr [esp + 0x38]
00b4e820 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e826 d944243c             fld dword ptr [esp + 0x3c]
00b4e82a d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e830 d95c243c             fstp dword ptr [esp + 0x3c]
00b4e834 d9442444             fld dword ptr [esp + 0x44]
00b4e838 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e83e d95c2444             fstp dword ptr [esp + 0x44]
00b4e842 d9442448             fld dword ptr [esp + 0x48]
00b4e846 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e84c d95c2448             fstp dword ptr [esp + 0x48]
00b4e850 d944244c             fld dword ptr [esp + 0x4c]
00b4e854 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e85a d95c244c             fstp dword ptr [esp + 0x4c]
00b4e85e d9442454             fld dword ptr [esp + 0x54]
00b4e862 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e868 d95c2454             fstp dword ptr [esp + 0x54]
00b4e86c d9442458             fld dword ptr [esp + 0x58]
00b4e870 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e876 d95c2458             fstp dword ptr [esp + 0x58]
00b4e87a d944245c             fld dword ptr [esp + 0x5c]
00b4e87e d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e884 d95c245c             fstp dword ptr [esp + 0x5c]
00b4e888 d9442464             fld dword ptr [esp + 0x64]
00b4e88c d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e892 d95c2464             fstp dword ptr [esp + 0x64]
00b4e896 d9442468             fld dword ptr [esp + 0x68]
00b4e89a d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e8a0 d95c2468             fstp dword ptr [esp + 0x68]
00b4e8a4 d944246c             fld dword ptr [esp + 0x6c]
00b4e8a8 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e8ae d95c246c             fstp dword ptr [esp + 0x6c]
00b4e8b2 d9442440             fld dword ptr [esp + 0x40]
00b4e8b6 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e8bc d95c2414             fstp dword ptr [esp + 0x14]
00b4e8c0 d9c9                 fxch st(1)
00b4e8c2 d8442414             fadd dword ptr [esp + 0x14]
00b4e8c6 d95c2434             fstp dword ptr [esp + 0x34]
00b4e8ca d8442414             fadd dword ptr [esp + 0x14]
00b4e8ce d95c2438             fstp dword ptr [esp + 0x38]
00b4e8d2 d944243c             fld dword ptr [esp + 0x3c]
00b4e8d6 d8442414             fadd dword ptr [esp + 0x14]
00b4e8da d95c243c             fstp dword ptr [esp + 0x3c]
00b4e8de d9442450             fld dword ptr [esp + 0x50]
00b4e8e2 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e8e8 d9442444             fld dword ptr [esp + 0x44]
00b4e8ec d8c1                 fadd st(1)
00b4e8ee d95c2444             fstp dword ptr [esp + 0x44]
00b4e8f2 d9442448             fld dword ptr [esp + 0x48]
00b4e8f6 d8c1                 fadd st(1)
00b4e8f8 d95c2448             fstp dword ptr [esp + 0x48]
00b4e8fc d944244c             fld dword ptr [esp + 0x4c]
00b4e900 d8c1                 fadd st(1)
00b4e902 d95c244c             fstp dword ptr [esp + 0x4c]
00b4e906 ddd8                 fstp st(0)
00b4e908 d9442460             fld dword ptr [esp + 0x60]
00b4e90c d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e912 d9442454             fld dword ptr [esp + 0x54]
00b4e916 d8c1                 fadd st(1)
00b4e918 d95c2454             fstp dword ptr [esp + 0x54]
00b4e91c d9442458             fld dword ptr [esp + 0x58]
00b4e920 d8c1                 fadd st(1)
00b4e922 d95c2458             fstp dword ptr [esp + 0x58]
00b4e926 d944245c             fld dword ptr [esp + 0x5c]
00b4e92a d8c1                 fadd st(1)
00b4e92c d95c245c             fstp dword ptr [esp + 0x5c]
00b4e930 ddd8                 fstp st(0)
00b4e932 d9442470             fld dword ptr [esp + 0x70]
00b4e936 d80d9cf52201         fmul dword ptr [0x122f59c]
00b4e93c d9442464             fld dword ptr [esp + 0x64]
00b4e940 d8c1                 fadd st(1)
00b4e942 d95c2464             fstp dword ptr [esp + 0x64]
00b4e946 d9442468             fld dword ptr [esp + 0x68]
00b4e94a d8c1                 fadd st(1)
00b4e94c d95c2468             fstp dword ptr [esp + 0x68]
00b4e950 d944246c             fld dword ptr [esp + 0x6c]
00b4e954 d8c1                 fadd st(1)
00b4e956 d95c246c             fstp dword ptr [esp + 0x6c]
00b4e95a ddd8                 fstp st(0)
00b4e95c d905a8222a01         fld dword ptr [0x12a22a8]
00b4e962 c7442430caf249f1     mov dword ptr [esp + 0x30], 0xf149f2ca
00b4e96a c7442474caf249f1     mov dword ptr [esp + 0x74], 0xf149f2ca
00b4e972 8d8c2460010000       lea ecx, [esp + 0x160]
00b4e979 ba0d000000           mov edx, 0xd
00b4e97e 8bff                 mov edi, edi
00b4e980 d941f8               fld dword ptr [ecx - 8]
00b4e983 d941fc               fld dword ptr [ecx - 4]
00b4e986 d901                 fld dword ptr [ecx]
00b4e988 d9442458             fld dword ptr [esp + 0x58]
00b4e98c d8c9                 fmul st(1)
00b4e98e d9442448             fld dword ptr [esp + 0x48]
00b4e992 d8cb                 fmul st(3)
00b4e994 dec1                 faddp st(1)
00b4e996 d9442438             fld dword ptr [esp + 0x38]
00b4e99a d8cc                 fmul st(4)
00b4e99c dec1                 faddp st(1)
00b4e99e d8442468             fadd dword ptr [esp + 0x68]
00b4e9a2 d95c2410             fstp dword ptr [esp + 0x10]
00b4e9a6 d9442460             fld dword ptr [esp + 0x60]
00b4e9aa d8c9                 fmul st(1)
00b4e9ac d9442450             fld dword ptr [esp + 0x50]
00b4e9b0 d8cb                 fmul st(3)
00b4e9b2 dec1                 faddp st(1)
00b4e9b4 d9442440             fld dword ptr [esp + 0x40]
00b4e9b8 d8cc                 fmul st(4)
00b4e9ba dec1                 faddp st(1)
00b4e9bc d8442470             fadd dword ptr [esp + 0x70]
00b4e9c0 d905d8de2201         fld dword ptr [0x122ded8]
00b4e9c6 d8f1                 fdiv st(1)
00b4e9c8 d95c2414             fstp dword ptr [esp + 0x14]
00b4e9cc ddd8                 fstp st(0)
00b4e9ce d9442454             fld dword ptr [esp + 0x54]
00b4e9d2 d8c9                 fmul st(1)
00b4e9d4 d9cb                 fxch st(3)
00b4e9d6 d84c2434             fmul dword ptr [esp + 0x34]
00b4e9da dec3                 faddp st(3)
00b4e9dc d9442444             fld dword ptr [esp + 0x44]
00b4e9e0 d8ca                 fmul st(2)
00b4e9e2 dec3                 faddp st(3)
00b4e9e4 d9ca                 fxch st(2)
00b4e9e6 d8442464             fadd dword ptr [esp + 0x64]
00b4e9ea d84c2414             fmul dword ptr [esp + 0x14]
00b4e9ee ddda                 fstp st(2)
00b4e9f0 ddd8                 fstp st(0)
00b4e9f2 d9442410             fld dword ptr [esp + 0x10]
00b4e9f6 d84c2414             fmul dword ptr [esp + 0x14]
00b4e9fa d95c2428             fstp dword ptr [esp + 0x28]
00b4e9fe d9442430             fld dword ptr [esp + 0x30]
00b4ea02 d8d9                 fcomp st(1)
00b4ea04 dfe0                 fnstsw ax
00b4ea06 f6c405               test ah, 5
00b4ea09 7a04                 jp 0xb4ea0f
00b4ea0b d9542430             fst dword ptr [esp + 0x30]
00b4ea0f d9442418             fld dword ptr [esp + 0x18]
00b4ea13 d8d9                 fcomp st(1)
00b4ea15 dfe0                 fnstsw ax
00b4ea17 f6c441               test ah, 0x41
00b4ea1a 7506                 jne 0xb4ea22
00b4ea1c d95c2418             fstp dword ptr [esp + 0x18]
00b4ea20 eb02                 jmp 0xb4ea24
00b4ea22 ddd8                 fstp st(0)
00b4ea24 d9442474             fld dword ptr [esp + 0x74]
00b4ea28 d85c2428             fcomp dword ptr [esp + 0x28]
00b4ea2c dfe0                 fnstsw ax
00b4ea2e f6c405               test ah, 5
00b4ea31 7a08                 jp 0xb4ea3b
00b4ea33 8b442428             mov eax, dword ptr [esp + 0x28]
00b4ea37 89442474             mov dword ptr [esp + 0x74], eax
00b4ea3b d8542428             fcom dword ptr [esp + 0x28]
00b4ea3f dfe0                 fnstsw ax
00b4ea41 f6c441               test ah, 0x41
00b4ea44 7506                 jne 0xb4ea4c
00b4ea46 ddd8                 fstp st(0)
00b4ea48 d9442428             fld dword ptr [esp + 0x28]
00b4ea4c d94104               fld dword ptr [ecx + 4]
00b4ea4f d94108               fld dword ptr [ecx + 8]
00b4ea52 d9410c               fld dword ptr [ecx + 0xc]
00b4ea55 d9442458             fld dword ptr [esp + 0x58]
00b4ea59 d8c9                 fmul st(1)
00b4ea5b d9442448             fld dword ptr [esp + 0x48]
00b4ea5f d8cb                 fmul st(3)
00b4ea61 dec1                 faddp st(1)
00b4ea63 d9442438             fld dword ptr [esp + 0x38]
00b4ea67 d8cc                 fmul st(4)
00b4ea69 dec1                 faddp st(1)
00b4ea6b d8442468             fadd dword ptr [esp + 0x68]
00b4ea6f d95c2410             fstp dword ptr [esp + 0x10]
00b4ea73 d9442460             fld dword ptr [esp + 0x60]
00b4ea77 d8c9                 fmul st(1)
00b4ea79 d9442450             fld dword ptr [esp + 0x50]
00b4ea7d d8cb                 fmul st(3)
00b4ea7f dec1                 faddp st(1)
00b4ea81 d9442440             fld dword ptr [esp + 0x40]
00b4ea85 d8cc                 fmul st(4)
00b4ea87 dec1                 faddp st(1)
00b4ea89 d8442470             fadd dword ptr [esp + 0x70]
00b4ea8d d905d8de2201         fld dword ptr [0x122ded8]
00b4ea93 d8f1                 fdiv st(1)
00b4ea95 d95c2414             fstp dword ptr [esp + 0x14]
00b4ea99 ddd8                 fstp st(0)
00b4ea9b d9442454             fld dword ptr [esp + 0x54]
00b4ea9f d8c9                 fmul st(1)
00b4eaa1 d9cb                 fxch st(3)
00b4eaa3 d84c2434             fmul dword ptr [esp + 0x34]
00b4eaa7 dec3                 faddp st(3)
00b4eaa9 d9442444             fld dword ptr [esp + 0x44]
00b4eaad d8ca                 fmul st(2)
00b4eaaf dec3                 faddp st(3)
00b4eab1 d9ca                 fxch st(2)
00b4eab3 d8442464             fadd dword ptr [esp + 0x64]
00b4eab7 d84c2414             fmul dword ptr [esp + 0x14]
00b4eabb ddda                 fstp st(2)
00b4eabd ddd8                 fstp st(0)
00b4eabf d9442410             fld dword ptr [esp + 0x10]
00b4eac3 d84c2414             fmul dword ptr [esp + 0x14]
00b4eac7 d95c2428             fstp dword ptr [esp + 0x28]
00b4eacb d9442430             fld dword ptr [esp + 0x30]
00b4eacf d8d9                 fcomp st(1)
00b4ead1 dfe0                 fnstsw ax
00b4ead3 f6c405               test ah, 5
00b4ead6 7a04                 jp 0xb4eadc
00b4ead8 d9542430             fst dword ptr [esp + 0x30]
00b4eadc d9442418             fld dword ptr [esp + 0x18]
00b4eae0 d8d9                 fcomp st(1)
00b4eae2 dfe0                 fnstsw ax
00b4eae4 f6c441               test ah, 0x41
00b4eae7 7506                 jne 0xb4eaef
00b4eae9 d95c2418             fstp dword ptr [esp + 0x18]
00b4eaed eb02                 jmp 0xb4eaf1
00b4eaef ddd8                 fstp st(0)
00b4eaf1 d9442474             fld dword ptr [esp + 0x74]
00b4eaf5 d85c2428             fcomp dword ptr [esp + 0x28]
00b4eaf9 dfe0                 fnstsw ax
00b4eafb f6c405               test ah, 5
00b4eafe 7a08                 jp 0xb4eb08
00b4eb00 8b442428             mov eax, dword ptr [esp + 0x28]
00b4eb04 89442474             mov dword ptr [esp + 0x74], eax
00b4eb08 d8542428             fcom dword ptr [esp + 0x28]
00b4eb0c dfe0                 fnstsw ax
00b4eb0e f6c441               test ah, 0x41
00b4eb11 7506                 jne 0xb4eb19
00b4eb13 ddd8                 fstp st(0)
00b4eb15 d9442428             fld dword ptr [esp + 0x28]
00b4eb19 83c118               add ecx, 0x18
00b4eb1c 4a                   dec edx
00b4eb1d 0f855dfeffff         jne 0xb4e980
00b4eb23 d9442418             fld dword ptr [esp + 0x18]
00b4eb27 d9e0                 fchs 
00b4eb29 d9c1                 fld st(1)
00b4eb2b d9e0                 fchs 
00b4eb2d d9442440             fld dword ptr [esp + 0x40]
00b4eb31 d8ca                 fmul st(2)
00b4eb33 d8442434             fadd dword ptr [esp + 0x34]
00b4eb37 d95c2434             fstp dword ptr [esp + 0x34]
00b4eb3b d9442440             fld dword ptr [esp + 0x40]
00b4eb3f d8c9                 fmul st(1)
00b4eb41 d8442438             fadd dword ptr [esp + 0x38]
00b4eb45 d95c2438             fstp dword ptr [esp + 0x38]
00b4eb49 d9442440             fld dword ptr [esp + 0x40]
00b4eb4d d80ddcde2201         fmul dword ptr [0x122dedc]
00b4eb53 d844243c             fadd dword ptr [esp + 0x3c]
00b4eb57 d95c243c             fstp dword ptr [esp + 0x3c]
00b4eb5b 8b4c243c             mov ecx, dword ptr [esp + 0x3c]
00b4eb5f d9442450             fld dword ptr [esp + 0x50]
00b4eb63 894c243c             mov dword ptr [esp + 0x3c], ecx
00b4eb67 d8ca                 fmul st(2)
00b4eb69 d8442444             fadd dword ptr [esp + 0x44]
00b4eb6d d95c2444             fstp dword ptr [esp + 0x44]
00b4eb71 d9442450             fld dword ptr [esp + 0x50]
00b4eb75 d8c9                 fmul st(1)
00b4eb77 d8442448             fadd dword ptr [esp + 0x48]
00b4eb7b d95c2448             fstp dword ptr [esp + 0x48]
00b4eb7f d9442450             fld dword ptr [esp + 0x50]
00b4eb83 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4eb89 d844244c             fadd dword ptr [esp + 0x4c]
00b4eb8d d95c244c             fstp dword ptr [esp + 0x4c]
00b4eb91 8b54244c             mov edx, dword ptr [esp + 0x4c]
00b4eb95 d9442460             fld dword ptr [esp + 0x60]
00b4eb99 8954244c             mov dword ptr [esp + 0x4c], edx
00b4eb9d d8ca                 fmul st(2)
00b4eb9f d8442454             fadd dword ptr [esp + 0x54]
00b4eba3 d95c2454             fstp dword ptr [esp + 0x54]
00b4eba7 d9442460             fld dword ptr [esp + 0x60]
00b4ebab d8c9                 fmul st(1)
00b4ebad d8442458             fadd dword ptr [esp + 0x58]
00b4ebb1 d95c2458             fstp dword ptr [esp + 0x58]
00b4ebb5 d9442460             fld dword ptr [esp + 0x60]
00b4ebb9 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4ebbf d844245c             fadd dword ptr [esp + 0x5c]
00b4ebc3 d95c245c             fstp dword ptr [esp + 0x5c]
00b4ebc7 d9442470             fld dword ptr [esp + 0x70]
00b4ebcb d8ca                 fmul st(2)
00b4ebcd d8442464             fadd dword ptr [esp + 0x64]
00b4ebd1 d95c2464             fstp dword ptr [esp + 0x64]
00b4ebd5 d9442470             fld dword ptr [esp + 0x70]
00b4ebd9 d8c9                 fmul st(1)
00b4ebdb d8442468             fadd dword ptr [esp + 0x68]
00b4ebdf d95c2468             fstp dword ptr [esp + 0x68]
00b4ebe3 ddd8                 fstp st(0)
00b4ebe5 ddd8                 fstp st(0)
00b4ebe7 d9442470             fld dword ptr [esp + 0x70]
00b4ebeb d80ddcde2201         fmul dword ptr [0x122dedc]
00b4ebf1 d844246c             fadd dword ptr [esp + 0x6c]
00b4ebf5 d95c246c             fstp dword ptr [esp + 0x6c]
00b4ebf9 d9442430             fld dword ptr [esp + 0x30]
00b4ebfd d8642418             fsub dword ptr [esp + 0x18]
00b4ec01 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4ec07 d99c24cc000000       fstp dword ptr [esp + 0xcc]
00b4ec0e d86c2474             fsubr dword ptr [esp + 0x74]
00b4ec12 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4ec18 d98424cc000000       fld dword ptr [esp + 0xcc]
00b4ec1f d84c2434             fmul dword ptr [esp + 0x34]
00b4ec23 d95c2434             fstp dword ptr [esp + 0x34]
00b4ec27 d9442438             fld dword ptr [esp + 0x38]
00b4ec2b d8c9                 fmul st(1)
00b4ec2d d95c2438             fstp dword ptr [esp + 0x38]
00b4ec31 d9442444             fld dword ptr [esp + 0x44]
00b4ec35 d88c24cc000000       fmul dword ptr [esp + 0xcc]
00b4ec3c d95c2444             fstp dword ptr [esp + 0x44]
00b4ec40 d9442448             fld dword ptr [esp + 0x48]
00b4ec44 d8c9                 fmul st(1)
00b4ec46 d95c2448             fstp dword ptr [esp + 0x48]
00b4ec4a d9442454             fld dword ptr [esp + 0x54]
00b4ec4e d88c24cc000000       fmul dword ptr [esp + 0xcc]
00b4ec55 d95c2454             fstp dword ptr [esp + 0x54]
00b4ec59 d9442458             fld dword ptr [esp + 0x58]
00b4ec5d 8b44245c             mov eax, dword ptr [esp + 0x5c]
00b4ec61 d8c9                 fmul st(1)
00b4ec63 8944245c             mov dword ptr [esp + 0x5c], eax
00b4ec67 33c0                 xor eax, eax
00b4ec69 d95c2458             fstp dword ptr [esp + 0x58]
00b4ec6d d9442464             fld dword ptr [esp + 0x64]
00b4ec71 d88c24cc000000       fmul dword ptr [esp + 0xcc]
00b4ec78 d95c2464             fstp dword ptr [esp + 0x64]
00b4ec7c d84c2468             fmul dword ptr [esp + 0x68]
00b4ec80 d944246c             fld dword ptr [esp + 0x6c]
00b4ec84 85c0                 test eax, eax
00b4ec86 7508                 jne 0xb4ec90
00b4ec88 d905d8de2201         fld dword ptr [0x122ded8]
00b4ec8e eb06                 jmp 0xb4ec96
00b4ec90 d905dcde2201         fld dword ptr [0x122dedc]
00b4ec96 83f801               cmp eax, 1
00b4ec99 d99c848c000000       fstp dword ptr [esp + eax*4 + 0x8c]
00b4eca0 7508                 jne 0xb4ecaa
00b4eca2 d905d8de2201         fld dword ptr [0x122ded8]
00b4eca8 eb06                 jmp 0xb4ecb0
00b4ecaa d905dcde2201         fld dword ptr [0x122dedc]
00b4ecb0 83f802               cmp eax, 2
00b4ecb3 d99c849c000000       fstp dword ptr [esp + eax*4 + 0x9c]
00b4ecba 7508                 jne 0xb4ecc4
00b4ecbc d905d8de2201         fld dword ptr [0x122ded8]
00b4ecc2 eb06                 jmp 0xb4ecca
00b4ecc4 d905dcde2201         fld dword ptr [0x122dedc]
00b4ecca 83f803               cmp eax, 3
00b4eccd d99c84ac000000       fstp dword ptr [esp + eax*4 + 0xac]
00b4ecd4 7508                 jne 0xb4ecde
00b4ecd6 d905d8de2201         fld dword ptr [0x122ded8]
00b4ecdc eb06                 jmp 0xb4ece4
00b4ecde d905dcde2201         fld dword ptr [0x122dedc]
00b4ece4 d99c84bc000000       fstp dword ptr [esp + eax*4 + 0xbc]
00b4eceb 40                   inc eax
00b4ecec 83f804               cmp eax, 4
00b4ecef 7c93                 jl 0xb4ec84
00b4ecf1 8d430c               lea eax, [ebx + 0xc]
00b4ecf4 b910000000           mov ecx, 0x10
00b4ecf9 8db4248c000000       lea esi, [esp + 0x8c]
00b4ed00 8bf8                 mov edi, eax
00b4ed02 f3a5                 rep movsd dword ptr es:[edi], dword ptr [esi]
00b4ed04 d98310010000         fld dword ptr [ebx + 0x110]
00b4ed0a d9e0                 fchs 
00b4ed0c d98314010000         fld dword ptr [ebx + 0x114]
00b4ed12 d9e0                 fchs 
00b4ed14 d98318010000         fld dword ptr [ebx + 0x118]
00b4ed1a d9e0                 fchs 
00b4ed1c d99c24d4000000       fstp dword ptr [esp + 0xd4]
00b4ed23 d9c1                 fld st(1)
00b4ed25 d8480c               fmul dword ptr [eax + 0xc]
00b4ed28 d800                 fadd dword ptr [eax]
00b4ed2a 8d4808               lea ecx, [eax + 8]
00b4ed2d 8d942490000000       lea edx, [esp + 0x90]
00b4ed34 be04000000           mov esi, 4
00b4ed39 d918                 fstp dword ptr [eax]
00b4ed3b d9c0                 fld st(0)
00b4ed3d d8480c               fmul dword ptr [eax + 0xc]
00b4ed40 d84004               fadd dword ptr [eax + 4]
00b4ed43 d95804               fstp dword ptr [eax + 4]
00b4ed46 d98424d4000000       fld dword ptr [esp + 0xd4]
00b4ed4d d8480c               fmul dword ptr [eax + 0xc]
00b4ed50 d84008               fadd dword ptr [eax + 8]
00b4ed53 d95808               fstp dword ptr [eax + 8]
00b4ed56 d9c1                 fld st(1)
00b4ed58 d8481c               fmul dword ptr [eax + 0x1c]
00b4ed5b d84010               fadd dword ptr [eax + 0x10]
00b4ed5e d95810               fstp dword ptr [eax + 0x10]
00b4ed61 d9c0                 fld st(0)
00b4ed63 d8481c               fmul dword ptr [eax + 0x1c]
00b4ed66 d84014               fadd dword ptr [eax + 0x14]
00b4ed69 d95814               fstp dword ptr [eax + 0x14]
00b4ed6c d98424d4000000       fld dword ptr [esp + 0xd4]
00b4ed73 d8481c               fmul dword ptr [eax + 0x1c]
00b4ed76 d84018               fadd dword ptr [eax + 0x18]
00b4ed79 d95818               fstp dword ptr [eax + 0x18]
00b4ed7c d9c1                 fld st(1)
00b4ed7e d8482c               fmul dword ptr [eax + 0x2c]
00b4ed81 d84020               fadd dword ptr [eax + 0x20]
00b4ed84 d95820               fstp dword ptr [eax + 0x20]
00b4ed87 d9c0                 fld st(0)
00b4ed89 d8482c               fmul dword ptr [eax + 0x2c]
00b4ed8c d84024               fadd dword ptr [eax + 0x24]
00b4ed8f d95824               fstp dword ptr [eax + 0x24]
00b4ed92 d98424d4000000       fld dword ptr [esp + 0xd4]
00b4ed99 d8482c               fmul dword ptr [eax + 0x2c]
00b4ed9c d84028               fadd dword ptr [eax + 0x28]
00b4ed9f d95828               fstp dword ptr [eax + 0x28]
00b4eda2 d9c9                 fxch st(1)
00b4eda4 d8483c               fmul dword ptr [eax + 0x3c]
00b4eda7 d84030               fadd dword ptr [eax + 0x30]
00b4edaa d95830               fstp dword ptr [eax + 0x30]
00b4edad d8483c               fmul dword ptr [eax + 0x3c]
00b4edb0 d84034               fadd dword ptr [eax + 0x34]
00b4edb3 d95834               fstp dword ptr [eax + 0x34]
00b4edb6 d98424d4000000       fld dword ptr [esp + 0xd4]
00b4edbd d8483c               fmul dword ptr [eax + 0x3c]
00b4edc0 d84038               fadd dword ptr [eax + 0x38]
00b4edc3 d95838               fstp dword ptr [eax + 0x38]
00b4edc6 eb08                 jmp 0xb4edd0
00b4edc8 8da42400000000       lea esp, [esp]
00b4edcf 90                   nop 
00b4edd0 d9842400010000       fld dword ptr [esp + 0x100]
00b4edd7 83c110               add ecx, 0x10
00b4edda d849e8               fmul dword ptr [ecx - 0x18]
00b4eddd 83c210               add edx, 0x10
00b4ede0 4e                   dec esi
00b4ede1 d9842424010000       fld dword ptr [esp + 0x124]
00b4ede8 d849f4               fmul dword ptr [ecx - 0xc]
00b4edeb dec1                 faddp st(1)
00b4eded d984240c010000       fld dword ptr [esp + 0x10c]
00b4edf4 d849ec               fmul dword ptr [ecx - 0x14]
00b4edf7 dec1                 faddp st(1)
00b4edf9 d9842418010000       fld dword ptr [esp + 0x118]
00b4ee00 d849f0               fmul dword ptr [ecx - 0x10]
00b4ee03 dec1                 faddp st(1)
00b4ee05 d95aec               fstp dword ptr [edx - 0x14]
00b4ee08 d9842428010000       fld dword ptr [esp + 0x128]
00b4ee0f d849f4               fmul dword ptr [ecx - 0xc]
00b4ee12 d9842404010000       fld dword ptr [esp + 0x104]
00b4ee19 d849e8               fmul dword ptr [ecx - 0x18]
00b4ee1c dec1                 faddp st(1)
00b4ee1e d9842410010000       fld dword ptr [esp + 0x110]
00b4ee25 d849ec               fmul dword ptr [ecx - 0x14]
00b4ee28 dec1                 faddp st(1)
00b4ee2a d984241c010000       fld dword ptr [esp + 0x11c]
00b4ee31 d849f0               fmul dword ptr [ecx - 0x10]
00b4ee34 dec1                 faddp st(1)
00b4ee36 d95af0               fstp dword ptr [edx - 0x10]
00b4ee39 d984242c010000       fld dword ptr [esp + 0x12c]
00b4ee40 d849f4               fmul dword ptr [ecx - 0xc]
00b4ee43 d9842408010000       fld dword ptr [esp + 0x108]
00b4ee4a d849e8               fmul dword ptr [ecx - 0x18]
00b4ee4d dec1                 faddp st(1)
00b4ee4f d9842414010000       fld dword ptr [esp + 0x114]
00b4ee56 d849ec               fmul dword ptr [ecx - 0x14]
00b4ee59 dec1                 faddp st(1)
00b4ee5b d9842420010000       fld dword ptr [esp + 0x120]
00b4ee62 d849f0               fmul dword ptr [ecx - 0x10]
00b4ee65 dec1                 faddp st(1)
00b4ee67 d95af4               fstp dword ptr [edx - 0xc]
00b4ee6a d941e8               fld dword ptr [ecx - 0x18]
00b4ee6d d841f0               fadd dword ptr [ecx - 0x10]
00b4ee70 d841ec               fadd dword ptr [ecx - 0x14]
00b4ee73 d80ddcde2201         fmul dword ptr [0x122dedc]
00b4ee79 d841f4               fadd dword ptr [ecx - 0xc]
00b4ee7c d95af8               fstp dword ptr [edx - 8]
00b4ee7f 0f854bffffff         jne 0xb4edd0
00b4ee85 b910000000           mov ecx, 0x10
00b4ee8a 8db4248c000000       lea esi, [esp + 0x8c]
00b4ee91 8bf8                 mov edi, eax
00b4ee93 f3a5                 rep movsd dword ptr es:[edi], dword ptr [esi]
00b4ee95 8d4808               lea ecx, [eax + 8]
00b4ee98 8d942404010000       lea edx, [esp + 0x104]
00b4ee9f be04000000           mov esi, 4
00b4eea4 d9442434             fld dword ptr [esp + 0x34]
00b4eea8 83c110               add ecx, 0x10
00b4eeab d849e8               fmul dword ptr [ecx - 0x18]
00b4eeae 83c210               add edx, 0x10
00b4eeb1 4e                   dec esi
00b4eeb2 d9442464             fld dword ptr [esp + 0x64]
00b4eeb6 d849f4               fmul dword ptr [ecx - 0xc]
00b4eeb9 dec1                 faddp st(1)
00b4eebb d9442454             fld dword ptr [esp + 0x54]
00b4eebf d849f0               fmul dword ptr [ecx - 0x10]
00b4eec2 dec1                 faddp st(1)
00b4eec4 d9442444             fld dword ptr [esp + 0x44]
00b4eec8 d849ec               fmul dword ptr [ecx - 0x14]
00b4eecb dec1                 faddp st(1)
00b4eecd d95aec               fstp dword ptr [edx - 0x14]
00b4eed0 d9c1                 fld st(1)
00b4eed2 d849f4               fmul dword ptr [ecx - 0xc]
00b4eed5 d9442458             fld dword ptr [esp + 0x58]
00b4eed9 d849f0               fmul dword ptr [ecx - 0x10]
00b4eedc dec1                 faddp st(1)
00b4eede d9442448             fld dword ptr [esp + 0x48]
00b4eee2 d849ec               fmul dword ptr [ecx - 0x14]
00b4eee5 dec1                 faddp st(1)
00b4eee7 d9442438             fld dword ptr [esp + 0x38]
00b4eeeb d849e8               fmul dword ptr [ecx - 0x18]
00b4eeee dec1                 faddp st(1)
00b4eef0 d95af0               fstp dword ptr [edx - 0x10]
00b4eef3 d9c0                 fld st(0)
00b4eef5 d849f4               fmul dword ptr [ecx - 0xc]
00b4eef8 d944245c             fld dword ptr [esp + 0x5c]
00b4eefc d849f0               fmul dword ptr [ecx - 0x10]
00b4eeff dec1                 faddp st(1)
00b4ef01 d944244c             fld dword ptr [esp + 0x4c]
00b4ef05 d849ec               fmul dword ptr [ecx - 0x14]
00b4ef08 dec1                 faddp st(1)
00b4ef0a d944243c             fld dword ptr [esp + 0x3c]
00b4ef0e d849e8               fmul dword ptr [ecx - 0x18]
00b4ef11 dec1                 faddp st(1)
00b4ef13 d95af4               fstp dword ptr [edx - 0xc]
00b4ef16 d9442470             fld dword ptr [esp + 0x70]
00b4ef1a d849f4               fmul dword ptr [ecx - 0xc]
00b4ef1d d9442460             fld dword ptr [esp + 0x60]
00b4ef21 d849f0               fmul dword ptr [ecx - 0x10]
00b4ef24 dec1                 faddp st(1)
00b4ef26 d9442450             fld dword ptr [esp + 0x50]
00b4ef2a d849ec               fmul dword ptr [ecx - 0x14]
00b4ef2d dec1                 faddp st(1)
00b4ef2f d9442440             fld dword ptr [esp + 0x40]
00b4ef33 d849e8               fmul dword ptr [ecx - 0x18]
00b4ef36 dec1                 faddp st(1)
00b4ef38 d95af8               fstp dword ptr [edx - 8]
00b4ef3b 0f8563ffffff         jne 0xb4eea4
00b4ef41 8bf8                 mov edi, eax
00b4ef43 ddd8                 fstp st(0)
00b4ef45 b910000000           mov ecx, 0x10
00b4ef4a ddd8                 fstp st(0)
00b4ef4c 8db42400010000       lea esi, [esp + 0x100]
00b4ef53 f3a5                 rep movsd dword ptr es:[edi], dword ptr [esi]
00b4ef55 8bcb                 mov ecx, ebx
00b4ef57 e804cbffff           call 0xb4ba60
00b4ef5c 8b8c24e4030000       mov ecx, dword ptr [esp + 0x3e4]
00b4ef63 51                   push ecx
00b4ef64 8bcb                 mov ecx, ebx
00b4ef66 e855cdffff           call 0xb4bcc0
00b4ef6b 5f                   pop edi
00b4ef6c 5e                   pop esi
00b4ef6d 5d                   pop ebp
00b4ef6e 5b                   pop ebx
00b4ef6f 81c4d0030000         add esp, 0x3d0
00b4ef75 c20400               ret 4
