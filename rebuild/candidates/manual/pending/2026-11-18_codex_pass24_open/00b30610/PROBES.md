# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- parked 01 inplace — COMPILE_FAIL.
- parked 02 out — COMPILE_FAIL.
- parked 03 copy2 — COMPILE_FAIL.
- parked 04 same inplace split — COMPILE_FAIL.
- parked 05 translation first — COMPILE_FAIL.
- parked 06 tail translation — COMPILE_FAIL.
- parked 07 pair translation — COMPILE_FAIL.
- parked 08 pair translation y — COMPILE_FAIL.
- parked 09 split accumulated xyz — COMPILE_FAIL.
- parked 10 split accumulated zyx — COMPILE_FAIL.
- parked 11 shared plane dot reverse — COMPILE_FAIL.
- parked 12 shared inplace zyx — COMPILE_FAIL.
- parked 20 shared inplace — DIFFER, 1264 v 710 bytes, 21 instruction hunks, first unequal byte at 0x00B30610.
- parked 21 shared out — DIFFER, 434 v 710 bytes, 10 instruction hunks, first unequal byte at 0x00B30610.
- parked 22 shared copy2 — DIFFER, 610 v 710 bytes, 10 instruction hunks, first unequal byte at 0x00B30610.
- parked 23 shared same inplace split — DIFFER, 678 v 710 bytes, 37 instruction hunks, first unequal byte at 0x00B3064B.
- parked 24 shared translation first — DIFFER, 506 v 710 bytes, 21 instruction hunks, first unequal byte at 0x00B30610.
- parked 25 shared tail translation — DIFFER, 534 v 710 bytes, 37 instruction hunks, first unequal byte at 0x00B30612.
- parked 26 shared pair translation — DIFFER, 474 v 710 bytes, 23 instruction hunks, first unequal byte at 0x00B30610.
- parked 27 shared pair translation y — DIFFER, 490 v 710 bytes, 19 instruction hunks, first unequal byte at 0x00B30610.
- parked 28 shared split accumulated xyz — DIFFER, 496 v 710 bytes, 17 instruction hunks, first unequal byte at 0x00B30612.
- parked 29 shared split accumulated zyx — DIFFER, 490 v 710 bytes, 20 instruction hunks, first unequal byte at 0x00B30610.
- parked 30 shared shared plane dot reverse — DIFFER, 506 v 710 bytes, 21 instruction hunks, first unequal byte at 0x00B30610.
- parked 31 shared shared inplace zyx — DIFFER, 1248 v 710 bytes, 24 instruction hunks, first unequal byte at 0x00B30610.
- parked handoff — DIFFER, 678 v 710 bytes, 37 instruction hunks, first unequal byte at 0x00B3064B.

The selected result is PARKED (678 object bytes, 710 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
