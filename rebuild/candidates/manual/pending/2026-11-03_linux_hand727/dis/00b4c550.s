00b4c550 81ecb0000000         sub esp, 0xb0
00b4c556 53                   push ebx
00b4c557 33db                 xor ebx, ebx
00b4c559 56                   push esi
00b4c55a 8db11c010000         lea esi, [ecx + 0x11c]
00b4c560 57                   push edi
00b4c561 8db9b4010000         lea edi, [ecx + 0x1b4]
00b4c567 83c10c               add ecx, 0xc
00b4c56a 899e80000000         mov dword ptr [esi + 0x80], ebx
00b4c570 889e94000000         mov byte ptr [esi + 0x94], bl
00b4c576 51                   push ecx
00b4c577 8d4c2410             lea ecx, [esp + 0x10]
00b4c57b 899f80000000         mov dword ptr [edi + 0x80], ebx
00b4c581 889f94000000         mov byte ptr [edi + 0x94], bl
00b4c587 895c2460             mov dword ptr [esp + 0x60], ebx
00b4c58b 895c2464             mov dword ptr [esp + 0x64], ebx
00b4c58f 895c2468             mov dword ptr [esp + 0x68], ebx
00b4c593 c744246c0000803f     mov dword ptr [esp + 0x6c], 0x3f800000
00b4c59b 895c2470             mov dword ptr [esp + 0x70], ebx
00b4c59f 895c2474             mov dword ptr [esp + 0x74], ebx
00b4c5a3 895c2478             mov dword ptr [esp + 0x78], ebx
00b4c5a7 c744247c0000803f     mov dword ptr [esp + 0x7c], 0x3f800000
00b4c5af 899c2480000000       mov dword ptr [esp + 0x80], ebx
00b4c5b6 c78424840000000000803f mov dword ptr [esp + 0x84], 0x3f800000
00b4c5c1 c78424880000000000803f mov dword ptr [esp + 0x88], 0x3f800000
00b4c5cc 899c248c000000       mov dword ptr [esp + 0x8c], ebx
00b4c5d3 899c2490000000       mov dword ptr [esp + 0x90], ebx
00b4c5da 899c2494000000       mov dword ptr [esp + 0x94], ebx
00b4c5e1 c78424980000000000803f mov dword ptr [esp + 0x98], 0x3f800000
00b4c5ec c784249c0000000000803f mov dword ptr [esp + 0x9c], 0x3f800000
00b4c5f7 899c24a0000000       mov dword ptr [esp + 0xa0], ebx
00b4c5fe c78424a40000000000803f mov dword ptr [esp + 0xa4], 0x3f800000
00b4c609 899c24a8000000       mov dword ptr [esp + 0xa8], ebx
00b4c610 c78424ac0000000000803f mov dword ptr [esp + 0xac], 0x3f800000
00b4c61b c78424b00000000000803f mov dword ptr [esp + 0xb0], 0x3f800000
00b4c626 c78424b40000000000803f mov dword ptr [esp + 0xb4], 0x3f800000
00b4c631 c78424b80000000000803f mov dword ptr [esp + 0xb8], 0x3f800000
00b4c63c c78424bc0000000000803f mov dword ptr [esp + 0xbc], 0x3f800000
00b4c647 e8b404f1ff           call 0xa5cb00
00b4c64c d9442430             fld dword ptr [esp + 0x30]
00b4c650 d84c2464             fmul dword ptr [esp + 0x64]
00b4c654 d9442420             fld dword ptr [esp + 0x20]
00b4c658 d84c2460             fmul dword ptr [esp + 0x60]
00b4c65c dec1                 faddp st(1)
00b4c65e d9442410             fld dword ptr [esp + 0x10]
00b4c662 d84c245c             fmul dword ptr [esp + 0x5c]
00b4c666 dec1                 faddp st(1)
00b4c668 d8442440             fadd dword ptr [esp + 0x40]
00b4c66c d9442434             fld dword ptr [esp + 0x34]
00b4c670 d84c2464             fmul dword ptr [esp + 0x64]
00b4c674 d9442424             fld dword ptr [esp + 0x24]
00b4c678 d84c2460             fmul dword ptr [esp + 0x60]
00b4c67c dec1                 faddp st(1)
00b4c67e d9442414             fld dword ptr [esp + 0x14]
00b4c682 d84c245c             fmul dword ptr [esp + 0x5c]
00b4c686 dec1                 faddp st(1)
00b4c688 d8442444             fadd dword ptr [esp + 0x44]
00b4c68c d9442438             fld dword ptr [esp + 0x38]
00b4c690 d84c2464             fmul dword ptr [esp + 0x64]
00b4c694 d9442428             fld dword ptr [esp + 0x28]
00b4c698 d84c2460             fmul dword ptr [esp + 0x60]
00b4c69c dec1                 faddp st(1)
00b4c69e d9442418             fld dword ptr [esp + 0x18]
00b4c6a2 d84c245c             fmul dword ptr [esp + 0x5c]
00b4c6a6 dec1                 faddp st(1)
00b4c6a8 d8442448             fadd dword ptr [esp + 0x48]
00b4c6ac d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4c6b2 d944242c             fld dword ptr [esp + 0x2c]
00b4c6b6 d84c2464             fmul dword ptr [esp + 0x64]
00b4c6ba d944241c             fld dword ptr [esp + 0x1c]
00b4c6be d84c2460             fmul dword ptr [esp + 0x60]
00b4c6c2 dec1                 faddp st(1)
00b4c6c4 d944240c             fld dword ptr [esp + 0xc]
00b4c6c8 d84c245c             fmul dword ptr [esp + 0x5c]
00b4c6cc dec1                 faddp st(1)
00b4c6ce d844243c             fadd dword ptr [esp + 0x3c]
00b4c6d2 d8c9                 fmul st(1)
00b4c6d4 d95c245c             fstp dword ptr [esp + 0x5c]
00b4c6d8 d9c0                 fld st(0)
00b4c6da d8cb                 fmul st(3)
00b4c6dc d95c2460             fstp dword ptr [esp + 0x60]
00b4c6e0 d8c9                 fmul st(1)
00b4c6e2 d95c2464             fstp dword ptr [esp + 0x64]
00b4c6e6 ddd8                 fstp st(0)
00b4c6e8 ddd8                 fstp st(0)
00b4c6ea d9442430             fld dword ptr [esp + 0x30]
00b4c6ee d84c2470             fmul dword ptr [esp + 0x70]
00b4c6f2 d9442420             fld dword ptr [esp + 0x20]
00b4c6f6 d84c246c             fmul dword ptr [esp + 0x6c]
00b4c6fa dec1                 faddp st(1)
00b4c6fc d9442410             fld dword ptr [esp + 0x10]
00b4c700 d84c2468             fmul dword ptr [esp + 0x68]
00b4c704 dec1                 faddp st(1)
00b4c706 d8442440             fadd dword ptr [esp + 0x40]
00b4c70a d9442434             fld dword ptr [esp + 0x34]
00b4c70e d84c2470             fmul dword ptr [esp + 0x70]
00b4c712 d9442424             fld dword ptr [esp + 0x24]
00b4c716 d84c246c             fmul dword ptr [esp + 0x6c]
00b4c71a dec1                 faddp st(1)
00b4c71c d9442414             fld dword ptr [esp + 0x14]
00b4c720 d84c2468             fmul dword ptr [esp + 0x68]
00b4c724 dec1                 faddp st(1)
00b4c726 d8442444             fadd dword ptr [esp + 0x44]
00b4c72a d9442438             fld dword ptr [esp + 0x38]
00b4c72e d84c2470             fmul dword ptr [esp + 0x70]
00b4c732 d9442428             fld dword ptr [esp + 0x28]
00b4c736 d84c246c             fmul dword ptr [esp + 0x6c]
00b4c73a dec1                 faddp st(1)
00b4c73c d9442418             fld dword ptr [esp + 0x18]
00b4c740 d84c2468             fmul dword ptr [esp + 0x68]
00b4c744 dec1                 faddp st(1)
00b4c746 d8442448             fadd dword ptr [esp + 0x48]
00b4c74a d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4c750 d944242c             fld dword ptr [esp + 0x2c]
00b4c754 d84c2470             fmul dword ptr [esp + 0x70]
00b4c758 d944241c             fld dword ptr [esp + 0x1c]
00b4c75c d84c246c             fmul dword ptr [esp + 0x6c]
00b4c760 dec1                 faddp st(1)
00b4c762 d944240c             fld dword ptr [esp + 0xc]
00b4c766 d84c2468             fmul dword ptr [esp + 0x68]
00b4c76a dec1                 faddp st(1)
00b4c76c d844243c             fadd dword ptr [esp + 0x3c]
00b4c770 d8c9                 fmul st(1)
00b4c772 d95c2468             fstp dword ptr [esp + 0x68]
00b4c776 d9c0                 fld st(0)
00b4c778 d8cb                 fmul st(3)
00b4c77a d95c246c             fstp dword ptr [esp + 0x6c]
00b4c77e d8c9                 fmul st(1)
00b4c780 d95c2470             fstp dword ptr [esp + 0x70]
00b4c784 ddd8                 fstp st(0)
00b4c786 ddd8                 fstp st(0)
00b4c788 d9442430             fld dword ptr [esp + 0x30]
00b4c78c d84c247c             fmul dword ptr [esp + 0x7c]
00b4c790 d9442420             fld dword ptr [esp + 0x20]
00b4c794 d84c2478             fmul dword ptr [esp + 0x78]
00b4c798 dec1                 faddp st(1)
00b4c79a d9442410             fld dword ptr [esp + 0x10]
00b4c79e d84c2474             fmul dword ptr [esp + 0x74]
00b4c7a2 dec1                 faddp st(1)
00b4c7a4 d8442440             fadd dword ptr [esp + 0x40]
00b4c7a8 d9442434             fld dword ptr [esp + 0x34]
00b4c7ac d84c247c             fmul dword ptr [esp + 0x7c]
00b4c7b0 d9442424             fld dword ptr [esp + 0x24]
00b4c7b4 d84c2478             fmul dword ptr [esp + 0x78]
00b4c7b8 dec1                 faddp st(1)
00b4c7ba d9442414             fld dword ptr [esp + 0x14]
00b4c7be d84c2474             fmul dword ptr [esp + 0x74]
00b4c7c2 dec1                 faddp st(1)
00b4c7c4 d8442444             fadd dword ptr [esp + 0x44]
00b4c7c8 d9442438             fld dword ptr [esp + 0x38]
00b4c7cc d84c247c             fmul dword ptr [esp + 0x7c]
00b4c7d0 d9442428             fld dword ptr [esp + 0x28]
00b4c7d4 d84c2478             fmul dword ptr [esp + 0x78]
00b4c7d8 dec1                 faddp st(1)
00b4c7da d9442418             fld dword ptr [esp + 0x18]
00b4c7de d84c2474             fmul dword ptr [esp + 0x74]
00b4c7e2 dec1                 faddp st(1)
00b4c7e4 d8442448             fadd dword ptr [esp + 0x48]
00b4c7e8 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4c7ee d944242c             fld dword ptr [esp + 0x2c]
00b4c7f2 d84c247c             fmul dword ptr [esp + 0x7c]
00b4c7f6 d944241c             fld dword ptr [esp + 0x1c]
00b4c7fa d84c2478             fmul dword ptr [esp + 0x78]
00b4c7fe dec1                 faddp st(1)
00b4c800 d944240c             fld dword ptr [esp + 0xc]
00b4c804 d84c2474             fmul dword ptr [esp + 0x74]
00b4c808 dec1                 faddp st(1)
00b4c80a d844243c             fadd dword ptr [esp + 0x3c]
00b4c80e d8c9                 fmul st(1)
00b4c810 d95c2474             fstp dword ptr [esp + 0x74]
00b4c814 d9c0                 fld st(0)
00b4c816 d8cb                 fmul st(3)
00b4c818 d95c2478             fstp dword ptr [esp + 0x78]
00b4c81c d8c9                 fmul st(1)
00b4c81e d95c247c             fstp dword ptr [esp + 0x7c]
00b4c822 ddd8                 fstp st(0)
00b4c824 ddd8                 fstp st(0)
00b4c826 d9442430             fld dword ptr [esp + 0x30]
00b4c82a d88c2488000000       fmul dword ptr [esp + 0x88]
00b4c831 d9442420             fld dword ptr [esp + 0x20]
00b4c835 d88c2484000000       fmul dword ptr [esp + 0x84]
00b4c83c dec1                 faddp st(1)
00b4c83e d9442410             fld dword ptr [esp + 0x10]
00b4c842 d88c2480000000       fmul dword ptr [esp + 0x80]
00b4c849 dec1                 faddp st(1)
00b4c84b d8442440             fadd dword ptr [esp + 0x40]
00b4c84f d9442434             fld dword ptr [esp + 0x34]
00b4c853 d88c2488000000       fmul dword ptr [esp + 0x88]
00b4c85a d9442424             fld dword ptr [esp + 0x24]
00b4c85e d88c2484000000       fmul dword ptr [esp + 0x84]
00b4c865 dec1                 faddp st(1)
00b4c867 d9442414             fld dword ptr [esp + 0x14]
00b4c86b d88c2480000000       fmul dword ptr [esp + 0x80]
00b4c872 dec1                 faddp st(1)
00b4c874 d8442444             fadd dword ptr [esp + 0x44]
00b4c878 d9442438             fld dword ptr [esp + 0x38]
00b4c87c d88c2488000000       fmul dword ptr [esp + 0x88]
00b4c883 d9442428             fld dword ptr [esp + 0x28]
00b4c887 d88c2484000000       fmul dword ptr [esp + 0x84]
00b4c88e dec1                 faddp st(1)
00b4c890 d9442418             fld dword ptr [esp + 0x18]
00b4c894 d88c2480000000       fmul dword ptr [esp + 0x80]
00b4c89b dec1                 faddp st(1)
00b4c89d d8442448             fadd dword ptr [esp + 0x48]
00b4c8a1 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4c8a7 d944242c             fld dword ptr [esp + 0x2c]
00b4c8ab d88c2488000000       fmul dword ptr [esp + 0x88]
00b4c8b2 d944241c             fld dword ptr [esp + 0x1c]
00b4c8b6 d88c2484000000       fmul dword ptr [esp + 0x84]
00b4c8bd dec1                 faddp st(1)
00b4c8bf d944240c             fld dword ptr [esp + 0xc]
00b4c8c3 d88c2480000000       fmul dword ptr [esp + 0x80]
00b4c8ca dec1                 faddp st(1)
00b4c8cc d844243c             fadd dword ptr [esp + 0x3c]
00b4c8d0 d8c9                 fmul st(1)
00b4c8d2 d99c2480000000       fstp dword ptr [esp + 0x80]
00b4c8d9 d9c0                 fld st(0)
00b4c8db d8cb                 fmul st(3)
00b4c8dd d99c2484000000       fstp dword ptr [esp + 0x84]
00b4c8e4 d8c9                 fmul st(1)
00b4c8e6 d99c2488000000       fstp dword ptr [esp + 0x88]
00b4c8ed ddd8                 fstp st(0)
00b4c8ef ddd8                 fstp st(0)
00b4c8f1 d9442430             fld dword ptr [esp + 0x30]
00b4c8f5 d88c2494000000       fmul dword ptr [esp + 0x94]
00b4c8fc d9442420             fld dword ptr [esp + 0x20]
00b4c900 d88c2490000000       fmul dword ptr [esp + 0x90]
00b4c907 dec1                 faddp st(1)
00b4c909 d9442410             fld dword ptr [esp + 0x10]
00b4c90d d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4c914 dec1                 faddp st(1)
00b4c916 d8442440             fadd dword ptr [esp + 0x40]
00b4c91a d9442434             fld dword ptr [esp + 0x34]
00b4c91e d88c2494000000       fmul dword ptr [esp + 0x94]
00b4c925 d9442424             fld dword ptr [esp + 0x24]
00b4c929 d88c2490000000       fmul dword ptr [esp + 0x90]
00b4c930 dec1                 faddp st(1)
00b4c932 d9442414             fld dword ptr [esp + 0x14]
00b4c936 d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4c93d dec1                 faddp st(1)
00b4c93f d8442444             fadd dword ptr [esp + 0x44]
00b4c943 d9442438             fld dword ptr [esp + 0x38]
00b4c947 d88c2494000000       fmul dword ptr [esp + 0x94]
00b4c94e d9442428             fld dword ptr [esp + 0x28]
00b4c952 d88c2490000000       fmul dword ptr [esp + 0x90]
00b4c959 dec1                 faddp st(1)
00b4c95b d9442418             fld dword ptr [esp + 0x18]
00b4c95f d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4c966 dec1                 faddp st(1)
00b4c968 d8442448             fadd dword ptr [esp + 0x48]
00b4c96c d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4c972 d944242c             fld dword ptr [esp + 0x2c]
00b4c976 d88c2494000000       fmul dword ptr [esp + 0x94]
00b4c97d d944241c             fld dword ptr [esp + 0x1c]
00b4c981 d88c2490000000       fmul dword ptr [esp + 0x90]
00b4c988 dec1                 faddp st(1)
00b4c98a d944240c             fld dword ptr [esp + 0xc]
00b4c98e d88c248c000000       fmul dword ptr [esp + 0x8c]
00b4c995 dec1                 faddp st(1)
00b4c997 d844243c             fadd dword ptr [esp + 0x3c]
00b4c99b d8c9                 fmul st(1)
00b4c99d d99c248c000000       fstp dword ptr [esp + 0x8c]
00b4c9a4 d9c0                 fld st(0)
00b4c9a6 d8cb                 fmul st(3)
00b4c9a8 d99c2490000000       fstp dword ptr [esp + 0x90]
00b4c9af d8c9                 fmul st(1)
00b4c9b1 d99c2494000000       fstp dword ptr [esp + 0x94]
00b4c9b8 ddd8                 fstp st(0)
00b4c9ba ddd8                 fstp st(0)
00b4c9bc d9442430             fld dword ptr [esp + 0x30]
00b4c9c0 d88c24a0000000       fmul dword ptr [esp + 0xa0]
00b4c9c7 d9442420             fld dword ptr [esp + 0x20]
00b4c9cb d88c249c000000       fmul dword ptr [esp + 0x9c]
00b4c9d2 dec1                 faddp st(1)
00b4c9d4 d9442410             fld dword ptr [esp + 0x10]
00b4c9d8 d88c2498000000       fmul dword ptr [esp + 0x98]
00b4c9df dec1                 faddp st(1)
00b4c9e1 d8442440             fadd dword ptr [esp + 0x40]
00b4c9e5 d9442434             fld dword ptr [esp + 0x34]
00b4c9e9 d88c24a0000000       fmul dword ptr [esp + 0xa0]
00b4c9f0 d9442424             fld dword ptr [esp + 0x24]
00b4c9f4 d88c249c000000       fmul dword ptr [esp + 0x9c]
00b4c9fb dec1                 faddp st(1)
00b4c9fd d9442414             fld dword ptr [esp + 0x14]
00b4ca01 d88c2498000000       fmul dword ptr [esp + 0x98]
00b4ca08 dec1                 faddp st(1)
00b4ca0a d8442444             fadd dword ptr [esp + 0x44]
00b4ca0e d9442438             fld dword ptr [esp + 0x38]
00b4ca12 d88c24a0000000       fmul dword ptr [esp + 0xa0]
00b4ca19 d9442428             fld dword ptr [esp + 0x28]
00b4ca1d d88c249c000000       fmul dword ptr [esp + 0x9c]
00b4ca24 dec1                 faddp st(1)
00b4ca26 d9442418             fld dword ptr [esp + 0x18]
00b4ca2a d88c2498000000       fmul dword ptr [esp + 0x98]
00b4ca31 dec1                 faddp st(1)
00b4ca33 d8442448             fadd dword ptr [esp + 0x48]
00b4ca37 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4ca3d d944242c             fld dword ptr [esp + 0x2c]
00b4ca41 d88c24a0000000       fmul dword ptr [esp + 0xa0]
00b4ca48 d944241c             fld dword ptr [esp + 0x1c]
00b4ca4c d88c249c000000       fmul dword ptr [esp + 0x9c]
00b4ca53 dec1                 faddp st(1)
00b4ca55 d944240c             fld dword ptr [esp + 0xc]
00b4ca59 d88c2498000000       fmul dword ptr [esp + 0x98]
00b4ca60 dec1                 faddp st(1)
00b4ca62 d844243c             fadd dword ptr [esp + 0x3c]
00b4ca66 d8c9                 fmul st(1)
00b4ca68 d99c2498000000       fstp dword ptr [esp + 0x98]
00b4ca6f d9c0                 fld st(0)
00b4ca71 d8cb                 fmul st(3)
00b4ca73 d99c249c000000       fstp dword ptr [esp + 0x9c]
00b4ca7a d8c9                 fmul st(1)
00b4ca7c d99c24a0000000       fstp dword ptr [esp + 0xa0]
00b4ca83 ddd8                 fstp st(0)
00b4ca85 ddd8                 fstp st(0)
00b4ca87 d9442430             fld dword ptr [esp + 0x30]
00b4ca8b d88c24ac000000       fmul dword ptr [esp + 0xac]
00b4ca92 d9442420             fld dword ptr [esp + 0x20]
00b4ca96 d88c24a8000000       fmul dword ptr [esp + 0xa8]
00b4ca9d dec1                 faddp st(1)
00b4ca9f d9442410             fld dword ptr [esp + 0x10]
00b4caa3 d88c24a4000000       fmul dword ptr [esp + 0xa4]
00b4caaa dec1                 faddp st(1)
00b4caac d8442440             fadd dword ptr [esp + 0x40]
00b4cab0 d9442434             fld dword ptr [esp + 0x34]
00b4cab4 d88c24ac000000       fmul dword ptr [esp + 0xac]
00b4cabb d9442424             fld dword ptr [esp + 0x24]
00b4cabf d88c24a8000000       fmul dword ptr [esp + 0xa8]
00b4cac6 dec1                 faddp st(1)
00b4cac8 d9442414             fld dword ptr [esp + 0x14]
00b4cacc d88c24a4000000       fmul dword ptr [esp + 0xa4]
00b4cad3 dec1                 faddp st(1)
00b4cad5 d8442444             fadd dword ptr [esp + 0x44]
00b4cad9 d9442438             fld dword ptr [esp + 0x38]
00b4cadd d88c24ac000000       fmul dword ptr [esp + 0xac]
00b4cae4 d9442428             fld dword ptr [esp + 0x28]
00b4cae8 d88c24a8000000       fmul dword ptr [esp + 0xa8]
00b4caef dec1                 faddp st(1)
00b4caf1 d9442418             fld dword ptr [esp + 0x18]
00b4caf5 d88c24a4000000       fmul dword ptr [esp + 0xa4]
00b4cafc dec1                 faddp st(1)
00b4cafe d8442448             fadd dword ptr [esp + 0x48]
00b4cb02 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4cb08 d944242c             fld dword ptr [esp + 0x2c]
00b4cb0c d88c24ac000000       fmul dword ptr [esp + 0xac]
00b4cb13 d944241c             fld dword ptr [esp + 0x1c]
00b4cb17 d88c24a8000000       fmul dword ptr [esp + 0xa8]
00b4cb1e dec1                 faddp st(1)
00b4cb20 d944240c             fld dword ptr [esp + 0xc]
00b4cb24 d88c24a4000000       fmul dword ptr [esp + 0xa4]
00b4cb2b dec1                 faddp st(1)
00b4cb2d d844243c             fadd dword ptr [esp + 0x3c]
00b4cb31 d8c9                 fmul st(1)
00b4cb33 d99c24a4000000       fstp dword ptr [esp + 0xa4]
00b4cb3a d9c0                 fld st(0)
00b4cb3c d8cb                 fmul st(3)
00b4cb3e d99c24a8000000       fstp dword ptr [esp + 0xa8]
00b4cb45 d8c9                 fmul st(1)
00b4cb47 d99c24ac000000       fstp dword ptr [esp + 0xac]
00b4cb4e ddd8                 fstp st(0)
00b4cb50 ddd8                 fstp st(0)
00b4cb52 d9442430             fld dword ptr [esp + 0x30]
00b4cb56 d88c24b8000000       fmul dword ptr [esp + 0xb8]
00b4cb5d d9442420             fld dword ptr [esp + 0x20]
00b4cb61 d88c24b4000000       fmul dword ptr [esp + 0xb4]
00b4cb68 dec1                 faddp st(1)
00b4cb6a d9442410             fld dword ptr [esp + 0x10]
00b4cb6e d88c24b0000000       fmul dword ptr [esp + 0xb0]
00b4cb75 dec1                 faddp st(1)
00b4cb77 d8442440             fadd dword ptr [esp + 0x40]
00b4cb7b d9442434             fld dword ptr [esp + 0x34]
00b4cb7f d88c24b8000000       fmul dword ptr [esp + 0xb8]
00b4cb86 d9442424             fld dword ptr [esp + 0x24]
00b4cb8a d88c24b4000000       fmul dword ptr [esp + 0xb4]
00b4cb91 dec1                 faddp st(1)
00b4cb93 d9442414             fld dword ptr [esp + 0x14]
00b4cb97 d88c24b0000000       fmul dword ptr [esp + 0xb0]
00b4cb9e dec1                 faddp st(1)
00b4cba0 d8442444             fadd dword ptr [esp + 0x44]
00b4cba4 d9442438             fld dword ptr [esp + 0x38]
00b4cba8 d88c24b8000000       fmul dword ptr [esp + 0xb8]
00b4cbaf d9442428             fld dword ptr [esp + 0x28]
00b4cbb3 d88c24b4000000       fmul dword ptr [esp + 0xb4]
00b4cbba dec1                 faddp st(1)
00b4cbbc d9442418             fld dword ptr [esp + 0x18]
00b4cbc0 d88c24b0000000       fmul dword ptr [esp + 0xb0]
00b4cbc7 dec1                 faddp st(1)
00b4cbc9 d8442448             fadd dword ptr [esp + 0x48]
00b4cbcd d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4cbd3 d944242c             fld dword ptr [esp + 0x2c]
00b4cbd7 d88c24b8000000       fmul dword ptr [esp + 0xb8]
00b4cbde d944241c             fld dword ptr [esp + 0x1c]
00b4cbe2 d88c24b4000000       fmul dword ptr [esp + 0xb4]
00b4cbe9 dec1                 faddp st(1)
00b4cbeb d944240c             fld dword ptr [esp + 0xc]
00b4cbef d88c24b0000000       fmul dword ptr [esp + 0xb0]
00b4cbf6 dec1                 faddp st(1)
00b4cbf8 d844243c             fadd dword ptr [esp + 0x3c]
00b4cbfc d8c9                 fmul st(1)
00b4cbfe 8d442468             lea eax, [esp + 0x68]
00b4cc02 50                   push eax
00b4cc03 8d4c2478             lea ecx, [esp + 0x78]
00b4cc07 d99c24b4000000       fstp dword ptr [esp + 0xb4]
00b4cc0e 51                   push ecx
00b4cc0f 8d542464             lea edx, [esp + 0x64]
00b4cc13 d9c0                 fld st(0)
00b4cc15 52                   push edx
00b4cc16 d8cb                 fmul st(3)
00b4cc18 8d4c2458             lea ecx, [esp + 0x58]
00b4cc1c 899e80000000         mov dword ptr [esi + 0x80], ebx
00b4cc22 889e94000000         mov byte ptr [esi + 0x94], bl
00b4cc28 d99c24c0000000       fstp dword ptr [esp + 0xc0]
00b4cc2f d8c9                 fmul st(1)
00b4cc31 d99c24c4000000       fstp dword ptr [esp + 0xc4]
00b4cc38 ddd8                 fstp st(0)
00b4cc3a ddd8                 fstp st(0)
00b4cc3c e82f55efff           call 0xa42170
00b4cc41 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b4cc47 8b4c244c             mov ecx, dword ptr [esp + 0x4c]
00b4cc4b c1e004               shl eax, 4
00b4cc4e 03c6                 add eax, esi
00b4cc50 8908                 mov dword ptr [eax], ecx
00b4cc52 8b542450             mov edx, dword ptr [esp + 0x50]
00b4cc56 895004               mov dword ptr [eax + 4], edx
00b4cc59 8b4c2454             mov ecx, dword ptr [esp + 0x54]
00b4cc5d 894808               mov dword ptr [eax + 8], ecx
00b4cc60 8b542458             mov edx, dword ptr [esp + 0x58]
00b4cc64 89500c               mov dword ptr [eax + 0xc], edx
00b4cc67 8b9680000000         mov edx, dword ptr [esi + 0x80]
00b4cc6d 42                   inc edx
00b4cc6e 8d8424a4000000       lea eax, [esp + 0xa4]
00b4cc75 50                   push eax
00b4cc76 8d8c249c000000       lea ecx, [esp + 0x9c]
00b4cc7d 899680000000         mov dword ptr [esi + 0x80], edx
00b4cc83 51                   push ecx
00b4cc84 8d942494000000       lea edx, [esp + 0x94]
00b4cc8b 52                   push edx
00b4cc8c 8d4c2458             lea ecx, [esp + 0x58]
00b4cc90 e8db54efff           call 0xa42170
00b4cc95 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b4cc9b 8b4c244c             mov ecx, dword ptr [esp + 0x4c]
00b4cc9f c1e004               shl eax, 4
00b4cca2 03c6                 add eax, esi
00b4cca4 8908                 mov dword ptr [eax], ecx
00b4cca6 8b542450             mov edx, dword ptr [esp + 0x50]
00b4ccaa 895004               mov dword ptr [eax + 4], edx
00b4ccad 8b4c2454             mov ecx, dword ptr [esp + 0x54]
00b4ccb1 894808               mov dword ptr [eax + 8], ecx
00b4ccb4 8b542458             mov edx, dword ptr [esp + 0x58]
00b4ccb8 89500c               mov dword ptr [eax + 0xc], edx
00b4ccbb 8b9680000000         mov edx, dword ptr [esi + 0x80]
00b4ccc1 42                   inc edx
00b4ccc2 8d442474             lea eax, [esp + 0x74]
00b4ccc6 50                   push eax
00b4ccc7 8d8c2490000000       lea ecx, [esp + 0x90]
00b4ccce 899680000000         mov dword ptr [esi + 0x80], edx
00b4ccd4 51                   push ecx
00b4ccd5 8d542464             lea edx, [esp + 0x64]
00b4ccd9 52                   push edx
00b4ccda 8d4c2458             lea ecx, [esp + 0x58]
00b4ccde e88d54efff           call 0xa42170
00b4cce3 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b4cce9 8b4c244c             mov ecx, dword ptr [esp + 0x4c]
00b4cced c1e004               shl eax, 4
00b4ccf0 03c6                 add eax, esi
00b4ccf2 8908                 mov dword ptr [eax], ecx
00b4ccf4 8b542450             mov edx, dword ptr [esp + 0x50]
00b4ccf8 895004               mov dword ptr [eax + 4], edx
00b4ccfb 8b4c2454             mov ecx, dword ptr [esp + 0x54]
00b4ccff 894808               mov dword ptr [eax + 8], ecx
00b4cd02 8b542458             mov edx, dword ptr [esp + 0x58]
00b4cd06 89500c               mov dword ptr [eax + 0xc], edx
00b4cd09 8b9680000000         mov edx, dword ptr [esi + 0x80]
00b4cd0f 8d842498000000       lea eax, [esp + 0x98]
00b4cd16 42                   inc edx
00b4cd17 50                   push eax
00b4cd18 8d8c2484000000       lea ecx, [esp + 0x84]
00b4cd1f 899680000000         mov dword ptr [esi + 0x80], edx
00b4cd25 51                   push ecx
00b4cd26 8d542470             lea edx, [esp + 0x70]
00b4cd2a 52                   push edx
00b4cd2b 8d4c2458             lea ecx, [esp + 0x58]
00b4cd2f e83c54efff           call 0xa42170
00b4cd34 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b4cd3a 8b4c244c             mov ecx, dword ptr [esp + 0x4c]
00b4cd3e c1e004               shl eax, 4
00b4cd41 03c6                 add eax, esi
00b4cd43 8908                 mov dword ptr [eax], ecx
00b4cd45 8b542450             mov edx, dword ptr [esp + 0x50]
00b4cd49 895004               mov dword ptr [eax + 4], edx
00b4cd4c 8b4c2454             mov ecx, dword ptr [esp + 0x54]
00b4cd50 894808               mov dword ptr [eax + 8], ecx
00b4cd53 8b542458             mov edx, dword ptr [esp + 0x58]
00b4cd57 89500c               mov dword ptr [eax + 0xc], edx
00b4cd5a 8b9680000000         mov edx, dword ptr [esi + 0x80]
00b4cd60 42                   inc edx
00b4cd61 8d84248c000000       lea eax, [esp + 0x8c]
00b4cd68 50                   push eax
00b4cd69 8d4c246c             lea ecx, [esp + 0x6c]
00b4cd6d 899680000000         mov dword ptr [esi + 0x80], edx
00b4cd73 51                   push ecx
00b4cd74 8d542464             lea edx, [esp + 0x64]
00b4cd78 52                   push edx
00b4cd79 8d4c2458             lea ecx, [esp + 0x58]
00b4cd7d e8ee53efff           call 0xa42170
00b4cd82 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b4cd88 8b4c244c             mov ecx, dword ptr [esp + 0x4c]
00b4cd8c c1e004               shl eax, 4
00b4cd8f 03c6                 add eax, esi
00b4cd91 8908                 mov dword ptr [eax], ecx
00b4cd93 8b542450             mov edx, dword ptr [esp + 0x50]
00b4cd97 895004               mov dword ptr [eax + 4], edx
00b4cd9a 8b4c2454             mov ecx, dword ptr [esp + 0x54]
00b4cd9e 894808               mov dword ptr [eax + 8], ecx
00b4cda1 8b542458             mov edx, dword ptr [esp + 0x58]
00b4cda5 89500c               mov dword ptr [eax + 0xc], edx
00b4cda8 8b9680000000         mov edx, dword ptr [esi + 0x80]
00b4cdae 42                   inc edx
00b4cdaf 8d842480000000       lea eax, [esp + 0x80]
00b4cdb6 50                   push eax
00b4cdb7 8d8c24a8000000       lea ecx, [esp + 0xa8]
00b4cdbe 899680000000         mov dword ptr [esi + 0x80], edx
00b4cdc4 51                   push ecx
00b4cdc5 8d54247c             lea edx, [esp + 0x7c]
00b4cdc9 52                   push edx
00b4cdca 8d4c2458             lea ecx, [esp + 0x58]
00b4cdce e89d53efff           call 0xa42170
00b4cdd3 8b8680000000         mov eax, dword ptr [esi + 0x80]
00b4cdd9 8b4c244c             mov ecx, dword ptr [esp + 0x4c]
00b4cddd c1e004               shl eax, 4
00b4cde0 03c6                 add eax, esi
00b4cde2 8908                 mov dword ptr [eax], ecx
00b4cde4 8b542450             mov edx, dword ptr [esp + 0x50]
00b4cde8 895004               mov dword ptr [eax + 4], edx
00b4cdeb 8b4c2454             mov ecx, dword ptr [esp + 0x54]
00b4cdef 894808               mov dword ptr [eax + 8], ecx
00b4cdf2 8b542458             mov edx, dword ptr [esp + 0x58]
00b4cdf6 89500c               mov dword ptr [eax + 0xc], edx
00b4cdf9 ff8680000000         inc dword ptr [esi + 0x80]
00b4cdff b926000000           mov ecx, 0x26
00b4ce04 f3a5                 rep movsd dword ptr es:[edi], dword ptr [esi]
00b4ce06 5f                   pop edi
00b4ce07 5e                   pop esi
00b4ce08 5b                   pop ebx
00b4ce09 81c4b0000000         add esp, 0xb0
00b4ce0f c20400               ret 4
