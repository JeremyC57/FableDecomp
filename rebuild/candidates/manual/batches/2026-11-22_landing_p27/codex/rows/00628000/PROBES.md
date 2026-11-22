# CTCQuickAccessMenu::Activate source probes

| Source variant | Bytes | Matching instructions | Pick-any EQ / NEQ |
| --- | --- | --- | --- |
| geometry-park-create-component-local | 1323 / 1315 | 357 / 421 | 0 / 0 |
| geometry-park-branch-sum | 1327 / 1315 | 271 / 421 | 0 / 0 |
| geometry-park-branch-direct | 1336 / 1315 | 292 / 421 | 0 / 0 |
| geometry-park-branch-counted-order | 1323 / 1315 | 337 / 421 | 0 / 0 |
| geometry-park-interface-guard | 1323 / 1315 | 357 / 421 | 0 / 0 |

| Shared source variant | Bytes | Matching instructions | Pick-any EQ / NEQ | Observation |
| --- | --- | --- | --- | --- |
| geometry-activate-shared2 | 1312 / 1315 | 290 / 421 | 5 / 4 | Original shared classes, private list APIs and calls through their actual vtable slots. |
| geometry-activate-default | 1318 / 1315 | 286 / 421 | 5 / 4 | Retail branch proportions, guarded offset initialization and typed inventory maps. |
| geometry-activate-bounded-expression | 1323 / 1315 | 357 / 421 | 5 / 4 | Reversing the final null tests restores many native register choices, but the early this spill and manager register remain different. |
| geometry-activate-donor-dead-abilities | 1318 / 1315 | 286 / 421 | 5 / 4 | Recovered the disabled abilities arm and original Magic localization. The arm emits no retail calls. |
| geometry-activate-total-sum | 1316 / 1315 | 273 / 421 | 5 / 4 | Boolean sum changes assignment and spill order. |

| Additional candidate | Bytes | Matching instructions | Pick-any EQ / NEQ | Observation |
| --- | --- | --- | --- | --- |
| geometry-activate-allocation-v-pushback | 1323 / 1315 | 357 / 421 | 6 / 3 | The shared size-mode vector append restores the native grow owner. |
| geometry-activate-partial-tested | 1323 / 1315 | 357 / 421 | 6 / 3 | Retained shared candidate and synchronized fixture covering all menu-presence branches. |

The geometry-activate-native-sibling trials expose the original item-list body and the reconstructed expression submenu, separately and together, before and after Activate. Both list calls stay out of line. The retained 1323-byte Activate leaf still has the highest normalized instruction coverage.

## Additional VC7.1 probes

The experiment series isolates the donor-order final guard, the count lifetime, and the actual same-TU companion. The selected exact leaf remains bit-identical in the visible-body variants. The pointer-escape experiments are diagnostic controls.

