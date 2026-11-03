00b70a90 CEngineWaterRenderer::RenderEnvironmentMap DIFFER(3567v3567) flags= []
@@ retail 618-619 built 618-619
   00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71278  8b3d08000000           mov edi, dword ptr [8]
   00b7127e  8b90882f0000           mov edx, dword ptr [eax + 0x2f88]
   00b71284  3bd7                   cmp edx, edi
@@ retail 670-671 built 670-671
   00b71343  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71349  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7134e  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b7134e  8b3d08000000           mov edi, dword ptr [8]
   00b71354  8b909c2f0000           mov edx, dword ptr [eax + 0x2f9c]
   00b7135a  3bd7                   cmp edx, edi
@@ retail 766-767 built 766-767
   00b71498  e8836be1ff             call 0x988020
   00b7149d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b714a2  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00b714a2  8b3510000000           mov esi, dword ptr [0x10]
   00b714a8  8ba820290000           mov ebp, dword ptr [eax + 0x2920]
   00b714ae  3bee                   cmp ebp, esi
@@ retail 794-795 built 794-795
   00b71506  ff820c280000           inc dword ptr [edx + 0x280c]
   00b7150c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71511  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00b71511  8b3514000000           mov esi, dword ptr [0x14]
   00b71517  8ba834290000           mov ebp, dword ptr [eax + 0x2934]
   00b7151d  3bee                   cmp ebp, esi
@@ retail 801-801 built 801-802
   00b71527  8bfe                   mov edi, esi
   00b71529  7455                   je 0xb71580
 + 00b7152b  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
   00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 802-803 built 803-803
   00b71529  7455                   je 0xb71580
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71534  85c6                   test esi, eax
   00b71536  7522                   jne 0xb7155a
@@ retail 907-908 built 907-908
   00b71694  e86784e1ff             call 0x989b00
   00b71699  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00b7169f  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b7169f  8b1508000000           mov edx, dword ptr [8]
   00b716a5  8bb1e82e0000           mov esi, dword ptr [ecx + 0x2ee8]
   00b716ab  3bf2                   cmp esi, edx
@@ retail 933-934 built 933-934
   00b716fa  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00b71701  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00b71707  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b71707  8b1508000000           mov edx, dword ptr [8]
   00b7170d  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71712  8bb0882f0000           mov esi, dword ptr [eax + 0x2f88]
@@ retail 940-942 built 940-942
   00b71720  8bfa                   mov edi, edx
   00b71722  7450                   je 0xb71774
 - 00b71724  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b71727  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 + 00b71724  8b5108                 mov edx, dword ptr [ecx + 8]
 + 00b71727  8bb014280000           mov esi, dword ptr [eax + 0x2814]
   00b7172d  85d6                   test esi, edx
   00b7172f  7522                   jne 0xb71753
DIFFLINES 10 retail_instr 1051 built_instr 1051
