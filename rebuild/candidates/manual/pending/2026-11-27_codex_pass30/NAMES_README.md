# names/ (Codex pass 30)

Rules for using these tables: `docs/NAMING_FIRST_TIME.md`.

- **answers/ (spell these):**
  - `NAME_ANSWERS.tsv`: one line per retail address the landed tree names. It gives the verdict (PROVEN, PROVEN_RTTI, PROVEN_LANDED_WRONG, REGISTERED_GUESS, FOLDED, UNSETTLED), `use_this_spelling`, its basis, the landed rows that spell it wrong, hint spellings for unsettled addresses, and lock owners. Built from the naming tracker by `tools/name_answers.py` at the commit named in README_CODEX.md.
  - `descriptive_names.tsv`: the registered best guesses, each with its basis.
  - `ESTABLISHED_SPELLINGS.tsv`: data spellings proven by hand.
- **pdb/ (evidence):**
  - mangled names from the three PDBs (`egor_pdb_names`, `fablewin_pdb_names`, `egod_pdb_names` / `egod_carved_names`);
  - per-object function lists, the full publics dumps (`*.publics.txt`) and publics without procedure records;
  - Ego_r signatures, source files and source map;
  - class layouts (`struct_layouts_egor.tsv`), class keys and `fable_types.h`;
  - retail RTTI vftable slots (`rtti-vtables.tsv`) and the CGameScriptInterface slot table;
  - the FSE manifest and library / vendor signatures.
- **state/ (the tree today):**
  - `reloc_bindings.tsv` (callers' spellings per address) and `self_symbols.tsv` (landed selves), which include stand-ins: check them against answers/;
  - `data_vas.tsv`;
  - the tracker's `OPEN.tsv` / `SHARED_PDB.tsv` / `SUMMARY.md`;
  - `T3_PORT_QUEUE.tsv` and `NAME_FIXES_QUEUE.tsv` (held and queued);
  - `LOCKS.tsv`: rows another lane holds. Do not edit them; report them in NAME_CONFLICTS.tsv.
- **hints/ (NOT evidence):** catalog and manifest labels, Ghidra label files, landed source paths. Many are placeholders (`vfunc_N`, `_global::Class_Method`, `Unproven`, address suffixes).
