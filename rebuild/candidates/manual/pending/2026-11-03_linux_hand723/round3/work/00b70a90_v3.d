00b70a90 CEngineWaterRenderer::RenderEnvironmentMap DIFFER(3545v3567) flags= []
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
@@ retail 426-440 built 426-440
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
 + 00b70f7e  8b442418               mov eax, dword ptr [esp + 0x18]
 + 00b70f82  8b5054                 mov edx, dword ptr [eax + 0x54]
 + 00b70f85  81c2c0000000           add edx, 0xc0
 + 00b70f8b  8b0a                   mov ecx, dword ptr [edx]
 + 00b70f8d  894c2448               mov dword ptr [esp + 0x48], ecx
 + 00b70f91  8b4a04                 mov ecx, dword ptr [edx + 4]
 + 00b70f94  894c244c               mov dword ptr [esp + 0x4c], ecx
 + 00b70f98  8b4a08                 mov ecx, dword ptr [edx + 8]
 + 00b70f9b  894c2450               mov dword ptr [esp + 0x50], ecx
 + 00b70f9f  8b520c                 mov edx, dword ptr [edx + 0xc]
 + 00b70fa2  89542454               mov dword ptr [esp + 0x54], edx
 + 00b70fa6  8b4854                 mov ecx, dword ptr [eax + 0x54]
 + 00b70fa9  81c1d0000000           add ecx, 0xd0
 + 00b70faf  8b11                   mov edx, dword ptr [ecx]
   00b70faf  89542458               mov dword ptr [esp + 0x58], edx
   00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
@@ retail 441-442 built 441-442
   00b70fad  8b10                   mov edx, dword ptr [eax]
   00b70faf  89542458               mov dword ptr [esp + 0x58], edx
 - 00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
 + 00b70fb5  8b5104                 mov edx, dword ptr [ecx + 4]
   00b70fb6  8954245c               mov dword ptr [esp + 0x5c], edx
   00b70fba  8b5008                 mov edx, dword ptr [eax + 8]
@@ retail 443-444 built 443-444
   00b70fb3  8b5004                 mov edx, dword ptr [eax + 4]
   00b70fb6  8954245c               mov dword ptr [esp + 0x5c], edx
 - 00b70fba  8b5008                 mov edx, dword ptr [eax + 8]
 + 00b70fbc  8b5108                 mov edx, dword ptr [ecx + 8]
   00b70fbd  89542460               mov dword ptr [esp + 0x60], edx
   00b70fc1  8b400c                 mov eax, dword ptr [eax + 0xc]
@@ retail 445-448 built 445-448
   00b70fba  8b5008                 mov edx, dword ptr [eax + 8]
   00b70fbd  89542460               mov dword ptr [esp + 0x60], edx
 - 00b70fc1  8b400c                 mov eax, dword ptr [eax + 0xc]
 - 00b70fc4  89442464               mov dword ptr [esp + 0x64], eax
 - 00b70fc8  8b4954                 mov ecx, dword ptr [ecx + 0x54]
 + 00b70fc3  8b490c                 mov ecx, dword ptr [ecx + 0xc]
 + 00b70fc6  894c2464               mov dword ptr [esp + 0x64], ecx
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
@@ retail 548-549 built 546-546
   00b71164  84c0                   test al, al
   00b71166  d80d9cf52201           fmul dword ptr [0x122f59c]
 - 00b7116c  d80d087f2301           fmul dword ptr [0x1237f08]
   00b71172  d8e9                   fsubr st(1)
   00b71174  d95c2420               fstp dword ptr [esp + 0x20]
@@ retail 556-557 built 553-554
   00b7117f  894c2414               mov dword ptr [esp + 0x14], ecx
   00b71183  eb13                   jmp 0xb71198
 - 00b71185  d905d8de2201           fld dword ptr [0x122ded8]
 + 00b71175  d94660                 fld dword ptr [esi + 0x60]
   00b7118b  d8b6d0000000           fdiv dword ptr [esi + 0xd0]
   00b71191  d84e60                 fmul dword ptr [esi + 0x60]
