00b761d0 CEngineWaterRenderer::RenderForeground DIFFER(2531v2531) flags= []
@@ retail 253-254 built 253-254
   00b764ee  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b764f4  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b764f7  85c6                   test esi, eax
 + 00b764f7  85f0                   test eax, esi
   00b764f9  7522                   jne 0xb7651d
   00b764fb  0bc6                   or eax, esi
@@ retail 255-257 built 255-257
   00b764f7  85c6                   test esi, eax
   00b764f9  7522                   jne 0xb7651d
 - 00b764fb  0bc6                   or eax, esi
 - 00b764fd  894208                 mov dword ptr [edx + 8], eax
 + 00b764fb  0bf0                   or esi, eax
 + 00b764fd  897208                 mov dword ptr [edx + 8], esi
   00b76500  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b76506  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 280-281 built 280-281
   00b7655b  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b76561  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b76564  85c6                   test esi, eax
 + 00b76564  85f0                   test eax, esi
   00b76566  7522                   jne 0xb7658a
   00b76568  0bc6                   or eax, esi
@@ retail 282-284 built 282-284
   00b76564  85c6                   test esi, eax
   00b76566  7522                   jne 0xb7658a
 - 00b76568  0bc6                   or eax, esi
 - 00b7656a  894208                 mov dword ptr [edx + 8], eax
 + 00b76568  0bf0                   or esi, eax
 + 00b7656a  897208                 mov dword ptr [edx + 8], esi
   00b7656d  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b76573  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 299-300 built 299-300
   00b765a5  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b765ab  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b765b0  8b3dcc6d3901           mov edi, dword ptr [0x1396dcc]
 + 00b765b0  8b3d04000000           mov edi, dword ptr [4]
   00b765b6  8bb0fc2e0000           mov esi, dword ptr [eax + 0x2efc]
   00b765bc  3bf7                   cmp esi, edi
@@ retail 307-308 built 307-308
   00b765c8  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b765ce  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b765d1  85c6                   test esi, eax
 + 00b765d1  85f0                   test eax, esi
   00b765d3  7522                   jne 0xb765f7
   00b765d5  0bc6                   or eax, esi
@@ retail 309-311 built 309-311
   00b765d1  85c6                   test esi, eax
   00b765d3  7522                   jne 0xb765f7
 - 00b765d5  0bc6                   or eax, esi
 - 00b765d7  894208                 mov dword ptr [edx + 8], eax
 + 00b765d5  0bf0                   or esi, eax
 + 00b765d7  897208                 mov dword ptr [edx + 8], esi
   00b765da  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b765e0  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 326-327 built 326-327
   00b76612  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b76618  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7661d  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b7661d  8b3d08000000           mov edi, dword ptr [8]
   00b76623  8bb09c2f0000           mov esi, dword ptr [eax + 0x2f9c]
   00b76629  3bf7                   cmp esi, edi
@@ retail 334-335 built 334-335
   00b76635  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b7663b  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b7663e  85c6                   test esi, eax
 + 00b7663e  85f0                   test eax, esi
   00b76640  7522                   jne 0xb76664
   00b76642  0bc6                   or eax, esi
@@ retail 336-338 built 336-338
   00b7663e  85c6                   test esi, eax
   00b76640  7522                   jne 0xb76664
 - 00b76642  0bc6                   or eax, esi
 - 00b76644  894208                 mov dword ptr [edx + 8], eax
 + 00b76642  0bf0                   or esi, eax
 + 00b76644  897208                 mov dword ptr [edx + 8], esi
   00b76647  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b7664d  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 353-354 built 353-354
   00b7667f  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b76685  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b7668a  8b3dcc6d3901           mov edi, dword ptr [0x1396dcc]
 + 00b7668a  8b3d04000000           mov edi, dword ptr [4]
   00b76690  8bb0102f0000           mov esi, dword ptr [eax + 0x2f10]
   00b76696  3bf7                   cmp esi, edi
@@ retail 361-362 built 361-362
   00b766a2  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b766a8  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b766ab  85c6                   test esi, eax
 + 00b766ab  85f0                   test eax, esi
   00b766ad  7522                   jne 0xb766d1
   00b766af  0bc6                   or eax, esi
@@ retail 363-365 built 363-365
   00b766ab  85c6                   test esi, eax
   00b766ad  7522                   jne 0xb766d1
 - 00b766af  0bc6                   or eax, esi
 - 00b766b1  894208                 mov dword ptr [edx + 8], eax
 + 00b766af  0bf0                   or esi, eax
 + 00b766b1  897208                 mov dword ptr [edx + 8], esi
   00b766b4  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b766ba  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 380-381 built 380-381
   00b766ec  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b766f2  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b766f7  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b766f7  8b3d08000000           mov edi, dword ptr [8]
   00b766fd  8bb0b02f0000           mov esi, dword ptr [eax + 0x2fb0]
   00b76703  3bf7                   cmp esi, edi
@@ retail 388-389 built 388-389
   00b7670f  8b8014280000           mov eax, dword ptr [eax + 0x2814]
   00b76715  8b7208                 mov esi, dword ptr [edx + 8]
 - 00b76718  85c6                   test esi, eax
 + 00b76718  85f0                   test eax, esi
   00b7671a  7522                   jne 0xb7673e
   00b7671c  0bc6                   or eax, esi
@@ retail 390-392 built 390-392
   00b76718  85c6                   test esi, eax
   00b7671a  7522                   jne 0xb7673e
 - 00b7671c  0bc6                   or eax, esi
 - 00b7671e  894208                 mov dword ptr [edx + 8], eax
 + 00b7671c  0bf0                   or esi, eax
 + 00b7671e  897208                 mov dword ptr [edx + 8], esi
   00b76721  8b8108280000           mov eax, dword ptr [ecx + 0x2808]
   00b76727  8954c108               mov dword ptr [ecx + eax*8 + 8], edx
@@ retail 407-408 built 407-408
   00b76759  ff810c280000           inc dword ptr [ecx + 0x280c]
   00b7675f  a1186e4301             mov eax, dword ptr [0x1436e18]
 - 00b76764  8b3dd06d3901           mov edi, dword ptr [0x1396dd0]
 + 00b76764  8b3d08000000           mov edi, dword ptr [8]
   00b7676a  8bb0242f0000           mov esi, dword ptr [eax + 0x2f24]
   00b76770  3bf7                   cmp esi, edi
DIFFLINES 23 retail_instr 715 built_instr 715
