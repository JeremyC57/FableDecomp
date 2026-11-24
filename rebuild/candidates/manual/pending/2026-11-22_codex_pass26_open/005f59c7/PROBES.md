# 005F59C7 probe trail

The ten object grades below include the packet baseline and the two retained historical regrades carried by this pass. Five distinct probes were built after the assignment. The behavior test builds are independent of the retail object grades.

| Grade | Candidate | Observation |
| --- | --- | --- |
| 1–3 | `baseline`, `archive1`, `archive2` | The inherited shapes each built to 2649/2652 bytes. |
| 4 | `expressions_original` | Original text-bank access and donor category types retained the same leaf grade. |
| 5 | `donor_accessors` | Replaced direct inventory-field reads with FableWin's getters. 2649/2652 bytes with 11 equal and 4 different mapped helper copies on the supplied binding table. |
| 6 | `shared_subdef_and_map` | Use the shared GetSubDef owner and set speed around both template owners' includes. The compiler deferred their bodies to the final size mode. Leaf and helper grades were unchanged. |
| 7 | `donor_pointer_calls` | Recover the donor's GetParentThing and counted-pointer Get calls. An end-of-file speed reset makes both referenced helper bodies exact, but regresses six native size-built helpers and introduces wrong leaf destructor targets. |
| 8 | `explicit_shared_instances` | Explicit instantiations under a temporary speed setting still defer generated bodies to the translation unit's final setting. A force-inline HasTC still emits the COMDAT wrapper. |
| 9 | `speed_helper_specializations` | Shared typed specializations receive the native default speed setting directly. The leaf bytes stayed identical to the donor accessor build. The two referenced copies became exact, and all eleven previously equal copies stayed equal. |
| 10 | `typed_text_bank` | The existing typed smart-pointer owner for GetTextBank agrees with the debug donor and preserves the 2649-byte leaf and both exact helper copies. Prior selection. |

| 11 | `shared_speed_bitset` | Form the genuine checked STLport bitset specialization before CThing and preserve the caller's size mode. The leaf stays at 2649/2652. Both HasTC and the bitset method match, and the corrected comparison reports 18 EQ with zero NEQ. Prior selection after the owner authorized continued evidence-based grading. |

### Follow-up donor and compiler probes

- `original_textbank_checked_deref` gives 2651/2652 bytes, 638/869 aligned instructions, and 15 supplied equals with 1 differing.
- `original_textbank_checked_deref_statement` gives 2651/2652 bytes, 638/869 aligned instructions, and 15 supplied equals with 1 differing.
- `original_component_checked_get` gives 2649/2652 bytes, 638/869 aligned instructions, and 15 supplied equals with 1 differing.
- `original_component_checked_arrow` gives 2649/2652 bytes, 638/869 aligned instructions, and 15 supplied equals with 1 differing.
- `original_component_checked_accessors` gives 2649/2652 bytes, 638/869 aligned instructions, and 15 supplied equals with 1 differing.

The selected source and behavior test were independently replayed on merged tree c6a01d2430da, and the corrected report confirms all 18 helpers and 278 checked operands.

### Category title and cleanup scopes

- `expressions_scoped_common_exit` uses a common return through container destruction. It gives 2632/2652 bytes and 510/869 aligned instructions.
- `expressions_return_after_cleanup` and `expressions_scoped_result_exit` give 2649/2652 bytes and 638/869 aligned instructions. Both keep the text-bank register assignment.
- `expressions_scoped_category_title` ends the wide-string lifetime before the final return and gives 2649/2652 bytes and 572/869 aligned instructions.

### Counted-component release checks

FableWin 01ABFFD7 checks that Info and Data are both present or both absent before, during and after the original release. FableWin 01AC1C9D names the corresponding `Validate`. `expressions_checked_release_validate` and `expressions_checked_release_arms` reproduce the release-dead checks in named and direct forms. Both preserve the selected 2649/2652 bytes and 638/869 aligned instructions.

### Typed text-bank access

