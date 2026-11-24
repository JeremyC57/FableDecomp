# Includer baseline at HEAD (2026-11-24, before landings nf378 / nf379 / nf380)

Every landed row that compiles another row's file through `#include "../src/compiled/..."` (1,364 rows: EH unwind funclets,
option S thin rows, fold rows; `naming_status.includers_map()`), regraded with `recheck.py` + `vland.py --recheck-landed` on HEAD
4f72f62cb2, so the patched `land_scripted.py` (which regrades a changed owner's includers) is not held by a fault that predates it.

- 1,326 RELOCATION_MATCH PASS.
- 38 DIFFER, listed in `KNOWN_NONMATCH.tsv`: all are funclet rows named `Unwind<addr>` (no `@`) whose recheck payload carries
  no `funclet_offset`, so vland grades the whole parent body against the funclet (e.g. `DIFFER(857v11)`). All 1,318 other funclet
  rows carry an offset and match. This is a payload gap in `recheck.py` for that naming form, not a code fault; until it is
  fixed, a landing that edits the parent of one of these 38 rows is held by land_scripted's includer step and needs a hand check.
- Tool follow-up queued (HANDOFF): derive `funclet_offset` for `Unwind<addr>` rows the way the `Unwind@<addr>` rows get it.
