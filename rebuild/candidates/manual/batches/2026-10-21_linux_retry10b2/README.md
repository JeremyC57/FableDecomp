# Retry 10b2 + chunk 249 fix-up (2026-10-21)

- 0089bdf0 EntitySetFacingAngle (retry B): `float cosPitch = (float)cos(g_zeroPitch);` fixed the x87 lifetime. Landed.
- 0098a9a0 CreatePixelShader (chunk 249): refused at the chunk landing on RELOC_IDENTITY (operator delete in the same-TU growth stand-in vs the CRT free thunk 0x00BFEA14); the coordinator rewrote the two vector-storage releases as free() in source and test, re-graded RELOCATION_MATCH and landed it here.
- 00567d00, 00a5b260, 009a7ca0: the int3-after-noreturn grader artefacts; verify_and_land.py refuses them on the one padding byte (90v89, 265v264, 145v144) as expected. They stay parked (retry index), sources in the retry B workspace.
