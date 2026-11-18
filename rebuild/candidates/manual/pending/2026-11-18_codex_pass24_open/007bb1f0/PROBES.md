# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- port 01 shared headers — COMPILE_FAIL.
- port 02 retail calls — COMPILE_FAIL.
- port 03 pointer compare — DIFFER, 958 v 960 bytes, 23 strict instruction hunks, first unequal byte at 0x007BB202.
- port final — DIFFER, 958 v 960 bytes, 10 strict instruction hunks, first unequal byte at 0x007BB202.
- shape 04 native gettc expand — DIFFER, 958 v 960 bytes, 23 strict instruction hunks, first unequal byte at 0x007BB202.
- shape 05 component has before get — DIFFER, 973 v 960 bytes, 29 strict instruction hunks, first unequal byte at 0x007BB1F9.
- shape 06 deed loop limit break — DIFFER, 958 v 960 bytes, 23 strict instruction hunks, first unequal byte at 0x007BB202.
- shape 07 deed state inner scope — DIFFER, 958 v 960 bytes, 23 strict instruction hunks, first unequal byte at 0x007BB202.
- shape 08 trespass state unnamed — DIFFER, 958 v 960 bytes, 23 strict instruction hunks, first unequal byte at 0x007BB202.
- shape 09 opinion getter reference — DIFFER, 958 v 960 bytes, 10 strict instruction hunks, first unequal byte at 0x007BB202.
- shape 10 frame age as difference — DIFFER, 953 v 960 bytes, 27 strict instruction hunks, first unequal byte at 0x007BB202.
- shape 11 original pointer assignment scope — DIFFER, 958 v 960 bytes, 23 strict instruction hunks, first unequal byte at 0x007BB202.
- shape 12 parent before world and opinion name — DIFFER, 958 v 960 bytes, 21 strict instruction hunks, first unequal byte at 0x007BB1F2.
- shape 13 parent qualified frame compare — DIFFER, 958 v 960 bytes, 22 strict instruction hunks, first unequal byte at 0x007BB1F2.
- shape 14 original loop break with opinion source — DIFFER, 958 v 960 bytes, 21 strict instruction hunks, first unequal byte at 0x007BB1F2.

The selected result is PARKED (958 object bytes, 960 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
