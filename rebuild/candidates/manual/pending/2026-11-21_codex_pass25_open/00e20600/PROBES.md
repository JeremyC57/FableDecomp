# Pass25 quest-script probes

Ten pass builds q01..q10, then required corrective verification only. q01 migrates definitions; q10 restores local spellings and cleans metadata. These two are administrative builds, not distinct algorithm shapes. No match is claimed.

| Tag | Bytes (built/retail) | Strict hunks | Purpose |
|---|---:|---:|---|
| q01_migrate | 14526/14510 | 43 | Byte-neutral move of row-local member bodies into opt-in shared header. |
| q02_depth1 | 14494/14510 | 170 | depth1 |
| q03_depth2 | 14494/14510 | 183 | depth2 |
| q04_baseforce1 | 14494/14510 | 170 | baseforce1 |
| q05_statement0 | 14414/14510 | 69 | statement0 |
| q06_statement1 | 14494/14510 | 183 | statement1 |
| q07_auto_dtor | 14462/14510 | 182 | auto dtor |
| q08_force_alive | 14526/14510 | 43 | force alive |
| q09_force_and | 14526/14510 | 43 | force and |
| q10_final | 14526/14510 | 43 | Final row-specific shared-header names and Ego_d local spelling cleanup. |
| q11_identity_final | 14526/14510 | 43 | Required identity fix: existing OUTOFLINE_GETSTRING opt-in, after audit found inherited old conversion binding. |

