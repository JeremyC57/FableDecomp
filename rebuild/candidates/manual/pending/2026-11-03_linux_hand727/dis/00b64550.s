00b64550 83ec7c               sub esp, 0x7c
00b64553 a108b53c01           mov eax, dword ptr [0x13cb508]
00b64558 53                   push ebx
00b64559 55                   push ebp
00b6455a 8be9                 mov ebp, ecx
00b6455c 8d8808280000         lea ecx, [eax + 0x2808]
00b64562 56                   push esi
00b64563 8b31                 mov esi, dword ptr [ecx]
00b64565 8d90243a0000         lea edx, [eax + 0x3a24]
00b6456b 8954f008             mov dword ptr [eax + esi*8 + 8], edx
00b6456f ff01                 inc dword ptr [ecx]
00b64571 d1a014280000         shl dword ptr [eax + 0x2814], 1
00b64577 a18c6e4301           mov eax, dword ptr [0x1436e8c]
00b6457c 8b8898010000         mov ecx, dword ptr [eax + 0x198]
00b64582 8b11                 mov edx, dword ptr [ecx]
00b64584 57                   push edi
00b64585 896c2414             mov dword ptr [esp + 0x14], ebp
00b64589 ff5204               call dword ptr [edx + 4]
00b6458c 8b10                 mov edx, dword ptr [eax]
00b6458e 8bc8                 mov ecx, eax
00b64590 ff5204               call dword ptr [edx + 4]
00b64593 8b0d5c6e4301         mov ecx, dword ptr [0x1436e5c]
00b64599 6800000001           push 0x1000000
00b6459e 8944242c             mov dword ptr [esp + 0x2c], eax
00b645a2 e8198e0200           call 0xb8d3c0
00b645a7 d9542410             fst dword ptr [esp + 0x10]
00b645ab d81ddcde2201         fcomp dword ptr [0x122dedc]
00b645b1 dfe0                 fnstsw ax
00b645b3 f6c405               test ah, 5
00b645b6 0f8bc8090000         jnp 0xb64f84
00b645bc d9442410             fld dword ptr [esp + 0x10]
00b645c0 d81dd8de2201         fcomp dword ptr [0x122ded8]
00b645c6 dfe0                 fnstsw ax
00b645c8 f6c441               test ah, 0x41
00b645cb 750a                 jne 0xb645d7
00b645cd c74424100000803f     mov dword ptr [esp + 0x10], 0x3f800000
00b645d5 eb15                 jmp 0xb645ec
00b645d7 d9442410             fld dword ptr [esp + 0x10]
00b645db d81d24172301         fcomp dword ptr [0x1231724]
00b645e1 dfe0                 fnstsw ax
00b645e3 f6c405               test ah, 5
00b645e6 0f8b98090000         jnp 0xb64f84
00b645ec a19c6e4301           mov eax, dword ptr [0x1436e9c]
00b645f1 8b481c               mov ecx, dword ptr [eax + 0x1c]
00b645f4 d94018               fld dword ptr [eax + 0x18]
00b645f7 8b5020               mov edx, dword ptr [eax + 0x20]
00b645fa a1186e4301           mov eax, dword ptr [0x1436e18]
00b645ff 8b356c6f3901         mov esi, dword ptr [0x1396f6c]
00b64605 8b9820290000         mov ebx, dword ptr [eax + 0x2920]
00b6460b 3bde                 cmp ebx, esi
00b6460d 894c2444             mov dword ptr [esp + 0x44], ecx
00b64611 8d881c290000         lea ecx, [eax + 0x291c]
00b64617 89542448             mov dword ptr [esp + 0x48], edx
00b6461b 8bd0                 mov edx, eax
00b6461d 8bfe                 mov edi, esi
00b6461f 7455                 je 0xb64676
00b64621 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b64627 8b7108               mov esi, dword ptr [ecx + 8]
00b6462a 85f0                 test eax, esi
00b6462c 7522                 jne 0xb64650
00b6462e 0bf0                 or esi, eax
00b64630 897108               mov dword ptr [ecx + 8], esi
00b64633 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b64639 894cc208             mov dword ptr [edx + eax*8 + 8], ecx
00b6463d 8b8208280000         mov eax, dword ptr [edx + 0x2808]
00b64643 8b7104               mov esi, dword ptr [ecx + 4]
00b64646 8974c20c             mov dword ptr [edx + eax*8 + 0xc], esi
00b6464a ff8208280000         inc dword ptr [edx + 0x2808]
00b64650 8a4110               mov al, byte ptr [ecx + 0x10]
00b64653 84c0                 test al, al
00b64655 897904               mov dword ptr [ecx + 4], edi
00b64658 7517                 jne 0xb64671
00b6465a c6411001             mov byte ptr [ecx + 0x10], 1
00b6465e 8b820c280000         mov eax, dword ptr [edx + 0x280c]
00b64664 898c8208200000       mov dword ptr [edx + eax*4 + 0x2008], ecx
00b6466b ff820c280000         inc dword ptr [edx + 0x280c]
00b64671 a1186e4301           mov eax, dword ptr [0x1436e18]
00b64676 8b35746f3901         mov esi, dword ptr [0x1396f74]
00b6467c 8b9834290000         mov ebx, dword ptr [eax + 0x2934]
00b64682 3bde                 cmp ebx, esi
00b64684 8d9030290000         lea edx, [eax + 0x2930]
00b6468a 8bc8                 mov ecx, eax
00b6468c 8bfe                 mov edi, esi
00b6468e 7455                 je 0xb646e5
00b64690 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b64696 8b7208               mov esi, dword ptr [edx + 8]
00b64699 85f0                 test eax, esi
00b6469b 7522                 jne 0xb646bf
00b6469d 0bf0                 or esi, eax
00b6469f 897208               mov dword ptr [edx + 8], esi
00b646a2 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b646a8 8954c108             mov dword ptr [ecx + eax*8 + 8], edx
00b646ac 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b646b2 8b7204               mov esi, dword ptr [edx + 4]
00b646b5 8974c10c             mov dword ptr [ecx + eax*8 + 0xc], esi
00b646b9 ff8108280000         inc dword ptr [ecx + 0x2808]
00b646bf 8a4210               mov al, byte ptr [edx + 0x10]
00b646c2 84c0                 test al, al
00b646c4 897a04               mov dword ptr [edx + 4], edi
00b646c7 7517                 jne 0xb646e0
00b646c9 c6421001             mov byte ptr [edx + 0x10], 1
00b646cd 8b810c280000         mov eax, dword ptr [ecx + 0x280c]
00b646d3 89948108200000       mov dword ptr [ecx + eax*4 + 0x2008], edx
00b646da ff810c280000         inc dword ptr [ecx + 0x280c]
00b646e0 a1186e4301           mov eax, dword ptr [0x1436e18]
00b646e5 8bb0bc280000         mov esi, dword ptr [eax + 0x28bc]
00b646eb 8d90b8280000         lea edx, [eax + 0x28b8]
00b646f1 bf01000000           mov edi, 1
00b646f6 3bf7                 cmp esi, edi
00b646f8 8bc8                 mov ecx, eax
00b646fa 7455                 je 0xb64751
00b646fc 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b64702 8b7208               mov esi, dword ptr [edx + 8]
00b64705 85f0                 test eax, esi
00b64707 7522                 jne 0xb6472b
00b64709 0bf0                 or esi, eax
00b6470b 897208               mov dword ptr [edx + 8], esi
00b6470e 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b64714 8954c108             mov dword ptr [ecx + eax*8 + 8], edx
00b64718 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b6471e 8b7204               mov esi, dword ptr [edx + 4]
00b64721 8974c10c             mov dword ptr [ecx + eax*8 + 0xc], esi
00b64725 ff8108280000         inc dword ptr [ecx + 0x2808]
00b6472b 8a4210               mov al, byte ptr [edx + 0x10]
00b6472e 84c0                 test al, al
00b64730 897a04               mov dword ptr [edx + 4], edi
00b64733 7517                 jne 0xb6474c
00b64735 c6421001             mov byte ptr [edx + 0x10], 1
00b64739 8b810c280000         mov eax, dword ptr [ecx + 0x280c]
00b6473f 89948108200000       mov dword ptr [ecx + eax*4 + 0x2008], edx
00b64746 ff810c280000         inc dword ptr [ecx + 0x280c]
00b6474c a1186e4301           mov eax, dword ptr [0x1436e18]
00b64751 8bb06c280000         mov esi, dword ptr [eax + 0x286c]
00b64757 8d9068280000         lea edx, [eax + 0x2868]
00b6475d 33db                 xor ebx, ebx
00b6475f 3bf3                 cmp esi, ebx
00b64761 8bc8                 mov ecx, eax
00b64763 7450                 je 0xb647b5
00b64765 8b8014280000         mov eax, dword ptr [eax + 0x2814]
00b6476b 8b7208               mov esi, dword ptr [edx + 8]
00b6476e 85f0                 test eax, esi
00b64770 7522                 jne 0xb64794
00b64772 0bf0                 or esi, eax
00b64774 897208               mov dword ptr [edx + 8], esi
00b64777 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b6477d 8954c108             mov dword ptr [ecx + eax*8 + 8], edx
00b64781 8b8108280000         mov eax, dword ptr [ecx + 0x2808]
00b64787 8b7204               mov esi, dword ptr [edx + 4]
00b6478a 8974c10c             mov dword ptr [ecx + eax*8 + 0xc], esi
00b6478e ff8108280000         inc dword ptr [ecx + 0x2808]
00b64794 8a4210               mov al, byte ptr [edx + 0x10]
00b64797 84c0                 test al, al
00b64799 895a04               mov dword ptr [edx + 4], ebx
00b6479c 7517                 jne 0xb647b5
00b6479e c6421001             mov byte ptr [edx + 0x10], 1
00b647a2 8b810c280000         mov eax, dword ptr [ecx + 0x280c]
00b647a8 89948108200000       mov dword ptr [ecx + eax*4 + 0x2008], edx
00b647af ff810c280000         inc dword ptr [ecx + 0x280c]
00b647b5 d80dd42a2a01         fmul dword ptr [0x12a2ad4]
00b647bb 8b0da06e4301         mov ecx, dword ptr [0x1436ea0]
00b647c1 d9442444             fld dword ptr [esp + 0x44]
00b647c5 d80dd42a2a01         fmul dword ptr [0x12a2ad4]
00b647cb d9442448             fld dword ptr [esp + 0x48]
00b647cf d80dd42a2a01         fmul dword ptr [0x12a2ad4]
00b647d5 d95c2454             fstp dword ptr [esp + 0x54]
00b647d9 d9c9                 fxch st(1)
00b647db d84154               fadd dword ptr [ecx + 0x54]
00b647de d95c2440             fstp dword ptr [esp + 0x40]
00b647e2 d84158               fadd dword ptr [ecx + 0x58]
00b647e5 d9442454             fld dword ptr [esp + 0x54]
00b647e9 d8415c               fadd dword ptr [ecx + 0x5c]
00b647ec d9442440             fld dword ptr [esp + 0x40]
00b647f0 d9c2                 fld st(2)
00b647f2 d9c3                 fld st(3)
00b647f4 d8898c000000         fmul dword ptr [ecx + 0x8c]
00b647fa d9c3                 fld st(3)
00b647fc d88998000000         fmul dword ptr [ecx + 0x98]
00b64802 dec1                 faddp st(1)
00b64804 d9442440             fld dword ptr [esp + 0x40]
00b64808 d88980000000         fmul dword ptr [ecx + 0x80]
00b6480e dec1                 faddp st(1)
00b64810 d881a4000000         fadd dword ptr [ecx + 0xa4]
00b64816 d95c2440             fstp dword ptr [esp + 0x40]
00b6481a d9c1                 fld st(1)
00b6481c d88984000000         fmul dword ptr [ecx + 0x84]
00b64822 d9cc                 fxch st(4)
00b64824 d88990000000         fmul dword ptr [ecx + 0x90]
00b6482a dec4                 faddp st(4)
00b6482c d9c2                 fld st(2)
00b6482e d8899c000000         fmul dword ptr [ecx + 0x9c]
00b64834 dec4                 faddp st(4)
00b64836 d9cb                 fxch st(3)
00b64838 d881a8000000         fadd dword ptr [ecx + 0xa8]
00b6483e d95c2444             fstp dword ptr [esp + 0x44]
00b64842 d9ca                 fxch st(2)
00b64844 d88994000000         fmul dword ptr [ecx + 0x94]
00b6484a d9ca                 fxch st(2)
00b6484c d88988000000         fmul dword ptr [ecx + 0x88]
00b64852 dec2                 faddp st(2)
00b64854 d889a0000000         fmul dword ptr [ecx + 0xa0]
00b6485a dec1                 faddp st(1)
00b6485c d881ac000000         fadd dword ptr [ecx + 0xac]
00b64862 dc1520802401         fcom qword ptr [0x1248020]
00b64868 dfe0                 fnstsw ax
00b6486a f6c441               test ah, 0x41
00b6486d 0f850f070000         jne 0xb64f82
00b64873 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b64879 6800000047           push 0x47000000
00b6487e d9c0                 fld st(0)
00b64880 d889b8000000         fmul dword ptr [ecx + 0xb8]
00b64886 d84c2444             fmul dword ptr [esp + 0x44]
00b6488a d881c8000000         fadd dword ptr [ecx + 0xc8]
00b64890 d95c2444             fstp dword ptr [esp + 0x44]
00b64894 d889bc000000         fmul dword ptr [ecx + 0xbc]
00b6489a d84c2448             fmul dword ptr [esp + 0x48]
00b6489e d8a9cc000000         fsubr dword ptr [ecx + 0xcc]
00b648a4 d95c2448             fstp dword ptr [esp + 0x48]
00b648a8 e863d5e7ff           call 0x9e1e10
00b648ad 6800000047           push 0x47000000
00b648b2 d95c241c             fstp dword ptr [esp + 0x1c]
00b648b6 e885d5e7ff           call 0x9e1e40
00b648bb d95c241c             fstp dword ptr [esp + 0x1c]
00b648bf 8b8590010000         mov eax, dword ptr [ebp + 0x190]
00b648c5 8b4878               mov ecx, dword ptr [eax + 0x78]
00b648c8 8b5074               mov edx, dword ptr [eax + 0x74]
00b648cb 83c074               add eax, 0x74
00b648ce 2bca                 sub ecx, edx
00b648d0 b8f31aca6b           mov eax, 0x6bca1af3
00b648d5 f7e9                 imul ecx
00b648d7 c1fa05               sar edx, 5
00b648da 8bc2                 mov eax, edx
00b648dc c1e81f               shr eax, 0x1f
00b648df 03c2                 add eax, edx
00b648e1 3bc3                 cmp eax, ebx
00b648e3 8944243c             mov dword ptr [esp + 0x3c], eax
00b648e7 895c2424             mov dword ptr [esp + 0x24], ebx
00b648eb 0f8ed5030000         jle 0xb64cc6
00b648f1 8b8d90010000         mov ecx, dword ptr [ebp + 0x190]
00b648f7 8b5174               mov edx, dword ptr [ecx + 0x74]
00b648fa 8b741a48             mov esi, dword ptr [edx + ebx + 0x48]
00b648fe 85f6                 test esi, esi
00b64900 0f84c0030000         je 0xb64cc6
00b64906 8b2d986e4301         mov ebp, dword ptr [0x1436e98]
00b6490c 8b8d54090000         mov ecx, dword ptr [ebp + 0x954]
00b64912 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b64918 8bb9e0010000         mov edi, dword ptr [ecx + 0x1e0]
00b6491e 2bd7                 sub edx, edi
00b64920 b8e9a28b2e           mov eax, 0x2e8ba2e9
00b64925 f7ea                 imul edx
00b64927 c1fa03               sar edx, 3
00b6492a 8bc2                 mov eax, edx
00b6492c c1e81f               shr eax, 0x1f
00b6492f 03c2                 add eax, edx
00b64931 3bf0                 cmp esi, eax
00b64933 7332                 jae 0xb64967
00b64935 8bd6                 mov edx, esi
00b64937 6bd22c               imul edx, edx, 0x2c
00b6493a 8bc7                 mov eax, edi
00b6493c 8d3c02               lea edi, [edx + eax]
00b6493f 85ff                 test edi, edi
00b64941 7424                 je 0xb64967
00b64943 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b64949 2bd0                 sub edx, eax
00b6494b b8e9a28b2e           mov eax, 0x2e8ba2e9
00b64950 f7ea                 imul edx
00b64952 c1fa03               sar edx, 3
00b64955 8bc2                 mov eax, edx
00b64957 c1e81f               shr eax, 0x1f
00b6495a 03c2                 add eax, edx
00b6495c 3bf0                 cmp esi, eax
00b6495e 1bc0                 sbb eax, eax
00b64960 23c7                 and eax, edi
00b64962 8b4028               mov eax, dword ptr [eax + 0x28]
00b64965 eb02                 jmp 0xb64969
00b64967 33c0                 xor eax, eax
00b64969 8b4008               mov eax, dword ptr [eax + 8]
00b6496c 85c0                 test eax, eax
00b6496e 740c                 je 0xb6497c
00b64970 8b781c               mov edi, dword ptr [eax + 0x1c]
00b64973 85ff                 test edi, edi
00b64975 0f95c0               setne al
00b64978 84c0                 test al, al
00b6497a 7575                 jne 0xb649f1
00b6497c 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b64982 8bb9e0010000         mov edi, dword ptr [ecx + 0x1e0]
00b64988 2bd7                 sub edx, edi
00b6498a b8e9a28b2e           mov eax, 0x2e8ba2e9
00b6498f f7ea                 imul edx
00b64991 c1fa03               sar edx, 3
00b64994 8bc2                 mov eax, edx
00b64996 c1e81f               shr eax, 0x1f
00b64999 03c2                 add eax, edx
00b6499b 3bf0                 cmp esi, eax
00b6499d 7342                 jae 0xb649e1
00b6499f 8bd6                 mov edx, esi
00b649a1 6bd22c               imul edx, edx, 0x2c
00b649a4 8bc7                 mov eax, edi
00b649a6 8d3c02               lea edi, [edx + eax]
00b649a9 85ff                 test edi, edi
00b649ab 7434                 je 0xb649e1
00b649ad 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b649b3 8be8                 mov ebp, eax
00b649b5 2bd5                 sub edx, ebp
00b649b7 b8e9a28b2e           mov eax, 0x2e8ba2e9
00b649bc f7ea                 imul edx
00b649be c1fa03               sar edx, 3
00b649c1 8bca                 mov ecx, edx
00b649c3 c1e91f               shr ecx, 0x1f
00b649c6 03ca                 add ecx, edx
00b649c8 3bf1                 cmp esi, ecx
00b649ca 1bc0                 sbb eax, eax
00b649cc 23c7                 and eax, edi
00b649ce 8b4028               mov eax, dword ptr [eax + 0x28]
00b649d1 6a00                 push 0
00b649d3 6a00                 push 0
00b649d5 8bc8                 mov ecx, eax
00b649d7 e8449ce9ff           call 0x9fe620
00b649dc e9c9020000           jmp 0xb64caa
00b649e1 33c0                 xor eax, eax
00b649e3 50                   push eax
00b649e4 50                   push eax
00b649e5 8bc8                 mov ecx, eax
00b649e7 e8349ce9ff           call 0x9fe620
00b649ec e9b9020000           jmp 0xb64caa
00b649f1 c744242000000000     mov dword ptr [esp + 0x20], 0
00b649f9 8bbd54090000         mov edi, dword ptr [ebp + 0x954]
00b649ff 8b87e0010000         mov eax, dword ptr [edi + 0x1e0]
00b64a05 8b8fe4010000         mov ecx, dword ptr [edi + 0x1e4]
00b64a0b 2bc8                 sub ecx, eax
00b64a0d b8e9a28b2e           mov eax, 0x2e8ba2e9
00b64a12 f7e9                 imul ecx
00b64a14 c1fa03               sar edx, 3
00b64a17 8bc2                 mov eax, edx
00b64a19 c1e81f               shr eax, 0x1f
00b64a1c 03c2                 add eax, edx
00b64a1e 3bf0                 cmp esi, eax
00b64a20 730f                 jae 0xb64a31
00b64a22 8b87e0010000         mov eax, dword ptr [edi + 0x1e0]
00b64a28 6bf62c               imul esi, esi, 0x2c
00b64a2b 03f0                 add esi, eax
00b64a2d 8bce                 mov ecx, esi
00b64a2f eb02                 jmp 0xb64a33
00b64a31 33c9                 xor ecx, ecx
00b64a33 6a01                 push 1
00b64a35 6a00                 push 0
00b64a37 6a00                 push 0
00b64a39 8d54242c             lea edx, [esp + 0x2c]
00b64a3d 52                   push edx
00b64a3e e88daee9ff           call 0x9ff8d0
00b64a43 84c0                 test al, al
00b64a45 0f8410020000         je 0xb64c5b
00b64a4b 8b442420             mov eax, dword ptr [esp + 0x20]
00b64a4f 8b742414             mov esi, dword ptr [esp + 0x14]
00b64a53 8b8e90010000         mov ecx, dword ptr [esi + 0x190]
00b64a59 8b7808               mov edi, dword ptr [eax + 8]
00b64a5c 8b4174               mov eax, dword ptr [ecx + 0x74]
00b64a5f 8b54033c             mov edx, dword ptr [ebx + eax + 0x3c]
00b64a63 52                   push edx
00b64a64 e8a7d3e7ff           call 0x9e1e10
00b64a69 e802a00900           call 0xbfea70
00b64a6e d9442440             fld dword ptr [esp + 0x40]
00b64a72 d8642418             fsub dword ptr [esp + 0x18]
00b64a76 8be8                 mov ebp, eax
00b64a78 896c2438             mov dword ptr [esp + 0x38], ebp
00b64a7c d9542430             fst dword ptr [esp + 0x30]
00b64a80 d9442444             fld dword ptr [esp + 0x44]
00b64a84 d864241c             fsub dword ptr [esp + 0x1c]
00b64a88 d9542434             fst dword ptr [esp + 0x34]
00b64a8c d9c0                 fld st(0)
00b64a8e d8c9                 fmul st(1)
00b64a90 d9c2                 fld st(2)
00b64a92 d8cb                 fmul st(3)
00b64a94 dec1                 faddp st(1)
00b64a96 d9fa                 fsqrt 
00b64a98 ddda                 fstp st(2)
00b64a9a ddd8                 fstp st(0)
00b64a9c d944241c             fld dword ptr [esp + 0x1c]
00b64aa0 d84c241c             fmul dword ptr [esp + 0x1c]
00b64aa4 d9442418             fld dword ptr [esp + 0x18]
00b64aa8 d84c2418             fmul dword ptr [esp + 0x18]
00b64aac dec1                 faddp st(1)
00b64aae d9fa                 fsqrt 
00b64ab0 d9c9                 fxch st(1)
00b64ab2 d9c9                 fxch st(1)
00b64ab4 def9                 fdivp st(1)
00b64ab6 d815dcde2201         fcom dword ptr [0x122dedc]
00b64abc dfe0                 fnstsw ax
00b64abe f6c405               test ah, 5
00b64ac1 7a0a                 jp 0xb64acd
00b64ac3 ddd8                 fstp st(0)
00b64ac5 d905dcde2201         fld dword ptr [0x122dedc]
00b64acb eb15                 jmp 0xb64ae2
00b64acd d815d8de2201         fcom dword ptr [0x122ded8]
00b64ad3 dfe0                 fnstsw ax
00b64ad5 f6c441               test ah, 0x41
00b64ad8 7508                 jne 0xb64ae2
00b64ada ddd8                 fstp st(0)
00b64adc d905d8de2201         fld dword ptr [0x122ded8]
00b64ae2 8b8690010000         mov eax, dword ptr [esi + 0x190]
00b64ae8 8b4874               mov ecx, dword ptr [eax + 0x74]
00b64aeb 0fb6541942           movzx edx, byte ptr [ecx + ebx + 0x42]
00b64af0 0fb6441941           movzx eax, byte ptr [ecx + ebx + 0x41]
00b64af5 03cb                 add ecx, ebx
00b64af7 8954242c             mov dword ptr [esp + 0x2c], edx
00b64afb 0fb65140             movzx edx, byte ptr [ecx + 0x40]
00b64aff db44242c             fild dword ptr [esp + 0x2c]
00b64b03 8944242c             mov dword ptr [esp + 0x2c], eax
00b64b07 0fb64143             movzx eax, byte ptr [ecx + 0x43]
00b64b0b d80d24172301         fmul dword ptr [0x1231724]
00b64b11 db44242c             fild dword ptr [esp + 0x2c]
00b64b15 8954242c             mov dword ptr [esp + 0x2c], edx
00b64b19 8b542428             mov edx, dword ptr [esp + 0x28]
00b64b1d d80d24172301         fmul dword ptr [0x1231724]
00b64b23 51                   push ecx
00b64b24 db442430             fild dword ptr [esp + 0x30]
00b64b28 89442430             mov dword ptr [esp + 0x30], eax
00b64b2c 8b4254               mov eax, dword ptr [edx + 0x54]
00b64b2f 05b0000000           add eax, 0xb0
00b64b34 d80d24172301         fmul dword ptr [0x1231724]
00b64b3a 8bf4                 mov esi, esp
00b64b3c d95c2478             fstp dword ptr [esp + 0x78]
00b64b40 db442430             fild dword ptr [esp + 0x30]
00b64b44 d80d24172301         fmul dword ptr [0x1231724]
00b64b4a d95c247c             fstp dword ptr [esp + 0x7c]
00b64b4e d9c9                 fxch st(1)
00b64b50 d808                 fmul dword ptr [eax]
00b64b52 d99c2480000000       fstp dword ptr [esp + 0x80]
00b64b59 d84804               fmul dword ptr [eax + 4]
00b64b5c d9442478             fld dword ptr [esp + 0x78]
00b64b60 d84808               fmul dword ptr [eax + 8]
00b64b63 d944247c             fld dword ptr [esp + 0x7c]
00b64b67 d8480c               fmul dword ptr [eax + 0xc]
00b64b6a d99c248c000000       fstp dword ptr [esp + 0x8c]
00b64b71 d9842480000000       fld dword ptr [esp + 0x80]
00b64b78 d84c2414             fmul dword ptr [esp + 0x14]
00b64b7c d95c2450             fstp dword ptr [esp + 0x50]
00b64b80 d9c9                 fxch st(1)
00b64b82 d84c2414             fmul dword ptr [esp + 0x14]
00b64b86 d95c2454             fstp dword ptr [esp + 0x54]
00b64b8a d84c2414             fmul dword ptr [esp + 0x14]
00b64b8e d984248c000000       fld dword ptr [esp + 0x8c]
00b64b95 d84c2414             fmul dword ptr [esp + 0x14]
00b64b99 d95c245c             fstp dword ptr [esp + 0x5c]
00b64b9d d9442450             fld dword ptr [esp + 0x50]
00b64ba1 d8ca                 fmul st(2)
00b64ba3 d95c2460             fstp dword ptr [esp + 0x60]
00b64ba7 d9442454             fld dword ptr [esp + 0x54]
00b64bab d8ca                 fmul st(2)
00b64bad d95c2464             fstp dword ptr [esp + 0x64]
00b64bb1 d8c9                 fmul st(1)
00b64bb3 d95c2468             fstp dword ptr [esp + 0x68]
00b64bb7 d84c245c             fmul dword ptr [esp + 0x5c]
00b64bbb d94144               fld dword ptr [ecx + 0x44]
00b64bbe d9442434             fld dword ptr [esp + 0x34]
00b64bc2 d8c9                 fmul st(1)
00b64bc4 d844241c             fadd dword ptr [esp + 0x1c]
00b64bc8 d95c2430             fstp dword ptr [esp + 0x30]
00b64bcc d9442438             fld dword ptr [esp + 0x38]
00b64bd0 d8c9                 fmul st(1)
00b64bd2 d8442420             fadd dword ptr [esp + 0x20]
00b64bd6 d95c2434             fstp dword ptr [esp + 0x34]
00b64bda ddd8                 fstp st(0)
00b64bdc d9442460             fld dword ptr [esp + 0x60]
00b64be0 d80d14002301         fmul dword ptr [0x1230014]
00b64be6 e8859e0900           call 0xbfea70
00b64beb d9442464             fld dword ptr [esp + 0x64]
00b64bef d80d14002301         fmul dword ptr [0x1230014]
00b64bf5 884602               mov byte ptr [esi + 2], al
00b64bf8 e8739e0900           call 0xbfea70
00b64bfd d9442468             fld dword ptr [esp + 0x68]
00b64c01 d80d14002301         fmul dword ptr [0x1230014]
00b64c07 884601               mov byte ptr [esi + 1], al
00b64c0a e8619e0900           call 0xbfea70
00b64c0f d80d14002301         fmul dword ptr [0x1230014]
00b64c15 8806                 mov byte ptr [esi], al
00b64c17 e8549e0900           call 0xbfea70
00b64c1c 884603               mov byte ptr [esi + 3], al
00b64c1f 8d442d00             lea eax, [ebp + ebp]
00b64c23 89442438             mov dword ptr [esp + 0x38], eax
00b64c27 db442438             fild dword ptr [esp + 0x38]
00b64c2b 8b4c2418             mov ecx, dword ptr [esp + 0x18]
00b64c2f d95c2438             fstp dword ptr [esp + 0x38]
00b64c33 8b442438             mov eax, dword ptr [esp + 0x38]
00b64c37 db44243c             fild dword ptr [esp + 0x3c]
00b64c3b d9442434             fld dword ptr [esp + 0x34]
00b64c3f 50                   push eax
00b64c40 50                   push eax
00b64c41 d8e1                 fsub st(1)
00b64c43 83ec08               sub esp, 8
00b64c46 d95c2404             fstp dword ptr [esp + 4]
00b64c4a d9442440             fld dword ptr [esp + 0x40]
00b64c4e d8e1                 fsub st(1)
00b64c50 d91c24               fstp dword ptr [esp]
00b64c53 57                   push edi
00b64c54 ddd8                 fstp st(0)
00b64c56 e8d5e8ffff           call 0xb63530
00b64c5b 8b742420             mov esi, dword ptr [esp + 0x20]
00b64c5f 85f6                 test esi, esi
00b64c61 7447                 je 0xb64caa
00b64c63 ff0e                 dec dword ptr [esi]
00b64c65 833e00               cmp dword ptr [esi], 0
00b64c68 7540                 jne 0xb64caa
00b64c6a f6461a10             test byte ptr [esi + 0x1a], 0x10
00b64c6e 7407                 je 0xb64c77
00b64c70 8bce                 mov ecx, esi
00b64c72 e8297fe9ff           call 0x9fcba0
00b64c77 8b460c               mov eax, dword ptr [esi + 0xc]
00b64c7a 33ff                 xor edi, edi
00b64c7c 3bc7                 cmp eax, edi
00b64c7e 741b                 je 0xb64c9b
00b64c80 ff08                 dec dword ptr [eax]
00b64c82 8b460c               mov eax, dword ptr [esi + 0xc]
00b64c85 3938                 cmp dword ptr [eax], edi
00b64c87 7512                 jne 0xb64c9b
00b64c89 8b4808               mov ecx, dword ptr [eax + 8]
00b64c8c ff5004               call dword ptr [eax + 4]
00b64c8f 8b4e0c               mov ecx, dword ptr [esi + 0xc]
00b64c92 51                   push ecx
00b64c93 e8249d0900           call 0xbfe9bc
00b64c98 83c404               add esp, 4
00b64c9b 56                   push esi
00b64c9c 897e08               mov dword ptr [esi + 8], edi
00b64c9f 897e0c               mov dword ptr [esi + 0xc], edi
00b64ca2 e8159d0900           call 0xbfe9bc
00b64ca7 83c404               add esp, 4
00b64caa 8b442424             mov eax, dword ptr [esp + 0x24]
00b64cae 8b4c243c             mov ecx, dword ptr [esp + 0x3c]
00b64cb2 8b6c2414             mov ebp, dword ptr [esp + 0x14]
00b64cb6 40                   inc eax
00b64cb7 83c34c               add ebx, 0x4c
00b64cba 3bc1                 cmp eax, ecx
00b64cbc 89442424             mov dword ptr [esp + 0x24], eax
00b64cc0 0f8c2bfcffff         jl 0xb648f1
00b64cc6 8b9590010000         mov edx, dword ptr [ebp + 0x190]
00b64ccc 8b7250               mov esi, dword ptr [edx + 0x50]
00b64ccf 85f6                 test esi, esi
00b64cd1 0f84ad020000         je 0xb64f84
00b64cd7 8b2d986e4301         mov ebp, dword ptr [0x1436e98]
00b64cdd 8b8d54090000         mov ecx, dword ptr [ebp + 0x954]
00b64ce3 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b64ce9 8bb9e0010000         mov edi, dword ptr [ecx + 0x1e0]
00b64cef 2bd7                 sub edx, edi
00b64cf1 b8e9a28b2e           mov eax, 0x2e8ba2e9
00b64cf6 f7ea                 imul edx
00b64cf8 c1fa03               sar edx, 3
00b64cfb 8bc2                 mov eax, edx
00b64cfd c1e81f               shr eax, 0x1f
00b64d00 03c2                 add eax, edx
00b64d02 3bf0                 cmp esi, eax
00b64d04 7334                 jae 0xb64d3a
00b64d06 8bd6                 mov edx, esi
00b64d08 6bd22c               imul edx, edx, 0x2c
00b64d0b 8bc7                 mov eax, edi
00b64d0d 8d3c02               lea edi, [edx + eax]
00b64d10 85ff                 test edi, edi
00b64d12 7426                 je 0xb64d3a
00b64d14 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b64d1a 8bd8                 mov ebx, eax
00b64d1c 2bd3                 sub edx, ebx
00b64d1e b8e9a28b2e           mov eax, 0x2e8ba2e9
00b64d23 f7ea                 imul edx
00b64d25 c1fa03               sar edx, 3
00b64d28 8bc2                 mov eax, edx
00b64d2a c1e81f               shr eax, 0x1f
00b64d2d 03c2                 add eax, edx
00b64d2f 3bf0                 cmp esi, eax
00b64d31 1bc0                 sbb eax, eax
00b64d33 23c7                 and eax, edi
00b64d35 8b4028               mov eax, dword ptr [eax + 0x28]
00b64d38 eb02                 jmp 0xb64d3c
00b64d3a 33c0                 xor eax, eax
00b64d3c 8b4008               mov eax, dword ptr [eax + 8]
00b64d3f 85c0                 test eax, eax
00b64d41 7410                 je 0xb64d53
00b64d43 8b781c               mov edi, dword ptr [eax + 0x1c]
00b64d46 85ff                 test edi, edi
00b64d48 0f95c0               setne al
00b64d4b 84c0                 test al, al
00b64d4d 0f8595000000         jne 0xb64de8
00b64d53 8b81e0010000         mov eax, dword ptr [ecx + 0x1e0]
00b64d59 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b64d5f 2bd0                 sub edx, eax
00b64d61 b8e9a28b2e           mov eax, 0x2e8ba2e9
00b64d66 f7ea                 imul edx
00b64d68 c1fa03               sar edx, 3
00b64d6b 8bc2                 mov eax, edx
00b64d6d c1e81f               shr eax, 0x1f
00b64d70 03c2                 add eax, edx
00b64d72 3bf0                 cmp esi, eax
00b64d74 7354                 jae 0xb64dca
00b64d76 8b81e0010000         mov eax, dword ptr [ecx + 0x1e0]
00b64d7c 8bd6                 mov edx, esi
00b64d7e 6bd22c               imul edx, edx, 0x2c
00b64d81 8d3c02               lea edi, [edx + eax]
00b64d84 85ff                 test edi, edi
00b64d86 7442                 je 0xb64dca
00b64d88 8b91e4010000         mov edx, dword ptr [ecx + 0x1e4]
00b64d8e 8bd8                 mov ebx, eax
00b64d90 2bd3                 sub edx, ebx
00b64d92 b8e9a28b2e           mov eax, 0x2e8ba2e9
00b64d97 f7ea                 imul edx
00b64d99 c1fa03               sar edx, 3
00b64d9c 8bca                 mov ecx, edx
00b64d9e c1e91f               shr ecx, 0x1f
00b64da1 03ca                 add ecx, edx
00b64da3 3bf1                 cmp esi, ecx
00b64da5 1bc0                 sbb eax, eax
00b64da7 23c7                 and eax, edi
00b64da9 8b4028               mov eax, dword ptr [eax + 0x28]
00b64dac 6a00                 push 0
00b64dae 6a00                 push 0
00b64db0 8bc8                 mov ecx, eax
00b64db2 e86998e9ff           call 0x9fe620
00b64db7 8b0d08b53c01         mov ecx, dword ptr [0x13cb508]
00b64dbd e87e0aeaff           call 0xa05840
00b64dc2 5f                   pop edi
00b64dc3 5e                   pop esi
00b64dc4 5d                   pop ebp
00b64dc5 5b                   pop ebx
00b64dc6 83c47c               add esp, 0x7c
00b64dc9 c3                   ret 
00b64dca 33c0                 xor eax, eax
00b64dcc 50                   push eax
00b64dcd 50                   push eax
00b64dce 8bc8                 mov ecx, eax
00b64dd0 e84b98e9ff           call 0x9fe620
00b64dd5 8b0d08b53c01         mov ecx, dword ptr [0x13cb508]
00b64ddb e8600aeaff           call 0xa05840
00b64de0 5f                   pop edi
00b64de1 5e                   pop esi
00b64de2 5d                   pop ebp
00b64de3 5b                   pop ebx
00b64de4 83c47c               add esp, 0x7c
00b64de7 c3                   ret 
00b64de8 c744242000000000     mov dword ptr [esp + 0x20], 0
00b64df0 8bbd54090000         mov edi, dword ptr [ebp + 0x954]
00b64df6 8b97e0010000         mov edx, dword ptr [edi + 0x1e0]
00b64dfc 8b8fe4010000         mov ecx, dword ptr [edi + 0x1e4]
00b64e02 2bca                 sub ecx, edx
00b64e04 b8e9a28b2e           mov eax, 0x2e8ba2e9
00b64e09 f7e9                 imul ecx
00b64e0b c1fa03               sar edx, 3
00b64e0e 8bc2                 mov eax, edx
00b64e10 c1e81f               shr eax, 0x1f
00b64e13 03c2                 add eax, edx
00b64e15 3bf0                 cmp esi, eax
00b64e17 730f                 jae 0xb64e28
00b64e19 8b87e0010000         mov eax, dword ptr [edi + 0x1e0]
00b64e1f 6bf62c               imul esi, esi, 0x2c
00b64e22 03f0                 add esi, eax
00b64e24 8bce                 mov ecx, esi
00b64e26 eb02                 jmp 0xb64e2a
00b64e28 33c9                 xor ecx, ecx
00b64e2a 6a00                 push 0
00b64e2c 6a00                 push 0
00b64e2e 6a00                 push 0
00b64e30 8d54242c             lea edx, [esp + 0x2c]
00b64e34 52                   push edx
00b64e35 e896aae9ff           call 0x9ff8d0
00b64e3a 84c0                 test al, al
00b64e3c 0f84df000000         je 0xb64f21
00b64e42 8b442420             mov eax, dword ptr [esp + 0x20]
00b64e46 8b7808               mov edi, dword ptr [eax + 8]
00b64e49 8b5c2414             mov ebx, dword ptr [esp + 0x14]
00b64e4d 8b8390010000         mov eax, dword ptr [ebx + 0x190]
00b64e53 db404c               fild dword ptr [eax + 0x4c]
00b64e56 51                   push ecx
00b64e57 d91c24               fstp dword ptr [esp]
00b64e5a e8b1cfe7ff           call 0x9e1e10
00b64e5f e80c9c0900           call 0xbfea70
00b64e64 d9442410             fld dword ptr [esp + 0x10]
00b64e68 8b4c2428             mov ecx, dword ptr [esp + 0x28]
00b64e6c 8be8                 mov ebp, eax
00b64e6e 8b4154               mov eax, dword ptr [ecx + 0x54]
00b64e71 d888a0000000         fmul dword ptr [eax + 0xa0]
00b64e77 05a0000000           add eax, 0xa0
00b64e7c d9442410             fld dword ptr [esp + 0x10]
00b64e80 51                   push ecx
00b64e81 d84804               fmul dword ptr [eax + 4]
00b64e84 896c243c             mov dword ptr [esp + 0x3c], ebp
00b64e88 8bf4                 mov esi, esp
00b64e8a d95c2464             fstp dword ptr [esp + 0x64]
00b64e8e d9442414             fld dword ptr [esp + 0x14]
00b64e92 d84808               fmul dword ptr [eax + 8]
00b64e95 d95c2468             fstp dword ptr [esp + 0x68]
00b64e99 d9442414             fld dword ptr [esp + 0x14]
00b64e9d d8480c               fmul dword ptr [eax + 0xc]
00b64ea0 d95c246c             fstp dword ptr [esp + 0x6c]
00b64ea4 d80d14002301         fmul dword ptr [0x1230014]
00b64eaa e8c19b0900           call 0xbfea70
00b64eaf d9442464             fld dword ptr [esp + 0x64]
00b64eb3 d80d14002301         fmul dword ptr [0x1230014]
00b64eb9 884602               mov byte ptr [esi + 2], al
00b64ebc e8af9b0900           call 0xbfea70
00b64ec1 d9442468             fld dword ptr [esp + 0x68]
00b64ec5 d80d14002301         fmul dword ptr [0x1230014]
00b64ecb 884601               mov byte ptr [esi + 1], al
00b64ece e89d9b0900           call 0xbfea70
00b64ed3 d944246c             fld dword ptr [esp + 0x6c]
00b64ed7 d80d14002301         fmul dword ptr [0x1230014]
00b64edd 8806                 mov byte ptr [esi], al
00b64edf e88c9b0900           call 0xbfea70
00b64ee4 8d542d00             lea edx, [ebp + ebp]
00b64ee8 89542440             mov dword ptr [esp + 0x40], edx
00b64eec db442440             fild dword ptr [esp + 0x40]
00b64ef0 884603               mov byte ptr [esi + 3], al
00b64ef3 8bcb                 mov ecx, ebx
00b64ef5 d95c2440             fstp dword ptr [esp + 0x40]
00b64ef9 8b442440             mov eax, dword ptr [esp + 0x40]
00b64efd db44243c             fild dword ptr [esp + 0x3c]
00b64f01 d9442448             fld dword ptr [esp + 0x48]
00b64f05 50                   push eax
00b64f06 50                   push eax
00b64f07 d8e1                 fsub st(1)
00b64f09 83ec08               sub esp, 8
00b64f0c d95c2404             fstp dword ptr [esp + 4]
00b64f10 d9442454             fld dword ptr [esp + 0x54]
00b64f14 d8e1                 fsub st(1)
00b64f16 d91c24               fstp dword ptr [esp]
00b64f19 57                   push edi
00b64f1a ddd8                 fstp st(0)
00b64f1c e80fe6ffff           call 0xb63530
00b64f21 8b742420             mov esi, dword ptr [esp + 0x20]
00b64f25 33ff                 xor edi, edi
00b64f27 3bf7                 cmp esi, edi
00b64f29 7459                 je 0xb64f84
00b64f2b ff0e                 dec dword ptr [esi]
00b64f2d 393e                 cmp dword ptr [esi], edi
00b64f2f 7553                 jne 0xb64f84
00b64f31 f6461a10             test byte ptr [esi + 0x1a], 0x10
00b64f35 7407                 je 0xb64f3e
00b64f37 8bce                 mov ecx, esi
00b64f39 e8627ce9ff           call 0x9fcba0
00b64f3e 8b460c               mov eax, dword ptr [esi + 0xc]
00b64f41 3bc7                 cmp eax, edi
00b64f43 741b                 je 0xb64f60
00b64f45 ff08                 dec dword ptr [eax]
00b64f47 8b460c               mov eax, dword ptr [esi + 0xc]
00b64f4a 3938                 cmp dword ptr [eax], edi
00b64f4c 7512                 jne 0xb64f60
00b64f4e 8b4808               mov ecx, dword ptr [eax + 8]
00b64f51 ff5004               call dword ptr [eax + 4]
00b64f54 8b460c               mov eax, dword ptr [esi + 0xc]
00b64f57 50                   push eax
00b64f58 e85f9a0900           call 0xbfe9bc
00b64f5d 83c404               add esp, 4
00b64f60 56                   push esi
00b64f61 897e08               mov dword ptr [esi + 8], edi
00b64f64 897e0c               mov dword ptr [esi + 0xc], edi
00b64f67 e8509a0900           call 0xbfe9bc
00b64f6c 8b0d08b53c01         mov ecx, dword ptr [0x13cb508]
00b64f72 83c404               add esp, 4
00b64f75 e8c608eaff           call 0xa05840
00b64f7a 5f                   pop edi
00b64f7b 5e                   pop esi
00b64f7c 5d                   pop ebp
00b64f7d 5b                   pop ebx
00b64f7e 83c47c               add esp, 0x7c
00b64f81 c3                   ret 
00b64f82 ddd8                 fstp st(0)
00b64f84 8b0d08b53c01         mov ecx, dword ptr [0x13cb508]
00b64f8a e8b108eaff           call 0xa05840
00b64f8f 5f                   pop edi
00b64f90 5e                   pop esi
00b64f91 5d                   pop ebp
00b64f92 5b                   pop ebx
00b64f93 83c47c               add esp, 0x7c
00b64f96 c3                   ret 
