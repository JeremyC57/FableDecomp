006aa2e0 CThingPlayerCreature::DrawPreparePrimitivesForRendering DIFFER(1087v1080) flags= []
@@ retail 202-202 built 202-204
   006aa547  85c0                   test eax, eax
   006aa549  742a                   je 0x6aa575
 + 006aa54b  8bcf                   mov ecx, edi
 + 006aa54d  e800000000             call 0x6aa552
   006aa54b  f6809100000001         test byte ptr [eax + 0x91], 1
   006aa552  7521                   jne 0x6aa575
DIFFLINES 2 retail_instr 356 built_instr 358
