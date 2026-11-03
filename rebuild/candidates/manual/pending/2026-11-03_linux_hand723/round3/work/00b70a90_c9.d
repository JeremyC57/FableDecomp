00b70a90 CEngineWaterRenderer::RenderEnvironmentMap DIFFER(3507v3567) flags= []
@@ retail 0-1 built 0-1
 - 00b70a90  83ec58                 sub esp, 0x58
 + 00b70a90  83ec38                 sub esp, 0x38
   00b70a93  a18c6e4301             mov eax, dword ptr [0x1436e8c]
   00b70a98  894c2404               mov dword ptr [esp + 4], ecx
@@ retail 146-147 built 146-147
   00b70c2e  5f                     pop edi
   00b70c2f  5b                     pop ebx
 - 00b70c30  83c458                 add esp, 0x58
 + 00b70c30  83c438                 add esp, 0x38
   00b70c33  c3                     ret 
   00b70c34  6a44                   push 0x44
@@ retail 176-177 built 176-177
   00b70c78  5f                     pop edi
   00b70c79  5b                     pop ebx
 - 00b70c7a  83c458                 add esp, 0x58
 + 00b70c7a  83c438                 add esp, 0x38
   00b70c7d  c3                     ret 
   00b70c7e  33c0                   xor eax, eax
@@ retail 411-413 built 411-413
   00b70f33  ff9204010000           call dword ptr [edx + 0x104]
   00b70f39  8b86b83c0000           mov eax, dword ptr [esi + 0x3cb8]
 - 00b70f3f  bf02000000             mov edi, 2
 - 00b70f44  3bc7                   cmp eax, edi
 + 00b70f3f  bb02000000             mov ebx, 2
 + 00b70f44  3bc3                   cmp eax, ebx
   00b70f46  7d06                   jge 0xb70f4e
   00b70f48  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
@@ retail 414-415 built 414-415
   00b70f44  3bc7                   cmp eax, edi
   00b70f46  7d06                   jge 0xb70f4e
 - 00b70f48  89beb83c0000           mov dword ptr [esi + 0x3cb8], edi
 + 00b70f48  899eb83c0000           mov dword ptr [esi + 0x3cb8], ebx
   00b70f4e  8b6c2414               mov ebp, dword ptr [esp + 0x14]
   00b70f52  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
@@ retail 426-448 built 426-428
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
 + 00b70f7e  8b7c2418               mov edi, dword ptr [esp + 0x18]
 + 00b70f82  8b4f54                 mov ecx, dword ptr [edi + 0x54]
   00b70fcb  8b91b4010000           mov edx, dword ptr [ecx + 0x1b4]
   00b70fd1  d822                   fsub dword ptr [edx]
@@ retail 503-504 built 483-484
   00b710a5  8b4c242c               mov ecx, dword ptr [esp + 0x2c]
   00b710a9  894a0c                 mov dword ptr [edx + 0xc], ecx
 - 00b710ac  8b98d8030000           mov ebx, dword ptr [eax + 0x3d8]
 + 00b71066  8b88d8030000           mov ecx, dword ptr [eax + 0x3d8]
   00b710b2  d9542438               fst dword ptr [esp + 0x38]
   00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
@@ retail 505-505 built 485-492
   00b710ac  8b98d8030000           mov ebx, dword ptr [eax + 0x3d8]
   00b710b2  d9542438               fst dword ptr [esp + 0x38]
 + 00b71070  0bcb                   or ecx, ebx
 + 00b71072  d954243c               fst dword ptr [esp + 0x3c]
 + 00b71076  8988d8030000           mov dword ptr [eax + 0x3d8], ecx
 + 00b7107c  d9542440               fst dword ptr [esp + 0x40]
 + 00b71080  a100000000             mov eax, dword ptr [0]
 + 00b71085  d95c2444               fstp dword ptr [esp + 0x44]
 + 00b71089  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
   00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
   00b710ba  d954243c               fst dword ptr [esp + 0x3c]
