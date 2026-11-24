# nf362 b1 / b2: GetDef and Reset copies made retail-shaped in 18 size-pragma rows

Agent NF362 (Account A naming queue; hand-back `pending/2026-11-24_nf362_handback/`), from NF358's census (T3 nf358 0x0042B312 / 0x0042B040 /
0x00429C6F).
- **b1 (17 rows)**: one added line, `#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD` (the landed NF341 / NF349 opt-in, same comment
  form). GetDef<NUISystem::CUIMiscThingsDef> 101 -> 98 B (0058578b 005a04f7 005a29f4 005a8a5d 005a9cbe 005aaf28 005ad571 005c6b26 005c7452
  005c8459 005c90fc 005c9a95 005cacb9) and GetDef<CThingObjectDef> 101 -> 98 B (00584b86 005b3aef 005b76d1 005b8743), each equal to the
  owner's object; GetDef<CThingBaseDef> also turns EQ in 9 of them. The 101 v 98 B difference is the default SetDynamicCast, not the
  optimize state (Step 1 in NOTES.md).
- **b2 (005cdd86)**: NF352's RESET_SIZE_BODY in place (this row includes CCountedPointer.h above its pragmas; pragscan clean): Reset 62 -> 59 B.

Rulings (advisor decision, owner delegated, 2026-11-24): Q1 REREAD alone is an existing opt-in; Q2 the helper changes are judged by the drop
test (coordinator run, below). P2 (include move) rejected: not byte-neutral (`??0CCPPointerInfo` 26 -> 25 B). P1 (a y-off sibling of the
NF352 Reset block) approved with conditions, for agent NF364. 005a29f4 / 005a9cbe / 005aaf28 / 005ad571 keep their 62 B Reset copy (HEAD).

Coordinator verification, HEAD and repo builds both by the coordinator (`$S/v362H.sh`, `$S/v362N.sh`):
- vland `--recheck-landed`: 18/18 RELOCATION_MATCH PASS before and after; identical WARN sets.
- pk_all `--leaf` HEAD v repo: per row only the claimed GetDef / Reset copies turn NEQ -> EQ; 0 new NEQ; reachflip 0 NEQ flips (18 pairs);
  pickany_vs_retail 0 new NEQ lines.
- Drop test (`$S/tools/droprow.py`, logs/droptest_coordinator.txt): 64 dropped externals: 52 `?Assign@?$CDefPointer@...` helpers and 12
  UND, all with no retail address (pk_all NOADDR) and no reloc_bindings line; 12 `?A0x` names differ only by the build-path namespace hash
  (all reappear after normalising the hash).
- T3 nf307 0x00584B86 (an unrelated binder port) was parked during the landing and restored in place (queue identical).