@@ retail 558-559 built 555-555
   00b71185  d905d8de2201           fld dword ptr [0x122ded8]
   00b7118b  d8b6d0000000           fdiv dword ptr [esi + 0xd0]
 - 00b71191  d84e60                 fmul dword ptr [esi + 0x60]
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 618-619 built 614-615
   00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71262  8b3d08000000           mov edi, dword ptr [8]
   00b7127e  8b90882f0000           mov edx, dword ptr [eax + 0x2f88]
   00b71284  3bd7                   cmp edx, edi
@@ retail 670-671 built 666-667
   00b71343  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71349  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7134e  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71338  8b3d08000000           mov edi, dword ptr [8]
   00b71354  8b909c2f0000           mov edx, dword ptr [eax + 0x2f9c]
   00b7135a  3bd7                   cmp edx, edi
@@ retail 766-767 built 762-763
   00b71498  e8836be1ff             call 0x988020
   00b7149d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b714a2  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00b7148c  8b3510000000           mov esi, dword ptr [0x10]
   00b714a8  8ba820290000           mov ebp, dword ptr [eax + 0x2920]
   00b714ae  3bee                   cmp ebp, esi
@@ retail 794-795 built 790-791
   00b71506  ff820c280000           inc dword ptr [edx + 0x280c]
   00b7150c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71511  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00b714fb  8b3514000000           mov esi, dword ptr [0x14]
   00b71517  8ba834290000           mov ebp, dword ptr [eax + 0x2934]
   00b7151d  3bee                   cmp ebp, esi
@@ retail 801-801 built 797-798
   00b71527  8bfe                   mov edi, esi
   00b71529  7455                   je 0xb71580
 + 00b71515  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
   00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 802-803 built 799-799
   00b71529  7455                   je 0xb71580
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71534  85c6                   test esi, eax
   00b71536  7522                   jne 0xb7155a
@@ retail 827-827 built 823-824
   00b7158e  8bc8                   mov ecx, eax
   00b71590  7450                   je 0xb715e2
 + 00b7157c  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
   00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 828-829 built 825-825
   00b71590  7450                   je 0xb715e2
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7159b  85c6                   test esi, eax
   00b7159d  7522                   jne 0xb715c1
@@ retail 907-908 built 903-904
   00b71694  e86784e1ff             call 0x989b00
   00b71699  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00b7169f  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b71689  8b1508000000           mov edx, dword ptr [8]
   00b716a5  8bb1e82e0000           mov esi, dword ptr [ecx + 0x2ee8]
   00b716ab  3bf2                   cmp esi, edx
@@ retail 913-913 built 909-910
   00b716b3  8bfa                   mov edi, edx
   00b716b5  7456                   je 0xb7170d
 + 00b716a1  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
   00b716b7  8b5008                 mov edx, dword ptr [eax + 8]
   00b716ba  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
@@ retail 914-915 built 911-911
   00b716b5  7456                   je 0xb7170d
   00b716b7  8b5008                 mov edx, dword ptr [eax + 8]
 - 00b716ba  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
   00b716c0  85d6                   test esi, edx
   00b716c2  7522                   jne 0xb716e6
@@ retail 933-934 built 929-930
   00b716fa  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00b71701  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00b71707  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b716f1  8b1508000000           mov edx, dword ptr [8]
   00b7170d  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71712  8bb0882f0000           mov esi, dword ptr [eax + 0x2f88]
@@ retail 940-942 built 936-938
   00b71720  8bfa                   mov edi, edx
   00b71722  7450                   je 0xb71774
 - 00b71724  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b71727  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 + 00b7170e  8bb014280000           mov esi, dword ptr [eax + 0x2814]
 + 00b71714  8b5108                 mov edx, dword ptr [ecx + 8]
   00b7172d  85d6                   test esi, edx
   00b7172f  7522                   jne 0xb71753
DIFFLINES 43 retail_instr 1051 built_instr 1047
