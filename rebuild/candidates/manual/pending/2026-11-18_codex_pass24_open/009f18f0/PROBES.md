# Compiler probes

- package baseline — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.
- parked 01 copy start pos after one write — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.
- parked 02 no wrap first arm — DIFFER, 171 v 166 bytes, 15 instruction hunks, first unequal byte at 0x009F18F1.
- parked 03 wrap then unconditional return — DIFFER, 166 v 166 bytes, 19 instruction hunks, first unequal byte at 0x009F18F6.
- parked 04 end pos named value — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.
- parked 05 unsigned mask explicit — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.
- parked 06 pointer arithmetic for first copy — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.
- parked 07 use buffer pointer named — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.
- parked 08 construct next read position — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.
- parked 09 success arm update length — DIFFER, 166 v 166 bytes, 19 instruction hunks, first unequal byte at 0x009F18F6.
- parked 10 opaque overflow mask — DIFFER, 150 v 166 bytes, 16 instruction hunks, first unequal byte at 0x009F18F6.
- parked 11 byte count scoped — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.
- parked handoff — DIFFER, 166 v 166 bytes, 20 instruction hunks, first unequal byte at 0x009F18F1.

The selected result is PARKED (166 object bytes, 166 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
