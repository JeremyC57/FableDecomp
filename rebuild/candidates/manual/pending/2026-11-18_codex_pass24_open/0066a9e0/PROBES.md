# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 header body — MASKED_MATCH_PENDING_AUDIT, 626 v 626 bytes, 0 instruction hunks.
- parked 02 last get string — DIFFER, 622 v 626 bytes, 6 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 03 last get string as const ref — DIFFER, 622 v 626 bytes, 6 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 04 last get string as value — DIFFER, 622 v 626 bytes, 6 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 05 last conversion as const ref — MASKED_MATCH_PENDING_AUDIT, 626 v 626 bytes, 0 instruction hunks.
- parked 06 last conversion explicit — DIFFER, 622 v 626 bytes, 6 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 07 last named string — DIFFER, 626 v 626 bytes, 1 instruction hunks, first unequal byte at 0x0066AC01.
- parked 08 last named string reference — DIFFER, 626 v 626 bytes, 1 instruction hunks, first unequal byte at 0x0066AC01.
- parked 09 last direct qualified free function — DIFFER, 622 v 626 bytes, 6 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 10 last grouped virtual result — DIFFER, 622 v 626 bytes, 6 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 11 last cast through conversion — DIFFER, 645 v 626 bytes, 3 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 12 force inline forwarder — DIFFER, 632 v 626 bytes, 2 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 13 force inline conversion — DIFFER, 556 v 626 bytes, 17 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 14 last named def string — DIFFER, 628 v 626 bytes, 4 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 15 last named def reference — DIFFER, 628 v 626 bytes, 4 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 16 last def reference cast — DIFFER, 626 v 626 bytes, 1 instruction hunks, first unequal byte at 0x0066AC01.
- parked 17 last def named address — DIFFER, 628 v 626 bytes, 4 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 18 last temp and call split — DIFFER, 630 v 626 bytes, 5 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 19 last temp in lambda style block — DIFFER, 626 v 626 bytes, 1 instruction hunks, first unequal byte at 0x0066AC01.
- parked 20 explicit failed pair bools — DIFFER, 608 v 626 bytes, 20 instruction hunks, first unequal byte at 0x0066A9EF.
- parked 21 const invalid handle — DIFFER, 626 v 626 bytes, 1 instruction hunks, first unequal byte at 0x0066AC01.
- parked handoff — DIFFER, 626 v 626 bytes, 1 instruction hunks, first unequal byte at 0x0066AC01.

The selected result is PARKED (626 object bytes, 626 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
