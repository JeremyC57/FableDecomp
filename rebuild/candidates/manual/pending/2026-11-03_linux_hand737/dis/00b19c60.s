00b19c60 81eca4000000         sub esp, 0xa4
00b19c66 56                   push esi
00b19c67 8bf1                 mov esi, ecx
00b19c69 f6464804             test byte ptr [esi + 0x48], 4
00b19c6d 57                   push edi
00b19c6e 0f8484090000         je 0xb1a5f8
00b19c74 8a4614               mov al, byte ptr [esi + 0x14]
00b19c77 84c0                 test al, al
00b19c79 0f8479090000         je 0xb1a5f8
00b19c7f 53                   push ebx
00b19c80 55                   push ebp
00b19c81 e86a0dfdff           call 0xaea9f0
00b19c86 8a5e48               mov bl, byte ptr [esi + 0x48]
00b19c89 89442410             mov dword ptr [esp + 0x10], eax
00b19c8d db442410             fild dword ptr [esp + 0x10]
00b19c91 db4638               fild dword ptr [esi + 0x38]
00b19c94 8ac3                 mov al, bl
00b19c96 c0e805               shr al, 5
00b19c99 2401                 and al, 1
00b19c9b d80d00632601         fmul dword ptr [0x1266300]
00b19ca1 d95c2420             fstp dword ptr [esp + 0x20]
00b19ca5 db463c               fild dword ptr [esi + 0x3c]
00b19ca8 d80d00632601         fmul dword ptr [0x1266300]
00b19cae d95c2410             fstp dword ptr [esp + 0x10]
00b19cb2 7405                 je 0xb19cb9
00b19cb4 8d4e4c               lea ecx, [esi + 0x4c]
00b19cb7 eb2b                 jmp 0xb19ce4
00b19cb9 f6c340               test bl, 0x40
00b19cbc 7405                 je 0xb19cc3
00b19cbe 8d4e50               lea ecx, [esi + 0x50]
00b19cc1 eb21                 jmp 0xb19ce4
00b19cc3 84db                 test bl, bl
00b19cc5 7905                 jns 0xb19ccc
00b19cc7 8d4e54               lea ecx, [esi + 0x54]
00b19cca eb18                 jmp 0xb19ce4
00b19ccc c644241aff           mov byte ptr [esp + 0x1a], 0xff
00b19cd1 c6442419ff           mov byte ptr [esp + 0x19], 0xff
00b19cd6 c6442418ff           mov byte ptr [esp + 0x18], 0xff
00b19cdb c644241bff           mov byte ptr [esp + 0x1b], 0xff
00b19ce0 8d4c2418             lea ecx, [esp + 0x18]
00b19ce4 84db                 test bl, bl
00b19ce6 0fb65102             movzx edx, byte ptr [ecx + 2]
00b19cea 89542414             mov dword ptr [esp + 0x14], edx
00b19cee 0fb65101             movzx edx, byte ptr [ecx + 1]
00b19cf2 db442414             fild dword ptr [esp + 0x14]
00b19cf6 89542414             mov dword ptr [esp + 0x14], edx
00b19cfa 0fb611               movzx edx, byte ptr [ecx]
00b19cfd d80d24172301         fmul dword ptr [0x1231724]
00b19d03 0fb64903             movzx ecx, byte ptr [ecx + 3]
00b19d07 d95c2434             fstp dword ptr [esp + 0x34]
00b19d0b db442414             fild dword ptr [esp + 0x14]
00b19d0f 89542414             mov dword ptr [esp + 0x14], edx
00b19d13 894c2418             mov dword ptr [esp + 0x18], ecx
00b19d17 d80d24172301         fmul dword ptr [0x1231724]
00b19d1d d95c2438             fstp dword ptr [esp + 0x38]
00b19d21 db442414             fild dword ptr [esp + 0x14]
00b19d25 d80d24172301         fmul dword ptr [0x1231724]
00b19d2b d95c243c             fstp dword ptr [esp + 0x3c]
00b19d2f db442418             fild dword ptr [esp + 0x18]
00b19d33 d80d24172301         fmul dword ptr [0x1231724]
00b19d39 d95c2440             fstp dword ptr [esp + 0x40]
00b19d3d 7905                 jns 0xb19d44
00b19d3f 8d4654               lea eax, [esi + 0x54]
00b19d42 eb2b                 jmp 0xb19d6f
00b19d44 f6c340               test bl, 0x40
00b19d47 7405                 je 0xb19d4e
00b19d49 8d4650               lea eax, [esi + 0x50]
00b19d4c eb21                 jmp 0xb19d6f
00b19d4e 84c0                 test al, al
00b19d50 7405                 je 0xb19d57
00b19d52 8d464c               lea eax, [esi + 0x4c]
00b19d55 eb18                 jmp 0xb19d6f
00b19d57 c644241aff           mov byte ptr [esp + 0x1a], 0xff
00b19d5c c6442419ff           mov byte ptr [esp + 0x19], 0xff
00b19d61 c6442418ff           mov byte ptr [esp + 0x18], 0xff
00b19d66 c644241bff           mov byte ptr [esp + 0x1b], 0xff
00b19d6b 8d442418             lea eax, [esp + 0x18]
00b19d6f f6c340               test bl, 0x40
00b19d72 0fb65002             movzx edx, byte ptr [eax + 2]
00b19d76 0fb64801             movzx ecx, byte ptr [eax + 1]
00b19d7a 89542414             mov dword ptr [esp + 0x14], edx
00b19d7e 0fb610               movzx edx, byte ptr [eax]
00b19d81 db442414             fild dword ptr [esp + 0x14]
00b19d85 0fb64003             movzx eax, byte ptr [eax + 3]
00b19d89 d80d24172301         fmul dword ptr [0x1231724]
00b19d8f 894c2414             mov dword ptr [esp + 0x14], ecx
00b19d93 89442418             mov dword ptr [esp + 0x18], eax
00b19d97 d95c2444             fstp dword ptr [esp + 0x44]
00b19d9b db442414             fild dword ptr [esp + 0x14]
00b19d9f 89542414             mov dword ptr [esp + 0x14], edx
00b19da3 d80d24172301         fmul dword ptr [0x1231724]
00b19da9 d95c2448             fstp dword ptr [esp + 0x48]
00b19dad db442414             fild dword ptr [esp + 0x14]
00b19db1 d80d24172301         fmul dword ptr [0x1231724]
00b19db7 d95c244c             fstp dword ptr [esp + 0x4c]
00b19dbb db442418             fild dword ptr [esp + 0x18]
00b19dbf d80d24172301         fmul dword ptr [0x1231724]
00b19dc5 d95c2450             fstp dword ptr [esp + 0x50]
00b19dc9 7408                 je 0xb19dd3
00b19dcb 8d4650               lea eax, [esi + 0x50]
00b19dce e9d8000000           jmp 0xb19eab
00b19dd3 d9442444             fld dword ptr [esp + 0x44]
00b19dd7 d80d9cf52201         fmul dword ptr [0x122f59c]
00b19ddd d9442448             fld dword ptr [esp + 0x48]
00b19de1 d80d9cf52201         fmul dword ptr [0x122f59c]
00b19de7 d944244c             fld dword ptr [esp + 0x4c]
00b19deb d80d9cf52201         fmul dword ptr [0x122f59c]
00b19df1 d95c2478             fstp dword ptr [esp + 0x78]
00b19df5 d9442450             fld dword ptr [esp + 0x50]
00b19df9 d80d9cf52201         fmul dword ptr [0x122f59c]
00b19dff d95c247c             fstp dword ptr [esp + 0x7c]
00b19e03 d9442434             fld dword ptr [esp + 0x34]
00b19e07 d80d9cf52201         fmul dword ptr [0x122f59c]
00b19e0d d9442438             fld dword ptr [esp + 0x38]
00b19e11 d80d9cf52201         fmul dword ptr [0x122f59c]
00b19e17 d95c2428             fstp dword ptr [esp + 0x28]
00b19e1b d944243c             fld dword ptr [esp + 0x3c]
00b19e1f d80d9cf52201         fmul dword ptr [0x122f59c]
00b19e25 d95c242c             fstp dword ptr [esp + 0x2c]
00b19e29 d9442440             fld dword ptr [esp + 0x40]
00b19e2d d80d9cf52201         fmul dword ptr [0x122f59c]
00b19e33 d95c2430             fstp dword ptr [esp + 0x30]
00b19e37 d8c2                 fadd st(2)
00b19e39 d95c2454             fstp dword ptr [esp + 0x54]
00b19e3d d9442428             fld dword ptr [esp + 0x28]
00b19e41 d8c1                 fadd st(1)
00b19e43 ddda                 fstp st(2)
00b19e45 ddd8                 fstp st(0)
00b19e47 d944242c             fld dword ptr [esp + 0x2c]
00b19e4b d8442478             fadd dword ptr [esp + 0x78]
00b19e4f d95c245c             fstp dword ptr [esp + 0x5c]
00b19e53 d9442430             fld dword ptr [esp + 0x30]
00b19e57 d844247c             fadd dword ptr [esp + 0x7c]
00b19e5b d95c2460             fstp dword ptr [esp + 0x60]
00b19e5f d9442454             fld dword ptr [esp + 0x54]
00b19e63 d80d14002301         fmul dword ptr [0x1230014]
00b19e69 e8024c0e00           call 0xbfea70
00b19e6e d80d14002301         fmul dword ptr [0x1230014]
00b19e74 8844241a             mov byte ptr [esp + 0x1a], al
00b19e78 e8f34b0e00           call 0xbfea70
00b19e7d d944245c             fld dword ptr [esp + 0x5c]
00b19e81 d80d14002301         fmul dword ptr [0x1230014]
00b19e87 88442419             mov byte ptr [esp + 0x19], al
00b19e8b e8e04b0e00           call 0xbfea70
00b19e90 d9442460             fld dword ptr [esp + 0x60]
00b19e94 d80d14002301         fmul dword ptr [0x1230014]
00b19e9a 88442418             mov byte ptr [esp + 0x18], al
00b19e9e e8cd4b0e00           call 0xbfea70
00b19ea3 8844241b             mov byte ptr [esp + 0x1b], al
00b19ea7 8d442418             lea eax, [esp + 0x18]
00b19eab f6c302               test bl, 2
00b19eae 0fb64802             movzx ecx, byte ptr [eax + 2]
00b19eb2 0fb65001             movzx edx, byte ptr [eax + 1]
00b19eb6 894c2414             mov dword ptr [esp + 0x14], ecx
00b19eba 0fb608               movzx ecx, byte ptr [eax]
00b19ebd db442414             fild dword ptr [esp + 0x14]
00b19ec1 89542414             mov dword ptr [esp + 0x14], edx
00b19ec5 0fb65003             movzx edx, byte ptr [eax + 3]
00b19ec9 d80d24172301         fmul dword ptr [0x1231724]
00b19ecf 89542418             mov dword ptr [esp + 0x18], edx
00b19ed3 d95c2424             fstp dword ptr [esp + 0x24]
00b19ed7 c744241c00000000     mov dword ptr [esp + 0x1c], 0
00b19edf db442414             fild dword ptr [esp + 0x14]
00b19ee3 894c2414             mov dword ptr [esp + 0x14], ecx
00b19ee7 d80d24172301         fmul dword ptr [0x1231724]
00b19eed d95c2428             fstp dword ptr [esp + 0x28]
00b19ef1 db442414             fild dword ptr [esp + 0x14]
00b19ef5 d80d24172301         fmul dword ptr [0x1231724]
00b19efb d95c242c             fstp dword ptr [esp + 0x2c]
00b19eff db442418             fild dword ptr [esp + 0x18]
00b19f03 c744241800000000     mov dword ptr [esp + 0x18], 0
00b19f0b d80d24172301         fmul dword ptr [0x1231724]
00b19f11 d95c2430             fstp dword ptr [esp + 0x30]
00b19f15 0f848b030000         je 0xb1a2a6
00b19f1b db460c               fild dword ptr [esi + 0xc]
00b19f1e d88424b8000000       fadd dword ptr [esp + 0xb8]
00b19f25 d9c9                 fxch st(1)
00b19f27 d84e1c               fmul dword ptr [esi + 0x1c]
00b19f2a def9                 fdivp st(1)
00b19f2c d954241c             fst dword ptr [esp + 0x1c]
00b19f30 d81ddcde2201         fcomp dword ptr [0x122dedc]
00b19f36 dfe0                 fnstsw ax
00b19f38 f6c405               test ah, 5
00b19f3b 7a0a                 jp 0xb19f47
00b19f3d c744241c00000000     mov dword ptr [esp + 0x1c], 0
00b19f45 eb19                 jmp 0xb19f60
00b19f47 d944241c             fld dword ptr [esp + 0x1c]
00b19f4b d81dd8de2201         fcomp dword ptr [0x122ded8]
00b19f51 dfe0                 fnstsw ax
00b19f53 f6c441               test ah, 0x41
00b19f56 7508                 jne 0xb19f60
00b19f58 c744241c0000803f     mov dword ptr [esp + 0x1c], 0x3f800000
00b19f60 d944241c             fld dword ptr [esp + 0x1c]
00b19f64 8b44241c             mov eax, dword ptr [esp + 0x1c]
00b19f68 d81ddcde2201         fcomp dword ptr [0x122dedc]
00b19f6e 89442418             mov dword ptr [esp + 0x18], eax
00b19f72 dfe0                 fnstsw ax
00b19f74 f6c405               test ah, 5
00b19f77 7a0a                 jp 0xb19f83
00b19f79 c744241800000000     mov dword ptr [esp + 0x18], 0
00b19f81 eb21                 jmp 0xb19fa4
00b19f83 d905d8de2201         fld dword ptr [0x122ded8]
00b19f89 d8253cba2901         fsub dword ptr [0x129ba3c]
00b19f8f d944241c             fld dword ptr [esp + 0x1c]
00b19f93 d8d9                 fcomp st(1)
00b19f95 dfe0                 fnstsw ax
00b19f97 f6c441               test ah, 0x41
00b19f9a 7506                 jne 0xb19fa2
00b19f9c d95c2418             fstp dword ptr [esp + 0x18]
00b19fa0 eb02                 jmp 0xb19fa4
00b19fa2 ddd8                 fstp st(0)
00b19fa4 d944241c             fld dword ptr [esp + 0x1c]
00b19fa8 d81d9cf52201         fcomp dword ptr [0x122f59c]
00b19fae dfe0                 fnstsw ax
00b19fb0 f6c405               test ah, 5
00b19fb3 0f8bef020000         jnp 0xb1a2a8
00b19fb9 d944241c             fld dword ptr [esp + 0x1c]
00b19fbd d8259cf52201         fsub dword ptr [0x122f59c]
00b19fc3 dcc0                 fadd st(0), st(0)
00b19fc5 d9442444             fld dword ptr [esp + 0x44]
00b19fc9 d8c9                 fmul st(1)
00b19fcb d9442448             fld dword ptr [esp + 0x48]
00b19fcf d8ca                 fmul st(2)
00b19fd1 d95c2458             fstp dword ptr [esp + 0x58]
00b19fd5 d944244c             fld dword ptr [esp + 0x4c]
00b19fd9 d8ca                 fmul st(2)
00b19fdb d95c245c             fstp dword ptr [esp + 0x5c]
00b19fdf d9442450             fld dword ptr [esp + 0x50]
00b19fe3 d8ca                 fmul st(2)
00b19fe5 d95c2460             fstp dword ptr [esp + 0x60]
00b19fe9 d905d8de2201         fld dword ptr [0x122ded8]
00b19fef d8e2                 fsub st(2)
00b19ff1 d9442424             fld dword ptr [esp + 0x24]
00b19ff5 d8c9                 fmul st(1)
00b19ff7 d95c2444             fstp dword ptr [esp + 0x44]
00b19ffb d9442428             fld dword ptr [esp + 0x28]
00b19fff d8c9                 fmul st(1)
00b1a001 d95c2448             fstp dword ptr [esp + 0x48]
00b1a005 d944242c             fld dword ptr [esp + 0x2c]
00b1a009 d8c9                 fmul st(1)
00b1a00b d95c244c             fstp dword ptr [esp + 0x4c]
00b1a00f d9442430             fld dword ptr [esp + 0x30]
00b1a013 d8c9                 fmul st(1)
00b1a015 d95c2450             fstp dword ptr [esp + 0x50]
00b1a019 ddd8                 fstp st(0)
00b1a01b d9442444             fld dword ptr [esp + 0x44]
00b1a01f d8c1                 fadd st(1)
00b1a021 d95c2424             fstp dword ptr [esp + 0x24]
00b1a025 8b542424             mov edx, dword ptr [esp + 0x24]
00b1a029 89542434             mov dword ptr [esp + 0x34], edx
00b1a02d ddd8                 fstp st(0)
00b1a02f ddd8                 fstp st(0)
00b1a031 d9442448             fld dword ptr [esp + 0x48]
00b1a035 d8442458             fadd dword ptr [esp + 0x58]
00b1a039 d95c2428             fstp dword ptr [esp + 0x28]
00b1a03d 8b442428             mov eax, dword ptr [esp + 0x28]
00b1a041 d944244c             fld dword ptr [esp + 0x4c]
00b1a045 89442438             mov dword ptr [esp + 0x38], eax
00b1a049 d844245c             fadd dword ptr [esp + 0x5c]
00b1a04d d95c242c             fstp dword ptr [esp + 0x2c]
00b1a051 8b4c242c             mov ecx, dword ptr [esp + 0x2c]
00b1a055 d9442450             fld dword ptr [esp + 0x50]
00b1a059 894c243c             mov dword ptr [esp + 0x3c], ecx
00b1a05d d8442460             fadd dword ptr [esp + 0x60]
00b1a061 d95c2430             fstp dword ptr [esp + 0x30]
00b1a065 8b542430             mov edx, dword ptr [esp + 0x30]
00b1a069 89542440             mov dword ptr [esp + 0x40], edx
00b1a06d d944241c             fld dword ptr [esp + 0x1c]
00b1a071 8bbc24c0000000       mov edi, dword ptr [esp + 0xc0]
00b1a078 d85c2420             fcomp dword ptr [esp + 0x20]
00b1a07c 8b4754               mov eax, dword ptr [edi + 0x54]
00b1a07f 89442414             mov dword ptr [esp + 0x14], eax
00b1a083 dfe0                 fnstsw ax
00b1a085 f6c405               test ah, 5
00b1a088 7a1b                 jp 0xb1a0a5
00b1a08a d944241c             fld dword ptr [esp + 0x1c]
00b1a08e 51                   push ecx
00b1a08f d8742424             fdiv dword ptr [esp + 0x24]
00b1a093 8bce                 mov ecx, esi
00b1a095 d91c24               fstp dword ptr [esp]
00b1a098 e8f30c0000           call 0xb1ad90
00b1a09d d84c2414             fmul dword ptr [esp + 0x14]
00b1a0a1 d95c2414             fstp dword ptr [esp + 0x14]
00b1a0a5 d944241c             fld dword ptr [esp + 0x1c]
00b1a0a9 d85c2410             fcomp dword ptr [esp + 0x10]
00b1a0ad dfe0                 fnstsw ax
00b1a0af f6c441               test ah, 0x41
00b1a0b2 7529                 jne 0xb1a0dd
00b1a0b4 d905d8de2201         fld dword ptr [0x122ded8]
00b1a0ba 51                   push ecx
00b1a0bb d8642420             fsub dword ptr [esp + 0x20]
00b1a0bf 8bce                 mov ecx, esi
00b1a0c1 d905d8de2201         fld dword ptr [0x122ded8]
00b1a0c7 d8642414             fsub dword ptr [esp + 0x14]
00b1a0cb def9                 fdivp st(1)
00b1a0cd d91c24               fstp dword ptr [esp]
00b1a0d0 e8bb0c0000           call 0xb1ad90
00b1a0d5 d84c2414             fmul dword ptr [esp + 0x14]
00b1a0d9 d95c2414             fstp dword ptr [esp + 0x14]
00b1a0dd d9442414             fld dword ptr [esp + 0x14]
00b1a0e1 c744241c0000803f     mov dword ptr [esp + 0x1c], 0x3f800000
00b1a0e9 d81dd8de2201         fcomp dword ptr [0x122ded8]
00b1a0ef dfe0                 fnstsw ax
00b1a0f1 f6c405               test ah, 5
00b1a0f4 0f8aa6020000         jp 0xb1a3a0
00b1a0fa 8a4e48               mov cl, byte ptr [esi + 0x48]
00b1a0fd f6c110               test cl, 0x10
00b1a100 0f8481020000         je 0xb1a387
00b1a106 0fb65646             movzx edx, byte ptr [esi + 0x46]
00b1a10a 0fb64645             movzx eax, byte ptr [esi + 0x45]
00b1a10e 89542410             mov dword ptr [esp + 0x10], edx
00b1a112 0fb65644             movzx edx, byte ptr [esi + 0x44]
00b1a116 db442410             fild dword ptr [esp + 0x10]
00b1a11a 89442410             mov dword ptr [esp + 0x10], eax
00b1a11e d80d24172301         fmul dword ptr [0x1231724]
00b1a124 0fb64647             movzx eax, byte ptr [esi + 0x47]
00b1a128 db442410             fild dword ptr [esp + 0x10]
00b1a12c 89542410             mov dword ptr [esp + 0x10], edx
00b1a130 d80d24172301         fmul dword ptr [0x1231724]
00b1a136 0fb65646             movzx edx, byte ptr [esi + 0x46]
00b1a13a db442410             fild dword ptr [esp + 0x10]
00b1a13e 89442410             mov dword ptr [esp + 0x10], eax
00b1a142 d80d24172301         fmul dword ptr [0x1231724]
00b1a148 0fb64645             movzx eax, byte ptr [esi + 0x45]
00b1a14c d95c244c             fstp dword ptr [esp + 0x4c]
00b1a150 db442410             fild dword ptr [esp + 0x10]
00b1a154 89542410             mov dword ptr [esp + 0x10], edx
00b1a158 0fb65644             movzx edx, byte ptr [esi + 0x44]
00b1a15c d80d24172301         fmul dword ptr [0x1231724]
00b1a162 d95c2450             fstp dword ptr [esp + 0x50]
00b1a166 db442410             fild dword ptr [esp + 0x10]
00b1a16a 89442410             mov dword ptr [esp + 0x10], eax
00b1a16e 0fb64647             movzx eax, byte ptr [esi + 0x47]
00b1a172 d80d24172301         fmul dword ptr [0x1231724]
00b1a178 db442410             fild dword ptr [esp + 0x10]
00b1a17c 89542410             mov dword ptr [esp + 0x10], edx
00b1a180 d80d24172301         fmul dword ptr [0x1231724]
00b1a186 db442410             fild dword ptr [esp + 0x10]
00b1a18a 89442410             mov dword ptr [esp + 0x10], eax
00b1a18e d80d24172301         fmul dword ptr [0x1231724]
00b1a194 d95c2478             fstp dword ptr [esp + 0x78]
00b1a198 db442410             fild dword ptr [esp + 0x10]
00b1a19c d80d24172301         fmul dword ptr [0x1231724]
00b1a1a2 d95c247c             fstp dword ptr [esp + 0x7c]
00b1a1a6 d905d8de2201         fld dword ptr [0x122ded8]
00b1a1ac d8e2                 fsub st(2)
00b1a1ae d95c2454             fstp dword ptr [esp + 0x54]
00b1a1b2 d905d8de2201         fld dword ptr [0x122ded8]
00b1a1b8 d8e1                 fsub st(1)
00b1a1ba d95c2458             fstp dword ptr [esp + 0x58]
00b1a1be ddd8                 fstp st(0)
00b1a1c0 ddd8                 fstp st(0)
00b1a1c2 d905d8de2201         fld dword ptr [0x122ded8]
00b1a1c8 d8642478             fsub dword ptr [esp + 0x78]
00b1a1cc d95c245c             fstp dword ptr [esp + 0x5c]
00b1a1d0 d905d8de2201         fld dword ptr [0x122ded8]
00b1a1d6 d864247c             fsub dword ptr [esp + 0x7c]
00b1a1da d9442454             fld dword ptr [esp + 0x54]
00b1a1de d84c2414             fmul dword ptr [esp + 0x14]
00b1a1e2 d9442458             fld dword ptr [esp + 0x58]
00b1a1e6 d84c2414             fmul dword ptr [esp + 0x14]
00b1a1ea d95c2428             fstp dword ptr [esp + 0x28]
00b1a1ee d944245c             fld dword ptr [esp + 0x5c]
00b1a1f2 d84c2414             fmul dword ptr [esp + 0x14]
00b1a1f6 d95c242c             fstp dword ptr [esp + 0x2c]
00b1a1fa d9c9                 fxch st(1)
00b1a1fc d95c2430             fstp dword ptr [esp + 0x30]
00b1a200 d8c2                 fadd st(2)
00b1a202 d95c2470             fstp dword ptr [esp + 0x70]
00b1a206 d9442428             fld dword ptr [esp + 0x28]
00b1a20a d8c1                 fadd st(1)
00b1a20c ddda                 fstp st(2)
00b1a20e ddd8                 fstp st(0)
00b1a210 d944242c             fld dword ptr [esp + 0x2c]
00b1a214 d844244c             fadd dword ptr [esp + 0x4c]
00b1a218 d9442430             fld dword ptr [esp + 0x30]
00b1a21c d8442450             fadd dword ptr [esp + 0x50]
00b1a220 d95c247c             fstp dword ptr [esp + 0x7c]
00b1a224 d9442470             fld dword ptr [esp + 0x70]
00b1a228 d84c2434             fmul dword ptr [esp + 0x34]
00b1a22c d95c2434             fstp dword ptr [esp + 0x34]
00b1a230 d9c9                 fxch st(1)
00b1a232 d84c2438             fmul dword ptr [esp + 0x38]
00b1a236 d95c2438             fstp dword ptr [esp + 0x38]
00b1a23a d84c243c             fmul dword ptr [esp + 0x3c]
00b1a23e d954243c             fst dword ptr [esp + 0x3c]
00b1a242 d9442440             fld dword ptr [esp + 0x40]
00b1a246 d84c247c             fmul dword ptr [esp + 0x7c]
00b1a24a d95c2440             fstp dword ptr [esp + 0x40]
00b1a24e 0fb65646             movzx edx, byte ptr [esi + 0x46]
00b1a252 0fb64645             movzx eax, byte ptr [esi + 0x45]
00b1a256 89542410             mov dword ptr [esp + 0x10], edx
00b1a25a 0fb65644             movzx edx, byte ptr [esi + 0x44]
00b1a25e db442410             fild dword ptr [esp + 0x10]
00b1a262 89442410             mov dword ptr [esp + 0x10], eax
00b1a266 d80d24172301         fmul dword ptr [0x1231724]
00b1a26c db442410             fild dword ptr [esp + 0x10]
00b1a270 89542410             mov dword ptr [esp + 0x10], edx
00b1a274 d80d24172301         fmul dword ptr [0x1231724]
00b1a27a d95c2458             fstp dword ptr [esp + 0x58]
00b1a27e db442410             fild dword ptr [esp + 0x10]
00b1a282 d80d24172301         fmul dword ptr [0x1231724]
00b1a288 d95c245c             fstp dword ptr [esp + 0x5c]
00b1a28c d9442434             fld dword ptr [esp + 0x34]
00b1a290 d8d9                 fcomp st(1)
00b1a292 dfe0                 fnstsw ax
00b1a294 f6c441               test ah, 0x41
00b1a297 0f84be000000         je 0xb1a35b
00b1a29d d95c2434             fstp dword ptr [esp + 0x34]
00b1a2a1 e9b7000000           jmp 0xb1a35d
00b1a2a6 ddd8                 fstp st(0)
00b1a2a8 d944241c             fld dword ptr [esp + 0x1c]
00b1a2ac dcc0                 fadd st(0), st(0)
00b1a2ae d9442424             fld dword ptr [esp + 0x24]
00b1a2b2 d8c9                 fmul st(1)
00b1a2b4 d9442428             fld dword ptr [esp + 0x28]
00b1a2b8 d8ca                 fmul st(2)
00b1a2ba d95c2458             fstp dword ptr [esp + 0x58]
00b1a2be d944242c             fld dword ptr [esp + 0x2c]
00b1a2c2 d8ca                 fmul st(2)
00b1a2c4 d95c245c             fstp dword ptr [esp + 0x5c]
00b1a2c8 d9442430             fld dword ptr [esp + 0x30]
00b1a2cc d8ca                 fmul st(2)
00b1a2ce d95c2460             fstp dword ptr [esp + 0x60]
00b1a2d2 d905d8de2201         fld dword ptr [0x122ded8]
00b1a2d8 d8e2                 fsub st(2)
00b1a2da d9442434             fld dword ptr [esp + 0x34]
00b1a2de d8c9                 fmul st(1)
00b1a2e0 d95c2424             fstp dword ptr [esp + 0x24]
00b1a2e4 d9442438             fld dword ptr [esp + 0x38]
00b1a2e8 d8c9                 fmul st(1)
00b1a2ea d95c2428             fstp dword ptr [esp + 0x28]
00b1a2ee d944243c             fld dword ptr [esp + 0x3c]
00b1a2f2 d8c9                 fmul st(1)
00b1a2f4 d95c242c             fstp dword ptr [esp + 0x2c]
00b1a2f8 d9442440             fld dword ptr [esp + 0x40]
00b1a2fc d8c9                 fmul st(1)
00b1a2fe d95c2430             fstp dword ptr [esp + 0x30]
00b1a302 ddd8                 fstp st(0)
00b1a304 d9442424             fld dword ptr [esp + 0x24]
00b1a308 d8c1                 fadd st(1)
00b1a30a d95c2444             fstp dword ptr [esp + 0x44]
00b1a30e 8b4c2444             mov ecx, dword ptr [esp + 0x44]
00b1a312 894c2434             mov dword ptr [esp + 0x34], ecx
00b1a316 ddd8                 fstp st(0)
00b1a318 ddd8                 fstp st(0)
00b1a31a d9442428             fld dword ptr [esp + 0x28]
00b1a31e d8442458             fadd dword ptr [esp + 0x58]
00b1a322 d95c2448             fstp dword ptr [esp + 0x48]
00b1a326 8b542448             mov edx, dword ptr [esp + 0x48]
00b1a32a d944242c             fld dword ptr [esp + 0x2c]
00b1a32e 89542438             mov dword ptr [esp + 0x38], edx
00b1a332 d844245c             fadd dword ptr [esp + 0x5c]
00b1a336 d95c244c             fstp dword ptr [esp + 0x4c]
00b1a33a 8b44244c             mov eax, dword ptr [esp + 0x4c]
00b1a33e d9442430             fld dword ptr [esp + 0x30]
00b1a342 8944243c             mov dword ptr [esp + 0x3c], eax
00b1a346 d8442460             fadd dword ptr [esp + 0x60]
00b1a34a d95c2450             fstp dword ptr [esp + 0x50]
00b1a34e 8b4c2450             mov ecx, dword ptr [esp + 0x50]
00b1a352 894c2440             mov dword ptr [esp + 0x40], ecx
00b1a356 e912fdffff           jmp 0xb1a06d
00b1a35b ddd8                 fstp st(0)
00b1a35d d9442438             fld dword ptr [esp + 0x38]
00b1a361 d85c2458             fcomp dword ptr [esp + 0x58]
00b1a365 dfe0                 fnstsw ax
00b1a367 f6c441               test ah, 0x41
00b1a36a 7408                 je 0xb1a374
00b1a36c 8b442458             mov eax, dword ptr [esp + 0x58]
00b1a370 89442438             mov dword ptr [esp + 0x38], eax
00b1a374 d85c245c             fcomp dword ptr [esp + 0x5c]
00b1a378 dfe0                 fnstsw ax
00b1a37a f6c441               test ah, 0x41
00b1a37d 7408                 je 0xb1a387
00b1a37f 8b54245c             mov edx, dword ptr [esp + 0x5c]
00b1a383 8954243c             mov dword ptr [esp + 0x3c], edx
00b1a387 f6c101               test cl, 1
00b1a38a 7414                 je 0xb1a3a0
00b1a38c d905d8de2201         fld dword ptr [0x122ded8]
00b1a392 d86640               fsub dword ptr [esi + 0x40]
00b1a395 d84c2414             fmul dword ptr [esp + 0x14]
00b1a399 d84640               fadd dword ptr [esi + 0x40]
00b1a39c d95c241c             fstp dword ptr [esp + 0x1c]
00b1a3a0 8d44241c             lea eax, [esp + 0x1c]
00b1a3a4 50                   push eax
00b1a3a5 8bcf                 mov ecx, edi
00b1a3a7 e8d4ebfdff           call 0xaf8f80
00b1a3ac 8b4e34               mov ecx, dword ptr [esi + 0x34]
00b1a3af 41                   inc ecx
00b1a3b0 894c2410             mov dword ptr [esp + 0x10], ecx
00b1a3b4 db442410             fild dword ptr [esp + 0x10]
00b1a3b8 8b9424b8000000       mov edx, dword ptr [esp + 0xb8]
00b1a3bf 8b8c24c4000000       mov ecx, dword ptr [esp + 0xc4]
00b1a3c6 d80d00632601         fmul dword ptr [0x1266300]
00b1a3cc 6a00                 push 0
00b1a3ce 6a00                 push 0
00b1a3d0 52                   push edx
00b1a3d1 d9c0                 fld st(0)
00b1a3d3 d84e2c               fmul dword ptr [esi + 0x2c]
00b1a3d6 8d4618               lea eax, [esi + 0x18]
00b1a3d9 d9462c               fld dword ptr [esi + 0x2c]
00b1a3dc 50                   push eax
00b1a3dd d94630               fld dword ptr [esi + 0x30]
00b1a3e0 51                   push ecx
00b1a3e1 d8e1                 fsub st(1)
00b1a3e3 8d542478             lea edx, [esp + 0x78]
00b1a3e7 52                   push edx
00b1a3e8 8bce                 mov ecx, esi
00b1a3ea d84c2430             fmul dword ptr [esp + 0x30]
00b1a3ee d8c1                 fadd st(1)
00b1a3f0 d84c2434             fmul dword ptr [esp + 0x34]
00b1a3f4 d99c2498000000       fstp dword ptr [esp + 0x98]
00b1a3fb ddd8                 fstp st(0)
00b1a3fd d9c9                 fxch st(1)
00b1a3ff d84e30               fmul dword ptr [esi + 0x30]
00b1a402 d8e1                 fsub st(1)
00b1a404 d84c2430             fmul dword ptr [esp + 0x30]
00b1a408 d8c1                 fadd st(1)
00b1a40a d84c2434             fmul dword ptr [esp + 0x34]
00b1a40e d95c2434             fstp dword ptr [esp + 0x34]
00b1a412 ddd8                 fstp st(0)
00b1a414 e82782fdff           call 0xaf2640
00b1a419 f6475040             test byte ptr [edi + 0x50], 0x40
00b1a41d 0f85a5000000         jne 0xb1a4c8
00b1a423 8d842484000000       lea eax, [esp + 0x84]
00b1a42a 50                   push eax
00b1a42b 8bcf                 mov ecx, edi
00b1a42d e81e7efdff           call 0xaf2250
00b1a432 d9442464             fld dword ptr [esp + 0x64]
00b1a436 d9442468             fld dword ptr [esp + 0x68]
00b1a43a d984249c000000       fld dword ptr [esp + 0x9c]
00b1a441 d84c246c             fmul dword ptr [esp + 0x6c]
00b1a445 d9842490000000       fld dword ptr [esp + 0x90]
00b1a44c d84c2468             fmul dword ptr [esp + 0x68]
00b1a450 dec1                 faddp st(1)
00b1a452 d9842484000000       fld dword ptr [esp + 0x84]
00b1a459 d84c2464             fmul dword ptr [esp + 0x64]
00b1a45d dec1                 faddp st(1)
00b1a45f d88424a8000000       fadd dword ptr [esp + 0xa8]
00b1a466 d95c2464             fstp dword ptr [esp + 0x64]
00b1a46a d98424a0000000       fld dword ptr [esp + 0xa0]
00b1a471 d84c246c             fmul dword ptr [esp + 0x6c]
00b1a475 d9842494000000       fld dword ptr [esp + 0x94]
00b1a47c d84c2468             fmul dword ptr [esp + 0x68]
00b1a480 dec1                 faddp st(1)
00b1a482 d9842488000000       fld dword ptr [esp + 0x88]
00b1a489 d8cb                 fmul st(3)
00b1a48b dec1                 faddp st(1)
00b1a48d d88424ac000000       fadd dword ptr [esp + 0xac]
00b1a494 d95c2468             fstp dword ptr [esp + 0x68]
00b1a498 d98424a4000000       fld dword ptr [esp + 0xa4]
00b1a49f d84c246c             fmul dword ptr [esp + 0x6c]
00b1a4a3 d9842498000000       fld dword ptr [esp + 0x98]
00b1a4aa d8ca                 fmul st(2)
00b1a4ac dec1                 faddp st(1)
00b1a4ae d984248c000000       fld dword ptr [esp + 0x8c]
00b1a4b5 d8cb                 fmul st(3)
00b1a4b7 dec1                 faddp st(1)
00b1a4b9 d88424b0000000       fadd dword ptr [esp + 0xb0]
00b1a4c0 d95c246c             fstp dword ptr [esp + 0x6c]
00b1a4c4 ddd8                 fstp st(0)
00b1a4c6 ddd8                 fstp st(0)
00b1a4c8 d9442434             fld dword ptr [esp + 0x34]
00b1a4cc d80d14002301         fmul dword ptr [0x1230014]
00b1a4d2 d95c2410             fstp dword ptr [esp + 0x10]
00b1a4d6 d9442410             fld dword ptr [esp + 0x10]
00b1a4da db5c2420             fistp dword ptr [esp + 0x20]
00b1a4de d9442438             fld dword ptr [esp + 0x38]
00b1a4e2 8b442420             mov eax, dword ptr [esp + 0x20]
00b1a4e6 d80d14002301         fmul dword ptr [0x1230014]
00b1a4ec d95c2410             fstp dword ptr [esp + 0x10]
00b1a4f0 d9442410             fld dword ptr [esp + 0x10]
00b1a4f4 db5c2420             fistp dword ptr [esp + 0x20]
00b1a4f8 d944243c             fld dword ptr [esp + 0x3c]
00b1a4fc 8b4c2420             mov ecx, dword ptr [esp + 0x20]
00b1a500 d80d14002301         fmul dword ptr [0x1230014]
00b1a506 894c2414             mov dword ptr [esp + 0x14], ecx
00b1a50a d95c2410             fstp dword ptr [esp + 0x10]
00b1a50e d9442410             fld dword ptr [esp + 0x10]
00b1a512 db5c2420             fistp dword ptr [esp + 0x20]
00b1a516 d9442440             fld dword ptr [esp + 0x40]
00b1a51a 8b7c2420             mov edi, dword ptr [esp + 0x20]
00b1a51e d80d14002301         fmul dword ptr [0x1230014]
00b1a524 8bdf                 mov ebx, edi
00b1a526 d95c2410             fstp dword ptr [esp + 0x10]
00b1a52a d9442410             fld dword ptr [esp + 0x10]
00b1a52e db5c2420             fistp dword ptr [esp + 0x20]
00b1a532 85c0                 test eax, eax
00b1a534 8b6c2420             mov ebp, dword ptr [esp + 0x20]
00b1a538 8bd5                 mov edx, ebp
00b1a53a 7d04                 jge 0xb1a540
00b1a53c 33c0                 xor eax, eax
00b1a53e eb0c                 jmp 0xb1a54c
00b1a540 3dff000000           cmp eax, 0xff
00b1a545 7e05                 jle 0xb1a54c
00b1a547 b8ff000000           mov eax, 0xff
00b1a54c 85c9                 test ecx, ecx
00b1a54e 7d0a                 jge 0xb1a55a
00b1a550 c744241400000000     mov dword ptr [esp + 0x14], 0
00b1a558 eb10                 jmp 0xb1a56a
00b1a55a 81f9ff000000         cmp ecx, 0xff
00b1a560 7e08                 jle 0xb1a56a
00b1a562 c7442414ff000000     mov dword ptr [esp + 0x14], 0xff
00b1a56a 85ff                 test edi, edi
00b1a56c 7d04                 jge 0xb1a572
00b1a56e 33db                 xor ebx, ebx
00b1a570 eb0d                 jmp 0xb1a57f
00b1a572 81ffff000000         cmp edi, 0xff
00b1a578 7e05                 jle 0xb1a57f
00b1a57a bbff000000           mov ebx, 0xff
00b1a57f 85ed                 test ebp, ebp
00b1a581 7d04                 jge 0xb1a587
00b1a583 33d2                 xor edx, edx
00b1a585 eb0d                 jmp 0xb1a594
00b1a587 81fdff000000         cmp ebp, 0xff
00b1a58d 7e05                 jle 0xb1a594
00b1a58f baff000000           mov edx, 0xff
00b1a594 8a4c2414             mov cl, byte ptr [esp + 0x14]
00b1a598 6a00                 push 0
00b1a59a 6aff                 push -1
00b1a59c 6a00                 push 0
00b1a59e 88442426             mov byte ptr [esp + 0x26], al
00b1a5a2 8b442428             mov eax, dword ptr [esp + 0x28]
00b1a5a6 50                   push eax
00b1a5a7 8854242b             mov byte ptr [esp + 0x2b], dl
00b1a5ab 8b9424cc000000       mov edx, dword ptr [esp + 0xcc]
00b1a5b2 8b7a34               mov edi, dword ptr [edx + 0x34]
00b1a5b5 884c2429             mov byte ptr [esp + 0x29], cl
00b1a5b9 8b8c2490000000       mov ecx, dword ptr [esp + 0x90]
00b1a5c0 51                   push ecx
00b1a5c1 8d54242c             lea edx, [esp + 0x2c]
00b1a5c5 52                   push edx
00b1a5c6 8d44247c             lea eax, [esp + 0x7c]
00b1a5ca 50                   push eax
00b1a5cb 6a00                 push 0
00b1a5cd 885c2438             mov byte ptr [esp + 0x38], bl
00b1a5d1 8b1f                 mov ebx, dword ptr [edi]
00b1a5d3 6a00                 push 0
00b1a5d5 8d8c24a8000000       lea ecx, [esp + 0xa8]
00b1a5dc e81f7ab2ff           call 0x642000
00b1a5e1 50                   push eax
00b1a5e2 83c658               add esi, 0x58
00b1a5e5 56                   push esi
00b1a5e6 8bcf                 mov ecx, edi
00b1a5e8 ff535c               call dword ptr [ebx + 0x5c]
00b1a5eb 5d                   pop ebp
00b1a5ec 5b                   pop ebx
00b1a5ed 5f                   pop edi
00b1a5ee 5e                   pop esi
00b1a5ef 81c4a4000000         add esp, 0xa4
00b1a5f5 c21400               ret 0x14
00b1a5f8 8b465c               mov eax, dword ptr [esi + 0x5c]
00b1a5fb 83c658               add esi, 0x58
00b1a5fe 33ff                 xor edi, edi
00b1a600 3bc7                 cmp eax, edi
00b1a602 740e                 je 0xb1a612
00b1a604 83780401             cmp dword ptr [eax + 4], 1
00b1a608 7508                 jne 0xb1a612
00b1a60a 8bc8                 mov ecx, eax
00b1a60c 8b11                 mov edx, dword ptr [ecx]
00b1a60e 56                   push esi
00b1a60f ff520c               call dword ptr [edx + 0xc]
00b1a612 8b4e04               mov ecx, dword ptr [esi + 4]
00b1a615 3bcf                 cmp ecx, edi
00b1a617 740d                 je 0xb1a626
00b1a619 ff4904               dec dword ptr [ecx + 4]
00b1a61c 7505                 jne 0xb1a623
00b1a61e 8b01                 mov eax, dword ptr [ecx]
00b1a620 ff5004               call dword ptr [eax + 4]
00b1a623 897e04               mov dword ptr [esi + 4], edi
00b1a626 897e04               mov dword ptr [esi + 4], edi
00b1a629 5f                   pop edi
00b1a62a 5e                   pop esi
00b1a62b 81c4a4000000         add esp, 0xa4
00b1a631 c21400               ret 0x14
