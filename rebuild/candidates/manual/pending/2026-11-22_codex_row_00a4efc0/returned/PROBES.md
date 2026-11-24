# Compiler probes

Each row below is one compiler invocation. The positional column uses the same object relocation offsets in both buffers. Differences in an instruction length can shift all following comparisons until alignment recovers.

| Address | Probe | Result | Object bytes | Positional differences | Mapped EQ / NEQ | Purpose |
|---|---|---|---:|---:|---|---|
| 00a4efc0 | b3 | DIFFER | 3491 | 896 | 19 / 3 | Rebuild the supplied best source and six-header overlay. Original comparison and three differing mapped vector helpers reproduced. |
| 00a50510 | landed | MASKED_EQUAL | 1748 | 0 | 2 / 2 | Rebuild each landlord source with its original macros as a control. |
| 00aa0fa0 | landed | MASKED_EQUAL | 4460 | 0 | 4 / 3 | Rebuild each landlord source with its original macros as a control. |
| 00bd2d90 | landed | MASKED_EQUAL | 721 | 0 | 0 / 0 | Rebuild each landlord source with its original macros as a control. |
| 00a50510 | ifbody | DIFFER | 1748 | 10 | 2 / 2 | Select the b3 allocator body in every sibling, including the sibling without the dead assertion macro. |
| 00aa0fa0 | ifbody | MASKED_EQUAL | 4460 | 0 | 4 / 3 | Select the b3 allocator body in every sibling, including the sibling without the dead assertion macro. |
| 00bd2d90 | ifbody | MASKED_EQUAL | 721 | 0 | 0 / 0 | Select the b3 allocator body in every sibling, including the sibling without the dead assertion macro. |
| 00a4efc0 | string_pair | DIFFER | 3491 | 896 | 19 / 3 | Use original FableWin explicit if/else exits for both CCharString getters, directly visible at 0186C770 and 0186D390. |
| 00a4efc0 | carray_forwarders | DIFFER | 3491 | 896 | 19 / 3 | Enable typed CArray push_back and special members, corroborated by the donor caller and destructor. Vector implementation differs by library revision. |
| 00a4efc0 | library_dispatch | DIFFER | 3491 | 896 | 19 / 3 | Restore the public copy -> __copy_aux -> __copy_ptrs chain from STLport 4.6.2 _algobase.h. |
| 00a4efc0 | donor_and_library | DIFFER | 3491 | 896 | 19 / 3 | Combine the three independently motivated string/array/library changes without altering the row body. |
| 00a4efc0 | common_alloc_donor | DIFFER | 3491 | 899 | 19 / 3 | Use the genuinely shared default allocator with the combined donor/library changes. The scaling residue also returns. |
| 00a4efc0 | resize_owners | DIFFER | 3491 | 896 | 21 / 1 | Use the original resize forwarding chain and the independently matched 00486189 and 004861DF size profiles. |
| 00a4efc0 | all_owners | DIFFER | 3491 | 896 | 21 / 1 | Give the byte fill-insert specialization its measured frame pointer and size profile, keeping generic copy dispatch. |
| 00a4efc0 | pod_owners | COMPILE_FAIL | — | — | — / — | First POD-owner integration lacked the original backward trivial-copy declaration. Compile failure, no grade. |
| 00a4efc0 | pod_owners_complete | DIFFER | 3491 | 896 | 23 / 0 | Restore the authenticated backward-copy entry point and the source of the independently landed byte fill owner. |
| 00a4efc0 | native_backward | DIFFER | 3491 | 896 | 22 / 1 | Measure the original backward-copy entry point without the POD owner’s direct primitive calls. |
| 00a4efc0 | c1 | DIFFER | 3491 | 896 | 19 / 3 | Rebuild the supplied Ego_d allocator-local candidate. Compare the same pool body in each sibling. |
| 00a50510 | c1 | DIFFER | 1748 | 34 | 2 / 2 | Rebuild the supplied Ego_d allocator-local candidate. Compare the same pool body in each sibling. |
| 00aa0fa0 | c1 | DIFFER | 4460 | 3 | 4 / 3 | Rebuild the supplied Ego_d allocator-local candidate. Compare the same pool body in each sibling. |
| 00bd2d90 | c1 | DIFFER | 721 | 23 | 0 / 0 | Rebuild the supplied Ego_d allocator-local candidate. Compare the same pool body in each sibling. |
| 00a4efc0 | same_tu_ifbody | DIFFER | 3491 | 896 | 22 / 6 | Compile the original animation-sequences sibling and the target together, using one if-shaped allocator and the shared /Gr TU. Sibling control only. |
| 00a50510 | same_tu_ifbody | DIFFER | 1757 | — | 22 / 6 | Compile the original animation-sequences sibling and the target together, using one if-shaped allocator and the shared /Gr TU. Sibling control only. |
| 00a4efc0 | same_tu_ternary | DIFFER | 3491 | 899 | 22 / 6 | Repeat the same-compiland test using the default ternary allocator in both functions. |
| 00a50510 | same_tu_ternary | DIFFER | 1757 | — | 22 / 6 | Repeat the same-compiland test using the default ternary allocator in both functions. |
| 00a4efc0 | owners_original_backward | DIFFER | 3411 | — | 25 / 0 | Restore both nontrivial random-access backward-copy instantiations and size-build the byte overflow body. |
| 00a4efc0 | owners_complete_backward | DIFFER | 3411 | — | 25 / 0 | Test the original out-of-line backward-copy choice on the same owner set. |
| 00a4efc0 | owner_byte_separate | DIFFER | 3491 | 896 | 25 / 0 | Keep the byte growth function visibly out of line as at the native call sites, retaining the two genuine backward-copy instances. |
| 00a4efc0 | byte_fill_inline | DIFFER | 3491 | 896 | 25 / 0 | Expand fill_n inside the byte-vector overflow specialization as at 00485B6C. The 132-byte owner now matches too. |
| 00a4efc0 | selected | DIFFER | 3491 | 896 | 25 / 0 | Move the independently tested owner bodies to one additive shared opt-in header and regrade. |
| 00a4efc0 | selected_common_alloc | DIFFER | 3491 | 899 | 25 / 0 | Run the shared default allocator against the helper-clean source; it reintroduces the scaling difference. |
| 00a50510 | selected_headers | MASKED_EQUAL | 1748 | 0 | 2 / 2 | Recompile each unchanged landed user on the selected include tree. |
| 00aa0fa0 | selected_headers | MASKED_EQUAL | 4460 | 0 | 4 / 3 | Recompile each unchanged landed user on the selected include tree. |
| 00bd2d90 | selected_headers | MASKED_EQUAL | 721 | 0 | 0 / 0 | Recompile each unchanged landed user on the selected include tree. |
| 00a4efc0 | verified_scope | DIFFER | 3491 | 896 | 26 / 0 | Use the authentic protected _M_allocate_and_copy member scope in STLport _vector.h. The corrected mangled name maps to 00A53650 and all 83 bytes match. |
| 00a4efc0 | deliverable | DIFFER | 3491 | 896 | 26 / 0 | Rebuild target or untouched witness after final header patching and assertion pinning. |
| 00a50510 | deliverable | MASKED_EQUAL | 1748 | 0 | 2 / 2 | Rebuild target or untouched witness after final header patching and assertion pinning. |
| 00a50510 | deliverable_ifbody | DIFFER | 1748 | 10 | 2 / 2 | Rebuild the same target allocator shape in each witness on the exact handoff headers. |
| 00aa0fa0 | deliverable | MASKED_EQUAL | 4460 | 0 | 4 / 3 | Rebuild target or untouched witness after final header patching and assertion pinning. |
| 00aa0fa0 | deliverable_ifbody | MASKED_EQUAL | 4460 | 0 | 4 / 3 | Rebuild the same target allocator shape in each witness on the exact handoff headers. |
| 00bd2d90 | deliverable | MASKED_EQUAL | 721 | 0 | 0 / 0 | Rebuild target or untouched witness after final header patching and assertion pinning. |
| 00bd2d90 | deliverable_ifbody | MASKED_EQUAL | 721 | 0 | 0 / 0 | Rebuild the same target allocator shape in each witness on the exact handoff headers. |

