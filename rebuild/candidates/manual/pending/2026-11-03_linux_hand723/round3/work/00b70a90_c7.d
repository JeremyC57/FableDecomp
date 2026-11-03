00b70a90 CEngineWaterRenderer::RenderEnvironmentMap DIFFER(3576v3567) flags= []
@@ retail 426-450 built 426-430
   00b70f73  e8a870e1ff             call 0x988020
   00b70f78  d905d8de2201           fld dword ptr [0x122ded8]
 - 00b70f7e  8b4c2418               mov ecx, dword ptr [esp + 0x18]
 - 00b70f82  8b4154                 mov eax, dword ptr [ecx + 0x54]
 - 00b70f85  05c0000000             add eax, 0xc0
 - 00b70f8a  8b10                   mov edx, dword ptr [eax]
 - 00b70f8c  89542448               mov dword ptr [esp + 0x48], edx
 - 00b70f90  8b5004                 mov edx, dword ptr [eax + 4]
 - 00b70f93  8954244c               mov dword ptr [esp + 0x4c], edx
 - 00b70f97  8b5008                 mov edx, dword ptr [eax + 8]
 - 00b70f9a  89542450               mov dword ptr [esp + 0x50], edx
 - 00b70f9e  8b400c                 mov eax, dword ptr [eax + 0xc]
 - 00b70fa1  89442454               mov dword ptr [esp + 0x54], eax
 - 00b70fa5  8b4154                 mov eax, dword ptr [ecx + 0x54]
 - 00b70fa8  05d0000000             add eax, 0xd0
 - 00b70fad  8b10                   mov edx, dword ptr [eax]
 - 00b70faf  89542458               mov dword ptr [esp + 0x58], edx
 - 00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
 - 00b70fb6  8954245c               mov dword ptr [esp + 0x5c], edx
 - 00b70fba  8b5008                 mov edx, dword ptr [eax + 8]
 - 00b70fbd  89542460               mov dword ptr [esp + 0x60], edx
 - 00b70fc1  8b400c                 mov eax, dword ptr [eax + 0xc]
 - 00b70fc4  89442464               mov dword ptr [esp + 0x64], eax
 - 00b70fc8  8b4954                 mov ecx, dword ptr [ecx + 0x54]
 - 00b70fcb  8b91b4010000           mov edx, dword ptr [ecx + 0x1b4]
 - 00b70fd1  d822                   fsub dword ptr [edx]
 + 00b70f7e  8b542418               mov edx, dword ptr [esp + 0x18]
 + 00b70f82  8b4a54                 mov ecx, dword ptr [edx + 0x54]
 + 00b70f85  8b81b4010000           mov eax, dword ptr [ecx + 0x1b4]
 + 00b70f8b  d820                   fsub dword ptr [eax]
   00b70fd3  d80d14002301           fmul dword ptr [0x1230014]
   00b70fd9  d80d24172301           fmul dword ptr [0x1231724]
