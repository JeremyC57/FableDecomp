# Compiler probes

- package baseline — COMPILE_FAIL.
- parked 01 ported header — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 02 cross grouped pass23 — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 03 cross xy group — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 04 cross xz group — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 05 cross named result — DIFFER, 716 v 716 bytes, 9 instruction hunks, first unequal byte at 0x008373E3.
- parked 06 subtract x group — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 07 subtract xy group — DIFFER, 716 v 716 bytes, 5 instruction hunks, first unequal byte at 0x0083727D.
- parked 08 subtract named result — DIFFER, 787 v 716 bytes, 11 instruction hunks, first unequal byte at 0x00837249.
- parked 09 normalise square helper — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 10 using named cross method — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 11 using cross by components — DIFFER, 714 v 716 bytes, 5 instruction hunks, first unequal byte at 0x008372D0.
- parked 12 using cross grouped xz — DIFFER, 714 v 716 bytes, 5 instruction hunks, first unequal byte at 0x008372D0.
- parked 13 using forward const ref — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 14 using right const ref — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 15 using explicit branch return — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked 16 using cross and assignment — DIFFER, 744 v 716 bytes, 15 instruction hunks, first unequal byte at 0x008372D0.
- parked 17 original peek pos — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.
- parked handoff — DIFFER, 716 v 716 bytes, 3 instruction hunks, first unequal byte at 0x008373EC.

The selected result is PARKED (716 object bytes, 716 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
