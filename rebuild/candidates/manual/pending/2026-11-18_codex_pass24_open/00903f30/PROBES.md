# Compiler probes

- package baseline — DIFFER, 254 v 254 bytes, 10 instruction hunks, first unequal byte at 0x00903F75.
- parked 01 route parameter inline false test — DIFFER, 254 v 254 bytes, 10 instruction hunks, first unequal byte at 0x00903F75.
- parked 02 route guard clause — DIFFER, 254 v 254 bytes, 10 instruction hunks, first unequal byte at 0x00903F75.
- parked 03 route gettc before mapping — DIFFER, 249 v 254 bytes, 14 instruction hunks, first unequal byte at 0x00903F30.
- parked 04 route materialise bool in existing helper — DIFFER, 262 v 254 bytes, 13 instruction hunks, first unequal byte at 0x00903F44.
- parked 05 route pointer alias before query — DIFFER, 254 v 254 bytes, 10 instruction hunks, first unequal byte at 0x00903F75.
- parked 06 route plain if with compare — DIFFER, 246 v 254 bytes, 13 instruction hunks, first unequal byte at 0x00903F44.
- parked 07 route conversion is separate switch — DIFFER, 254 v 254 bytes, 10 instruction hunks, first unequal byte at 0x00903F75.
- parked 08 route move value reference — DIFFER, 254 v 254 bytes, 10 instruction hunks, first unequal byte at 0x00903F75.
- parked 09 route branch assignment conversion — DIFFER, 253 v 254 bytes, 8 instruction hunks, first unequal byte at 0x00903F44.
- parked 10 route control via peekcc — COMPILE_FAIL.
- parked 11 route inline constructor no named move — DIFFER, 280 v 254 bytes, 13 instruction hunks, first unequal byte at 0x00903F30.
- parked 12 route if via const ref cast — DIFFER, 246 v 254 bytes, 13 instruction hunks, first unequal byte at 0x00903F44.
- parked handoff — DIFFER, 254 v 254 bytes, 10 instruction hunks, first unequal byte at 0x00903F75.

The selected result is PARKED (254 object bytes, 254 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
