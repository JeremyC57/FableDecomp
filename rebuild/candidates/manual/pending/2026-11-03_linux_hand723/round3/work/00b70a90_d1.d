00b70a90 CEngineWaterRenderer::RenderEnvironmentMap DIFFER(3520v3567) flags= []
@@ retail 0-1 built 0-1
 - 00b70a90  83ec58                 sub esp, 0x58
 + 00b70a90  83ec54                 sub esp, 0x54
   00b70a93  a18c6e4301             mov eax, dword ptr [0x1436e8c]
   00b70a98  894c2404               mov dword ptr [esp + 4], ecx
@@ retail 11-13 built 11-13
   00b70aab  8bc8                   mov ecx, eax
   00b70aad  ff5204                 call dword ptr [edx + 4]
 - 00b70ab0  8b15986e4301           mov edx, dword ptr [0x1436e98]
 - 00b70ab6  8bba54090000           mov edi, dword ptr [edx + 0x954]
 + 00b70ab0  8b0d00000000           mov ecx, dword ptr [0]
 + 00b70ab6  8bb954090000           mov edi, dword ptr [ecx + 0x954]
   00b70abc  89442410               mov dword ptr [esp + 0x10], eax
   00b70ac0  8b4054                 mov eax, dword ptr [eax + 0x54]
@@ retail 15-16 built 15-18
   00b70abc  89442410               mov dword ptr [esp + 0x10], eax
   00b70ac0  8b4054                 mov eax, dword ptr [eax + 0x54]
 - 00b70ac3  8b80a8010000           mov eax, dword ptr [eax + 0x1a8]
 + 00b70ac3  05a8010000             add eax, 0x1a8
 + 00b70ac8  89442414               mov dword ptr [esp + 0x14], eax
 + 00b70acc  8b00                   mov eax, dword ptr [eax]
   00b70ac9  8b08                   mov ecx, dword ptr [eax]
   00b70acb  85c9                   test ecx, ecx
@@ retail 146-147 built 148-149
   00b70c2e  5f                     pop edi
   00b70c2f  5b                     pop ebx
 - 00b70c30  83c458                 add esp, 0x58
 + 00b70c35  83c454                 add esp, 0x54
   00b70c33  c3                     ret 
   00b70c34  6a44                   push 0x44
@@ retail 176-177 built 178-179
   00b70c78  5f                     pop edi
   00b70c79  5b                     pop ebx
 - 00b70c7a  83c458                 add esp, 0x58
 + 00b70c7f  83c454                 add esp, 0x54
   00b70c7d  c3                     ret 
   00b70c7e  33c0                   xor eax, eax
@@ retail 427-434 built 429-438
   00b70f78  d905d8de2201           fld dword ptr [0x122ded8]
   00b70f7e  8b4c2418               mov ecx, dword ptr [esp + 0x18]
 - 00b70f82  8b4154                 mov eax, dword ptr [ecx + 0x54]
 - 00b70f85  05c0000000             add eax, 0xc0
 - 00b70f8a  8b10                   mov edx, dword ptr [eax]
 - 00b70f8c  89542448               mov dword ptr [esp + 0x48], edx
 - 00b70f90  8b5004                 mov edx, dword ptr [eax + 4]
 - 00b70f93  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b70f97  8b5008                 mov edx, dword ptr [eax + 8]
 + 00b70f87  8b5154                 mov edx, dword ptr [ecx + 0x54]
 + 00b70f8a  81c2c0000000           add edx, 0xc0
 + 00b70f90  8b02                   mov eax, dword ptr [edx]
 + 00b70f92  89442444               mov dword ptr [esp + 0x44], eax
 + 00b70f96  8b4204                 mov eax, dword ptr [edx + 4]
 + 00b70f99  89442448               mov dword ptr [esp + 0x48], eax
 + 00b70f9d  8b4208                 mov eax, dword ptr [edx + 8]
 + 00b70fa0  8944244c               mov dword ptr [esp + 0x4c], eax
 + 00b70fa4  8b520c                 mov edx, dword ptr [edx + 0xc]
   00b70f9a  89542450               mov dword ptr [esp + 0x50], edx
   00b70f9e  8b400c                 mov eax, dword ptr [eax + 0xc]
