# Landing unit 5 (Account A, 2026-11-24): 10 rows onto their PDB symbol together with every landed caller

The fifth wave of `unit_fix.py`, landed with `land_scripted.py` (unit-atomic, staleness-checked). The route and proof are as unit 1
(`batches/2026-11-24_landing_unit1/README.md`).

**Run unit5:**
- 79 unit rows; 44 self edits accepted.
- 5 units accepted: 10 sources, 5 table lines.
- Skipped (`SKIPPED.tsv`): 10 deferred to the next wave (shared source), 18 with a caller edit not accepted, 8 over the caller cap,
  2 table, and 1 other caller case.

**Landed:** 10 rows, none held. Table: 5 replaces, no misses, no new multi-target symbol. `logs/recheck.log`: 10 MATCH / RELOCATION_MATCH
PASS, and every self grades its PDB symbol.
