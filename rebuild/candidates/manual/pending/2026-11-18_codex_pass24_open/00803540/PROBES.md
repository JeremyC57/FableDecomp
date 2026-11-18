# Compiler probes

- package baseline — DIFFER, 1108 v 1108 bytes, 5 instruction hunks, first unequal byte at 0x0080380C.
- parked 01 walk sin cos separate — DIFFER, 1100 v 1108 bytes, 56 instruction hunks, first unequal byte at 0x00803542.
- parked 02 walk cos sin separate — DIFFER, 1100 v 1108 bytes, 55 instruction hunks, first unequal byte at 0x00803542.
- parked 03 walk sin cos position named — DIFFER, 1108 v 1108 bytes, 5 instruction hunks, first unequal byte at 0x0080380C.
- parked 04 walk sin cos angle scaled double — DIFFER, 1108 v 1108 bytes, 5 instruction hunks, first unequal byte at 0x0080380C.
- parked 05 walk cast entire sine — DIFFER, 1108 v 1108 bytes, 6 instruction hunks, first unequal byte at 0x0080380C.
- parked 06 walk cast entire cosine — DIFFER, 1108 v 1108 bytes, 6 instruction hunks, first unequal byte at 0x0080380C.
- parked 07 walk sin product group — DIFFER, 1108 v 1108 bytes, 5 instruction hunks, first unequal byte at 0x0080380C.
- parked 08 walk tail keep scaled named — DIFFER, 1108 v 1108 bytes, 5 instruction hunks, first unequal byte at 0x0080380C.
- parked 09 walk tail reference receiver — DIFFER, 1108 v 1108 bytes, 5 instruction hunks, first unequal byte at 0x0080380C.
- parked 10 walk tail spelling add assign — DIFFER, 1132 v 1108 bytes, 9 instruction hunks, first unequal byte at 0x0080354F.
- parked 11 walk scale vector grouping — DIFFER, 1067 v 1108 bytes, 22 instruction hunks, first unequal byte at 0x0080354F.
- parked 12 walk scale store order group — DIFFER, 1108 v 1108 bytes, 9 instruction hunks, first unequal byte at 0x0080380C.
- parked 13 walk move offsets before sine — DIFFER, 1108 v 1108 bytes, 9 instruction hunks, first unequal byte at 0x008037F7.
- parked handoff — DIFFER, 1108 v 1108 bytes, 5 instruction hunks, first unequal byte at 0x0080380C.

The selected result is PARKED (1108 object bytes, 1108 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
