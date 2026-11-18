- guard_path_success: DIFFER, 170 object bytes, 15 instruction diff hunks
- iterator_second: DIFFER, 177 object bytes, 19 instruction diff hunks
- main_clean: MASKED_MATCH_PENDING_AUDIT, 169 object bytes, 0 instruction diff hunks
- named_sound_manager: DIFFER, 170 object bytes, 15 instruction diff hunks
- named_track_ref: DIFFER, 170 object bytes, 15 instruction diff hunks
- package_baseline: DIFFER, 170 object bytes, 15 instruction diff hunks
- pointer_entry: DIFFER, 170 object bytes, 15 instruction diff hunks
- pointer_fade: DIFFER, 170 object bytes, 15 instruction diff hunks
- positive_sound_and_found: DIFFER, 170 object bytes, 15 instruction diff hunks
- shared_tu: DIFFER, 170 object bytes, 15 instruction diff hunks
- shared_whole_tu: COMPILE_FAIL, n/a object bytes, n/a instruction diff hunks
- store_in_success_arms: MASKED_MATCH_PENDING_AUDIT, 169 object bytes, 0 instruction diff hunks
- Final branch-local music state assignments reproduce the 169-byte function. All four operands verified, both tests passed.

The selected result is RELOCATION_MATCH (169 object bytes, 169 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
