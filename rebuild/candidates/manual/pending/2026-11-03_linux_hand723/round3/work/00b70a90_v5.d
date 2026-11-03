00b70a90 CEngineWaterRenderer::RenderEnvironmentMap DIFFER(3553v3567) flags= []
@@ retail 0-1 built 0-1
 - 00b70a90  83ec58                 sub esp, 0x58
 + 00b70a90  83ec54                 sub esp, 0x54
   00b70a93  a18c6e4301             mov eax, dword ptr [0x1436e8c]
   00b70a98  894c2404               mov dword ptr [esp + 4], ecx
@@ retail 11-13 built 11-14
   00b70aab  8bc8                   mov ecx, eax
   00b70aad  ff5204                 call dword ptr [edx + 4]
 - 00b70ab0  8b15986e4301           mov edx, dword ptr [0x1436e98]
 - 00b70ab6  8bba54090000           mov edi, dword ptr [edx + 0x954]
 + 00b70ab0  8b0d00000000           mov ecx, dword ptr [0]
 + 00b70ab6  8b5054                 mov edx, dword ptr [eax + 0x54]
 + 00b70ab9  8bb954090000           mov edi, dword ptr [ecx + 0x954]
   00b70abc  89442410               mov dword ptr [esp + 0x10], eax
   00b70ac0  8b4054                 mov eax, dword ptr [eax + 0x54]
@@ retail 14-16 built 15-16
   00b70ab6  8bba54090000           mov edi, dword ptr [edx + 0x954]
   00b70abc  89442410               mov dword ptr [esp + 0x10], eax
 - 00b70ac0  8b4054                 mov eax, dword ptr [eax + 0x54]
 - 00b70ac3  8b80a8010000           mov eax, dword ptr [eax + 0x1a8]
 + 00b70ac3  8b82a8010000           mov eax, dword ptr [edx + 0x1a8]
   00b70ac9  8b08                   mov ecx, dword ptr [eax]
   00b70acb  85c9                   test ecx, ecx
@@ retail 146-147 built 146-147
   00b70c2e  5f                     pop edi
   00b70c2f  5b                     pop ebx
 - 00b70c30  83c458                 add esp, 0x58
 + 00b70c30  83c454                 add esp, 0x54
   00b70c33  c3                     ret 
   00b70c34  6a44                   push 0x44
@@ retail 176-177 built 176-177
   00b70c78  5f                     pop edi
   00b70c79  5b                     pop ebx
 - 00b70c7a  83c458                 add esp, 0x58
 + 00b70c7a  83c454                 add esp, 0x54
   00b70c7d  c3                     ret 
   00b70c7e  33c0                   xor eax, eax
@@ retail 426-434 built 426-436
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
 + 00b70f7e  8b442418               mov eax, dword ptr [esp + 0x18]
 + 00b70f82  8b5054                 mov edx, dword ptr [eax + 0x54]
 + 00b70f85  81c2c0000000           add edx, 0xc0
 + 00b70f8b  8b0a                   mov ecx, dword ptr [edx]
 + 00b70f8d  894c2444               mov dword ptr [esp + 0x44], ecx
 + 00b70f91  8b4a04                 mov ecx, dword ptr [edx + 4]
 + 00b70f94  894c2448               mov dword ptr [esp + 0x48], ecx
 + 00b70f98  8b4a08                 mov ecx, dword ptr [edx + 8]
 + 00b70f9b  894c244c               mov dword ptr [esp + 0x4c], ecx
 + 00b70f9f  8b520c                 mov edx, dword ptr [edx + 0xc]
   00b70f9a  89542450               mov dword ptr [esp + 0x50], edx
   00b70f9e  8b400c                 mov eax, dword ptr [eax + 0xc]
