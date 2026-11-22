# 0x00A4EFC0 -- AUTH11 log (Step A of S/w22/auth11/BRIEF.md; ruling: ADVISOR_DECISIONS_B last entry, 2026-11-22)
1. CHECK (no grading): CHECK.md. overflow_check is used by Ego_d line 25 (the 16-B size test; VC10-calibrated, v10/at.cpp), not
   only by the 261-B assert; FableWin.pdb shows it in all 12 instantiations; Ego_r.pdb's one out-of-line copy (release) has only
   `this` / `elements` (local optimised away; confirms the parameter name). => step 3.
2. Overlay: hdr/ = AUTH7's overlay (S/w22/auth7/00a4efc0/hdr) re-based onto the CURRENT repo headers (HEAD 0fbb3d82fa) with
   git merge-file against its base 8e771c0ec0 (all six merged clean; base copies in orig/, old base in base/). Added (add-only) the
   opt-in FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD_LOCAL in engine/CSimpleMemoryPool.h; pin_asserts: pinned 0.

## PRE-REGISTERED (written before any grade)
- Candidate c1_00a4efc0.cpp = AUTH7's b3_00a4efc0.cpp with its one define FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_IFBODY
  replaced by FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD_LOCAL. That opt-in is Ego_d's exact shape: parameter `elements`,
  block local `unsigned __int64 overflow_check = (unsigned __int64)elements * sizeof(T);`, `if (overflow_check > 0xffffffff)
  { FABLE_RETAIL_ASSERT(false); return 0; }`, `return (T*)Allocate(elements * sizeof(T));`, `return 0;`. It differs from
  IFBODY only in the named local and the parameter name. Nothing else changes; no re-fit afterwards.
- Control (overlay staleness after the re-base, not a candidate grade): b3_00a4efc0.cpp as is under hdr/; expected DIFFER 16.
- Grades: control once; c1 once (/Gr, FABLE_HDR_OVERLAY=hdr); order readout once (ord.sh c1); then each user once with the SAME
  opt-in defined after `#define FABLE_RETAIL_CSIMPLEMEMORYPOOL` in a copy of the landed source (users/<addr>_c1.cpp), same overlay:
  0x00A50510, 0x00AA0FA0, 0x00BD2D90.
- Acceptance (fixed now): c1 is MATCH or RELOCATION_MATCH AND all three users are MATCH / RELOCATION_MATCH. Then a land-ready
  hand-back; otherwise OWNER_PACKET.md. Either way the row stops here.
- Expectation stated in advance (not a criterion): AUTH2's V13 (same structure, names `count` / `total_size`) gave row 16 and users
  35 / 15 / 9, so a no-flip result is expected.

- Control contingency (added before grading, advisor): if the b3 control is not DIFFER(3491v3491) residue 16 on the same position-
  compare hunk, c1 is NOT graded; the moved baseline is reported first. User witness: before any user result is believed, the
  user copy is preprocessed under the same env and must contain `overflow_check` (proof the opt-in was live). Readout: h/c1.il.json
  must exist and be fresh (size near AUTH7's b2.il.json, 1.89 MB) or the readout is a tool failure (two tries at most). The
  readout and the three user grades are done whatever c1 gives.

## Results
3. Control b3 (hdr/ re-based): DIFFER(3491v3491); udiff identical to AUTH7's b3.q.out (= residue 16, same position-compare hunk).
   Overlay current; contingency not triggered.
4. c1 graded ONCE: DIFFER(3491v3491), built bytes identical to the b3 control (residue 16, same hunk). Witness: pp/c1.i holds
   `overflow_check` (opt-in live).
5. Order readout ONCE (ord.sh c1; h/c1.il.json fresh, 1,891,058 B): compressed-first in all three blocks (retail: compressed /
   uncompressed / compressed). position c=0380 ps=03a0 pf=4821 qf=3301 qs=02e0 L=9b9b R=398a L>R; scaling c=22c0 pf=4881 qf=3361
   L=bc1b R=3a6a L>R. (b2/b3: pf 3781 qf 3661 L=7a5b R=3cea.) The local re-draws the finish fields (+0x10a0 / -0x360) but no flip.
6. Users, ONCE each, same opt-in (witness: each preprocessed copy holds `overflow_check`):
   0x00A50510 DIFFER(1748v1748), 0x00AA0FA0 DIFFER(4460v4460), 0x00BD2D90 DIFFER(721v721). Each built object is byte-identical to
   AUTH2's V13 (`count` / `total_size`) built object, so the residues are AUTH2's: 35 / 15 / 9. The names have no code effect.
7. Acceptance NOT met (row no flip; all three users break). => OWNER_PACKET.md. b3 stays the recorded best text. Row stopped.
   Grades used: 5 (control, c1, three users); readouts 1; VC10 measurements 2.
