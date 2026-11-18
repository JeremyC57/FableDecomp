# Compiler probes

- package baseline — PREPROCESS_FAIL.
- parked 01 new validation header — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- parked 02 default definition pointer — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- parked 03 appearance check if and call — DIFFER, 407 v 439 bytes, 28 instruction hunks, first unequal byte at 0x00680061.
- parked 04 appearance check named pointer — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- parked 05 dummy check split conditions — DIFFER, 407 v 439 bytes, 36 instruction hunks, first unequal byte at 0x00680052.
- parked 06 dummy check reference stats — DIFFER, 402 v 439 bytes, 29 instruction hunks, first unequal byte at 0x00680052.
- parked 07 dummy check copy stats — DIFFER, 402 v 439 bytes, 29 instruction hunks, first unequal byte at 0x00680052.
- parked 08 dummy persist property check switch — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- parked 09 dummy check qualified has object — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- parked 10 release pointer in block — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- parked 11 mesh index nested guards — DIFFER, 434 v 439 bytes, 8 instruction hunks, first unequal byte at 0x00680090.
- parked 12 dummy test result consumed — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- parked handoff — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.

## Second review

After HasDummyObject, retail reloads pappearance_def.Object into edi at 0x00680196 and the conditional temporary-cleanup bitmask into eax at 0x0068019A. The selected compiler build reloads those independent values in the opposite order.

- tight second 01 dummy object macro equivalence — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 02 stat pointer direct getter — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 03 negated validation failure path — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680158.
- tight second 04 validate macro shape — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 05 any weapon property before dummy — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680158.
- tight second 06 dummy name bound to canonical string — DIFFER, 434 v 439 bytes, 4 instruction hunks, first unequal byte at 0x00680158.
- tight second 07 cond expr dummy skips empty mesh — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 08 cond expr dummy check failure — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 09 explicit global crc argument — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 10 dummy pointer fn argument scoped — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 11 anim filter visible string lifetime — DIFFER, 427 v 439 bytes, 12 instruction hunks, first unequal byte at 0x006800CD.
- tight second 12 anim and dummy conditions in void — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 13 release using original inline chain — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 14 dummy stat const ref — COMPILE_FAIL.
- tight second 15 dummy check as direct method no not — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 16 appearance ref scoped macro — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 17 dummy mesh stats named in conditional — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.
- tight second 18 dummy check two typed qualified calls — DIFFER, 439 v 439 bytes, 2 instruction hunks, first unequal byte at 0x00680197.

The selected result is PARKED (439 object bytes, 439 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