@@ retail 435-440 built 437-442
   00b70f97  8b5008                 mov edx, dword ptr [eax + 8]
   00b70f9a  89542450               mov dword ptr [esp + 0x50], edx
 - 00b70f9e  8b400c                 mov eax, dword ptr [eax + 0xc]
 - 00b70fa1  89442454               mov dword ptr [esp + 0x54], eax
 - 00b70fa5  8b4154                 mov eax, dword ptr [ecx + 0x54]
 - 00b70fa8  05d0000000             add eax, 0xd0
 - 00b70fad  8b10                   mov edx, dword ptr [eax]
 + 00b70fa6  8b4854                 mov ecx, dword ptr [eax + 0x54]
 + 00b70fa9  81c1d0000000           add ecx, 0xd0
 + 00b70faf  8b11                   mov edx, dword ptr [ecx]
 + 00b70fb1  89542454               mov dword ptr [esp + 0x54], edx
 + 00b70fb5  8b5104                 mov edx, dword ptr [ecx + 4]
   00b70faf  89542458               mov dword ptr [esp + 0x58], edx
   00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
@@ retail 441-442 built 443-444
   00b70fad  8b10                   mov edx, dword ptr [eax]
   00b70faf  89542458               mov dword ptr [esp + 0x58], edx
 - 00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
 + 00b70fbc  8b5108                 mov edx, dword ptr [ecx + 8]
   00b70fb6  8954245c               mov dword ptr [esp + 0x5c], edx
   00b70fba  8b5008                 mov edx, dword ptr [eax + 8]
@@ retail 443-448 built 445-448
   00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
   00b70fb6  8954245c               mov dword ptr [esp + 0x5c], edx
 - 00b70fba  8b5008                 mov edx, dword ptr [eax + 8]
 - 00b70fbd  89542460               mov dword ptr [esp + 0x60], edx
 - 00b70fc1  8b400c                 mov eax, dword ptr [eax + 0xc]
 - 00b70fc4  89442464               mov dword ptr [esp + 0x64], eax
 - 00b70fc8  8b4954                 mov ecx, dword ptr [ecx + 0x54]
 + 00b70fc3  8b490c                 mov ecx, dword ptr [ecx + 0xc]
 + 00b70fc6  894c2460               mov dword ptr [esp + 0x60], ecx
 + 00b70fca  8b4854                 mov ecx, dword ptr [eax + 0x54]
   00b70fcb  8b91b4010000           mov edx, dword ptr [ecx + 0x1b4]
   00b70fd1  d822                   fsub dword ptr [edx]
@@ retail 450-452 built 450-450
   00b70fcb  8b91b4010000           mov edx, dword ptr [ecx + 0x1b4]
   00b70fd1  d822                   fsub dword ptr [edx]
 - 00b70fd3  d80d14002301           fmul dword ptr [0x1230014]
 - 00b70fd9  d80d24172301           fmul dword ptr [0x1231724]
   00b70fdf  d815dcde2201           fcom dword ptr [0x122dedc]
   00b70fe5  dfe0                   fnstsw ax
@@ retail 469-471 built 467-469
   00b71017  d88d04030000           fmul dword ptr [ebp + 0x304]
   00b7101d  8b95ac020000           mov edx, dword ptr [ebp + 0x2ac]
 - 00b71023  89442438               mov dword ptr [esp + 0x38], eax
 - 00b71027  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 + 00b71019  8944241c               mov dword ptr [esp + 0x1c], eax
 + 00b7101d  8b4c241c               mov ecx, dword ptr [esp + 0x1c]
   00b7102b  8b85b0020000           mov eax, dword ptr [ebp + 0x2b0]
   00b71031  d95c2444               fstp dword ptr [esp + 0x44]
