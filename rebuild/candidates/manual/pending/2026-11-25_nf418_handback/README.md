# NF418 hand-back (2026-11-25): made-up data symbols standing in for retail literals

31 rows landed as `2026-11-25_landing_nf418_s1` (17 string rows) and `_f1` (14 float-pool rows), RELOCATION_MATCH PASS,
only `??_C@` / `__real@` sections new; 9 f1 rows held (their tests are self-contained copies; T3 `nf418 0x00437500 ... tests`).
Table: 33 deletes by the lander; 4 stale deletes (t0) and 3 rewitnesses (`_kPi` / `_kTwoPi` / `_kMinusPi` -> 00c0b4c0) applied by hand.

Coordinator also landed NF418's detector as tools/decomp_pipeline/crawl/baked_symbols.py (source pass `baked_symbol_names`, object
pass `baked_symbols_in_object`; keyed on rebuild/oracles/string_vas.tsv, which now includes NF418's 68 added VAs; raises if the
list is missing) and wired the source pass into tools/landed_metrics.py's `baked_string` grade. Census after this landing: 39 rows
(NF418 predicted 56 at HEAD -> 39). Those 39 rows leave the genuine figures until each spells its literal (owner decision 2026-11-25).
Classification of all 524 .rdata table bindings: CLASSIFICATION.tsv.
