# Compiler probes

- package baseline — DIFFER, 259 v 259 bytes, 2 instruction hunks, first unequal byte at 0x00A55FCA.
- parked 01 normal components read — MASKED_MATCH_PENDING_AUDIT, 259 v 259 bytes, 0 instruction hunks.
- parked 02 ref components — DIFFER, 259 v 259 bytes, 2 instruction hunks, first unequal byte at 0x00A55FD3.
- parked 03 read product after squares — DIFFER, 247 v 259 bytes, 19 instruction hunks, first unequal byte at 0x00A55F92.
- parked 04 first use each product — DIFFER, 241 v 259 bytes, 17 instruction hunks, first unequal byte at 0x00A55F92.
- parked 05 product values written late — DIFFER, 259 v 259 bytes, 2 instruction hunks, first unequal byte at 0x00A55FCA.
- parked 06 squaring math function — PREPROCESS_FAIL.
- parked 07 group reference for products — DIFFER, 259 v 259 bytes, 2 instruction hunks, first unequal byte at 0x00A55FCA.
- parked 08 axis multiplication after sine — DIFFER, 255 v 259 bytes, 17 instruction hunks, first unequal byte at 0x00A55F92.
- parked 09 square y z x decl — DIFFER, 247 v 259 bytes, 15 instruction hunks, first unequal byte at 0x00A55F92.
- parked 10 cos then squares then sin — DIFFER, 251 v 259 bytes, 15 instruction hunks, first unequal byte at 0x00A55F92.
- parked 11 gfsqr — COMPILE_FAIL.
- parked 12 gfsqr grouped — DIFFER, 259 v 259 bytes, 2 instruction hunks, first unequal byte at 0x00A55FCA.
- parked 13 cast axis — DIFFER, 259 v 259 bytes, 2 instruction hunks, first unequal byte at 0x00A55FCA.
- parked 14 scope alias first x square — DIFFER, 259 v 259 bytes, 2 instruction hunks, first unequal byte at 0x00A55FCA.
- parked 15 normal alias after first square — MASKED_MATCH_PENDING_AUDIT, 259 v 259 bytes, 0 instruction hunks.
- parked 16 normal alias before trig — MASKED_MATCH_PENDING_AUDIT, 259 v 259 bytes, 0 instruction hunks.
- parked 17 direct axis address — DIFFER, 259 v 259 bytes, 2 instruction hunks, first unequal byte at 0x00A55FCA.
- parked 18 canonical angle scale — MASKED_MATCH_PENDING_AUDIT, 259 v 259 bytes, 0 instruction hunks.

The selected result is RELOCATION_MATCH (259 object bytes, 259 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