@@ retail 472-486 built 470-484
   00b71027  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00b7102b  8b85b0020000           mov eax, dword ptr [ebp + 0x2b0]
 - 00b71031  d95c2444               fstp dword ptr [esp + 0x44]
 - 00b71035  894c2420               mov dword ptr [esp + 0x20], ecx
 - 00b71039  d9442420               fld dword ptr [esp + 0x20]
 - 00b7103d  8b4c2444               mov ecx, dword ptr [esp + 0x44]
 - 00b71041  894c242c               mov dword ptr [esp + 0x2c], ecx
 - 00b71045  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71049  89542424               mov dword ptr [esp + 0x24], edx
 - 00b7104d  8954243c               mov dword ptr [esp + 0x3c], edx
 - 00b71051  d95c2420               fstp dword ptr [esp + 0x20]
 - 00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
 - 00b71059  d9442424               fld dword ptr [esp + 0x24]
 - 00b7105d  89442428               mov dword ptr [esp + 0x28], eax
 - 00b71061  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71065  89442440               mov dword ptr [esp + 0x40], eax
 + 00b71027  d95c2428               fstp dword ptr [esp + 0x28]
 + 00b7102b  894c2434               mov dword ptr [esp + 0x34], ecx
 + 00b7102f  d9442434               fld dword ptr [esp + 0x34]
 + 00b71033  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 + 00b71037  894c2440               mov dword ptr [esp + 0x40], ecx
 + 00b7103b  d84c2440               fmul dword ptr [esp + 0x40]
 + 00b7103f  89542438               mov dword ptr [esp + 0x38], edx
 + 00b71043  89542420               mov dword ptr [esp + 0x20], edx
 + 00b71047  d95c2434               fstp dword ptr [esp + 0x34]
 + 00b7104b  8b4c2434               mov ecx, dword ptr [esp + 0x34]
 + 00b7104f  d9442438               fld dword ptr [esp + 0x38]
 + 00b71053  8944243c               mov dword ptr [esp + 0x3c], eax
 + 00b71057  d84c2440               fmul dword ptr [esp + 0x40]
 + 00b7105b  89442424               mov dword ptr [esp + 0x24], eax
   00b71069  a1146e4301             mov eax, dword ptr [0x1436e14]
   00b7106e  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
@@ retail 488-489 built 486-487
   00b71069  a1146e4301             mov eax, dword ptr [0x1436e14]
   00b7106e  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
 - 00b71074  d95c2424               fstp dword ptr [esp + 0x24]
 + 00b7106a  d95c2438               fstp dword ptr [esp + 0x38]
   00b71078  83c250                 add edx, 0x50
   00b7107b  d9442428               fld dword ptr [esp + 0x28]
@@ retail 490-491 built 488-489
   00b71074  d95c2424               fstp dword ptr [esp + 0x24]
   00b71078  83c250                 add edx, 0x50
 - 00b7107b  d9442428               fld dword ptr [esp + 0x28]
 + 00b71071  d944243c               fld dword ptr [esp + 0x3c]
   00b7107f  890a                   mov dword ptr [edx], ecx
   00b71081  d84c242c               fmul dword ptr [esp + 0x2c]
@@ retail 492-494 built 490-492
   00b7107b  d9442428               fld dword ptr [esp + 0x28]
   00b7107f  890a                   mov dword ptr [edx], ecx
 - 00b71081  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71085  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 + 00b71077  d84c2440               fmul dword ptr [esp + 0x40]
 + 00b7107b  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00b71089  894a04                 mov dword ptr [edx + 4], ecx
   00b7108c  d95c2428               fstp dword ptr [esp + 0x28]
@@ retail 495-497 built 493-495
   00b71085  8b4c2424               mov ecx, dword ptr [esp + 0x24]
   00b71089  894a04                 mov dword ptr [edx + 4], ecx
 - 00b7108c  d95c2428               fstp dword ptr [esp + 0x28]
 - 00b71090  8b4c2428               mov ecx, dword ptr [esp + 0x28]
 + 00b71082  d95c243c               fstp dword ptr [esp + 0x3c]
 + 00b71086  8b4c243c               mov ecx, dword ptr [esp + 0x3c]
   00b71094  d905d8de2201           fld dword ptr [0x122ded8]
   00b7109a  894a08                 mov dword ptr [edx + 8], ecx