@@ retail 506-513 built 493-493
   00b710b2  d9542438               fst dword ptr [esp + 0x38]
   00b710b6  8b4c2438               mov ecx, dword ptr [esp + 0x38]
 - 00b710ba  d954243c               fst dword ptr [esp + 0x3c]
 - 00b710be  0bdf                   or ebx, edi
 - 00b710c0  d9542440               fst dword ptr [esp + 0x40]
 - 00b710c4  8998d8030000           mov dword ptr [eax + 0x3d8], ebx
 - 00b710ca  d95c2444               fstp dword ptr [esp + 0x44]
 - 00b710ce  a1146e4301             mov eax, dword ptr [0x1436e14]
 - 00b710d3  8b90cc030000           mov edx, dword ptr [eax + 0x3cc]
   00b710d9  83c260                 add edx, 0x60
   00b710dc  890a                   mov dword ptr [edx], ecx
@@ retail 521-522 built 501-502
   00b710ec  8b4c2444               mov ecx, dword ptr [esp + 0x44]
   00b710f0  894a0c                 mov dword ptr [edx + 0xc], ecx
 - 00b710f3  09b8d8030000           or dword ptr [eax + 0x3d8], edi
 + 00b710ad  0998d8030000           or dword ptr [eax + 0x3d8], ebx
   00b710f9  a1a06e4301             mov eax, dword ptr [0x1436ea0]
   00b710fe  8b90bc010000           mov edx, dword ptr [eax + 0x1bc]
@@ retail 560-560 built 540-541
   00b71191  d84e60                 fmul dword ptr [esi + 0x60]
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
 + 00b71152  8b5754                 mov edx, dword ptr [edi + 0x54]
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00b7119e  8d542448               lea edx, [esp + 0x48]
@@ retail 561-562 built 542-543
   00b71194  d95c2414               fstp dword ptr [esp + 0x14]
   00b71198  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b7119e  8d542448               lea edx, [esp + 0x48]
 + 00b7115b  81c2c0000000           add edx, 0xc0
   00b711a2  52                     push edx
   00b711a3  6a00                   push 0
@@ retail 565-565 built 546-547
   00b711a3  6a00                   push 0
   00b711a5  e8a689e1ff             call 0x989b50
 + 00b71169  8b4754                 mov eax, dword ptr [edi + 0x54]
   00b711aa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
   00b711b0  8d442458               lea eax, [esp + 0x58]
@@ retail 566-567 built 548-549
   00b711a5  e8a689e1ff             call 0x989b50
   00b711aa  8b0d146e4301           mov ecx, dword ptr [0x1436e14]
 - 00b711b0  8d442458               lea eax, [esp + 0x58]
 + 00b71172  05d0000000             add eax, 0xd0
   00b711b4  50                     push eax
   00b711b5  bb01000000             mov ebx, 1
@@ retail 591-592 built 573-574
   00b71201  6a02                   push 2
   00b71203  e8f888e1ff             call 0x989b00
 - 00b71208  a1186e4301             mov eax, dword ptr [0x1436e18]
 + 00b711cb  8b0d00000000           mov ecx, dword ptr [0]
   00b7120d  8b3dc86d3901           mov edi, dword ptr [0x1396dc8]
   00b71213  8b90e82e0000           mov edx, dword ptr [eax + 0x2ee8]
@@ retail 593-594 built 575-576
   00b71208  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b7120d  8b3dc86d3901           mov edi, dword ptr [0x1396dc8]
 - 00b71213  8b90e82e0000           mov edx, dword ptr [eax + 0x2ee8]
 + 00b711d7  8b91e82e0000           mov edx, dword ptr [ecx + 0x2ee8]
   00b71219  3bd7                   cmp edx, edi
   00b7121b  8d88e42e0000           lea ecx, [eax + 0x2ee4]
@@ retail 595-596 built 577-578
   00b71213  8b90e82e0000           mov edx, dword ptr [eax + 0x2ee8]
   00b71219  3bd7                   cmp edx, edi
 - 00b7121b  8d88e42e0000           lea ecx, [eax + 0x2ee4]
 + 00b711df  8d81e42e0000           lea eax, [ecx + 0x2ee4]
   00b71221  7450                   je 0xb71273
   00b71223  8b9014280000           mov edx, dword ptr [eax + 0x2814]
