# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 ported header — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 02 group entire origin plus perp — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 03 group perp scale — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 04 group radius formal — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 05 named scaled perp — DIFFER, 682 v 693 bytes, 41 instruction hunks, first unequal byte at 0x006981B9.
- parked 06 named first test pos — DIFFER, 732 v 693 bytes, 20 instruction hunks, first unequal byte at 0x00698172.
- parked 07 named origin reference — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 08 group both normalise calls — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 09 compare using negative guard — DIFFER, 688 v 693 bytes, 14 instruction hunks, first unequal byte at 0x00698218.
- parked 10 declare using reference in condition — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 11 calculate passes from positive side — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 12 add xy group — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 13 scale x paren — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 14 subtract x group — DIFFER, 693 v 693 bytes, 12 instruction hunks, first unequal byte at 0x006981BA.
- parked 15 initialise add — DIFFER, 725 v 693 bytes, 20 instruction hunks, first unequal byte at 0x00698218.
- parked 16 add named result — DIFFER, 725 v 693 bytes, 20 instruction hunks, first unequal byte at 0x00698218.
- parked 17 scale named result — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 18 initialise scale — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.
- parked 19 subtract xy group — DIFFER, 693 v 693 bytes, 12 instruction hunks, first unequal byte at 0x006981BA.
- parked handoff — DIFFER, 693 v 693 bytes, 10 instruction hunks, first unequal byte at 0x00698299.

The selected result is PARKED (693 object bytes, 693 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
