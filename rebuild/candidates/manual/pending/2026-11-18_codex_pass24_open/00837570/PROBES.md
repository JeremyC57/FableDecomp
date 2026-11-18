# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 ported header — DIFFER, 697 v 697 bytes, 7 instruction hunks, first unequal byte at 0x00837575.
- parked 02 empty name first else matching — DIFFER, 738 v 697 bytes, 28 instruction hunks, first unequal byte at 0x00837572.
- parked 03 name first else empty matching — DIFFER, 738 v 697 bytes, 30 instruction hunks, first unequal byte at 0x00837572.
- parked 04 empty name with inverted condition — DIFFER, 697 v 697 bytes, 8 instruction hunks, first unequal byte at 0x00837575.
- parked 05 empty name construct start — DIFFER, 759 v 697 bytes, 19 instruction hunks, first unequal byte at 0x00837572.
- parked 06 empty name reference cast — DIFFER, 697 v 697 bytes, 7 instruction hunks, first unequal byte at 0x00837575.
- parked 07 empty name reference — DIFFER, 724 v 697 bytes, 27 instruction hunks, first unequal byte at 0x00837572.
- parked 08 empty name value — DIFFER, 724 v 697 bytes, 27 instruction hunks, first unequal byte at 0x00837572.
- parked 09 prepend last line then end — DIFFER, 684 v 697 bytes, 25 instruction hunks, first unequal byte at 0x00837573.
- parked 10 construct last line full — DIFFER, 714 v 697 bytes, 34 instruction hunks, first unequal byte at 0x00837572.
- parked 11 append last line via explicit op — DIFFER, 697 v 697 bytes, 7 instruction hunks, first unequal byte at 0x00837575.
- parked 12 empty name conditional empty literal — DIFFER, 748 v 697 bytes, 32 instruction hunks, first unequal byte at 0x00837574.
- parked 13 original base save — DIFFER, 697 v 697 bytes, 7 instruction hunks, first unequal byte at 0x00837575.
- parked handoff — DIFFER, 697 v 697 bytes, 7 instruction hunks, first unequal byte at 0x00837575.

The selected result is PARKED (697 object bytes, 697 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
