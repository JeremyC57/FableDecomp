# acctB hand-back, near-miss retry chunk 3005 (2 previously parked rows: 0 matched, 2 still parked, new measurements)

Worked by an Opus 5.5 agent starting from the parked sources of r3001 and r3002; the acctB local coordinator regraded both
parked sources. Nothing is landed and there is no row to land here: this folder records what was measured.

| address | result | notes |
|---|---|---|
| 0x00A9EB90 C3DAnimationManager::BlendAnimations | still parked, DIFFER 1337v1337 | one instruction: 0x00A9EC5A retail `fld dword ptr [eax+edx]` (d9 04 10), ours `[edx+eax]` (d9 04 02). Mechanism measured with c2 hooks: the SIB index is the address operand with the smaller c2 symbol-record id mod 1024 (records come from 32-record chunks numbered at creation, c2.dll 0x107603DD); ours has the to-pointer IV at 1021 and the spilled offset at 1064 (= 40), retail's shape needs all three SSE-loop IVs past 1024, i.e. about one more chunk of records allocated after the SSE inline locals and before loop strength reduction. Dead-code diagnostics reach retail's bytes (8 dead temps in SetSlerp, or 6 in each GFFastSin expansion); no natural spelling tried does (list in NOTES.md). The r3004 outer-name sweep alone does nothing here |
| 0x00550DC0 NUISystem::CTable::Draw | still parked, DIFFER 1399v1399 | frame slots only (20 masked lines), unchanged: retail 0x00550EFE `fstp [esp+74h]` / 0x00550F53 `fstp [esp+3Ch]`, ours 3Ch / 74h; retail 0x00550F59 `[esp+38h]` / 0x00550F63 `[esp+34h]`, ours 34h / 38h. Outer names k = 0..16 never move these stores (not the r3004 key). Byte- and slot-neutral: the PDB member `operator*`, a converting constructor through `operator=(const CRGBColour&)`. Named parent / table colours change the code |

Budget note: 1 grade per row (the baseline regrade); **passes 2 and 3 were not used on either row** -- about 700 free trace
compiles found no natural source that reaches retail's state, so there was nothing worth grading. Neither row is proven
unmatchable. Both stay in the acctB near-miss queue; the next attempt on 0x00A9EB90 should look for the original nesting
of the SetSlerp / GFFastSin helpers (a construct that keeps about 32 more records alive during the scalar-loop inlining).

`rows/parked/` holds the unchanged best sources (new `// Grade:` lines) and their tests (both pass under g++ and VC7.1 per
the agent; I regraded both sources: DIFFER as recorded). The overlay they need is r3001's (`hdr/` of
`2026-11-11_acctB_r3001`, CMatrix3x4.h superset) plus r3002's; not repeated here. `NOTES.md` has the full sweep tables,
`LESSONS.md` the record-id mechanism for GOTCHAS. Observation for header merges: FableWin.pdb has CRGBFloatColour's product as
a const member `operator*`; the repo header models a free `operator*` (byte-neutral in this row).