## Extended investigation

The following entries include controls, repeated results and compile failures. An independent source hypothesis can produce the same object as an earlier probe. All counts are for the target unless a witness address is shown.

- 00a4efc0 · extra2_453_native_malloc · COMPILE_FAIL · — bytes · — aligned instructions. Historical 4.5.3 implementation, including its by-reference resize. The native-malloc probe removes the OOM path absent from retail.
- 00a4efc0 · extra2_453_reference · COMPILE_FAIL · — bytes · — aligned instructions. Historical 4.5.3 implementation, including its by-reference resize. The native-malloc probe removes the OOM path absent from retail.
- 00a4efc0 · extra2_complete_constbase_common · DIFFER · 3491 bytes · 1068 aligned instructions. Restore the bounds check on both complete CArray indexers and compare the indicated const receiver and allocator.
- 00a4efc0 · extra2_complete_constbase_ifbody · DIFFER · 3491 bytes · 1073 aligned instructions. Restore the bounds check on both complete CArray indexers and compare the indicated const receiver and allocator.
- 00a4efc0 · extra2_complete_constbase_original · DIFFER · 3491 bytes · 1073 aligned instructions. Restore the bounds check on both complete CArray indexers and compare the indicated const receiver and allocator.
- 00a4efc0 · extra2_complete_release_common · DIFFER · 3491 bytes · 1068 aligned instructions. Restore the bounds check on both complete CArray indexers and compare the indicated const receiver and allocator.
- 00a4efc0 · extra2_complete_release_ifbody · DIFFER · 3491 bytes · 1073 aligned instructions. Restore the bounds check on both complete CArray indexers and compare the indicated const receiver and allocator.
- 00a4efc0 · extra2_complete_release_original · DIFFER · 3491 bytes · 1073 aligned instructions. Restore the bounds check on both complete CArray indexers and compare the indicated const receiver and allocator.
- 00a4efc0 · extra2_context_event_bodies · DIFFER · 3491 bytes · 1073 aligned instructions. See the compiler run ledger for the named control.
- 00a4efc0 · extra2_context_headers · DIFFER · 3491 bytes · 1073 aligned instructions. See the compiler run ledger for the named control.
- 00a4efc0 · extra2_index_conditional_common · DIFFER · 3505 bytes · 1011 aligned instructions. Measure the donor const indexer calling the mutable base, including simplified and donor diagnostic forms.
- 00a4efc0 · extra2_index_conditional_if · DIFFER · 3505 bytes · 1025 aligned instructions. Measure the donor const indexer calling the mutable base, including simplified and donor diagnostic forms.
- 00a4efc0 · extra2_index_const_common · DIFFER · 3491 bytes · 1066 aligned instructions. Measure the donor const indexer calling the mutable base, including simplified and donor diagnostic forms.
- 00a4efc0 · extra2_index_const_if · DIFFER · 3491 bytes · 1071 aligned instructions. Measure the donor const indexer calling the mutable base, including simplified and donor diagnostic forms.
- 00a4efc0 · extra2_index_donor_common · DIFFER · 3505 bytes · 1011 aligned instructions. Measure the donor const indexer calling the mutable base, including simplified and donor diagnostic forms.
- 00a4efc0 · extra2_index_donor_if · DIFFER · 3505 bytes · 1038 aligned instructions. Measure the donor const indexer calling the mutable base, including simplified and donor diagnostic forms.
- 00a4efc0 · extra2_index_message_common · DIFFER · 3505 bytes · 1022 aligned instructions. Measure the donor const indexer calling the mutable base, including simplified and donor diagnostic forms.
- 00a4efc0 · extra2_index_message_if · DIFFER · 3507 bytes · 1021 aligned instructions. Measure the donor const indexer calling the mutable base, including simplified and donor diagnostic forms.
- 00a4efc0 · extra2_package_all_donor · COMPILE_FAIL · — bytes · — aligned instructions. Use the shared project library with the indicated donor math, keyframe or complete CArray declarations. Typed variants enable the original reference typedefs.
- 00a4efc0 · extra2_package_all_donor_typed · DIFFER · 3475 bytes · 1070 aligned instructions. Use the shared project library with the indicated donor math, keyframe or complete CArray declarations. Typed variants enable the original reference typedefs.
- 00a4efc0 · extra2_package_full_carray · COMPILE_FAIL · — bytes · — aligned instructions. Use the shared project library with the indicated donor math, keyframe or complete CArray declarations. Typed variants enable the original reference typedefs.
- 00a4efc0 · extra2_package_full_carray_typed · DIFFER · 3475 bytes · 1070 aligned instructions. Use the shared project library with the indicated donor math, keyframe or complete CArray declarations. Typed variants enable the original reference typedefs.
- 00a4efc0 · extra2_package_keyframe_members · DIFFER · 3491 bytes · 1073 aligned instructions. Use the shared project library with the indicated donor math, keyframe or complete CArray declarations. Typed variants enable the original reference typedefs.
- 00a4efc0 · extra2_package_original_vmath · DIFFER · 3491 bytes · 1073 aligned instructions. Use the shared project library with the indicated donor math, keyframe or complete CArray declarations. Typed variants enable the original reference typedefs.
- 00a4efc0 · extra2_stock46 · DIFFER · 3574 bytes · 806 aligned instructions. Stock STLport 4.6.2 using its native allocator and resize ABI.
- 00a4efc0 · extra2_upstream453_all_donor · DIFFER · 3491 bytes · 1055 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_upstream453_full_carray · DIFFER · 3491 bytes · 1055 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_upstream453_keyframe_members · DIFFER · 3491 bytes · 1064 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_upstream453_normal · DIFFER · 3491 bytes · 1064 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_upstream453_original_vmath · DIFFER · 3491 bytes · 1064 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_upstream462_all_donor · DIFFER · 3491 bytes · 1055 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_upstream462_full_carray · DIFFER · 3491 bytes · 1055 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_upstream462_keyframe_members · DIFFER · 3491 bytes · 1064 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_upstream462_original_vmath · DIFFER · 3491 bytes · 1064 aligned instructions. Combine the vendor library with the indicated donor math, keyframe or CArray declarations.
- 00a4efc0 · extra2_vendor_malloc · DIFFER · 3577 bytes · 964 aligned instructions. Toggle the native malloc path and the documented resize reference ABI in the vendor source.
- 00a4efc0 · extra2_vendor_resize · DIFFER · 3494 bytes · 837 aligned instructions. Toggle the native malloc path and the documented resize reference ABI in the vendor source.
- 00a4efc0 · extra2_vendor_retail · DIFFER · 3491 bytes · 1064 aligned instructions. Toggle the native malloc path and the documented resize reference ABI in the vendor source.
- 00a4efc0 · extra3_assert_complete_conditional_common · DIFFER · 3505 bytes · 1013 aligned instructions. Inspect donor debug assertion temporary lifetimes and the nested message construction. A named intermediate was diagnostic only.
- 00a4efc0 · extra3_assert_complete_conditional_if · DIFFER · 3505 bytes · 1027 aligned instructions. Inspect donor debug assertion temporary lifetimes and the nested message construction. A named intermediate was diagnostic only.
- 00a4efc0 · extra3_assert_complete_dead_common · DIFFER · 3505 bytes · 1013 aligned instructions. Inspect donor debug assertion temporary lifetimes and the nested message construction. A named intermediate was diagnostic only.
- 00a4efc0 · extra3_assert_complete_dead_if · DIFFER · 3505 bytes · 1040 aligned instructions. Inspect donor debug assertion temporary lifetimes and the nested message construction. A named intermediate was diagnostic only.
- 00a4efc0 · extra3_assert_minimal_conditional_common · DIFFER · 3505 bytes · 1011 aligned instructions. Inspect donor debug assertion temporary lifetimes and the nested message construction. A named intermediate was diagnostic only.
- 00a4efc0 · extra3_assert_minimal_conditional_if · DIFFER · 3505 bytes · 1025 aligned instructions. Inspect donor debug assertion temporary lifetimes and the nested message construction. A named intermediate was diagnostic only.
- 00a4efc0 · extra3_assert_minimal_dead_common · DIFFER · 3505 bytes · 1011 aligned instructions. Inspect donor debug assertion temporary lifetimes and the nested message construction. A named intermediate was diagnostic only.
- 00a4efc0 · extra3_assert_minimal_dead_if · DIFFER · 3505 bytes · 1038 aligned instructions. Inspect donor debug assertion temporary lifetimes and the nested message construction. A named intermediate was diagnostic only.
- 00a4efc0 · extra3_base_access_asserted_common · DIFFER · 3491 bytes · 1068 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_access_asserted_if · DIFFER · 3491 bytes · 1073 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_access_core_common · DIFFER · 3481 bytes · 1059 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_access_core_if · DIFFER · 3475 bytes · 1070 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_access_original_common · DIFFER · 3491 bytes · 1068 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_access_original_if · DIFFER · 3491 bytes · 1073 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_using_asserted_common · DIFFER · 3491 bytes · 1068 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_using_asserted_if · DIFFER · 3491 bytes · 1073 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_using_core_common · DIFFER · 3481 bytes · 1059 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_using_core_if · DIFFER · 3475 bytes · 1070 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_using_original_common · DIFFER · 3491 bytes · 1068 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_base_using_original_if · DIFFER · 3491 bytes · 1073 aligned instructions. Measure base push_back access and using declarations against the full CArray and original math.
- 00a4efc0 · extra3_combined_scope_common · DIFFER · 3481 bytes · 1059 aligned instructions. Combine original string getters, uninitialized-copy scope and scoped library loops.
- 00a4efc0 · extra3_combined_scope_if · DIFFER · 3481 bytes · 1059 aligned instructions. Combine original string getters, uninitialized-copy scope and scoped library loops.
- 00a4efc0 · extra3_copy_chain_common · DIFFER · 3481 bytes · 1059 aligned instructions. Restore upstream public std::copy and its original pointer dispatch.
- 00a4efc0 · extra3_copy_chain_if · DIFFER · 3491 bytes · 1060 aligned instructions. Restore upstream public std::copy and its original pointer dispatch.
- 00a4efc0 · extra3_for_scope_common · DIFFER · 3479 bytes · 1050 aligned instructions. Extend the evidenced VC6-compatible for scope to the library loops.
- 00a4efc0 · extra3_for_scope_if · DIFFER · 3481 bytes · 1059 aligned instructions. Extend the evidenced VC6-compatible for scope to the library loops.
- 00a4efc0 · extra3_growth_algo_common · DIFFER · 3481 bytes · 1059 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_algo_if · DIFFER · 3475 bytes · 1070 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_overflow_common · DIFFER · 3481 bytes · 1059 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_overflow_if · DIFFER · 3475 bytes · 1070 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_quaternion_capture_common · DIFFER · 3481 bytes · 1059 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_quaternion_capture_if · DIFFER · 3475 bytes · 1070 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_rebind_common · DIFFER · 3481 bytes · 1059 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_rebind_if · DIFFER · 3475 bytes · 1070 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_shared_common · DIFFER · 3481 bytes · 1059 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_growth_shared_if · DIFFER · 3475 bytes · 1070 aligned instructions. Compare complete CArray with original vector overflow scope, copy dispatch, allocator rebind or original keyframe types.
- 00a4efc0 · extra3_malloc_chain_common · DIFFER · 3481 bytes · 1059 aligned instructions. Restore the original malloc allocator allocate/deallocate member chain, optionally with allocator rebinding.
- 00a4efc0 · extra3_malloc_chain_if · DIFFER · 3475 bytes · 1070 aligned instructions. Restore the original malloc allocator allocate/deallocate member chain, optionally with allocator rebinding.
- 00a4efc0 · extra3_malloc_rebind_common · DIFFER · 3481 bytes · 1059 aligned instructions. Restore the original malloc allocator allocate/deallocate member chain, optionally with allocator rebinding.
- 00a4efc0 · extra3_malloc_rebind_if · DIFFER · 3475 bytes · 1070 aligned instructions. Restore the original malloc allocator allocate/deallocate member chain, optionally with allocator rebinding.
- 00a4efc0 · extra3_original_copy_common · DIFFER · 3481 bytes · 1059 aligned instructions. Isolate STLport source blocks in uninitialized copy, FableWin string getter exits, or both.
- 00a4efc0 · extra3_original_copy_if · DIFFER · 3475 bytes · 1070 aligned instructions. Isolate STLport source blocks in uninitialized copy, FableWin string getter exits, or both.
- 00a4efc0 · extra3_original_getters_common · DIFFER · 3481 bytes · 1059 aligned instructions. Isolate STLport source blocks in uninitialized copy, FableWin string getter exits, or both.
- 00a4efc0 · extra3_original_getters_if · DIFFER · 3475 bytes · 1070 aligned instructions. Isolate STLport source blocks in uninitialized copy, FableWin string getter exits, or both.
- 00a4efc0 · extra3_original_runtime_common · DIFFER · 3481 bytes · 1059 aligned instructions. Isolate STLport source blocks in uninitialized copy, FableWin string getter exits, or both.
- 00a4efc0 · extra3_original_runtime_if · DIFFER · 3475 bytes · 1070 aligned instructions. Isolate STLport source blocks in uninitialized copy, FableWin string getter exits, or both.
- 00a4efc0 · extra3_receiver_mutable_this_empty_common · DIFFER · 3481 bytes · 1059 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_mutable_this_empty_if · DIFFER · 3475 bytes · 1070 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_mutable_this_guard_common · DIFFER · 3491 bytes · 1068 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_mutable_this_guard_if · DIFFER · 3491 bytes · 1073 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_mutable_vector_empty_common · DIFFER · 3481 bytes · 1059 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_mutable_vector_empty_if · DIFFER · 3475 bytes · 1070 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_mutable_vector_guard_common · DIFFER · 3491 bytes · 1068 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_mutable_vector_guard_if · DIFFER · 3491 bytes · 1073 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_stl_checked_empty_common · DIFFER · 3479 bytes · 1050 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_stl_checked_empty_if · DIFFER · 3481 bytes · 1059 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_stl_checked_guard_common · DIFFER · 3491 bytes · 1068 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_receiver_stl_checked_guard_if · DIFFER · 3491 bytes · 1073 aligned instructions. Use the evidenced mutable-vector access from the const CArray subscript, or the shared checked-vector subscript.
- 00a4efc0 · extra3_release_assertions_common · DIFFER · 3481 bytes · 1059 aligned instructions. Disable the global release assertion arm, including in the allocator.
- 00a4efc0 · extra3_release_assertions_if · DIFFER · 3481 bytes · 1059 aligned instructions. Disable the global release assertion arm, including in the allocator.
- 00a4efc0 · extra3_tag_abi_common · DIFFER · 3479 bytes · 1050 aligned instructions. Use the original upstream POD tag declarations for Boolean and iterator dispatch.
- 00a4efc0 · extra3_tag_abi_if · DIFFER · 3481 bytes · 1059 aligned instructions. Use the original upstream POD tag declarations for Boolean and iterator dispatch.
- 00a4efc0 · extra3_verified_array_common · DIFFER · 3481 bytes · 1059 aligned instructions. Recompile the comparison-order candidate or the unchanged witness with an explicitly selected common allocator.
- 00a4efc0 · extra3_verified_array_if · DIFFER · 3475 bytes · 1070 aligned instructions. Recompile the comparison-order candidate or the unchanged witness with an explicitly selected common allocator.
- 00a4efc0 · extra4_assert_expression_complete_conditional_common · DIFFER · 3505 bytes · 1013 aligned instructions. Model the condition as nested expressions so message temporaries end before the debugger test.
- 00a4efc0 · extra4_assert_expression_complete_conditional_if · DIFFER · 3505 bytes · 1027 aligned instructions. Model the condition as nested expressions so message temporaries end before the debugger test.
- 00a4efc0 · extra4_assert_expression_complete_dead_common · DIFFER · 3505 bytes · 1013 aligned instructions. Model the condition as nested expressions so message temporaries end before the debugger test.
- 00a4efc0 · extra4_assert_expression_complete_dead_if · DIFFER · 3505 bytes · 1040 aligned instructions. Model the condition as nested expressions so message temporaries end before the debugger test.
- 00a4efc0 · extra4_assert_expression_minimal_conditional_common · DIFFER · 3505 bytes · 1011 aligned instructions. Model the condition as nested expressions so message temporaries end before the debugger test.
- 00a4efc0 · extra4_assert_expression_minimal_conditional_if · DIFFER · 3505 bytes · 1025 aligned instructions. Model the condition as nested expressions so message temporaries end before the debugger test.
- 00a4efc0 · extra4_assert_expression_minimal_dead_common · DIFFER · 3505 bytes · 1011 aligned instructions. Model the condition as nested expressions so message temporaries end before the debugger test.
- 00a4efc0 · extra4_assert_expression_minimal_dead_if · DIFFER · 3505 bytes · 1038 aligned instructions. Model the condition as nested expressions so message temporaries end before the debugger test.
- 00a4efc0 · extra4_push_copied_stlp_empty_common · DIFFER · 3481 bytes · 1059 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_copied_stlp_empty_if · DIFFER · 3475 bytes · 1070 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_copied_stlp_guard_common · DIFFER · 3491 bytes · 1068 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_copied_stlp_guard_if · DIFFER · 3491 bytes · 1073 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_donor_insert_empty_common · DIFFER · 3249 bytes · 559 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_donor_insert_empty_if · DIFFER · 3249 bytes · 556 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_donor_insert_guard_common · DIFFER · 3287 bytes · 579 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_donor_insert_guard_if · DIFFER · 3287 bytes · 576 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_donor_overflow_empty_common · DIFFER · 3502 bytes · 672 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_donor_overflow_empty_if · DIFFER · 3508 bytes · 669 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_donor_overflow_guard_common · DIFFER · 3544 bytes · 664 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_donor_overflow_guard_if · DIFFER · 3544 bytes · 664 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_native_insert_empty_common · DIFFER · 3220 bytes · 590 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_native_insert_empty_if · DIFFER · 3220 bytes · 590 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_native_insert_guard_common · DIFFER · 3220 bytes · 590 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_push_native_insert_guard_if · DIFFER · 3220 bytes · 590 aligned instructions. Model the six-line original debug CArray::push_back directly. Compare its capacity test or native pointer test with the original STLport overflow or insert paths.
- 00a4efc0 · extra4_size_asserted_common · DIFFER · 3491 bytes · 1068 aligned instructions. Reverse the operands in the element-count times element-size expression, preserving every local and member type.
- 00a4efc0 · extra4_size_asserted_if · DIFFER · 3491 bytes · 1073 aligned instructions. Reverse the operands in the element-count times element-size expression, preserving every local and member type.
- 00a4efc0 · extra4_size_baseline_common · DIFFER · 3491 bytes · 1068 aligned instructions. Reverse the operands in the element-count times element-size expression, preserving every local and member type.
- 00a4efc0 · extra4_size_baseline_if · DIFFER · 3491 bytes · 1073 aligned instructions. Reverse the operands in the element-count times element-size expression, preserving every local and member type.
- 00a4efc0 · extra4_size_full_common · DIFFER · 3481 bytes · 1059 aligned instructions. Reverse the operands in the element-count times element-size expression, preserving every local and member type.
- 00a4efc0 · extra4_size_full_if · DIFFER · 3475 bytes · 1070 aligned instructions. Reverse the operands in the element-count times element-size expression, preserving every local and member type.
- 00a4efc0 · extra4_vendor_complete_generic_common · DIFFER · 3481 bytes · 1056 aligned instructions. Compile the upstream library with the native by-reference resize and malloc path, plus the retail tag constructor model. Compare generic and reconstructed helper owners.
- 00a4efc0 · extra4_vendor_complete_generic_if · DIFFER · 3491 bytes · 1068 aligned instructions. Compile the upstream library with the native by-reference resize and malloc path, plus the retail tag constructor model. Compare generic and reconstructed helper owners.
- 00a4efc0 · extra4_vendor_complete_owners_common · DIFFER · 3481 bytes · 1056 aligned instructions. Compile the upstream library with the native by-reference resize and malloc path, plus the retail tag constructor model. Compare generic and reconstructed helper owners.
- 00a4efc0 · extra4_vendor_complete_owners_if · DIFFER · 3491 bytes · 1068 aligned instructions. Compile the upstream library with the native by-reference resize and malloc path, plus the retail tag constructor model. Compare generic and reconstructed helper owners.
- 00a4efc0 · extra4_vendor_project_generic_common · DIFFER · 3491 bytes · 1065 aligned instructions. Compile the upstream library with the native by-reference resize and malloc path, plus the retail tag constructor model. Compare generic and reconstructed helper owners.
- 00a4efc0 · extra4_vendor_project_generic_if · DIFFER · 3491 bytes · 1068 aligned instructions. Compile the upstream library with the native by-reference resize and malloc path, plus the retail tag constructor model. Compare generic and reconstructed helper owners.
- 00a4efc0 · extra4_vendor_project_owners_common · DIFFER · 3491 bytes · 1065 aligned instructions. Compile the upstream library with the native by-reference resize and malloc path, plus the retail tag constructor model. Compare generic and reconstructed helper owners.
- 00a4efc0 · extra4_vendor_project_owners_if · DIFFER · 3491 bytes · 1068 aligned instructions. Compile the upstream library with the native by-reference resize and malloc path, plus the retail tag constructor model. Compare generic and reconstructed helper owners.
- 00a50510 · extra3_verified_array_common · MASKED_EQUAL · 1748 bytes · 515 aligned instructions. Recompile the comparison-order candidate or the unchanged witness with an explicitly selected common allocator.
- 00a50510 · extra3_verified_array_if · DIFFER · 1748 bytes · 510 aligned instructions. Recompile the comparison-order candidate or the unchanged witness with an explicitly selected common allocator.
- 00aa0fa0 · extra3_verified_array_common · MASKED_EQUAL · 4460 bytes · 1205 aligned instructions. Recompile the comparison-order candidate or the unchanged witness with an explicitly selected common allocator.
- 00aa0fa0 · extra3_verified_array_if · MASKED_EQUAL · 4460 bytes · 1205 aligned instructions. Recompile the comparison-order candidate or the unchanged witness with an explicitly selected common allocator.
- 00bd2d90 · extra3_verified_array_common · MASKED_EQUAL · 721 bytes · 226 aligned instructions. Recompile the comparison-order candidate or the unchanged witness with an explicitly selected common allocator.
- 00bd2d90 · extra3_verified_array_if · MASKED_EQUAL · 721 bytes · 226 aligned instructions. Recompile the comparison-order candidate or the unchanged witness with an explicitly selected common allocator.


