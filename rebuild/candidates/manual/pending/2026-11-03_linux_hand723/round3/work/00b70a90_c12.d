00b70a90 CEngineWaterRenderer::RenderEnvironmentMap DIFFER(3577v3567) flags= []
@@ retail 425-425 built 425-455
   00b70f6d  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00b70f73  e8a870e1ff             call 0x988020
 + 00b70f78  8b442418               mov eax, dword ptr [esp + 0x18]
 + 00b70f7c  8b5054                 mov edx, dword ptr [eax + 0x54]
 + 00b70f7f  81c2c0000000           add edx, 0xc0
 + 00b70f85  8b0a                   mov ecx, dword ptr [edx]
 + 00b70f87  894c2448               mov dword ptr [esp + 0x48], ecx
 + 00b70f8b  8b4a04                 mov ecx, dword ptr [edx + 4]
 + 00b70f8e  894c244c               mov dword ptr [esp + 0x4c], ecx
 + 00b70f92  8b4a08                 mov ecx, dword ptr [edx + 8]
 + 00b70f95  894c2450               mov dword ptr [esp + 0x50], ecx
 + 00b70f99  8b520c                 mov edx, dword ptr [edx + 0xc]
 + 00b70f9c  89542454               mov dword ptr [esp + 0x54], edx
 + 00b70fa0  8b4854                 mov ecx, dword ptr [eax + 0x54]
 + 00b70fa3  81c1d0000000           add ecx, 0xd0
 + 00b70fa9  8b11                   mov edx, dword ptr [ecx]
 + 00b70fab  89542458               mov dword ptr [esp + 0x58], edx
 + 00b70faf  8b5104                 mov edx, dword ptr [ecx + 4]
 + 00b70fb2  8954245c               mov dword ptr [esp + 0x5c], edx
 + 00b70fb6  8b5108                 mov edx, dword ptr [ecx + 8]
 + 00b70fb9  89542460               mov dword ptr [esp + 0x60], edx
 + 00b70fbd  8b490c                 mov ecx, dword ptr [ecx + 0xc]
 + 00b70fc0  894c2464               mov dword ptr [esp + 0x64], ecx
 + 00b70fc4  8b4054                 mov eax, dword ptr [eax + 0x54]
 + 00b70fc7  d98038010000           fld dword ptr [eax + 0x138]
 + 00b70fcd  8b80b4010000           mov eax, dword ptr [eax + 0x1b4]
 + 00b70fd3  d88d04030000           fmul dword ptr [ebp + 0x304]
 + 00b70fd9  8b95a8020000           mov edx, dword ptr [ebp + 0x2a8]
 + 00b70fdf  8b8dac020000           mov ecx, dword ptr [ebp + 0x2ac]
 + 00b70fe5  89542438               mov dword ptr [esp + 0x38], edx
 + 00b70fe9  8b95b0020000           mov edx, dword ptr [ebp + 0x2b0]
 + 00b70fef  d95c2444               fstp dword ptr [esp + 0x44]
   00b70f78  d905d8de2201           fld dword ptr [0x122ded8]
   00b70f7e  8b4c2418               mov ecx, dword ptr [esp + 0x18]
@@ retail 426-450 built 456-459
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
 + 00b70ff9  894c243c               mov dword ptr [esp + 0x3c], ecx
 + 00b70ffd  d820                   fsub dword ptr [eax]
 + 00b70fff  89542440               mov dword ptr [esp + 0x40], edx
   00b70fd3  d80d14002301           fmul dword ptr [0x1230014]
   00b70fd9  d80d24172301           fmul dword ptr [0x1231724]
@@ retail 465-470 built 474-474
   00b71003  ddd8                   fstp st(0)
   00b71005  d905d8de2201           fld dword ptr [0x122ded8]
 - 00b7100b  d98138010000           fld dword ptr [ecx + 0x138]
 - 00b71011  8b85a8020000           mov eax, dword ptr [ebp + 0x2a8]
 - 00b71017  d88d04030000           fmul dword ptr [ebp + 0x304]
 - 00b7101d  8b95ac020000           mov edx, dword ptr [ebp + 0x2ac]
 - 00b71023  89442438               mov dword ptr [esp + 0x38], eax
   00b71027  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00b7102b  8b85b0020000           mov eax, dword ptr [ebp + 0x2b0]
@@ retail 471-473 built 475-477
   00b71023  89442438               mov dword ptr [esp + 0x38], eax
   00b71027  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 - 00b7102b  8b85b0020000           mov eax, dword ptr [ebp + 0x2b0]
 - 00b71031  d95c2444               fstp dword ptr [esp + 0x44]
 + 00b7103f  8b54243c               mov edx, dword ptr [esp + 0x3c]
 + 00b71043  8b442440               mov eax, dword ptr [esp + 0x40]
   00b71035  894c2420               mov dword ptr [esp + 0x20], ecx
   00b71039  d9442420               fld dword ptr [esp + 0x20]
@@ retail 479-480 built 483-485
   00b71045  d84c242c               fmul dword ptr [esp + 0x2c]
   00b71049  89542424               mov dword ptr [esp + 0x24], edx
 - 00b7104d  8954243c               mov dword ptr [esp + 0x3c], edx
 + 00b7105f  89442428               mov dword ptr [esp + 0x28], eax
 + 00b71063  a100000000             mov eax, dword ptr [0]
   00b71051  d95c2420               fstp dword ptr [esp + 0x20]
   00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
