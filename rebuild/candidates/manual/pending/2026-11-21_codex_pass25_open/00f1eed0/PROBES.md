# Pass25 quest-script probes

Ten pass builds q01..q10, then required corrective verification only. q01 migrates definitions; q10 restores local spellings and cleans metadata. These two are administrative builds, not distinct algorithm shapes. No match is claimed.

| Tag | Bytes (built/retail) | Strict hunks | Purpose |
|---|---:|---:|---|
| q01_migrate | 11221/9262 | 205 | Byte-neutral move of row-local member bodies into opt-in shared header. |
| q02_cold0 | 11045/9262 | 177 | cold0 |
| q03_spawn0 | 11093/9262 | 185 | spawn0 |
| q04_tail0 | 11173/9262 | 195 | tail0 |
| q05_cold1 | 11221/9262 | 205 | cold1 |
| q06_return_only0 | 11045/9262 | 177 | return only0 |
| q07_cold0_T55 | 9156/9262 | 100 | cold0 T55 |
| q08_force_timer | 11171/9262 | 169 | force timer |
| q09_force_timer_T55 | 9128/9262 | 114 | force timer T55 |
| q10_final | 9156/9262 | 100 | Final row-specific shared-header names and Ego_d local spelling cleanup. |
| q11_identity_final | 9156/9262 | 100 | Required identity fix: existing OUTOFLINE_GETSTRING opt-in, after audit found inherited old conversion binding. |
| q12_current_headers | 9156/9262 | 100 | Required header portability recheck after dropping 62 stale inherited overlays; no code change. |