## Exact-match continuation

- 00a4efc0 · extra5_boundary_disabled_flagfirst_common · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_disabled_flagfirst_if · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_disabled_outer_common · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_disabled_outer_if · DIFFER · 3491 bytes · 1068 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_disabled_predfirst_common · DIFFER · 3491 bytes · 1055 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_disabled_predfirst_if · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_disabled_report_common · DIFFER · 3491 bytes · 1055 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_disabled_report_if · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_bare_report_common · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_bare_report_if · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_break_only_common · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_break_only_if · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_guarded_report_common · DIFFER · 3475 bytes · 1070 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_guarded_report_if · DIFFER · 3491 bytes · 1060 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_message_only_common · DIFFER · 3475 bytes · 1070 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_message_only_if · DIFFER · 3491 bytes · 1073 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_static_check_common · DIFFER · 3481 bytes · 1059 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_boundary_release_static_check_if · DIFFER · 3475 bytes · 1070 normalized instructions. Test which parts of the donor subscript diagnostic remain behind a disabled release check.
- 00a4efc0 · extra5_copy_sdk_both_common · DIFFER · 3860 bytes · 696 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sdk_both_if · DIFFER · 3860 bytes · 694 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sdk_loop_common · DIFFER · 3860 bytes · 696 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sdk_loop_if · DIFFER · 3860 bytes · 694 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sdk_parameter_sgi_order_common · DIFFER · 3860 bytes · 696 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sdk_parameter_sgi_order_if · DIFFER · 3860 bytes · 694 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sdk_placement_common · DIFFER · 3481 bytes · 1059 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sdk_placement_if · DIFFER · 3475 bytes · 1070 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sgi_walker_reverse_common · DIFFER · 3481 bytes · 1059 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_copy_sgi_walker_reverse_if · DIFFER · 3475 bytes · 1070 normalized instructions. Distinguish SGI destination walker, SDK destination parameter, increment order and placement-new cast.
- 00a4efc0 · extra5_exact_delivery_project · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Make the opt-in self-contained; eliminate the unrelated allocator patch and test both header sets.
- 00a4efc0 · extra5_exact_delivery_vs2003 · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Make the opt-in self-contained; eliminate the unrelated allocator patch and test both header sets.
- 00a4efc0 · extra5_final_exact_project · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Replay additive opt-ins and witnesses on clean headers.
- 00a4efc0 · extra5_final_exact_vs2003 · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Replay additive opt-ins and witnesses on clean headers.
- 00a4efc0 · extra5_index_assert_arm_common · DIFFER · 3491 bytes · 1060 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_assert_arm_if · DIFFER · 3491 bytes · 1068 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_false_outer_common · DIFFER · 3479 bytes · 1050 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_false_outer_if · DIFFER · 3481 bytes · 1059 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_predicate_common · DIFFER · 3479 bytes · 1050 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_predicate_if · DIFFER · 3481 bytes · 1059 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_trace_only_common · DIFFER · 3491 bytes · 1068 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_trace_only_if · DIFFER · 3491 bytes · 1068 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_void_pred_common · DIFFER · 3481 bytes · 1059 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_index_void_pred_if · DIFFER · 3475 bytes · 1070 normalized instructions. Release bounds predicate retention, empty arm, or conditional release check.
- 00a4efc0 · extra5_linkage_inline_after · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Use inline linkage for two TU helpers; compare placing their definitions before and after the target.
- 00a4efc0 · extra5_linkage_inline_before · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Use inline linkage for two TU helpers; compare placing their definitions before and after the target.
- 00a4efc0 · extra5_location_native_c1_common · DIFFER · 3505 bytes · 1013 normalized instructions. Test the recovered diagnostic source coordinates and messages in dead paths.
- 00a4efc0 · extra5_location_native_c1_if · DIFFER · 3505 bytes · 1027 normalized instructions. Test the recovered diagnostic source coordinates and messages in dead paths.
- 00a4efc0 · extra5_location_native_common · DIFFER · 3505 bytes · 1013 normalized instructions. Test the recovered diagnostic source coordinates and messages in dead paths.
- 00a4efc0 · extra5_location_native_empty_pool_common · DIFFER · 3505 bytes · 1013 normalized instructions. Test the recovered diagnostic source coordinates and messages in dead paths.
- 00a4efc0 · extra5_location_native_empty_pool_if · DIFFER · 3505 bytes · 1023 normalized instructions. Test the recovered diagnostic source coordinates and messages in dead paths.
- 00a4efc0 · extra5_location_native_if · DIFFER · 3505 bytes · 1021 normalized instructions. Test the recovered diagnostic source coordinates and messages in dead paths.
- 00a4efc0 · extra5_location_short_common · DIFFER · 3491 bytes · 1060 normalized instructions. Test the recovered diagnostic source coordinates and messages in dead paths.
- 00a4efc0 · extra5_location_short_if · DIFFER · 3491 bytes · 1068 normalized instructions. Test the recovered diagnostic source coordinates and messages in dead paths.
- 00a4efc0 · extra5_loop_postfix_carray_common · DIFFER · 3481 bytes · 1059 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_postfix_carray_if · DIFFER · 3475 bytes · 1070 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_postfix_shared_common · DIFFER · 3491 bytes · 1068 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_postfix_shared_if · DIFFER · 3491 bytes · 1073 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_source_limit_first_carray_common · DIFFER · 3481 bytes · 1043 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_source_limit_first_carray_if · DIFFER · 3475 bytes · 1054 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_source_limit_first_shared_common · DIFFER · 3491 bytes · 1052 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_source_limit_first_shared_if · DIFFER · 3491 bytes · 1057 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_vc6_empty_statement_carray_common · DIFFER · 3481 bytes · 1059 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_vc6_empty_statement_carray_if · DIFFER · 3475 bytes · 1070 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_vc6_empty_statement_shared_common · DIFFER · 3491 bytes · 1068 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_loop_vc6_empty_statement_shared_if · DIFFER · 3491 bytes · 1073 normalized instructions. Test loop increment/condition spelling or equivalent VC6 for scope without new locals.
- 00a4efc0 · extra5_math_crtmax_complete_common · DIFFER · 3479 bytes · 1050 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_crtmax_complete_if · DIFFER · 3481 bytes · 1059 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_crtmax_shared_common · DIFFER · 3491 bytes · 1068 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_crtmax_shared_if · DIFFER · 3491 bytes · 1073 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_donor_squares_complete_common · DIFFER · 3479 bytes · 1050 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_donor_squares_complete_if · DIFFER · 3475 bytes · 1070 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_donor_squares_shared_common · DIFFER · 3491 bytes · 1068 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_donor_squares_shared_if · DIFFER · 3491 bytes · 1073 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_header_idioms_complete_common · DIFFER · 3479 bytes · 1050 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_header_idioms_complete_if · DIFFER · 3491 bytes · 1073 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_header_idioms_shared_common · DIFFER · 3491 bytes · 1068 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_header_idioms_shared_if · DIFFER · 3491 bytes · 1073 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_sqrt_squares_complete_common · DIFFER · 3479 bytes · 1050 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_sqrt_squares_complete_if · DIFFER · 3475 bytes · 1070 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_sqrt_squares_shared_common · DIFFER · 3491 bytes · 1068 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_sqrt_squares_shared_if · DIFFER · 3491 bytes · 1073 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_winmax_complete_common · DIFFER · 3479 bytes · 1050 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_winmax_complete_if · DIFFER · 3481 bytes · 1059 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_winmax_shared_common · DIFFER · 3491 bytes · 1068 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_math_winmax_shared_if · DIFFER · 3491 bytes · 1073 normalized instructions. Original-style grouping of math products and max macro expansions.
- 00a4efc0 · extra5_pool_empty_donor · DIFFER · 3505 bytes · 1027 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_empty_off · DIFFER · 3481 bytes · 1059 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_empty_shared · DIFFER · 3491 bytes · 1060 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_message_donor · DIFFER · 3505 bytes · 1040 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_message_off · DIFFER · 3475 bytes · 1070 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_message_shared · DIFFER · 3491 bytes · 1073 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_native_donor · DIFFER · 3505 bytes · 1018 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_native_off · DIFFER · 3491 bytes · 1053 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_native_release_donor · DIFFER · 3505 bytes · 1018 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_native_release_off · DIFFER · 3491 bytes · 1053 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_native_release_shared · DIFFER · 3491 bytes · 1062 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_pool_native_shared · DIFFER · 3491 bytes · 1062 normalized instructions. Release allocator diagnostic as corroborated by FableWin allocation routine.
- 00a4efc0 · extra5_query_hp_classic_common · DIFFER · 3491 bytes · 1064 normalized instructions. Original vendor iterator query configuration.
- 00a4efc0 · extra5_query_hp_classic_if · DIFFER · 3491 bytes · 1067 normalized instructions. Original vendor iterator query configuration.
- 00a4efc0 · extra5_query_hp_complete_common · DIFFER · 3481 bytes · 1055 normalized instructions. Original vendor iterator query configuration.
- 00a4efc0 · extra5_query_hp_complete_if · DIFFER · 3491 bytes · 1054 normalized instructions. Original vendor iterator query configuration.
- 00a4efc0 · extra5_query_traits_classic_common · DIFFER · 3491 bytes · 1065 normalized instructions. Original vendor iterator query configuration.
- 00a4efc0 · extra5_query_traits_classic_if · DIFFER · 3491 bytes · 1068 normalized instructions. Original vendor iterator query configuration.
- 00a4efc0 · extra5_query_traits_complete_common · DIFFER · 3481 bytes · 1056 normalized instructions. Original vendor iterator query configuration.
- 00a4efc0 · extra5_query_traits_complete_if · DIFFER · 3491 bytes · 1068 normalized instructions. Original vendor iterator query configuration.
- 00a4efc0 · extra5_relation_negated_carray_common · DIFFER · 3481 bytes · 1059 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_negated_carray_if · DIFFER · 3475 bytes · 1070 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_negated_reversed_carray_common · DIFFER · 3481 bytes · 1054 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_negated_reversed_carray_if · DIFFER · 3475 bytes · 1064 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_negated_reversed_shared_common · DIFFER · 3491 bytes · 1063 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_negated_reversed_shared_if · DIFFER · 3491 bytes · 1068 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_negated_shared_common · DIFFER · 3491 bytes · 1068 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_negated_shared_if · DIFFER · 3491 bytes · 1073 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_reversed_carray_common · DIFFER · 3481 bytes · 1054 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_reversed_carray_if · DIFFER · 3475 bytes · 1064 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_reversed_shared_common · DIFFER · 3491 bytes · 1063 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_relation_reversed_shared_if · DIFFER · 3491 bytes · 1068 normalized instructions. Equivalent unsigned comparison spelling across all three palettes.
- 00a4efc0 · extra5_rtm_asserted_common · DIFFER · 3491 bytes · 1068 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00a4efc0 · extra5_rtm_asserted_if · DIFFER · 3491 bytes · 1073 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00a4efc0 · extra5_rtm_complete_common · DIFFER · 3481 bytes · 1059 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00a4efc0 · extra5_rtm_complete_if · DIFFER · 3475 bytes · 1070 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00a4efc0 · extra5_rtm_primary_common · DIFFER · 3491 bytes · 1068 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00a4efc0 · extra5_rtm_primary_if · DIFFER · 3491 bytes · 1073 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00a4efc0 · extra5_signed_production · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Fresh grade on the final delivered source.
- 00a4efc0 · extra5_signed_rtm · MASKED_EQUAL · 3491 bytes · 1084 normalized instructions. Fresh grade on the final delivered source.
- 00a4efc0 · extra5_word_owner_control_common · DIFFER · 3481 bytes · 1059 normalized instructions. Restore native out-of-line vector-word overflow owner when testing the copy walker.
- 00a4efc0 · extra5_word_owner_control_if · DIFFER · 3475 bytes · 1070 normalized instructions. Restore native out-of-line vector-word overflow owner when testing the copy walker.
- 00a4efc0 · extra5_word_owner_iterator_result_common · DIFFER · 3481 bytes · 1059 normalized instructions. Restore native out-of-line vector-word overflow owner when testing the copy walker.
- 00a4efc0 · extra5_word_owner_iterator_result_if · DIFFER · 3475 bytes · 1070 normalized instructions. Restore native out-of-line vector-word overflow owner when testing the copy walker.
- 00a4efc0 · extra5_word_owner_reversed_increment_common · DIFFER · 3481 bytes · 1059 normalized instructions. Restore native out-of-line vector-word overflow owner when testing the copy walker.
- 00a4efc0 · extra5_word_owner_reversed_increment_if · DIFFER · 3475 bytes · 1070 normalized instructions. Restore native out-of-line vector-word overflow owner when testing the copy walker.
- 00a50510 · extra5_exact_delivery_witness · MASKED_EQUAL · 1748 bytes · 515 normalized instructions. Make the opt-in self-contained; eliminate the unrelated allocator patch and test both header sets.
- 00a50510 · extra5_final_exact_witness · MASKED_EQUAL · 1748 bytes · 515 normalized instructions. Replay additive opt-ins and witnesses on clean headers.
- 00a50510 · extra5_rtm_witness_common · MASKED_EQUAL · 1748 bytes · 515 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00a50510 · extra5_rtm_witness_if · DIFFER · 1748 bytes · 510 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00aa0fa0 · extra5_exact_delivery_witness · MASKED_EQUAL · 4460 bytes · 1205 normalized instructions. Make the opt-in self-contained; eliminate the unrelated allocator patch and test both header sets.
- 00aa0fa0 · extra5_final_exact_witness · MASKED_EQUAL · 4460 bytes · 1205 normalized instructions. Replay additive opt-ins and witnesses on clean headers.
- 00aa0fa0 · extra5_rtm_witness_common · MASKED_EQUAL · 4460 bytes · 1205 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00aa0fa0 · extra5_rtm_witness_if · MASKED_EQUAL · 4460 bytes · 1205 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00bd2d90 · extra5_exact_delivery_witness · MASKED_EQUAL · 721 bytes · 226 normalized instructions. Make the opt-in self-contained; eliminate the unrelated allocator patch and test both header sets.
- 00bd2d90 · extra5_final_exact_witness · MASKED_EQUAL · 721 bytes · 226 normalized instructions. Replay additive opt-ins and witnesses on clean headers.
- 00bd2d90 · extra5_rtm_witness_common · MASKED_EQUAL · 721 bytes · 226 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
- 00bd2d90 · extra5_rtm_witness_if · MASKED_EQUAL · 721 bytes · 226 normalized instructions. Compile against the pristine Visual Studio 2003 RTM include directory.
