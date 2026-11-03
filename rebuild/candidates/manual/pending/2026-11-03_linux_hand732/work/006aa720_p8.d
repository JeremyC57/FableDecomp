006aa720 CThingPlayerCreature::SetPlayerCreatureActionForMelee DIFFER(1230v1212) flags= []
@@ retail 215-215 built 215-216
   006aa9a3  8b7570                 mov esi, dword ptr [ebp + 0x70]
   006aa9a6  85f6                   test esi, esi
 + 006aa9a8  c744243000000000       mov dword ptr [esp + 0x30], 0
   006aa9a8  c644242800             mov byte ptr [esp + 0x28], 0
   006aa9ad  c744243000000000       mov dword ptr [esp + 0x30], 0
@@ retail 216-217 built 217-217
   006aa9a6  85f6                   test esi, esi
   006aa9a8  c644242800             mov byte ptr [esp + 0x28], 0
 - 006aa9ad  c744243000000000       mov dword ptr [esp + 0x30], 0
   006aa9b5  7403                   je 0x6aa9ba
   006aa9b7  ff4604                 inc dword ptr [esi + 4]
@@ retail 260-261 built 260-262
   006aaa24  e8a774fbff             call 0x661ed0
   006aaa29  8b5028                 mov edx, dword ptr [eax + 0x28]
 - 006aaa2c  52                     push edx
 + 006aaa2c  8bc2                   mov eax, edx
 + 006aaa2e  50                     push eax
   006aaa2d  8bcd                   mov ecx, ebp
   006aaa2f  e89cc9e1ff             call 0x4c73d0
@@ retail 262-262 built 263-264
   006aaa2c  52                     push edx
   006aaa2d  8bcd                   mov ecx, ebp
 + 006aaa31  8954242c               mov dword ptr [esp + 0x2c], edx
   006aaa2f  e89cc9e1ff             call 0x4c73d0
   006aaa34  50                     push eax
@@ retail 279-280 built 281-282
   006aaa5d  f7472400004000         test dword ptr [edi + 0x24], 0x400000
   006aaa64  7450                   je 0x6aaab6
 - 006aaa66  8d442428               lea eax, [esp + 0x28]
 + 006aaa6c  8d4c2428               lea ecx, [esp + 0x28]
   006aaa6a  8d7744                 lea esi, [edi + 0x44]
   006aaa6d  50                     push eax
@@ retail 281-282 built 283-284
   006aaa66  8d442428               lea eax, [esp + 0x28]
   006aaa6a  8d7744                 lea esi, [edi + 0x44]
 - 006aaa6d  50                     push eax
 + 006aaa73  51                     push ecx
   006aaa6e  8bce                   mov ecx, esi
   006aaa70  c744242c36000000       mov dword ptr [esp + 0x2c], 0x36
@@ retail 321-323 built 323-325
   006aaad8  50                     push eax
   006aaad9  e8b2e2fbff             call 0x668d90
 - 006aaade  8b4c2430               mov ecx, dword ptr [esp + 0x30]
 - 006aaae2  85c9                   test ecx, ecx
 + 006aaae4  8b442430               mov eax, dword ptr [esp + 0x30]
 + 006aaae8  85c0                   test eax, eax
   006aaae4  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aaae8  0f84a0000000           je 0x6aab8e
@@ retail 325-326 built 327-334
   006aaae4  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aaae8  0f84a0000000           je 0x6aab8e
 - 006aaaee  ff4904                 dec dword ptr [ecx + 4]
 + 006aaaf4  8b5804                 mov ebx, dword ptr [eax + 4]
 + 006aaaf7  8bc8                   mov ecx, eax
 + 006aaaf9  83c004                 add eax, 4
 + 006aaafc  4b                     dec ebx
 + 006aaafd  8bd3                   mov edx, ebx
 + 006aaaff  85d2                   test edx, edx
 + 006aab01  8918                   mov dword ptr [eax], ebx
   006aaaf1  0f8597000000           jne 0x6aab8e
   006aaaf7  8b11                   mov edx, dword ptr [ecx]
DIFFLINES 16 retail_instr 412 built_instr 420
