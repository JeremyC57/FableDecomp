# Compiler probes

- package baseline — PREPROCESS_FAIL.
- parked 01 ported header — COMPILE_FAIL.
- parked 02 remove duplicate square — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 03 square neg product group — DIFFER, 1315 v 1315 bytes, 7 instruction hunks, first unequal byte at 0x0087601C.
- parked 04 square call cos angle group — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 05 square call sin angle group — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 06 square both trig groups — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 07 square both trig named values — DIFFER, 1274 v 1315 bytes, 46 instruction hunks, first unequal byte at 0x00875FF7.
- parked 08 square offset scale group — DIFFER, 1315 v 1315 bytes, 4 instruction hunks, first unequal byte at 0x008760C0.
- parked 09 square offset add group — DIFFER, 1315 v 1315 bytes, 4 instruction hunks, first unequal byte at 0x008760C0.
- parked 10 square offset sub group — DIFFER, 1315 v 1315 bytes, 7 instruction hunks, first unequal byte at 0x008760C0.
- parked 11 square rotation basis explicit refs — DIFFER, 1315 v 1315 bytes, 7 instruction hunks, first unequal byte at 0x008760C0.
- parked 12 square c1 loop postincrement — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 13 square countdown equivalent index — DIFFER, 1315 v 1315 bytes, 8 instruction hunks, first unequal byte at 0x008760C0.
- parked 14 square add and scale group — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 15 loop c1 init before second axis — DIFFER, 1315 v 1315 bytes, 7 instruction hunks, first unequal byte at 0x008760C0.
- parked 16 loop c1 init before rotated basis — DIFFER, 1315 v 1315 bytes, 7 instruction hunks, first unequal byte at 0x0087605F.
- parked 17 loop c1 while increment — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 18 loop c1 dowhile — DIFFER, 1315 v 1315 bytes, 6 instruction hunks, first unequal byte at 0x008760C0.
- parked 19 loop c1 move array decl before axes — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 20 rotation axis scale copy first — DIFFER, 1291 v 1315 bytes, 27 instruction hunks, first unequal byte at 0x00875FF7.
- parked 21 rotation angle float times named — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.
- parked 22 rotation axis named cosine first — DIFFER, 1306 v 1315 bytes, 63 instruction hunks, first unequal byte at 0x00875FD0.
- parked 23 rotation axis named sine first — DIFFER, 1315 v 1315 bytes, 51 instruction hunks, first unequal byte at 0x00875FD0.
- parked handoff — DIFFER, 1315 v 1315 bytes, 5 instruction hunks, first unequal byte at 0x008760C0.

The selected result is PARKED (1315 object bytes, 1315 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
