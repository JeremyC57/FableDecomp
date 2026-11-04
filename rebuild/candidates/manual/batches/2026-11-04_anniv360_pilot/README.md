# Pilot 2026-11-04: do the Anniversary Xbox 360 Test PDB's locals help parked TLC rows match?

**Answer: no, not usefully.** One agent took 20 parked rows (DIFFER) that have an exact-name procedure in
`WellingtonGame-Xbox360-Test.pdb`. It rewrote them following the 360 parameters and locals (types, order, line table), then graded
the baseline and best versions with `vland.py` without landing anything (`RESULTS.tsv`, `NOTES.md`).

- **Matches: 0.**
- **Improved: 2**, both still DIFFER, and in both the 360 data was only part of the fix (`drafts/`):
  - 0x00914650 `CAIStateGroup_BirdStartled::IsRunnable`: 72 -> 6 differing instructions. The 360 `C2DBoxI area` local led to the
    `C2DBoxI(C2DCoordI, radius)` constructor. Closing the frame gap needed an inner scope, which the 360 file does not record.
  - 0x009216e0 `CAIStateGroup_MinionFollowThing::IsRunnable`: 94 -> 45. The fix was again scope placement, inferred from retail.
- **Unchanged: 18.** On 7 of them the rewrite the 360 data suggests compiles to identical code under VC7.1.
- **Why it can't do more** (coordinator check on all 44,972 Fable procedures, `tools/t4.py`):
  - Register records never outnumber the function's parameters, so they are parameters only. A local appears only when it lives
    on the stack (22,239 such records).
  - The whole file has 13 scope (block) records.
  - Symbol order is not declaration order.
  - Anniversary code also drifted: 3 of the 20 functions were rewritten or stubbed, and 1 exact-name match is the wrong template
    instantiation.
- **Keep:** the `C2DBoxI(C2DCoordI, radius)` form is a recipe worth trying on box-heavy parks (0x004FEB70, 0x00936C70). It helped
  1 of 5 rows that tried it. The 360 file stays a lookup for local types and original source files, under the usage rule in
  `docs/re_reference/FABLE_ANNIVERSARY.md`.
- Tools: `tools/proc_dump.py` (procedure params/locals with types and lines), `tools/cls_dump.py`, plus the coordinator's measurement
  scripts (`t12.py` name matching, `t3.py` class member comparison, `t4.py` register/scope check, `procs2.py`).
- The retail-byte oracle and the full 360 dumps stay in scratch.
