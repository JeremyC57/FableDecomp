# Compiler probes

- package baseline — DIFFER, 614 v 616 bytes, 2 instruction hunks, first unequal byte at 0x0089CE87.
- parked 01 show cone boolean — DIFFER, 606 v 616 bytes, 3 instruction hunks, first unequal byte at 0x0089CE87.
- parked 02 show cone bool ref — DIFFER, 606 v 616 bytes, 3 instruction hunks, first unequal byte at 0x0089CE87.
- parked 03 show cone float cos — DIFFER, 614 v 616 bytes, 2 instruction hunks, first unequal byte at 0x0089CE87.
- parked 04 show cone literal first — DIFFER, 614 v 616 bytes, 2 instruction hunks, first unequal byte at 0x0089CE87.
- parked 05 show cone comparison to zero — DIFFER, 624 v 616 bytes, 4 instruction hunks, first unequal byte at 0x0089CE87.
- parked 06 show cone comparison double bound — DIFFER, 618 v 616 bytes, 35 instruction hunks, first unequal byte at 0x0089CE72.
- parked 07 show cone group dot — DIFFER, 614 v 616 bytes, 2 instruction hunks, first unequal byte at 0x0089CE87.
- parked 08 show cone group cos — DIFFER, 614 v 616 bytes, 2 instruction hunks, first unequal byte at 0x0089CE87.
- parked 09 show cone dot as ref — DIFFER, 620 v 616 bytes, 4 instruction hunks, first unequal byte at 0x0089CE87.
- parked 10 show cone float casts — DIFFER, 614 v 616 bytes, 2 instruction hunks, first unequal byte at 0x0089CE87.
- parked 11 show cone conditional success — DIFFER, 614 v 616 bytes, 2 instruction hunks, first unequal byte at 0x0089CE87.
- parked 12 show cone direction ref — DIFFER, 605 v 616 bytes, 34 instruction hunks, first unequal byte at 0x0089CE73.
- parked handoff — DIFFER, 614 v 616 bytes, 2 instruction hunks, first unequal byte at 0x0089CE87.

## Additional main-chat comparisons

- main_close_named_dot_only — DIFFER, 618 bytes, 4 strict instruction hunks.
- main_close_named_both — DIFFER, 618 bytes, 35 strict instruction hunks.
- main_close_compare_float_return_fall — DIFFER, 610 bytes, 5 strict instruction hunks.
- main_close_double_sub_zero — DIFFER, 624 bytes, 4 strict instruction hunks.
- main_close_one_epsilon_cmp — DIFFER, 641 bytes, 14 strict instruction hunks.
- main_close_nested_continue — DIFFER, 614 bytes, 2 strict instruction hunks.
- main_close_named_cast_operand — DIFFER, 618 bytes, 35 strict instruction hunks.
- main_close_named_live_temp — DIFFER, 610 bytes, 35 strict instruction hunks.

The selected result is PARKED (614 object bytes, 616 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
