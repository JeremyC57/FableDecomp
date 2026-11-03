006aa720 CThingPlayerCreature::SetPlayerCreatureActionForMelee DIFFER(1218v1212) flags= []
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
DIFFLINES 5 retail_instr 412 built_instr 414
