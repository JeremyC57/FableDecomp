# Probe history

The chronological records below distinguish source changes, compile failures, and repeat checks. The instruction count is a diagnostic alignment. A zero masked-byte residue and an independent operand audit determine a match.

| Probe | Outcome | Compiled bytes | Aligned instructions | Experiment |
|---|---|---:|---:|---|
| baseline | DIFFER | 2124 | 431 | Compile the supplied draft on the unmodified package headers. |
| bed_component | COMPILE_FAIL |  |  | bed component |
| bed_registry | DIFFER | 1997 | 420 | bed registry |
| bed_direct_user | DIFFER | 2060 | 283 | bed direct user |
| bed_clear_building | DIFFER | 2044 | 329 | bed clear building |
| bed_cached_village | DIFFER | 2028 | 323 | bed cached village |
| bed_parent_getter | COMPILE_FAIL |  |  | bed parent getter |
| bed_shared | COMPILE_FAIL |  |  | bed shared |
| bed_real_calls | COMPILE_FAIL |  |  | bed real calls |
| bed_real_parent | COMPILE_FAIL |  |  | bed real parent |
| bed_live_found | COMPILE_FAIL |  |  | bed live found |
| bed_conditional_price | COMPILE_FAIL |  |  | bed conditional price |
| bed_real_calls_typed | DIFFER | 2224 | 264 | Use shared ownership, subtitle and component lookup declarations with their required opt-ins. |
| bed_real_parent_typed | DIFFER | 2224 | 264 | bed real parent typed |
| bed_live_found_typed | DIFFER | 2366 | 346 | bed live found typed |
| bed_conditional_price_typed | DIFFER | 2367 | 400 | bed conditional price typed |
| bed_debug_guard | DIFFER | 2224 | 264 | bed debug guard |
| bed_outer_scope | DIFFER | 2224 | 264 | bed outer scope |
| bed_owner_assignment | DIFFER | 2216 | 248 | bed owner assignment |
| bed-focused-ip_orig | COMPILE_FAIL |  |  | bed focused -ip orig |
| bed-focused-ip_direct | COMPILE_FAIL |  |  | bed focused -ip direct |
| bed-focused-ip_chain | COMPILE_FAIL |  |  | bed focused -ip chain |
| bed-focused-flat_donor | COMPILE_FAIL |  |  | bed focused -flat donor |
| bed-focused-flat_coalesce | COMPILE_FAIL |  |  | bed focused -flat coalesce |
| bed-focused-flat_coalesce_chain | COMPILE_FAIL |  |  | bed focused -flat coalesce chain |
| bed-focused-flat_common | COMPILE_FAIL |  |  | bed focused -flat common |
| bed-focused2-ip_orig | DIFFER | 2407 | 432 | bed focused 2-ip orig (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| bed-focused2-ip_direct | DIFFER | 2392 | 230 | bed focused 2-ip direct (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| bed-focused2-ip_chain | DIFFER | 2407 | 432 | bed focused 2-ip chain (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| bed-focused2-flat_donor | COMPILE_FAIL |  |  | bed focused 2-flat donor (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| bed-focused2-flat_coalesce | COMPILE_FAIL |  |  | bed focused 2-flat coalesce (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| bed-focused2-flat_coalesce_chain | COMPILE_FAIL |  |  | bed focused 2-flat coalesce chain (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| bed-focused2-flat_common | COMPILE_FAIL |  |  | bed focused 2-flat common (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| bed-focused3-flat_donor | DIFFER | 2258 | 412 | bed focused 3-flat donor |
| bed-focused3-flat_coalesce | DIFFER | 2263 | 437 | bed focused 3-flat coalesce |
| bed-focused3-flat_coalesce_chain | DIFFER | 2263 | 437 | bed focused 3-flat coalesce chain |
| bed-focused3-flat_common | DIFFER | 2120 | 394 | bed focused 3-flat common |
| bed-focused3-flat_subdef | DIFFER | 2195 | 493 | bed focused 3-flat subdef |
| bed-focused3-flat_common_sub | DIFFER | 2055 | 435 | bed focused 3-flat common sub |
| bed-focused3-flat_single_confirm | DIFFER | 1999 | 453 | bed focused 3-flat single confirm |
| bed-focused4-branch_jumps | COMPILE_FAIL |  |  | bed focused 4-branch jumps |
| bed-focused4-flat_subdef_noouter | DIFFER | 2195 | 493 | bed focused 4-flat subdef noouter |
| bed-focused4-flat_subdef_named_ip | DIFFER | 2163 | 522 | bed focused 4-flat subdef named ip |
| bed-focused4-flat_subdef_scope | DIFFER | 2195 | 493 | bed focused 4-flat subdef scope |
| bed-focused5-branch_jumps_ip | DIFFER | 1947 | 417 | bed focused 5-branch jumps ip |
| bed-focused5-loop_reversed_named | DIFFER | 2149 | 594 | bed focused 5-loop reversed named |
| bed-focused5-loop_locals_named | DIFFER | 2145 | 477 | bed focused 5-loop locals named |
| bed-focused5-loop_reversed_jumps | DIFFER | 1933 | 456 | bed focused 5-loop reversed jumps |
| bed-focused5-loop_locals_jumps | DIFFER | 1928 | 301 | bed focused 5-loop locals jumps |
| bed_final_separate_denial_source | MASKED_EQUAL | 2145 | 596 | Write the occupied-bed and not-usable failures in separate natural branches; let the compiler merge their common code. |
| bed_final_ordered_early_exits | MASKED_EQUAL | 2145 | 596 | Use two ordinary early exits with the original ownership precedence. |
| bed_final_two_branch_labels | MASKED_EQUAL | 2145 | 596 | bed final two branch labels |
| bed_final_owned_or_unoccupied | MASKED_EQUAL | 2145 | 596 | bed final owned or unoccupied |
| bed_final_positive_nested | DIFFER | 2091 | 539 | bed final positive nested |
| bed_final_owner_first_then_denial | MASKED_EQUAL | 2145 | 596 | bed final owner first then denial |
| handback_d558d19cdc_eefea70 | MASKED_EQUAL | 2145 | 596 | Independent verification of a selected source. (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
