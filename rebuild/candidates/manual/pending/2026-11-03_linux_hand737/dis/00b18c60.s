00b18c60 d9442408             fld dword ptr [esp + 8]
00b18c64 83ec34               sub esp, 0x34
00b18c67 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18c6d 53                   push ebx
00b18c6e 55                   push ebp
00b18c6f 56                   push esi
00b18c70 57                   push edi
00b18c71 8b7c2448             mov edi, dword ptr [esp + 0x48]
00b18c75 dfe0                 fnstsw ax
00b18c77 8bf1                 mov esi, ecx
00b18c79 f6c441               test ah, 0x41
00b18c7c 7505                 jne 0xb18c83
00b18c7e 8a4748               mov al, byte ptr [edi + 0x48]
00b18c81 eb03                 jmp 0xb18c86
00b18c83 8a4648               mov al, byte ptr [esi + 0x48]
00b18c86 8a5e48               mov bl, byte ptr [esi + 0x48]
00b18c89 d944244c             fld dword ptr [esp + 0x4c]
00b18c8d d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18c93 c0e802               shr al, 2
00b18c96 2401                 and al, 1
00b18c98 c0e002               shl al, 2
00b18c9b 32c3                 xor al, bl
00b18c9d 2404                 and al, 4
00b18c9f 8acb                 mov cl, bl
00b18ca1 32c8                 xor cl, al
00b18ca3 dfe0                 fnstsw ax
00b18ca5 884e48               mov byte ptr [esi + 0x48], cl
00b18ca8 f6c441               test ah, 0x41
00b18cab 8ad1                 mov dl, cl
00b18cad 7505                 jne 0xb18cb4
00b18caf 8b4718               mov eax, dword ptr [edi + 0x18]
00b18cb2 eb03                 jmp 0xb18cb7
00b18cb4 8b4618               mov eax, dword ptr [esi + 0x18]
00b18cb7 d944244c             fld dword ptr [esp + 0x4c]
00b18cbb 894618               mov dword ptr [esi + 0x18], eax
00b18cbe d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18cc4 dfe0                 fnstsw ax
00b18cc6 f6c441               test ah, 0x41
00b18cc9 7505                 jne 0xb18cd0
00b18ccb 8a4748               mov al, byte ptr [edi + 0x48]
00b18cce eb02                 jmp 0xb18cd2
00b18cd0 8ac2                 mov al, dl
00b18cd2 d905d8de2201         fld dword ptr [0x122ded8]
00b18cd8 d0e8                 shr al, 1
00b18cda d864244c             fsub dword ptr [esp + 0x4c]
00b18cde 2401                 and al, 1
00b18ce0 d0e0                 shl al, 1
00b18ce2 32c2                 xor al, dl
00b18ce4 d95c2410             fstp dword ptr [esp + 0x10]
00b18ce8 2402                 and al, 2
00b18cea d944244c             fld dword ptr [esp + 0x4c]
00b18cee 8ac8                 mov cl, al
00b18cf0 32ca                 xor cl, dl
00b18cf2 884e48               mov byte ptr [esi + 0x48], cl
00b18cf5 d84f1c               fmul dword ptr [edi + 0x1c]
00b18cf8 d9442410             fld dword ptr [esp + 0x10]
00b18cfc d84e1c               fmul dword ptr [esi + 0x1c]
00b18cff dec1                 faddp st(1)
00b18d01 d95e1c               fstp dword ptr [esi + 0x1c]
00b18d04 d944244c             fld dword ptr [esp + 0x4c]
00b18d08 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18d0e dfe0                 fnstsw ax
00b18d10 f6c441               test ah, 0x41
00b18d13 7505                 jne 0xb18d1a
00b18d15 8a4748               mov al, byte ptr [edi + 0x48]
00b18d18 eb02                 jmp 0xb18d1c
00b18d1a 8ac1                 mov al, cl
00b18d1c d944244c             fld dword ptr [esp + 0x4c]
00b18d20 c0e803               shr al, 3
00b18d23 2401                 and al, 1
00b18d25 c0e003               shl al, 3
00b18d28 32c1                 xor al, cl
00b18d2a 2408                 and al, 8
00b18d2c 32c1                 xor al, cl
00b18d2e 884648               mov byte ptr [esi + 0x48], al
00b18d31 d84f20               fmul dword ptr [edi + 0x20]
00b18d34 d9442410             fld dword ptr [esp + 0x10]
00b18d38 8b4e34               mov ecx, dword ptr [esi + 0x34]
00b18d3b d84e20               fmul dword ptr [esi + 0x20]
00b18d3e 894c2448             mov dword ptr [esp + 0x48], ecx
00b18d42 dec1                 faddp st(1)
00b18d44 d95e20               fstp dword ptr [esi + 0x20]
00b18d47 d944244c             fld dword ptr [esp + 0x4c]
00b18d4b d84f2c               fmul dword ptr [edi + 0x2c]
00b18d4e d9442410             fld dword ptr [esp + 0x10]
00b18d52 d84e2c               fmul dword ptr [esi + 0x2c]
00b18d55 dec1                 faddp st(1)
00b18d57 d95e2c               fstp dword ptr [esi + 0x2c]
00b18d5a d944244c             fld dword ptr [esp + 0x4c]
00b18d5e d84f30               fmul dword ptr [edi + 0x30]
00b18d61 d9442410             fld dword ptr [esp + 0x10]
00b18d65 d84e30               fmul dword ptr [esi + 0x30]
00b18d68 dec1                 faddp st(1)
00b18d6a d95e30               fstp dword ptr [esi + 0x30]
00b18d6d 8b4734               mov eax, dword ptr [edi + 0x34]
00b18d70 db442448             fild dword ptr [esp + 0x48]
00b18d74 89442418             mov dword ptr [esp + 0x18], eax
00b18d78 d84c2410             fmul dword ptr [esp + 0x10]
00b18d7c db442418             fild dword ptr [esp + 0x18]
00b18d80 d84c244c             fmul dword ptr [esp + 0x4c]
00b18d84 dec1                 faddp st(1)
00b18d86 d95c2448             fstp dword ptr [esp + 0x48]
00b18d8a d9442448             fld dword ptr [esp + 0x48]
00b18d8e db5c2418             fistp dword ptr [esp + 0x18]
00b18d92 8b4e38               mov ecx, dword ptr [esi + 0x38]
00b18d95 8b542418             mov edx, dword ptr [esp + 0x18]
00b18d99 894c2448             mov dword ptr [esp + 0x48], ecx
00b18d9d db442448             fild dword ptr [esp + 0x48]
00b18da1 895634               mov dword ptr [esi + 0x34], edx
00b18da4 8b4738               mov eax, dword ptr [edi + 0x38]
00b18da7 d84c2410             fmul dword ptr [esp + 0x10]
00b18dab 89442418             mov dword ptr [esp + 0x18], eax
00b18daf db442418             fild dword ptr [esp + 0x18]
00b18db3 d84c244c             fmul dword ptr [esp + 0x4c]
00b18db7 dec1                 faddp st(1)
00b18db9 d95c2448             fstp dword ptr [esp + 0x48]
00b18dbd d9442448             fld dword ptr [esp + 0x48]
00b18dc1 db5c2418             fistp dword ptr [esp + 0x18]
00b18dc5 8b4e3c               mov ecx, dword ptr [esi + 0x3c]
00b18dc8 8b542418             mov edx, dword ptr [esp + 0x18]
00b18dcc 894c2448             mov dword ptr [esp + 0x48], ecx
00b18dd0 db442448             fild dword ptr [esp + 0x48]
00b18dd4 895638               mov dword ptr [esi + 0x38], edx
00b18dd7 8b473c               mov eax, dword ptr [edi + 0x3c]
00b18dda d84c2410             fmul dword ptr [esp + 0x10]
00b18dde 89442418             mov dword ptr [esp + 0x18], eax
00b18de2 db442418             fild dword ptr [esp + 0x18]
00b18de6 d84c244c             fmul dword ptr [esp + 0x4c]
00b18dea dec1                 faddp st(1)
00b18dec d95c2448             fstp dword ptr [esp + 0x48]
00b18df0 d9442448             fld dword ptr [esp + 0x48]
00b18df4 db5c2418             fistp dword ptr [esp + 0x18]
00b18df8 d944244c             fld dword ptr [esp + 0x4c]
00b18dfc 8b542418             mov edx, dword ptr [esp + 0x18]
00b18e00 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18e06 89563c               mov dword ptr [esi + 0x3c], edx
00b18e09 dfe0                 fnstsw ax
00b18e0b f6c441               test ah, 0x41
00b18e0e 7505                 jne 0xb18e15
00b18e10 8a4748               mov al, byte ptr [edi + 0x48]
00b18e13 eb03                 jmp 0xb18e18
00b18e15 8a4648               mov al, byte ptr [esi + 0x48]
00b18e18 8a4e48               mov cl, byte ptr [esi + 0x48]
00b18e1b d944244c             fld dword ptr [esp + 0x4c]
00b18e1f 2401                 and al, 1
00b18e21 32c8                 xor cl, al
00b18e23 8a4648               mov al, byte ptr [esi + 0x48]
00b18e26 80e101               and cl, 1
00b18e29 32c1                 xor al, cl
00b18e2b 884648               mov byte ptr [esi + 0x48], al
00b18e2e d84f40               fmul dword ptr [edi + 0x40]
00b18e31 d9442410             fld dword ptr [esp + 0x10]
00b18e35 8ac8                 mov cl, al
00b18e37 d84e40               fmul dword ptr [esi + 0x40]
00b18e3a dec1                 faddp st(1)
00b18e3c d95e40               fstp dword ptr [esi + 0x40]
00b18e3f d944244c             fld dword ptr [esp + 0x4c]
00b18e43 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18e49 dfe0                 fnstsw ax
00b18e4b f6c441               test ah, 0x41
00b18e4e 7505                 jne 0xb18e55
00b18e50 8a4749               mov al, byte ptr [edi + 0x49]
00b18e53 eb03                 jmp 0xb18e58
00b18e55 8a4649               mov al, byte ptr [esi + 0x49]
00b18e58 8a5649               mov dl, byte ptr [esi + 0x49]
00b18e5b d944244c             fld dword ptr [esp + 0x4c]
00b18e5f d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18e65 2401                 and al, 1
00b18e67 32d0                 xor dl, al
00b18e69 8a4649               mov al, byte ptr [esi + 0x49]
00b18e6c 80e201               and dl, 1
00b18e6f 32c2                 xor al, dl
00b18e71 884649               mov byte ptr [esi + 0x49], al
00b18e74 dfe0                 fnstsw ax
00b18e76 f6c441               test ah, 0x41
00b18e79 7505                 jne 0xb18e80
00b18e7b 8a4748               mov al, byte ptr [edi + 0x48]
00b18e7e eb02                 jmp 0xb18e82
00b18e80 8ac1                 mov al, cl
00b18e82 d944244c             fld dword ptr [esp + 0x4c]
00b18e86 c0e804               shr al, 4
00b18e89 d80d40b22501         fmul dword ptr [0x125b240]
00b18e8f 2401                 and al, 1
00b18e91 c0e004               shl al, 4
00b18e94 32c1                 xor al, cl
00b18e96 2410                 and al, 0x10
00b18e98 d9542418             fst dword ptr [esp + 0x18]
00b18e9c 32c1                 xor al, cl
00b18e9e d95c2448             fstp dword ptr [esp + 0x48]
00b18ea2 884648               mov byte ptr [esi + 0x48], al
00b18ea5 d9442448             fld dword ptr [esp + 0x48]
00b18ea9 db5c241c             fistp dword ptr [esp + 0x1c]
00b18ead 8a5f45               mov bl, byte ptr [edi + 0x45]
00b18eb0 8b44241c             mov eax, dword ptr [esp + 0x1c]
00b18eb4 8a5744               mov dl, byte ptr [edi + 0x44]
00b18eb7 50                   push eax
00b18eb8 51                   push ecx
00b18eb9 8a4f47               mov cl, byte ptr [edi + 0x47]
00b18ebc 8bc4                 mov eax, esp
00b18ebe 885c2450             mov byte ptr [esp + 0x50], bl
00b18ec2 8a5f46               mov bl, byte ptr [edi + 0x46]
00b18ec5 885802               mov byte ptr [eax + 2], bl
00b18ec8 8a5c2450             mov bl, byte ptr [esp + 0x50]
00b18ecc 885801               mov byte ptr [eax + 1], bl
00b18ecf 884803               mov byte ptr [eax + 3], cl
00b18ed2 8810                 mov byte ptr [eax], dl
00b18ed4 8a5e45               mov bl, byte ptr [esi + 0x45]
00b18ed7 8a5644               mov dl, byte ptr [esi + 0x44]
00b18eda 51                   push ecx
00b18edb 8a4e47               mov cl, byte ptr [esi + 0x47]
00b18ede 8bc4                 mov eax, esp
00b18ee0 885c2454             mov byte ptr [esp + 0x54], bl
00b18ee4 8a5e46               mov bl, byte ptr [esi + 0x46]
00b18ee7 884803               mov byte ptr [eax + 3], cl
00b18eea 885802               mov byte ptr [eax + 2], bl
00b18eed 8a5c2454             mov bl, byte ptr [esp + 0x54]
00b18ef1 8d4c2420             lea ecx, [esp + 0x20]
00b18ef5 51                   push ecx
00b18ef6 885801               mov byte ptr [eax + 1], bl
00b18ef9 8810                 mov byte ptr [eax], dl
00b18efb e850b3f2ff           call 0xa44250
00b18f00 d944245c             fld dword ptr [esp + 0x5c]
00b18f04 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18f0a 8b542424             mov edx, dword ptr [esp + 0x24]
00b18f0e 83c410               add esp, 0x10
00b18f11 895644               mov dword ptr [esi + 0x44], edx
00b18f14 dfe0                 fnstsw ax
00b18f16 f6c441               test ah, 0x41
00b18f19 7505                 jne 0xb18f20
00b18f1b 8a4748               mov al, byte ptr [edi + 0x48]
00b18f1e eb03                 jmp 0xb18f23
00b18f20 8a4648               mov al, byte ptr [esi + 0x48]
00b18f23 8a5e48               mov bl, byte ptr [esi + 0x48]
00b18f26 d944244c             fld dword ptr [esp + 0x4c]
00b18f2a d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18f30 c0e805               shr al, 5
00b18f33 2401                 and al, 1
00b18f35 c0e005               shl al, 5
00b18f38 32c3                 xor al, bl
00b18f3a 2420                 and al, 0x20
00b18f3c 8acb                 mov cl, bl
00b18f3e 32c8                 xor cl, al
00b18f40 dfe0                 fnstsw ax
00b18f42 884e48               mov byte ptr [esi + 0x48], cl
00b18f45 f6c441               test ah, 0x41
00b18f48 8ad1                 mov dl, cl
00b18f4a 7505                 jne 0xb18f51
00b18f4c 8a4f48               mov cl, byte ptr [edi + 0x48]
00b18f4f eb02                 jmp 0xb18f53
00b18f51 8aca                 mov cl, dl
00b18f53 d944244c             fld dword ptr [esp + 0x4c]
00b18f57 c0e906               shr cl, 6
00b18f5a d81d9cf52201         fcomp dword ptr [0x122f59c]
00b18f60 80e101               and cl, 1
00b18f63 c0e106               shl cl, 6
00b18f66 32ca                 xor cl, dl
00b18f68 80e140               and cl, 0x40
00b18f6b dfe0                 fnstsw ax
00b18f6d 32ca                 xor cl, dl
00b18f6f 884e48               mov byte ptr [esi + 0x48], cl
00b18f72 f6c441               test ah, 0x41
00b18f75 7505                 jne 0xb18f7c
00b18f77 8a4748               mov al, byte ptr [edi + 0x48]
00b18f7a eb02                 jmp 0xb18f7e
00b18f7c 8ac1                 mov al, cl
00b18f7e c0e807               shr al, 7
00b18f81 c0e007               shl al, 7
00b18f84 80e17f               and cl, 0x7f
00b18f87 0ac8                 or cl, al
00b18f89 8b442418             mov eax, dword ptr [esp + 0x18]
00b18f8d 884e48               mov byte ptr [esi + 0x48], cl
00b18f90 89442448             mov dword ptr [esp + 0x48], eax
00b18f94 d9442448             fld dword ptr [esp + 0x48]
00b18f98 db5c241c             fistp dword ptr [esp + 0x1c]
00b18f9c 8a5f4d               mov bl, byte ptr [edi + 0x4d]
00b18f9f 8b4c241c             mov ecx, dword ptr [esp + 0x1c]
00b18fa3 8a574c               mov dl, byte ptr [edi + 0x4c]
00b18fa6 51                   push ecx
00b18fa7 51                   push ecx
00b18fa8 8a4f4f               mov cl, byte ptr [edi + 0x4f]
00b18fab 8bc4                 mov eax, esp
00b18fad 885c2450             mov byte ptr [esp + 0x50], bl
00b18fb1 8a5f4e               mov bl, byte ptr [edi + 0x4e]
00b18fb4 885802               mov byte ptr [eax + 2], bl
00b18fb7 8a5c2450             mov bl, byte ptr [esp + 0x50]
00b18fbb 885801               mov byte ptr [eax + 1], bl
00b18fbe 8810                 mov byte ptr [eax], dl
00b18fc0 884803               mov byte ptr [eax + 3], cl
00b18fc3 8a5e4d               mov bl, byte ptr [esi + 0x4d]
00b18fc6 8a564c               mov dl, byte ptr [esi + 0x4c]
00b18fc9 51                   push ecx
00b18fca 8a4e4f               mov cl, byte ptr [esi + 0x4f]
00b18fcd 8bc4                 mov eax, esp
00b18fcf 885c2454             mov byte ptr [esp + 0x54], bl
00b18fd3 8a5e4e               mov bl, byte ptr [esi + 0x4e]
00b18fd6 8810                 mov byte ptr [eax], dl
00b18fd8 885802               mov byte ptr [eax + 2], bl
00b18fdb 8a5c2454             mov bl, byte ptr [esp + 0x54]
00b18fdf 8d542420             lea edx, [esp + 0x20]
00b18fe3 52                   push edx
00b18fe4 885801               mov byte ptr [eax + 1], bl
00b18fe7 884803               mov byte ptr [eax + 3], cl
00b18fea e861b2f2ff           call 0xa44250
00b18fef 8b442424             mov eax, dword ptr [esp + 0x24]
00b18ff3 8b4c2428             mov ecx, dword ptr [esp + 0x28]
00b18ff7 83c410               add esp, 0x10
00b18ffa 89464c               mov dword ptr [esi + 0x4c], eax
00b18ffd 894c2448             mov dword ptr [esp + 0x48], ecx
00b19001 d9442448             fld dword ptr [esp + 0x48]
00b19005 db5c241c             fistp dword ptr [esp + 0x1c]
00b19009 8a5f51               mov bl, byte ptr [edi + 0x51]
00b1900c 8b54241c             mov edx, dword ptr [esp + 0x1c]
00b19010 52                   push edx
00b19011 8a5750               mov dl, byte ptr [edi + 0x50]
00b19014 51                   push ecx
00b19015 8a4f53               mov cl, byte ptr [edi + 0x53]
00b19018 8bc4                 mov eax, esp
00b1901a 885c2450             mov byte ptr [esp + 0x50], bl
00b1901e 8a5f52               mov bl, byte ptr [edi + 0x52]
00b19021 885802               mov byte ptr [eax + 2], bl
00b19024 8a5c2450             mov bl, byte ptr [esp + 0x50]
00b19028 885801               mov byte ptr [eax + 1], bl
00b1902b 8810                 mov byte ptr [eax], dl
00b1902d 884803               mov byte ptr [eax + 3], cl
00b19030 8a5e51               mov bl, byte ptr [esi + 0x51]
00b19033 8a5650               mov dl, byte ptr [esi + 0x50]
00b19036 51                   push ecx
00b19037 8a4e53               mov cl, byte ptr [esi + 0x53]
00b1903a 8bc4                 mov eax, esp
00b1903c 885c2454             mov byte ptr [esp + 0x54], bl
00b19040 8a5e52               mov bl, byte ptr [esi + 0x52]
00b19043 885802               mov byte ptr [eax + 2], bl
00b19046 8a5c2454             mov bl, byte ptr [esp + 0x54]
00b1904a 885801               mov byte ptr [eax + 1], bl
00b1904d 8810                 mov byte ptr [eax], dl
00b1904f 884803               mov byte ptr [eax + 3], cl
00b19052 8d442420             lea eax, [esp + 0x20]
00b19056 50                   push eax
00b19057 e8f4b1f2ff           call 0xa44250
00b1905c 8b4c2424             mov ecx, dword ptr [esp + 0x24]
00b19060 8b542428             mov edx, dword ptr [esp + 0x28]
00b19064 83c410               add esp, 0x10
00b19067 894e50               mov dword ptr [esi + 0x50], ecx
00b1906a 89542448             mov dword ptr [esp + 0x48], edx
00b1906e d9442448             fld dword ptr [esp + 0x48]
00b19072 db5c241c             fistp dword ptr [esp + 0x1c]
00b19076 8b44241c             mov eax, dword ptr [esp + 0x1c]
00b1907a 50                   push eax
00b1907b 51                   push ecx
00b1907c 8a4f57               mov cl, byte ptr [edi + 0x57]
00b1907f 8a5f55               mov bl, byte ptr [edi + 0x55]
00b19082 8a5754               mov dl, byte ptr [edi + 0x54]
00b19085 8bc4                 mov eax, esp
00b19087 885c2450             mov byte ptr [esp + 0x50], bl
00b1908b 8a5f56               mov bl, byte ptr [edi + 0x56]
00b1908e 885802               mov byte ptr [eax + 2], bl
00b19091 8a5c2450             mov bl, byte ptr [esp + 0x50]
00b19095 885801               mov byte ptr [eax + 1], bl
00b19098 884803               mov byte ptr [eax + 3], cl
00b1909b 8810                 mov byte ptr [eax], dl
00b1909d 8a5e55               mov bl, byte ptr [esi + 0x55]
00b190a0 8a5654               mov dl, byte ptr [esi + 0x54]
00b190a3 51                   push ecx
00b190a4 8a4e57               mov cl, byte ptr [esi + 0x57]
00b190a7 8bc4                 mov eax, esp
00b190a9 885c2454             mov byte ptr [esp + 0x54], bl
00b190ad 8a5e56               mov bl, byte ptr [esi + 0x56]
00b190b0 884803               mov byte ptr [eax + 3], cl
00b190b3 885802               mov byte ptr [eax + 2], bl
00b190b6 8a5c2454             mov bl, byte ptr [esp + 0x54]
00b190ba 8d4c2424             lea ecx, [esp + 0x24]
00b190be 51                   push ecx
00b190bf 885801               mov byte ptr [eax + 1], bl
00b190c2 8810                 mov byte ptr [eax], dl
00b190c4 e887b1f2ff           call 0xa44250
00b190c9 d944245c             fld dword ptr [esp + 0x5c]
00b190cd 8b542428             mov edx, dword ptr [esp + 0x28]
00b190d1 895654               mov dword ptr [esi + 0x54], edx
00b190d4 d84f24               fmul dword ptr [edi + 0x24]
00b190d7 d9442420             fld dword ptr [esp + 0x20]
00b190db 83c410               add esp, 0x10
00b190de d84e24               fmul dword ptr [esi + 0x24]
00b190e1 dec1                 faddp st(1)
00b190e3 d95e24               fstp dword ptr [esi + 0x24]
00b190e6 d944244c             fld dword ptr [esp + 0x4c]
00b190ea d81d9cf52201         fcomp dword ptr [0x122f59c]
00b190f0 dfe0                 fnstsw ax
00b190f2 f6c441               test ah, 0x41
00b190f5 7505                 jne 0xb190fc
00b190f7 8a4749               mov al, byte ptr [edi + 0x49]
00b190fa eb03                 jmp 0xb190ff
00b190fc 8a4649               mov al, byte ptr [esi + 0x49]
00b190ff 8a4e49               mov cl, byte ptr [esi + 0x49]
00b19102 d944244c             fld dword ptr [esp + 0x4c]
00b19106 d0e8                 shr al, 1
00b19108 2401                 and al, 1
00b1910a d0e0                 shl al, 1
00b1910c 32c1                 xor al, cl
00b1910e 2402                 and al, 2
00b19110 8ad9                 mov bl, cl
00b19112 32d8                 xor bl, al
00b19114 885e49               mov byte ptr [esi + 0x49], bl
00b19117 d84f28               fmul dword ptr [edi + 0x28]
00b1911a d9442410             fld dword ptr [esp + 0x10]
00b1911e b8abaaaa2a           mov eax, 0x2aaaaaab
00b19123 d84e28               fmul dword ptr [esi + 0x28]
00b19126 dec1                 faddp st(1)
00b19128 d95e28               fstp dword ptr [esi + 0x28]
00b1912b 8b5760               mov edx, dword ptr [edi + 0x60]
00b1912e 8b4f64               mov ecx, dword ptr [edi + 0x64]
00b19131 2bca                 sub ecx, edx
00b19133 f7e9                 imul ecx
00b19135 d1fa                 sar edx, 1
00b19137 8bc2                 mov eax, edx
00b19139 c1e81f               shr eax, 0x1f
00b1913c 03c2                 add eax, edx
00b1913e 33c9                 xor ecx, ecx
00b19140 83f804               cmp eax, 4
00b19143 8944241c             mov dword ptr [esp + 0x1c], eax
00b19147 0f8cec010000         jl 0xb19339
00b1914d d9442410             fld dword ptr [esp + 0x10]
00b19151 83c0fc               add eax, -4
00b19154 c1e802               shr eax, 2
00b19157 40                   inc eax
00b19158 89442448             mov dword ptr [esp + 0x48], eax
00b1915c c1e002               shl eax, 2
00b1915f 89442418             mov dword ptr [esp + 0x18], eax
00b19163 8b5760               mov edx, dword ptr [edi + 0x60]
00b19166 d944244c             fld dword ptr [esp + 0x4c]
00b1916a d80c0a               fmul dword ptr [edx + ecx]
00b1916d 8b4660               mov eax, dword ptr [esi + 0x60]
00b19170 d944244c             fld dword ptr [esp + 0x4c]
00b19174 03d1                 add edx, ecx
00b19176 d84a04               fmul dword ptr [edx + 4]
00b19179 03c1                 add eax, ecx
00b1917b d944244c             fld dword ptr [esp + 0x4c]
00b1917f 8d5924               lea ebx, [ecx + 0x24]
00b19182 d84a08               fmul dword ptr [edx + 8]
00b19185 d95c2440             fstp dword ptr [esp + 0x40]
00b19189 d9c2                 fld st(2)
00b1918b d808                 fmul dword ptr [eax]
00b1918d d9c3                 fld st(3)
00b1918f d84804               fmul dword ptr [eax + 4]
00b19192 d95c2430             fstp dword ptr [esp + 0x30]
00b19196 d9c3                 fld st(3)
00b19198 d84808               fmul dword ptr [eax + 8]
00b1919b d95c2434             fstp dword ptr [esp + 0x34]
00b1919f d8c2                 fadd st(2)
00b191a1 d95c2420             fstp dword ptr [esp + 0x20]
00b191a5 8b542420             mov edx, dword ptr [esp + 0x20]
00b191a9 d9442430             fld dword ptr [esp + 0x30]
00b191ad 8910                 mov dword ptr [eax], edx
00b191af d8c1                 fadd st(1)
00b191b1 d95c2424             fstp dword ptr [esp + 0x24]
00b191b5 8b542424             mov edx, dword ptr [esp + 0x24]
00b191b9 895004               mov dword ptr [eax + 4], edx
00b191bc ddd8                 fstp st(0)
00b191be ddd8                 fstp st(0)
00b191c0 d9442434             fld dword ptr [esp + 0x34]
00b191c4 d8442440             fadd dword ptr [esp + 0x40]
00b191c8 d95c2428             fstp dword ptr [esp + 0x28]
00b191cc 8b542428             mov edx, dword ptr [esp + 0x28]
00b191d0 d944244c             fld dword ptr [esp + 0x4c]
00b191d4 895008               mov dword ptr [eax + 8], edx
00b191d7 8b4760               mov eax, dword ptr [edi + 0x60]
00b191da d84c010c             fmul dword ptr [ecx + eax + 0xc]
00b191de 8d54010c             lea edx, [ecx + eax + 0xc]
00b191e2 d944244c             fld dword ptr [esp + 0x4c]
00b191e6 8b4660               mov eax, dword ptr [esi + 0x60]
00b191e9 d84a04               fmul dword ptr [edx + 4]
00b191ec 8d44010c             lea eax, [ecx + eax + 0xc]
00b191f0 d944244c             fld dword ptr [esp + 0x4c]
00b191f4 d84a08               fmul dword ptr [edx + 8]
00b191f7 d95c2440             fstp dword ptr [esp + 0x40]
00b191fb d9c2                 fld st(2)
00b191fd d808                 fmul dword ptr [eax]
00b191ff d9c3                 fld st(3)
00b19201 d84804               fmul dword ptr [eax + 4]
00b19204 d95c2430             fstp dword ptr [esp + 0x30]
00b19208 d9c3                 fld st(3)
00b1920a d84808               fmul dword ptr [eax + 8]
00b1920d d95c2434             fstp dword ptr [esp + 0x34]
00b19211 d8c2                 fadd st(2)
00b19213 d95c2420             fstp dword ptr [esp + 0x20]
00b19217 8b542420             mov edx, dword ptr [esp + 0x20]
00b1921b d9442430             fld dword ptr [esp + 0x30]
00b1921f 8910                 mov dword ptr [eax], edx
00b19221 d8c1                 fadd st(1)
00b19223 d95c2424             fstp dword ptr [esp + 0x24]
00b19227 8b542424             mov edx, dword ptr [esp + 0x24]
00b1922b 895004               mov dword ptr [eax + 4], edx
00b1922e ddd8                 fstp st(0)
00b19230 ddd8                 fstp st(0)
00b19232 d9442434             fld dword ptr [esp + 0x34]
00b19236 d8442440             fadd dword ptr [esp + 0x40]
00b1923a d95c2428             fstp dword ptr [esp + 0x28]
00b1923e 8b542428             mov edx, dword ptr [esp + 0x28]
00b19242 d944244c             fld dword ptr [esp + 0x4c]
00b19246 895008               mov dword ptr [eax + 8], edx
00b19249 8b5760               mov edx, dword ptr [edi + 0x60]
00b1924c 8b6e60               mov ebp, dword ptr [esi + 0x60]
00b1924f 8d43f4               lea eax, [ebx - 0xc]
00b19252 d80c02               fmul dword ptr [edx + eax]
00b19255 03d0                 add edx, eax
00b19257 d944244c             fld dword ptr [esp + 0x4c]
00b1925b 03c5                 add eax, ebp
00b1925d d84a04               fmul dword ptr [edx + 4]
00b19260 d944244c             fld dword ptr [esp + 0x4c]
00b19264 d84a08               fmul dword ptr [edx + 8]
00b19267 83c130               add ecx, 0x30
00b1926a d95c2440             fstp dword ptr [esp + 0x40]
00b1926e d9c2                 fld st(2)
00b19270 d808                 fmul dword ptr [eax]
00b19272 d9c3                 fld st(3)
00b19274 d84804               fmul dword ptr [eax + 4]
00b19277 d95c2430             fstp dword ptr [esp + 0x30]
00b1927b d9c3                 fld st(3)
00b1927d d84808               fmul dword ptr [eax + 8]
00b19280 d95c2434             fstp dword ptr [esp + 0x34]
00b19284 d8c2                 fadd st(2)
00b19286 d95c2420             fstp dword ptr [esp + 0x20]
00b1928a 8b542420             mov edx, dword ptr [esp + 0x20]
00b1928e d9442430             fld dword ptr [esp + 0x30]
00b19292 8910                 mov dword ptr [eax], edx
00b19294 d8c1                 fadd st(1)
00b19296 d95c2424             fstp dword ptr [esp + 0x24]
00b1929a 8b542424             mov edx, dword ptr [esp + 0x24]
00b1929e 895004               mov dword ptr [eax + 4], edx
00b192a1 ddd8                 fstp st(0)
00b192a3 ddd8                 fstp st(0)
00b192a5 d9442434             fld dword ptr [esp + 0x34]
00b192a9 d8442440             fadd dword ptr [esp + 0x40]
00b192ad d95c2428             fstp dword ptr [esp + 0x28]
00b192b1 8b542428             mov edx, dword ptr [esp + 0x28]
00b192b5 d944244c             fld dword ptr [esp + 0x4c]
00b192b9 895008               mov dword ptr [eax + 8], edx
00b192bc 8b5760               mov edx, dword ptr [edi + 0x60]
00b192bf d80c1a               fmul dword ptr [edx + ebx]
00b192c2 8b4660               mov eax, dword ptr [esi + 0x60]
00b192c5 d944244c             fld dword ptr [esp + 0x4c]
00b192c9 03d3                 add edx, ebx
00b192cb d84a04               fmul dword ptr [edx + 4]
00b192ce 03c3                 add eax, ebx
00b192d0 d944244c             fld dword ptr [esp + 0x4c]
00b192d4 d84a08               fmul dword ptr [edx + 8]
00b192d7 d95c2440             fstp dword ptr [esp + 0x40]
00b192db d9c2                 fld st(2)
00b192dd d808                 fmul dword ptr [eax]
00b192df d9c3                 fld st(3)
00b192e1 d84804               fmul dword ptr [eax + 4]
00b192e4 d95c2430             fstp dword ptr [esp + 0x30]
00b192e8 d9c3                 fld st(3)
00b192ea d84808               fmul dword ptr [eax + 8]
00b192ed d95c2434             fstp dword ptr [esp + 0x34]
00b192f1 d8c2                 fadd st(2)
00b192f3 d95c2420             fstp dword ptr [esp + 0x20]
00b192f7 8b542420             mov edx, dword ptr [esp + 0x20]
00b192fb d9442430             fld dword ptr [esp + 0x30]
00b192ff 8910                 mov dword ptr [eax], edx
00b19301 d8c1                 fadd st(1)
00b19303 d95c2424             fstp dword ptr [esp + 0x24]
00b19307 8b542424             mov edx, dword ptr [esp + 0x24]
00b1930b 895004               mov dword ptr [eax + 4], edx
00b1930e ddd8                 fstp st(0)
00b19310 ddd8                 fstp st(0)
00b19312 d9442434             fld dword ptr [esp + 0x34]
00b19316 d8442440             fadd dword ptr [esp + 0x40]
00b1931a d95c2428             fstp dword ptr [esp + 0x28]
00b1931e 8b542428             mov edx, dword ptr [esp + 0x28]
00b19322 895008               mov dword ptr [eax + 8], edx
00b19325 ff4c2448             dec dword ptr [esp + 0x48]
00b19329 0f8534feffff         jne 0xb19163
00b1932f 8b44241c             mov eax, dword ptr [esp + 0x1c]
00b19333 ddd8                 fstp st(0)
00b19335 8b4c2418             mov ecx, dword ptr [esp + 0x18]
00b19339 3bc8                 cmp ecx, eax
00b1933b 0f8384000000         jae 0xb193c5
00b19341 d9442410             fld dword ptr [esp + 0x10]
00b19345 8d1449               lea edx, [ecx + ecx*2]
00b19348 c1e202               shl edx, 2
00b1934b 2bc1                 sub eax, ecx
00b1934d 8bd8                 mov ebx, eax
00b1934f 90                   nop 
00b19350 8b4f60               mov ecx, dword ptr [edi + 0x60]
00b19353 d944244c             fld dword ptr [esp + 0x4c]
00b19357 d80c11               fmul dword ptr [ecx + edx]
00b1935a 8b4660               mov eax, dword ptr [esi + 0x60]
00b1935d d944244c             fld dword ptr [esp + 0x4c]
00b19361 03ca                 add ecx, edx
00b19363 d84904               fmul dword ptr [ecx + 4]
00b19366 03c2                 add eax, edx
00b19368 d944244c             fld dword ptr [esp + 0x4c]
00b1936c 83c20c               add edx, 0xc
00b1936f 4b                   dec ebx
00b19370 d84908               fmul dword ptr [ecx + 8]
00b19373 d95c2440             fstp dword ptr [esp + 0x40]
00b19377 d9c2                 fld st(2)
00b19379 d808                 fmul dword ptr [eax]
00b1937b d9c3                 fld st(3)
00b1937d d84804               fmul dword ptr [eax + 4]
00b19380 d95c2430             fstp dword ptr [esp + 0x30]
00b19384 d9c3                 fld st(3)
00b19386 d84808               fmul dword ptr [eax + 8]
00b19389 d95c2434             fstp dword ptr [esp + 0x34]
00b1938d d8c2                 fadd st(2)
00b1938f d95c2420             fstp dword ptr [esp + 0x20]
00b19393 8b4c2420             mov ecx, dword ptr [esp + 0x20]
00b19397 d9442430             fld dword ptr [esp + 0x30]
00b1939b 8908                 mov dword ptr [eax], ecx
00b1939d d8c1                 fadd st(1)
00b1939f d95c2424             fstp dword ptr [esp + 0x24]
00b193a3 8b4c2424             mov ecx, dword ptr [esp + 0x24]
00b193a7 894804               mov dword ptr [eax + 4], ecx
00b193aa ddd8                 fstp st(0)
00b193ac ddd8                 fstp st(0)
00b193ae d9442434             fld dword ptr [esp + 0x34]
00b193b2 d8442440             fadd dword ptr [esp + 0x40]
00b193b6 d95c2428             fstp dword ptr [esp + 0x28]
00b193ba 8b4c2428             mov ecx, dword ptr [esp + 0x28]
00b193be 894808               mov dword ptr [eax + 8], ecx
00b193c1 758d                 jne 0xb19350
00b193c3 ddd8                 fstp st(0)
00b193c5 5f                   pop edi
00b193c6 5e                   pop esi
00b193c7 5d                   pop ebp
00b193c8 5b                   pop ebx
00b193c9 83c434               add esp, 0x34
00b193cc c20800               ret 8
