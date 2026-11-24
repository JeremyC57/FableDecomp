# Landing unit 3 (Account A, 2026-11-24): 59 rows onto their PDB symbol together with every landed caller

The third wave of `unit_fix.py`, landed with `land_scripted.py` (unit-atomic, staleness-checked). The route and proof are as unit 1
(`batches/2026-11-24_landing_unit1/README.md`).

**Run unit3** (`unit_fix.py` in a fresh directory, after the 2026-11-24 A/B sync at f2464fe5d7):
- 117 unit rows; 82 self edits accepted; 74 within the caller cap.
- 27 units accepted: 61 sources, 27 table lines. Skipped (`SKIPPED.tsv`): 26 deferred to the next wave (shared source), 18 with a caller
  edit not accepted, 8 over the caller cap, 2 table, and 1 other caller case.
- Held at landing as a whole unit: 0x004166E2 with its caller 0x00417001. A lane lock from the 2026-11-20 repair pass (RP1) and an
  open 2026-11-20 A claims line (CVsyncCallbackTimer option A, status handed-back) both name the address. unit1 and unit2 held the same
  unit. It needs that claim closed first.
- Table: 26 replaces, 0 misses, no new multi-target symbol.

## Recheck
`logs/recheck.log` (landed tree, 59 rows): 59 MATCH / RELOCATION_MATCH PASS; every self grades its PDB symbol. Its SHARED_HEADER_AVAILABLE
warnings are pre-existing local models in caller rows; the unit2 log has the same class, 457 lines.
