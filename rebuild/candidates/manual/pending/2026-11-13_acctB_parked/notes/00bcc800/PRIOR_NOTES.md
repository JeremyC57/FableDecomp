# Hand chunk 2061 (CEnginePrimitiveManagerWeaponTrails): 0 of 1 byte-exact (in progress)

Row 0x00BCC800 CEnginePrimitiveManagerWeaponTrails::RenderWeaponTrail(const CEngineInternalPrimitiveWeaponTrail&), 3205 bytes,
`push ebp; lea ebp,[esp-74h]; sub esp,114h`, `ret 4`. Leaf ?RenderWeaponTrail@CEnginePrimitiveManagerWeaponTrails@@AAEXABVCEngineInternalPrimitiveWeaponTrail@@@Z.

## Tools (this directory)
- `bash bin/gr.sh cand/cNN.cpp "<what changed>"` = full-row grade (q.py, overlay hdr/), logs one line to grades.log.
- `bash bin/md.sh -n 0 [--frame] [--regs]` = relocation-masked diff of the LAST grade (--frame masks frame displacements).
- tmp/q_last.txt = last q.py side-by-side.

## Progress (session 2, resumed from c22 = 947 equal / 1048 masked)
- c24: ReturnResource with `header->Last = resource;` written in BOTH arms (retail duplicates the store) -> 950 / 1050. KEEP.
- c28: FableWin.pdb static fastcall helpers CEnginePrimitiveRenderUtil::GetDynamicVBUsage / GetDynamicVBPool / GetDynamicVBLockFlags
  (hdr opt-in FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB) -> fixed Lock's argument register (retail eax). KEEP.
- c27/c29: `unsigned long first_step = 0;` declared before `current_time` -> retail's store order at 0x00BCCEE3. KEEP. c29 = 953 / 1050.
- no effect: named key in ReturnResource (c25), `if (id)` (c26), early return in ReturnResource (c30), key[3] declared first in GetBuffer (c31).

## Remaining diff (c29)
1. Frame slot permutation (retail -> ours): 0x34->0x54 (GFFloatToLongFloor `one` + vertex_count node), 0x38->0x34 (fraction),
   0x3c..0x44 key[3] -> 0x38..0x40, 0x48->0x44 (frame data), 0x4c->0x48, 0x50->0x4c, 0x54->0x50 (buffer), 0x5c<->0x60.
2. 0x00BCCDA9 `lea edx,[key]; push edx` (ours ecx) at the FindNode call inside GetBuffer's AddKey.
3. Tail 0x00BCD32D..0x00BCD3DF ReturnResource: retail keeps tree in ebx (spilled to [ebp+7Ch] around the mask's `mov ebx,1`), key
   (resource+0x34) in edi, node in [ebp+7Ch] (leaf path stores it -> leaf block laid out first); ours: tree edi, key spilled, node ebx.

## Do not retry
c25, c26, c30, c31 (see above); everything in grades.log 1..35.
