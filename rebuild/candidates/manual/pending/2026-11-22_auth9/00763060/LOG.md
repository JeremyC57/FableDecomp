# 0x00763060 NShape::FractureShape (AUTH9, 2026-11-22). Graded with VC7.1 3077 via q.py, /Gr. Variants in t/.
- g0: repo 0x007634E0 row as-is, leaf UpdateSubdivisions: RELOCATION_MATCH; leaf FractureShape at 0x00763060: RELOCATION_MATCH (baseline).
- A: FractureShape alone, callees 0x00760020/0x00760190 declaration-only: DIFFER(1164v1146). Same-TU callee knowledge changes the caller.
- B: 0x007634E0 with all three NShape helpers declaration-only: DIFFER(639v627) (frame 0x30 v 0x34).
- C: 0x007634E0 keeping callee copies, FractureShape declaration-only: DIFFER(639v627). => strong TU-mate copy route (GOTCHAS nf96a).
- D: FractureShape + Codex's callee copies (not owner-verbatim): RELOCATION_MATCH.
- E: FractureShape + owner-verbatim copies of 0x00760020/0x00760190 (+FABLE_RETAIL_C3DVECTOR_CROSSWITHUP): FractureShape, 0x00760020,
  0x00760190 all RELOCATION_MATCH. pickany_vs_retail (codex27 tool): 11 EQ, 0 NEQ.
- F: 0x007634E0 with owner-verbatim callee copies: UpdateSubdivisions RELOCATION_MATCH.
- G1 (only 0x00760020 declared): DIFFER(1156v1146); G2 (only 0x00760190 declared): DIFFER(1164v1146): both copies licensed.
- H (no fill_n device): RELOCATION_MATCH, leafrel 0 problems, pick-any 11 EQ. I (also no CBasicShape/SubdivisionTriangle headers and their
  opt-ins): first SRC_FAIL (std::distance comes from PASS22_LOWER_BOUND_*), with those kept: RELOCATION_MATCH, leafrel 0, pick-any 11 EQ.
- Final NShape_FractureShape_00763060.cpp (I + header, `__fastcall` owner tokens): rg1 RELOCATION_MATCH. Test PASS (first run FAIL on my
  wrong expectation: it_begin advances one per pass, so p3 is offered twice).
- 0x007634E0: P1 / P2 RELOCATION_MATCH; P2 copies at 0x00760020/0x00760190/0x00763060 RELOCATION_MATCH; pick-any 18 EQ both; tests PASS.
