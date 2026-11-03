00b70230 CEngineWaterRenderer::RenderSeaBackground DIFFER(2138v2138) flags= []
@@ retail 93-94 built 93-94
   00b7036e  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b70374  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b70377  85f0                   test eax, esi
 + 00b70377  85c6                   test esi, eax
   00b70379  7522                   jne 0xb7039d
   00b7037b  0bf0                   or esi, eax
@@ retail 95-97 built 95-97
   00b70377  85f0                   test eax, esi
   00b70379  7522                   jne 0xb7039d
 - 00b7037b  0bf0                   or esi, eax
 - 00b7037d  897208                 mov dword ptr [edx + 8], esi
 + 00b7037b  0bc6                   or eax, esi
 + 00b7037d  894208                 mov dword ptr [edx + 8], eax
   00b70380  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b70386  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 120-121 built 120-121
   00b703da  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b703e0  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b703e3  85f0                   test eax, esi
 + 00b703e3  85c6                   test esi, eax
   00b703e5  7522                   jne 0xb70409
   00b703e7  0bf0                   or esi, eax
@@ retail 122-124 built 122-124
   00b703e3  85f0                   test eax, esi
   00b703e5  7522                   jne 0xb70409
 - 00b703e7  0bf0                   or esi, eax
 - 00b703e9  897208                 mov dword ptr [edx + 8], esi
 + 00b703e7  0bc6                   or eax, esi
 + 00b703e9  894208                 mov dword ptr [edx + 8], eax
   00b703ec  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b703f2  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 139-140 built 139-140
   00b70423  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b70429  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7042e  8b35d06d3901           mov esi, dword ptr [0x1396dd0]
 + 00b7042e  8b3508000000           mov esi, dword ptr [8]
   00b70434  39b0fc2e0000           cmp dword ptr [eax + 0x2efc], esi
   00b7043a  8d90f82e0000           lea edx, [eax + 0x2ef8]
@@ retail 147-148 built 147-148
   00b70446  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7044c  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7044f  85f0                   test eax, esi
 + 00b7044f  85c6                   test esi, eax
   00b70451  7522                   jne 0xb70475
   00b70453  0bf0                   or esi, eax
@@ retail 149-151 built 149-151
   00b7044f  85f0                   test eax, esi
   00b70451  7522                   jne 0xb70475
 - 00b70453  0bf0                   or esi, eax
 - 00b70455  897208                 mov dword ptr [edx + 8], esi
 + 00b70453  0bc6                   or eax, esi
 + 00b70455  894208                 mov dword ptr [edx + 8], eax
   00b70458  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b7045e  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 166-167 built 166-167
   00b7048f  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b70495  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7049a  8b35d06d3901           mov esi, dword ptr [0x1396dd0]
 + 00b7049a  8b3508000000           mov esi, dword ptr [8]
   00b704a0  39b09c2f0000           cmp dword ptr [eax + 0x2f9c], esi
   00b704a6  8d90982f0000           lea edx, [eax + 0x2f98]
@@ retail 193-194 built 193-194
   00b704fb  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b70501  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b70506  8b35d06d3901           mov esi, dword ptr [0x1396dd0]
 + 00b70506  8b3508000000           mov esi, dword ptr [8]
   00b7050c  39b0102f0000           cmp dword ptr [eax + 0x2f10], esi
   00b70512  8d900c2f0000           lea edx, [eax + 0x2f0c]
@@ retail 220-221 built 220-221
   00b70567  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b7056d  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b70572  8b35d06d3901           mov esi, dword ptr [0x1396dd0]
 + 00b70572  8b3508000000           mov esi, dword ptr [8]
   00b70578  39b0b02f0000           cmp dword ptr [eax + 0x2fb0], esi
   00b7057e  8d90ac2f0000           lea edx, [eax + 0x2fac]
@@ retail 416-419 built 416-419
   00b7087e  a384784301             mov dword ptr [0x1437884], eax
   00b70883  c70574784301cdcc4c3f   mov dword ptr [0x1437874], 0x3f4ccccd
 - 00b7088d  c70578784301cdcc4c3f   mov dword ptr [0x1437878], 0x3f4ccccd
 - 00b70897  c7057c78430100000000   mov dword ptr [0x143787c], 0
 - 00b708a1  c7058078430100000000   mov dword ptr [0x1437880], 0
 + 00b7088d  c70504000000cdcc4c3f   mov dword ptr [4], 0x3f4ccccd
 + 00b70897  c7050800000000000000   mov dword ptr [8], 0
 + 00b708a1  c7050c00000000000000   mov dword ptr [0xc], 0
   00b708ab  a802                   test al, 2
   00b708ad  7530                   jne 0xb708df
