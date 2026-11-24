# Landing unit 4 (Account A, 2026-11-24): 25 rows onto their PDB symbol together with every landed caller

The fourth wave of `unit_fix.py`, run after the RP1 landing, landed with `land_scripted.py` (unit-atomic, staleness-checked). The route and
proof are as unit 1 (`batches/2026-11-24_landing_unit1/README.md`).

**Run unit4:**
- 91 unit rows; 56 self edits accepted; 48 within the caller cap.
- 12 units accepted: 25 sources, 12 table lines.
- Skipped (`SKIPPED.tsv`): 15 deferred to the next wave (shared source), 18 with a caller edit not accepted, 8 over the caller cap,
  2 table, and 1 other caller case.

**This batch** (23 rows, 11 table replaces, no misses, no new multi-target symbol):
- `logs/recheck.log`: 23 MATCH / RELOCATION_MATCH PASS, and every self grades its PDB symbol.
- The unit 0x004166E2 / 0x00417001 was held here, by the unit4 claim line itself: its note named the address. `land_scripted.py` matched any
  claim line not marked done / released, not only the newest line for an item as COORDINATION.md section 2 says. The tool is fixed
  (commit e7aaf1b456), and the unit landed from the same run as `batches/2026-11-24_landing_unit4b` (2 rows, 1 table replace, 2 PASS). The unit
  had been held in every wave since unit1, first by RP1's unlanded hand-back (`batches/2026-11-24_landing_rp1`).

Tracker after unit4 + unit4b: agree_pdb 23,951.