@@ retail 435-437 built 439-439
   00b70f97  8b5008                 mov edx, dword ptr [eax + 8]
   00b70f9a  89542450               mov dword ptr [esp + 0x50], edx
 - 00b70f9e  8b400c                 mov eax, dword ptr [eax + 0xc]
 - 00b70fa1  89442454               mov dword ptr [esp + 0x54], eax
   00b70fa5  8b4154                 mov eax, dword ptr [ecx + 0x54]
   00b70fa8  05d0000000             add eax, 0xd0
@@ retail 440-440 built 442-444
   00b70fa8  05d0000000             add eax, 0xd0
   00b70fad  8b10                   mov edx, dword ptr [eax]
 + 00b70fb5  89542454               mov dword ptr [esp + 0x54], edx
 + 00b70fb9  8b5004                 mov edx, dword ptr [eax + 4]
   00b70faf  89542458               mov dword ptr [esp + 0x58], edx
   00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
@@ retail 441-442 built 445-446
   00b70fad  8b10                   mov edx, dword ptr [eax]
   00b70faf  89542458               mov dword ptr [esp + 0x58], edx
 - 00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
 + 00b70fc0  8b5008                 mov edx, dword ptr [eax + 8]
   00b70fb6  8954245c               mov dword ptr [esp + 0x5c], edx
   00b70fba  8b5008                 mov edx, dword ptr [eax + 8]
@@ retail 443-445 built 447-447
   00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
   00b70fb6  8954245c               mov dword ptr [esp + 0x5c], edx
 - 00b70fba  8b5008                 mov edx, dword ptr [eax + 8]
 - 00b70fbd  89542460               mov dword ptr [esp + 0x60], edx
   00b70fc1  8b400c                 mov eax, dword ptr [eax + 0xc]
   00b70fc4  89442464               mov dword ptr [esp + 0x64], eax
@@ retail 446-451 built 448-452
   00b70fbd  89542460               mov dword ptr [esp + 0x60], edx
   00b70fc1  8b400c                 mov eax, dword ptr [eax + 0xc]
 - 00b70fc4  89442464               mov dword ptr [esp + 0x64], eax
 - 00b70fc8  8b4954                 mov ecx, dword ptr [ecx + 0x54]
 - 00b70fcb  8b91b4010000           mov edx, dword ptr [ecx + 0x1b4]
 - 00b70fd1  d822                   fsub dword ptr [edx]
 - 00b70fd3  d80d14002301           fmul dword ptr [0x1230014]
 + 00b70fca  8b54241c               mov edx, dword ptr [esp + 0x1c]
 + 00b70fce  89442460               mov dword ptr [esp + 0x60], eax
 + 00b70fd2  8b420c                 mov eax, dword ptr [edx + 0xc]
 + 00b70fd5  d820                   fsub dword ptr [eax]
   00b70fd9  d80d24172301           fmul dword ptr [0x1231724]
   00b70fdf  d815dcde2201           fcom dword ptr [0x122dedc]
@@ retail 465-478 built 466-471
   00b71003  ddd8                   fstp st(0)
   00b71005  d905d8de2201           fld dword ptr [0x122ded8]
 - 00b7100b  d98138010000           fld dword ptr [ecx + 0x138]
 - 00b71011  8b85a8020000           mov eax, dword ptr [ebp + 0x2a8]
 - 00b71017  d88d04030000           fmul dword ptr [ebp + 0x304]
 - 00b7101d  8b95ac020000           mov edx, dword ptr [ebp + 0x2ac]
 - 00b71023  89442438               mov dword ptr [esp + 0x38], eax
 - 00b71027  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 - 00b7102b  8b85b0020000           mov eax, dword ptr [ebp + 0x2b0]
 - 00b71031  d95c2444               fstp dword ptr [esp + 0x44]
 - 00b71035  894c2420               mov dword ptr [esp + 0x20], ecx
 - 00b71039  d9442420               fld dword ptr [esp + 0x20]
 - 00b7103d  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 - 00b71041  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00b71045  d84c242c               fmul dword ptr [esp + 0x2c]
 + 00b71009  8b4154                 mov eax, dword ptr [ecx + 0x54]
 + 00b7100c  d985a8020000           fld dword ptr [ebp + 0x2a8]
 + 00b71012  d985ac020000           fld dword ptr [ebp + 0x2ac]
 + 00b71018  8b95b0020000           mov edx, dword ptr [ebp + 0x2b0]
 + 00b7101e  d98504030000           fld dword ptr [ebp + 0x304]
   00b71049  89542424               mov dword ptr [esp + 0x24], edx
   00b7104d  8954243c               mov dword ptr [esp + 0x3c], edx
