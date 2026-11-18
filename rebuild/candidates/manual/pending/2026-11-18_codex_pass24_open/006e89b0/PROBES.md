# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 header body — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- parked 02 preferred branch return named — DIFFER, 354 v 344 bytes, 8 instruction hunks, first unequal byte at 0x006E8A36.
- parked 03 preferred branch return ref — DIFFER, 354 v 344 bytes, 8 instruction hunks, first unequal byte at 0x006E8A36.
- parked 04 preferred branch float as ref — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- parked 05 preferred branch result components — DIFFER, 342 v 344 bytes, 5 instruction hunks, first unequal byte at 0x006E8A7A.
- parked 06 preferred branch result component y group — DIFFER, 342 v 344 bytes, 5 instruction hunks, first unequal byte at 0x006E8A7A.
- parked 07 preferred branch result component z group — DIFFER, 342 v 344 bytes, 5 instruction hunks, first unequal byte at 0x006E8A7A.
- parked 08 preferred branch result components x z group — DIFFER, 342 v 344 bytes, 5 instruction hunks, first unequal byte at 0x006E8A7A.
- parked 09 preferred branch inplace operand — DIFFER, 354 v 344 bytes, 8 instruction hunks, first unequal byte at 0x006E8A36.
- parked 10 preferred branch ret no local dist — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- parked 11 preferred branch copy with member assign — DIFFER, 352 v 344 bytes, 11 instruction hunks, first unequal byte at 0x006E8A36.
- parked 12 preferred branch use second scaled const ref — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- parked handoff — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.

## Second review

In the preferred-distance branch, retail multiplies prong.X by the distance at 0x006E8A7A before restoring esi at 0x006E8A7C. The selected compiler build restores esi first.

- tight second 01 vector scale x group — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 02 vector scale native named result — DIFFER, 334 v 344 bytes, 8 instruction hunks, first unequal byte at 0x006E8A36.
- tight second 03 vector scale initialise named result — DIFFER, 334 v 344 bytes, 8 instruction hunks, first unequal byte at 0x006E8A36.
- tight second 04 preferred scale grouped argument — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 05 preferred scale grouped result — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 06 preferred scale explicit operator — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 07 preferred float separation const ref — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 08 preferred distance return group — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 09 fallback distance parenthesized product — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 10 occupied prong implicit ctor — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 11 preferred combatdef scope — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 12 prefer const prong value — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 13 preferred helper original fastcall — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 14 preferred helper inline fastcall — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 15 preferred helper fastcall with scale group — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 16 preferred scale direct components and ref — DIFFER, 342 v 344 bytes, 5 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 17 preferred scale float x ref components — DIFFER, 342 v 344 bytes, 5 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 18 preferred scale uses outer radius block — DIFFER, 350 v 344 bytes, 6 instruction hunks, first unequal byte at 0x006E8A2A.
- tight second 19 preferred ring threshold reference — DIFFER, 349 v 344 bytes, 3 instruction hunks, first unequal byte at 0x006E8A32.
- tight second 20 preferred threshold named bool — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 21 preferred helper combines distance and separation — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.
- tight second 22 preferred helper explicit fallback span — DIFFER, 344 v 344 bytes, 2 instruction hunks, first unequal byte at 0x006E8A7A.

The selected result is PARKED (344 object bytes, 344 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
