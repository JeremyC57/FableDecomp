# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- port 01 shared headers — DIFFER, 1157 v 1023 bytes, 84 strict instruction hunks, first unequal byte at 0x00B63802.
- port 02 retail calls — DIFFER, 1059 v 1023 bytes, 68 strict instruction hunks, first unequal byte at 0x00B63802.
- port final — MASKED_MATCH_PENDING_AUDIT, 1023 v 1023 bytes, 0 strict instruction hunks.
- shape 04 vendor vector context — DIFFER, 1059 v 1023 bytes, 68 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 05 by value construct — DIFFER, 1059 v 1023 bytes, 68 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 06 retail texture touch guard — DIFFER, 1059 v 1023 bytes, 68 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 07 cache loop bound — DIFFER, 1039 v 1023 bytes, 39 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 08 signed flare count calculation — DIFFER, 1059 v 1023 bytes, 68 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 09 scope flare and texture vectors — DIFFER, 1059 v 1023 bytes, 68 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 10 texture success guard continue — DIFFER, 1059 v 1023 bytes, 68 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 11 native frame scopes — DIFFER, 1059 v 1023 bytes, 68 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 12 local append pointer and environment first — DIFFER, 1009 v 1023 bytes, 52 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 13 signed flare bound plus append — DIFFER, 1009 v 1023 bytes, 53 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 14 original texture bound without named end — DIFFER, 1040 v 1023 bytes, 48 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 15 flag update via accumulation — DIFFER, 1019 v 1023 bytes, 50 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 16 visible stl overflow local append — DIFFER, 1023 v 1023 bytes, 2 strict instruction hunks, first unequal byte at 0x00B63880.
- shape 17 frame bound const reference — DIFFER, 1014 v 1023 bytes, 53 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 18 append branch early texture continue — DIFFER, 1009 v 1023 bytes, 52 strict instruction hunks, first unequal byte at 0x00B63802.
- shape 19 native texture set order — MASKED_MATCH_PENDING_AUDIT, 1023 v 1023 bytes, 0 strict instruction hunks.
- shape 20 original locals texture set order — MASKED_MATCH_PENDING_AUDIT, 1023 v 1023 bytes, 0 strict instruction hunks.

The selected result is RELOCATION_MATCH (1023 object bytes, 1023 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
