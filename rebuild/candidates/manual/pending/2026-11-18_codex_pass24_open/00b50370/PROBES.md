# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 ported header — DIFFER, 406 v 406 bytes, 3 instruction hunks, first unequal byte at 0x00B50462.
- parked 02 object constant order native — DIFFER, 406 v 406 bytes, 5 instruction hunks, first unequal byte at 0x00B5044E.
- parked 03 split scene data alias — DIFFER, 406 v 406 bytes, 3 instruction hunks, first unequal byte at 0x00B50462.
- parked 04 duplicate constants chained — DIFFER, 406 v 406 bytes, 3 instruction hunks, first unequal byte at 0x00B50462.
- parked 05 initialise scaler before settings — DIFFER, 406 v 406 bytes, 5 instruction hunks, first unequal byte at 0x00B50451.
- parked 06 camera settings first — DIFFER, 406 v 406 bytes, 5 instruction hunks, first unequal byte at 0x00B5044E.
- parked 07 body set scaler members — DIFFER, 406 v 406 bytes, 4 instruction hunks, first unequal byte at 0x00B50462.
- parked 08 in struct brace offsets — DIFFER, 406 v 406 bytes, 3 instruction hunks, first unequal byte at 0x00B50462.
- parked 09 constexpr logical group — DIFFER, 406 v 406 bytes, 5 instruction hunks, first unequal byte at 0x00B50462.
- parked 10 independent source settings — DIFFER, 406 v 406 bytes, 4 instruction hunks, first unequal byte at 0x00B50452.
- parked 11 reverse constant assignment — DIFFER, 406 v 406 bytes, 5 instruction hunks, first unequal byte at 0x00B50455.
- parked 12 move initializer list fields — DIFFER, 409 v 406 bytes, 5 instruction hunks, first unequal byte at 0x00B503C4.
- parked handoff — DIFFER, 406 v 406 bytes, 3 instruction hunks, first unequal byte at 0x00B50462.

The selected result is PARKED (406 object bytes, 406 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