@@ retail 481-481 built 486-489
   00b7104d  8954243c               mov dword ptr [esp + 0x3c], edx
   00b71051  d95c2420               fstp dword ptr [esp + 0x20]
 + 00b7106c  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
 + 00b71072  d9442424               fld dword ptr [esp + 0x24]
 + 00b71076  d84c242c               fmul dword ptr [esp + 0x2c]
   00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
   00b71059  d9442424               fld dword ptr [esp + 0x24]
@@ retail 482-484 built 490-496
   00b71051  d95c2420               fstp dword ptr [esp + 0x20]
   00b71055  8b4c2420               mov ecx, dword ptr [esp + 0x20]
 - 00b71059  d9442424               fld dword ptr [esp + 0x24]
 - 00b7105d  89442428               mov dword ptr [esp + 0x28], eax
 + 00b7107e  83c250                 add edx, 0x50
 + 00b71081  890a                   mov dword ptr [edx], ecx
 + 00b71083  d95c2424               fstp dword ptr [esp + 0x24]
 + 00b71087  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 + 00b7108b  d9442428               fld dword ptr [esp + 0x28]
 + 00b7108f  894a04                 mov dword ptr [edx + 4], ecx
   00b71061  d84c242c               fmul dword ptr [esp + 0x2c]
   00b71065  89442440               mov dword ptr [esp + 0x40], eax
@@ retail 485-495 built 497-497
   00b7105d  89442428               mov dword ptr [esp + 0x28], eax
   00b71061  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71065  89442440               mov dword ptr [esp + 0x40], eax
 - 00b71069  a1146e4301             mov eax, dword ptr [0x1436e14]
 - 00b7106e  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
 - 00b71074  d95c2424               fstp dword ptr [esp + 0x24]
 - 00b71078  83c250                 add edx, 0x50
 - 00b7107b  d9442428               fld dword ptr [esp + 0x28]
 - 00b7107f  890a                   mov dword ptr [edx], ecx
 - 00b71081  d84c242c               fmul dword ptr [esp + 0x2c]
 - 00b71085  8b4c2424               mov ecx, dword ptr [esp + 0x24]
 - 00b71089  894a04                 mov dword ptr [edx + 4], ecx
   00b7108c  d95c2428               fstp dword ptr [esp + 0x28]
   00b71090  8b4c2428               mov ecx, dword ptr [esp + 0x28]
@@ retail 618-619 built 620-621
   00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71282  8b3d08000000           mov edi, dword ptr [8]
   00b7127e  8b90882f0000           mov edx, dword ptr [eax + 0x2f88]
   00b71284  3bd7                   cmp edx, edi
@@ retail 670-671 built 672-673
   00b71343  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71349  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7134e  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71358  8b3d08000000           mov edi, dword ptr [8]
   00b71354  8b909c2f0000           mov edx, dword ptr [eax + 0x2f9c]
   00b7135a  3bd7                   cmp edx, edi
@@ retail 766-767 built 768-769
   00b71498  e8836be1ff             call 0x988020
   00b7149d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b714a2  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00b714ac  8b3510000000           mov esi, dword ptr [0x10]
   00b714a8  8ba820290000           mov ebp, dword ptr [eax + 0x2920]
   00b714ae  3bee                   cmp ebp, esi
@@ retail 794-795 built 796-797
   00b71506  ff820c280000           inc dword ptr [edx + 0x280c]
   00b7150c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71511  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00b7151b  8b3514000000           mov esi, dword ptr [0x14]
   00b71517  8ba834290000           mov ebp, dword ptr [eax + 0x2934]
   00b7151d  3bee                   cmp ebp, esi
@@ retail 801-801 built 803-804
   00b71527  8bfe                   mov edi, esi
   00b71529  7455                   je 0xb71580
 + 00b71535  8b8014280000           mov eax, dword ptr [eax + 0x2814]
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
 + 00b7159c  8b8014280000           mov eax, dword ptr [eax + 0x2814]
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
 + 00b716a9  8b1508000000           mov edx, dword ptr [8]
   00b716a5  8bb1e82e0000           mov esi, dword ptr [ecx + 0x2ee8]
   00b716ab  3bf2                   cmp esi, edx
@@ retail 913-913 built 915-916
   00b716b3  8bfa                   mov edi, edx
   00b716b5  7456                   je 0xb7170d
 + 00b716c1  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
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
 + 00b71711  8b1508000000           mov edx, dword ptr [8]
   00b7170d  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71712  8bb0882f0000           mov esi, dword ptr [eax + 0x2f88]
@@ retail 940-942 built 942-944
   00b71720  8bfa                   mov edi, edx
   00b71722  7450                   je 0xb71774
 - 00b71724  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b71727  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 + 00b7172e  8bb014280000           mov esi, dword ptr [eax + 0x2814]
 + 00b71734  8b5108                 mov edx, dword ptr [ecx + 8]
   00b7172d  85d6                   test esi, edx
   00b7172f  7522                   jne 0xb71753
DIFFLINES 96 retail_instr 1051 built_instr 1053
