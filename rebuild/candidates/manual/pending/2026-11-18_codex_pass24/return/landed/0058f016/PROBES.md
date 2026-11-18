# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- port 01 shared headers — COMPILE_FAIL.
- port 02 retail calls — COMPILE_FAIL.
- port 03 typed ui calls — DIFFER, 1580 v 1587 bytes, 7 strict instruction hunks, first unequal byte at 0x0058F23A.
- port final — MASKED_MATCH_PENDING_AUDIT, 1587 v 1587 bytes, 0 strict instruction hunks.
- port handoff — MASKED_MATCH_PENDING_AUDIT, 1587 v 1587 bytes, 0 strict instruction hunks.
- shape 04 retail map default constructor and expanded peektc — DIFFER, 1586 v 1587 bytes, 3 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 05 map merge distinct named reference — DIFFER, 1586 v 1587 bytes, 3 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 06 snapshot item reference — DIFFER, 1586 v 1587 bytes, 1 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 07 gift guard nested — DIFFER, 1586 v 1587 bytes, 3 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 08 gift guard after target check — DIFFER, 1586 v 1587 bytes, 3 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 09 original static title — DIFFER, 1586 v 1587 bytes, 3 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 10 gift menu entry typed — DIFFER, 1586 v 1587 bytes, 3 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 11 merge inverted branch original title — DIFFER, 1586 v 1587 bytes, 4 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 12 actual bitset and title — MASKED_MATCH_PENDING_AUDIT, 1587 v 1587 bytes, 0 strict instruction hunks.
- shape 13 no item alias direct pair — DIFFER, 1588 v 1587 bytes, 8 strict instruction hunks, first unequal byte at 0x0058F23A.
- shape 14 original donor names — DIFFER, 1587 v 1587 bytes, 2 strict instruction hunks, first unequal byte at 0x0058F275.
- shape 15 original subtitle local — MASKED_MATCH_PENDING_AUDIT, 1587 v 1587 bytes, 0 strict instruction hunks.
- shape 16 native tree dtor — MASKED_MATCH_PENDING_AUDIT, 1587 v 1587 bytes, 0 strict instruction hunks.
- shape 17 native tree dtor original item expression — DIFFER, 1587 v 1587 bytes, 2 strict instruction hunks, first unequal byte at 0x0058F275.
- shape 18 retail wide literal — MASKED_MATCH_PENDING_AUDIT, 1587 v 1587 bytes, 0 strict instruction hunks.

The selected result is RELOCATION_MATCH (1587 object bytes, 1587 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