@@ retail 597-599 built 579-581
   00b7121b  8d88e42e0000           lea ecx, [eax + 0x2ee4]
   00b71221  7450                   je 0xb71273
 - 00b71223  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 - 00b71229  8b7108                 mov esi, dword ptr [ecx + 8]
 + 00b711e7  8b9114280000           mov edx, dword ptr [ecx + 0x2814]
 + 00b711ed  8b7008                 mov esi, dword ptr [eax + 8]
   00b7122c  85d6                   test esi, edx
   00b7122e  7522                   jne 0xb71252
@@ retail 602-610 built 584-592
   00b7122e  7522                   jne 0xb71252
   00b71230  0bd6                   or edx, esi
 - 00b71232  895108                 mov dword ptr [ecx + 8], edx
 - 00b71235  8b9008280000           mov edx, dword ptr [eax + 0x2808]
 - 00b7123b  894cd008               mov dword ptr [eax + edx*8 + 8], ecx
 - 00b7123f  8b9008280000           mov edx, dword ptr [eax + 0x2808]
 - 00b71245  8b7104                 mov esi, dword ptr [ecx + 4]
 - 00b71248  8974d00c               mov dword ptr [eax + edx*8 + 0xc], esi
 - 00b7124c  ff8008280000           inc dword ptr [eax + 0x2808]
 - 00b71252  8a5110                 mov dl, byte ptr [ecx + 0x10]
 + 00b711f6  895008                 mov dword ptr [eax + 8], edx
 + 00b711f9  8b9108280000           mov edx, dword ptr [ecx + 0x2808]
 + 00b711ff  8944d108               mov dword ptr [ecx + edx*8 + 8], eax
 + 00b71203  8b9108280000           mov edx, dword ptr [ecx + 0x2808]
 + 00b71209  8b7004                 mov esi, dword ptr [eax + 4]
 + 00b7120c  8974d10c               mov dword ptr [ecx + edx*8 + 0xc], esi
 + 00b71210  ff8108280000           inc dword ptr [ecx + 0x2808]
 + 00b71216  8a5010                 mov dl, byte ptr [eax + 0x10]
   00b71255  84d2                   test dl, dl
   00b71257  897904                 mov dword ptr [ecx + 4], edi
@@ retail 611-612 built 593-594
   00b71252  8a5110                 mov dl, byte ptr [ecx + 0x10]
   00b71255  84d2                   test dl, dl
 - 00b71257  897904                 mov dword ptr [ecx + 4], edi
 + 00b7121b  897804                 mov dword ptr [eax + 4], edi
   00b7125a  7517                   jne 0xb71273
   00b7125c  c6411001               mov byte ptr [ecx + 0x10], 1
@@ retail 613-617 built 595-599
   00b71257  897904                 mov dword ptr [ecx + 4], edi
   00b7125a  7517                   jne 0xb71273
 - 00b7125c  c6411001               mov byte ptr [ecx + 0x10], 1
 - 00b71260  8b900c280000           mov edx, dword ptr [eax + 0x280c]
 - 00b71266  898c9008200000         mov dword ptr [eax + edx*4 + 0x2008], ecx
 - 00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
 + 00b71220  c6401001               mov byte ptr [eax + 0x10], 1
 + 00b71224  8b910c280000           mov edx, dword ptr [ecx + 0x280c]
 + 00b7122a  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
 + 00b71231  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
@@ retail 618-619 built 600-601
   00b7126d  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71273  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71278  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b7123c  8b3d08000000           mov edi, dword ptr [8]
   00b7127e  8b90882f0000           mov edx, dword ptr [eax + 0x2f88]
   00b71284  3bd7                   cmp edx, edi
@@ retail 670-671 built 652-653
   00b71343  ff800c280000           inc dword ptr [eax + 0x280c]
   00b71349  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7134e  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b71312  8b3d08000000           mov edi, dword ptr [8]
   00b71354  8b909c2f0000           mov edx, dword ptr [eax + 0x2f9c]
   00b7135a  3bd7                   cmp edx, edi
