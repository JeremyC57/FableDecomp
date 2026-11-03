006ab820 83ec7c               sub esp, 0x7c
006ab823 55                   push ebp
006ab824 56                   push esi
006ab825 8bf1                 mov esi, ecx
006ab827 8b8e00010000         mov ecx, dword ptr [esi + 0x100]
006ab82d 33ed                 xor ebp, ebp
006ab82f 3bcd                 cmp ecx, ebp
006ab831 7464                 je 0x6ab897
006ab833 8a4161               mov al, byte ptr [ecx + 0x61]
006ab836 84c0                 test al, al
006ab838 755d                 jne 0x6ab897
006ab83a 8b01                 mov eax, dword ptr [ecx]
006ab83c ff90c4000000         call dword ptr [eax + 0xc4]
006ab842 84c0                 test al, al
006ab844 7451                 je 0x6ab897
006ab846 89aefc010000         mov dword ptr [esi + 0x1fc], ebp
006ab84c 8b7660               mov esi, dword ptr [esi + 0x60]
006ab84f 8b16                 mov edx, dword ptr [esi]
006ab851 8d442454             lea eax, [esp + 0x54]
006ab855 50                   push eax
006ab856 8bce                 mov ecx, esi
006ab858 ff9218010000         call dword ptr [edx + 0x118]
006ab85e 896c243c             mov dword ptr [esp + 0x3c], ebp
006ab862 896c2440             mov dword ptr [esp + 0x40], ebp
006ab866 c74424440000803f     mov dword ptr [esp + 0x44], 0x3f800000
006ab86e 8b08                 mov ecx, dword ptr [eax]
006ab870 894c2448             mov dword ptr [esp + 0x48], ecx
006ab874 8b5004               mov edx, dword ptr [eax + 4]
006ab877 8954244c             mov dword ptr [esp + 0x4c], edx
006ab87b 8b4008               mov eax, dword ptr [eax + 8]
006ab87e 89442450             mov dword ptr [esp + 0x50], eax
006ab882 8b16                 mov edx, dword ptr [esi]
006ab884 8d44243c             lea eax, [esp + 0x3c]
006ab888 50                   push eax
006ab889 8bce                 mov ecx, esi
006ab88b ff922c010000         call dword ptr [edx + 0x12c]
006ab891 5e                   pop esi
006ab892 5d                   pop ebp
006ab893 83c47c               add esp, 0x7c
006ab896 c3                   ret 
006ab897 f7462400004000       test dword ptr [esi + 0x24], 0x400000
006ab89e 53                   push ebx
006ab89f 57                   push edi
006ab8a0 7433                 je 0x6ab8d5
006ab8a2 8d4c2418             lea ecx, [esp + 0x18]
006ab8a6 8d7e44               lea edi, [esi + 0x44]
006ab8a9 51                   push ecx
006ab8aa 8bcf                 mov ecx, edi
006ab8ac c744241c36000000     mov dword ptr [esp + 0x1c], 0x36
006ab8b4 e8f7acd8ff           call 0x4365b0
006ab8b9 8b7f04               mov edi, dword ptr [edi + 4]
006ab8bc 3bc7                 cmp eax, edi
006ab8be 7405                 je 0x6ab8c5
006ab8c0 833836               cmp dword ptr [eax], 0x36
006ab8c3 7e02                 jle 0x6ab8c7
006ab8c5 8bc7                 mov eax, edi
006ab8c7 8b4004               mov eax, dword ptr [eax + 4]
006ab8ca 8a480f               mov cl, byte ptr [eax + 0xf]
006ab8cd 84c9                 test cl, cl
006ab8cf 7404                 je 0x6ab8d5
006ab8d1 b301                 mov bl, 1
006ab8d3 eb02                 jmp 0x6ab8d7
006ab8d5 32db                 xor bl, bl
006ab8d7 8bce                 mov ecx, esi
006ab8d9 885c2413             mov byte ptr [esp + 0x13], bl
006ab8dd e84ec1e1ff           call 0x4c7a30
006ab8e2 8a881a020000         mov cl, byte ptr [eax + 0x21a]
006ab8e8 84c9                 test cl, cl
006ab8ea 8bce                 mov ecx, esi
006ab8ec 0f84b2000000         je 0x6ab9a4
006ab8f2 e8a9c1e1ff           call 0x4c7aa0
006ab8f7 8b4834               mov ecx, dword ptr [eax + 0x34]
006ab8fa 8b11                 mov edx, dword ptr [ecx]
006ab8fc 8d442418             lea eax, [esp + 0x18]
006ab900 50                   push eax
006ab901 ff5228               call dword ptr [edx + 0x28]
006ab904 84c0                 test al, al
006ab906 0f848a000000         je 0x6ab996
006ab90c 8b442418             mov eax, dword ptr [esp + 0x18]
006ab910 8b481c               mov ecx, dword ptr [eax + 0x1c]
006ab913 894c2420             mov dword ptr [esp + 0x20], ecx
006ab917 d9442420             fld dword ptr [esp + 0x20]
006ab91b d84c2420             fmul dword ptr [esp + 0x20]
006ab91f 8b5020               mov edx, dword ptr [eax + 0x20]
006ab922 89542424             mov dword ptr [esp + 0x24], edx
006ab926 d9442424             fld dword ptr [esp + 0x24]
006ab92a d84c2424             fmul dword ptr [esp + 0x24]
006ab92e c744242800000000     mov dword ptr [esp + 0x28], 0
006ab936 dec1                 faddp st(1)
006ab938 d9053cba2901         fld dword ptr [0x129ba3c]
006ab93e d80d3cba2901         fmul dword ptr [0x129ba3c]
006ab944 ded9                 fcompp 
006ab946 dfe0                 fnstsw ax
006ab948 f6c405               test ah, 5
006ab94b 7a49                 jp 0x6ab996
006ab94d 8d4c2420             lea ecx, [esp + 0x20]
006ab951 e8ea8a3600           call 0xa14440
006ab956 8b442420             mov eax, dword ptr [esp + 0x20]
006ab95a 8b542424             mov edx, dword ptr [esp + 0x24]
006ab95e 8b4e60               mov ecx, dword ptr [esi + 0x60]
006ab961 89442468             mov dword ptr [esp + 0x68], eax
006ab965 8b442428             mov eax, dword ptr [esp + 0x28]
006ab969 89442470             mov dword ptr [esp + 0x70], eax
006ab96d 8d44245c             lea eax, [esp + 0x5c]
006ab971 c744245c00000000     mov dword ptr [esp + 0x5c], 0
006ab979 c744246000000000     mov dword ptr [esp + 0x60], 0
006ab981 c74424640000803f     mov dword ptr [esp + 0x64], 0x3f800000
006ab989 8954246c             mov dword ptr [esp + 0x6c], edx
006ab98d 8b11                 mov edx, dword ptr [ecx]
006ab98f 50                   push eax
006ab990 ff922c010000         call dword ptr [edx + 0x12c]
006ab996 5f                   pop edi
006ab997 5b                   pop ebx
006ab998 89aefc010000         mov dword ptr [esi + 0x1fc], ebp
006ab99e 5e                   pop esi
006ab99f 5d                   pop ebp
006ab9a0 83c47c               add esp, 0x7c
006ab9a3 c3                   ret 
006ab9a4 e887c0e1ff           call 0x4c7a30
006ab9a9 8bc8                 mov ecx, eax
006ab9ab e820bbddff           call 0x4874d0
006ab9b0 84c0                 test al, al
006ab9b2 0f84e8000000         je 0x6abaa0
006ab9b8 8b4620               mov eax, dword ptr [esi + 0x20]
006ab9bb f6c401               test ah, 1
006ab9be 742a                 je 0x6ab9ea
006ab9c0 8d4c2418             lea ecx, [esp + 0x18]
006ab9c4 8d7e44               lea edi, [esi + 0x44]
006ab9c7 51                   push ecx
006ab9c8 8bcf                 mov ecx, edi
006ab9ca c744241c08000000     mov dword ptr [esp + 0x1c], 8
006ab9d2 e8d9abd8ff           call 0x4365b0
006ab9d7 8b7f04               mov edi, dword ptr [edi + 4]
006ab9da 3bc7                 cmp eax, edi
006ab9dc 7405                 je 0x6ab9e3
006ab9de 833808               cmp dword ptr [eax], 8
006ab9e1 7e02                 jle 0x6ab9e5
006ab9e3 8bc7                 mov eax, edi
006ab9e5 8b4804               mov ecx, dword ptr [eax + 4]
006ab9e8 eb04                 jmp 0x6ab9ee
006ab9ea 8b4c241c             mov ecx, dword ptr [esp + 0x1c]
006ab9ee 8b11                 mov edx, dword ptr [ecx]
006ab9f0 680000803f           push 0x3f800000
006ab9f5 8d442448             lea eax, [esp + 0x48]
006ab9f9 50                   push eax
006ab9fa ff9294000000         call dword ptr [edx + 0x94]
006aba00 8bce                 mov ecx, esi
006aba02 e8c9b9e1ff           call 0x4c73d0
006aba07 d9442448             fld dword ptr [esp + 0x48]
006aba0b d86004               fsub dword ptr [eax + 4]
006aba0e d9442444             fld dword ptr [esp + 0x44]
006aba12 d820                 fsub dword ptr [eax]
006aba14 c744243400000000     mov dword ptr [esp + 0x34], 0
006aba1c d95c242c             fstp dword ptr [esp + 0x2c]
006aba20 d9542430             fst dword ptr [esp + 0x30]
006aba24 d944242c             fld dword ptr [esp + 0x2c]
006aba28 d84c242c             fmul dword ptr [esp + 0x2c]
006aba2c d9c1                 fld st(1)
006aba2e d8ca                 fmul st(2)
006aba30 dec1                 faddp st(1)
006aba32 d9053cba2901         fld dword ptr [0x129ba3c]
006aba38 d80d3cba2901         fmul dword ptr [0x129ba3c]
006aba3e ded9                 fcompp 
006aba40 dfe0                 fnstsw ax
006aba42 ddd8                 fstp st(0)
006aba44 f6c405               test ah, 5
006aba47 7a49                 jp 0x6aba92
006aba49 8d4c242c             lea ecx, [esp + 0x2c]
006aba4d e8ee893600           call 0xa14440
006aba52 8b54242c             mov edx, dword ptr [esp + 0x2c]
006aba56 8b442430             mov eax, dword ptr [esp + 0x30]
006aba5a 8b4e60               mov ecx, dword ptr [esi + 0x60]
006aba5d 89542468             mov dword ptr [esp + 0x68], edx
006aba61 8b542434             mov edx, dword ptr [esp + 0x34]
006aba65 89542470             mov dword ptr [esp + 0x70], edx
006aba69 8d54245c             lea edx, [esp + 0x5c]
006aba6d c744245c00000000     mov dword ptr [esp + 0x5c], 0
006aba75 c744246000000000     mov dword ptr [esp + 0x60], 0
006aba7d c74424640000803f     mov dword ptr [esp + 0x64], 0x3f800000
006aba85 8944246c             mov dword ptr [esp + 0x6c], eax
006aba89 8b01                 mov eax, dword ptr [ecx]
006aba8b 52                   push edx
006aba8c ff902c010000         call dword ptr [eax + 0x12c]
006aba92 5f                   pop edi
006aba93 5b                   pop ebx
006aba94 89aefc010000         mov dword ptr [esi + 0x1fc], ebp
006aba9a 5e                   pop esi
006aba9b 5d                   pop ebp
006aba9c 83c47c               add esp, 0x7c
006aba9f c3                   ret 
006abaa0 8bce                 mov ecx, esi
006abaa2 e889bfe1ff           call 0x4c7a30
006abaa7 8a880a020000         mov cl, byte ptr [eax + 0x20a]
006abaad 84c9                 test cl, cl
006abaaf 0f8459010000         je 0x6abc0e
006abab5 8b4620               mov eax, dword ptr [esi + 0x20]
006abab8 f6c401               test ah, 1
006ababb 742a                 je 0x6abae7
006ababd 8d442418             lea eax, [esp + 0x18]
006abac1 8d7e44               lea edi, [esi + 0x44]
006abac4 50                   push eax
006abac5 8bcf                 mov ecx, edi
006abac7 c744241c08000000     mov dword ptr [esp + 0x1c], 8
006abacf e8dcaad8ff           call 0x4365b0
006abad4 8b7f04               mov edi, dword ptr [edi + 4]
006abad7 3bc7                 cmp eax, edi
006abad9 7405                 je 0x6abae0
006abadb 833808               cmp dword ptr [eax], 8
006abade 7e02                 jle 0x6abae2
006abae0 8bc7                 mov eax, edi
006abae2 8b4804               mov ecx, dword ptr [eax + 4]
006abae5 eb04                 jmp 0x6abaeb
006abae7 8b4c241c             mov ecx, dword ptr [esp + 0x1c]
006abaeb 8b11                 mov edx, dword ptr [ecx]
006abaed ff9290000000         call dword ptr [edx + 0x90]
006abaf3 3bc5                 cmp eax, ebp
006abaf5 0f84c1000000         je 0x6abbbc
006abafb 8b4864               mov ecx, dword ptr [eax + 0x64]
006abafe 3bcd                 cmp ecx, ebp
006abb00 7411                 je 0x6abb13
006abb02 680000803f           push 0x3f800000
006abb07 8d442460             lea eax, [esp + 0x60]
006abb0b 50                   push eax
006abb0c e84f55e1ff           call 0x4c1060
006abb11 eb07                 jmp 0x6abb1a
006abb13 8bc8                 mov ecx, eax
006abb15 e8b6b8e1ff           call 0x4c73d0
006abb1a 8b08                 mov ecx, dword ptr [eax]
006abb1c 8b5004               mov edx, dword ptr [eax + 4]
006abb1f 8b4008               mov eax, dword ptr [eax + 8]
006abb22 894c2444             mov dword ptr [esp + 0x44], ecx
006abb26 8bce                 mov ecx, esi
006abb28 89542448             mov dword ptr [esp + 0x48], edx
006abb2c 8944244c             mov dword ptr [esp + 0x4c], eax
006abb30 e8cb91ffff           call 0x6a4d00
006abb35 8d542444             lea edx, [esp + 0x44]
006abb39 8bc8                 mov ecx, eax
006abb3b e8005afbff           call 0x661540
006abb40 dd0570ed2201         fld qword ptr [0x122ed70]
006abb46 d9ff                 fcos 
006abb48 8b4e60               mov ecx, dword ptr [esi + 0x60]
006abb4b c744245c00000000     mov dword ptr [esp + 0x5c], 0
006abb53 c744246000000000     mov dword ptr [esp + 0x60], 0
006abb5b c74424640000803f     mov dword ptr [esp + 0x64], 0x3f800000
006abb63 d95c2418             fstp dword ptr [esp + 0x18]
006abb67 dc0db8f22401         fmul qword ptr [0x124f2b8]
006abb6d d9c0                 fld st(0)
006abb6f d9fe                 fsin 
006abb71 d84c2418             fmul dword ptr [esp + 0x18]
006abb75 d9c9                 fxch st(1)
006abb77 d9ff                 fcos 
006abb79 d84c2418             fmul dword ptr [esp + 0x18]
006abb7d d95c2448             fstp dword ptr [esp + 0x48]
006abb81 8b542448             mov edx, dword ptr [esp + 0x48]
006abb85 dd0570ed2201         fld qword ptr [0x122ed70]
006abb8b 8954246c             mov dword ptr [esp + 0x6c], edx
006abb8f d9fe                 fsin 
006abb91 d95c244c             fstp dword ptr [esp + 0x4c]
006abb95 8b44244c             mov eax, dword ptr [esp + 0x4c]
006abb99 89442470             mov dword ptr [esp + 0x70], eax
006abb9d 8d44245c             lea eax, [esp + 0x5c]
006abba1 d95c2468             fstp dword ptr [esp + 0x68]
006abba5 8b11                 mov edx, dword ptr [ecx]
006abba7 50                   push eax
006abba8 ff922c010000         call dword ptr [edx + 0x12c]
006abbae 5f                   pop edi
006abbaf 5b                   pop ebx
006abbb0 89aefc010000         mov dword ptr [esi + 0x1fc], ebp
006abbb6 5e                   pop esi
006abbb7 5d                   pop ebp
006abbb8 83c47c               add esp, 0x7c
006abbbb c3                   ret 
006abbbc 8b7e60               mov edi, dword ptr [esi + 0x60]
006abbbf 8b1f                 mov ebx, dword ptr [edi]
006abbc1 8d44245c             lea eax, [esp + 0x5c]
006abbc5 8bd3                 mov edx, ebx
006abbc7 50                   push eax
006abbc8 8bcf                 mov ecx, edi
006abbca c744244800000000     mov dword ptr [esp + 0x48], 0
006abbd2 c744244c00000000     mov dword ptr [esp + 0x4c], 0
006abbda c74424500000803f     mov dword ptr [esp + 0x50], 0x3f800000
006abbe2 ff9218010000         call dword ptr [edx + 0x118]
006abbe8 50                   push eax
006abbe9 8d4c2448             lea ecx, [esp + 0x48]
006abbed 51                   push ecx
006abbee 8d4c2434             lea ecx, [esp + 0x34]
006abbf2 e859b4ddff           call 0x487050
006abbf7 50                   push eax
006abbf8 8bcf                 mov ecx, edi
006abbfa ff932c010000         call dword ptr [ebx + 0x12c]
006abc00 5f                   pop edi
006abc01 5b                   pop ebx
006abc02 89aefc010000         mov dword ptr [esi + 0x1fc], ebp
006abc08 5e                   pop esi
006abc09 5d                   pop ebp
006abc0a 83c47c               add esp, 0x7c
006abc0d c3                   ret 
006abc0e d98688010000         fld dword ptr [esi + 0x188]
006abc14 d98684010000         fld dword ptr [esi + 0x184]
006abc1a d98680010000         fld dword ptr [esi + 0x180]
006abc20 d9c0                 fld st(0)
006abc22 d8c9                 fmul st(1)
006abc24 d9c2                 fld st(2)
006abc26 d8cb                 fmul st(3)
006abc28 dec1                 faddp st(1)
006abc2a d9c3                 fld st(3)
006abc2c d8cc                 fmul st(4)
006abc2e dec1                 faddp st(1)
006abc30 d9053cba2901         fld dword ptr [0x129ba3c]
006abc36 d80d3cba2901         fmul dword ptr [0x129ba3c]
006abc3c ded9                 fcompp 
006abc3e ddd8                 fstp st(0)
006abc40 dfe0                 fnstsw ax
006abc42 ddd8                 fstp st(0)
006abc44 f6c405               test ah, 5
006abc47 ddd8                 fstp st(0)
006abc49 0f8a91030000         jp 0x6abfe0
006abc4f 8b4e60               mov ecx, dword ptr [esi + 0x60]
006abc52 8b11                 mov edx, dword ptr [ecx]
006abc54 8d44242c             lea eax, [esp + 0x2c]
006abc58 50                   push eax
006abc59 ff9218010000         call dword ptr [edx + 0x118]
006abc5f 8b9684010000         mov edx, dword ptr [esi + 0x184]
006abc65 8b8e80010000         mov ecx, dword ptr [esi + 0x180]
006abc6b 89542424             mov dword ptr [esp + 0x24], edx
006abc6f d9442424             fld dword ptr [esp + 0x24]
006abc73 d84c2424             fmul dword ptr [esp + 0x24]
006abc77 894c2420             mov dword ptr [esp + 0x20], ecx
006abc7b d9442420             fld dword ptr [esp + 0x20]
006abc7f c744242800000000     mov dword ptr [esp + 0x28], 0
006abc87 d84c2420             fmul dword ptr [esp + 0x20]
006abc8b dec1                 faddp st(1)
006abc8d d9053cba2901         fld dword ptr [0x129ba3c]
006abc93 d80d3cba2901         fmul dword ptr [0x129ba3c]
006abc99 ded9                 fcompp 
006abc9b dfe0                 fnstsw ax
006abc9d f6c405               test ah, 5
006abca0 7a09                 jp 0x6abcab
006abca2 8d4c2420             lea ecx, [esp + 0x20]
006abca6 e895873600           call 0xa14440
006abcab d9442434             fld dword ptr [esp + 0x34]
006abcaf d84c2428             fmul dword ptr [esp + 0x28]
006abcb3 d9442430             fld dword ptr [esp + 0x30]
006abcb7 d84c2424             fmul dword ptr [esp + 0x24]
006abcbb dec1                 faddp st(1)
006abcbd d944242c             fld dword ptr [esp + 0x2c]
006abcc1 d84c2420             fmul dword ptr [esp + 0x20]
006abcc5 dec1                 faddp st(1)
006abcc7 d9542414             fst dword ptr [esp + 0x14]
006abccb d81de0de2201         fcomp dword ptr [0x122dee0]
006abcd1 dfe0                 fnstsw ax
006abcd3 f6c405               test ah, 5
006abcd6 7a0a                 jp 0x6abce2
006abcd8 c7442414000080bf     mov dword ptr [esp + 0x14], 0xbf800000
006abce0 eb19                 jmp 0x6abcfb
006abce2 d9442414             fld dword ptr [esp + 0x14]
006abce6 d81dd8de2201         fcomp dword ptr [0x122ded8]
006abcec dfe0                 fnstsw ax
006abcee f6c441               test ah, 0x41
006abcf1 7508                 jne 0x6abcfb
006abcf3 c74424140000803f     mov dword ptr [esp + 0x14], 0x3f800000
006abcfb d9442414             fld dword ptr [esp + 0x14]
006abcff e89e2e5500           call 0xbfeba2
006abd04 d80d087f2301         fmul dword ptr [0x1237f08]
006abd0a d9442424             fld dword ptr [esp + 0x24]
006abd0e d84c242c             fmul dword ptr [esp + 0x2c]
006abd12 d9442430             fld dword ptr [esp + 0x30]
006abd16 d84c2420             fmul dword ptr [esp + 0x20]
006abd1a dee9                 fsubp st(1)
006abd1c d81ddcde2201         fcomp dword ptr [0x122dedc]
006abd22 dfe0                 fnstsw ax
006abd24 f6c441               test ah, 0x41
006abd27 7508                 jne 0x6abd31
006abd29 d905e0de2201         fld dword ptr [0x122dee0]
006abd2f eb06                 jmp 0x6abd37
006abd31 d905d8de2201         fld dword ptr [0x122ded8]
006abd37 d8c9                 fmul st(1)
006abd39 8bce                 mov ecx, esi
006abd3b d95c2414             fstp dword ptr [esp + 0x14]
006abd3f ddd8                 fstp st(0)
006abd41 d9442414             fld dword ptr [esp + 0x14]
006abd45 d80de0de2201         fmul dword ptr [0x122dee0]
006abd4b d95c241c             fstp dword ptr [esp + 0x1c]
006abd4f e87cdcffff           call 0x6a99d0
006abd54 d80de8d12501         fmul dword ptr [0x125d1e8]
006abd5a 8bce                 mov ecx, esi
006abd5c d85c2414             fcomp dword ptr [esp + 0x14]
006abd60 dfe0                 fnstsw ax
006abd62 f6c441               test ah, 0x41
006abd65 750d                 jne 0x6abd74
006abd67 e864dcffff           call 0x6a99d0
006abd6c d80de8d12501         fmul dword ptr [0x125d1e8]
006abd72 eb23                 jmp 0x6abd97
006abd74 e857dcffff           call 0x6a99d0
006abd79 d80d008e2301         fmul dword ptr [0x1238e00]
006abd7f d85c2414             fcomp dword ptr [esp + 0x14]
006abd83 dfe0                 fnstsw ax
006abd85 f6c405               test ah, 5
006abd88 7a11                 jp 0x6abd9b
006abd8a 8bce                 mov ecx, esi
006abd8c e83fdcffff           call 0x6a99d0
006abd91 d80d008e2301         fmul dword ptr [0x1238e00]
006abd97 d95c2414             fstp dword ptr [esp + 0x14]
006abd9b 8b442414             mov eax, dword ptr [esp + 0x14]
006abd9f d9442414             fld dword ptr [esp + 0x14]
006abda3 d80de0de2201         fmul dword ptr [0x122dee0]
006abda9 50                   push eax
006abdaa 8d4c2448             lea ecx, [esp + 0x48]
006abdae 51                   push ecx
006abdaf 8d4c2434             lea ecx, [esp + 0x34]
006abdb3 d95c2420             fstp dword ptr [esp + 0x20]
006abdb7 c744244c00000000     mov dword ptr [esp + 0x4c], 0
006abdbf c744245000000000     mov dword ptr [esp + 0x50], 0
006abdc7 c74424540000803f     mov dword ptr [esp + 0x54], 0x3f800000
006abdcf e88c7e3600           call 0xa13c60
006abdd4 8b54242c             mov edx, dword ptr [esp + 0x2c]
006abdd8 8b442430             mov eax, dword ptr [esp + 0x30]
006abddc 8b4e60               mov ecx, dword ptr [esi + 0x60]
006abddf 89542468             mov dword ptr [esp + 0x68], edx
006abde3 8b542434             mov edx, dword ptr [esp + 0x34]
006abde7 89542470             mov dword ptr [esp + 0x70], edx
006abdeb 8d54245c             lea edx, [esp + 0x5c]
006abdef c744245c00000000     mov dword ptr [esp + 0x5c], 0
006abdf7 c744246000000000     mov dword ptr [esp + 0x60], 0
006abdff c74424640000803f     mov dword ptr [esp + 0x64], 0x3f800000
006abe07 8944246c             mov dword ptr [esp + 0x6c], eax
006abe0b 8b01                 mov eax, dword ptr [ecx]
006abe0d 52                   push edx
006abe0e ff902c010000         call dword ptr [eax + 0x12c]
006abe14 84db                 test bl, bl
006abe16 c744241400000000     mov dword ptr [esp + 0x14], 0
006abe1e 757d                 jne 0x6abe9d
006abe20 8b8600020000         mov eax, dword ptr [esi + 0x200]
006abe26 83f804               cmp eax, 4
006abe29 7405                 je 0x6abe30
006abe2b 83f803               cmp eax, 3
006abe2e 756d                 jne 0x6abe9d
006abe30 d9442418             fld dword ptr [esp + 0x18]
006abe34 d944241c             fld dword ptr [esp + 0x1c]
006abe38 dae9                 fucompp 
006abe3a dfe0                 fnstsw ax
006abe3c f6c444               test ah, 0x44
006abe3f 7a5c                 jp 0x6abe9d
006abe41 d9442418             fld dword ptr [esp + 0x18]
006abe45 d9e1                 fabs 
006abe47 d81de4d12501         fcomp dword ptr [0x125d1e4]
006abe4d dfe0                 fnstsw ax
006abe4f f6c441               test ah, 0x41
006abe52 7549                 jne 0x6abe9d
006abe54 d98688010000         fld dword ptr [esi + 0x188]
006abe5a 8b86ec010000         mov eax, dword ptr [esi + 0x1ec]
006abe60 d98684010000         fld dword ptr [esi + 0x184]
006abe66 d98680010000         fld dword ptr [esi + 0x180]
006abe6c d9c0                 fld st(0)
006abe6e d8c9                 fmul st(1)
006abe70 d9c2                 fld st(2)
006abe72 d8cb                 fmul st(3)
006abe74 dec1                 faddp st(1)
006abe76 d9c3                 fld st(3)
006abe78 d8cc                 fmul st(4)
006abe7a dec1                 faddp st(1)
006abe7c d9fa                 fsqrt 
006abe7e dddb                 fstp st(3)
006abe80 ddd8                 fstp st(0)
006abe82 ddd8                 fstp st(0)
006abe84 d84c2418             fmul dword ptr [esp + 0x18]
006abe88 d94070               fld dword ptr [eax + 0x70]
006abe8b d80d008e2301         fmul dword ptr [0x1238e00]
006abe91 def9                 fdivp st(1)
006abe93 d80de0d12501         fmul dword ptr [0x125d1e0]
006abe99 d95c2414             fstp dword ptr [esp + 0x14]
006abe9d 8b8eec010000         mov ecx, dword ptr [esi + 0x1ec]
006abea3 d94178               fld dword ptr [ecx + 0x78]
006abea6 8b7e60               mov edi, dword ptr [esi + 0x60]
006abea9 d80d008e2301         fmul dword ptr [0x1238e00]
006abeaf 8daefc010000         lea ebp, [esi + 0x1fc]
006abeb5 d95c241c             fstp dword ptr [esp + 0x1c]
006abeb9 d9470c               fld dword ptr [edi + 0xc]
006abebc d86718               fsub dword ptr [edi + 0x18]
006abebf d94710               fld dword ptr [edi + 0x10]
006abec2 d8671c               fsub dword ptr [edi + 0x1c]
006abec5 d94714               fld dword ptr [edi + 0x14]
006abec8 d86720               fsub dword ptr [edi + 0x20]
006abecb d9c0                 fld st(0)
006abecd d8c9                 fmul st(1)
006abecf d9c2                 fld st(2)
006abed1 d8cb                 fmul st(3)
006abed3 dec1                 faddp st(1)
006abed5 d9c3                 fld st(3)
006abed7 d8cc                 fmul st(4)
006abed9 dec1                 faddp st(1)
006abedb d81d5ca12501         fcomp dword ptr [0x125a15c]
006abee1 ddd8                 fstp st(0)
006abee3 dfe0                 fnstsw ax
006abee5 ddd8                 fstp st(0)
006abee7 f6c405               test ah, 5
006abeea ddd8                 fstp st(0)
006abeec 7a4e                 jp 0x6abf3c
006abeee c7450000000000       mov dword ptr [ebp], 0
006abef5 8b1f                 mov ebx, dword ptr [edi]
006abef7 8d44245c             lea eax, [esp + 0x5c]
006abefb 8bd3                 mov edx, ebx
006abefd 50                   push eax
006abefe 8bcf                 mov ecx, edi
006abf00 c744244800000000     mov dword ptr [esp + 0x48], 0
006abf08 c744244c00000000     mov dword ptr [esp + 0x4c], 0
006abf10 c74424500000803f     mov dword ptr [esp + 0x50], 0x3f800000
006abf18 ff9218010000         call dword ptr [edx + 0x118]
006abf1e 50                   push eax
006abf1f 8d4c2448             lea ecx, [esp + 0x48]
006abf23 51                   push ecx
006abf24 8d4c247c             lea ecx, [esp + 0x7c]
006abf28 e823b1ddff           call 0x487050
006abf2d 50                   push eax
006abf2e 8bcf                 mov ecx, edi
006abf30 ff932c010000         call dword ptr [ebx + 0x12c]
006abf36 8a5c2413             mov bl, byte ptr [esp + 0x13]
006abf3a eb0e                 jmp 0x6abf4a
006abf3c 55                   push ebp
006abf3d 8d542420             lea edx, [esp + 0x20]
006abf41 8d4c2418             lea ecx, [esp + 0x18]
006abf45 e8d61c0000           call 0x6adc20
006abf4a 8b96ec010000         mov edx, dword ptr [esi + 0x1ec]
006abf50 d94274               fld dword ptr [edx + 0x74]
006abf53 d80d008e2301         fmul dword ptr [0x1238e00]
006abf59 d9542418             fst dword ptr [esp + 0x18]
006abf5d d9e0                 fchs 
006abf5f d85500               fcom dword ptr [ebp]
006abf62 dfe0                 fnstsw ax
006abf64 f6c441               test ah, 0x41
006abf67 7505                 jne 0x6abf6e
006abf69 d95d00               fstp dword ptr [ebp]
006abf6c eb17                 jmp 0x6abf85
006abf6e ddd8                 fstp st(0)
006abf70 d94500               fld dword ptr [ebp]
006abf73 d85c2418             fcomp dword ptr [esp + 0x18]
006abf77 dfe0                 fnstsw ax
006abf79 f6c441               test ah, 0x41
006abf7c 7507                 jne 0x6abf85
006abf7e 8b442418             mov eax, dword ptr [esp + 0x18]
006abf82 894500               mov dword ptr [ebp], eax
006abf85 d94500               fld dword ptr [ebp]
006abf88 d9e1                 fabs 
006abf8a d81d3cba2901         fcomp dword ptr [0x129ba3c]
006abf90 dfe0                 fnstsw ax
006abf92 f6c441               test ah, 0x41
006abf95 0f858f000000         jne 0x6ac02a
006abf9b 8b4e60               mov ecx, dword ptr [esi + 0x60]
006abf9e 8d44245c             lea eax, [esp + 0x5c]
006abfa2 c744244400000000     mov dword ptr [esp + 0x44], 0
006abfaa c744244800000000     mov dword ptr [esp + 0x48], 0
006abfb2 c744244c0000803f     mov dword ptr [esp + 0x4c], 0x3f800000
006abfba 8b11                 mov edx, dword ptr [ecx]
006abfbc 50                   push eax
006abfbd ff9218010000         call dword ptr [edx + 0x118]
006abfc3 8b4d00               mov ecx, dword ptr [ebp]
006abfc6 51                   push ecx
006abfc7 8d542460             lea edx, [esp + 0x60]
006abfcb 52                   push edx
006abfcc 8d4c244c             lea ecx, [esp + 0x4c]
006abfd0 e88b7c3600           call 0xa13c60
006abfd5 8b7e60               mov edi, dword ptr [esi + 0x60]
006abfd8 8b2f                 mov ebp, dword ptr [edi]
006abfda 8d44245c             lea eax, [esp + 0x5c]
006abfde eb32                 jmp 0x6ac012
006abfe0 8b7e60               mov edi, dword ptr [esi + 0x60]
006abfe3 89aefc010000         mov dword ptr [esi + 0x1fc], ebp
006abfe9 8b2f                 mov ebp, dword ptr [edi]
006abfeb 8d44245c             lea eax, [esp + 0x5c]
006abfef 8bd5                 mov edx, ebp
006abff1 50                   push eax
006abff2 8bcf                 mov ecx, edi
006abff4 c744244800000000     mov dword ptr [esp + 0x48], 0
006abffc c744244c00000000     mov dword ptr [esp + 0x4c], 0
006ac004 c74424500000803f     mov dword ptr [esp + 0x50], 0x3f800000
006ac00c ff9218010000         call dword ptr [edx + 0x118]
006ac012 8d4c2444             lea ecx, [esp + 0x44]
006ac016 50                   push eax
006ac017 51                   push ecx
006ac018 8d4c247c             lea ecx, [esp + 0x7c]
006ac01c e82fb0ddff           call 0x487050
006ac021 8bcf                 mov ecx, edi
006ac023 50                   push eax
006ac024 ff952c010000         call dword ptr [ebp + 0x12c]
006ac02a 8b8600020000         mov eax, dword ptr [esi + 0x200]
006ac030 83f801               cmp eax, 1
006ac033 0f84e3000000         je 0x6ac11c
006ac039 84db                 test bl, bl
006ac03b 7509                 jne 0x6ac046
006ac03d 83f802               cmp eax, 2
006ac040 0f85ea000000         jne 0x6ac130
006ac046 8b4e60               mov ecx, dword ptr [esi + 0x60]
006ac049 8b11                 mov edx, dword ptr [ecx]
006ac04b ff92fc000000         call dword ptr [edx + 0xfc]
006ac051 d95c2418             fstp dword ptr [esp + 0x18]
006ac055 d98680010000         fld dword ptr [esi + 0x180]
006ac05b d98684010000         fld dword ptr [esi + 0x184]
006ac061 d9f3                 fpatan 
006ac063 d80d087f2301         fmul dword ptr [0x1237f08]
006ac069 d815dcde2201         fcom dword ptr [0x122dedc]
006ac06f d954241c             fst dword ptr [esp + 0x1c]
006ac073 dfe0                 fnstsw ax
006ac075 f6c405               test ah, 5
006ac078 7b0d                 jnp 0x6ac087
006ac07a d815d8de2201         fcom dword ptr [0x122ded8]
006ac080 dfe0                 fnstsw ax
006ac082 f6c401               test ah, 1
006ac085 7524                 jne 0x6ac0ab
006ac087 ddd8                 fstp st(0)
006ac089 d944241c             fld dword ptr [esp + 0x1c]
006ac08d dd05007f2301         fld qword ptr [0x1237f00]
006ac093 e8902a5500           call 0xbfeb28
006ac098 d815dcde2201         fcom dword ptr [0x122dedc]
006ac09e dfe0                 fnstsw ax
006ac0a0 f6c405               test ah, 5
006ac0a3 7a06                 jp 0x6ac0ab
006ac0a5 d805d8de2201         fadd dword ptr [0x122ded8]
006ac0ab d9442418             fld dword ptr [esp + 0x18]
006ac0af d8e1                 fsub st(1)
006ac0b1 d95c2414             fstp dword ptr [esp + 0x14]
006ac0b5 ddd8                 fstp st(0)
006ac0b7 d9442414             fld dword ptr [esp + 0x14]
006ac0bb d81d98872301         fcomp dword ptr [0x1238798]
006ac0c1 d9442414             fld dword ptr [esp + 0x14]
006ac0c5 dfe0                 fnstsw ax
006ac0c7 f6c405               test ah, 5
006ac0ca 7a19                 jp 0x6ac0e5
006ac0cc d8259cf52201         fsub dword ptr [0x122f59c]
006ac0d2 dd05007f2301         fld qword ptr [0x1237f00]
006ac0d8 e84b2a5500           call 0xbfeb28
006ac0dd d8059cf52201         fadd dword ptr [0x122f59c]
006ac0e3 eb28                 jmp 0x6ac10d
006ac0e5 d81d9cf52201         fcomp dword ptr [0x122f59c]
006ac0eb d9442414             fld dword ptr [esp + 0x14]
006ac0ef dfe0                 fnstsw ax
006ac0f1 f6c401               test ah, 1
006ac0f4 7517                 jne 0x6ac10d
006ac0f6 d82598872301         fsub dword ptr [0x1238798]
006ac0fc dd05007f2301         fld qword ptr [0x1237f00]
006ac102 e8212a5500           call 0xbfeb28
006ac107 d8259cf52201         fsub dword ptr [0x122f59c]
006ac10d d9e1                 fabs 
006ac10f d81d008e2301         fcomp dword ptr [0x1238e00]
006ac115 dfe0                 fnstsw ax
006ac117 f6c441               test ah, 0x41
006ac11a 7514                 jne 0x6ac130
006ac11c 33c0                 xor eax, eax
006ac11e 898688010000         mov dword ptr [esi + 0x188], eax
006ac124 898684010000         mov dword ptr [esi + 0x184], eax
006ac12a 898680010000         mov dword ptr [esi + 0x180], eax
006ac130 5f                   pop edi
006ac131 5b                   pop ebx
006ac132 5e                   pop esi
006ac133 5d                   pop ebp
006ac134 83c47c               add esp, 0x7c
006ac137 c3                   ret 
