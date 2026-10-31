# retry/sec6b -- parked-row retry sweep, wave 2 section 6b (99 rows: the 88 section-6 rows not reached + the 11 section-5b rows
not opened): 7 of 99 landed; the sweep is complete

Landed 2026-10-31 (vland --land 7/7 RELOCATION_MATCH + PASS, multi-check 0): CString::ConcatInPlace 0x00C0C270 (C++; a
`__forceinline` accessor plus `inline_depth(0)` regions pick the out-of-line call sites) and six retail_asm rows:
CEngineLightingManager::LookupSmallObjectLightCache 0x00B46A30 (bare `fistp dword` floor helper; the box constructor assigns
its members one by one so both vector temporaries survive) and five rows around C3DBoundingBox::CalcShortestDistanceToPointSquared's
hand SSE kernel (pointers homed and reloaded into fixed xmm registers behind g_SSEAvailable, the landed 0x00BF7DF0 form):
0x00B49810 CalcShortestDistanceToPointSquared, 0x00B9C500 CalcShortestDistanceToPoint (respelled: the fsqrt twin), 0x00B4CED0 and
0x00B9C2E0 CShadowClipVolume::ClipBoundingBox (bool& and one-argument overloads), 0x00B99BB0
CEngineInternalPrimitiveRepeatedStaticMesh::IsInsideFrustum. Every `// Retail-asm:` range was disassembled from the retail bytes
before landing (the 0x00B99BB0 range starts one byte into its first instruction; harmless).

84 of 99 attempted: 29 improved (pending/.../differ/, e.g. 00cddcb0 63 -> 14 lines with forceinline dtors, 00bfad90/00bfa2a0
574 -> 104, DecalCollides 00b1d550 first full body 546v576), 48 held (register allocation, exit-cleanup tail merging, LTCG
cross-function effects, frame-slot sharing, outlined placement new, folded constant products), 15 not reached (four
CPSCDecalRenderer/00d9c680 stubs of 0.9-2 KB and the 11 section-5b rows: 00a1a260 00a1b5a0 00a1d6e0 00a475c0 00a79b30 00a7a8d0
00ab4bb0 00acc490 00ace600 00a87960 00a23190) -- all return to the chunk pool. 3 NAME_FIXES (CalcShortestDistanceToPoint,
CLocalDetailPrimitiveBase::ClipPrimitive, CLocalDetailPrimitiveMeshZSpriteBatch::Load) + 8 review rows queued (prefix sec6b;
IsInsideFrustum x3 modelled as __fastcall free functions, 0x01436E80 nine spellings, the stale ConsumeMapBrush binding at
0x00C0C270 retired here, sec5b's six SSE rows now have a landed precedent). 8 BOUNDS_FIX rows (OVERLONG catalog extents cut at
the ret). Labels: ghidra_out/labels_parked_retry2_sec6b_2026-10-31.tsv. Header debt: the `#define Class ClassLayout` rename
lever (00b277a0, 00b30b50, 00be6880, 00bfad90, 00b46a30) marks engine/ headers that lack the method declarations.
