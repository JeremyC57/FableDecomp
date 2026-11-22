# 0x00A68DE0 LOG (AUTH23)
0. Recorded draft (r0 = pending/2026-11-13_codex_released/differ/00a68de0.cpp): not gradable. cl "hangs" because the draft includes engine/CVolumeTextureFileInfo.h,
   which never landed: on this lane ANY missing #include makes cl.exe hang instead of printing C1083 (mini/h2.cpp, a 2-line TU; h0.cpp control compiles in 0.2 s). Not a grade.
1. p1 = fresh port on today's headers, Ego_d locals / statement order (see NOTES): DIFFER(939v919).
2. p2 = p1 + countdown cz / cy loops from the box dimensions (retail loads each bound once: 0x00A690DF / 0x00A690E7) + `X * GetByteColourDepth()` operand order for line_length (retail keeps X in ebx across the call):
00a68de0 CVolumeTexture::LoadFromDataStream DIFFER(923v919) flags= []
3. p3 = p2 + X as the LEFT operand of overflow_check (Ego_d line 652 = 53 B only with X first: od/v1.cpp variants A / E; ours was 51) + Y * Z order in overflow_check2 / data_length:
00a68de0 CVolumeTexture::LoadFromDataStream DIFFER(923v919) flags= []
4. Pre-registered diagnostic: p3 under /Oa (retail keeps X in ebx and Z in edi across real calls and re-reads PMem from the box after the copy, i.e. treats the box as unaliased; /Oa is a known TU flag of 42 landed rows). One grade:
00a68de0 CVolumeTexture::LoadFromDataStream DIFFER(946v919) flags= ['/Oa']
   /Oa result: frame 0xB4, the inline Read caches the stream fields across `rep movs` (retail re-reads them), X still a memory operand. Rejected; not retail's flag.
Not grades: VC10 /Od line sizes (odall10, p1 / p2 / od/v1.cpp), mini TUs (mini/al.cpp, cse.cpp, kill.cpp, k2.cpp, k3.cpp, h0..h2.cpp).
Grades used: 4 (p1, p2, p3, p3 /Oa). Best: p3 DIFFER(923v919), 43 masked lines. ESCALATE.md written.
5. Pre-registered candidate (advisor pointer to the landed 2D twin 0x009FB750, which carries its same-TU Lock body): p4 = p3 + the landed text of CVolumeTexture::Lock 0x00A685E0 (and its GetDimensions) in the TU before the function + Ego_d-style count-up cz / cy loops. Expectation: locked_box becomes unaliased (X in ebx, Z in edi, countdown from one load).
00a68de0 CVolumeTexture::LoadFromDataStream DIFFER(911v919) flags= []
6. p5 = p4 with Z * Y operand order (p1's) in overflow_check2 / data_length: 00a68de0 CVolumeTexture::LoadFromDataStream DIFFER(911v919) flags= []
Grades used: 6 (p1, p2, p3, p3 /Oa, p4, p5). Best: p4 DIFFER(911v919), 41 masked lines; ESCALATE.md UPDATE section.
