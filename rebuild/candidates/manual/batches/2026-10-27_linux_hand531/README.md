# Chunk 531 (agent531, 2026-10-28): 11 of 20 re-queued single functions landed (228-505 bytes)

Re-queue chunk of 20 rows of 228-505 bytes (0x00BCF4F0..0x00E81B40): engine animated-mesh primitives, landscape meshes and
LOD, CMatrix3x4, libpk strings / audio queues, and quest scripts. One Opus 5.5 agent at high effort.

- **Landed:** 8 rows with `vland.py --land` (1 MATCH 0x00C1CDA0 CMatrix3x4::PreMultiply, 7 RELOCATION_MATCH), and 3 rows as
  grade `retail_asm` (payload_retail_asm.json): 0x00BF7DF0 CLandscapeBackgroundTreeNode::GetDesiredLODBand (inlined
  C3DBoundingBox::CalcShortestDistanceToPointSquared hand SSE kernel `maxps/minps/shufps` behind g_SSEAvailable; the box
  and point pointers are spilled and reloaded because the asm names them), 0x00C3FF90 LookupAzimuthElevation and
  0x00E81B40 CFishCompOrganiser::GetBiggestFishWeight (bare `fistp dword` helpers). Every test PASS under g++ and VC7.1;
  no RELOC_IDENTITY, CONFLICT or DUPLICATE.
- **Parked (5, differ/):** 0x00BCF4F0 Initialise (store/reload order), 0x00BE1C10 2DViewport::Update (store order),
  0x00BE2B70 GetMappingDirectionBlend (one unused frame slot), 0x00BEEA00 `__adjust_heap` (interleaved pair copy),
  0x00C0C270 CString::ConcatInPlace (VC7.1 inline budget: retail inlines 7 of 10 GetData calls).
- **Skipped (4):** 0x00C174A0 (libpk LTCG register convention with EH and /GS), 0x00C2E610 (libpk EH, `and esp,-8`, class
  unproven), 0x00C43B30 and 0x00CAFA10 (not attempted).
- **Stand-in callee bodies (not retail, stay unlanded):** BuildSharedIndexBufferGenerationTables 0x00BE2CD0 in 0x00BE3110,
  Compare 0x00411570 in 0x00E3AC70, RoundHalfDownToLong 0x00C3FF70 in 0x00C3FF90.

Name fixes (NAME_FIXES.tsv, 15 rows, deferred): 0x00E80D60 PlayCutscene, 0x00E81A60 CheckForRod, 0x00BEE420
`std::__push_heap`, 0x00BD8D00 / 0x00BD8A10 STLport `_Rb_tree::operator=` / `_M_erase`, 0x006CADE0
`vector<CCompositeBlendDesc>::operator=`, CCharString / CWideString `operator=` return const references (0x0099EFB0 has 24
landed spellings). Competing global spellings to settle: 0x01436EA8, 0x01436EA0, 0x0143BB2C, 0x0143BB58.
