# retry/sec5b -- parked-row retry sweep, wave 2 section 5b (the 39 section-5 rows that got no graded edit): 5 of 39 landed

Landed 2026-10-31 (vland --land 5/5 RELOCATION_MATCH + PASS, multi-check 0): 0x00AF90C0, 0x00AFB7C0, 0x00B00DB0, 0x00B0FBF0,
0x00B0FFC0, all grade retail_asm -- C++ bodies around the engine's inline `fld dword / fistp dword` float-to-long helper, each
`// Retail-asm:` claim verified against the retail bytes before landing. 28 rows attempted, 16 improved (pending/.../differ/,
e.g. CLipSyncSampleData::ReadFromFile 257 -> 11 diff lines, CPSCSingleSprite::DatabaseSerialise 565 -> 44), 7 held (6 are SSE
rows whose only matching form is inline asm: intrinsics can emit the instructions, so by the owner rule they are not retail_asm
and stay parked in pending/.../retail_asm_review/ with the asm oracles as evidence), 11 unopened for budget -> appended to section
6b (`$S/retry/sec6b`). 13 NAME_FIXES (+ REVIEW 0x009D5060) and 6 review rows queued (prefix sec5b); 23 oracle_extra rows,
BOUNDS_FIX rows recorded as MISSING lengths. Labels: ghidra_out/labels_parked_retry2_sec5b_2026-10-31.tsv.