@@ retail 499-502 built 497-500
   00b71094  d905d8de2201           fld dword ptr [0x122ded8]
   00b7109a  894a08                 mov dword ptr [edx + 8], ecx
 - 00b7109d  d864242c               fsub dword ptr [esp + 0x2c]
 - 00b710a1  d95c242c               fstp dword ptr [esp + 0x2c]
 - 00b710a5  8b4c242c               mov ecx, dword ptr [esp + 0x2c]
 + 00b71093  d8642440               fsub dword ptr [esp + 0x40]
 + 00b71097  d95c2440               fstp dword ptr [esp + 0x40]
 + 00b7109b  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b710a9  894a0c                 mov dword ptr [edx + 0xc], ecx
   00b710ac  8b98d8030000           mov ebx, dword ptr [eax + 0x3d8]
@@ retail 504-504 built 502-504
   00b710a9  894a0c                 mov dword ptr [edx + 0xc], ecx
   00b710ac  8b98d8030000           mov ebx, dword ptr [eax + 0x3d8]
 + 00b710a8  d9542434               fst dword ptr [esp + 0x34]
 + 00b710ac  8b4c2434               mov ecx, dword ptr [esp + 0x34]
   00b710b2  d9542438               fst dword ptr [esp + 0x38]
   00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
@@ retail 505-506 built 505-506
   00b710ac  8b98d8030000           mov ebx, dword ptr [eax + 0x3d8]
   00b710b2  d9542438               fst dword ptr [esp + 0x38]
 - 00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 + 00b710b4  0bdf                   or ebx, edi
   00b710ba  d954243c               fst dword ptr [esp + 0x3c]
   00b710be  0bdf                   or ebx, edi
@@ retail 507-509 built 507-507
   00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00b710ba  d954243c               fst dword ptr [esp + 0x3c]
 - 00b710be  0bdf                   or ebx, edi
 - 00b710c0  d9542440               fst dword ptr [esp + 0x40]
   00b710c4  8998d8030000           mov dword ptr [eax + 0x3d8], ebx
   00b710ca  d95c2444               fstp dword ptr [esp + 0x44]
@@ retail 510-511 built 508-509
   00b710c0  d9542440               fst dword ptr [esp + 0x40]
   00b710c4  8998d8030000           mov dword ptr [eax + 0x3d8], ebx
 - 00b710ca  d95c2444               fstp dword ptr [esp + 0x44]
 + 00b710c0  d95c2440               fstp dword ptr [esp + 0x40]
   00b710ce  a1146e4301             mov eax, dword ptr [0x1436e14]
   00b710d3  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
@@ retail 515-515 built 513-515
   00b710d9  83c260                 add edx, 0x60
   00b710dc  890a                   mov dword ptr [edx], ecx
 + 00b710d4  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 + 00b710d8  894a04                 mov dword ptr [edx + 4], ecx
   00b710de  8b4c243c               mov ecx, dword ptr [esp + 0x3c]
   00b710e2  894a04                 mov dword ptr [edx + 4], ecx
@@ retail 516-517 built 516-517
   00b710dc  890a                   mov dword ptr [edx], ecx
   00b710de  8b4c243c               mov ecx, dword ptr [esp + 0x3c]
 - 00b710e2  894a04                 mov dword ptr [edx + 4], ecx
 + 00b710df  894a08                 mov dword ptr [edx + 8], ecx
   00b710e5  8b4c2440               mov ecx, dword ptr [esp + 0x40]
   00b710e9  894a08                 mov dword ptr [edx + 8], ecx
@@ retail 518-520 built 518-518
   00b710e2  894a04                 mov dword ptr [edx + 4], ecx
   00b710e5  8b4c2440               mov ecx, dword ptr [esp + 0x40]
 - 00b710e9  894a08                 mov dword ptr [edx + 8], ecx
 - 00b710ec  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b710f0  894a0c                 mov dword ptr [edx + 0xc], ecx
   00b710f3  09b8d8030000           or dword ptr [eax + 0x3d8], edi
