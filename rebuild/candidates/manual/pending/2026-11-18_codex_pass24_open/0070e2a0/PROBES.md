# Compiler probes

- package baseline — PREPROCESS_FAIL.
- parked 01 ported header — COMPILE_FAIL.
- parked 02 counted include — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.
- parked 03 second blend ref temp — DIFFER, 616 v 618 bytes, 20 instruction hunks, first unequal byte at 0x0070E31C.
- parked 04 second blend named value — DIFFER, 616 v 618 bytes, 20 instruction hunks, first unequal byte at 0x0070E31C.
- parked 05 first blend explicit scope — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.
- parked 06 initial blend argument reference — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.
- parked 07 scope peek tc nested — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.
- parked 08 morph use early guard — DIFFER, 618 v 618 bytes, 9 instruction hunks, first unequal byte at 0x0070E32A.
- parked 09 morph use bool result — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.
- parked 10 morph assign local reference — DIFFER, 610 v 618 bytes, 17 instruction hunks, first unequal byte at 0x0070E2A2.
- parked 11 scope each end blend assignment — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.
- parked 12 morph converted const value — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.
- parked 13 morph peek gettc original chain — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.
- parked handoff — DIFFER, 618 v 618 bytes, 7 instruction hunks, first unequal byte at 0x0070E32A.

The selected result is PARKED (618 object bytes, 618 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
