# Landing access 2 (Account A, 2026-11-23): 188 rows respelled onto the PDB access / const by script (route 1 widened)

Script: `tools/decomp_pipeline/linux_lane/lane_tools/access_fix.py --wide` (route 1 of `docs/journal/2026-11/2026-11-23_naming_finish_plan.md`).
The edit and the per-row proof are the same as access 1 (`batches/2026-11-23_landing_access1/README.md`).

- **Pool.** Any landed row whose trusted measured self symbol (`self_symbols.tsv`, MATCH / RELOCATION_MATCH) is not a PDB name and differs
  from exactly one PDB symbol of the same `?name@scope@@` only in access / const: 497 candidates.
  - **Excluded at selection:** 14,847 rows with no single near PDB symbol, 193 rows with a table binder on the old spelling (units), 32
    virtual rows the RTTI placement does not confirm, and 8 non-virtual rows missing from one PDB name set.
- **Identity.**
  - The script never changes a row's class or member name, only its access letter or `const`.
  - Virtual targets need the retail RTTI vftable placement (`labels_rtti_port.tsv`) to name exactly the PDB symbol at the address.
  - Non-virtual targets need the PDB symbol in both the Ego_r and the FableWin name sets.
- **Run `accw2`.** 243 generated, 205 accepted (84%), 38 rejected (28 SRC_FAIL, 8 `.text` relocation differences, 2 LINK_FAIL), 254 skipped
  (`RESULTS.tsv`).
- **Held: 17 accepted rows** whose addresses are in T3_PORT_QUEUE.tsv's address or unit columns (`rows_held.txt`). No live lock or open
  claim names any of them.
- **Table.** No change.

## Recheck
`logs/recheck.log` (landed tree, 188 rows): 188 MATCH / RELOCATION_MATCH PASS; the graded symbol equals the PDB symbol in all 188.
