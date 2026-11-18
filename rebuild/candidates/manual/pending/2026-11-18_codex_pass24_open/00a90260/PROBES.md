# Compiler probes

- package baseline — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.
- parked 01 remove unused local classes — COMPILE_FAIL.
- parked 02 total indices const — COMPILE_FAIL.
- parked 03 initialize scoped ptr ctor — COMPILE_FAIL.
- parked 04 count using ifelse reference — COMPILE_FAIL.
- parked 05 degenerate count using zero or value — COMPILE_FAIL.
- parked 06 scope offset order — COMPILE_FAIL.
- parked 07 copy use reference pointers — COMPILE_FAIL.
- parked 08 copy use size t multiply — COMPILE_FAIL.
- parked 09 copy loop invert single cond — COMPILE_FAIL.
- parked 10 copy loop count empty scope — COMPILE_FAIL.
- parked 11 copy loop apply start after copy — COMPILE_FAIL.
- parked 12 copy loop read source before index sizes — COMPILE_FAIL.
- parked 20 shared remove unused local classes — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.
- parked 21 shared total indices const — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.
- parked 22 shared initialize scoped ptr ctor — DIFFER, 488 v 496 bytes, 41 instruction hunks, first unequal byte at 0x00A90262.
- parked 23 shared count using ifelse reference — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.
- parked 24 shared degenerate count using zero or value — DIFFER, 506 v 496 bytes, 43 instruction hunks, first unequal byte at 0x00A90272.
- parked 25 shared scope offset order — DIFFER, 505 v 496 bytes, 33 instruction hunks, first unequal byte at 0x00A90272.
- parked 26 shared copy use reference pointers — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.
- parked 27 shared copy use size t multiply — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.
- parked 28 shared copy loop invert single cond — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.
- parked 29 shared copy loop count empty scope — DIFFER, 502 v 496 bytes, 17 instruction hunks, first unequal byte at 0x00A902EA.
- parked 30 shared copy loop apply start after copy — DIFFER, 500 v 496 bytes, 21 instruction hunks, first unequal byte at 0x00A90262.
- parked 31 shared copy loop read source before index sizes — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.
- parked handoff — DIFFER, 498 v 496 bytes, 35 instruction hunks, first unequal byte at 0x00A90272.

The selected result is PARKED (498 object bytes, 496 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