@@ retail 465-467 built 445-468
   00b71003  ddd8                   fstp st(0)
   00b71005  d905d8de2201           fld dword ptr [0x122ded8]
 - 00b7100b  d98138010000           fld dword ptr [ecx + 0x138]
 - 00b71011  8b85a8020000           mov eax, dword ptr [ebp + 0x2a8]
 + 00b70fc5  81c1c0000000           add ecx, 0xc0
 + 00b70fcb  8b01                   mov eax, dword ptr [ecx]
 + 00b70fcd  89442448               mov dword ptr [esp + 0x48], eax
 + 00b70fd1  8b4104                 mov eax, dword ptr [ecx + 4]
 + 00b70fd4  8944244c               mov dword ptr [esp + 0x4c], eax
 + 00b70fd8  8b4108                 mov eax, dword ptr [ecx + 8]
 + 00b70fdb  89442450               mov dword ptr [esp + 0x50], eax
 + 00b70fdf  8b490c                 mov ecx, dword ptr [ecx + 0xc]
 + 00b70fe2  894c2454               mov dword ptr [esp + 0x54], ecx
 + 00b70fe6  8b4254                 mov eax, dword ptr [edx + 0x54]
 + 00b70fe9  05d0000000             add eax, 0xd0
 + 00b70fee  8b08                   mov ecx, dword ptr [eax]
 + 00b70ff0  894c2458               mov dword ptr [esp + 0x58], ecx
 + 00b70ff4  8b4804                 mov ecx, dword ptr [eax + 4]
 + 00b70ff7  894c245c               mov dword ptr [esp + 0x5c], ecx
 + 00b70ffb  8b4808                 mov ecx, dword ptr [eax + 8]
 + 00b70ffe  894c2460               mov dword ptr [esp + 0x60], ecx
 + 00b71002  8b400c                 mov eax, dword ptr [eax + 0xc]
 + 00b71005  8b8da8020000           mov ecx, dword ptr [ebp + 0x2a8]
 + 00b7100b  89442464               mov dword ptr [esp + 0x64], eax
 + 00b7100f  8b5254                 mov edx, dword ptr [edx + 0x54]
 + 00b71012  d98238010000           fld dword ptr [edx + 0x138]
 + 00b71018  8b85ac020000           mov eax, dword ptr [ebp + 0x2ac]
   00b71017  d88d04030000           fmul dword ptr [ebp + 0x304]
   00b7101d  8b95ac020000           mov edx, dword ptr [ebp + 0x2ac]
@@ retail 468-472 built 469-473
   00b71011  8b85a8020000           mov eax, dword ptr [ebp + 0x2a8]
   00b71017  d88d04030000           fmul dword ptr [ebp + 0x304]
 - 00b7101d  8b95ac020000           mov edx, dword ptr [ebp + 0x2ac]
 - 00b71023  89442438               mov dword ptr [esp + 0x38], eax
 - 00b71027  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 - 00b7102b  8b85b0020000           mov eax, dword ptr [ebp + 0x2b0]
 + 00b71024  894c2438               mov dword ptr [esp + 0x38], ecx
 + 00b71028  8b8db0020000           mov ecx, dword ptr [ebp + 0x2b0]
 + 00b7102e  894c2440               mov dword ptr [esp + 0x40], ecx
 + 00b71032  8b542440               mov edx, dword ptr [esp + 0x40]
   00b71031  d95c2444               fstp dword ptr [esp + 0x44]
   00b71035  894c2420               mov dword ptr [esp + 0x20], ecx
@@ retail 473-474 built 474-479
   00b7102b  8b85b0020000           mov eax, dword ptr [ebp + 0x2b0]
   00b71031  d95c2444               fstp dword ptr [esp + 0x44]
 - 00b71035  894c2420               mov dword ptr [esp + 0x20], ecx
 + 00b7103a  8944243c               mov dword ptr [esp + 0x3c], eax
 + 00b7103e  8b442438               mov eax, dword ptr [esp + 0x38]
 + 00b71042  8b4c243c               mov ecx, dword ptr [esp + 0x3c]
 + 00b71046  894c2424               mov dword ptr [esp + 0x24], ecx
 + 00b7104a  89442420               mov dword ptr [esp + 0x20], eax
   00b71039  d9442420               fld dword ptr [esp + 0x20]
   00b7103d  8b4c2444               mov ecx, dword ptr [esp + 0x44]
@@ retail 475-477 built 480-482
   00b71035  894c2420               mov dword ptr [esp + 0x20], ecx
   00b71039  d9442420               fld dword ptr [esp + 0x20]
 - 00b7103d  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 - 00b71041  894c242c               mov dword ptr [esp + 0x2c], ecx
 + 00b71052  8b442444               mov eax, dword ptr [esp + 0x44]
 + 00b71056  8944242c               mov dword ptr [esp + 0x2c], eax
   00b71045  d84c242c               fmul dword ptr [esp + 0x2c]
   00b71049  89542424               mov dword ptr [esp + 0x24], edx
@@ retail 478-480 built 483-485
   00b71041  894c242c               mov dword ptr [esp + 0x2c], ecx
   00b71045  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71049  89542424               mov dword ptr [esp + 0x24], edx
 - 00b7104d  8954243c               mov dword ptr [esp + 0x3c], edx
 + 00b7105e  a100000000             mov eax, dword ptr [0]
 + 00b71063  8b88cc030000           mov ecx, dword ptr [eax + 0x3cc]
   00b71051  d95c2420               fstp dword ptr [esp + 0x20]
   00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
