# NF437 hand-back (2026-11-25): the CVectorMap LowerBound split, round 1

`NOTES.md` has the method, every gate, and the plan for the rest.

## Landed: 39 rows (`batches/2026-11-24_landing_nf437_lb1`)
- **What changed.** Each row's own stand-in map model is replaced by the shared `CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >` from `engine/CVectorMap.h`, the form 1,725 landed rows already use. The call now binds the established `LowerBound` spelling at 0x0040F020 / 0x004365B0.
- **What did not change.** Each row's own symbol is unchanged; this is a callee-only respell.
- **Headers:** none changed, so the coordinator's equivalence rerun did not apply (owner speed rule).
- **Gates:**
  - vland: RELOCATION_MATCH on both sides;
  - objsec: equal under the rename;
  - pk: 0 new NEQ, and 4 rows improve;
  - /FORCE duplicates go from 7 to 0;
  - name_gate is no worse than HEAD (R-NF435-1).
- **Table:** 16 stand-in lines deleted. Two lines are rewitnessed to held rows; the coordinator applied those by hand, because the lander skips held witnesses.
- **R7 (coordinator):** seven rows emit row-written inline helper COMDATs with no retail home that nothing references. They replace equivalent unreferenced stand-in COMDATs HEAD already emitted, so the swap is like-for-like. Booked as the nf437 helper-COMDAT debt line.

## Effect on the tracker
None yet. The two addresses still carry 14 stand-in spellings, so they stay split.

## Left (T3 nf435 LowerBound split line)
- 3 held rows (0x006CE730, 0x0085B5B0, 0x007E5230).
- 9 rows on other T3 lines, with 4 portable partner rows.
- 0x0089EC60, after the 0x0074AC80 rename (re-verified as `CTCCreatureGenerator::AddCreatureTriggerer`; plan in NOTES).
- 0x00898BB0.
- Tasks 2 and 3 (the 0x0074AC80 / 0x00A36FE0 rewrites, NF435's residue) were not started.