@@ retail 523-523 built 521-522
   00b710f3  09b8d8030000           or dword ptr [eax + 0x3d8], edi
   00b710f9  a1a06e4301             mov eax, dword ptr [0x1436ea0]
 + 00b710f4  d980b4010000           fld dword ptr [eax + 0x1b4]
   00b710fe  8b90bc010000           mov edx, dword ptr [eax + 0x1bc]
   00b71104  d980b8010000           fld dword ptr [eax + 0x1b8]
@@ retail 525-530 built 524-529
   00b710fe  8b90bc010000           mov edx, dword ptr [eax + 0x1bc]
   00b71104  d980b8010000           fld dword ptr [eax + 0x1b8]
 - 00b7110a  8b80b4010000           mov eax, dword ptr [eax + 0x1b4]
 - 00b71110  d95c2434               fstp dword ptr [esp + 0x34]
 - 00b71114  8d4c2430               lea ecx, [esp + 0x30]
 - 00b71118  89542418               mov dword ptr [esp + 0x18], edx
 - 00b7111c  89442430               mov dword ptr [esp + 0x30], eax
 + 00b71106  8d4c242c               lea ecx, [esp + 0x2c]
 + 00b7110a  d9c9                   fxch st(1)
 + 00b7110c  8954243c               mov dword ptr [esp + 0x3c], edx
 + 00b71110  d95c242c               fstp dword ptr [esp + 0x2c]
 + 00b71114  d95c2430               fstp dword ptr [esp + 0x30]
   00b71120  e8eb33eaff             call 0xa14510
   00b71125  8b35a06e4301           mov esi, dword ptr [0x1436ea0]
@@ retail 534-536 built 533-535
   00b7112b  d94660                 fld dword ptr [esi + 0x60]
   00b7112e  d80d087f2301           fmul dword ptr [0x1237f08]
 - 00b71134  d95c241c               fstp dword ptr [esp + 0x1c]
 - 00b71138  d9442434               fld dword ptr [esp + 0x34]
 + 00b7112c  d95c2418               fstp dword ptr [esp + 0x18]
 + 00b71130  d9442430               fld dword ptr [esp + 0x30]
   00b7113c  e861da0800             call 0xbfeba2
   00b71141  d80d087f2301           fmul dword ptr [0x1237f08]
@@ retail 538-539 built 537-538
   00b7113c  e861da0800             call 0xbfeba2
   00b71141  d80d087f2301           fmul dword ptr [0x1237f08]
 - 00b71147  d9442430               fld dword ptr [esp + 0x30]
 + 00b7113f  d944242c               fld dword ptr [esp + 0x2c]
   00b7114b  d81ddcde2201           fcomp dword ptr [0x122dedc]
   00b71151  dfe0                   fnstsw ax
@@ retail 550-551 built 549-550
   00b7116c  d80d087f2301           fmul dword ptr [0x1237f08]
   00b71172  d8e9                   fsubr st(1)
 - 00b71174  d95c2420               fstp dword ptr [esp + 0x20]
 + 00b7116c  d95c241c               fstp dword ptr [esp + 0x1c]
   00b71178  ddd8                   fstp st(0)
   00b7117a  7409                   je 0xb71185
@@ retail 553-555 built 552-554
   00b71178  ddd8                   fstp st(0)
   00b7117a  7409                   je 0xb71185
 - 00b7117c  8b4e64                 mov ecx, dword ptr [esi + 0x64]
 - 00b7117f  894c2414               mov dword ptr [esp + 0x14], ecx
 + 00b71174  8b4664                 mov eax, dword ptr [esi + 0x64]
 + 00b71177  89442414               mov dword ptr [esp + 0x14], eax
   00b71183  eb13                   jmp 0xb71198
   00b71185  d905d8de2201           fld dword ptr [0x122ded8]
@@ retail 556-557 built 555-556
   00b7117f  894c2414               mov dword ptr [esp + 0x14], ecx
   00b71183  eb13                   jmp 0xb71198
 - 00b71185  d905d8de2201           fld dword ptr [0x122ded8]
 + 00b7117d  d94660                 fld dword ptr [esi + 0x60]
   00b7118b  d8b6d0000000           fdiv dword ptr [esi + 0xd0]
   00b71191  d84e60                 fmul dword ptr [esi + 0x60]
