# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 header body — DIFFER, 287 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7BB5.
- parked 02 compound twice — MASKED_MATCH_PENDING_AUDIT, 287 v 287 bytes, 0 instruction hunks.
- parked 03 offset ref — DIFFER, 281 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7B4B.
- parked 04 glyph pointer alias — DIFFER, 287 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7BB5.
- parked 05 advance through alias — DIFFER, 287 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7BB5.
- parked 06 offset arithmetic group — MASKED_MATCH_PENDING_AUDIT, 287 v 287 bytes, 0 instruction hunks.
- parked 07 pre increment in header — DIFFER, 287 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7BB5.
- parked 08 read char after test — DIFFER, 287 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7BB5.
- parked 09 loop continue lift — DIFFER, 287 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7BB5.
- parked 10 for header — DIFFER, 288 v 287 bytes, 5 instruction hunks, first unequal byte at 0x00AB7B4B.
- parked 11 keep letter declaration separate — DIFFER, 287 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7BB5.
- parked 12 x y ternary — DIFFER, 287 v 287 bytes, 3 instruction hunks, first unequal byte at 0x00AB7BB5.
- parked final — MASKED_MATCH_PENDING_AUDIT, 287 v 287 bytes, 0 instruction hunks.

The selected result is RELOCATION_MATCH (287 object bytes, 287 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
