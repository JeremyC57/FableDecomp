# Compiler probes

- package baseline — PREPROCESS_FAIL.
- parked 01 ported header — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 02 collision begin end named — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 03 collision find result const ref — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 04 collision if inverted return — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 05 collision original blocked — DIFFER, 774 v 768 bytes, 16 instruction hunks, first unequal byte at 0x007C8207.
- parked 06 collision fp left rotated receiver — COMPILE_FAIL.
- parked 07 collision position uninitialized — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 08 collision direction uninitialized — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 09 collision vector names before facing — DIFFER, 782 v 768 bytes, 23 instruction hunks, first unequal byte at 0x007C8207.
- parked 10 collision dot swapped — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 11 collision dot original formal — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 12 collision normalise newton group — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.
- parked 13 collision use find forwarder — DIFFER, 768 v 768 bytes, 18 instruction hunks, first unequal byte at 0x007C820B.
- parked handoff — DIFFER, 768 v 768 bytes, 14 instruction hunks, first unequal byte at 0x007C8213.

The selected result is PARKED (768 object bytes, 768 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
