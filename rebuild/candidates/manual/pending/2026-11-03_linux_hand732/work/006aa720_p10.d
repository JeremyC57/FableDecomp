006aa720 CThingPlayerCreature::SetPlayerCreatureActionForMelee DIFFER(1230v1212) flags= []
@@ retail 255-257 built 255-255
   006aaa18  eb04                   jmp 0x6aaa1e
   006aaa1a  8b742428               mov esi, dword ptr [esp + 0x28]
 - 006aaa1e  84db                   test bl, bl
 - 006aaa20  752a                   jne 0x6aaa4c
   006aaa22  8bcf                   mov ecx, edi
   006aaa24  e8a774fbff             call 0x661ed0
@@ retail 259-259 built 257-258
   006aaa22  8bcf                   mov ecx, edi
   006aaa24  e8a774fbff             call 0x661ed0
 + 006aaa25  84db                   test bl, bl
   006aaa29  8b5028                 mov edx, dword ptr [eax + 0x28]
   006aaa2c  52                     push edx
@@ retail 260-261 built 259-263
   006aaa24  e8a774fbff             call 0x661ed0
   006aaa29  8b5028                 mov edx, dword ptr [eax + 0x28]
 - 006aaa2c  52                     push edx
 + 006aaa2a  89542428               mov dword ptr [esp + 0x28], edx
 + 006aaa2e  7522                   jne 0x6aaa52
 + 006aaa30  8bc2                   mov eax, edx
 + 006aaa32  50                     push eax
   006aaa2d  8bcd                   mov ecx, ebp
   006aaa2f  e89cc9e1ff             call 0x4c73d0
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
DIFFLINES 18 retail_instr 412 built_instr 420
