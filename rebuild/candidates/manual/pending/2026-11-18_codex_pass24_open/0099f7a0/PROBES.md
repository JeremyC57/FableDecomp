# Compiler probes

- package baseline — DIFFER, 130 v 130 bytes, 3 instruction hunks, first unequal byte at 0x0099F7BA.
- parked 01 copy assignment compound increment — DIFFER, 126 v 130 bytes, 2 instruction hunks, first unequal byte at 0x0099F7B8.
- parked 02 value beta copy pointer — DIFFER, 130 v 130 bytes, 2 instruction hunks, first unequal byte at 0x0099F7BA.
- parked 03 value use separate no ref overwrite — DIFFER, 130 v 130 bytes, 2 instruction hunks, first unequal byte at 0x0099F7BA.
- parked 04 reference share ref count — DIFFER, 130 v 130 bytes, 2 instruction hunks, first unequal byte at 0x0099F7BA.
- parked 05 value early out assign — DIFFER, 130 v 130 bytes, 3 instruction hunks, first unequal byte at 0x0099F7BA.
- parked 06 shared assignment via ctor — DIFFER, 126 v 130 bytes, 2 instruction hunks, first unequal byte at 0x0099F7B8.
- parked 07 shared copy ctor via assignment — DIFFER, 134 v 130 bytes, 5 instruction hunks, first unequal byte at 0x0099F7BA.
- parked 08 string param reference before copy — DIFFER, 130 v 130 bytes, 3 instruction hunks, first unequal byte at 0x0099F7BA.
- parked 09 copy reference member at start — DIFFER, 130 v 130 bytes, 3 instruction hunks, first unequal byte at 0x0099F7BA.
- parked 10 ctor implicit assignment increment — DIFFER, 126 v 130 bytes, 2 instruction hunks, first unequal byte at 0x0099F7B8.
- parked 11 assign result as expression — DIFFER, 130 v 130 bytes, 3 instruction hunks, first unequal byte at 0x0099F7BA.
- parked handoff — DIFFER, 126 v 130 bytes, 2 instruction hunks, first unequal byte at 0x0099F7B8.

## Additional main-chat comparisons

- main_count_result_value_default_assign — DIFFER, 130 bytes, 2 strict instruction hunks.
- main_count_result_value_copy_init — DIFFER, 134 bytes, 4 strict instruction hunks.
- main_count_result_value_delegated_copy — DIFFER, 134 bytes, 4 strict instruction hunks.
- main_count_pointer_first_guard_default_assign — DIFFER, 136 bytes, 6 strict instruction hunks.
- main_count_pointer_first_guard_copy_init — DIFFER, 147 bytes, 11 strict instruction hunks.
- main_count_pointer_first_guard_delegated_copy — DIFFER, 147 bytes, 11 strict instruction hunks.
- main_count_reference_after_test_default_assign — DIFFER, 130 bytes, 3 strict instruction hunks.
- main_count_reference_after_test_copy_init — DIFFER, 134 bytes, 6 strict instruction hunks.
- main_count_reference_after_test_delegated_copy — DIFFER, 134 bytes, 6 strict instruction hunks.
- main_count_increment_then_assign_default_assign — DIFFER, 128 bytes, 4 strict instruction hunks.
- main_count_increment_then_assign_copy_init — DIFFER, 132 bytes, 6 strict instruction hunks.
- main_count_increment_then_assign_delegated_copy — DIFFER, 132 bytes, 6 strict instruction hunks.
- main_count_through_input_lvalue_default_assign — DIFFER, 130 bytes, 3 strict instruction hunks.
- main_count_through_input_lvalue_copy_init — DIFFER, 134 bytes, 6 strict instruction hunks.
- main_count_through_input_lvalue_delegated_copy — DIFFER, 134 bytes, 6 strict instruction hunks.
- main_count_member_alias_check_default_assign — DIFFER, 131 bytes, 4 strict instruction hunks.
- main_count_member_alias_check_copy_init — DIFFER, 137 bytes, 8 strict instruction hunks.
- main_count_member_alias_check_delegated_copy — DIFFER, 137 bytes, 8 strict instruction hunks.
- main_landed_assign_local — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_assign_named_count — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_assign_post — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_assign_refptr — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_both_local — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_both_named_count — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_both_post — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_both_refptr — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_ctor_local — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_ctor_named_count — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_ctor_post — DIFFER, 126 bytes, 2 strict instruction hunks.
- main_landed_ctor_refptr — DIFFER, 126 bytes, 2 strict instruction hunks.

The selected result is PARKED (126 object bytes, 130 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