`expressions_force_getter`, `expressions_counted_access`, `expressions_force_counted_access` and `expressions_named_counted_access` test the actual typed accessor and equivalent counted-pointer read layers. Each gives 2649/2652 bytes and 638/869 aligned instructions. `expressions_const_counted_owner` gives 2651/2652 and 638/869 after naming a counted-owner reference. The text-bank register is still EDI.

### Scope, counted release, and the optimized donor

Ego_r 0058465D uses EDI for the text bank. It calls all three counted-component releases and the five Microsoft STL `_Tidy` functions out of line after the last category. Retail also expands the counted-pointer destructors to out-of-line ReleaseData calls and expands the STLport vector cleanup. Neither donor places an extra source operation after the last category.

- `expressions_release_count_expression` and `expressions_explicit_list_component_result` give 2649/2652 bytes and 638/869 aligned instructions. `expressions_title_scope_before_list_exit` gives 2649/2652 and 565/869.
- `expressions_named_final_category_strings`, `expressions_named_first_category_symbol`, `expressions_named_first_symbol_and_last_category` and `expressions_named_category_strings` preserve the original temporary order and produce 2649/2652 and 638/869.
- The original STLport 4.6.2 allocator assertion, named vector allocation and forcing the existing destruction wrappers are `expressions_stlport_checked_deallocation`, `expressions_stlport_named_vector_allocation` and `expressions_stlport_expanded_vector_destruction`. All give 2649/2652 bytes and 638/869 aligned instructions.
- The merged verification replays the selected `shared_speed_bitset` on tree 6a20af7d676e, with all 18 corrected helper copies, 278 resolved operand targets and both behavior tests passing.

### Original PDB statement lines

FableWin stream 271 and Ego_r stream 274 contain CodeView line records for `tc_inventory_abilities.cpp` with the same MD5 `b0b1c41ea06569bdefcae2b403d2692f`. Both identify one vector declaration at line 3059, the text-bank reference at 3197, the title construction at 3199, the four category title assignments and calls at 3201–3211, and the return with cleanup at 3213. The FableWin 3067 diagnostic prints `Failed success assert on GetDefinitionManager().GetDef(items[count].first, &pthing_def)`. All three new probes have the selected leaf's byte-for-byte SHA-256 `00b6236894c1b895e30a75904861b91648f50016ab2a2787d10b70f489ae6ad2` and the same 638/869 aligned instructions.

- `expressions_original_success_assert` preserves the verified call and restores its release-dead success diagnostic.
- `expressions_original_line_scopes` also groups the five vectors at their recorded declaration and braces the frame-child statement.
- `expressions_original_inventory_declaration` places the inventory pointer immediately before its output use.

### Typed owners and function-wide allocation

- `expressions_visible_called_component_release` keeps the genuine counted-component releases visible at their native out-of-line calls. `expressions_original_typed_null_def_ctor` passes the original null-definition enum at the three definition-pointer constructions. Both reproduce the selected leaf byte for byte.
- `expressions_visible_all_items_traversal` adds the exact 33-byte `NInventory::CTCInventoryBase::GetAllItems` loop over the original CCategory vector. `expressions_visible_definition_manager_owner` adds the exact 8-byte manager forwarding method. `expressions_visible_peek_component_typed_owner` adds the exact 6-byte typed GetThingManager and 9-byte PeekComponent bodies. Each leaves the complete leaf unchanged. The first PeekComponent attempt lacked the CClassFactory declaration opt-in and produced no object.
- FableWin 01AC0EC4 records the original typed GetTC path through HasTC, mutable GetTC and the checked cast. `expressions_original_gettc_chain` and `expressions_original_gettc_named_base` select the authentic call chain but leave Find out of line, giving 2634/2652 and 630/869. `expressions_expanded_original_gettc_chain` and `expressions_expanded_original_find_comparator` retain the original native expansion of Find and reproduce the selected object bytes, including all mapped helper outcomes.
- The selected `expressions_original_procedure_order` retains the original comparator, the vector declaration grouping, and the frame-child scope. It places the category builder after the leaf, as in both original donor compilands. All leaf bytes match the prior selection, corrected helper comparison remains 18 equal, and 278 semantic operands resolve.
- FableWin lines 3064 and 3112 both show the original VC6-style for-scope guard documented in GOTCHAS.md. `expressions_original_scoped_for_loops`, `expressions_original_slot_scoped_for_loop`, and `expressions_original_scoped_for_and_verify` test the original markers individually and together with the confirmed success assertion. All three give the selected bytes.
- FableWin 01AC0C8A, 01AC2F74 and 018BE540 identify the typed checked-cast result `new_val` and the debug assertion `new_val!=NULL`. Both the existing shared assertion and the reconstructed result check preserve the selected leaf bytes.

