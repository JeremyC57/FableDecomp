# 2026-11-28 catalog relabel (catalog text only, no code change)

Owner request: fix the catalog-name / leaf mismatches the pass 30 audit counted (its "about 4,110" was a crude count; see
`pending/2026-11-28_catalog_name_review/README.md` for the corrected picture).

**78 relabels** (`RELABELS.tsv`): `auto-re-candidates.tsv` name, `functions.tsv` name and module, and `build_candidates.ps1`
Module (29 changed) now follow the row's own leaf. Gate (advisor, owner delegated):
- the source `// Leaf:` line, the tracker's `self_leaf` and (for a split) its one PDB spelling are the same full mangled string;
  `agree_pdb` single spelling, or `split_fixable` whose own leaf is the PDB name; `agree_std` only for a `std::` leaf over a
  placeholder catalog name;
- skipped: T3 queue lines and binders, live locks, open claims, every `RESERVED.tsv`, the descriptive registry, SHARED_PDB names,
  operators, cosmetic-only differences, anything that would lose a class or namespace;
- 12 rows where catalog and leaf are two different real names (a folded body may carry both) are saved for review, not applied.

Checks: each new name's last identifier is in its own demangled leaf (78/78); tracker before/after 0 status changes; 41 rows
regraded from the repo (all 29 Module changes + 12 sampled): 31 MATCH/RELOCATION_MATCH PASS under strict headers, the other 10
refused only by the strict shared-header rule on unchanged sources (pre-existing debt) and MATCH/RELOCATION_MATCH PASS without it.
