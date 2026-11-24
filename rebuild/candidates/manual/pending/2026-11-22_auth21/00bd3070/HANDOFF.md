# 0x00BD3070 HANDOFF (AUTH21)
- State: ESCALATED (ESCALATE.md), with the advisor's question open.
- Best rule-clean source: `00bd3070.cpp` (= c6), DIFFER(11288v11288), rc 151, built hash 506c5c674d66. Stock flags, overlay `hdr/`,
  header patch `hdr_00bd3070.patch` (add-only plus one pinned assert). Grade with `g.sh <src> <tag>` (OV= overrides the overlay).
- Bracket: `p2.cpp` (b6 + C3DVECTOR_CTOR_FORCEINLINE, row-local PeekGraphic `begin()[index]`), rc 129. The evidence leans against it.
- Every row-local FABLE_FORCEINLINE member body was moved to a header opt-in. The table is in NOTES.md. One pre-registered fix
  (C3DVector ctor FORCEINLINE) closed the only unexpanded call.
- What is left: 13 commutative operand picks (11 test/or, 1 x87, 1 SIB), plus jump-table and relocation-display hunks.
- In flight: equiv_check over all landed rows, old = repo rebuild/include, new = `x/newinc` (repo + overlay). Status: running (PID 29668). Output
  `equiv.tsv` (one line per differing row; empty = equivalent) and `x/equiv.out`. Rows it lists need recheck_rows.py. When it ends,
  `x/newinc` (21 MB) can be deleted.
- Not graded, not pre-registered: nothing pending. Possible next step: the GFGetTopBitInByte home needs a header claim.