FableWin 01EC9837 and 01EC983B clear the two original pointer locals at frame offsets -128 and -124, source lines 3096 and 3097. Ego_d names those slots pexpressions_list_component and pexpressions_list and records two four-byte statements. The calls at lines 3100 and 3101 then assign their final values. The selected source and test now reproduce those statements separately. All four initialization probes preserve the same 2649-byte leaf; the paired source passed the native and LP64 fixtures and the corrected operand audit.

FableWin 018DAAD0 and 01BC9E15 show that both CDefPointer<CThingBaseDef const> and CDefPointer<CExpressionDef const> test Object, call CIVCountedPointeeBase::Release, and write zero on the nonnull branch. FableWin 018727C0 shows the separate decrement and test, deletion and explicit else-return of that original owner. The accepted source enables the same shared PDB-backed release and direct dereference options as the region script. All previously selected leaf bytes, 278 operands and 18 corrected helper copies were independently rechecked. A separate reference-to-count probe reaches 802/869 instructions at 2682/2652 bytes and hoists the string-length sentinel, but loses helper matches and introduces extra comparisons. It identifies where the allocation decision can change; it is not selected.

FableWin source line 3070 contains both short-circuited definition lookups. Its original code block at +0x3E3..+0x4C0 contains exp_type. The rebuilt source and test now use that common condition. All 278 operands and 18 corrected helper copies were replayed. Native and portable tests pass. The 2649-byte primary leaf is unchanged. The original allocator trace gives the zero constant priority 48 (67 references), behind rTextBank at 56. The diagnostic with six additional references to zero gives it priority 61, ahead of rTextBank. This explains the role exchange but does not establish the source of those references.

## Per-block allocation evidence

The read-only VC7.1 trace preserves every generated function and normalized COFF relocation. The bank reference gets all 56 priority points in a single block from the bank lookup through the four category calls. Its cost of seven is multiplied by eight referenced candidates. The count-reference diagnostic preserves that same bank block. It raises zero from 48 to 61 solely through five definition cleanup blocks. The early missing-inventory return contributes two points, the three item-loop definition releases contribute three apiece, and the final definition release contributes two. Each added contribution comes from a separate zero comparison after the decrement. The native and selected leaves use the decrement flags at all five sites.

- `expr_live_weights_*` tried the evidenced local lifetimes around the definition loop, containment loop, action block and assigned branch. Direct reference initialization, grouped bank expressions and an explicit wide-string assignment also retained the selected bytes.
- `expr_cleanup_weights_*` tried the genuine count member with grouped increments and comparisons and an equivalent lvalue cast. Explicit STLport default allocator construction, individual switch scope and the unused original total's post-increment also retained 2,649 bytes and 638/869 instructions.

The selected leaf replayed byte-identically on `17db647dc01f`, with the usual per-TU anonymous symbol suffix normalized. The corrected 18-helper comparison, 278-target audit, native fixture and strict portable C++03 fixture passed. `BINDING_EVIDENCE.json` records the exact control and its source boundary.

FableWin and Ego_d independently name the category builder parameters menuparam, pexpressions_list, expressions, ptop_list, ring_pic_name and text_tag. Recovered all its scoped locals, including plist_pic_component in two sibling scopes, psubtitle_real and the same ptext variable at both title sites. Two debug calls identify the original checked viewport and description casts. Both donors also give misc_thing_def_index and pmiscthingsdef in GetMiscThingsDef. The selected source restores the two original helper bodies. Their 1233 and 101 bytes are each exact. All seven misc-helper operands resolve, in addition to the existing 278 verified row and helper operands. The primary leaf retains 638/869 instructions. Native and strict portable row fixtures passed.