| Source variant | Bytes | Matching instructions | Stock mapped weak EQ / NEQ |
| --- | --- | --- | --- |
| tail_quick_alias_flags_early_486 | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_flags_early_alias | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_flags_early_no_alias | 1312 / 1315 | 269 / 421 | 5 / 4 |
| tail_quick_alias_flags_early_p4 | 1309 / 1315 | 397 / 421 | 4 / 5 |
| tail_quick_alias_flags_early_p6 | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_flags_early_pentium | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_flags_early_size | 1011 / 1315 | 157 / 421 | 5 / 4 |
| tail_quick_alias_flags_late_486 | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_flags_late_alias | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_flags_late_no_alias | 1314 / 1315 | 269 / 421 | 5 / 4 |
| tail_quick_alias_flags_late_p4 | 1311 / 1315 | 396 / 421 | 4 / 5 |
| tail_quick_alias_flags_late_p6 | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_flags_late_pentium | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_flags_late_size | 1009 / 1315 | 160 / 421 | 5 / 4 |
| tail_quick_alias_flags_plain_486 | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_alias_flags_plain_alias | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_alias_flags_plain_no_alias | 1312 / 1315 | 278 / 421 | 5 / 4 |
| tail_quick_alias_flags_plain_p4 | 1307 / 1315 | 369 / 421 | 4 / 5 |
| tail_quick_alias_flags_plain_p6 | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_alias_flags_plain_pentium | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_alias_flags_plain_size | 1005 / 1315 | 160 / 421 | 5 / 4 |
| tail_quick_alias_gate_early_has_descending | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_gate_early_manual_unsigned | 1322 / 1315 | 290 / 421 | 6 / 3 |
| tail_quick_alias_gate_early_negative_or_zero | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_gate_early_sign_min | 1326 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_gate_early_two_conditions | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_gate_late_has_descending | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_gate_late_manual_unsigned | 1324 / 1315 | 276 / 421 | 6 / 3 |
| tail_quick_alias_gate_late_negative_or_zero | 1319 / 1315 | 411 / 421 | 6 / 3 |
| tail_quick_alias_gate_late_sign_min | 1330 / 1315 | 384 / 421 | 6 / 3 |
| tail_quick_alias_gate_late_two_conditions | 1319 / 1315 | 411 / 421 | 6 / 3 |
| tail_quick_alias_sibling_early_both_after | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_early_both_before | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_early_expressions_after | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_early_expressions_before | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_early_items_after | 1315 / 1315 | 413 / 421 | 6 / 4 |
| tail_quick_alias_sibling_early_items_before | 1315 / 1315 | 413 / 421 | 6 / 4 |
| tail_quick_alias_sibling_late_both_after | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_late_both_before | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_late_expressions_after | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_late_expressions_before | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_late_items_after | 1317 / 1315 | 412 / 421 | 6 / 4 |
| tail_quick_alias_sibling_late_items_before | 1317 / 1315 | 412 / 421 | 6 / 4 |
| tail_quick_alias_sibling_plain_both_after | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_plain_both_before | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_plain_expressions_after | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_plain_expressions_before | COMPILE_FAIL | — | — |
| tail_quick_alias_sibling_plain_items_after | 1313 / 1315 | 384 / 421 | 6 / 4 |
| tail_quick_alias_sibling_plain_items_before | 1313 / 1315 | 384 / 421 | 6 / 4 |
| tail_quick_alias_values_early_const_cwide_ref | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_values_early_cwide_cast | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_values_early_positivity_record | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_values_early_widen_int | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_values_early_widen_uint | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_alias_values_late_const_cwide_ref | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_values_late_cwide_cast | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_values_late_positivity_record | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_values_late_widen_int | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_alias_values_late_widen_uint | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_combined_owners | 1317 / 1315 | 412 / 421 | 10 / 1 |
| tail_quick_control_inlining_59 | 1181 / 1315 | 198 / 421 | 6 / 3 |
| tail_quick_control_inlining_72 | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_control_legacy_debug | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_control_size_helpers_speed_leaf | 1323 / 1315 | 357 / 421 | 5 / 4 |
| tail_quick_erase_noinline_strong | 1317 / 1315 | 412 / 421 | 10 / 1 |
| tail_quick_erase_noinline_weak | 1317 / 1315 | 412 / 421 | 10 / 1 |
| tail_quick_erase_plain_strong | 1317 / 1315 | 412 / 421 | 10 / 1 |
| tail_quick_erase_plain_weak | 1317 / 1315 | 412 / 421 | 10 / 1 |
| tail_quick_escape_external_by_const_ref | 1318 / 1315 | 286 / 421 | 13 / 1 |
| tail_quick_escape_external_by_ref | 1318 / 1315 | 286 / 421 | 13 / 1 |
| tail_quick_escape_external_by_value | 1315 / 1315 | 421 / 421 | 13 / 1 |
| tail_quick_escape_local_alias_not_escaping | 1315 / 1315 | 421 / 421 | 13 / 1 |
| tail_quick_escape_local_by_const_ref | 1315 / 1315 | 421 / 421 | 13 / 1 |
| tail_quick_escape_local_by_ref | 1315 / 1315 | 421 / 421 | 13 / 1 |
| tail_quick_escape_pointer_store | 1318 / 1315 | 286 / 421 | 13 / 1 |
| tail_quick_escape_pointer_store_at_return | 1318 / 1315 | 286 / 421 | 13 / 1 |
| tail_quick_escape_unchanged | 1315 / 1315 | 421 / 421 | 13 / 1 |
| tail_quick_exact_leaf_external | 1315 / 1315 | 421 / 421 | 13 / 1 |
| tail_quick_exact_leaf_local_inline | 1315 / 1315 | 421 / 421 | 13 / 2 |
| tail_quick_exact_leaf_local_noinline | 1315 / 1315 | 421 / 421 | 13 / 2 |
| tail_quick_expr_after_unused | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_append_calls | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_append_parentheses | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_assign_condition | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_const_ref | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_expr_decl_condition_named | 1325 / 1315 | 384 / 421 | 6 / 3 |
| tail_quick_expr_double_bracket | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_early_count | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_expr_existing_total | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_existing_total_effect | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_existing_total_inside | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_initialization_brackets | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_const_ref_default | 1340 / 1315 | 342 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_const_ref_positive_scope | 1340 / 1315 | 342 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_scope_only_default | 1312 / 1315 | 277 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_scope_only_positive_scope | 1312 / 1315 | 277 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_string_cast_default | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_string_cast_positive_scope | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_string_default | 1340 / 1315 | 342 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_string_positive_scope | 1340 / 1315 | 342 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_typed_callee_default | 1320 / 1315 | 267 / 421 | 6 / 3 |
| tail_quick_expr_menuentry_typed_callee_positive_scope | 1320 / 1315 | 267 / 421 | 6 / 3 |
| tail_quick_expr_mutable | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_negative_first | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_number_local | 1321 / 1315 | 358 / 421 | 6 / 3 |
| tail_quick_expr_prefix_local | 1321 / 1315 | 353 / 421 | 6 / 3 |
| tail_quick_expr_prefix_ref | 1321 / 1315 | 353 / 421 | 6 / 3 |
| tail_quick_expr_prefix_then_number | 1325 / 1315 | 384 / 421 | 6 / 3 |
| tail_quick_expr_same_twice | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_test_conjunction | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_test_disjunction | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_test_lhs_zero | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_test_nonnegative | 1321 / 1315 | 276 / 421 | 6 / 3 |
| tail_quick_expr_test_not_nonpositive | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_test_ternary | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_expr_unsigned_positive | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_owner_release_depth_one_fable_forceinline | 1167 / 1315 | 197 / 421 | 9 / 2 |
| tail_quick_owner_release_depth_one_inline | 1167 / 1315 | 197 / 421 | 9 / 2 |
| tail_quick_owner_release_depth_three_fable_forceinline | 1323 / 1315 | 357 / 421 | 9 / 2 |
| tail_quick_owner_release_depth_three_inline | 1323 / 1315 | 357 / 421 | 9 / 2 |
| tail_quick_owner_release_depth_two_fable_forceinline | 1323 / 1315 | 357 / 421 | 9 / 2 |
| tail_quick_owner_release_depth_two_inline | 1323 / 1315 | 357 / 421 | 9 / 2 |
| tail_quick_owner_release_no_depth_fable_forceinline | 1323 / 1315 | 357 / 421 | 9 / 2 |
| tail_quick_owner_release_no_depth_inline | 1323 / 1315 | 357 / 421 | 9 / 2 |
| tail_quick_owner_tree_size_fable_forceinline | 1323 / 1315 | 357 / 421 | 7 / 2 |
| tail_quick_owner_tree_size_inline | 1323 / 1315 | 357 / 421 | 7 / 2 |
| tail_quick_owner_tree_size_t_off_fable_forceinline | 1323 / 1315 | 357 / 421 | 7 / 2 |
| tail_quick_owner_tree_size_t_off_inline | 1323 / 1315 | 357 / 421 | 7 / 2 |
| tail_quick_owner_tree_speed_fable_forceinline | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_owner_tree_speed_inline | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_reduce_assert_arm_only_early | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_reduce_assert_arm_only_late | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_reduce_assert_arm_only_plain | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_assert_only_early | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_reduce_assert_only_late | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_reduce_assert_only_plain | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_drop_assert_dead_arm | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_drop_cdefclassbase_getsubdef_noinline | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_drop_cdefinitionmanager | COMPILE_FAIL | — | — |
| tail_quick_reduce_drop_cdefinitionmanager_getdef_global_body | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_drop_cdefpointer_setdynamiccast_reread | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_drop_cexpressionsubdef | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_drop_cexpressionsubdef_statgettypename | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_drop_cgamedefinitionmanager | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_drop_cresourcelist | COMPILE_FAIL | — | — |
| tail_quick_reduce_drop_ctcquickaccessmenu_expressions | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_full_with_owners | 1315 / 1315 | 421 / 421 | 13 / 2 |
| tail_quick_reduce_full_without_dead_arm | 1315 / 1315 | 421 / 421 | 9 / 4 |
| tail_quick_reduce_keep_assert_dead_arm | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_cdefclassbase_getsubdef_noinline | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_cdefinitionmanager | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_cdefinitionmanager_getdef_global_body | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_cdefpointer_setdynamiccast_reread | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_cexpressionsubdef | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_cexpressionsubdef_statgettypename | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_cgamedefinitionmanager | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_cresourcelist | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_keep_ctcquickaccessmenu_expressions | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_reduce_macros_with_owners | 1318 / 1315 | 286 / 421 | 10 / 1 |
| tail_quick_reduce_new_defines_only | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_scope_counter_right_after_aliased | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_counter_right_after_inverted | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_counter_right_after_normal | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_counter_right_after_reference | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_counter_separate_aliased | 1317 / 1315 | 411 / 421 | 6 / 3 |
| tail_quick_scope_counter_separate_inverted | 1317 / 1315 | 411 / 421 | 6 / 3 |
| tail_quick_scope_counter_separate_normal | 1317 / 1315 | 411 / 421 | 6 / 3 |
| tail_quick_scope_counter_separate_reference | 1317 / 1315 | 411 / 421 | 6 / 3 |
| tail_quick_scope_early_assigned_cond | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_scope_early_assigned_do | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_scope_early_assigned_neg_count | 1319 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_early_assigned_nested | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_scope_early_assigned_sign_check | 1327 / 1315 | 380 / 421 | 6 / 3 |
| tail_quick_scope_early_assigned_true | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_scope_early_title_bank | 1320 / 1315 | 406 / 421 | 6 / 3 |
| tail_quick_scope_early_title_build_then_scope | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_scope_early_title_init | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_scope_early_title_named_symbol | 1315 / 1315 | 281 / 421 | 6 / 3 |
| tail_quick_scope_early_title_parens | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_scope_early_title_ref | 1316 / 1315 | 381 / 421 | 6 / 3 |
| tail_quick_scope_early_title_symbol | 1315 / 1315 | 413 / 421 | 6 / 3 |
| tail_quick_scope_late_assigned_cond | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_late_assigned_do | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_late_assigned_neg_count | 1318 / 1315 | 269 / 421 | 6 / 3 |
| tail_quick_scope_late_assigned_nested | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_late_assigned_sign_check | 1325 / 1315 | 384 / 421 | 6 / 3 |
| tail_quick_scope_late_assigned_true | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_late_title_bank | 1318 / 1315 | 407 / 421 | 6 / 3 |
| tail_quick_scope_late_title_build_then_scope | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_late_title_init | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_late_title_named_symbol | 1317 / 1315 | 281 / 421 | 6 / 3 |
| tail_quick_scope_late_title_parens | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_scope_late_title_ref | 1314 / 1315 | 382 / 421 | 6 / 3 |
| tail_quick_scope_late_title_symbol | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_shape_abilities_zero | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_shape_available_total_display_count | 1336 / 1315 | 292 / 421 | 6 / 3 |
| tail_quick_shape_available_total_named_count | 1329 / 1315 | 320 / 421 | 6 / 3 |
| tail_quick_shape_available_total_named_count_inside | 1329 / 1315 | 320 / 421 | 6 / 3 |
| tail_quick_shape_available_total_plain_count | 1336 / 1315 | 292 / 421 | 6 / 3 |
| tail_quick_shape_counts_before_list_cast | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_shape_counts_combined | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_shape_counts_decl_then_set | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_shape_counts_expressions_first | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_shape_early_expressions_empty | 1328 / 1315 | 362 / 421 | 6 / 3 |
| tail_quick_shape_early_items_empty | 1334 / 1315 | 345 / 421 | 6 / 3 |
| tail_quick_shape_expressions_first_display_count | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_shape_expressions_first_named_count | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_shape_expressions_first_named_count_inside | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_shape_expressions_first_plain_count | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_shape_expressions_or_items_explicit_display_count | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_shape_expressions_or_items_explicit_named_count | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_shape_expressions_or_items_explicit_named_count_inside | 1317 / 1315 | 412 / 421 | 6 / 3 |
| tail_quick_shape_expressions_or_items_explicit_plain_count | 1318 / 1315 | 286 / 421 | 6 / 3 |
| tail_quick_shape_items_first_display_count | 1323 / 1315 | 357 / 421 | 6 / 3 |
| tail_quick_shape_items_first_named_count | 1329 / 1315 | 332 / 421 | 6 / 3 |
| tail_quick_shape_items_first_named_count_inside | 1329 / 1315 | 332 / 421 | 6 / 3 |
| tail_quick_shape_items_then_total_display_count | 1348 / 1315 | 284 / 421 | 6 / 3 |
| tail_quick_shape_items_then_total_named_count | 1339 / 1315 | 324 / 421 | 6 / 3 |
| tail_quick_shape_items_then_total_named_count_inside | 1339 / 1315 | 324 / 421 | 6 / 3 |
| tail_quick_shape_items_then_total_plain_count | 1348 / 1315 | 284 / 421 | 6 / 3 |
| tail_quick_visible_getexpr_early_after | 1315 / 1315 | 421 / 421 | 9 / 4 |
| tail_quick_visible_getexpr_early_before | 1315 / 1315 | 421 / 421 | 9 / 4 |
| tail_quick_visible_getexpr_late_after | 1315 / 1315 | 421 / 421 | 9 / 4 |
| tail_quick_visible_getexpr_late_before | 1315 / 1315 | 421 / 421 | 9 / 4 |
| tail_quick_visible_getexpr_plain_after | 1315 / 1315 | 421 / 421 | 9 / 4 |
| tail_quick_visible_getexpr_plain_before | 1315 / 1315 | 421 / 421 | 9 / 4 |

The selected row has 15 exact mapped weak bodies after the independently established binding corrections. Its strong GetExpressionsListUI owner is graded separately.

Both assigned leaves now coexist byte-exact in one genuine source TU: List 1056/1056 and Activate 1315/1315. All 57 List operands have supporting evidence. Use rows/00627be0/paired_exact_review/PAIRED_SOURCE.cpp with the cached paired header overlay. The source is still under review because emitted category and vector helpers differ. The full factorial ablations are recorded in runtime/quick_reduce_chain.log and runtime/quick_pair_specific.log. The chain must contain both actual category bodies and both vector bodies at once. Earlier probes retaining the generic overflow declaration inadvertently omitted that final link.

Accepted the complete shared TU after verifying both reserved functions, two native category builders, the real direct list-base destructor target and all other mapped copies. The two previously partial row views and fixture histories are retained locally. The duplicated two-object link is superseded by the original single-object ownership, recorded in STRONG_COPIES.json.
