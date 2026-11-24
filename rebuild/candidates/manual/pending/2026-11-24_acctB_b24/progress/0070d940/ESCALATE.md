# 0x0070D940 -- escalation notes (B24C7, 2026-11-24; replaces B24C6's compile-order hypothesis)
- B24C6's candidate (compile order / budget path for SetStartState, SetEndState, ~vector, overflow) is REFUTED by a forced-budget trace
  (inl/f1.txt): forcing those callees down c2's budget path expands the pre-loop SetStartState / SetEndState / push_back that retail calls.
  B24C6's competing one (bigger inline bodies) cannot act either: pen = 0 at every decision (caller growth <= ~4070 v the 9000 gate).
- Three of the four sites are explained and fixed by source shape (v3: transitions loop written as a while; byte evidence in NOTES.md).
- Open: the first combination pass's vector<CAnimSetIt>::_M_insert_overflow (retail calls 0x00711080 at 0x0070DF89, expands the second
  pass's). Our trace: cost 73 (c1 52 + ext 21), f 1.25 -> score 68 (< 74, expanded); second pass f 31.25 -> 49. Retail needs the first
  site at f <= 0.91 (with cost 73) or cost 80..109 at f 1.25. No loop shape tried makes the site colder (NOTES.md B24C7).
## B24C7 update (2026-11-24): devices in the current draft, and the TU-composition fact
- `/d2inlT67` (diagnostic until the row matches; EXTRA_RULES 2026-11-08 preference 2): the first combination pass's
  vector<CAnimSetIt>::_M_insert_overflow scores 68 (cost 73 = c1 52 + ext 21, f 1.25); retail calls it (0x0070DF89) and expands the
  second pass's (our score 49). Our COMDAT copy is instruction-equal to retail 0x00711080, so the callee cost is not the difference.
  No loop shape tried makes the site colder (while / guarded do-while / for(;;)+break). Max score that must stay expanded: 66; T67 and
  T68 both give retail's call sequence.
- Forced converting operator= (hdr CIVCountedPointer.h CONVERT_ASSIGN under ASSIGN_FORCEINLINE): retail expands
  `transition = transitions.back()` (op= cost 46 -> score 144 at f 0.0625) and CALLS ~vector (cost 36 -> score 113) in the same block:
  no single threshold fits both, so the expansion is forced like the existing same-type ASSIGN_FORCEINLINE.
- TU-composition fact (diagnostic c7/bis/d1.cpp = v10 + a same-TU stand-in body for CAnimSet::GetMeshAnim under auto_inline(off),
  NOT a candidate): the zero register disappears (`cmp r,ebp` 20 -> 1), the function shrinks 821 -> 794 instructions (retail 785),
  normalised-equal 451 -> 556. Probe c7/gp/p6.cpp v p4.cpp: a by-value-returned CIVCountedPointer keeps its scope-end null store when
  the callee is only declared, and drops it when the callee's body is in the TU. Retail's GetMeshAnim 0x0070C900 lies ahead of the row
  (the landed row is a separate object here), so retail's c2 knew it, and `start` / `end` were not treated as aliased. This is the
  residue the owner's retail-grouping build mode should remove (with 0x0070C900 compiled first in the same object).
