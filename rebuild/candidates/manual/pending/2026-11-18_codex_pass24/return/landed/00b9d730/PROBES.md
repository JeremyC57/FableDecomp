# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 header body — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 02 alias mesh — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 03 alias primitive — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 04 alias scale — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 05 alias sphere radius — MASKED_MATCH_PENDING_AUDIT, 429 v 429 bytes, 0 instruction hunks.
- parked 06 apply scale after init — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 07 double product — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 08 double cast scale — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 09 alias camera — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D738.
- parked 10 group radius expression — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 11 group scale expression — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked 12 move radius first — DIFFER, 429 v 429 bytes, 9 instruction hunks, first unequal byte at 0x00B9D73C.
- parked 13 pass operand directly — DIFFER, 429 v 429 bytes, 2 instruction hunks, first unequal byte at 0x00B9D7A5.
- parked final — MASKED_MATCH_PENDING_AUDIT, 429 v 429 bytes, 0 instruction hunks.

The selected result is RELOCATION_MATCH (429 object bytes, 429 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
