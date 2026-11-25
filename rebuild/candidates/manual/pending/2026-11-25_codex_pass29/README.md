# Codex pass 29 (2026-11-25): 200 parked rows, including AUTH26's

**Owner's requests (2026-11-25):**
- "dont launch new author, create 200 row reservation for codex and make handoff with all tools and files it will need to properly name and use headers for those rows. it should give 200 parked rows to codex"
- "codex already has pdbs, but any tables of the pdbs would be good to include"
- "auth 26 can be released so codex can take its rows and continue the ones that arent matched"

## The rows
`rows.tsv` and `RESERVED.tsv` hold 200 rows, 199,236 B in all, each 69 to 1,769 B.

**How they were picked:** `tools/select_rows.py --order closest --include INCLUDE_AUTH26.tsv --n 200 --locks <nflocks>`.
- **AUTH26's 8 rows go first.** AUTH26 is released and its locks were freed.
  - 0x00723E50 was set aside by the owner at g18 (2 instructions off). Its draft on record is now `pending/2026-11-25_auth26/00723e50/g18.cpp`, and its auth26 notes ride in the package.
  - 0x00723E50 and 0x009F3DA0 were in Codex pass 26 and not matched there; `--include` gives them back.
- **The other 192 follow the pass-27 rule, closest first:** status `parked`, a DIFFER grade and a .cpp draft on record, no Codex pass, not landed, claimed or locked.
- **Exclusions:** 360 parked rows have no DIFFER grade on record. 0x00970DF0's record says retail length 0 and is excluded (new selector rule).
- **Counts:** 218 rows were eligible. 148 rows have Ego_d records, 182 have Ego_r / FableWin procedure records, and 11 carry a NAME CONFLICT flag.

## Build
The zip lives in scratch, never in git. It holds no PDB, exe or retail bytes.
1. `tools/prep_evidence.py`, with EGOD_PDB, EGOD_EXTRACT, REL_EXTRACT, EGOR_SYMBOLS and FABLEWIN_SYMBOLS set.
2. `build.py`, with EGOD_EXTRACT, REL_EXTRACT, PDBX, FABLE_EXE and EGOD_PDB_LINK set.

## What is new against pass 28
- **More PDB tables in `names/`:**
  - Ego_r / FableWin publics dumps;
  - publics without procedure records;
  - Ego_r signatures, source files and source map;
  - FableWin editor symbols;
  - the CGameScriptInterface slot table;
  - Ego_d names.
- **The project's own name state in `names/`:** `self_symbols.tsv`, `data_vas.tsv`, the tracker's OPEN.tsv / SUMMARY.md / SHARED_PDB.tsv, and T3_PORT_QUEUE.tsv.
- **Reference tools in `tools/reference/`:** the coordinator's name gate (`name_gate.py`) and the code it uses (naming_status, reloc_identity, purity, build_data_vas, check_portable_spellings), plus the vftable slot checker `vtw425.py`.
- **FableWin helpers:** `tools/fwdis.py` and `tools/fwsym.py`, generalized from AUTH26's, which name a FableWin /Od body's callees in exact PDB spelling.
- **Docs:** `NAMING_AND_HEADERS.md` section 9 (the name gate as a hand checklist; the lessons from nf424-nf426 and AUTH26) and README_CODEX "What changed since pass 28".
- **Lessons:** the AUTH26 landing READMEs, BRIEF and HISTORY, and the baked_address audit README.