@@ retail 479-482 built 472-485
   00b71045  d84c242c               fmul dword ptr [esp + 0x2c]
   00b71049  89542424               mov dword ptr [esp + 0x24], edx
 - 00b7104d  8954243c               mov dword ptr [esp + 0x3c], edx
 - 00b71051  d95c2420               fstp dword ptr [esp + 0x20]
 - 00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
 + 00b71028  d88838010000           fmul dword ptr [eax + 0x138]
 + 00b7102e  a100000000             mov eax, dword ptr [0]
 + 00b71033  8b88cc030000           mov ecx, dword ptr [eax + 0x3cc]
 + 00b71039  83c150                 add ecx, 0x50
 + 00b7103c  d95c2428               fstp dword ptr [esp + 0x28]
 + 00b71040  d9c9                   fxch st(1)
 + 00b71042  d84c2428               fmul dword ptr [esp + 0x28]
 + 00b71046  d95c2434               fstp dword ptr [esp + 0x34]
 + 00b7104a  8b542434               mov edx, dword ptr [esp + 0x34]
 + 00b7104e  8911                   mov dword ptr [ecx], edx
 + 00b71050  d84c2428               fmul dword ptr [esp + 0x28]
 + 00b71054  d95c2438               fstp dword ptr [esp + 0x38]
 + 00b71058  8b542438               mov edx, dword ptr [esp + 0x38]
   00b71059  d9442424               fld dword ptr [esp + 0x24]
   00b7105d  89442428               mov dword ptr [esp + 0x28], eax
@@ retail 483-486 built 486-504
   00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
   00b71059  d9442424               fld dword ptr [esp + 0x24]
 - 00b7105d  89442428               mov dword ptr [esp + 0x28], eax
 - 00b71061  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71065  89442440               mov dword ptr [esp + 0x40], eax
 + 00b71060  895104                 mov dword ptr [ecx + 4], edx
 + 00b71063  d84c2428               fmul dword ptr [esp + 0x28]
 + 00b71067  d95c243c               fstp dword ptr [esp + 0x3c]
 + 00b7106b  8b54243c               mov edx, dword ptr [esp + 0x3c]
 + 00b7106f  d90500000000           fld dword ptr [0]
 + 00b71075  895108                 mov dword ptr [ecx + 8], edx
 + 00b71078  d8642428               fsub dword ptr [esp + 0x28]
 + 00b7107c  d95c2440               fstp dword ptr [esp + 0x40]
 + 00b71080  8b542440               mov edx, dword ptr [esp + 0x40]
 + 00b71084  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00b71087  8bb0d8030000           mov esi, dword ptr [eax + 0x3d8]
 + 00b7108d  d9542434               fst dword ptr [esp + 0x34]
 + 00b71091  8b542434               mov edx, dword ptr [esp + 0x34]
 + 00b71095  d9542438               fst dword ptr [esp + 0x38]
 + 00b71099  0bf7                   or esi, edi
 + 00b7109b  d954243c               fst dword ptr [esp + 0x3c]
 + 00b7109f  89b0d8030000           mov dword ptr [eax + 0x3d8], esi
 + 00b710a5  d95c2440               fstp dword ptr [esp + 0x40]
   00b71069  a1146e4301             mov eax, dword ptr [0x1436e14]
   00b7106e  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
