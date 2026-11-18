# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 ported header — DIFFER, 777 v 777 bytes, 8 instruction hunks, first unequal byte at 0x00A3C4FA.
- parked 02 index store after lerps — DIFFER, 777 v 777 bytes, 48 instruction hunks, first unequal byte at 0x00A3C3BF.
- parked 03 index store between lerps — DIFFER, 777 v 777 bytes, 36 instruction hunks, first unequal byte at 0x00A3C48C.
- parked 04 lerp weight before time — DIFFER, 777 v 777 bytes, 25 instruction hunks, first unequal byte at 0x00A3C4C7.
- parked 05 lerp wrong cse reverse construction — DIFFER, 777 v 777 bytes, 22 instruction hunks, first unequal byte at 0x00A3C48C.
- parked 06 lerp group time value — DIFFER, 777 v 777 bytes, 9 instruction hunks, first unequal byte at 0x00A3C4FA.
- parked 07 lerp group weight value — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C57A.
- parked 08 lerp single expression helper — DIFFER, 777 v 777 bytes, 10 instruction hunks, first unequal byte at 0x00A3C4FA.
- parked 09 lerp vector access plain — DIFFER, 777 v 777 bytes, 8 instruction hunks, first unequal byte at 0x00A3C4FA.
- parked 10 lerp group old values — DIFFER, 777 v 777 bytes, 8 instruction hunks, first unequal byte at 0x00A3C4FA.
- parked 11 lerp float local — DIFFER, 777 v 777 bytes, 8 instruction hunks, first unequal byte at 0x00A3C4FA.
- parked 12 lerp math expressions — DIFFER, 741 v 777 bytes, 40 instruction hunks, first unequal byte at 0x00A3C48C.
- parked 13 lerp group time and weight — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C576.
- parked 14 lerp group time and weight index — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C576.
- parked 15 lerp group weight phase only — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C57A.
- parked 16 lerp group weight receiver and index — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C57A.
- parked 17 lerp group weight change step order — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C57A.
- parked 18 lerp group weight reference value — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C57A.
- parked 19 lerp group weight named time — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C576.
- parked 20 lerp group weight named blend count — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C57A.
- parked 21 lerp group weight named index local — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C57A.
- parked 22 lerp group weight time separated — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C576.
- parked handoff — DIFFER, 777 v 777 bytes, 3 instruction hunks, first unequal byte at 0x00A3C57A.

The selected result is PARKED (777 object bytes, 777 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