@@ retail 481-482 built 486-487
   00b7104d  8954243c               mov dword ptr [esp + 0x3c], edx
   00b71051  d95c2420               fstp dword ptr [esp + 0x20]
 - 00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
 + 00b7106d  89542428               mov dword ptr [esp + 0x28], edx
   00b71059  d9442424               fld dword ptr [esp + 0x24]
   00b7105d  89442428               mov dword ptr [esp + 0x28], eax
@@ retail 483-484 built 488-489
   00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
   00b71059  d9442424               fld dword ptr [esp + 0x24]
 - 00b7105d  89442428               mov dword ptr [esp + 0x28], eax
 + 00b71075  8b542420               mov edx, dword ptr [esp + 0x20]
   00b71061  d84c242c               fmul dword ptr [esp + 0x2c]
   00b71065  89442440               mov dword ptr [esp + 0x40], eax
@@ retail 485-488 built 490-492
   00b7105d  89442428               mov dword ptr [esp + 0x28], eax
   00b71061  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71065  89442440               mov dword ptr [esp + 0x40], eax
 - 00b71069  a1146e4301             mov eax, dword ptr [0x1436e14]
 - 00b7106e  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
 + 00b7107d  83c150                 add ecx, 0x50
 + 00b71080  8911                   mov dword ptr [ecx], edx
   00b71074  d95c2424               fstp dword ptr [esp + 0x24]
   00b71078  83c250                 add edx, 0x50
@@ retail 489-490 built 493-494
   00b7106e  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
   00b71074  d95c2424               fstp dword ptr [esp + 0x24]
 - 00b71078  83c250                 add edx, 0x50
 + 00b71086  8b542424               mov edx, dword ptr [esp + 0x24]
   00b7107b  d9442428               fld dword ptr [esp + 0x28]
   00b7107f  890a                   mov dword ptr [edx], ecx
@@ retail 491-492 built 495-496
   00b71078  83c250                 add edx, 0x50
   00b7107b  d9442428               fld dword ptr [esp + 0x28]
 - 00b7107f  890a                   mov dword ptr [edx], ecx
 + 00b7108e  895104                 mov dword ptr [ecx + 4], edx
   00b71081  d84c242c               fmul dword ptr [esp + 0x2c]
   00b71085  8b4c2424               mov ecx, dword ptr [esp + 0x24]
@@ retail 493-495 built 497-497
   00b7107f  890a                   mov dword ptr [edx], ecx
   00b71081  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71085  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 - 00b71089  894a04                 mov dword ptr [edx + 4], ecx
   00b7108c  d95c2428               fstp dword ptr [esp + 0x28]
   00b71090  8b4c2428               mov ecx, dword ptr [esp + 0x28]
@@ retail 496-497 built 498-499
   00b71089  894a04                 mov dword ptr [edx + 4], ecx
   00b7108c  d95c2428               fstp dword ptr [esp + 0x28]
 - 00b71090  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 + 00b71099  8b542428               mov edx, dword ptr [esp + 0x28]
   00b71094  d905d8de2201           fld dword ptr [0x122ded8]
   00b7109a  894a08                 mov dword ptr [edx + 8], ecx
@@ retail 498-499 built 500-501
   00b71090  8b4c2428               mov ecx, dword ptr [esp + 0x28]
   00b71094  d905d8de2201           fld dword ptr [0x122ded8]
 - 00b7109a  894a08                 mov dword ptr [edx + 8], ecx
 + 00b710a3  895108                 mov dword ptr [ecx + 8], edx
   00b7109d  d864242c               fsub dword ptr [esp + 0x2c]
   00b710a1  d95c242c               fstp dword ptr [esp + 0x2c]
