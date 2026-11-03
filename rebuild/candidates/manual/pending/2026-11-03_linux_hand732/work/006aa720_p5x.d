006aa720 CThingPlayerCreature::SetPlayerCreatureActionForMelee DIFFER(1224v1212) flags= []
@@ retail 321-323 built 321-323
   006aaad8  50                     push eax
   006aaad9  e8b2e2fbff             call 0x668d90
 - 006aaade  8b4c2430               mov ecx, dword ptr [esp + 0x30]
 - 006aaae2  85c9                   test ecx, ecx
 + 006aaade  8b442430               mov eax, dword ptr [esp + 0x30]
 + 006aaae2  85c0                   test eax, eax
   006aaae4  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aaae8  0f84a0000000           je 0x6aab8e
@@ retail 325-326 built 325-332
   006aaae4  8b74241c               mov esi, dword ptr [esp + 0x1c]
   006aaae8  0f84a0000000           je 0x6aab8e
 - 006aaaee  ff4904                 dec dword ptr [ecx + 4]
 + 006aaaee  8b5804                 mov ebx, dword ptr [eax + 4]
 + 006aaaf1  8bc8                   mov ecx, eax
 + 006aaaf3  83c004                 add eax, 4
 + 006aaaf6  4b                     dec ebx
 + 006aaaf7  8bd3                   mov edx, ebx
 + 006aaaf9  85d2                   test edx, edx
 + 006aaafb  8918                   mov dword ptr [eax], ebx
   006aaaf1  0f8597000000           jne 0x6aab8e
   006aaaf7  8b11                   mov edx, dword ptr [ecx]
DIFFLINES 9 retail_instr 412 built_instr 418
