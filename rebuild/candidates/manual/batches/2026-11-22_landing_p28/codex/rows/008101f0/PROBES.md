# Probe history

The chronological records below distinguish source changes, compile failures, and repeat checks. The instruction count is a diagnostic alignment. A zero masked-byte residue and an independent operand audit determine a match.

| Probe | Outcome | Compiled bytes | Aligned instructions | Experiment |
|---|---|---:|---:|---|
| baseline | DIFFER | 1608 | 197 | Compile the supplied draft on the unmodified package headers. |
| engine_vector | COMPILE_FAIL |  |  | engine vector |
| shared_geometry | DIFFER | 1554 | 194 | shared geometry |
| trail_definition | DIFFER | 1554 | 194 | trail definition |
| trail_horizontal_direction | DIFFER | 1554 | 195 | trail horizontal direction |
| trail_target_prediction | DIFFER | 1830 | 233 | trail target prediction |
| trail_lateral_deviation | DIFFER | 1929 | 205 | trail lateral deviation |
| trail_height_compare | DIFFER | 1890 | 215 | trail height compare |
| trail_signed_turns | COMPILE_FAIL |  |  | trail signed turns |
| trail_cross_turn | COMPILE_FAIL |  |  | trail cross turn |
| trail_cross_turn_abi | DIFFER | 1927 | 239 | trail cross turn abi |
| trail_map_cell | DIFFER | 1989 | 231 | trail map cell |
| trail_shared | COMPILE_FAIL |  |  | trail shared |
| trail_shared_iterator | COMPILE_FAIL |  |  | trail shared iterator |
| trail_whole_headers | COMPILE_FAIL |  |  | trail whole headers |
| trail_donor_lifetimes | COMPILE_FAIL |  |  | trail donor lifetimes |
| trail_native_vectors | COMPILE_FAIL |  |  | trail native vectors |
| trail_whole_headers_hierarchy | COMPILE_FAIL |  |  | trail whole headers hierarchy |
| trail_donor_lifetimes_hierarchy | COMPILE_FAIL |  |  | trail donor lifetimes hierarchy |
| trail_native_vectors_hierarchy | COMPILE_FAIL |  |  | trail native vectors hierarchy |
| trail_whole_headers_rtti | COMPILE_FAIL |  |  | trail whole headers rtti |
| trail_donor_lifetimes_rtti | COMPILE_FAIL |  |  | trail donor lifetimes rtti |
| trail_native_vectors_rtti | COMPILE_FAIL |  |  | trail native vectors rtti |
| trail_whole_headers_original_layout | COMPILE_FAIL |  |  | trail whole headers original layout (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| trail_donor_lifetimes_original_layout | COMPILE_FAIL |  |  | trail donor lifetimes original layout (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| trail_native_vectors_original_layout | COMPILE_FAIL |  |  | trail native vectors original layout (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| trail_whole_headers_ip | DIFFER | 1966 | 245 | trail whole headers ip |
| trail_donor_lifetimes_ip | DIFFER | 1942 | 313 | trail donor lifetimes ip |
| trail_native_vectors_ip | DIFFER | 1942 | 313 | trail native vectors ip |
| trail_retained_vector_arg | DIFFER | 1918 | 312 | Pass the 3D creation position to the native CWorldMap overload without converting it. |
| trail_pointer_lifetimes | DIFFER | 1918 | 312 | trail pointer lifetimes |
| trail_fps_owner | DIFFER | 1918 | 312 | Use the real CMainGameComponent::GetConstantFPS call at all original sites. |
| handback_d558d19cdc_6210a30 | DIFFER | 1918 | 312 | Independent verification of a selected source. (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |

## Continued main-chat grades

All 1,914 bytes and 555 instructions match. All 74 primary operands, the 12 mapped inline copies and three additional STLport owners passed. The extra owners are make_pair (0080E200, 48 bytes), the converting pair constructor (0080DE90, 47 bytes), and list::push_back (00810D40, 25 bytes). Their nine operands match. The source computes trap_index from the same division as the period test before the game callbacks, as the named donor local does. Expiry is strictly past the bound. The final eligible frame can still spawn. The native test covers the original 24 scenarios and a dedicated lifetime-boundary case. The parentheses around the random product reproduce the final two scheduled operations in the later height-query block.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_traildonor_both | 1929 | 408 | 12 / 2 |
| round2_traildonor_flow | 1929 | 408 | 12 / 2 |
| round2_traildonor_list | 1929 | 408 | 12 / 2 |
| round2_traildonor_rev | COMPILE_FAIL |  |  /  |
| round2_traildonor_revlist | COMPILE_FAIL |  |  /  |
| round2_traildonor_yxz | 1929 | 408 | 12 / 2 |
| round2_trailground_float | COMPILE_FAIL |  |  /  |
| round2_trailground_inline | COMPILE_FAIL |  |  /  |
| round2_trailground_post | COMPILE_FAIL |  |  /  |
| round2_trailground_pre | COMPILE_FAIL |  |  /  |
| round2_trailheight_add | 1947 | 523 | 13 / 1 |
| round2_trailheight_checks | 1914 | 553 | 13 / 1 |
| round2_trailheight_componentctor | 1914 | 553 | 13 / 1 |
| round2_trailheight_place | 1914 | 553 | 13 / 1 |
| round2_trailheight_tanh | 1914 | 552 | 13 / 1 |
| round2_trailheight_vectorcopy | 1914 | 553 | 13 / 1 |
| round2_trailinputs_double | 1914 | 552 | 12 / 0 |
| round2_trailinputs_floatzero | 1914 | 552 | 12 / 0 |
| round2_trailinputs_ints | 1914 | 552 | 12 / 0 |
| round2_trailinputs_namedrng | 1914 | 552 | 12 / 0 |
| round2_trailinputs_normal | 1914 | 552 | 12 / 0 |
| round2_trailinputs_readonly | 1914 | 552 | 12 / 0 |
| round2_trailinputs_visiblerng | 1914 | 552 | 13 / 0 |
| round2_trailinputs_zcast | 1914 | 552 | 12 / 0 |
| round2_trailmath_addnamed | 1947 | 524 | 14 / 0 |
| round2_trailmath_addxy | 1914 | 534 | 14 / 0 |
| round2_trailmath_arc | 1914 | 552 | 14 / 0 |
| round2_trailmath_both | 1914 | 552 | 14 / 0 |
| round2_trailmath_const | 1914 | 553 | 14 / 0 |
| round2_trailmath_owner | 1914 | 553 | 14 / 0 |
| round2_trailmath_paren | 1914 | 552 | 14 / 0 |
| round2_trailpairs_copy0 | 1914 | 553 | 14 / 0 |
| round2_trailpairs_copy1 | 1914 | 553 | 13 / 1 |
| round2_trailpairs_copylate | 1914 | 553 | 14 / 0 |
| round2_trailpairs_copyref | 1914 | 553 | 14 / 0 |
| round2_trailpairs_namedadd | 1914 | 553 | 14 / 0 |
| round2_trailpairs_parens | 1914 | 553 | 14 / 0 |
| round2_trailsched_division_group | 1914 | 555 | 12 / 0 |
| round2_trailsched_float_cast | 1914 | 552 | 12 / 0 |
| round2_trailsched_height_group | 1914 | 552 | 12 / 0 |
| round2_trailsched_random_double | 1914 | 552 | 12 / 0 |
| round2_trailsched_random_doublecast | 1914 | 553 | 12 / 0 |
| round2_trailsched_random_product | 1914 | 555 | 12 / 0 |
| round2_trailsched_sum_group | 1914 | 552 | 12 / 0 |
| round2_trailsched_tanh_cast | 1914 | 555 | 12 / 0 |
| round2_trailsched_tanh_paren | 1914 | 555 | 12 / 0 |
| round2_trailsched_threshold | 1914 | 552 | 12 / 0 |
| round2_trailshape_depth0 | 1925 | 445 | 13 / 1 |
| round2_trailshape_depth1 | 1947 | 350 | 13 / 1 |
| round2_trailshape_depth2 | 1947 | 350 | 13 / 1 |
| round2_trailshape_list_split | 1925 | 445 | 13 / 1 |
| round2_trailshape_no_reverse | 1945 | 342 | 13 / 1 |
| round2_trailshape_original | 1947 | 350 | 13 / 1 |
| round2_trailtemps_copy | 1914 | 552 | 12 / 0 |
| round2_trailtemps_copybase | 1914 | 547 | 12 / 0 |
| round2_trailtemps_explicitcast | 1914 | 552 | 12 / 0 |
| round2_trailtemps_named | 1914 | 552 | 12 / 0 |
| round2_trailtemps_namedup | 1914 | 552 | 12 / 0 |
| round2_trailtemps_ref | 1914 | 552 | 12 / 0 |
| round2_trailtemps_selfcall | 1914 | 552 | 12 / 0 |
| round2_trailtemps_splittemp | 1914 | 552 | 12 / 0 |
| round2_trailtime_double | 1914 | 546 | 13 / 1 |
| round2_trailtime_getdecl | 1925 | 526 | 13 / 0 |
| round2_trailtime_index | 1914 | 549 | 13 / 1 |
| round2_trailtime_named | 1925 | 471 | 13 / 1 |
| round2_trailtime_tanh | 1914 | 547 | 13 / 1 |
| round2_trailvisible_after | 1914 | 552 | 15 / 0 |
| round2_trailvisible_afterfloat | 1914 | 553 | 15 / 0 |
| round2_trailvisible_before | 1914 | 552 | 15 / 0 |
| round2_trailvisible_beforefloat | 1914 | 553 | 15 / 0 |
| round2_trailvisible_decl | 1914 | 552 | 12 / 0 |
| round2_trailvisible_declafter | 1914 | 552 | 13 / 0 |
| round2_trailvisible_declfloat | 1914 | 553 | 12 / 0 |
| round2_trailworld_float | 1914 | 553 | 12 / 1 |
| round2_trailworld_inline | 1914 | 552 | 12 / 1 |
| round2_trailworld_post | 1914 | 552 | 12 / 1 |
| round2_trailworld_pre | 1914 | 552 | 12 / 1 |