@@ retail 501-504 built 503-506
   00b7109d  d864242c               fsub dword ptr [esp + 0x2c]
   00b710a1  d95c242c               fstp dword ptr [esp + 0x2c]
 - 00b710a5  8b4c242c               mov ecx, dword ptr [esp + 0x2c]
 - 00b710a9  894a0c                 mov dword ptr [edx + 0xc], ecx
 - 00b710ac  8b98d8030000           mov ebx, dword ptr [eax + 0x3d8]
 + 00b710ae  8b54242c               mov edx, dword ptr [esp + 0x2c]
 + 00b710b2  89510c                 mov dword ptr [ecx + 0xc], edx
 + 00b710b5  8bb0d8030000           mov esi, dword ptr [eax + 0x3d8]
   00b710b2  d9542438               fst dword ptr [esp + 0x38]
   00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
@@ retail 505-506 built 507-508
   00b710ac  8b98d8030000           mov ebx, dword ptr [eax + 0x3d8]
   00b710b2  d9542438               fst dword ptr [esp + 0x38]
 - 00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 + 00b710bf  8b542438               mov edx, dword ptr [esp + 0x38]
   00b710ba  d954243c               fst dword ptr [esp + 0x3c]
   00b710be  0bdf                   or ebx, edi
@@ retail 507-508 built 509-510
   00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00b710ba  d954243c               fst dword ptr [esp + 0x3c]
 - 00b710be  0bdf                   or ebx, edi
 + 00b710c7  0bf7                   or esi, edi
   00b710c0  d9542440               fst dword ptr [esp + 0x40]
   00b710c4  8998d8030000           mov dword ptr [eax + 0x3d8], ebx
@@ retail 509-510 built 511-512
   00b710be  0bdf                   or ebx, edi
   00b710c0  d9542440               fst dword ptr [esp + 0x40]
 - 00b710c4  8998d8030000           mov dword ptr [eax + 0x3d8], ebx
 + 00b710cd  89b0d8030000           mov dword ptr [eax + 0x3d8], esi
   00b710ca  d95c2444               fstp dword ptr [esp + 0x44]
   00b710ce  a1146e4301             mov eax, dword ptr [0x1436e14]
@@ retail 512-521 built 514-523
   00b710ca  d95c2444               fstp dword ptr [esp + 0x44]
   00b710ce  a1146e4301             mov eax, dword ptr [0x1436e14]
 - 00b710d3  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
 - 00b710d9  83c260                 add edx, 0x60
 - 00b710dc  890a                   mov dword ptr [edx], ecx
 - 00b710de  8b4c243c               mov ecx, dword ptr [esp + 0x3c]
 - 00b710e2  894a04                 mov dword ptr [edx + 4], ecx
 - 00b710e5  8b4c2440               mov ecx, dword ptr [esp + 0x40]
 - 00b710e9  894a08                 mov dword ptr [edx + 8], ecx
 - 00b710ec  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 - 00b710f0  894a0c                 mov dword ptr [edx + 0xc], ecx
 + 00b710dc  8b88cc030000           mov ecx, dword ptr [eax + 0x3cc]
 + 00b710e2  83c160                 add ecx, 0x60
 + 00b710e5  8911                   mov dword ptr [ecx], edx
 + 00b710e7  8b54243c               mov edx, dword ptr [esp + 0x3c]
 + 00b710eb  895104                 mov dword ptr [ecx + 4], edx
 + 00b710ee  8b542440               mov edx, dword ptr [esp + 0x40]
 + 00b710f2  895108                 mov dword ptr [ecx + 8], edx
 + 00b710f5  8b542444               mov edx, dword ptr [esp + 0x44]
 + 00b710f9  89510c                 mov dword ptr [ecx + 0xc], edx
   00b710f3  09b8d8030000           or dword ptr [eax + 0x3d8], edi
   00b710f9  a1a06e4301             mov eax, dword ptr [0x1436ea0]
@@ retail 523-524 built 525-526
   00b710f3  09b8d8030000           or dword ptr [eax + 0x3d8], edi
   00b710f9  a1a06e4301             mov eax, dword ptr [0x1436ea0]
 - 00b710fe  8b90bc010000           mov edx, dword ptr [eax + 0x1bc]
 + 00b71107  8b88bc010000           mov ecx, dword ptr [eax + 0x1bc]
   00b71104  d980b8010000           fld dword ptr [eax + 0x1b8]
   00b7110a  8b80b4010000           mov eax, dword ptr [eax + 0x1b4]
