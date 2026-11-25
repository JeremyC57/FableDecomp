# NF415 hand-back (2026-11-25): wide empty string, bare-integer strings, data addresses, detector pass 2

13 rows, all ACCEPT, landed as `2026-11-24_landing_nf415_b1..b4` (RELOCATION_MATCH PASS, 0 LITERAL_DIFF).
- b1 004fdab0 + 004a05c0: file-static `s_Empty` / `s_HeroStateExt` arrays -> `L""` / `L".hs"`; table lines `_s_Empty`, `_s_HeroStateExt` deleted.
- b2 six rows passed a string VA as a bare integer -> spelt literal (cast to the stand-in's existing `int`; the cast goes with the T3 ports).
- b3 00a66550 baked float-pool read -> `return 0.0f;` (`__real@00000000`).
- b4 four baked .data / IAT VAs -> the table-bound data symbol / `__imp__` import.

Rulings: q2 (MATCH -> RELOCATION_MATCH on fixing a baked immediate) accepted as advisor decision (GOTCHAS); the 4 self lines updated.
q1 (the 27 "data" casts are 38 rows of baked vftable stores): reported to the owner with a recommendation; no grade created; rows queued as class ports (T3 `nf415 vtable port`).

Coordinator applied NF415's purity patch (second pass keyed on rebuild/oracles/string_vas.tsv, 563 verified VAs), changed to raise
if the list is missing. Controls: 6 hits on b2's HEAD rows, 0 over rebuild/src/compiled after landing.
Still open: 88 reloc_bindings lines bind a data symbol to a string VA (table_string_symbols.tsv; 69 real) -- the `_s_Empty` form, invisible to a hex scan.