The ten depth-zero return sites are source ordinals 15,17,18,25,26,33,56,58,59,60 after expanding AR_COUNT and AR_START (zero based). Every region surrounds one if-return statement. This retains the natural top-tested while and plain for loops; no old >=1 guard, ||0 bottom or dead assertions are present.
- donor_failure_result — Use original TRetVoid local and original Return() calls at termination exits; exact donor method and Ego_d local. DIFFER 9076/9262 bytes, 100 instruction hunks.
- qp01_for_scope_default — Donor-confirmed constant-false for-scope wrapper; no fitted return cleanup regions; default. DIFFER 9374/9262 bytes, 98 instruction hunks.
- qp01_for_scope_T55 — Donor-confirmed constant-false for-scope wrapper; no fitted return cleanup regions; T55. DIFFER 8472/9262 bytes, 102 instruction hunks.
- qp02_plain_constructors — Donor for-scope guard with plain_constructors. DIFFER 9414/9262 bytes, 153 instruction hunks.
- qp02_for_empty_statement — Donor for-scope guard with for_empty_statement. DIFFER 9374/9262 bytes, 98 instruction hunks.
- qp03_extra_truth — Donor-based scope/frequency context: extra_truth. DIFFER 9374/9262 bytes, 98 instruction hunks.
- qp03_return_early — Donor-based scope/frequency context: return_early. DIFFER 9374/9262 bytes, 98 instruction hunks.
- qp03_for_headers — Donor-based scope/frequency context: for_headers. DIFFER 9262/9262 bytes, 5 instruction hunks.
- qp04_donor_both — Donor line1496 division and1498 receiver/argument order: both. DIFFER 9262/9262 bytes, 6 instruction hunks.
- qp05_integer_scope_guard — Classic integer-zero for-scope workaround, portability guard, and donor-confirmed spawn-position operand order. DIFFER 9262/9262 bytes, 5 instruction hunks.
- qp06_checkpoint — Portable macro/header checkpoint; default flags; five-hunk native residue retained. DIFFER 9262/9262 bytes, 5 instruction hunks.
- qp07_source_lines — Donor late wait-loop reconstruction: source_lines. DIFFER 9262/9262 bytes, 5 instruction hunks.
- qp07_bool_comparisons — Donor late wait-loop reconstruction: bool_comparisons. DIFFER 9262/9262 bytes, 5 instruction hunks.
- qp08_gift_exit — One scoped return enforces the retail gift-branch target to the called-timer cleanup. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp09_single_countdown_conversation — Restore donor’s one countdown-scope Conversation, shared by four assignments, instead of four macro locals. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp10_all_post — Donor-compatible discarded postincrement in all_post induction clauses; no new named local. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp11_game_reference — Restore original CLevelScriptBase Game and MasterScriptData reference members and source member access. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp12_colour3_plain — Donor-confirmed three-argument CRGBColour construction: plain. DIFFER 9262/9262 bytes, 6 instruction hunks.
- qp12_colour3_force — Donor-confirmed three-argument CRGBColour construction: force. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp13_original_getter_chain — Restore donor IsNull/operator-arrow chains in GetDataString and GetPos instead of flattened Data access. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp14_controller_exit — Independent raw-operand audit: target the controller-wait cancellation to the retail called-timer cleanup. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp15_reference_checkpoint — Faithful typed/header checkpoint; only one audited SIB byte remains, no inlining threshold flag. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp16_conversion_forwarder — Prevent a conflicting seventeen-byte conversion COMDAT by forcing the original GetString forwarder. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp17_visible_getstring — Keep the original GetString body visible while forcing only its conversion forwarder, preserving owned helper bodies. DIFFER 9262/9262 bytes, 7 instruction hunks.
- qp18_explicit_getstring_bracket — Diagnostic older-source bracket: call the retail GetString explicitly, avoiding a conflicting conversion helper copy; donor source review required. DIFFER 9252/9262 bytes, 72 instruction hunks.
- main_arena_crt_stdlib — Use the CRT header or guarded ABI declaration for the actual rand call to avoid the raw VC keyword and measure compiler symbol order. DIFFER 9262/9262, 1 strict hunks.
- main_arena_crt_cstdlib — Use the CRT header or guarded ABI declaration for the actual rand call to avoid the raw VC keyword and measure compiler symbol order. DIFFER 9262/9262, 1 strict hunks.
- main_arena_portable_abi — Use the CRT header or guarded ABI declaration for the actual rand call to avoid the raw VC keyword and measure compiler symbol order. DIFFER 9262/9262, 1 strict hunks.
- qp19_stdlib_checkpoint — Adopt standard CRT rand declaration, independently confirmed by root as byte/symbol neutral. DIFFER 9262/9262 bytes, 1 instruction hunks.
- main_arena_donor_loop_break — Source-revision bracket: restore the donor’s named i and loop condition; a single unconditional break models the single native gift. DIFFER 9262/9262, 1 strict hunks.
- main_arena_explicit_first_gift — Source-revision bracket: represent the observed one-gift limit in the bounds of the original donor loop. DIFFER 9334/9262, 146 strict hunks.
- main_arena_single_loop_with_guard — Source-revision bracket: preserve the native positive guard and the donor’s for scope in a single-iteration loop. DIFFER 9294/9262, 112 strict hunks.
- main_arena_invalid_handles_retail_assert — The debug smart-pointer GetPos/GetDataString null arms invoke the original debugger message. Restore the project release error-site spelling (RETAIL_ASSERT) and measure the effect on operand hash. DIFFER 9262/9262, 1 strict hunks.
- main_arena_invalid_handles_retail_arm — The debug smart-pointer GetPos/GetDataString null arms invoke the original debugger message. Restore the project release error-site spelling (RETAIL_ARM) and measure the effect on operand hash. DIFFER 9262/9262, 1 strict hunks.
- main_arena_invalid_handles_disabled_diagnostics — The debug smart-pointer GetPos/GetDataString null arms invoke the original debugger message. Restore the project release error-site spelling (disabled_diagnostics) and measure the effect on operand hash. DIFFER 9262/9262, 1 strict hunks.
- main_arena_proven_script_access — Use the PDB-proven shared-header declarations and access for script_access; check symbol order and emitted leaf. DIFFER 9262/9262, 1 strict hunks.
- main_arena_proven_counted — Use the PDB-proven shared-header declarations and access for counted; check symbol order and emitted leaf. DIFFER 9262/9262, 1 strict hunks.
- main_arena_proven_definitions — Use the PDB-proven shared-header declarations and access for definitions; check symbol order and emitted leaf. DIFFER 9278/9262, 5 strict hunks.
- main_arena_proven_vector_ctor — Use the PDB-proven shared-header declarations and access for vector_ctor; check symbol order and emitted leaf. DIFFER 9262/9262, 1 strict hunks.
- main_arena_proven_original_colour — Use the PDB-proven shared-header declarations and access for original_colour; check symbol order and emitted leaf. DIFFER 9262/9262, 1 strict hunks.
- main_arena_proven_script_all — Use the PDB-proven shared-header declarations and access for script_all; check symbol order and emitted leaf. DIFFER 9278/9262, 5 strict hunks.
- main_arena_proven_all — Use the PDB-proven shared-header declarations and access for all; check symbol order and emitted leaf. DIFFER 9278/9262, 5 strict hunks.
- retail_codeview_Z7 — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Z7. DIFFER 9262/9262, 1 hunks.
- retail_codeview_Zi — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Zi. DIFFER 9262/9262, 1 hunks.
- retail_codeview_Zd — Retail contains an RSDS reference to Fable.pdb. Check whether VC7.1 debug-symbol emission influences the remaining code decisions while optimizing: /Zd. DIFFER 9262/9262, 1 hunks.
- qp20_original_index_typedef — Restore STLport4.6.2 vector reference/const_reference operator[] typedef spelling. DIFFER 9262/9262 bytes, 1 instruction hunks.
- qp21_plain_visible_string — Restore original plain-inline conversion and GetString bodies together to resolve conflicting emitted conversion ownership. DIFFER 9262/9262 bytes, 7 instruction hunks.
- qp22_visible_string_depth1 — Retain real string helper bodies and scope gift expression inline depth to expand its conversion while calling nested GetString. DIFFER 9262/9262 bytes, 7 instruction hunks.
- qp23_visible_string_depth0 — Bracket default visible string helpers with zero gift-expression inline depth to assess the conversion call boundary. DIFFER 9278/9262 bytes, 10 instruction hunks.
- qp24_donor_direct_vector_index — Diagnostic donor-confirmed direct vector start-plus-index operator[] body; Dinkumware donor has no nested begin call, retail ABI remains STLport. DIFFER 9262/9262 bytes, 128 instruction hunks.
- joint_original_next_member — Compile beside the unchanged supplied GetEndRoundQuestion body in its original same-object order and check only the reserved PlayWave leaf. DIFFER 9262/9262 bytes, 1 instruction hunks.
- joint_original_shared_headers — Use the PDB script-object common header declarations needed by both reserved arena and gate methods. DIFFER 9262/9262 bytes, 1 instruction hunks.
- joint_two_reserved_methods — Compile the complete reserved PlayWave and GuardOuter Main bodies in their original debug object order; compare PlayWave with the isolated row on these same headers. DIFFER 9262/9262 bytes, 1 instruction hunks.
- joint_reserved_and_neighbour — Compile both real reserved functions and the unchanged supplied intermediate arena member in recorded order. DIFFER 9262/9262 bytes, 1 instruction hunks.
- original_first_round_negation — Use the original materialized first-round truth: both nonzero tests at donor +129/+138 jump to the same false materialization, then +157 tests the resulting bool. DIFFER 9262/9262 bytes, 1 instruction hunks.
- original_first_round_equal_false — Use an explicit comparison of the original combined first-round predicate with false. DIFFER 9525/9262 bytes, 117 instruction hunks.
- first_round_integer_zero — Restore the ephemeral first-round test value shown by donor +13A/+146 before +157 using first round integer zero. DIFFER 9525/9262 bytes, 117 instruction hunks.
- first_round_int_result — Restore the ephemeral first-round test value shown by donor +13A/+146 before +157 using first round int result. DIFFER 9525/9262 bytes, 117 instruction hunks.
- first_round_bool_result — Restore the ephemeral first-round test value shown by donor +13A/+146 before +157 using first round bool result. DIFFER 9525/9262 bytes, 117 instruction hunks.
- first_round_explicit_bool — Restore the ephemeral first-round test value shown by donor +13A/+146 before +157 using first round explicit bool. DIFFER 9525/9262 bytes, 117 instruction hunks.
- countdown_boolean_T72 — Reconcile the original materialized boolean with retail vector cleanup boundaries using the compiler inline score 72. DIFFER 9262/9262 bytes, 9 instruction hunks.
- countdown_boolean_T68 — Reconcile the original materialized boolean with retail vector cleanup boundaries using the compiler inline score 68. DIFFER 9070/9262 bytes, 81 instruction hunks.
- countdown_boolean_T64 — Reconcile the original materialized boolean with retail vector cleanup boundaries using the compiler inline score 64. DIFFER 8849/9262 bytes, 98 instruction hunks.
- countdown_boolean_T60 — Reconcile the original materialized boolean with retail vector cleanup boundaries using the compiler inline score 60. DIFFER 8486/9262 bytes, 110 instruction hunks.
- countdown_boolean_T56 — Reconcile the original materialized boolean with retail vector cleanup boundaries using the compiler inline score 56. DIFFER 8486/9262 bytes, 110 instruction hunks.
- countdown_boolean_T76 — Reconcile the original materialized boolean with retail vector cleanup boundaries using the compiler inline score 76. DIFFER 9653/9262 bytes, 145 instruction hunks.
- countdown_boolean_T80 — Reconcile the original materialized boolean with retail vector cleanup boundaries using the compiler inline score 80. DIFFER 9989/9262 bytes, 145 instruction hunks.
- countdown_nested_negated — Separate the two countdown gates while preserving the original source conditions, jumps and empty-scope lifetime: nested_negated. DIFFER 9262/9262 bytes, 1 instruction hunks.
- countdown_nested_materialized — Separate the two countdown gates while preserving the original source conditions, jumps and empty-scope lifetime: nested_materialized. DIFFER 9525/9262 bytes, 117 instruction hunks.
- original_x86_G5 — Check whether the remaining LEA operand choice follows the native compiler instruction-scheduling target /G5 with the genuine body and original 32-bit x86 ABI. DIFFER 9262/9262 bytes, 1 instruction hunks.
- original_x86_G6 — Check whether the remaining LEA operand choice follows the native compiler instruction-scheduling target /G6 with the genuine body and original 32-bit x86 ABI. DIFFER 9262/9262 bytes, 1 instruction hunks.
- original_x86_G7 — Check whether the remaining LEA operand choice follows the native compiler instruction-scheduling target /G7 with the genuine body and original 32-bit x86 ABI. DIFFER 9246/9262 bytes, 158 instruction hunks.
- original_x86_GB — Check whether the remaining LEA operand choice follows the native compiler instruction-scheduling target /GB with the genuine body and original 32-bit x86 ABI. DIFFER 9262/9262 bytes, 1 instruction hunks.
- full_stl_original_library — Compiled the selected production source directly with the preserved upstream STLport 4.6.2 headers and VC7.1. Upstream's bit-packed vector<bool> fails the supplied CFireHeartPatternDef layout assertion.
- full_stl_vector12 — Used the upstream library's ordinary vector<bool> instantiation to recover the engine's verified twelve-byte vector. Kept every project ABI assertion enabled. The real compiler emitted 9,262 bytes and 321 relocations. Every byte and every symbol/offset was identical to the selected project-header build, including the single SIB byte differing from retail. The project vector excerpt therefore reproduces the complete original-library trial for this leaf.

