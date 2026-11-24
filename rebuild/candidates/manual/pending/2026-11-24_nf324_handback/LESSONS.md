# NF324 lessons (2026-11-24, flatten v4 over the 1,744 flattened virtual members)

- The v3 candidate filter, not the compile, lost most of the pool: 664 entries have an underscore in the class name (CAIStateGroup_X,
  CCreatureAction_X), 265 PDB members take parameters, 235 selves are `__stdcall` / `__cdecl`. Take Class and Method from the PDB symbol,
  never by splitting the flattened name.
- The second big lever is the switch set: v3 knew 4 class-model switches. "Any `FABLE_RETAIL_*` macro a landed row defines, no `_PASS`"
  (NF318 Q2) moves ~590 entries from "no single class-model switch" into reach. Read `#else` of `#ifndef X` as "needs X" and an `||` chain
  as "needs one landed member of it".
- Most of these rows are tiny constant-return stubs: the bytes never depend on the class model, so the class model (local / switch /
  header) and identity are the whole job. The grade is the cheap part (about 1.4 s per row side on vland).
- Precedent preambles (the smallest landed row that defines the switch) drag in up to 140 unrelated define / include lines. Minimise them
  by clang (drop the extra includes, then each line, keep a removal while clang passes); the class switches themselves stay.
- objsec ALL_EQ does not compare symbol tables: a renamed pick-any COMDAT or a new defined symbol with no relocation to it passes it.
  The v4 objcheck adds DEF (under the one rename) and UND set equality and refuses any new `??_7` / `??_R`. Controls in
  evidence/objcheck_controls.txt.
- Load the whole-tree text indexes (56k files) once: a per-entry regex over every landed file made the first triage run crawl.
- A precedent's `#pragma` lines are never copied: a pragma changes codegen for the row.
- Rows another landing touched while you work (flatten4 landed 5 of these entries mid-run): check the landed Leaf against the PDB symbol
  before generating, and let the grade refuse any row whose landed source changed since generation.
- A gate added late (the override gate, the pad-use guard, the in-class census check) must be rerun over every row already graded: the
  final regeneration (work/fin12) caught 103 false skips from two over-broad gates, and the final triage (tri5) caught one more identity
  hold (0x0045655E) in a batch generated before the in-class check existed.
- `--no-pre` triage cannot see precheck failures, and a zombie `timeout` answers `kill -0`: check a finished job by its output or by
  `ps -o stat`, not by `kill -0` alone.
- Landed sources can carry retail byte-dump comments (`// Retail: <hex bytes> => <asm>`). Drop them in the generated source and keep the
  exact HEAD copy out of hand-back payloads.
- Headers land under you (NF321 / NF322 / NF323 mid-run): keep the header overlay as a full tree, sync it from the repo before each grade
  (tools/treesync.py), and compare orig/ with the repo again just before hand-back.
- The census over landed tests finds thousands of mock bodies (quest-script tests model CGameScriptInterface members). They are harmless,
  because a test links alone or with its own row only (verify_and_land.py `behaviour_of`). Count source hits and test hits separately.
- With `--hdr-tree`, any `_RTTI_DECLS` block in the tree counts as a landed switch, including another agent's block whose definer has
  not landed. Hold rows on such a macro, or flag them, before you report them done.