@@ retail 487-521 built 505-514
   00b71065  89442440               mov dword ptr [esp + 0x40], eax
   00b71069  a1146e4301             mov eax, dword ptr [0x1436e14]
 - 00b7106e  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
 - 00b71074  d95c2424               fstp dword ptr [esp + 0x24]
 - 00b71078  83c250                 add edx, 0x50
 - 00b7107b  d9442428               fld dword ptr [esp + 0x28]
 - 00b7107f  890a                   mov dword ptr [edx], ecx
 - 00b71081  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71085  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 - 00b71089  894a04                 mov dword ptr [edx + 4], ecx
 - 00b7108c  d95c2428               fstp dword ptr [esp + 0x28]
 - 00b71090  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 - 00b71094  d905d8de2201           fld dword ptr [0x122ded8]
 - 00b7109a  894a08                 mov dword ptr [edx + 8], ecx
 - 00b7109d  d864242c               fsub dword ptr [esp + 0x2c]
 - 00b710a1  d95c242c               fstp dword ptr [esp + 0x2c]
 - 00b710a5  8b4c242c               mov ecx, dword ptr [esp + 0x2c]
 - 00b710a9  894a0c                 mov dword ptr [edx + 0xc], ecx
 - 00b710ac  8b98d8030000           mov ebx, dword ptr [eax + 0x3d8]
 - 00b710b2  d9542438               fst dword ptr [esp + 0x38]
 - 00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 - 00b710ba  d954243c               fst dword ptr [esp + 0x3c]
 - 00b710be  0bdf                   or ebx, edi
 - 00b710c0  d9542440               fst dword ptr [esp + 0x40]
 - 00b710c4  8998d8030000           mov dword ptr [eax + 0x3d8], ebx
 - 00b710ca  d95c2444               fstp dword ptr [esp + 0x44]
 - 00b710ce  a1146e4301             mov eax, dword ptr [0x1436e14]
 - 00b710d3  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
 - 00b710d9  83c260                 add edx, 0x60
 - 00b710dc  890a                   mov dword ptr [edx], ecx
 - 00b710de  8b4c243c               mov ecx, dword ptr [esp + 0x3c]
 - 00b710e2  894a04                 mov dword ptr [edx + 4], ecx
 - 00b710e5  8b4c2440               mov ecx, dword ptr [esp + 0x40]
 - 00b710e9  894a08                 mov dword ptr [edx + 8], ecx
 - 00b710ec  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 - 00b710f0  894a0c                 mov dword ptr [edx + 0xc], ecx
 + 00b710ae  8b88cc030000           mov ecx, dword ptr [eax + 0x3cc]
 + 00b710b4  83c160                 add ecx, 0x60
 + 00b710b7  8911                   mov dword ptr [ecx], edx
 + 00b710b9  8b542438               mov edx, dword ptr [esp + 0x38]
 + 00b710bd  895104                 mov dword ptr [ecx + 4], edx
 + 00b710c0  8b54243c               mov edx, dword ptr [esp + 0x3c]
 + 00b710c4  895108                 mov dword ptr [ecx + 8], edx
 + 00b710c7  8b542440               mov edx, dword ptr [esp + 0x40]
 + 00b710cb  89510c                 mov dword ptr [ecx + 0xc], edx
   00b710f3  09b8d8030000           or dword ptr [eax + 0x3d8], edi
   00b710f9  a1a06e4301             mov eax, dword ptr [0x1436ea0]
@@ retail 523-524 built 516-518
   00b710f3  09b8d8030000           or dword ptr [eax + 0x3d8], edi
   00b710f9  a1a06e4301             mov eax, dword ptr [0x1436ea0]
 - 00b710fe  8b90bc010000           mov edx, dword ptr [eax + 0x1bc]
 + 00b710d9  d980b4010000           fld dword ptr [eax + 0x1b4]
 + 00b710df  8d4c242c               lea ecx, [esp + 0x2c]
   00b71104  d980b8010000           fld dword ptr [eax + 0x1b8]
   00b7110a  8b80b4010000           mov eax, dword ptr [eax + 0x1b4]
@@ retail 525-530 built 519-524
   00b710fe  8b90bc010000           mov edx, dword ptr [eax + 0x1bc]
   00b71104  d980b8010000           fld dword ptr [eax + 0x1b8]
 - 00b7110a  8b80b4010000           mov eax, dword ptr [eax + 0x1b4]
 - 00b71110  d95c2434               fstp dword ptr [esp + 0x34]
 - 00b71114  8d4c2430               lea ecx, [esp + 0x30]
 - 00b71118  89542418               mov dword ptr [esp + 0x18], edx
 - 00b7111c  89442430               mov dword ptr [esp + 0x30], eax
 + 00b710e9  8b80bc010000           mov eax, dword ptr [eax + 0x1bc]
 + 00b710ef  d9c9                   fxch st(1)
 + 00b710f1  8944243c               mov dword ptr [esp + 0x3c], eax
 + 00b710f5  d95c242c               fstp dword ptr [esp + 0x2c]
 + 00b710f9  d95c2430               fstp dword ptr [esp + 0x30]
   00b71120  e8eb33eaff             call 0xa14510
   00b71125  8b35a06e4301           mov esi, dword ptr [0x1436ea0]
@@ retail 535-536 built 529-530
   00b7112e  d80d087f2301           fmul dword ptr [0x1237f08]
   00b71134  d95c241c               fstp dword ptr [esp + 0x1c]
 - 00b71138  d9442434               fld dword ptr [esp + 0x34]
 + 00b71115  d9442430               fld dword ptr [esp + 0x30]
   00b7113c  e861da0800             call 0xbfeba2
   00b71141  d80d087f2301           fmul dword ptr [0x1237f08]
