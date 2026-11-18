# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 ported header — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 02 height add onto ahead — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 03 height const ahead — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 04 height const reference to ahead — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 05 height value copy ahead — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 06 height const reference to offset — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 07 height value copy offset — DIFFER, 652 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BAEA.
- parked 08 height named result vector — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 09 height components ctor — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 10 height components group — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 11 height components assign — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 12 height using vector add assign — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 13 height add add xy group — DIFFER, 642 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 14 height add initialise add — DIFFER, 652 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BAEA.
- parked 15 height add add named result — DIFFER, 652 v 642 bytes, 4 instruction hunks, first unequal byte at 0x00A1BAEA.
- parked 16 height field references — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 17 height field values — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 18 height ahead field references — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 19 height offset field references — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 20 height offset field values — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 21 height named xyz values — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 22 height named xyz compound — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 23 height named add constant to triplet — DIFFER, 642 v 642 bytes, 5 instruction hunks, first unequal byte at 0x00A1BB08.
- parked 24 height grouped constructor reference — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.
- parked handoff — DIFFER, 642 v 642 bytes, 3 instruction hunks, first unequal byte at 0x00A1BB08.

The selected result is PARKED (642 object bytes, 642 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
