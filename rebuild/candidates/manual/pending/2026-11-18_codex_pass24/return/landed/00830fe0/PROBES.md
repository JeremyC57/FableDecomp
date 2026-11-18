# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- port 01 persist context — COMPILE_FAIL.
- port 02 float class — COMPILE_FAIL.
- port 03 virtual base — DIFFER, 1504 v 1504 bytes, 3 strict instruction hunks, first unequal byte at 0x008311D0.
- port final — MASKED_MATCH_PENDING_AUDIT, 1504 v 1504 bytes, 0 strict instruction hunks.
- shape 04 float traits static default — COMPILE_FAIL.
- shape 05 base failure early return — DIFFER, 1498 v 1504 bytes, 5 strict instruction hunks, first unequal byte at 0x00830FF3.
- shape 06 persistence vector component order — DIFFER, 1504 v 1504 bytes, 3 strict instruction hunks, first unequal byte at 0x008311D0.
- shape 07 save relationship conjunction scope — DIFFER, 1504 v 1504 bytes, 3 strict instruction hunks, first unequal byte at 0x008311D0.
- shape 08 bool init split — DIFFER, 1504 v 1504 bytes, 3 strict instruction hunks, first unequal byte at 0x008311D0.
- shape 09 set relationships only when live — DIFFER, 1504 v 1504 bytes, 3 strict instruction hunks, first unequal byte at 0x008311D0.
- shape 10 all transfer components in subscope — DIFFER, 1504 v 1504 bytes, 3 strict instruction hunks, first unequal byte at 0x008311D0.
- shape 11 persist binary float caller extent — DIFFER, 1504 v 1504 bytes, 3 strict instruction hunks, first unequal byte at 0x008311D0.
- shape 12 counted pointer original release option — DIFFER, 1504 v 1504 bytes, 5 strict instruction hunks, first unequal byte at 0x008311D0.
- shape 13 float template value ctor enabled — DIFFER, 1504 v 1504 bytes, 3 strict instruction hunks, first unequal byte at 0x008311D0.
- shape 14 float explicit const default constructor — DIFFER, 1366 v 1504 bytes, 5 strict instruction hunks, first unequal byte at 0x00830FF5.
- shape 15 vector transfer expands — DIFFER, 1337 v 1504 bytes, 2 strict instruction hunks, first unequal byte at 0x00830FF5.
- shape 16 visible traits reader — MASKED_MATCH_PENDING_AUDIT, 1504 v 1504 bytes, 0 strict instruction hunks.
- shape 17 visible original stream scope — MASKED_MATCH_PENDING_AUDIT, 1504 v 1504 bytes, 0 strict instruction hunks.

The selected result is RELOCATION_MATCH (1504 object bytes, 1504 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