## Subsequent retry

The selected PlayWave now uses the original shared resource classes’ compiler-generated members. Removing the three ScriptedThingBase and two MovieBase out-of-line switches and the explicit Movie counted-pointer destructor declaration leaves the leaf and its 321 named operands unchanged. Seven additional separately bound helper bodies now agree with the retail function extents. Together they cover 308 bytes across helpers. Their raw relocation fields were resolved independently; see RESOURCE_HELPERS.json. These bytes lie outside the assigned leaf.

At `0x00F1FFBA` the index and base of one address remain reversed. Both operands have coefficient one. The solver receipt in INSTRUCTION_EQUIVALENCE.json covers every 32-bit input to the instruction. There is still one different non-relocated byte. All 573 compared direct non-STL engine call-name stems in the unoptimized source follow the donor order. This comparison excludes indirect calls, imports, compiler helpers and STL calls and does not settle which temporary the retail compiler ranked first.

Two independent precompiled header reconstructions retained exactly the same leaf and bindings. The immediate original translation-unit neighbours (AnimateCrowd before PlayWave, GivePrizeFund after it) individually, together, and combined with the two other supplied arena members left the same one-byte difference. The strings experiment produced accurate 25-byte copies of the CDefString getter and conversion together, but changed the reward expression from a call to GetString into a direct table lookup. Early and late helper definitions and scoped depths at the real reward statement did not repair that. The selected object's unused conversion still has a 17-byte body where its separately owned native implementation is 25 bytes. Other measured alternatives include genuine resource members in isolation, scoped locals from the donor and nine inlining thresholds. None produced the full retail leaf. Further evidence would need to distinguish the original release translation unit’s inline expansion order before the HUD access.

Individual compiler results are listed in RETRY_GRADES.tsv.