FableWin 01DFC5A0 and Ego_r 0075F5C0 independently identify the GetDef formals global_index and def, local pbase_def and the nested positive-index condition at lib_definition_manager.inl lines 43–55. FableWin 01DFC5DE calls its actual Get accessor. All three shared 98-byte instantiations, the exact 1233-byte category helper, and the exact 101-byte misc helper retain the prior sections and relocated symbols. The reserved leaf remains 2649/2652 bytes. The native and strict portable fixtures pass.

# Compiler attempts for 005f59c7

Each line records a compiler invocation. Regrading an inherited file establishes a baseline. Historical rechecks and compiler failures do not establish a new successful source shape.

- `baseline`  DIFFER  2649/2652  174 differing instruction groups.
- `archive1`  DIFFER  2649/2652  174 differing instruction groups.
- `archive2`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original`  DIFFER  2649/2652  174 differing instruction groups.
- `donor_accessors`  DIFFER  2649/2652  174 differing instruction groups.
- `shared_subdef_and_map`  DIFFER  2649/2652  174 differing instruction groups.
- `donor_pointer_calls`  DIFFER  2649/2652  174 differing instruction groups.
- `explicit_shared_instances`  DIFFER  2649/2652  174 differing instruction groups.
- `speed_helper_specializations`  DIFFER  2649/2652  174 differing instruction groups.
- `typed_text_bank`  DIFFER  2649/2652  174 differing instruction groups.
- `final_bc43fead2811_2d5b883f`  COMPILE_FAIL  —/2652
- `final_7fecd714b055_2d5b883f`  DIFFER  2649/2652  174 differing instruction groups.
- `shared_speed_bitset`  DIFFER  2649/2652  174 differing instruction groups.
- `final_0d5d4a262147_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `original_textbank_checked_deref`  DIFFER  2651/2652  174 differing instruction groups.
- `original_textbank_checked_deref_statement`  DIFFER  2651/2652  174 differing instruction groups.
- `original_component_checked_get`  DIFFER  2649/2652  174 differing instruction groups.
- `original_component_checked_arrow`  DIFFER  2649/2652  174 differing instruction groups.
- `original_component_checked_accessors`  DIFFER  2649/2652  174 differing instruction groups.
- `final_b2d50dac51c5_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `final_c6a01d2430da_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `final_baaf0e57189b_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_scoped_common_exit`  DIFFER  2632/2652  183 differing instruction groups.
- `expressions_return_after_cleanup`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_scoped_result_exit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_scoped_category_title`  DIFFER  2649/2652  212 differing instruction groups.
- `expressions_checked_release_validate`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_checked_release_arms`  DIFFER  2649/2652  174 differing instruction groups.
- `partial_tree_efee91b366bb_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_force_getter`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_counted_access`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_force_counted_access`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_named_counted_access`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_const_counted_owner`  DIFFER  2651/2652  174 differing instruction groups.
- `expressions_release_count_expression`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_title_scope_before_list_exit`  DIFFER  2649/2652  204 differing instruction groups.
- `expressions_explicit_list_component_result`  DIFFER  2649/2652  174 differing instruction groups.
- `partial_tree_2de865f160dd_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `final_2de865f160dd_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_named_final_category_strings`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_named_first_category_symbol`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_named_first_symbol_and_last_category`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_named_category_strings`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_stlport_checked_deallocation`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_stlport_named_vector_allocation`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_stlport_expanded_vector_destruction`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_success_assert`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_line_scopes`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_inventory_declaration`  DIFFER  2649/2652  174 differing instruction groups.
- `final_6a20af7d676e_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `partial_tree_6a20af7d676e_83700dd1`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_visible_called_component_release`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_visible_all_items_traversal`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_typed_null_def_ctor`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_visible_definition_manager_owner`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_visible_peek_component_owner`  COMPILE_FAIL  —/2652
- `expressions_original_gettc_chain`  DIFFER  2634/2652  174 differing instruction groups.
- `expressions_original_gettc_named_base`  DIFFER  2634/2652  174 differing instruction groups.
- `expressions_expanded_original_gettc_chain`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_expanded_original_find_comparator`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_visible_peek_component_typed_owner`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_procedure_order`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_scoped_for_loops`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_slot_scoped_for_loop`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_scoped_for_and_verify`  DIFFER  2649/2652  174 differing instruction groups.
- `final_6a20af7d676e_30bc5f45`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_existing_checked_cast_owner`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_checked_cast_result`  DIFFER  2649/2652  174 differing instruction groups.
- `main_original_slot_initializers_both`  DIFFER  2649/2652  174 differing instruction groups.
- `main_original_slot_initializers_component`  DIFFER  2649/2652  174 differing instruction groups.
- `main_original_slot_initializers_list`  DIFFER  2649/2652  174 differing instruction groups.
- `main_original_slot_initializers_typed_zero`  DIFFER  2649/2652  174 differing instruction groups.
- `partial_tree_6a20af7d676e_bcc89bf8`  DIFFER  2649/2652  174 differing instruction groups.
- `observed_build_defaults_gr`  DIFFER  2649/2652  174 differing instruction groups.
- `observed_build_defaults_gz`  DIFFER  2649/2652  174 differing instruction groups.
- `observed_build_defaults_gdmd`  DIFFER  2640/2652  175 differing instruction groups.
- `observed_build_defaults_grmd`  DIFFER  2640/2652  175 differing instruction groups.
- `observed_build_defaults_grgf`  DIFFER  2649/2652  174 differing instruction groups.
- `observed_build_defaults_grgy`  DIFFER  2649/2652  174 differing instruction groups.
- `observed_build_defaults_grgtgr`  DIFFER  2649/2652  174 differing instruction groups.
- `observed_build_defaults_zp4gr`  COMPILE_FAIL  —/2652
- `observed_build_defaults_gy`  DIFFER  2649/2652  174 differing instruction groups.
- `partial_tree_1d4aa74860b1_bcc89bf8`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_neighbor_profile_size_fp`  DIFFER  2788/2652  206 differing instruction groups.
- `expressions_neighbor_profile_size_omit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_neighbor_profile_size_fp_expand`  DIFFER  2788/2652  206 differing instruction groups.
- `expressions_neighbor_profile_size_fp_no_pool`  DIFFER  2788/2652  206 differing instruction groups.
- `partial_tree_a6c63044ee0e_bcc89bf8`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_neighbor_profile_size_fp_intrinsic`  DIFFER  2788/2652  206 differing instruction groups.
- `expressions_neighbor_profile_speed_fp`  DIFFER  2788/2652  206 differing instruction groups.
- `expressions_neighbor_profile_speed_pragma_only`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_neighbor_profile_size_fp_noglobal`  DIFFER  5139/2652  288 differing instruction groups.
- `expressions_neighbor_profile_size_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_neighbor_profile_size_disableinline`  DIFFER  2732/2652  178 differing instruction groups.
- `expressions_literal_materialization_implicit_implicit_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_literal_materialization_implicit_implicit_value`  DIFFER  2637/2652  196 differing instruction groups.
- `expressions_literal_materialization_implicit_implicit_length`  DIFFER  2637/2652  196 differing instruction groups.
- `expressions_literal_materialization_implicit_value_implicit`  DIFFER  2637/2652  176 differing instruction groups.
- `expressions_literal_materialization_implicit_value_value`  DIFFER  2625/2652  93 differing instruction groups.
- `expressions_literal_materialization_implicit_value_length`  DIFFER  2625/2652  93 differing instruction groups.
- `expressions_literal_materialization_implicit_length_implicit`  DIFFER  2637/2652  176 differing instruction groups.
- `expressions_literal_materialization_implicit_length_value`  DIFFER  2625/2652  93 differing instruction groups.
- `expressions_literal_materialization_implicit_length_length`  DIFFER  2625/2652  93 differing instruction groups.
- `expressions_literal_materialization_value_implicit_implicit`  DIFFER  2637/2652  158 differing instruction groups.
- `expressions_literal_materialization_value_implicit_value`  DIFFER  2625/2652  93 differing instruction groups.
- `expressions_literal_materialization_value_implicit_length`  DIFFER  2625/2652  93 differing instruction groups.
- `expressions_literal_materialization_value_value_implicit`  DIFFER  2625/2652  89 differing instruction groups.
- `expressions_literal_materialization_value_value_value`  DIFFER  2613/2652  120 differing instruction groups.
- `expressions_literal_materialization_value_value_length`  DIFFER  2613/2652  120 differing instruction groups.
- `expressions_literal_materialization_value_length_implicit`  DIFFER  2625/2652  89 differing instruction groups.
- `expressions_literal_materialization_value_length_value`  DIFFER  2613/2652  120 differing instruction groups.
- `expressions_literal_materialization_value_length_length`  DIFFER  2613/2652  120 differing instruction groups.
- `expressions_literal_materialization_length_implicit_implicit`  DIFFER  2637/2652  158 differing instruction groups.
- `expressions_literal_materialization_length_implicit_value`  DIFFER  2625/2652  93 differing instruction groups.
- `expressions_literal_materialization_length_implicit_length`  DIFFER  2625/2652  93 differing instruction groups.
- `expressions_literal_materialization_length_value_implicit`  DIFFER  2625/2652  89 differing instruction groups.
- `expressions_literal_materialization_length_value_value`  DIFFER  2613/2652  120 differing instruction groups.
- `expressions_literal_materialization_length_value_length`  DIFFER  2613/2652  120 differing instruction groups.
- `expressions_literal_materialization_length_length_implicit`  DIFFER  2625/2652  89 differing instruction groups.
- `expressions_literal_materialization_length_length_value`  DIFFER  2613/2652  120 differing instruction groups.
- `expressions_literal_materialization_length_length_length`  DIFFER  2613/2652  120 differing instruction groups.
- `expressions_string_count_default_signed_long`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_string_count_default_complement`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_string_count_default_hex_unsigned`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_string_count_default_long_type`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_string_count_default_grouped`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_string_count_default_unsigned_complement`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_g5_selected`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_g5_full_size`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_g5_inverted`  DIFFER  3772/2652  154 differing instruction groups.
- `expressions_saved_constant_g5_force_speed`  DIFFER  3772/2652  154 differing instruction groups.
- `expressions_saved_constant_g6_selected`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_g6_full_size`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_g6_inverted`  DIFFER  3772/2652  154 differing instruction groups.
- `expressions_saved_constant_g6_force_speed`  DIFFER  3772/2652  154 differing instruction groups.
- `expressions_saved_constant_g7_selected`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_g7_full_size`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_g7_inverted`  DIFFER  3780/2652  165 differing instruction groups.
- `expressions_saved_constant_g7_force_speed`  DIFFER  3780/2652  165 differing instruction groups.
- `expressions_saved_constant_gb_selected`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_gb_full_size`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_saved_constant_gb_inverted`  DIFFER  3772/2652  154 differing instruction groups.
- `expressions_saved_constant_gb_force_speed`  DIFFER  3772/2652  154 differing instruction groups.
- `expressions_original_def_scope_release_original_scopes_selected`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_def_scope_release_original_scopes_direct`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_def_scope_release_count_selected`  DIFFER  2615/2652  184 differing instruction groups.
- `expressions_original_def_scope_release_count_direct`  DIFFER  2615/2652  184 differing instruction groups.
- `expressions_original_def_scope_normal_selected`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_def_scope_normal_direct`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_original_def_scope_countref_selected`  DIFFER  2682/2652  55 differing instruction groups.
- `expressions_original_def_scope_countref_direct`  DIFFER  2682/2652  55 differing instruction groups.
- `expressions_true_count_flow_pre_eq_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_pre_eq_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_pre_not_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_pre_not_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_pre_zero_first_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_pre_zero_first_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_pre_unsigned_zero_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_pre_unsigned_zero_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_post_eq_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_post_eq_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_post_not_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_post_not_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_post_zero_first_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_post_zero_first_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_post_unsigned_zero_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_post_unsigned_zero_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_sub_eq_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_sub_eq_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_sub_not_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_sub_not_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_sub_zero_first_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_sub_zero_first_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_sub_unsigned_zero_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_sub_unsigned_zero_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_assign_eq_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_assign_eq_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_assign_not_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_assign_not_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_assign_zero_first_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_assign_zero_first_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_assign_unsigned_zero_implicit`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_true_count_flow_assign_unsigned_zero_member`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_line_3070_normal`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_line_3070_explicit_true`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_line_3070_double_group`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_line_3070_reverse_zero`  DIFFER  2649/2652  174 differing instruction groups.
- `expressions_line_3070_early_failure`  DIFFER  2692/2652  183 differing instruction groups.
- `root_expressions_debugline_3070_original_scope`  DIFFER  2649/2652  174 differing instruction groups.
- `partial_tree_ecee975d2a37_312bb061`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_definition_scope`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_containment_scope`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_action_scope`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_assignment_scope`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_definition_containment_scopes`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_contained_action_scope`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_complete_lexical_scopes`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_direct_bank_reference`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_bank_rhs_grouped`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_bank_caller_grouped`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_bank_assignment_rhs_grouped`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_bank_explicit_wide_assignment`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_live_weights_distinct_case_blocks`  COMPILE_FAIL  —/2652
- `partial_tree_17db647dc01f_312bb061`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_member_comma`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_group_before_branch`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_group_member_lvalue`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_unsigned_member_binding`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_address_and_deref`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_guard_branch_diagnostic`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_default_allocator_only_items`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_default_allocator_all_vectors`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_switch_inner_blocks`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_post_increment_inside`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_cleanup_weights_post_increment_direct`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_explicit_lookup`  DIFFER  2637/2652  158 differing instruction groups.
- `expr_string_temp_explicit_icon`  DIFFER  2637/2652  176 differing instruction groups.
- `expr_string_temp_explicit_guide`  DIFFER  2637/2652  196 differing instruction groups.
- `expr_string_temp_explicit_create`  DIFFER  2630/2652  180 differing instruction groups.
- `expr_string_temp_explicit_find`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_explicit_lookup_icon`  DIFFER  2625/2652  89 differing instruction groups.
- `expr_string_temp_explicit_lookup_guide`  DIFFER  2625/2652  93 differing instruction groups.
- `expr_string_temp_explicit_icon_guide`  DIFFER  2625/2652  93 differing instruction groups.
- `expr_string_temp_explicit_lookup_icon_guide`  DIFFER  2613/2652  120 differing instruction groups.
- `expr_string_temp_explicit_lookup_icon_guide_create_find`  DIFFER  2594/2652  173 differing instruction groups.
- `expr_string_temp_cast_ref_lookup`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_cast_ref_icon`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_cast_ref_guide`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_cast_ref_create`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_cast_ref_find`  DIFFER  2688/2652  135 differing instruction groups.
- `expr_string_temp_cast_ref_lookup_icon`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_cast_ref_lookup_guide`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_cast_ref_icon_guide`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_cast_ref_lookup_icon_guide`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_string_temp_cast_ref_lookup_icon_guide_create_find`  DIFFER  2688/2652  135 differing instruction groups.
- `expr_orig_category_named`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_orig_category_named_misc`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_orig_category_authentic_scopes_casts`  DIFFER  2649/2652  174 differing instruction groups.
- `partial_tree_5bf00538c146_54e4b771`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_raw_global_lookup_exact_lookup`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_raw_global_lookup_exact_lookup_explicit_null`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_raw_global_lookup_original_subdef`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_raw_global_lookup_original_both`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_raw_global_lookup_original_both_direct_init`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_original_header_getdef_header_out_of_line`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_original_header_getdef_header_plain_inline`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_original_header_getdef_header_template`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_original_header_getdef_header_original_pragma`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_original_definition_cast_exact_original`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_original_definition_cast_compare_nonnull`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_original_definition_cast_qualified_this`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_original_definition_cast_original_cast_annotation`  DIFFER  2649/2652  174 differing instruction groups.
- `expr_selected_original_template_parameters`  DIFFER  2649/2652  174 differing instruction groups.
- `partial_tree_5bf00538c146_21e5685f`  DIFFER  2649/2652  174 differing instruction groups.
