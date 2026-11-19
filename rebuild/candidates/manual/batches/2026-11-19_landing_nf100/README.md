# 2026-11-19 landing nf100: naming round 9, NF100 (NF96 / NF97 leftovers, 61 rows)

Records: `naming/2026-11-19_agentNF100/` (selfcheck 0 problems; scripts `t/li_a.py`, `t/li_b.py`). 11 entries: 4 done, 7 blocked.

## NThingMap::LayerInfo 0x0137DAD8 (q11463 / q11282)
An exe scan found 77 landed rows binding the level table under stand-ins (29 `?LayerInfo_0137dad8@@3PAUCLayerInfo@@A`, 48
`?g_MapwhoLevels@@3PAUSMapwhoLevel@@A`), not ~35. 56 rows (28 + 28) now bind `?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B`, which 22 rows
already used (an existing split, narrowed). Mechanism: the opt-in `FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY` in engine/NThingMap_CThingMap.h
declares only `NThingMap::CLayerInfo` and `LayerInfo[]`, so rows that still model the rest of NThingMap locally can use the shared
declaration without a new local model (advisor decision, owner delegated: accepted; the includer recheck is the gate). 18
`g_MapwhoLevels` rows are left (`liB_left.txt`, one `t/li_b.py` payload) for the next round.
0x006378E0's bare `__assume(0)` (landed text) is now the guarded EXTRA_RULES form with an `// Inferred:` line (same bytes).

## Other entries
- q11237 done: 0x005C150B onto the headers with a same-TU copy of 0x005BC450 (body verbatim, Fidelity rule); its callers 0x005C176A /
  0x005C1818 moved with its new leaf.
- q11464 done: 0x0095A6C0 onto the DiveBomb header set; 0x0095AD20 respelled in place (its leaf now matches what 0x0095B6F0 binds).
- q11466 / q11467 done as catalog fixes (queued); their landed callers were already right.
- Blocked: q11236 (0x005BDEC1 DIFFER 72v71: the uninitialised `bool dummy_bool` takes its own slot), q11400 (0x0063E950, a dozen local
  classes), q11444 / q11447 / q11448 (0x005BF414 DIFFER 103v110, register choice; its callers 0x005BF4ED / 0x005C30D3 wait).

## Rows (61): 5 leaf renames (RENAMES_applied.tsv, no file moves), MATCH / RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
Headers (additive opt-ins): NInventory_CTCInventoryBase.h `CITEM_ISSAMEITEMTYPEASDEFINDEX`, NThingMap_CThingMap.h `LAYERINFO_ONLY`,
CAIStateGroup_HoverHornetQueen.h `ISROUTETOPOSBLOCKEDBYTARGET`. assert_names_check 0 mismatches.

## Checks
bindings_multi_check 1 (NullCoord baseline); funclet_includes_check 1355 / 0 broken. Includers + every other row the exe scan finds
referencing a respelled target: 186 good / 0 bad (`logs/includer_recheck.txt`); no-new-split gate: 0 created (`logs/split_gate.txt`).
