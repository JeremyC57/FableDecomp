# Landing access 1 (Account A, 2026-11-23): 170 T2 rows respelled onto the PDB access / const by script

Script: `tools/decomp_pipeline/linux_lane/lane_tools/access_fix.py`. This is route 1 of `docs/journal/2026-11/2026-11-23_naming_finish_plan.md`.

- **Scope.** A landed row whose own compiled symbol (the measurement pass) differs from the PDB symbol only in the member's access letter
  and / or `const`: tracker tier T2, fix `access`. Callers already bind the PDB symbol in `reloc_bindings.tsv` (all 179 accepted rows have a
  binder on the PDB spelling), so before this landing those references were left unresolved by the row's near-miss spelling.
- **Edit.** In the row's own local class model only:
  - the declaration statement is wrapped as `<pdb access>:` ... `<previous access>:`, so no member moves and no vtable slot changes;
  - `const` is added or removed on the declaration and on the out-of-line definition;
  - the `// Leaf:` becomes the PDB symbol.

  Virtuality never changes: the 16 rows where it would (Q->U, Q->M) are skipped.
- **Proof per row.** The graded symbol is exactly the PDB symbol. The HEAD and new objects are equal section by section under the one rename
  (objsec ALL_EQ, `.drectve` included). The grade is MATCH / RELOCATION_MATCH with the test passing.
- **Run `acc2`.** 241 rows; 203 generated, 179 accepted (88%), 24 rejected (17 SRC_FAIL, 5 `.text` relocation differences, 2 LINK_FAIL),
  38 skipped (`RESULTS.tsv`).
- **Held (9, `rows_held.txt`).** The addresses are in T3_PORT_QUEUE.tsv's address or unit columns, which reserves them (NF316 Q3). This
  covers the 3 rows that still have a binder on the OLD spelling: 0x00B80AD0 (witness 0x00B80FE0), 0x006D5260 (0x006D5850) and 0x0055FB80
  (0x00561450). Those move as units with their binders.
- **Reservation check.** 13 lane lock directories marked `released` and 7 finished Account A claim lines merely mentioning these addresses
  are not reservations. No live lock was found.
- **Table.** No change. The PDB spelling lines already exist, and no landed row's binder used the old spelling.

## Recheck
`logs/recheck.log` (landed tree, 170 rows): 170 MATCH / RELOCATION_MATCH PASS; the graded symbol equals the PDB symbol in all 170.
