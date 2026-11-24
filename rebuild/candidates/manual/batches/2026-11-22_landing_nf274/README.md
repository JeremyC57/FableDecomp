# Landing nf274 (Account A, 2026-11-22): 0x00BEB490's ??_GCWaterPatchMesh copy made retail's, and D4's four duplicate helpers made pick-any (3 rows)

Agent NF274 (entries 1 and 2 done; entry 3 scouted, re-queued). Hand-back: `pending/2026-11-22_nf274_handback/`. No header, table, name or
binding change (RENAMES / TABLE_CHANGES / NAME_FIXES empty).
- **Entry 1, 0x00BEB490** (nf272 ruling 1): the ctor row now includes 0x00BEAAA0's landed file first (the nf251 fold pattern; retail keeps
  ~CWaterPatchMesh 0x00BEAAA0, the ctor and ??_G 0x00BEB4D0 in one TU), then its unchanged constructor. Ctor leaf RELOCATION_MATCH, cmpobj SAME
  as HEAD; both emitted vtables (0x012A8134, 0x0129B860) slot-for-slot HEAD's. The TU's `??_GCWaterPatchMesh` pick-any copy goes from 30 bytes
  (calling ::operator delete, pk NEQ) to 100 bytes, pk EQ to 0x00BEB4D0 and SAME as the 0x00BEB4D0 / 0x00BEAAA0 objects' copies; link check
  over every emitter (0x00BEB490 / 0x00BEAAA0 / 0x00BEB4D0): every link order now keeps the 100-byte body in slot 0 (nf272 recorded the race).
  Cost: the ctor object also defines `??1CWaterPatchMesh@@UAE@XZ` strongly, body identical to 0x00BEAAA0's (one more equal-copy LNK4006).
- **Entry 2, D4 (0x00BDA150 / 0x00BD9CE0), shape S1:** `inline` on the four out-of-class helper definitions in each row's existing local model
  (`// Fidelity: scaffolding, not retail`). Both objects objsec ALL_EQ to HEAD, cmpobj 49 / 49 SAME; only COMDAT selection moves NODUPLICATES ->
  ANY. Pair link: 4 duplicates -> 0. FreeVirtual's NEQ pick-any copy (68 v the owner 0x00BD9B90's 97) still meets the owner's strong
  definition (1 or 2 by link order, as at HEAD). Probe P2 (not authored): the helper bodies with FreeVirtual declared only give 29 / 29 / 28
  bytes, pk EQ to retail 0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 (VB / IB still by Ego_r order only: the two bodies are byte-identical).
- **Recheck** (`logs/recheck.log`, landed tree): the 3 rows + 0x00BEAAA0 / 0x00BEB4D0 / 0x00BF5A30, 0x00BD9B90 and the queue rows
  0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 (10 rows): 10 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.

## Coordinator rulings on NF274's questions
1. Entry 1 lands. The extra `??1` is an identical-body equal copy (accepted debt, nf251 ruling 1 shape) and it removes the wrong-deallocator
   race. Two rows (0x00BEB4D0, 0x00BEB490) now include `CWaterPatchMesh_CWaterPatchMesh_00beaaa0.cpp` by path: a rename of that file must
   update both (funclet_includes_check catches it).
2. S1 lands. S3 (S1 + `/d2inlT114`: FreeVirtual / Deallocate copies EQ, helper copies 101 / 101 / 104) is not taken now: its files were never
   graded through vland and need regenerating; queued with the owner-side follow-up (0x00BD9B90 to pick-any, nf88).
3. Entry 3 re-queued as a unit: 0x00BFDE20 as a self rename (`??1CLandscapeLayerMesh@@QAE@XZ` -> `UAE`, both PDBs; the table already binds UAE
   there) plus its vtable 0x012A835C / `??_G`, with 0x00BFDD70 / 0x00BFDE80 as base ports.
Slips (two short entry-1 link batches in the foreground): noted.
