# Landing auth9 (Account A, 2026-11-22): 0x00763060 NShape::FractureShape (1 row)

Authoring agent AUTH9 (owner: no parking). Hand-back: `pending/2026-11-22_auth9/00763060/` (NOTES, LOG) and `007634e0/` (patches, pick-any).

- 0x00763060 lands RELOCATION_MATCH (1146 B, /Gr), test PASS, under LANDVERIFY_STRICT_HEADERS=1 with no warnings (`logs/`).
  - Text: Codex pass 26 (in 0x007634E0's TU). AUTH8 measured it Ego_d-exact (VC10) on lines 195-241 and all 23 named locals.
  - Leaf: verbatim in Ego_r.pdb (release tc_geometric_shapes.obj) and FableWin.pdb.
  - Pick-any copies: 11 EQ, 0 NEQ (AUTH9).
- 0x007634E0 gets AUTH9's P1, a Fidelity comment only: its FractureShape stays a strong same-TU copy. Declaring it instead measured
  DIFFER on both rows (new row 1164v1146, 0x007634E0 639v627).
  - The new row likewise keeps strong owner-verbatim copies of 0x00760020 / 0x00760190. Removing either one measured DIFFER.
- Recheck (`logs/recheck.log`): all 40 NShape rows in 00/76, both rows included, are MATCH or RELOCATION_MATCH + PASS.
- Link (`logs/link_check.txt`): the duplicates are the accepted same-TU kind (GOTCHAS nf96a): FractureShape x1,
  IsLineIntersectingWithOthersInList x2, IsTriangleWithPointsInside x2.
- reloc_bindings: 4 first witnesses added. The `_M_insert_overflow<vector<C3DVector>>` line for 0x00765B70 was held, because it would add a
  second spelling beside the July placeholder `InsertGrow@CRetailVector_CElement12_006f0810`. Queued (land1122auth9), with the
  one-definition note.
- No header change. AUTH9's optional P2 (owner-verbatim callee copies in 0x007634E0, all RELOCATION_MATCH) is kept in the hand-back, not applied.
