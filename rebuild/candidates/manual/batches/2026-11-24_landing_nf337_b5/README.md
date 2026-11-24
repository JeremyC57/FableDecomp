# Landing nf337 b5 / b5u / b6 (Account A, 2026-11-24): the two held NF337 batches, after the coordinator's rulings

Context and proof are in `batches/2026-11-24_landing_nf337_b1/README.md`. The agent's evidence is in `pending/2026-11-24_nf337_handback/evidence/b5`, `b6`.

## Rulings (advisor decision, owner delegated)
- **D-1 (b5), accepted under NF321 Q1.** Typing the PDB members as `float` adds exactly `UND __fltused` to four objects; the code, the
  relocations and every other symbol are unchanged. All three conditions hold:
  - the only UND delta is `__fltused`;
  - ten landed sibling operator= objects already carry it;
  - each member has two sources (`evidence/b5_float_sources.tsv`): it is `float` (0x0040) in both the Ego_r and the FableWin type records.
  b5 landed from its own run dir, with only `CLensFlareElementDef.h` and its own TABLE_CHANGES.
- **D-2 (b6), the first build was refused.** It wrote 0x0046F43E's operator= out of line, which changed its COMDAT selection from pick-any
  to no-duplicates. Both PDBs spell the operator= `??4CAbilityButtonGuiDef@@QAEAAV0@ABV0@@Z`, and `engine/CAbilityButtonGuiDef.h` says it is
  implicit; NF328 / NF331 keep implicit special members pick-any. The retry, which landed:
  - keeps the operator= implicit;
  - triggers its emission with the 0x00E95380 anonymous-namespace helper form (`// Fidelity: scaffolding, not retail`), so 0x0046F43E no
    longer defines `?Copy@CAbilityButtonGuiDef`;
  - moves 0x0046F43E onto the shared header.
  Its operator= section equals HEAD's, selection included, and the link of 0x0046F49F, 0x0046F43E and the witness 0x00476672 has 0 duplicates.
- **T3 tags:** the first run held the 0x0046F77C unit and the b6 unit on the nf298 0x0046F6AF line and the nf285 0x0046F43E line. Those lines
  describe exactly the stand-ins these units remove, so b5u (the held unit) and b6 re-ran with `--release-t3 nf324,nf301,nf285,nf298`, and
  both lines are closed (`T3_CLOSED.tsv`).
- **Results:** b5 4 rows + b5u 2 + b6 2, all landed, 0 failed. Includer rechecks: b5 5, b5u 7, b6 6, all PASS.
- **Catalog:** 0x00460589 and 0x0046F6AF are renamed from `_unattributed::operator=` to their classes (catalog, manifest, files), and both
  recheck RELOCATION_MATCH PASS.
