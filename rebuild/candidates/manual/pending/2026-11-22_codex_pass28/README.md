# Codex pass 28 (2026-11-22): the 25 longest parked rows

Owner: "get next 25 longest parked ready for codex handoff plus updated naming and headers".
- Rows: `tools/select_rows.py --order longest --n 25` (the pass-27 rule, retail length descending): UNMATCHED status `parked`, a DIFFER
  grade and a .cpp draft on record (a `.test.cpp` record maps to its sibling source), no Codex pass has had it, not landed, not claimed,
  not locked. 25 rows, 1,776-3,415 B (53,875 B); 241 eligible. Four drafts compile to stubs (0x00647AB4, 0x00A87960, 0x008E8060,
  0x00BE6880). 366 longer-or-shorter parked rows have no DIFFER grade on record and were not eligible.
- Build: `prep_evidence.py` (EGOD_PDB, EGOD_EXTRACT, REL_EXTRACT, EGOR_SYMBOLS, FABLEWIN_SYMBOLS), then `build.py` (EGOD_EXTRACT,
  REL_EXTRACT, PDBX, FABLE_EXE, EGOD_PDB_LINK). The zip is in scratch, never in git: no PDB, exe or retail bytes inside.
- Headers: today's `rebuild/include` (through the p27, nf235 and auth24 landings). Naming / headers guide: `NAMING_AND_HEADERS.md`
  with the new section 8 (pass 27 landing lessons). Lessons: the p27 landing README, NEXT_CODEX_QUESTIONS, today's auth / nf READMEs.
- Evidence: Ego_d records for 16 rows, Ego_r / FableWin records for 24; header overlays (acctC hdrdiff) for 0x00879900 and 0x00A2F740.