@@ retail 558-559 built 557-557
   00b71185  d905d8de2201           fld dword ptr [0x122ded8]
   00b7118b  d8b6d0000000           fdiv dword ptr [esi + 0xd0]
 - 00b71191  d84e60                 fmul dword ptr [esi + 0x60]
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 560-560 built 558-560
   00b71191  d84e60                 fmul dword ptr [esi + 0x60]
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
 + 00b7118a  8d4c2444               lea ecx, [esp + 0x44]
 + 00b7118e  51                     push ecx
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00b7119e  8d542448               lea edx, [esp + 0x48]
@@ retail 561-563 built 561-561
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b7119e  8d542448               lea edx, [esp + 0x48]
 - 00b711a2  52                     push edx
   00b711a3  6a00                   push 0
   00b711a5  e8a689e1ff             call 0x989b50
@@ retail 566-568 built 564-566
   00b711a5  e8a689e1ff             call 0x989b50
   00b711aa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b711b0  8d442458               lea eax, [esp + 0x58]
 - 00b711b4  50                     push eax
 + 00b711a2  8d542454               lea edx, [esp + 0x54]
 + 00b711a6  52                     push edx
   00b711b5  bb01000000             mov ebx, 1
   00b711ba  53                     push ebx
@@ retail 571-572 built 569-570
   00b711ba  53                     push ebx
   00b711bb  e89089e1ff             call 0x989b50
 - 00b711c0  d9442418               fld dword ptr [esp + 0x18]
 + 00b711b2  d944243c               fld dword ptr [esp + 0x3c]
   00b711c4  6a00                   push 0
   00b711c6  e8d7d90800             call 0xbfeba2
@@ retail 576-577 built 574-575
   00b711cb  d82d2c332a01           fsubr dword ptr [0x12a332c]
   00b711d1  d9442418               fld dword ptr [esp + 0x18]
 - 00b711d5  8b542424               mov edx, dword ptr [esp + 0x24]
 + 00b711c7  8b44241c               mov eax, dword ptr [esp + 0x1c]
   00b711d9  d80d9cf52201           fmul dword ptr [0x122f59c]
   00b711df  51                     push ecx
@@ retail 586-586 built 584-585
   00b711f0  d80524332a01           fadd dword ptr [0x12a3324]
   00b711f6  d91c24                 fstp dword ptr [esp]
 + 00b711eb  50                     push eax
   00b711f9  51                     push ecx
   00b711fa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 588-589 built 587-587
   00b711f9  51                     push ecx
   00b711fa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b71200  52                     push edx
   00b71201  6a02                   push 2
   00b71203  e8f888e1ff             call 0x989b00
@@ retail 618-619 built 616-617
   00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b7126a  8b3d08000000           mov edi, dword ptr [8]
   00b7127e  8b90882f0000           mov edx, dword ptr [eax + 0x2f88]
   00b71284  3bd7                   cmp edx, edi
@@ retail 670-671 built 668-669
   00b71343  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71349  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7134e  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71340  8b3d08000000           mov edi, dword ptr [8]
   00b71354  8b909c2f0000           mov edx, dword ptr [eax + 0x2f9c]
   00b7135a  3bd7                   cmp edx, edi
@@ retail 766-767 built 764-765
   00b71498  e8836be1ff             call 0x988020
   00b7149d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b714a2  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00b71494  8b3510000000           mov esi, dword ptr [0x10]
   00b714a8  8ba820290000           mov ebp, dword ptr [eax + 0x2920]
   00b714ae  3bee                   cmp ebp, esi
@@ retail 794-795 built 792-793
   00b71506  ff820c280000           inc dword ptr [edx + 0x280c]
   00b7150c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71511  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00b71503  8b3514000000           mov esi, dword ptr [0x14]
   00b71517  8ba834290000           mov ebp, dword ptr [eax + 0x2934]
   00b7151d  3bee                   cmp ebp, esi
