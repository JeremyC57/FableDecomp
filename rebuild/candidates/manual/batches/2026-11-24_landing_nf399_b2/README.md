# Landing nf399 b2: 0x004DF54F is ~vector<const CSteeringAgent*>, landed as one 3-row unit

Rows:
- 0x004DF54F (unit; MATCH): respelt from `_Forward` / `??1CMember_004df54f@@QAE@XZ` to `??1?$vector@PBVCSteeringAgent@@...@QAE@XZ`, the 5-byte `jmp` to ~_Vector_base at 0x004DF554.
- Binder 0x004DF507: its model gains the real vector member at +8.
- Census port 0x004DF365: takes opt-in `FABLE_RETAIL_STLP_VECTOR_BASE_DTOR_DECL_ONLY` (stlport/fable_containers.h). This hunk was merged onto nf398_b1's header; the two sit in different `#if` chains and no row uses both.

Table changes (0 misses; the multi-target count stays at 1):
- the stand-in line is deleted and the ~vector line added (witness 004df507);
- self_symbols carries one line for 004df54f, replacing the old one. The members' selves are unchanged, so they get no lines.

Rulings (advisor decision, owner delegated, 2026-11-25; pending/2026-11-25_nf399_handback/RULING_Q1_Q2.md):
- Q1, land as a unit: 0x006CC1E0's unreferenced (.debug$F-only) 14-byte ~vector copy becomes NEQ, booked as debt.
  - Its leaf matches only with the inline ~_Vector_base. NF399 measured both in-row fixes (p5a and p5b) and both fail.
  - Precedent: NF328 D1. R5 does not apply.
  - In object-mode builds a jmp definer must precede 0x006CC1E0 in link order (T3 nf399 0x006CC1E0).
- Q2, accepted: two scaffolding sections in 004df507 change (implicit ctor 9 -> 20 B, MakeOne 22 -> 33 B).
  - NF400 found no retail counterpart for the 20-byte ctor, so it is booked as debt (T3 nf400 0x004DF507).

NF400's pre-landing checks (pending/.../nf400_checks):
- C1 PASS:
  - The only retail reference is `call 0x004DF54F` at 0x004DF513 (inside 0x004DF507).
  - 133 rows reach the type; VC7.1 grades are identical on HEAD and the new table (28 MATCH, 105 RELOCATION_MATCH).
  - The only undefined reference is 004df507's leaf.
- C2 PASS: no `CMember_004df54f` user remains.
- C3: no match; debt.
- C4 PASS:
  - sizeof is 0x14: retail `push 14h` at 0x004DF366 and 0x004DF4CB.
  - The vector is at +8 (`lea ecx,[esi+8]` at 0x004DF50A; Ego_r DummyAgentList +8).
  - The 12-byte STLport vector replaced the 4-byte stand-in. Ego_r's 0x18 comes from its 16-byte Dinkumware vector.

Coordinator checks after landing:
- 3 of 3 landed; 0 LITERAL_DIFF.
- Recheck on the merged header: 0x0064E978, 0x0064AE80, 0x006CC1E0, 0x004DFB8F and 0x00813460 are all RELOCATION_MATCH PASS.
- Baselines: bindings multi 1, funclets 0 broken, S units 3 OK.
- T3 nf396 0x004DF54F closed (T3_CLOSED_BY_HAND.tsv); the nf399 0x004DF507 port line is kept.

NAME_FIX queued: 0x004DF4E7 is the copy ctor `??0CObstacleNavigatorExternalInterface@@QAE@ABV0@@Z`. Sources: FableWin.pdb and retail Clone. Its row needs a re-port (T3 nf400).
