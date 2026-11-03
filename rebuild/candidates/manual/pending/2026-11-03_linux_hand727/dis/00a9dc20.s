00a9dc20 83ec60               sub esp, 0x60
00a9dc23 84c0                 test al, al
00a9dc25 0f84b1020000         je 0xa9dedc
00a9dc2b 8b442464             mov eax, dword ptr [esp + 0x64]
00a9dc2f 50                   push eax
00a9dc30 8b44246c             mov eax, dword ptr [esp + 0x6c]
00a9dc34 51                   push ecx
00a9dc35 8b08                 mov ecx, dword ptr [eax]
00a9dc37 8d542438             lea edx, [esp + 0x38]
00a9dc3b 52                   push edx
00a9dc3c e8afe5faff           call 0xa4c1f0
00a9dc41 d9442434             fld dword ptr [esp + 0x34]
00a9dc45 d84e04               fmul dword ptr [esi + 4]
00a9dc48 c74424080000803f     mov dword ptr [esp + 8], 0x3f800000
00a9dc50 d9442430             fld dword ptr [esp + 0x30]
00a9dc54 d80e                 fmul dword ptr [esi]
00a9dc56 dec1                 faddp st(1)
00a9dc58 d9442438             fld dword ptr [esp + 0x38]
00a9dc5c d84e08               fmul dword ptr [esi + 8]
00a9dc5f dec1                 faddp st(1)
00a9dc61 d944243c             fld dword ptr [esp + 0x3c]
00a9dc65 d84e0c               fmul dword ptr [esi + 0xc]
00a9dc68 dec1                 faddp st(1)
00a9dc6a d95c2464             fstp dword ptr [esp + 0x64]
00a9dc6e d9442464             fld dword ptr [esp + 0x64]
00a9dc72 d81ddcde2201         fcomp dword ptr [0x122dedc]
00a9dc78 dfe0                 fnstsw ax
00a9dc7a f6c405               test ah, 5
00a9dc7d 7a12                 jp 0xa9dc91
00a9dc7f d9442464             fld dword ptr [esp + 0x64]
00a9dc83 c7442408000080bf     mov dword ptr [esp + 8], 0xbf800000
00a9dc8b d9e0                 fchs 
00a9dc8d d95c2464             fstp dword ptr [esp + 0x64]
00a9dc91 d905d8de2201         fld dword ptr [0x122ded8]
00a9dc97 d8642464             fsub dword ptr [esp + 0x64]
00a9dc9b d81d5ca12501         fcomp dword ptr [0x125a15c]
00a9dca1 dfe0                 fnstsw ax
00a9dca3 f6c441               test ah, 0x41
00a9dca6 0f852b010000         jne 0xa9ddd7
00a9dcac d9442464             fld dword ptr [esp + 0x64]
00a9dcb0 d805d8de2201         fadd dword ptr [0x122ded8]
00a9dcb6 d80d1ccd2901         fmul dword ptr [0x129cd1c]
00a9dcbc d9542468             fst dword ptr [esp + 0x68]
00a9dcc0 d8259cf52201         fsub dword ptr [0x122f59c]
00a9dcc6 d95c2404             fstp dword ptr [esp + 4]
00a9dcca d9442404             fld dword ptr [esp + 4]
00a9dcce db1c24               fistp dword ptr [esp]
00a9dcd1 db0424               fild dword ptr [esp]
00a9dcd4 8b0424               mov eax, dword ptr [esp]
00a9dcd7 d86c2468             fsubr dword ptr [esp + 0x68]
00a9dcdb d905d8de2201         fld dword ptr [0x122ded8]
00a9dce1 d8e1                 fsub st(1)
00a9dce3 d80c8530b53c01       fmul dword ptr [eax*4 + 0x13cb530]
00a9dcea d9c9                 fxch st(1)
00a9dcec d80c8534b53c01       fmul dword ptr [eax*4 + 0x13cb534]
00a9dcf3 dec1                 faddp st(1)
00a9dcf5 d80d087f2301         fmul dword ptr [0x1237f08]
00a9dcfb d91c24               fstp dword ptr [esp]
00a9dcfe d9442464             fld dword ptr [esp + 0x64]
00a9dd02 d84c2464             fmul dword ptr [esp + 0x64]
00a9dd06 d82dd8de2201         fsubr dword ptr [0x122ded8]
00a9dd0c d9fa                 fsqrt 
00a9dd0e d95c240c             fstp dword ptr [esp + 0xc]
00a9dd12 d905d8de2201         fld dword ptr [0x122ded8]
00a9dd18 d864246c             fsub dword ptr [esp + 0x6c]
00a9dd1c d80c24               fmul dword ptr [esp]
00a9dd1f d80d10002301         fmul dword ptr [0x1230010]
00a9dd25 d9542464             fst dword ptr [esp + 0x64]
00a9dd29 d8259cf52201         fsub dword ptr [0x122f59c]
00a9dd2f d95c2404             fstp dword ptr [esp + 4]
00a9dd33 d9442404             fld dword ptr [esp + 4]
00a9dd37 db5c2468             fistp dword ptr [esp + 0x68]
00a9dd3b db442468             fild dword ptr [esp + 0x68]
00a9dd3f 8b442468             mov eax, dword ptr [esp + 0x68]
00a9dd43 0500ffffff           add eax, 0xffffff00
00a9dd48 25ff030000           and eax, 0x3ff
00a9dd4d d86c2464             fsubr dword ptr [esp + 0x64]
00a9dd51 d905d8de2201         fld dword ptr [0x122ded8]
00a9dd57 d874240c             fdiv dword ptr [esp + 0xc]
00a9dd5b d95c2404             fstp dword ptr [esp + 4]
00a9dd5f d905d8de2201         fld dword ptr [0x122ded8]
00a9dd65 d8e1                 fsub st(1)
00a9dd67 d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00a9dd6e d9c9                 fxch st(1)
00a9dd70 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00a9dd77 dec1                 faddp st(1)
00a9dd79 d84c2404             fmul dword ptr [esp + 4]
00a9dd7d d95c2464             fstp dword ptr [esp + 0x64]
00a9dd81 d90424               fld dword ptr [esp]
00a9dd84 d84c246c             fmul dword ptr [esp + 0x6c]
00a9dd88 d80d10002301         fmul dword ptr [0x1230010]
00a9dd8e d9542468             fst dword ptr [esp + 0x68]
00a9dd92 d8259cf52201         fsub dword ptr [0x122f59c]
00a9dd98 d95c240c             fstp dword ptr [esp + 0xc]
00a9dd9c d944240c             fld dword ptr [esp + 0xc]
00a9dda0 db1c24               fistp dword ptr [esp]
00a9dda3 db0424               fild dword ptr [esp]
00a9dda6 8b0424               mov eax, dword ptr [esp]
00a9dda9 0500ffffff           add eax, 0xffffff00
00a9ddae 25ff030000           and eax, 0x3ff
00a9ddb3 d86c2468             fsubr dword ptr [esp + 0x68]
00a9ddb7 d905d8de2201         fld dword ptr [0x122ded8]
00a9ddbd d8e1                 fsub st(1)
00a9ddbf d80c8550d53c01       fmul dword ptr [eax*4 + 0x13cd550]
00a9ddc6 d9c9                 fxch st(1)
00a9ddc8 d80c8554d53c01       fmul dword ptr [eax*4 + 0x13cd554]
00a9ddcf dec1                 faddp st(1)
00a9ddd1 d84c2404             fmul dword ptr [esp + 4]
00a9ddd5 eb12                 jmp 0xa9dde9
00a9ddd7 d905d8de2201         fld dword ptr [0x122ded8]
00a9dddd d864246c             fsub dword ptr [esp + 0x6c]
00a9dde1 d95c2464             fstp dword ptr [esp + 0x64]
00a9dde5 d944246c             fld dword ptr [esp + 0x6c]
00a9dde9 d84c2408             fmul dword ptr [esp + 8]
00a9dded 8bce                 mov ecx, esi
00a9ddef d9442464             fld dword ptr [esp + 0x64]
00a9ddf3 d80e                 fmul dword ptr [esi]
00a9ddf5 d9442430             fld dword ptr [esp + 0x30]
00a9ddf9 d8ca                 fmul st(2)
00a9ddfb dec1                 faddp st(1)
00a9ddfd d95c2410             fstp dword ptr [esp + 0x10]
00a9de01 8b542410             mov edx, dword ptr [esp + 0x10]
00a9de05 d9442464             fld dword ptr [esp + 0x64]
00a9de09 d84e04               fmul dword ptr [esi + 4]
00a9de0c d9442434             fld dword ptr [esp + 0x34]
00a9de10 d8ca                 fmul st(2)
00a9de12 dec1                 faddp st(1)
00a9de14 d95c2414             fstp dword ptr [esp + 0x14]
00a9de18 8b442414             mov eax, dword ptr [esp + 0x14]
00a9de1c d9442464             fld dword ptr [esp + 0x64]
00a9de20 d84e08               fmul dword ptr [esi + 8]
00a9de23 d9c1                 fld st(1)
00a9de25 d84c2438             fmul dword ptr [esp + 0x38]
00a9de29 dec1                 faddp st(1)
00a9de2b d95c2418             fstp dword ptr [esp + 0x18]
00a9de2f d84c243c             fmul dword ptr [esp + 0x3c]
00a9de33 d9442464             fld dword ptr [esp + 0x64]
00a9de37 d84e0c               fmul dword ptr [esi + 0xc]
00a9de3a 8911                 mov dword ptr [ecx], edx
00a9de3c 8b542418             mov edx, dword ptr [esp + 0x18]
00a9de40 894104               mov dword ptr [ecx + 4], eax
00a9de43 dec1                 faddp st(1)
00a9de45 895108               mov dword ptr [ecx + 8], edx
00a9de48 d95c241c             fstp dword ptr [esp + 0x1c]
00a9de4c 8b44241c             mov eax, dword ptr [esp + 0x1c]
00a9de50 d9442440             fld dword ptr [esp + 0x40]
00a9de54 89410c               mov dword ptr [ecx + 0xc], eax
00a9de57 d86610               fsub dword ptr [esi + 0x10]
00a9de5a 8d4610               lea eax, [esi + 0x10]
00a9de5d d9442444             fld dword ptr [esp + 0x44]
00a9de61 d86004               fsub dword ptr [eax + 4]
00a9de64 d9442448             fld dword ptr [esp + 0x48]
00a9de68 d86008               fsub dword ptr [eax + 8]
00a9de6b d95c2418             fstp dword ptr [esp + 0x18]
00a9de6f d944244c             fld dword ptr [esp + 0x4c]
00a9de73 d8600c               fsub dword ptr [eax + 0xc]
00a9de76 d95c241c             fstp dword ptr [esp + 0x1c]
00a9de7a d9c9                 fxch st(1)
00a9de7c d84c246c             fmul dword ptr [esp + 0x6c]
00a9de80 d95c2420             fstp dword ptr [esp + 0x20]
00a9de84 d84c246c             fmul dword ptr [esp + 0x6c]
00a9de88 d9442418             fld dword ptr [esp + 0x18]
00a9de8c d84c246c             fmul dword ptr [esp + 0x6c]
00a9de90 d944241c             fld dword ptr [esp + 0x1c]
00a9de94 d84c246c             fmul dword ptr [esp + 0x6c]
00a9de98 d95c242c             fstp dword ptr [esp + 0x2c]
00a9de9c d9442420             fld dword ptr [esp + 0x20]
00a9dea0 d800                 fadd dword ptr [eax]
00a9dea2 d95c2410             fstp dword ptr [esp + 0x10]
00a9dea6 8b4c2410             mov ecx, dword ptr [esp + 0x10]
00a9deaa d9c9                 fxch st(1)
00a9deac d84004               fadd dword ptr [eax + 4]
00a9deaf d95c2414             fstp dword ptr [esp + 0x14]
00a9deb3 8b542414             mov edx, dword ptr [esp + 0x14]
00a9deb7 d84008               fadd dword ptr [eax + 8]
00a9deba d95c2418             fstp dword ptr [esp + 0x18]
00a9debe d944242c             fld dword ptr [esp + 0x2c]
00a9dec2 d8400c               fadd dword ptr [eax + 0xc]
00a9dec5 8908                 mov dword ptr [eax], ecx
00a9dec7 8b4c2418             mov ecx, dword ptr [esp + 0x18]
00a9decb 895004               mov dword ptr [eax + 4], edx
00a9dece d95c241c             fstp dword ptr [esp + 0x1c]
00a9ded2 8b54241c             mov edx, dword ptr [esp + 0x1c]
00a9ded6 894808               mov dword ptr [eax + 8], ecx
00a9ded9 89500c               mov dword ptr [eax + 0xc], edx
00a9dedc 83c460               add esp, 0x60
00a9dedf c20c00               ret 0xc