@@ retail 801-801 built 799-800
   00b71527  8bfe                   mov edi, esi
   00b71529  7455                   je 0xb71580
 + 00b7151d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
   00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 802-803 built 801-801
   00b71529  7455                   je 0xb71580
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71534  85c6                   test esi, eax
   00b71536  7522                   jne 0xb7155a
@@ retail 827-827 built 825-826
   00b7158e  8bc8                   mov ecx, eax
   00b71590  7450                   je 0xb715e2
 + 00b71584  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
   00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 828-829 built 827-827
   00b71590  7450                   je 0xb715e2
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7159b  85c6                   test esi, eax
   00b7159d  7522                   jne 0xb715c1
@@ retail 870-871 built 868-869
   00b71628  899fb83c0000           mov dword ptr [edi + 0x3cb8], ebx
   00b7162e  55                     push ebp
 - 00b7162f  8d442424               lea eax, [esp + 0x24]
 + 00b71621  8d442420               lea eax, [esp + 0x20]
   00b71633  50                     push eax
   00b71634  8bce                   mov ecx, esi
@@ retail 876-877 built 874-875
   00b7163b  8b38                   mov edi, dword ptr [eax]
   00b7163d  55                     push ebp
 - 00b7163e  8d4c2424               lea ecx, [esp + 0x24]
 + 00b71630  8d4c2420               lea ecx, [esp + 0x20]
   00b71642  51                     push ecx
   00b71643  8bce                   mov ecx, esi
@@ retail 890-892 built 888-890
   00b7165d  85f6                   test esi, esi
   00b7165f  55                     push ebp
 - 00b71660  89742424               mov dword ptr [esp + 0x24], esi
 - 00b71664  db442424               fild dword ptr [esp + 0x24]
 + 00b71652  89742420               mov dword ptr [esp + 0x20], esi
 + 00b71656  db442420               fild dword ptr [esp + 0x20]
   00b71668  55                     push ebp
   00b71669  7d06                   jge 0xb71671
@@ retail 898-900 built 896-898
   00b71674  85ff                   test edi, edi
   00b71676  d95c2404               fstp dword ptr [esp + 4]
 - 00b7167a  897c2430               mov dword ptr [esp + 0x30], edi
 - 00b7167e  db442430               fild dword ptr [esp + 0x30]
 + 00b7166c  897c242c               mov dword ptr [esp + 0x2c], edi
 + 00b71670  db44242c               fild dword ptr [esp + 0x2c]
   00b71682  7d06                   jge 0xb7168a
   00b71684  d805b4dc2201           fadd dword ptr [0x122dcb4]
@@ retail 907-908 built 905-906
   00b71694  e86784e1ff             call 0x989b00
   00b71699  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00b7169f  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b71691  8b1508000000           mov edx, dword ptr [8]
   00b716a5  8bb1e82e0000           mov esi, dword ptr [ecx + 0x2ee8]
   00b716ab  3bf2                   cmp esi, edx
@@ retail 933-934 built 931-932
   00b716fa  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00b71701  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00b71707  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b716f9  8b1508000000           mov edx, dword ptr [8]
   00b7170d  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71712  8bb0882f0000           mov esi, dword ptr [eax + 0x2f88]
@@ retail 940-942 built 938-940
   00b71720  8bfa                   mov edi, edx
   00b71722  7450                   je 0xb71774
 - 00b71724  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b71727  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 + 00b71716  8b5108                 mov edx, dword ptr [ecx + 8]
 + 00b71719  8bb014280000           mov esi, dword ptr [eax + 0x2814]
   00b7172d  85d6                   test esi, edx
   00b7172f  7522                   jne 0xb71753
@@ retail 1049-1050 built 1047-1048
   00b71879  5f                     pop edi
   00b7187a  5b                     pop ebx
 - 00b7187b  83c458                 add esp, 0x58
 + 00b7186d  83c454                 add esp, 0x54
   00b7187e  c3                     ret 
DIFFLINES 110 retail_instr 1051 built_instr 1049
