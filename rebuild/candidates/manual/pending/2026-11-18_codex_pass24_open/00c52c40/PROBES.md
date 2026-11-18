# Compiler probes

- package baseline — DIFFER, 75 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C67.
- parked 01 family cast before switch — DIFFER, 75 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C67.
- parked 02 case5 named zero model — DIFFER, 75 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C67.
- parked 03 case5 conditional break — DIFFER, 80 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C47.
- parked 04 case5 else test — DIFFER, 75 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C67.
- parked 05 case5 else select — DIFFER, 80 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C47.
- parked 06 case5 select named — DIFFER, 74 v 75 bytes, 6 instruction hunks, first unequal byte at 0x00C52C41.
- parked 07 case5 arm switch before default — DIFFER, 75 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C4D.
- parked 08 case5 if reversed model4 model2 — DIFFER, 75 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C67.
- parked 09 families if and model if — DIFFER, 78 v 75 bytes, 3 instruction hunks, first unequal byte at 0x00C52C44.
- parked 10 model reference in function — DIFFER, 75 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C67.
- parked handoff — DIFFER, 75 v 75 bytes, 2 instruction hunks, first unequal byte at 0x00C52C67.

The selected result is PARKED (75 object bytes, 75 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