Historical overlap: depth-one/two predicate definitions, forced-inline base cleanup and depth-one SetPredicate had already been explored in handback_NOTES; their repetition here reconfirms that header migration does not fix the base/member split. The useful deliverables are shared-header compliance, exact binding correction, tests and an honest parked source, not a new inline mechanism.
- donor_failure_result — Use original TRetVoid local and original Return() calls at termination exits; exact donor method and Ego_d local. DIFFER 14526/14510 bytes, 43 instruction hunks.
- donor_binding_scopes — Restore single rewards and both function-scope distance locals from original symbol records in addition to actual termination results. DIFFER 14526/14510 bytes, 43 instruction hunks.
- donor_and_forward — Restore the evidenced operator&& to GFPredicateAnd to CPredicate_And forwarding chain. DIFFER 14526/14510 bytes, 43 instruction hunks.
- donor_virtual_context — Include genuine virtual operator()/Clone for both predicates, with Clone using the evidenced constructor from members rather than a copy. DIFFER 14526/14510 bytes, 43 instruction hunks.
- donor_natural_cleanup — Keep both genuine vtable-visible bodies and remove inherited nested destructor controls. DIFFER 14494/14510 bytes, 183 instruction hunks.
- declaration_only_thing_dtor — Leave the genuine ScriptThing destructor external throughout, testing natural inlining of both predicate classes without pruning the base. DIFFER 14494/14510 bytes, 65 instruction hunks.
- unseen_thing_definition — Keep the ScriptThing destructor body visible while prohibiting expansion; compare the four predicate cleanup sites. DIFFER 14494/14510 bytes, 65 instruction hunks.
- prune_pair_only — Prevent nested expansion only in the composite predicate, leave leaf predicate destructor natural. DIFFER 14494/14510 bytes, 170 instruction hunks.
- prune_alive_only — Prevent nested expansion only in CIsThingAlive while the composite uses the natural destructor. DIFFER 14558/14510 bytes, 86 instruction hunks.
- unseen_leaf_prune_pair — Keep ScriptThing external and prune the composite destructor only. DIFFER 14478/14510 bytes, 57 instruction hunks.
- donor_bonus_index — Restore original signed bonus idx and leave the two observed reward arms intact. DIFFER 14526/14510 bytes, 43 instruction hunks.
- donor_bonus_join — Share the single container insertion after selecting the reward; FableWin and retail both show one virtual slot 585 after the branches. DIFFER 14510/14510 bytes, 47 instruction hunks.
- donor_for_scope — Original donor +760 XOR/JE/JMP wraps the entire increment loop; restore native-era loop scope and test donor_for_scope. DIFFER 14510/14510 bytes, 47 instruction hunks.
- donor_for_scope_and_wait — Original donor +760 XOR/JE/JMP wraps the entire increment loop; restore native-era loop scope and test donor_for_scope_and_wait. DIFFER 14510/14510 bytes, 47 instruction hunks.
- donor_named_def_and_getter — Correct the emitted global type to CDefPointer<const CScriptDef> and the master-data call to the original CTraderToEscort::GetMasterData. DIFFER 14510/14510 bytes, 47 instruction hunks.
- donor_checked_bonus — Restore both conditions proven by original assert strings at lines529 and537, once each at their real sites. DIFFER 14510/14510 bytes, 47 instruction hunks.
- selected_donor — Select source after verified debug donor, guarded portable compiler controls, shared headers and clean opt-ins. DIFFER 14510/14510 bytes, 47 instruction hunks.
- tu_legacy_for — Apply the debug-donor-confirmed legacy loop-scope workaround across the translation unit including all library headers. DIFFER 14510/14510 bytes, 47 instruction hunks.
- tu_legacy_for_natural — Evaluate the original loop workaround without the inherited nested destructor controls. DIFFER 14478/14510 bytes, 168 instruction hunks.
- force_base_at_zero — Determine whether the original base destructor may inline through nested cleanup suppression: force_base_at_zero. DIFFER 14510/14510 bytes, 47 instruction hunks.
- force_base_thing_external — Determine whether the original base destructor may inline through nested cleanup suppression: force_base_thing_external. DIFFER 14462/14510 bytes, 61 instruction hunks.
- force_base_cold_pair — Determine whether the original base destructor may inline through nested cleanup suppression: force_base_cold_pair. DIFFER 14462/14510 bytes, 61 instruction hunks.
- force_base_and_external — Determine whether the original base destructor may inline through nested cleanup suppression: force_base_and_external. DIFFER 14462/14510 bytes, 61 instruction hunks.
- force_base_natural_external — Determine whether the original base destructor may inline through nested cleanup suppression: force_base_natural_external. DIFFER 14462/14510 bytes, 61 instruction hunks.
- implicit_alive — Compare compiler-generated virtual destructors in the actual original classes against user-written empty destructors, preserving original layout and destructor calls. DIFFER 14638/14510 bytes, 89 instruction hunks.
- implicit_composite — Compare compiler-generated virtual destructors in the actual original classes against user-written empty destructors, preserving original layout and destructor calls. DIFFER 14462/14510 bytes, 168 instruction hunks.
- implicit_predicates — Compare compiler-generated virtual destructors in the actual original classes against user-written empty destructors, preserving original layout and destructor calls. DIFFER 14622/14510 bytes, 73 instruction hunks.
- implicit_composite_external_thing — Compare compiler-generated virtual destructors in the actual original classes against user-written empty destructors, preserving original layout and destructor calls. DIFFER 14478/14510 bytes, 85 instruction hunks.
- implicit_both_external_thing — Compare compiler-generated virtual destructors in the actual original classes against user-written empty destructors, preserving original layout and destructor calls. DIFFER 14446/14510 bytes, 128 instruction hunks.
- original_base — Use the shared PDB-complete class declarations for the base family and retain the selected destructor split. DIFFER 14510/14510 bytes, 47 instruction hunks.
- original_base_plain — Use the same original declarations without the nested destructor controls; judge hot/cold splits. DIFFER 14478/14510 bytes, 168 instruction hunks.
- original_lifetime — Use the shared PDB-complete class declarations for the lifetime family and retain the selected destructor split. DIFFER 14510/14510 bytes, 47 instruction hunks.
- original_lifetime_plain — Use the same original declarations without the nested destructor controls; judge hot/cold splits. DIFFER 14478/14510 bytes, 168 instruction hunks.
- original_script — Use the shared PDB-complete class declarations for the script family and retain the selected destructor split. COMPILE_FAIL ?/? bytes, ? instruction hunks.
- original_script_plain — Use the same original declarations without the nested destructor controls; judge hot/cold splits. COMPILE_FAIL ?/? bytes, ? instruction hunks.
- original_complete — Use the shared PDB-complete class declarations for the complete family and retain the selected destructor split. COMPILE_FAIL ?/? bytes, ? instruction hunks.
- original_complete_plain — Use the same original declarations without the nested destructor controls; judge hot/cold splits. COMPILE_FAIL ?/? bytes, ? instruction hunks.
- original_script_game_reference — Use the exact PDB-proven protected reference for CEntityScriptBase::Game at +4 and access the interface as an lvalue, including pause objects. DIFFER 14510/14510 bytes, 47 instruction hunks.
- original_complete_game_reference — Use the exact PDB-proven protected reference for CEntityScriptBase::Game at +4 and access the interface as an lvalue, including pause objects. DIFFER 14510/14510 bytes, 47 instruction hunks.
- original_complete_plain_game_reference — Use the exact PDB-proven protected reference for CEntityScriptBase::Game at +4 and access the interface as an lvalue, including pause objects. DIFFER 14478/14510 bytes, 168 instruction hunks.
- nested_external_at_1 — Prune only the consuming statement to depth 1 with original ScriptThing destructor external, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- nested_external_at_2 — Prune only the consuming statement to depth 2 with original ScriptThing destructor external, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- nested_external_at_3 — Prune only the consuming statement to depth 3 with original ScriptThing destructor external, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- nested_visible_at_1 — Prune only the consuming statement to depth 1 with original ScriptThing destructor visible, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- nested_visible_at_2 — Prune only the consuming statement to depth 2 with original ScriptThing destructor visible, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- nested_visible_at_3 — Prune only the consuming statement to depth 3 with original ScriptThing destructor visible, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- nested_explicit_only_at_1 — Prune only the consuming statement to depth 1 with original ScriptThing destructor explicit_only, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- nested_explicit_only_at_2 — Prune only the consuming statement to depth 2 with original ScriptThing destructor explicit_only, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- nested_explicit_only_at_3 — Prune only the consuming statement to depth 3 with original ScriptThing destructor explicit_only, compare the exact base/member/leaf calls. DIFFER 14478/14510 bytes, 68 instruction hunks.
- selected_original_types — Select the original protected Game reference and original complete class headers with verified local scopes, single bonus insertion, and the correct def-handle symbol. DIFFER 14510/14510 bytes, 47 instruction hunks.
- predicate_native_member_T25 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 25. DIFFER 14110/14510 bytes, 233 instruction hunks.
- predicate_native_member_T50 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 50. DIFFER 14414/14510 bytes, 65 instruction hunks.
- predicate_native_member_T75 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 75. DIFFER 14478/14510 bytes, 168 instruction hunks.
- predicate_native_member_T100 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 100. DIFFER 14654/14510 bytes, 104 instruction hunks.
- predicate_native_member_T150 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 150. DIFFER 14878/14510 bytes, 116 instruction hunks.
- predicate_external_member_T25 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 25. DIFFER 14126/14510 bytes, 116 instruction hunks.
- predicate_external_member_T50 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 50. DIFFER 14334/14510 bytes, 62 instruction hunks.
- predicate_external_member_T75 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 75. DIFFER 14478/14510 bytes, 68 instruction hunks.
- predicate_external_member_T100 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 100. DIFFER 14542/14510 bytes, 86 instruction hunks.
- predicate_external_member_T150 — Classify the two nested versus two outer predicate destructor decisions with genuine definitions, low-size shortcut disabled and score 150. DIFFER 14686/14510 bytes, 102 instruction hunks.
- class_definition_pair_inclass — Provide the original predicate definitions at their actual class declaration; distinguish whether definition order changes nested cleanup decisions (pair_inclass). DIFFER 14478/14510 bytes, 68 instruction hunks.
- class_definition_leaf_inclass — Provide the original predicate definitions at their actual class declaration; distinguish whether definition order changes nested cleanup decisions (leaf_inclass). DIFFER 14478/14510 bytes, 68 instruction hunks.
- class_definition_both_inclass — Provide the original predicate definitions at their actual class declaration; distinguish whether definition order changes nested cleanup decisions (both_inclass). DIFFER 14478/14510 bytes, 68 instruction hunks.
- class_definition_all_inclass_ctors — Provide the original predicate definitions at their actual class declaration; distinguish whether definition order changes nested cleanup decisions (all_inclass_ctors). DIFFER 14510/14510 bytes, 70 instruction hunks.
- class_definition_ctors_out_of_line — Provide the original predicate definitions at their actual class declaration; distinguish whether definition order changes nested cleanup decisions (ctors_out_of_line). DIFFER 14478/14510 bytes, 168 instruction hunks.
- class_definition_inclass_with_outer_ctl — Provide the original predicate definitions at their actual class declaration; distinguish whether definition order changes nested cleanup decisions (inclass_with_outer_ctl). DIFFER 14542/14510 bytes, 68 instruction hunks.
- budget_original_control — Original types in the documented budget path as an independent prefix control. DIFFER 15115/14510 bytes, 372 instruction hunks.
- budget_movie_called — Declare the Movie destructor out of line: every genuine use calls its COMDAT in retail. DIFFER 14667/14510 bytes, 388 instruction hunks.
- budget_thing_called — Declare ScriptThing cleanup out of line throughout the real TU; retain all original predicate declarations. DIFFER 14602/14510 bytes, 335 instruction hunks.
- budget_movie_thing_called — Model the two cleanup families that retail calls throughout the function as actual out-of-line members. DIFFER 14342/14510 bytes, 290 instruction hunks.
- budget_movie_visible_unexpanded — Expose the ScriptThing cleanup semantics for alias analysis but keep every call external. DIFFER 14342/14510 bytes, 290 instruction hunks.
- expdec_external_free_0 — Set the leaf destructor nested depth to free and the composite nested depth to 0 with external ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14462/14510 bytes, 61 instruction hunks.
- expdec_external_0_free — Set the leaf destructor nested depth to 0 and the composite nested depth to free with external ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14494/14510 bytes, 85 instruction hunks.
- expdec_external_1_0 — Set the leaf destructor nested depth to 1 and the composite nested depth to 0 with external ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14462/14510 bytes, 61 instruction hunks.
- expdec_external_0_1 — Set the leaf destructor nested depth to 0 and the composite nested depth to 1 with external ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14462/14510 bytes, 61 instruction hunks.
- expdec_external_free_1 — Set the leaf destructor nested depth to free and the composite nested depth to 1 with external ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14462/14510 bytes, 61 instruction hunks.
- expdec_external_1_free — Set the leaf destructor nested depth to 1 and the composite nested depth to free with external ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14494/14510 bytes, 85 instruction hunks.
- expdec_external_1_2 — Set the leaf destructor nested depth to 1 and the composite nested depth to 2 with external ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14494/14510 bytes, 85 instruction hunks.
- expdec_external_2_1 — Set the leaf destructor nested depth to 2 and the composite nested depth to 1 with external ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14462/14510 bytes, 61 instruction hunks.
- expdec_visible_free_0 — Set the leaf destructor nested depth to free and the composite nested depth to 0 with visible ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14478/14510 bytes, 168 instruction hunks.
- expdec_visible_0_free — Set the leaf destructor nested depth to 0 and the composite nested depth to free with visible ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14542/14510 bytes, 68 instruction hunks.
- expdec_visible_1_0 — Set the leaf destructor nested depth to 1 and the composite nested depth to 0 with visible ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14478/14510 bytes, 168 instruction hunks.
- expdec_visible_0_1 — Set the leaf destructor nested depth to 0 and the composite nested depth to 1 with visible ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14510/14510 bytes, 47 instruction hunks.
- expdec_visible_free_1 — Set the leaf destructor nested depth to free and the composite nested depth to 1 with visible ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14478/14510 bytes, 168 instruction hunks.
- expdec_visible_1_free — Set the leaf destructor nested depth to 1 and the composite nested depth to free with visible ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14478/14510 bytes, 168 instruction hunks.
- expdec_visible_1_2 — Set the leaf destructor nested depth to 1 and the composite nested depth to 2 with visible ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14478/14510 bytes, 168 instruction hunks.
- expdec_visible_2_1 — Set the leaf destructor nested depth to 2 and the composite nested depth to 1 with visible ScriptThing cleanup; audit direct member and base call sequence. DIFFER 14478/14510 bytes, 168 instruction hunks.
- two_proven_bonus_calls — Donor +0411 and +05f1 are separate slot-585 calls inside both bonus arms, before their join at +05f3. Keep one insertion in each arm.. DIFFER 14526/14510 bytes, 43 instruction hunks.
- both_script_diagnostics_removed — Reinstate the donor-identified checks in the true source arms; test the removed release expansion while retaining one native inventory insertion. DIFFER 14526/14510 bytes, 43 instruction hunks.
- both_script_diagnostics_error — Reinstate the donor-identified checks in the true source arms; test the error release expansion while retaining one native inventory insertion. DIFFER 14526/14510 bytes, 43 instruction hunks.
- both_script_diagnostics_folded — Reinstate the donor-identified checks in the true source arms; test the folded release expansion while retaining one native inventory insertion. DIFFER 14526/14510 bytes, 43 instruction hunks.
- both_script_diagnostics_shared — Reinstate the donor-identified checks in the true source arms; test the shared release expansion while retaining one native inventory insertion. DIFFER 14526/14510 bytes, 43 instruction hunks.
- donor_bonus_final — Re-select both debug-proven insertion calls. The cpp contains the two authentic arm shapes and the two elided diagnostics are documented.. DIFFER 14526/14510 bytes, 43 instruction hunks.
- member_capture_members_pair — Check whether VC7.1 records the depth of the member declaration for implicit destruction inside the true pair class; members. DIFFER 14494/14510 bytes, 65 instruction hunks.
- member_capture_members_alive — Check whether VC7.1 records the depth of the member declaration for implicit destruction inside the true alive class; members. DIFFER 14494/14510 bytes, 65 instruction hunks.
- member_capture_members_both — Check whether VC7.1 records the depth of the member declaration for implicit destruction inside the true both class; members. DIFFER 14494/14510 bytes, 65 instruction hunks.
- member_capture_declaration_pair — Check whether VC7.1 records the depth of the member declaration for implicit destruction inside the true pair class; declaration. DIFFER 14494/14510 bytes, 65 instruction hunks.
- member_capture_declaration_alive — Check whether VC7.1 records the depth of the member declaration for implicit destruction inside the true alive class; declaration. DIFFER 14494/14510 bytes, 65 instruction hunks.
- member_capture_declaration_both — Check whether VC7.1 records the depth of the member declaration for implicit destruction inside the true both class; declaration. DIFFER 14494/14510 bytes, 65 instruction hunks.
- donor_final_wait — Donor script lines 1188 and 1190 retain TT followed by WAIT_FOREVER and the final TT after the outer for loop, even though its release path is unreachable. DIFFER 14526/14510 bytes, 43 instruction hunks.
- donor_wait_until_answer — Original debug +2a70 shows one poll in the wait condition; it stores YNQAnswer, tests >=0, and advances a frame only when unanswered.. DIFFER 14526/14510 bytes, 43 instruction hunks.
- retail_script_conditions — Retain release behavior while documenting all four actual debug Error sites and the final frame wait; no invented debug operations in the retail execution path. DIFFER 14526/14510 bytes, 43 instruction hunks.
- actual_smart_pointer_getters — The two debug CScriptThing getters call PImp.IsNull() and its arrow operator before the game virtual; use those real members on the original smart pointer.. DIFFER 14526/14510 bytes, 43 instruction hunks.
- original_predicate_visibility — Declare both predicate components private as recorded by the original class type while preserving the existing default header path.. DIFFER 14526/14510 bytes, 43 instruction hunks.
- retail_codeview_Z7 — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Z7. DIFFER 14526/14510, 43 hunks.
- retail_codeview_Zi — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Zi. DIFFER 14526/14510, 43 hunks.
- retail_codeview_Zd — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Zd. DIFFER 14526/14510, 43 hunks.
- original_hero_spoken_top — Debug decision audit found the original tests hero_spoken before the first NewScriptFrame at donor +0x1468, while the previous do loop tested it at the bottom. Restore the actual top-tested loop. DIFFER 14526/14510 bytes, 43 instruction hunks.
- portable_crt_and_allocator — Use the portable CRT header for rand and the shared calling convention spelling in the restored trader declaration. DIFFER 14526/14510 bytes, 43 instruction hunks.
- review_spell_setter_visibility_1, _2, _3 — Rebuilt the three original SetPredicate forwarding bodies independently (20, 8 and 25 bytes), then made them visible progressively to the caller. Each helper matched its original body. All three compilations of the selected top-tested Main remained 14,526 bytes, 43 instruction hunks.

