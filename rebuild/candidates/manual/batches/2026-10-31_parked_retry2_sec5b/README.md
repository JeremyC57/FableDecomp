# retry/sec5b -- parked-row retry sweep, wave 2 section 5b (the 39 section-5 rows that got no graded edit): 7 of 39 landed

Landed 2026-10-31 (vland --land 5/5 RELOCATION_MATCH + PASS, multi-check 0): 0x00AF90C0, 0x00AFB7C0, 0x00B00DB0, 0x00B0FBF0,
0x00B0FFC0, all grade retail_asm -- C++ bodies around the engine's inline `fld dword / fistp dword` float-to-long helper, each
`// Retail-asm:` claim verified against the retail bytes before landing. 28 rows attempted, 16 improved (pending/.../differ/,
e.g. CLipSyncSampleData::ReadFromFile 257 -> 11 diff lines, CPSCSingleSprite::DatabaseSerialise 565 -> 44), 7 held (6 are SSE
rows whose only matching form is inline asm: intrinsics can emit the instructions, so by the owner rule they are not retail_asm
and stay parked in pending/.../retail_asm_review/ with the asm oracles as evidence), 11 unopened for budget -> appended to section
6b (`$S/retry/sec6b`). 13 NAME_FIXES (+ REVIEW 0x009D5060) and 6 review rows queued (prefix sec5b); 23 oracle_extra rows,
BOUNDS_FIX rows recorded as MISSING lengths. Labels: ghidra_out/labels_parked_retry2_sec5b_2026-10-31.tsv.

SSE follow-up 2026-10-31 (owner call: land the SSE rows where retail itself ran inline asm, the 0x00BF7DF0 form): the two
byte-exact review rows landed as retail_asm (`retail_asm_payload.json` / `retail_asm_oracle.tsv`, vland --land 2/2
RELOCATION_MATCH + PASS, multi-check 0) -- CMatrix4x4::AlignedSetInterpolate 0x00A5CEE0 (285) and
CParticleProgram::TransformVertices 0x00AE0E60 (457). Both `// Retail-asm:` claims were disassembled from the retail bytes
before landing: 0x00A5CF04 `mov [esp+0Ch],ecx` homes `this`, 0x00A5CF10..16 reload eax/edx/ecx right before the fixed
xmm0..xmm7 block; 0x00AE0F76/7E spill the register-held source/target, 0x00AE0F86..A3 reload eax/edx and run the fixed
xmm0..xmm2 block. Intrinsics neither home nor reload a register pointer nor fix the xmm numbering (probes 301v285 and 445v457).
TransformVertices's binding respelled to the Ego_r.pdb/FableWin.pdb spelling (private const, `const CArray<C3DGroup2>&`);
caller 0x00AE1260 PrepareForRendering respelled to match and rechecked. The other seven retail_asm_review/ rows
(0x00989230, 0x00A9D480, 0x00A9D580, 0x00A9D750, 0x00ADF960, 0x00AE1320, 0x00AE29D0) are still DIFFER and stay parked;
the same retail-asm evidence applies to them once their bodies match.
