# Compiler probes

- package baseline — DIFFER, 1052 v 1053 bytes, 52 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 01 shared — COMPILE_FAIL.
- parked security 02 shared alive predicate — DIFFER, 1058 v 1053 bytes, 92 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 03 navigation scratch implicit initialization — DIFFER, 1053 v 1053 bytes, 72 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 04 navigation radius argument first — DIFFER, 1058 v 1053 bytes, 92 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 05 navigation radius reference scope — DIFFER, 1058 v 1053 bytes, 92 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 06 frame timer with native lifetime order — DIFFER, 1074 v 1053 bytes, 76 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 07 look iteration post decrement — DIFFER, 1058 v 1053 bytes, 92 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 08 look iteration continuation combined — DIFFER, 1058 v 1053 bytes, 91 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 09 perception const reference first — DIFFER, 1058 v 1053 bytes, 92 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 10 navigation initial do loop — DIFFER, 1058 v 1053 bytes, 92 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 11 visible thing guard inverted — DIFFER, 1074 v 1053 bytes, 91 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 12 task dereference through accessors — DIFFER, 1058 v 1053 bytes, 92 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 13 task pointer null comparison — COMPILE_FAIL.
- parked security 14 look vector default ctor and assign — DIFFER, 1166 v 1053 bytes, 86 strict instruction hunks, first unequal byte at 0x0093FCD2.
- parked security 15 look vector rhs order — DIFFER, 1058 v 1053 bytes, 92 strict instruction hunks, first unequal byte at 0x0093FCD2.

The selected result is PARKED (1053 object bytes, 1053 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