## Subsequent retry

The selected source now uses CTimer::operator==(long seconds) const at all seven original comparison sites. The checked donor invokes that exact operation seven times, its function body calls GetTime, and the Ego_d local record gives the signed-long formal seconds. Plain inline still calls the operator five times, and the ordinary per-function budget produced a worse leaf. The selected opt-in expands just the original comparison helper. Its full method and the helper body appear in the test, which adds zero, negative, positive, pre-expiry, exact-expiry and past-expiry checks.

The optimized selected leaf still has 43 disassembly groups and 22 groups after dropping padding and trivial jumps. All 408 paired local branches have the same reduced destinations. The 664 generated relocation operands have been reviewed. The two CPredicateBase destructor calls have no corresponding retail call; the remaining 662 operand bindings are supported by the native targets and the symbol or literal checks. Four genuine same-TU helper implementations separately reached their complete native sizes and masked bytes (60, 79, 347 and 112). That visibility, and the earlier 157-byte COF_MainGates member, did not change Main.

The additional exact-length source under alternative_exact_size/ removes the opaque Movie counted-pointer destructor declaration. This recovers the genuine 66-byte helper and generates 14,510 bytes in Main, but the different movie cleanup increases the reduced comparison from 22 to 24 groups. Retail expands Movie destruction at that site and calls the nested counted-pointer destructor. The other source preserves that operation. Both retain the two predicate-base calls and both have 408/408 paired reduced destinations. The test covers the same method body. The selected TU's emitted CDefString conversion and Movie destructor differ from their separately owned implementations. See RESOURCE_HELPERS.json and each version's RETRY_OPERANDS.tsv.

Individual compiler results are listed in RETRY_GRADES.tsv.
