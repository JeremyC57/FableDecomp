# Landing unit 1 (Account A, 2026-11-24): 74 rows moved onto their PDB symbol together with every landed caller (181 sources)

Script: `tools/decomp_pipeline/linux_lane/lane_tools/unit_fix.py`, landed with `land_scripted.py` (unit-atomic).

**What the route does.** Each unit is one row whose own symbol misses its PDB symbol only by access / const / virtual / return. The
access_fix.py edits (`--wide --virtual --ret`) fix it. Landed callers still referenced the old spelling, which is why access 1-4 skipped
these rows. The row and every landed caller of the old spelling move together:
- **Callers.** Every landed source that names the member's identifier is compiled at HEAD (cap 60). The callers are the rows whose object
  contains the old spelling.
- **Caller edit.** Each caller's local-model declaration is edited with `access_fix.py --pairs`.

**Proof per row.**
- The self: the graded symbol is the PDB symbol, objsec ALL_EQ under old -> PDB, MATCH / RELOCATION_MATCH with the test passing.
- Each caller: objsec ALL_EQ under the one rename old -> PDB, its own graded symbol unchanged, grade passing.
- HEAD and new compiled at one path.
- Identity as access 3: a virtual target needs the RTTI vftable placement; a non-virtual one needs the symbol in both PDB name sets.

**Units.** A unit is accepted only when its self and every caller are accepted, and it lands whole or not at all (`UNITS.tsv`).
- Table: one `replace old -> PDB` per unit (`TABLE_CHANGES.tsv`). `apply_table.py`: 74 replaces, 0 misses, no new multi-target symbol.

**Run `unit1`.**
- 230 unit rows; 195 self edits accepted; 187 within the caller cap.
- 75 units accepted: 183 sources. One unit, 0x004166E2 with its caller 0x00417001, is held as a unit (an open claim matched).
- Skipped (`SKIPPED.tsv`):
  - 83 shared a caller with another unit. unit_fix.py now takes these in waves.
  - 11 had a caller that is another unit's self.
  - 12 had a caller reserved in T3_PORT_QUEUE.tsv.
  - 8 were over the caller cap.
  - 4 had a caller edit not accepted.
  - 2 were other cases.

## Recheck
`logs/recheck.log` (landed tree, 181 rows): 181 MATCH / RELOCATION_MATCH PASS; every self grades its PDB symbol (`land_scripted.py`).
