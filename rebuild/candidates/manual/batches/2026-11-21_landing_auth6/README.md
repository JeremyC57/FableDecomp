# Landing auth6 (Account A, 2026-11-21): 0x007602F0 NShape::IsShapeConvex (1 row, RELOCATION_MATCH /Gr)

AUTH5 (`pending/2026-11-21_auth5/`) took the row to a byte match on Ego_d's text; AUTH6 (`pending/2026-11-21_auth6/`) decoded the two
Ego_d lines AUTH5 left open. Owner order: no parking. Advisor rulings: ADVISOR_DECISIONS_B 0x007602F0 (p25Q) and 2026-11-21 (this file's labels).

- Text: Ego_d's, measured with VC10 SP1 (`odall10.py --lib fablelib --stl`): all 27 Ego_d lines, the length (1096), the named frame and
  the loop block equal Ego_d. The old three-float edge text stays rejected (66 / 86 B v Ego_d 24 / 44).
- Inferred (labelled in the source): FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST (new add-only opt-in in engine/GFAngles.h, the PAREN family;
  the default body leaves 12 lines), and the spellings of Ego_d 313 (`!= false`, 100 B) and 342 (`(float)(result * result)`, 122 B):
  Ego_d fixes their IL family, not the spelling, and VC7.1 builds the same bytes with or without them (0x008DBC80 size test).
- Existing opt-ins: C3DVECTOR_NORMALISE_SQUARE_HELPER + _SQUARE_GROUPED_PASS23, STLP_VECTOR_ORIGINAL_FRONT_PASS22, GFANGLES_SIGNED_ROUNDING.
- Contract (p25Q ruling): the four landed GFArcCos users (0x006AB820, 0x007C0690, 0x00852E10, 0x00A58130; grep complete, no other inline
  reaches GFArcCos, the TU helpers 0x00760020 / 0x00760190 do not use it) regrade MATCH with PAREN_CAST defined (pending/2026-11-21_auth5 gfreg).
- Recheck (`logs/recheck.log`): 48 rows = the direct includers of GFAngles.h, every landed tc_geometric_shapes row and the row: all
  MATCH / RELOCATION_MATCH + PASS. Link (`logs/link_check.txt`): no new duplicate; the pick-any GFArcCos copy equals retail 0x005DB180.
- Landed with the default lander: strict mode rejects only on the `_CIacos` import-thunk alias warning (RELOC_ALIAS), which the other
  landed _CIacos users carry too. Catalog flags written in full (`/O2 /Oy /W3 /Gr`).
- 0x00763060 (the pair's other row) is NOT landed: owner question (`pending/2026-11-21_auth6/00763060/ESCALATE.md`).
