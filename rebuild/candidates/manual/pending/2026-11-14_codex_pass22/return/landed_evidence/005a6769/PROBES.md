# 005A6769 probes

All attempts preserved in history/005a6769. RTM13.10.3077, /O2 /Oy unless scoped source pragmas say otherwise. Microprobes share their source family; infrastructure failures are not source attempts.

| Attempt | Result | Bytes | Strict ratio |
|---|---|---|---|
| baseline_package | DIFFER | 1336 | 0.9930394431554525 |
| sol00_merged12_baseline | COMPILE_FAIL | - | - |
| sol00b_merged12_baseline | DIFFER | 1336 | 0.9930394431554525 |
| sol01_real_player_chain | COMPILE_FAIL | - | - |
| sol01b_real_player_chain | DIFFER | 1336 | 0.9930394431554525 |
| sol02_original_buy_lookup | DIFFER | 1336 | 0.9930394431554525 |
| sol03_original_component_helper_chain | DIFFER | 1336 | 0.9930394431554525 |
| sol04_original_locals_and_discarded_cast | DIFFER | 1336 | 0.9930394431554525 |
| sol05_visible_building_getter | DIFFER | 1336 | 0.9930394431554525 |
| sol06_visible_real_type_helper | COMPILE_FAIL | - | - |
| sol06b_visible_real_type_helper | DIFFER | 1336 | 0.9930394431554525 |
| sol07_buy_eligibility_guard_region | DIFFER | 1336 | 0.9930394431554525 |
| sol08_wanted_type_branch_region | DIFFER | 1340 | 0.9721577726218097 |
| sol09_wanted_index_region | DIFFER | 1353 | 0.967741935483871 |
| sol10_sell_item_branch_region | DIFFER | 1339 | 0.9675174013921114 |
| sol11_buy_while_region | DIFFER | 1336 | 0.9930394431554525 |
| sol12_sell_while_region | DIFFER | 1336 | 0.9930394431554525 |
| sol13_named_wanted_counter_references | DIFFER | 1336 | 0.9953596287703016 |
| sol13_ref_0 | DIFFER | 1336 | 0.9953596287703016 |
| sol13_ref_01 | DIFFER | 1336 | 0.9953596287703016 |
| sol13_ref_02 | MASKED_MATCH_PENDING_AUDIT | 1336 | 1.0 |
| sol13_ref_1 | DIFFER | 1336 | 0.9930394431554525 |
| sol13_ref_2 | DIFFER | 1336 | 0.9953596287703016 |
| sol14_original_direct_inventory_type_arg | DIFFER | 1336 | 0.9930394431554525 |
| sol15_named_buy_predicate_region | DIFFER | 1336 | 0.9930394431554525 |
| sol16_named_sell_predicate_region | DIFFER | 1336 | 0.9930394431554525 |
| sol17_target_flags_shared_helper_profiles | MASKED_MATCH_PENDING_AUDIT | 1336 | 1.0 |
| sol18_explicit_shared_map_instantiation | MASKED_MATCH_PENDING_AUDIT | 1336 | 1.0 |
| sol19_original_map_external_specializations | MASKED_MATCH_PENDING_AUDIT | 1336 | 1.0 |
| sol20_final_evidence_metadata | MASKED_MATCH_PENDING_AUDIT | 1336 | 1.0 |

Source families explored: actual player-class/header rebase; full discarded shop lookup; original GetTC/Find/cast chain; original local scopes and discarded list cast; visible genuine GetBuildingThing; visible genuine GetTypeFromDefIndexPC; buy guard region; wanted type-branch region; indexed wanted traversal; sell type-branch region; while traversals; category-counter references; direct inventory-type argument; named predicate regions. The two while variants are one loop-shape family; the reference sweep and named predicate variants are diagnostics within their respective families, not independent strategies.

Baseline was1336 with three wrong sum operands. Counter references first reduced that to two; the clothes/items pair gave exact1336. Subsequent closure restored independently matching60-byte helper profiles, and final metadata regrade remains exact. No park or incomplete-residue claim is made.

Main fixture review: retained worker receipt; supplied nonnull empty vector backing, changed map keys from invalid negative enum values to0..101 (still3264pairs), and limited category enum values to representable0..3. Target/helper bodies unchanged. Fresh required-compiler tests follow.
