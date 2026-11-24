# Landing unit6 (Account A, 2026-11-24): 6 rows onto their PDB symbol together with every landed caller

Wave 6 of `unit_fix.py`, landed with `land_scripted.py` (unit-atomic, staleness-checked). The route and proof are as unit 1
(`batches/2026-11-24_landing_unit1/README.md`).

- Run: self accepted 39 of 74. Summary: `{"units": 74, "self_accepted": 39, "accepted_units": 3, "rows": 6, "table_lines": 3, "skipped": {"781": 1, "93": 1, "1700": 2, "87": 1, "363": 1, "77": 1, "138": 1, "callers": 1, "caller": 18, "table": 2, "deferred": 7}}` (`SKIPPED.tsv` lists the skipped units; "deferred" ones share a source and go to the next wave).
- Landed: 6 rows, none held. Table: `table: {'replace': 3} misses 0 | multi-target before 1 after 1 new [] `.
- `logs/recheck.log`: every row MATCH / RELOCATION_MATCH PASS, and every self grades its PDB symbol.
