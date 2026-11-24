# Landing flatten pilot 1 (Account A, 2026-11-23): 79 flattened `Class_Method` rows back onto their PDB members, by script

Route 2 of `docs/journal/2026-11/2026-11-23_naming_remainder.md`. Generated and verified by
`tools/decomp_pipeline/linux_lane/lane_tools/flatten_pilot.py` (v3), reviewed by the coordinator; no agent.

- **Shape.** A landed row that defines `R __fastcall Class_Method(const Class* p)` (or `(Class* p)`, `(void* p)`, `()`) for the PDB member
  `?Method@Class@@<access>XZ` (no parameters) becomes `R Class::Method() [const]` with the body kept word for word behind
  `[const] Class* p = this;`.
  - The member declaration goes into the row's own local `class` / `struct`, or a one-member local declaration where the class has none
    and no shared header exists. Forward-declared or undeclared classes that have an `engine/<Class>.h` are skipped: the shared-header rule
    needs an add-only opt-in there.
  - A virtual PDB member turns a leading `void* vtbl` member into the implicit vptr, or shrinks a leading `char pad[N]` by 4; otherwise the
    row is skipped.
  - The return type is taken from the PDB symbol when it is a simple type. The row gets a full `// Leaf:` line.
- **Accepted only when, compiled by vland on this lane:**
  - the new object's graded symbol is exactly the PDB symbol;
  - objsec HEAD v new is ALL_EQ under the one rename;
  - the grade is MATCH / RELOCATION_MATCH with the test passing;
  - no other landed source or test names the flattened function.
- **Counts.** Of 987 candidates (gate + measurement-pass symbols): 103 generated, 89 accepted, 14 rejected.
  - Skips: 498 need a header opt-in, 271 are named by other landed files (binder units), 75 have other shapes, 40 are virtual with no vptr
    slot.
- **Held at review (10, `rows_dropped.txt`, queued as T3 identity research).**
  - 5 PDB names are already claimed by another address in the catalog or by a landed self: ForceClose 0x00408360 v 0x00438440, FreeBuffer
    0x0040F470, IsBetting 0x005E9C30, GetAnimationSpeedValue 0x006625C0 v 0x005DA850, ClearPrimitiveHandles 0x00A46D90.
  - 5 are defined or named by other landed sources and need a look.
  - The pilot now rejects the identity case itself (the catalog / landed-self guard).
- **Review fix.** 73 of the 79 rows began with code, so v3 never wrote their Leaf line; the coordinator prepended it (the pilot now does)
  and rechecked.
- **Table.** No lines: none of the rows had a table binder.

## Recheck
`logs/recheck.log` (landed tree, the 79 rows with their Leaf lines): WINS 79, 0 DIFFER, 0 RELOC_CONFLICT. `RESULTS.tsv` lists every verdict.
