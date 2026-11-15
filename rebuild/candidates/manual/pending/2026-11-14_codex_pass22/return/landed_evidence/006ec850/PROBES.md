# 006ec850 probes

The supplied source was regraded before edits. No row was parked, so the ten-attempt parking floor does not apply. Every attempt is immutable under history/006ec850.

| Attempt | Result | Finding |
|---|---|---|
| sol00_supplied_baseline | COMPILE_FAIL | Historical WHEELINIT/UNSHEATHING/tree-erase opt-ins are absent from current headers. Excluded build repair. |
| sol01_shared_port_visible_map | COMPILE_FAIL | Missing map::erase opt-in. Excluded build repair. |
| sol02_shared_port_erase | MATCH, 1606 bytes | New declarations/accessor and existing real insert_unique body remove all frame differences. |
| sol03_prune_inner_insert | MATCH, 1606 bytes | The deeper _M_insert body is unnecessary; remove its opt-in. |
| sol04_pruned_metadata | COMPILE_FAIL | Removing JACK_STAGE1_UPDATE also removed the old GetCombatTypeDef declaration. Excluded dependency repair. |
| sol05_precise_combat_accessor | MATCH, 1606 bytes | Existing GETCOMBATTYPEDEF opt-in replaces the unrelated JACK opt-in. |
| sol06_all_union | MATCH, 1606 bytes | Final tree includes both accepted profile rows' headers; all three rows still match. |

The decisive lever is visible real STLport insert_unique, selected by FABLE_RETAIL_STLP_MAP. Its called out-of-line copy matches its own 180-byte retail extent. The old declaration-only copy exposed each pair result, preventing stack-slot sharing with the PeekTC key. Braces and named results could not fix that escape fact.

The final object also matches const LowerBound (60), _M_erase (51), __copy (97), nonconst GetRing (59), and iterator operator-- (79). _M_insert and vector<CProng>::operator= are declarations only in the final source. A diagnostic with the real _M_insert body also matched its own 191-byte extent, but that unnecessary opt-in was removed.

All 40 relocations of the row and emitted helper bodies were reviewed. Existing exact bindings agree; unbound nested/template identities follow both PDBs or real STLport source plus typed callers. No globals were invented.

The complete Init body is copied verbatim into a C++03 fixture. Ten executions cover empty input, direct melee mode, carrying fallback, dead/non-melee/missing weapons, ranged-mode precedence, outer-to-inner moves, zero inner rings, creation/reset/trimming of rings, and both categories of unplaced attackers. GCC64 PASS; VC71 /Od /W3 without /EHsc PASS. Fixture ring-placement callees expose assignment order and mutate the temporary maps; they are not claimed to reproduce the external engine's entire behavior.
