# NF420 hand-back (2026-11-25): the last data-symbol baked strings and NF418's held float rows

46 rows, all ACCEPT, landed as `2026-11-24_landing_nf420_s2` (37 string rows: the 36 the baked_symbols census found plus 0048f881,
whose file-static `s_TextGuiSkipScene` had no table line) and `_f2` (NF418's 9 held float-pool rows, now with linked tests).
Every use site spells retail's exact literal (LITERALS.tsv); 58 made-up data-symbol table lines deleted; `?g_zero@@3MB` rewitnessed
to 00732d00 (its last real reader) by hand. All 46 tests link the row object; every mutant fails with the test's own assertion.
baked_symbols census: 36 -> 0. Gates: vland PASS -> PASS, 0 LITERAL_DIFF, only `??_C@` / `__real@` sections new, /FORCE link 0 duplicates.
