# Pass 30 amendment: rule 7, strong definitions of other functions (2026-11-27)

Replaces rule 7 in README_CODEX.md ("Strong same-TU copies of other functions are kept"). Advisor decision (owner delegated), recorded
in docs/EXTRA_RULES.md, 2026-11-27.

7. **No strong definition of a function another row owns.** Every function your row's object defines, other than the leaf, is either
   inline (a COMDAT: a class-body member or an `inline` function in a shared header) or not defined at all.
   - A function at another retail address is declared from its shared header (`include/engine/<Class>.h`) and called, never defined in
     your row source. In object mode two strong definitions of one symbol are LNK2005.
   - If your row only matches with the callee's body in the TU (it was inlined into the leaf), that body belongs in the shared header as
     an inline member behind an add-only opt-in, not in the row. Say so in STATUS.tsv with the callee's retail address.
   - If the test needs the callee to link, define it in the test file, not the row.
   - Check: `dumpbin /symbols <row>.obj`. Every `External` symbol in a section other than the leaf's must be a COMDAT
     (selection `pick any` / `same size`); report any other in STATUS.tsv with the owner's retail address.
   - Rows already returned that break this are fixed by us; you do not need to revisit them.
