00b751f0 CEngineWaterRenderer::RenderIceForeground DIFFER(2722v2722) flags= []
@@ retail 233-234 built 233-234
   00b754cc  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b754d2  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b754d5  85c6                   test esi, eax
 + 00b754d5  85f0                   test eax, esi
   00b754d7  7522                   jne 0xb754fb
   00b754d9  0bc6                   or eax, esi
@@ retail 235-237 built 235-237
   00b754d5  85c6                   test esi, eax
   00b754d7  7522                   jne 0xb754fb
 - 00b754d9  0bc6                   or eax, esi
 - 00b754db  894208                 mov dword ptr [edx + 8], eax
 + 00b754d9  0bf0                   or esi, eax
 + 00b754db  897208                 mov dword ptr [edx + 8], esi
   00b754de  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b754e4  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 260-261 built 260-261
   00b75539  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7553f  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b75542  85c6                   test esi, eax
 + 00b75542  85f0                   test eax, esi
   00b75544  7522                   jne 0xb75568
   00b75546  0bc6                   or eax, esi
@@ retail 262-264 built 262-264
   00b75542  85c6                   test esi, eax
   00b75544  7522                   jne 0xb75568
 - 00b75546  0bc6                   or eax, esi
 - 00b75548  894208                 mov dword ptr [edx + 8], eax
 + 00b75546  0bf0                   or esi, eax
 + 00b75548  897208                 mov dword ptr [edx + 8], esi
   00b7554b  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b75551  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 279-280 built 279-280
   00b75583  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b75589  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7558e  8b35cc6d3901           mov esi, dword ptr [0x1396dcc]
 + 00b7558e  8b3504000000           mov esi, dword ptr [4]
   00b75594  39b0fc2e0000           cmp dword ptr [eax + 0x2efc], esi
   00b7559a  8d90f82e0000           lea edx, [eax + 0x2ef8]
@@ retail 287-288 built 287-288
   00b755a6  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b755ac  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b755af  85c6                   test esi, eax
 + 00b755af  85f0                   test eax, esi
   00b755b1  7522                   jne 0xb755d5
   00b755b3  0bc6                   or eax, esi
@@ retail 289-291 built 289-291
   00b755af  85c6                   test esi, eax
   00b755b1  7522                   jne 0xb755d5
 - 00b755b3  0bc6                   or eax, esi
 - 00b755b5  894208                 mov dword ptr [edx + 8], eax
 + 00b755b3  0bf0                   or esi, eax
 + 00b755b5  897208                 mov dword ptr [edx + 8], esi
   00b755b8  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b755be  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 306-307 built 306-307
   00b755f0  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b755f6  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b755fb  8b35d06d3901           mov esi, dword ptr [0x1396dd0]
 + 00b755fb  8b3508000000           mov esi, dword ptr [8]
   00b75601  39b09c2f0000           cmp dword ptr [eax + 0x2f9c], esi
   00b75607  8d90982f0000           lea edx, [eax + 0x2f98]
@@ retail 314-315 built 314-315
   00b75613  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b75619  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7561c  85c6                   test esi, eax
 + 00b7561c  85f0                   test eax, esi
   00b7561e  7522                   jne 0xb75642
   00b75620  0bc6                   or eax, esi
@@ retail 316-318 built 316-318
   00b7561c  85c6                   test esi, eax
   00b7561e  7522                   jne 0xb75642
 - 00b75620  0bc6                   or eax, esi
 - 00b75622  894208                 mov dword ptr [edx + 8], eax
 + 00b75620  0bf0                   or esi, eax
 + 00b75622  897208                 mov dword ptr [edx + 8], esi
   00b75625  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b7562b  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 341-342 built 341-342
   00b75680  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b75686  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b75689  85c6                   test esi, eax
 + 00b75689  85f0                   test eax, esi
   00b7568b  7522                   jne 0xb756af
   00b7568d  0bc6                   or eax, esi
@@ retail 343-345 built 343-345
   00b75689  85c6                   test esi, eax
   00b7568b  7522                   jne 0xb756af
 - 00b7568d  0bc6                   or eax, esi
 - 00b7568f  894208                 mov dword ptr [edx + 8], eax
 + 00b7568d  0bf0                   or esi, eax
 + 00b7568f  897208                 mov dword ptr [edx + 8], esi
   00b75692  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b75698  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 368-369 built 368-369
   00b756ed  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b756f3  8b7108                 mov esi, dword ptr [ecx + 8]
 - 00b756f6  85c6                   test esi, eax
 + 00b756f6  85f0                   test eax, esi
   00b756f8  7522                   jne 0xb7571c
   00b756fa  0bc6                   or eax, esi
@@ retail 370-372 built 370-372
   00b756f6  85c6                   test esi, eax
   00b756f8  7522                   jne 0xb7571c
 - 00b756fa  0bc6                   or eax, esi
 - 00b756fc  894108                 mov dword ptr [ecx + 8], eax
 + 00b756fa  0bf0                   or esi, eax
 + 00b756fc  897108                 mov dword ptr [ecx + 8], esi
   00b756ff  8b8208280000           mov eax, dword ptr [edx + 0x2808]
   00b75705  894cc208               mov dword ptr [edx + eax*8 + 8], ecx
DIFFLINES 20 retail_instr 784 built_instr 784