@@ retail 525-526 built 527-528
   00b710fe  8b90bc010000           mov edx, dword ptr [eax + 0x1bc]
   00b71104  d980b8010000           fld dword ptr [eax + 0x1b8]
 - 00b7110a  8b80b4010000           mov eax, dword ptr [eax + 0x1b4]
 + 00b71113  8b90b4010000           mov edx, dword ptr [eax + 0x1b4]
   00b71110  d95c2434               fstp dword ptr [esp + 0x34]
   00b71114  8d4c2430               lea ecx, [esp + 0x30]
@@ retail 527-527 built 529-530
   00b7110a  8b80b4010000           mov eax, dword ptr [eax + 0x1b4]
   00b71110  d95c2434               fstp dword ptr [esp + 0x34]
 + 00b7111d  894c2418               mov dword ptr [esp + 0x18], ecx
   00b71114  8d4c2430               lea ecx, [esp + 0x30]
   00b71118  89542418               mov dword ptr [esp + 0x18], edx
@@ retail 528-530 built 531-532
   00b71110  d95c2434               fstp dword ptr [esp + 0x34]
   00b71114  8d4c2430               lea ecx, [esp + 0x30]
 - 00b71118  89542418               mov dword ptr [esp + 0x18], edx
 - 00b7111c  89442430               mov dword ptr [esp + 0x30], eax
 + 00b71125  89542430               mov dword ptr [esp + 0x30], edx
   00b71120  e8eb33eaff             call 0xa14510
   00b71125  8b35a06e4301           mov esi, dword ptr [0x1436ea0]
@@ retail 553-555 built 555-557
   00b71178  ddd8                   fstp st(0)
   00b7117a  7409                   je 0xb71185
 - 00b7117c  8b4e64                 mov ecx, dword ptr [esi + 0x64]
 - 00b7117f  894c2414               mov dword ptr [esp + 0x14], ecx
 + 00b71185  8b4664                 mov eax, dword ptr [esi + 0x64]
 + 00b71188  89442414               mov dword ptr [esp + 0x14], eax
   00b71183  eb13                   jmp 0xb71198
   00b71185  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 560-560 built 562-564
   00b71191  d84e60                 fmul dword ptr [esi + 0x60]
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
 + 00b711a1  8d4c2448               lea ecx, [esp + 0x48]
 + 00b711a5  51                     push ecx
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00b7119e  8d542448               lea edx, [esp + 0x48]
@@ retail 561-563 built 565-565
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b7119e  8d542448               lea edx, [esp + 0x48]
 - 00b711a2  52                     push edx
   00b711a3  6a00                   push 0
   00b711a5  e8a689e1ff             call 0x989b50
@@ retail 566-568 built 568-570
   00b711a5  e8a689e1ff             call 0x989b50
   00b711aa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b711b0  8d442458               lea eax, [esp + 0x58]
 - 00b711b4  50                     push eax
 + 00b711b9  8d542458               lea edx, [esp + 0x58]
 + 00b711bd  52                     push edx
   00b711b5  bb01000000             mov ebx, 1
   00b711ba  53                     push ebx
@@ retail 576-577 built 578-579
   00b711cb  d82d2c332a01           fsubr dword ptr [0x12a332c]
   00b711d1  d9442418               fld dword ptr [esp + 0x18]
 - 00b711d5  8b542424               mov edx, dword ptr [esp + 0x24]
 + 00b711de  8b442420               mov eax, dword ptr [esp + 0x20]
   00b711d9  d80d9cf52201           fmul dword ptr [0x122f59c]
   00b711df  51                     push ecx
@@ retail 579-580 built 581-582
   00b711d9  d80d9cf52201           fmul dword ptr [0x122f59c]
   00b711df  51                     push ecx
 - 00b711e0  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 + 00b711e9  8b4c2428               mov ecx, dword ptr [esp + 0x28]
   00b711e4  dec1                   faddp st(1)
   00b711e6  d9f2                   fptan 
