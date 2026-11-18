# Compiler probes

- package baseline — PREPROCESS_FAIL.
- parked 01 ported header — DIFFER, 1057 v 1057 bytes, 5 instruction hunks, first unequal byte at 0x00657664.
- parked 02 typed base members — DIFFER, 1057 v 1057 bytes, 5 instruction hunks, first unequal byte at 0x00657664.
- parked 03 hotbar named anchor — DIFFER, 1059 v 1057 bytes, 43 instruction hunks, first unequal byte at 0x00657586.
- parked 04 hotbar ref anchor — DIFFER, 1059 v 1057 bytes, 43 instruction hunks, first unequal byte at 0x00657586.
- parked 05 hotbar named offset — DIFFER, 1057 v 1057 bytes, 1 instruction hunks, first unequal byte at 0x0065765B.
- parked 06 hotbar named sum — DIFFER, 1055 v 1057 bytes, 7 instruction hunks, first unequal byte at 0x006575A8.
- parked 07 hotbar inplace sum — DIFFER, 1065 v 1057 bytes, 13 instruction hunks, first unequal byte at 0x006575A8.
- parked 08 hotbar named alias all — DIFFER, 1059 v 1057 bytes, 43 instruction hunks, first unequal byte at 0x00657586.
- parked 09 hotbar change helper add — DIFFER, 1061 v 1057 bytes, 17 instruction hunks, first unequal byte at 0x006575A8.
- parked 10 hotbar named add result — DIFFER, 1073 v 1057 bytes, 17 instruction hunks, first unequal byte at 0x006575A8.
- parked 11 hotbar y x direct member — DIFFER, 1056 v 1057 bytes, 40 instruction hunks, first unequal byte at 0x00657586.
- parked 12 hotbar y x explicit orientation — DIFFER, 1056 v 1057 bytes, 40 instruction hunks, first unequal byte at 0x00657586.
- parked 13 hotbar use offset add — DIFFER, 1070 v 1057 bytes, 12 instruction hunks, first unequal byte at 0x006575A8.
- parked 14 hotbar copy offset c2d value — DIFFER, 1072 v 1057 bytes, 10 instruction hunks, first unequal byte at 0x006575A8.
- parked 15 hotbar offset ref after anchor — DIFFER, 1059 v 1057 bytes, 43 instruction hunks, first unequal byte at 0x00657586.
- parked 16 hotbar offset alias copy x — DIFFER, 1060 v 1057 bytes, 37 instruction hunks, first unequal byte at 0x00657586.
- parked 17 hotbar reverse alias only first branch — MASKED_MATCH_PENDING_AUDIT, 1057 v 1057 bytes, 0 instruction hunks.
- parked 18 hotbar name single x float — DIFFER, 1058 v 1057 bytes, 41 instruction hunks, first unequal byte at 0x00657586.
- parked 19 hotbar name single y float — DIFFER, 1058 v 1057 bytes, 41 instruction hunks, first unequal byte at 0x00657586.
- parked 20 hotbar group vector add result — DIFFER, 1057 v 1057 bytes, 1 instruction hunks, first unequal byte at 0x0065765B.
- parked final — MASKED_MATCH_PENDING_AUDIT, 1057 v 1057 bytes, 0 instruction hunks.

The selected result is RELOCATION_MATCH (1057 object bytes, 1057 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