@@ retail 538-539 built 532-533
   00b7113c  e861da0800             call 0xbfeba2
   00b71141  d80d087f2301           fmul dword ptr [0x1237f08]
 - 00b71147  d9442430               fld dword ptr [esp + 0x30]
 + 00b71124  d944242c               fld dword ptr [esp + 0x2c]
   00b7114b  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00b71151  dfe0                   fnstsw ax
@@ retail 548-549 built 542-542
   00b71164  84c0                   test al, al
   00b71166  d80d9cf52201           fmul dword ptr [0x122f59c]
 - 00b7116c  d80d087f2301           fmul dword ptr [0x1237f08]
   00b71172  d8e9                   fsubr st(1)
   00b71174  d95c2420               fstp dword ptr [esp + 0x20]
@@ retail 550-551 built 543-544
   00b7116c  d80d087f2301           fmul dword ptr [0x1237f08]
   00b71172  d8e9                   fsubr st(1)
 - 00b71174  d95c2420               fstp dword ptr [esp + 0x20]
 + 00b7114b  d95c2418               fstp dword ptr [esp + 0x18]
   00b71178  ddd8                   fstp st(0)
   00b7117a  7409                   je 0xb71185
@@ retail 556-557 built 549-550
   00b7117f  894c2414               mov dword ptr [esp + 0x14], ecx
   00b71183  eb13                   jmp 0xb71198
 - 00b71185  d905d8de2201           fld dword ptr [0x122ded8]
 + 00b7115c  d94660                 fld dword ptr [esi + 0x60]
   00b7118b  d8b6d0000000           fdiv dword ptr [esi + 0xd0]
   00b71191  d84e60                 fmul dword ptr [esi + 0x60]
@@ retail 558-559 built 551-551
   00b71185  d905d8de2201           fld dword ptr [0x122ded8]
   00b7118b  d8b6d0000000           fdiv dword ptr [esi + 0xd0]
 - 00b71191  d84e60                 fmul dword ptr [esi + 0x60]
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 561-562 built 553-554
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b7119e  8d542448               lea edx, [esp + 0x48]
 + 00b7116f  8d542444               lea edx, [esp + 0x44]
   00b711a2  52                     push edx
   00b711a3  6a00                   push 0
@@ retail 566-567 built 558-559
   00b711a5  e8a689e1ff             call 0x989b50
   00b711aa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b711b0  8d442458               lea eax, [esp + 0x58]
 + 00b71181  8d442454               lea eax, [esp + 0x54]
   00b711b4  50                     push eax
   00b711b5  bb01000000             mov ebx, 1
@@ retail 571-572 built 563-564
   00b711ba  53                     push ebx
   00b711bb  e89089e1ff             call 0x989b50
 - 00b711c0  d9442418               fld dword ptr [esp + 0x18]
 + 00b71191  d944243c               fld dword ptr [esp + 0x3c]
   00b711c4  6a00                   push 0
   00b711c6  e8d7d90800             call 0xbfeba2
@@ retail 576-577 built 568-569
   00b711cb  d82d2c332a01           fsubr dword ptr [0x12a332c]
   00b711d1  d9442418               fld dword ptr [esp + 0x18]
 - 00b711d5  8b542424               mov edx, dword ptr [esp + 0x24]
 + 00b711a6  8b54241c               mov edx, dword ptr [esp + 0x1c]
   00b711d9  d80d9cf52201           fmul dword ptr [0x122f59c]
   00b711df  51                     push ecx
@@ retail 618-619 built 610-611
   00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71249  8b3d08000000           mov edi, dword ptr [8]
   00b7127e  8b90882f0000           mov edx, dword ptr [eax + 0x2f88]
   00b71284  3bd7                   cmp edx, edi
@@ retail 670-671 built 662-663
   00b71343  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71349  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7134e  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b7131f  8b3d08000000           mov edi, dword ptr [8]
   00b71354  8b909c2f0000           mov edx, dword ptr [eax + 0x2f9c]
   00b7135a  3bd7                   cmp edx, edi
@@ retail 766-767 built 758-759
   00b71498  e8836be1ff             call 0x988020
   00b7149d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b714a2  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00b71473  8b3510000000           mov esi, dword ptr [0x10]
   00b714a8  8ba820290000           mov ebp, dword ptr [eax + 0x2920]
   00b714ae  3bee                   cmp ebp, esi
