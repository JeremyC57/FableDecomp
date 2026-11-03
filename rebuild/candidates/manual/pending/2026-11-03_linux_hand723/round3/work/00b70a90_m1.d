00b70a90 CEngineWaterRenderer::RenderEnvironmentMap DIFFER(3569v3567) flags= []
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
@@ retail 618-619 built 618-619
   00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b7127a  8b3d08000000           mov edi, dword ptr [8]
   00b7127e  8b90882f0000           mov edx, dword ptr [eax + 0x2f88]
   00b71284  3bd7                   cmp edx, edi
@@ retail 670-671 built 670-671
   00b71343  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71349  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7134e  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71350  8b3d08000000           mov edi, dword ptr [8]
   00b71354  8b909c2f0000           mov edx, dword ptr [eax + 0x2f9c]
   00b7135a  3bd7                   cmp edx, edi
@@ retail 675-677 built 675-677
   00b7135c  8d88982f0000           lea ecx, [eax + 0x2f98]
   00b71362  7450                   je 0xb713b4
 - 00b71364  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 - 00b7136a  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00b71366  8bb014280000           mov esi, dword ptr [eax + 0x2814]
 + 00b7136c  8b5108                 mov edx, dword ptr [ecx + 8]
   00b7136d  85d6                   test esi, edx
   00b7136f  7522                   jne 0xb71393
@@ retail 766-767 built 766-767
   00b71498  e8836be1ff             call 0x988020
   00b7149d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b714a2  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00b714a4  8b3510000000           mov esi, dword ptr [0x10]
   00b714a8  8ba820290000           mov ebp, dword ptr [eax + 0x2920]
   00b714ae  3bee                   cmp ebp, esi
@@ retail 775-776 built 775-776
   00b714bc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b714c2  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b714c5  85c6                   test esi, eax
 + 00b714c7  85f0                   test eax, esi
   00b714c7  7522                   jne 0xb714eb
   00b714c9  0bc6                   or eax, esi
@@ retail 777-779 built 777-779
   00b714c5  85c6                   test esi, eax
   00b714c7  7522                   jne 0xb714eb
 - 00b714c9  0bc6                   or eax, esi
 - 00b714cb  894108                 mov dword ptr [ecx + 8], eax
 + 00b714cb  0bf0                   or esi, eax
 + 00b714cd  897108                 mov dword ptr [ecx + 8], esi
   00b714ce  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00b714d4  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
@@ retail 794-795 built 794-795
   00b71506  ff820c280000           inc dword ptr [edx + 0x280c]
   00b7150c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71511  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00b71513  8b3514000000           mov esi, dword ptr [0x14]
   00b71517  8ba834290000           mov ebp, dword ptr [eax + 0x2934]
   00b7151d  3bee                   cmp ebp, esi
@@ retail 801-801 built 801-802
   00b71527  8bfe                   mov edi, esi
   00b71529  7455                   je 0xb71580
 + 00b7152d  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
   00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 802-804 built 803-804
   00b71529  7455                   je 0xb71580
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00b71534  85c6                   test esi, eax
 + 00b71536  85f0                   test eax, esi
   00b71536  7522                   jne 0xb7155a
   00b71538  0bc6                   or eax, esi
@@ retail 805-807 built 805-807
   00b71534  85c6                   test esi, eax
   00b71536  7522                   jne 0xb7155a
 - 00b71538  0bc6                   or eax, esi
 - 00b7153a  894208                 mov dword ptr [edx + 8], eax
 + 00b7153a  0bf0                   or esi, eax
 + 00b7153c  897208                 mov dword ptr [edx + 8], esi
   00b7153d  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b71543  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 827-827 built 827-828
   00b7158e  8bc8                   mov ecx, eax
   00b71590  7450                   je 0xb715e2
 + 00b71594  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
   00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 828-830 built 829-830
   00b71590  7450                   je 0xb715e2
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
 - 00b7159b  85c6                   test esi, eax
 + 00b7159d  85f0                   test eax, esi
   00b7159d  7522                   jne 0xb715c1
   00b7159f  0bc6                   or eax, esi
@@ retail 831-833 built 831-833
   00b7159b  85c6                   test esi, eax
   00b7159d  7522                   jne 0xb715c1
 - 00b7159f  0bc6                   or eax, esi
 - 00b715a1  894208                 mov dword ptr [edx + 8], eax
 + 00b715a1  0bf0                   or esi, eax
 + 00b715a3  897208                 mov dword ptr [edx + 8], esi
   00b715a4  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b715aa  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 907-908 built 907-908
   00b71694  e86784e1ff             call 0x989b00
   00b71699  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00b7169f  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b716a1  8b1508000000           mov edx, dword ptr [8]
   00b716a5  8bb1e82e0000           mov esi, dword ptr [ecx + 0x2ee8]
   00b716ab  3bf2                   cmp esi, edx
@@ retail 913-913 built 913-914
   00b716b3  8bfa                   mov edi, edx
   00b716b5  7456                   je 0xb7170d
 + 00b716b9  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
   00b716b7  8b5008                 mov edx, dword ptr [eax + 8]
   00b716ba  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
@@ retail 914-915 built 915-915
   00b716b5  7456                   je 0xb7170d
   00b716b7  8b5008                 mov edx, dword ptr [eax + 8]
 - 00b716ba  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
   00b716c0  85d6                   test esi, edx
   00b716c2  7522                   jne 0xb716e6
@@ retail 933-934 built 933-934
   00b716fa  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00b71701  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00b71707  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b71709  8b1508000000           mov edx, dword ptr [8]
   00b7170d  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71712  8bb0882f0000           mov esi, dword ptr [eax + 0x2f88]
@@ retail 940-942 built 940-942
   00b71720  8bfa                   mov edi, edx
   00b71722  7450                   je 0xb71774
 - 00b71724  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b71727  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 + 00b71726  8bb014280000           mov esi, dword ptr [eax + 0x2814]
 + 00b7172c  8b5108                 mov edx, dword ptr [ecx + 8]
   00b7172d  85d6                   test esi, edx
   00b7172f  7522                   jne 0xb71753
DIFFLINES 44 retail_instr 1051 built_instr 1051
