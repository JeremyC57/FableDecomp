# Local continuation attempts

All attempts below preserve their source and header snapshot in HISTORY. Setup failures, baseline reproductions and comment-only regrades are explicitly included; this is not a count of independent source-shape strategies. Prior Claude sessions are separately preserved under PRIOR_HANDOFFS. Historical source comments and old summaries are not current status.

| Tag | Status | Compiled bytes | Outcome |
|---|---|---:|---|
| baseline | PREPROCESS_FAIL |  | Original baseline or initial tool-setup check; inspect status before counting as a compiled strategy. |
| baseline_02 | DIFFER | 35593 | Reproduces supplied Action session-4 baseline after preprocessing setup was fixed. |
| action_pointer_after_frontend | COMPILE_FAIL |  | Replacement mistake: compilation failed. Not a completed source strategy. |
| action_pointer_after_frontend_02 | DIFFER | 35607 | Raw action pointer increases allocator priority and worsens residue; rejected. |
| validity_outer_guard | DIFFER | 35593 | Outer validity guard: same selected bytes as baseline. |
| game_after_frontend | DIFFER | 35592 | Move game initialization: no useful correction of entry register roles. |
| game_before_validity | DIFFER | 35593 | Earlier game initialization: no useful correction of entry register roles. |
| action_dereference | DIFFER | 35593 | Reference spelling canonicalized; same selected bytes as baseline. |
| bulk_count_reference | DIFFER | 35595 | Reference local gives add eax,8 / push [eax], unlike native push [eax+8]. |
| profile_bool_reference | DIFFER | 35659 | Reference bool changes frame unfavorably; rejected. |
| profile_bool_value | DIFFER | 35623 | Named bool restores six local setter argument shapes; later extended to thirteen. |
| profile_all_bool_values | DIFFER | 35658 | Thirteen setter bool homes improved; already present in incoming v23, do not double-count. |
| bulk_whole_calls | DIFFER | 35589 | Whole-call branches give the wrong register push shape. |
| profile_bools_unsigned_guard | DIFFER | 35658 | Unsigned NumberAugmentations <=0 restores native unsigned branch. |
| bulk_count_object | DIFFER | 35591 | CActionParamSLONG pointer restores native push [eax+8] for BUY_BULK/SELL_BULK. |
| combined_local_fixes_01 | DIFFER | 35656 | Old-baseline combination, superseded by incoming v23 plus subsequent fixes. |
| unload_whole_arms | DIFFER | 35595 | Corrects one literal push but changes other slot allocation; not selected. |
| trade_receiver_local | DIFFER | 35495 | Caches receiver BEFORE FindChild: load timing is wrong. Do not promote this shorter build. |
| combined_trade_cache | DIFFER | 35558 | Old-baseline cache combination; too-early receiver loads, research only. |
| combined_trade_arms | DIFFER | 35560 | Old-baseline branch combination; superseded by properly placed caches. |
| incoming_v23_baseline | DIFFER | 35697 | Reproduces updated Claude session-5 source: 35697 bytes, frame 0x808. |
| v23_bulk_guard | DIFFER | 35695 | Combines proven count-object and unsigned-guard changes on v23. |
| v23_trade_branch_locals | DIFFER | 35699 | Caches receiver AFTER successful FindChild in all four trade-menu arms. |
| v23_clan_direct | DIFFER | 35683 | Removes a begin() call native keeps; rejected. |
| v23_clan_assign_iterator | DIFFER | 35707 | Iterator assignment worsens frame/slots; rejected. |
| v23_clan_drop_alias | DIFFER | 35705 | Removing clan alias does not resolve residue. |
| v23_photo_get | DIFFER | 35697 | Alternative getter spelling, selected bytes unchanged from v23. |
| v23_photo_dereference | DIFFER | 35697 | Alternative dereference spelling, selected bytes unchanged from v23. |
| v23_photo_named_format | DIFFER | 35700 | Named pixel format does not give native push sequence. |
| v23_reverse_direct | DIFFER | 35697 | Reverse iterator direct initialization canonicalized to v23 bytes. |
| v23_reverse_assignment | DIFFER | 35697 | Reverse iterator assignment canonicalized to v23 bytes. |
| v23_reverse_postfix | DIFFER | 35697 | Reverse iterator postfix form canonicalized to v23 bytes. |
| v23_counted_ctor_visible | DIFFER | 35694 | Real visible CCountedPointer constructor, kept out of line: frame 0x808 -> 0x800; restores texture slot and ebx-held constant 8. |
| v23_combined_visible_ctor | DIFFER | 35696 | Combines frame fix, properly timed trade receivers, count-object and unsigned guard. |
| current_checkpoint_01 | DIFFER | 35696 | Exact recompilation after source status-comment update; not a new independent strategy. |
| photo_caption_named_text | DIFFER | 35702 | Named CWideString restores missing six-byte lea before OnCaptionEntered; selected. |
| wide_eq_nonnull_first | DIFFER | 35702 | Header return-arm reversal flips local diamond but does not place native cold block after the case; not selected. |
| wide_eq_case_guard | DIFFER | 35702 | Early break guard produces identical code; not selected. |
| current_checkpoint_02 | DIFFER | 35702 | Final selected-source verification after comment updates; not a new independent strategy. |
