# Landing unit 2 (Account A, 2026-11-24): 39 more rows onto their PDB symbol together with every landed caller (93 sources)

Second wave of `unit_fix.py`, landed with `land_scripted.py` (unit-atomic, staleness-checked). The route and proof are as unit 1
(`batches/2026-11-24_landing_unit1/README.md`). What is new: units that share a source are no longer skipped. Each run takes the first
unit per shared source and defers the rest to the next wave.

**Run unit2:**
- 156 unit rows, 121 self edits accepted, 113 within the caller cap.
- 41 units accepted: 97 sources. 51 units deferred to wave 3.
- Held at landing, each as a whole unit:
  - 0x004FCB30 with 0x00502850: stale, because rename1 changed 0x004FCB30's source after the run;
  - 0x004166E2 with 0x00417001: a claims line from 2026-11-20 still marked handed-back names the address.
- Table: 39 replaces, 0 misses, no new multi-target symbol.

## Recheck
`logs/recheck.log` (landed tree, 93 rows): 93 MATCH / RELOCATION_MATCH PASS; every self grades its PDB symbol.