@@ retail 766-767 built 748-749
   00b71498  e8836be1ff             call 0x988020
   00b7149d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b714a2  8b35786f3901           mov esi, dword ptr [0x1396f78]
 + 00b71466  8b3510000000           mov esi, dword ptr [0x10]
   00b714a8  8ba820290000           mov ebp, dword ptr [eax + 0x2920]
   00b714ae  3bee                   cmp ebp, esi
@@ retail 794-795 built 776-777
   00b71506  ff820c280000           inc dword ptr [edx + 0x280c]
   00b7150c  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b71511  8b357c6f3901           mov esi, dword ptr [0x1396f7c]
 + 00b714d5  8b3514000000           mov esi, dword ptr [0x14]
   00b71517  8ba834290000           mov ebp, dword ptr [eax + 0x2934]
   00b7151d  3bee                   cmp ebp, esi
@@ retail 801-801 built 783-784
   00b71527  8bfe                   mov edi, esi
   00b71529  7455                   je 0xb71580
 + 00b714ef  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
   00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 802-803 built 785-785
   00b71529  7455                   je 0xb71580
   00b7152b  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7152e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71534  85c6                   test esi, eax
   00b71536  7522                   jne 0xb7155a
@@ retail 827-827 built 809-810
   00b7158e  8bc8                   mov ecx, eax
   00b71590  7450                   je 0xb715e2
 + 00b71556  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
   00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
@@ retail 828-829 built 811-811
   00b71590  7450                   je 0xb715e2
   00b71592  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b71595  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7159b  85c6                   test esi, eax
   00b7159d  7522                   jne 0xb715c1
@@ retail 907-908 built 889-890
   00b71694  e86784e1ff             call 0x989b00
   00b71699  8b0d186e4301           mov ecx, dword ptr [0x1436e18]
 - 00b7169f  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b71663  8b1508000000           mov edx, dword ptr [8]
   00b716a5  8bb1e82e0000           mov esi, dword ptr [ecx + 0x2ee8]
   00b716ab  3bf2                   cmp esi, edx
@@ retail 913-913 built 895-896
   00b716b3  8bfa                   mov edi, edx
   00b716b5  7456                   je 0xb7170d
 + 00b7167b  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
   00b716b7  8b5008                 mov edx, dword ptr [eax + 8]
   00b716ba  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
@@ retail 914-915 built 897-897
   00b716b5  7456                   je 0xb7170d
   00b716b7  8b5008                 mov edx, dword ptr [eax + 8]
 - 00b716ba  8bb114280000           mov esi, dword ptr [ecx + 0x2814]
   00b716c0  85d6                   test esi, edx
   00b716c2  7522                   jne 0xb716e6
@@ retail 933-934 built 915-916
   00b716fa  89849108200000         mov dword ptr [ecx + edx*4 + 0x2008], eax
   00b71701  ff810c280000           inc dword ptr [ecx + 0x280c]
 - 00b71707  8b15d06d3901           mov edx, dword ptr [0x1396dd0]
 + 00b716cb  8b1508000000           mov edx, dword ptr [8]
   00b7170d  a1186e4301             mov eax, dword ptr [0x1436e18]
   00b71712  8bb0882f0000           mov esi, dword ptr [eax + 0x2f88]
@@ retail 940-942 built 922-924
   00b71720  8bfa                   mov edi, edx
   00b71722  7450                   je 0xb71774
 - 00b71724  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b71727  8b9014280000           mov edx, dword ptr [eax + 0x2814]
 + 00b716e8  8bb014280000           mov esi, dword ptr [eax + 0x2814]
 + 00b716ee  8b5108                 mov edx, dword ptr [ecx + 8]
   00b7172d  85d6                   test esi, edx
   00b7172f  7522                   jne 0xb71753
@@ retail 1049-1050 built 1031-1032
   00b71879  5f                     pop edi
   00b7187a  5b                     pop ebx
 - 00b7187b  83c458                 add esp, 0x58
 + 00b7183f  83c438                 add esp, 0x38
   00b7187e  c3                     ret 
DIFFLINES 81 retail_instr 1051 built_instr 1033
