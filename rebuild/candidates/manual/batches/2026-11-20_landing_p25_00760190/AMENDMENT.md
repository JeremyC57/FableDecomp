# Amendment to the landed 0x00760020 / 0x00760190 (p25L, after the advisor's handback review)

Main landed both helpers (b82d5ea9a and the commit before it) from the first versions of the batches. This amendment changes no bytes; both rows stay RELOCATION_MATCH on the repo headers as they are now. Full batch dirs: ../00760020, ../00760190. Their payload, test and strict vland were redone, but vland now skips both rows (WINS: 0) because they are already landed.

1. **Same define block for both TU-mates** (00760020.src.diff, 00760190.src.diff): C3DVECTOR, C2DVECTOR, C2DVECTOR_FROM3D, C2DLINEF_FROM3D, C2DLINEF_INTERSECTS_LINE, C3DVECTOR_CROSSWITHUP.
   - Before this, each was graded alone under a different set.
   - In the FractureShape TU (parked/00763060, every define), cmpcomdat shows both helper COMDATs SAME as retail (relocations only).
2. **0x00760190: perp = line.CrossWithUp()** (existing opt-in FABLE_RETAIL_C3DVECTOR_CROSSWITHUP; FableWin/Ego_r ?CrossWithUp@C3DVector@@QBE?AV1@XZ, body (Y, -X, 0)).
   - Its /Od line is 12 B, exactly Ego_d's line 159; the spelled-out constructor is 23 B. The bytes are identical.
   - With perp at 159, edge sits at Ego_d's line 157: 23 B there because the VC10 class iterator makes *it a call; 16 B in our /Od.
   - The landed statement order is therefore Ego_d's order, so the `// Inferred:` line becomes an `// Ego_d shape` line.
   - Ego_d line sizes v ours, line by line: 156: 16 v 16 (line); 157: 23 v 16 (edge); 159: 12 v 12 (perp); 161: 6 v 6 (circ test); 163: 18 v 13 (*=); 166: 29 v 28 (dot test).
   - Mutants were rerun with CrossWithUp mutants; see ../00760190/mutants.
3. **Header comment attributions** (hdr_comments.diff, comments only): NShape.h and the two C2DLineF.h opt-ins now name the landed rows 0x00760020 / 0x00760190 instead of the parked 0x00763060.
4. **newly_flagged_rows.tsv** (computed by grep, not asserted): landed 0x0075FE30 and 0x00763F50 carry local copies of NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation / ECirculationOrder, so both are re-port backlog. The first batches shipped this file empty.
