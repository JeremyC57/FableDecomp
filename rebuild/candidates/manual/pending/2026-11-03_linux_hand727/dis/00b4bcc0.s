00b4bcc0 81ecd0000000         sub esp, 0xd0
00b4bcc6 56                   push esi
00b4bcc7 8bf1                 mov esi, ecx
00b4bcc9 57                   push edi
00b4bcca 8d460c               lea eax, [esi + 0xc]
00b4bccd 50                   push eax
00b4bcce 8d4c245c             lea ecx, [esp + 0x5c]
00b4bcd2 c744241c00000000     mov dword ptr [esp + 0x1c], 0
00b4bcda c744242000000000     mov dword ptr [esp + 0x20], 0
00b4bce2 c744242400000000     mov dword ptr [esp + 0x24], 0
00b4bcea c74424280000803f     mov dword ptr [esp + 0x28], 0x3f800000
00b4bcf2 c744242c00000000     mov dword ptr [esp + 0x2c], 0
00b4bcfa c744243000000000     mov dword ptr [esp + 0x30], 0
00b4bd02 c74424340000803f     mov dword ptr [esp + 0x34], 0x3f800000
00b4bd0a c74424380000803f     mov dword ptr [esp + 0x38], 0x3f800000
00b4bd12 c744243c00000000     mov dword ptr [esp + 0x3c], 0
00b4bd1a c744244000000000     mov dword ptr [esp + 0x40], 0
00b4bd22 c74424440000803f     mov dword ptr [esp + 0x44], 0x3f800000
00b4bd2a c744244800000000     mov dword ptr [esp + 0x48], 0
00b4bd32 e8c90df1ff           call 0xa5cb00
00b4bd37 d944247c             fld dword ptr [esp + 0x7c]
00b4bd3b d84c2420             fmul dword ptr [esp + 0x20]
00b4bd3f d944246c             fld dword ptr [esp + 0x6c]
00b4bd43 d84c241c             fmul dword ptr [esp + 0x1c]
00b4bd47 dec1                 faddp st(1)
00b4bd49 d944245c             fld dword ptr [esp + 0x5c]
00b4bd4d d84c2418             fmul dword ptr [esp + 0x18]
00b4bd51 dec1                 faddp st(1)
00b4bd53 d884248c000000       fadd dword ptr [esp + 0x8c]
00b4bd5a d9842480000000       fld dword ptr [esp + 0x80]
00b4bd61 d84c2420             fmul dword ptr [esp + 0x20]
00b4bd65 d9442470             fld dword ptr [esp + 0x70]
00b4bd69 d84c241c             fmul dword ptr [esp + 0x1c]
00b4bd6d dec1                 faddp st(1)
00b4bd6f d9442460             fld dword ptr [esp + 0x60]
00b4bd73 d84c2418             fmul dword ptr [esp + 0x18]
00b4bd77 dec1                 faddp st(1)
00b4bd79 d8842490000000       fadd dword ptr [esp + 0x90]
00b4bd80 d9842484000000       fld dword ptr [esp + 0x84]
00b4bd87 d84c2420             fmul dword ptr [esp + 0x20]
00b4bd8b d9442474             fld dword ptr [esp + 0x74]
00b4bd8f d84c241c             fmul dword ptr [esp + 0x1c]
00b4bd93 dec1                 faddp st(1)
00b4bd95 d9442464             fld dword ptr [esp + 0x64]
00b4bd99 d84c2418             fmul dword ptr [esp + 0x18]
00b4bd9d dec1                 faddp st(1)
00b4bd9f d8842494000000       fadd dword ptr [esp + 0x94]
00b4bda6 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4bdac d9442478             fld dword ptr [esp + 0x78]
00b4bdb0 d84c2420             fmul dword ptr [esp + 0x20]
00b4bdb4 d9442468             fld dword ptr [esp + 0x68]
00b4bdb8 d84c241c             fmul dword ptr [esp + 0x1c]
00b4bdbc dec1                 faddp st(1)
00b4bdbe d9442458             fld dword ptr [esp + 0x58]
00b4bdc2 d84c2418             fmul dword ptr [esp + 0x18]
00b4bdc6 dec1                 faddp st(1)
00b4bdc8 d8842488000000       fadd dword ptr [esp + 0x88]
00b4bdcf d8c9                 fmul st(1)
00b4bdd1 d95c2418             fstp dword ptr [esp + 0x18]
00b4bdd5 d9c0                 fld st(0)
00b4bdd7 d8cb                 fmul st(3)
00b4bdd9 d95c241c             fstp dword ptr [esp + 0x1c]
00b4bddd d8c9                 fmul st(1)
00b4bddf d95c2420             fstp dword ptr [esp + 0x20]
00b4bde3 ddd8                 fstp st(0)
00b4bde5 ddd8                 fstp st(0)
00b4bde7 d944247c             fld dword ptr [esp + 0x7c]
00b4bdeb d84c242c             fmul dword ptr [esp + 0x2c]
00b4bdef d944246c             fld dword ptr [esp + 0x6c]
00b4bdf3 d84c2428             fmul dword ptr [esp + 0x28]
00b4bdf7 dec1                 faddp st(1)
00b4bdf9 d944245c             fld dword ptr [esp + 0x5c]
00b4bdfd d84c2424             fmul dword ptr [esp + 0x24]
00b4be01 dec1                 faddp st(1)
00b4be03 d884248c000000       fadd dword ptr [esp + 0x8c]
00b4be0a d9842480000000       fld dword ptr [esp + 0x80]
00b4be11 d84c242c             fmul dword ptr [esp + 0x2c]
00b4be15 d9442470             fld dword ptr [esp + 0x70]
00b4be19 d84c2428             fmul dword ptr [esp + 0x28]
00b4be1d dec1                 faddp st(1)
00b4be1f d9442460             fld dword ptr [esp + 0x60]
00b4be23 d84c2424             fmul dword ptr [esp + 0x24]
00b4be27 dec1                 faddp st(1)
00b4be29 d8842490000000       fadd dword ptr [esp + 0x90]
00b4be30 d9842484000000       fld dword ptr [esp + 0x84]
00b4be37 d84c242c             fmul dword ptr [esp + 0x2c]
00b4be3b d9442474             fld dword ptr [esp + 0x74]
00b4be3f d84c2428             fmul dword ptr [esp + 0x28]
00b4be43 dec1                 faddp st(1)
00b4be45 d9442464             fld dword ptr [esp + 0x64]
00b4be49 d84c2424             fmul dword ptr [esp + 0x24]
00b4be4d dec1                 faddp st(1)
00b4be4f d8842494000000       fadd dword ptr [esp + 0x94]
00b4be56 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4be5c d9442478             fld dword ptr [esp + 0x78]
00b4be60 d84c242c             fmul dword ptr [esp + 0x2c]
00b4be64 d9442468             fld dword ptr [esp + 0x68]
00b4be68 d84c2428             fmul dword ptr [esp + 0x28]
00b4be6c dec1                 faddp st(1)
00b4be6e d9442458             fld dword ptr [esp + 0x58]
00b4be72 d84c2424             fmul dword ptr [esp + 0x24]
00b4be76 dec1                 faddp st(1)
00b4be78 d8842488000000       fadd dword ptr [esp + 0x88]
00b4be7f d8c9                 fmul st(1)
00b4be81 d95c2424             fstp dword ptr [esp + 0x24]
00b4be85 d9c0                 fld st(0)
00b4be87 d8cb                 fmul st(3)
00b4be89 d95c2428             fstp dword ptr [esp + 0x28]
00b4be8d d8c9                 fmul st(1)
00b4be8f d95c242c             fstp dword ptr [esp + 0x2c]
00b4be93 ddd8                 fstp st(0)
00b4be95 ddd8                 fstp st(0)
00b4be97 d944247c             fld dword ptr [esp + 0x7c]
00b4be9b d84c2438             fmul dword ptr [esp + 0x38]
00b4be9f d944246c             fld dword ptr [esp + 0x6c]
00b4bea3 d84c2434             fmul dword ptr [esp + 0x34]
00b4bea7 dec1                 faddp st(1)
00b4bea9 d944245c             fld dword ptr [esp + 0x5c]
00b4bead d84c2430             fmul dword ptr [esp + 0x30]
00b4beb1 dec1                 faddp st(1)
00b4beb3 d884248c000000       fadd dword ptr [esp + 0x8c]
00b4beba d9842480000000       fld dword ptr [esp + 0x80]
00b4bec1 d84c2438             fmul dword ptr [esp + 0x38]
00b4bec5 d9442470             fld dword ptr [esp + 0x70]
00b4bec9 d84c2434             fmul dword ptr [esp + 0x34]
00b4becd dec1                 faddp st(1)
00b4becf d9442460             fld dword ptr [esp + 0x60]
00b4bed3 d84c2430             fmul dword ptr [esp + 0x30]
00b4bed7 dec1                 faddp st(1)
00b4bed9 d8842490000000       fadd dword ptr [esp + 0x90]
00b4bee0 d9842484000000       fld dword ptr [esp + 0x84]
00b4bee7 d84c2438             fmul dword ptr [esp + 0x38]
00b4beeb d9442474             fld dword ptr [esp + 0x74]
00b4beef d84c2434             fmul dword ptr [esp + 0x34]
00b4bef3 dec1                 faddp st(1)
00b4bef5 d9442464             fld dword ptr [esp + 0x64]
00b4bef9 d84c2430             fmul dword ptr [esp + 0x30]
00b4befd dec1                 faddp st(1)
00b4beff d8842494000000       fadd dword ptr [esp + 0x94]
00b4bf06 d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4bf0c d9442478             fld dword ptr [esp + 0x78]
00b4bf10 d84c2438             fmul dword ptr [esp + 0x38]
00b4bf14 d9442468             fld dword ptr [esp + 0x68]
00b4bf18 d84c2434             fmul dword ptr [esp + 0x34]
00b4bf1c dec1                 faddp st(1)
00b4bf1e d9442458             fld dword ptr [esp + 0x58]
00b4bf22 d84c2430             fmul dword ptr [esp + 0x30]
00b4bf26 dec1                 faddp st(1)
00b4bf28 d8842488000000       fadd dword ptr [esp + 0x88]
00b4bf2f d8c9                 fmul st(1)
00b4bf31 d95c2430             fstp dword ptr [esp + 0x30]
00b4bf35 d9c0                 fld st(0)
00b4bf37 d8cb                 fmul st(3)
00b4bf39 d95c2434             fstp dword ptr [esp + 0x34]
00b4bf3d d8c9                 fmul st(1)
00b4bf3f d95c2438             fstp dword ptr [esp + 0x38]
00b4bf43 ddd8                 fstp st(0)
00b4bf45 ddd8                 fstp st(0)
00b4bf47 d944247c             fld dword ptr [esp + 0x7c]
00b4bf4b d84c2444             fmul dword ptr [esp + 0x44]
00b4bf4f d944246c             fld dword ptr [esp + 0x6c]
00b4bf53 33c0                 xor eax, eax
00b4bf55 d84c2440             fmul dword ptr [esp + 0x40]
00b4bf59 89869c010000         mov dword ptr [esi + 0x19c], eax
00b4bf5f 8886b0010000         mov byte ptr [esi + 0x1b0], al
00b4bf65 898634020000         mov dword ptr [esi + 0x234], eax
00b4bf6b dec1                 faddp st(1)
00b4bf6d 888648020000         mov byte ptr [esi + 0x248], al
00b4bf73 d944245c             fld dword ptr [esp + 0x5c]
00b4bf77 8dbe1c010000         lea edi, [esi + 0x11c]
00b4bf7d d84c243c             fmul dword ptr [esp + 0x3c]
00b4bf81 8d4c2408             lea ecx, [esp + 8]
00b4bf85 51                   push ecx
00b4bf86 8d542428             lea edx, [esp + 0x28]
00b4bf8a dec1                 faddp st(1)
00b4bf8c 52                   push edx
00b4bf8d 8d442420             lea eax, [esp + 0x20]
00b4bf91 50                   push eax
00b4bf92 d8842498000000       fadd dword ptr [esp + 0x98]
00b4bf99 8d4c2454             lea ecx, [esp + 0x54]
00b4bf9d d984248c000000       fld dword ptr [esp + 0x8c]
00b4bfa4 d84c2450             fmul dword ptr [esp + 0x50]
00b4bfa8 d944247c             fld dword ptr [esp + 0x7c]
00b4bfac d84c244c             fmul dword ptr [esp + 0x4c]
00b4bfb0 dec1                 faddp st(1)
00b4bfb2 d944246c             fld dword ptr [esp + 0x6c]
00b4bfb6 d84c2448             fmul dword ptr [esp + 0x48]
00b4bfba dec1                 faddp st(1)
00b4bfbc d884249c000000       fadd dword ptr [esp + 0x9c]
00b4bfc3 d9842490000000       fld dword ptr [esp + 0x90]
00b4bfca d84c2450             fmul dword ptr [esp + 0x50]
00b4bfce d9842480000000       fld dword ptr [esp + 0x80]
00b4bfd5 d84c244c             fmul dword ptr [esp + 0x4c]
00b4bfd9 dec1                 faddp st(1)
00b4bfdb d9442470             fld dword ptr [esp + 0x70]
00b4bfdf d84c2448             fmul dword ptr [esp + 0x48]
00b4bfe3 dec1                 faddp st(1)
00b4bfe5 d88424a0000000       fadd dword ptr [esp + 0xa0]
00b4bfec d83dd8de2201         fdivr dword ptr [0x122ded8]
00b4bff2 d9842484000000       fld dword ptr [esp + 0x84]
00b4bff9 d84c2450             fmul dword ptr [esp + 0x50]
00b4bffd d9442474             fld dword ptr [esp + 0x74]
00b4c001 d84c244c             fmul dword ptr [esp + 0x4c]
00b4c005 dec1                 faddp st(1)
00b4c007 d9442464             fld dword ptr [esp + 0x64]
00b4c00b d84c2448             fmul dword ptr [esp + 0x48]
00b4c00f dec1                 faddp st(1)
00b4c011 d8842494000000       fadd dword ptr [esp + 0x94]
00b4c018 d8c9                 fmul st(1)
00b4c01a d95c2448             fstp dword ptr [esp + 0x48]
00b4c01e d9c0                 fld st(0)
00b4c020 d8cb                 fmul st(3)
00b4c022 d95c244c             fstp dword ptr [esp + 0x4c]
00b4c026 d8c9                 fmul st(1)
00b4c028 d95c2450             fstp dword ptr [esp + 0x50]
00b4c02c ddd8                 fstp st(0)
00b4c02e ddd8                 fstp st(0)
00b4c030 d986fc000000         fld dword ptr [esi + 0xfc]
00b4c036 d9c0                 fld st(0)
00b4c038 d88e04010000         fmul dword ptr [esi + 0x104]
00b4c03e d9c1                 fld st(1)
00b4c040 d88e08010000         fmul dword ptr [esi + 0x108]
00b4c046 d95c2458             fstp dword ptr [esp + 0x58]
00b4c04a d9c9                 fxch st(1)
00b4c04c d88e0c010000         fmul dword ptr [esi + 0x10c]
00b4c052 d95c245c             fstp dword ptr [esp + 0x5c]
00b4c056 d8442424             fadd dword ptr [esp + 0x24]
00b4c05a d95c2414             fstp dword ptr [esp + 0x14]
00b4c05e d9442458             fld dword ptr [esp + 0x58]
00b4c062 d8442428             fadd dword ptr [esp + 0x28]
00b4c066 d95c2418             fstp dword ptr [esp + 0x18]
00b4c06a d944245c             fld dword ptr [esp + 0x5c]
00b4c06e d844242c             fadd dword ptr [esp + 0x2c]
00b4c072 d95c241c             fstp dword ptr [esp + 0x1c]
00b4c076 e8f560efff           call 0xa42170
00b4c07b 8b8f80000000         mov ecx, dword ptr [edi + 0x80]
00b4c081 8b542448             mov edx, dword ptr [esp + 0x48]
00b4c085 c1e104               shl ecx, 4
00b4c088 03cf                 add ecx, edi
00b4c08a 8911                 mov dword ptr [ecx], edx
00b4c08c 8b44244c             mov eax, dword ptr [esp + 0x4c]
00b4c090 894104               mov dword ptr [ecx + 4], eax
00b4c093 8b542450             mov edx, dword ptr [esp + 0x50]
00b4c097 895108               mov dword ptr [ecx + 8], edx
00b4c09a 8b442454             mov eax, dword ptr [esp + 0x54]
00b4c09e 89410c               mov dword ptr [ecx + 0xc], eax
00b4c0a1 8b8780000000         mov eax, dword ptr [edi + 0x80]
00b4c0a7 40                   inc eax
00b4c0a8 898780000000         mov dword ptr [edi + 0x80], eax
00b4c0ae d986fc000000         fld dword ptr [esi + 0xfc]
00b4c0b4 d9c0                 fld st(0)
00b4c0b6 8d4c2448             lea ecx, [esp + 0x48]
00b4c0ba d88e04010000         fmul dword ptr [esi + 0x104]
00b4c0c0 51                   push ecx
00b4c0c1 d9c1                 fld st(1)
00b4c0c3 8d542434             lea edx, [esp + 0x34]
00b4c0c7 d88e08010000         fmul dword ptr [esi + 0x108]
00b4c0cd 52                   push edx
00b4c0ce 8d44242c             lea eax, [esp + 0x2c]
00b4c0d2 50                   push eax
00b4c0d3 d95c2418             fstp dword ptr [esp + 0x18]
00b4c0d7 8d4c2414             lea ecx, [esp + 0x14]
00b4c0db d9c9                 fxch st(1)
00b4c0dd d88e0c010000         fmul dword ptr [esi + 0x10c]
00b4c0e3 d95c241c             fstp dword ptr [esp + 0x1c]
00b4c0e7 d8442430             fadd dword ptr [esp + 0x30]
00b4c0eb d95c2454             fstp dword ptr [esp + 0x54]
00b4c0ef d9442418             fld dword ptr [esp + 0x18]
00b4c0f3 d8442434             fadd dword ptr [esp + 0x34]
00b4c0f7 d95c2458             fstp dword ptr [esp + 0x58]
00b4c0fb d944241c             fld dword ptr [esp + 0x1c]
00b4c0ff d8442438             fadd dword ptr [esp + 0x38]
00b4c103 d95c245c             fstp dword ptr [esp + 0x5c]
00b4c107 e86460efff           call 0xa42170
00b4c10c 8b8f80000000         mov ecx, dword ptr [edi + 0x80]
00b4c112 8b542408             mov edx, dword ptr [esp + 8]
00b4c116 c1e104               shl ecx, 4
00b4c119 03cf                 add ecx, edi
00b4c11b 8911                 mov dword ptr [ecx], edx
00b4c11d 8b44240c             mov eax, dword ptr [esp + 0xc]
00b4c121 894104               mov dword ptr [ecx + 4], eax
00b4c124 8b542410             mov edx, dword ptr [esp + 0x10]
00b4c128 895108               mov dword ptr [ecx + 8], edx
00b4c12b 8b442414             mov eax, dword ptr [esp + 0x14]
00b4c12f 89410c               mov dword ptr [ecx + 0xc], eax
00b4c132 8b8780000000         mov eax, dword ptr [edi + 0x80]
00b4c138 40                   inc eax
00b4c139 898780000000         mov dword ptr [edi + 0x80], eax
00b4c13f d986fc000000         fld dword ptr [esi + 0xfc]
00b4c145 d9c0                 fld st(0)
00b4c147 8d4c2448             lea ecx, [esp + 0x48]
00b4c14b d88e04010000         fmul dword ptr [esi + 0x104]
00b4c151 51                   push ecx
00b4c152 d9c1                 fld st(1)
00b4c154 8d542440             lea edx, [esp + 0x40]
00b4c158 d88e08010000         fmul dword ptr [esi + 0x108]
00b4c15e 52                   push edx
00b4c15f 8d442438             lea eax, [esp + 0x38]
00b4c163 50                   push eax
00b4c164 d95c2418             fstp dword ptr [esp + 0x18]
00b4c168 8d4c2414             lea ecx, [esp + 0x14]
00b4c16c d9c9                 fxch st(1)
00b4c16e d88e0c010000         fmul dword ptr [esi + 0x10c]
00b4c174 d95c241c             fstp dword ptr [esp + 0x1c]
00b4c178 d844243c             fadd dword ptr [esp + 0x3c]
00b4c17c d95c2454             fstp dword ptr [esp + 0x54]
00b4c180 d9442418             fld dword ptr [esp + 0x18]
00b4c184 d8442440             fadd dword ptr [esp + 0x40]
00b4c188 d95c2458             fstp dword ptr [esp + 0x58]
00b4c18c d944241c             fld dword ptr [esp + 0x1c]
00b4c190 d8442444             fadd dword ptr [esp + 0x44]
00b4c194 d95c245c             fstp dword ptr [esp + 0x5c]
00b4c198 e8d35fefff           call 0xa42170
00b4c19d 8b8f80000000         mov ecx, dword ptr [edi + 0x80]
00b4c1a3 8b542408             mov edx, dword ptr [esp + 8]
00b4c1a7 c1e104               shl ecx, 4
00b4c1aa 03cf                 add ecx, edi
00b4c1ac 8911                 mov dword ptr [ecx], edx
00b4c1ae 8b44240c             mov eax, dword ptr [esp + 0xc]
00b4c1b2 894104               mov dword ptr [ecx + 4], eax
00b4c1b5 8b542410             mov edx, dword ptr [esp + 0x10]
00b4c1b9 895108               mov dword ptr [ecx + 8], edx
00b4c1bc 8b442414             mov eax, dword ptr [esp + 0x14]
00b4c1c0 89410c               mov dword ptr [ecx + 0xc], eax
00b4c1c3 8b8780000000         mov eax, dword ptr [edi + 0x80]
00b4c1c9 40                   inc eax
00b4c1ca 898780000000         mov dword ptr [edi + 0x80], eax
00b4c1d0 d986fc000000         fld dword ptr [esi + 0xfc]
00b4c1d6 d9c0                 fld st(0)
00b4c1d8 8d4c2448             lea ecx, [esp + 0x48]
00b4c1dc d88e04010000         fmul dword ptr [esi + 0x104]
00b4c1e2 51                   push ecx
00b4c1e3 d9c1                 fld st(1)
00b4c1e5 8d54241c             lea edx, [esp + 0x1c]
00b4c1e9 d88e08010000         fmul dword ptr [esi + 0x108]
00b4c1ef 52                   push edx
00b4c1f0 8d442444             lea eax, [esp + 0x44]
00b4c1f4 50                   push eax
00b4c1f5 d95c2418             fstp dword ptr [esp + 0x18]
00b4c1f9 8d4c2414             lea ecx, [esp + 0x14]
00b4c1fd d9c9                 fxch st(1)
00b4c1ff d88e0c010000         fmul dword ptr [esi + 0x10c]
00b4c205 d95c241c             fstp dword ptr [esp + 0x1c]
00b4c209 d8442448             fadd dword ptr [esp + 0x48]
00b4c20d d95c2454             fstp dword ptr [esp + 0x54]
00b4c211 d9442418             fld dword ptr [esp + 0x18]
00b4c215 d844244c             fadd dword ptr [esp + 0x4c]
00b4c219 d95c2458             fstp dword ptr [esp + 0x58]
00b4c21d d944241c             fld dword ptr [esp + 0x1c]
00b4c221 d8442450             fadd dword ptr [esp + 0x50]
00b4c225 d95c245c             fstp dword ptr [esp + 0x5c]
00b4c229 e8425fefff           call 0xa42170
00b4c22e 8b8f80000000         mov ecx, dword ptr [edi + 0x80]
00b4c234 8b542408             mov edx, dword ptr [esp + 8]
00b4c238 c1e104               shl ecx, 4
00b4c23b 03cf                 add ecx, edi
00b4c23d 8911                 mov dword ptr [ecx], edx
00b4c23f 8b44240c             mov eax, dword ptr [esp + 0xc]
00b4c243 894104               mov dword ptr [ecx + 4], eax
00b4c246 8b542410             mov edx, dword ptr [esp + 0x10]
00b4c24a 895108               mov dword ptr [ecx + 8], edx
00b4c24d 8b442414             mov eax, dword ptr [esp + 0x14]
00b4c251 89410c               mov dword ptr [ecx + 0xc], eax
00b4c254 8b8c24dc000000       mov ecx, dword ptr [esp + 0xdc]
00b4c25b ff8780000000         inc dword ptr [edi + 0x80]
00b4c261 8b91c0010000         mov edx, dword ptr [ecx + 0x1c0]
00b4c267 8b81c4010000         mov eax, dword ptr [ecx + 0x1c4]
00b4c26d 8984249c000000       mov dword ptr [esp + 0x9c], eax
00b4c274 8b81cc010000         mov eax, dword ptr [ecx + 0x1cc]
00b4c27a 89942498000000       mov dword ptr [esp + 0x98], edx
00b4c281 8b91c8010000         mov edx, dword ptr [ecx + 0x1c8]
00b4c287 899424a0000000       mov dword ptr [esp + 0xa0], edx
00b4c28e d98424a0000000       fld dword ptr [esp + 0xa0]
00b4c295 8b91d0010000         mov edx, dword ptr [ecx + 0x1d0]
00b4c29b d88e0c010000         fmul dword ptr [esi + 0x10c]
00b4c2a1 d984249c000000       fld dword ptr [esp + 0x9c]
00b4c2a8 898424a4000000       mov dword ptr [esp + 0xa4], eax
00b4c2af 8b81d4010000         mov eax, dword ptr [ecx + 0x1d4]
00b4c2b5 899424a8000000       mov dword ptr [esp + 0xa8], edx
00b4c2bc 8b91d8010000         mov edx, dword ptr [ecx + 0x1d8]
00b4c2c2 898424ac000000       mov dword ptr [esp + 0xac], eax
00b4c2c9 8b81dc010000         mov eax, dword ptr [ecx + 0x1dc]
00b4c2cf 899424b0000000       mov dword ptr [esp + 0xb0], edx
00b4c2d6 8b91e0010000         mov edx, dword ptr [ecx + 0x1e0]
00b4c2dc 898424b4000000       mov dword ptr [esp + 0xb4], eax
00b4c2e3 8b81e4010000         mov eax, dword ptr [ecx + 0x1e4]
00b4c2e9 899424b8000000       mov dword ptr [esp + 0xb8], edx
00b4c2f0 8b91e8010000         mov edx, dword ptr [ecx + 0x1e8]
00b4c2f6 898424bc000000       mov dword ptr [esp + 0xbc], eax
00b4c2fd 8b81ec010000         mov eax, dword ptr [ecx + 0x1ec]
00b4c303 899424c0000000       mov dword ptr [esp + 0xc0], edx
00b4c30a 8b91f0010000         mov edx, dword ptr [ecx + 0x1f0]
00b4c310 898424c4000000       mov dword ptr [esp + 0xc4], eax
00b4c317 8b81f4010000         mov eax, dword ptr [ecx + 0x1f4]
00b4c31d 899424c8000000       mov dword ptr [esp + 0xc8], edx
00b4c324 8b91f8010000         mov edx, dword ptr [ecx + 0x1f8]
00b4c32a 898424cc000000       mov dword ptr [esp + 0xcc], eax
00b4c331 8b81fc010000         mov eax, dword ptr [ecx + 0x1fc]
00b4c337 899424d0000000       mov dword ptr [esp + 0xd0], edx
00b4c33e 898424d4000000       mov dword ptr [esp + 0xd4], eax
00b4c345 d88e08010000         fmul dword ptr [esi + 0x108]
00b4c34b dec1                 faddp st(1)
00b4c34d d9842498000000       fld dword ptr [esp + 0x98]
00b4c354 d88e04010000         fmul dword ptr [esi + 0x104]
00b4c35a dec1                 faddp st(1)
00b4c35c d81ddcde2201         fcomp dword ptr [0x122dedc]
00b4c362 dfe0                 fnstsw ax
00b4c364 f6c441               test ah, 0x41
00b4c367 7538                 jne 0xb4c3a1
00b4c369 8b9780000000         mov edx, dword ptr [edi + 0x80]
00b4c36f 8b842498000000       mov eax, dword ptr [esp + 0x98]
00b4c376 c1e204               shl edx, 4
00b4c379 03d7                 add edx, edi
00b4c37b 8902                 mov dword ptr [edx], eax
00b4c37d 8b84249c000000       mov eax, dword ptr [esp + 0x9c]
00b4c384 894204               mov dword ptr [edx + 4], eax
00b4c387 8b8424a0000000       mov eax, dword ptr [esp + 0xa0]
00b4c38e 894208               mov dword ptr [edx + 8], eax
00b4c391 8b8424a4000000       mov eax, dword ptr [esp + 0xa4]
00b4c398 89420c               mov dword ptr [edx + 0xc], eax
00b4c39b ff8780000000         inc dword ptr [edi + 0x80]
00b4c3a1 d98424b0000000       fld dword ptr [esp + 0xb0]
00b4c3a8 d88e0c010000         fmul dword ptr [esi + 0x10c]
00b4c3ae d98424ac000000       fld dword ptr [esp + 0xac]
00b4c3b5 d88e08010000         fmul dword ptr [esi + 0x108]
00b4c3bb dec1                 faddp st(1)
00b4c3bd d98424a8000000       fld dword ptr [esp + 0xa8]
00b4c3c4 d88e04010000         fmul dword ptr [esi + 0x104]
00b4c3ca dec1                 faddp st(1)
00b4c3cc d81ddcde2201         fcomp dword ptr [0x122dedc]
00b4c3d2 dfe0                 fnstsw ax
00b4c3d4 f6c441               test ah, 0x41
00b4c3d7 7538                 jne 0xb4c411
00b4c3d9 8b9780000000         mov edx, dword ptr [edi + 0x80]
00b4c3df 8b8424a8000000       mov eax, dword ptr [esp + 0xa8]
00b4c3e6 c1e204               shl edx, 4
00b4c3e9 03d7                 add edx, edi
00b4c3eb 8902                 mov dword ptr [edx], eax
00b4c3ed 8b8424ac000000       mov eax, dword ptr [esp + 0xac]
00b4c3f4 894204               mov dword ptr [edx + 4], eax
00b4c3f7 8b8424b0000000       mov eax, dword ptr [esp + 0xb0]
00b4c3fe 894208               mov dword ptr [edx + 8], eax
00b4c401 8b8424b4000000       mov eax, dword ptr [esp + 0xb4]
00b4c408 89420c               mov dword ptr [edx + 0xc], eax
00b4c40b ff8780000000         inc dword ptr [edi + 0x80]
00b4c411 d98424c0000000       fld dword ptr [esp + 0xc0]
00b4c418 d88e0c010000         fmul dword ptr [esi + 0x10c]
00b4c41e d98424bc000000       fld dword ptr [esp + 0xbc]
00b4c425 d88e08010000         fmul dword ptr [esi + 0x108]
00b4c42b dec1                 faddp st(1)
00b4c42d d98424b8000000       fld dword ptr [esp + 0xb8]
00b4c434 d88e04010000         fmul dword ptr [esi + 0x104]
00b4c43a dec1                 faddp st(1)
00b4c43c d81ddcde2201         fcomp dword ptr [0x122dedc]
00b4c442 dfe0                 fnstsw ax
00b4c444 f6c441               test ah, 0x41
00b4c447 7538                 jne 0xb4c481
00b4c449 8b9780000000         mov edx, dword ptr [edi + 0x80]
00b4c44f 8b8424b8000000       mov eax, dword ptr [esp + 0xb8]
00b4c456 c1e204               shl edx, 4
00b4c459 03d7                 add edx, edi
00b4c45b 8902                 mov dword ptr [edx], eax
00b4c45d 8b8424bc000000       mov eax, dword ptr [esp + 0xbc]
00b4c464 894204               mov dword ptr [edx + 4], eax
00b4c467 8b8424c0000000       mov eax, dword ptr [esp + 0xc0]
00b4c46e 894208               mov dword ptr [edx + 8], eax
00b4c471 8b8424c4000000       mov eax, dword ptr [esp + 0xc4]
00b4c478 89420c               mov dword ptr [edx + 0xc], eax
00b4c47b ff8780000000         inc dword ptr [edi + 0x80]
00b4c481 d98424d0000000       fld dword ptr [esp + 0xd0]
00b4c488 d88e0c010000         fmul dword ptr [esi + 0x10c]
00b4c48e d98424cc000000       fld dword ptr [esp + 0xcc]
00b4c495 d88e08010000         fmul dword ptr [esi + 0x108]
00b4c49b dec1                 faddp st(1)
00b4c49d d98424c8000000       fld dword ptr [esp + 0xc8]
00b4c4a4 d88e04010000         fmul dword ptr [esi + 0x104]
00b4c4aa dec1                 faddp st(1)
00b4c4ac d81ddcde2201         fcomp dword ptr [0x122dedc]
00b4c4b2 dfe0                 fnstsw ax
00b4c4b4 f6c441               test ah, 0x41
00b4c4b7 7538                 jne 0xb4c4f1
00b4c4b9 8b9780000000         mov edx, dword ptr [edi + 0x80]
00b4c4bf 8b8424c8000000       mov eax, dword ptr [esp + 0xc8]
00b4c4c6 c1e204               shl edx, 4
00b4c4c9 03d7                 add edx, edi
00b4c4cb 8902                 mov dword ptr [edx], eax
00b4c4cd 8b8424cc000000       mov eax, dword ptr [esp + 0xcc]
00b4c4d4 894204               mov dword ptr [edx + 4], eax
00b4c4d7 8b8424d0000000       mov eax, dword ptr [esp + 0xd0]
00b4c4de 894208               mov dword ptr [edx + 8], eax
00b4c4e1 8b8424d4000000       mov eax, dword ptr [esp + 0xd4]
00b4c4e8 89420c               mov dword ptr [edx + 0xc], eax
00b4c4eb ff8780000000         inc dword ptr [edi + 0x80]
00b4c4f1 8b5154               mov edx, dword ptr [ecx + 0x54]
00b4c4f4 8b4158               mov eax, dword ptr [ecx + 0x58]
00b4c4f7 8b495c               mov ecx, dword ptr [ecx + 0x5c]
00b4c4fa 89542408             mov dword ptr [esp + 8], edx
00b4c4fe 8b9600010000         mov edx, dword ptr [esi + 0x100]
00b4c504 8944240c             mov dword ptr [esp + 0xc], eax
00b4c508 894c2410             mov dword ptr [esp + 0x10], ecx
00b4c50c 8b4c2408             mov ecx, dword ptr [esp + 8]
00b4c510 8d8638020000         lea eax, [esi + 0x238]
00b4c516 8908                 mov dword ptr [eax], ecx
00b4c518 8b4c2410             mov ecx, dword ptr [esp + 0x10]
00b4c51c 89542414             mov dword ptr [esp + 0x14], edx
00b4c520 8b54240c             mov edx, dword ptr [esp + 0xc]
00b4c524 895004               mov dword ptr [eax + 4], edx
00b4c527 8b542414             mov edx, dword ptr [esp + 0x14]
00b4c52b 894808               mov dword ptr [eax + 8], ecx
00b4c52e 5f                   pop edi
00b4c52f c6864802000001       mov byte ptr [esi + 0x248], 1
00b4c536 89500c               mov dword ptr [eax + 0xc], edx
00b4c539 5e                   pop esi
00b4c53a 81c4d0000000         add esp, 0xd0
00b4c540 c20400               ret 4
