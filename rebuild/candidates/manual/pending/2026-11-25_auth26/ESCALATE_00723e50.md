# 0x00723E50 CTCPhysicsStandard::SetRHSetAsFlushOnNormal: ESCALATE (row 2)

Best: DIFFER(310v310). Grades used by AUTH26: 5 (g0..g4); Codex pass 26 before: 156+ with the same front residue.

## Questions for the advisor
1. **Base legality.** g0 (Codex's final) is clean except the front only because of FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_SEED_FIELDS_PASS26,
   a memcpy device with an extra `bits` local Ego_d does not list (Ego_d: n, seed, r). Is that device admissible? If not, the legal
   base is g2 (front + one tail frame-slot residue).
2. **The front residue is one cause** (NOTES.md): retail's order follows from best-ready-first exactly when the x87 chain is NOT
   eligible in the first slot after the GetFacingAngleXY call; ours when it is. Only Ego_d lines 453-454 sit between the call and the
   subtraction (`float xy = GetFacingAngleXY();`, `C3DVector right_2d;`), and both are checked against the PDB / FableWin (float const
   return, empty ctor). Every downstream respelling is ruled out by construction.
   What source construct makes VC7.1 3077 delay the first x87 op after an FP-returning call by one slot? Candidates I could not back
   with evidence (so did not grade): xy through an inline wrapper / extra assignment node; the call result as a conversion.
3. If the answer is "measure first": may I run the c2probe ready-cycle readout on g2 (an oracle, not a grade) to settle whether fsub
   is eligible at slot 1, and grade one candidate per finding?