@@ retail 586-586 built 588-589
   00b711f0  d80524332a01           fadd dword ptr [0x12a3324]
   00b711f6  d91c24                 fstp dword ptr [esp]
 + 00b71202  50                     push eax
   00b711f9  51                     push ecx
   00b711fa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 588-589 built 591-591
   00b711f9  51                     push ecx
   00b711fa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b71200  52                     push edx
   00b71201  6a02                   push 2
   00b71203  e8f888e1ff             call 0x989b00
@@ retail 618-619 built 620-621
   00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71281  8b3d08000000           mov edi, dword ptr [8]
   00b7127e  8b90882f0000           mov edx, dword ptr [eax + 0x2f88]
   00b71284  3bd7                   cmp edx, edi
@@ retail 670-671 built 672-673
   00b71343  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71349  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7134e  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71357  8b3d08000000           mov edi, dword ptr [8]
   00b71354  8b909c2f0000           mov edx, dword ptr [eax + 0x2f9c]
   00b7135a  3bd7                   cmp edx, edi
@@ retail 766-767 built 768-769
   00b71498  e8836be1ff             call 0x988020
   00b7149d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b714a2  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00b714ab  8b3510000000           mov esi, dword ptr [0x10]
   00b714a8  8ba820290000           mov ebp, dword ptr [eax + 0x2920]
   00b714ae  3bee                   cmp ebp, esi
@@ retail 794-795 built 796-797
   00b71506  ff820c280000           inc dword ptr [edx + 0x280c]
   00b7150c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71511  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00b7151a  8b3514000000           mov esi, dword ptr [0x14]
   00b71517  8ba834290000           mov ebp, dword ptr [eax + 0x2934]
   00b7151d  3bee                   cmp ebp, esi
@@ retail 801-801 built 803-804
   00b71527  8bfe                   mov edi, esi
   00b71529  7455                   je 0xb71580
 + 00b71534  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
   00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 802-803 built 805-805
   00b71529  7455                   je 0xb71580
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71534  85c6                   test esi, eax
   00b71536  7522                   jne 0xb7155a
@@ retail 827-827 built 829-830
   00b7158e  8bc8                   mov ecx, eax
   00b71590  7450                   je 0xb715e2
 + 00b7159b  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
   00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 828-829 built 831-831
   00b71590  7450                   je 0xb715e2
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7159b  85c6                   test esi, eax
   00b7159d  7522                   jne 0xb715c1
@@ retail 907-908 built 909-910
   00b71694  e86784e1ff             call 0x989b00
   00b71699  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00b7169f  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b716a8  8b1508000000           mov edx, dword ptr [8]
   00b716a5  8bb1e82e0000           mov esi, dword ptr [ecx + 0x2ee8]
   00b716ab  3bf2                   cmp esi, edx
@@ retail 913-913 built 915-916
   00b716b3  8bfa                   mov edi, edx
   00b716b5  7456                   je 0xb7170d
 + 00b716c0  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
   00b716b7  8b5008                 mov edx, dword ptr [eax + 8]
   00b716ba  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
@@ retail 914-915 built 917-917
   00b716b5  7456                   je 0xb7170d
   00b716b7  8b5008                 mov edx, dword ptr [eax + 8]
 - 00b716ba  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
   00b716c0  85d6                   test esi, edx
   00b716c2  7522                   jne 0xb716e6
@@ retail 933-934 built 935-936
   00b716fa  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00b71701  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00b71707  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b71710  8b1508000000           mov edx, dword ptr [8]
   00b7170d  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71712  8bb0882f0000           mov esi, dword ptr [eax + 0x2f88]
@@ retail 940-942 built 942-944
   00b71720  8bfa                   mov edi, edx
   00b71722  7450                   je 0xb71774
 - 00b71724  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b71727  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 + 00b7172d  8bb014280000           mov esi, dword ptr [eax + 0x2814]
 + 00b71733  8b5108                 mov edx, dword ptr [ecx + 8]
   00b7172d  85d6                   test esi, edx
   00b7172f  7522                   jne 0xb71753
DIFFLINES 117 retail_instr 1051 built_instr 1053