@@ retail 424-427 built 424-427
   00b708b2  a384784301             mov dword ptr [0x1437884], eax
   00b708b7  c7056478430100000000   mov dword ptr [0x1437864], 0
 - 00b708c1  c70568784301cdcccc3e   mov dword ptr [0x1437868], 0x3ecccccd
 - 00b708cb  c7056c784301cdcc4c3f   mov dword ptr [0x143786c], 0x3f4ccccd
 - 00b708d5  c705707843010000803f   mov dword ptr [0x1437870], 0x3f800000
 + 00b708c1  c70504000000cdcccc3e   mov dword ptr [4], 0x3ecccccd
 + 00b708cb  c70508000000cdcc4c3f   mov dword ptr [8], 0x3f4ccccd
 + 00b708d5  c7050c0000000000803f   mov dword ptr [0xc], 0x3f800000
   00b708df  8b159c6e4301           mov edx, dword ptr [0x1436e9c]
   00b708e5  d90564784301           fld dword ptr [0x1437864]
@@ retail 432-433 built 432-433
   00b708ee  d808                   fmul dword ptr [eax]
   00b708f0  8b1574784301           mov edx, dword ptr [0x1437874]
 - 00b708f6  d90568784301           fld dword ptr [0x1437868]
 + 00b708f6  d90504000000           fld dword ptr [4]
   00b708fc  d84804                 fmul dword ptr [eax + 4]
   00b708ff  d9056c784301           fld dword ptr [0x143786c]
@@ retail 434-435 built 434-435
   00b708f6  d90568784301           fld dword ptr [0x1437868]
   00b708fc  d84804                 fmul dword ptr [eax + 4]
 - 00b708ff  d9056c784301           fld dword ptr [0x143786c]
 + 00b708ff  d90508000000           fld dword ptr [8]
   00b70905  d84808                 fmul dword ptr [eax + 8]
   00b70908  d95c242c               fstp dword ptr [esp + 0x2c]
@@ retail 437-438 built 437-438
   00b70905  d84808                 fmul dword ptr [eax + 8]
   00b70908  d95c242c               fstp dword ptr [esp + 0x2c]
 - 00b7090c  d90570784301           fld dword ptr [0x1437870]
 + 00b7090c  d9050c000000           fld dword ptr [0xc]
   00b70912  d8480c                 fmul dword ptr [eax + 0xc]
   00b70915  a1146e4301             mov eax, dword ptr [0x1436e14]
@@ retail 444-445 built 444-445
   00b70923  8911                   mov dword ptr [ecx], edx
   00b70925  d95c2430               fstp dword ptr [esp + 0x30]
 - 00b70929  8b1578784301           mov edx, dword ptr [0x1437878]
 + 00b70929  8b1504000000           mov edx, dword ptr [4]
   00b7092f  d9c9                   fxch st(1)
   00b70931  895104                 mov dword ptr [ecx + 4], edx
@@ retail 448-449 built 448-449
   00b70931  895104                 mov dword ptr [ecx + 4], edx
   00b70934  d95c2414               fstp dword ptr [esp + 0x14]
 - 00b70938  8b157c784301           mov edx, dword ptr [0x143787c]
 + 00b70938  8b1508000000           mov edx, dword ptr [8]
   00b7093e  8b742414               mov esi, dword ptr [esp + 0x14]
   00b70942  d95c2418               fstp dword ptr [esp + 0x18]
@@ retail 452-453 built 452-453
   00b70942  d95c2418               fstp dword ptr [esp + 0x18]
   00b70946  895108                 mov dword ptr [ecx + 8], edx
 - 00b70949  8b1580784301           mov edx, dword ptr [0x1437880]
 + 00b70949  8b150c000000           mov edx, dword ptr [0xc]
   00b7094f  89510c                 mov dword ptr [ecx + 0xc], edx
   00b70952  8bb8d8030000           mov edi, dword ptr [eax + 0x3d8]
DIFFLINES 25 retail_instr 548 built_instr 548