@@ retail 794-795 built 786-787
   00b71506  ff820c280000           inc dword ptr [edx + 0x280c]
   00b7150c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71511  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00b714e2  8b3514000000           mov esi, dword ptr [0x14]
   00b71517  8ba834290000           mov ebp, dword ptr [eax + 0x2934]
   00b7151d  3bee                   cmp ebp, esi
@@ retail 801-801 built 793-794
   00b71527  8bfe                   mov edi, esi
   00b71529  7455                   je 0xb71580
 + 00b714fc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
   00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 802-803 built 795-795
   00b71529  7455                   je 0xb71580
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71534  85c6                   test esi, eax
   00b71536  7522                   jne 0xb7155a
@@ retail 827-827 built 819-820
   00b7158e  8bc8                   mov ecx, eax
   00b71590  7450                   je 0xb715e2
 + 00b71563  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
   00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 828-829 built 821-821
   00b71590  7450                   je 0xb715e2
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7159b  85c6                   test esi, eax
   00b7159d  7522                   jne 0xb715c1
@@ retail 870-871 built 862-863
   00b71628  899fb83c0000           mov dword ptr [edi + 0x3cb8], ebx
   00b7162e  55                     push ebp
 - 00b7162f  8d442424               lea eax, [esp + 0x24]
 + 00b71600  8d442420               lea eax, [esp + 0x20]
   00b71633  50                     push eax
   00b71634  8bce                   mov ecx, esi
@@ retail 876-877 built 868-869
   00b7163b  8b38                   mov edi, dword ptr [eax]
   00b7163d  55                     push ebp
 - 00b7163e  8d4c2424               lea ecx, [esp + 0x24]
 + 00b7160f  8d4c2420               lea ecx, [esp + 0x20]
   00b71642  51                     push ecx
   00b71643  8bce                   mov ecx, esi
@@ retail 890-892 built 882-884
   00b7165d  85f6                   test esi, esi
   00b7165f  55                     push ebp
 - 00b71660  89742424               mov dword ptr [esp + 0x24], esi
 - 00b71664  db442424               fild dword ptr [esp + 0x24]
 + 00b71631  89742420               mov dword ptr [esp + 0x20], esi
 + 00b71635  db442420               fild dword ptr [esp + 0x20]
   00b71668  55                     push ebp
   00b71669  7d06                   jge 0xb71671
@@ retail 898-900 built 890-892
   00b71674  85ff                   test edi, edi
   00b71676  d95c2404               fstp dword ptr [esp + 4]
 - 00b7167a  897c2430               mov dword ptr [esp + 0x30], edi
 - 00b7167e  db442430               fild dword ptr [esp + 0x30]
 + 00b7164b  897c242c               mov dword ptr [esp + 0x2c], edi
 + 00b7164f  db44242c               fild dword ptr [esp + 0x2c]
   00b71682  7d06                   jge 0xb7168a
   00b71684  d805b4dc2201           fadd dword ptr [0x122dcb4]
@@ retail 907-908 built 899-900
   00b71694  e86784e1ff             call 0x989b00
   00b71699  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00b7169f  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b71670  8b1508000000           mov edx, dword ptr [8]
   00b716a5  8bb1e82e0000           mov esi, dword ptr [ecx + 0x2ee8]
   00b716ab  3bf2                   cmp esi, edx
@@ retail 933-934 built 925-926
   00b716fa  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00b71701  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00b71707  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b716d8  8b1508000000           mov edx, dword ptr [8]
   00b7170d  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71712  8bb0882f0000           mov esi, dword ptr [eax + 0x2f88]
@@ retail 940-942 built 932-934
   00b71720  8bfa                   mov edi, edx
   00b71722  7450                   je 0xb71774
 - 00b71724  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b71727  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 + 00b716f5  8b5108                 mov edx, dword ptr [ecx + 8]
 + 00b716f8  8bb014280000           mov esi, dword ptr [eax + 0x2814]
   00b7172d  85d6                   test esi, edx
   00b7172f  7522                   jne 0xb71753
@@ retail 1049-1050 built 1041-1042
   00b71879  5f                     pop edi
   00b7187a  5b                     pop ebx
 - 00b7187b  83c458                 add esp, 0x58
 + 00b7184c  83c454                 add esp, 0x54
   00b7187e  c3                     ret 
DIFFLINES 143 retail_instr 1051 built_instr 1043
