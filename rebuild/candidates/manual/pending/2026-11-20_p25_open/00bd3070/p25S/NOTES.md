# 0x00BD3070 CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive -- p25S (2026-11-21)
Base: acctC wfE7 best (pending/2026-11-16_acctC_open/wfE7/00bd3070.cpp, e17/s10C1Gn16) was SRC_FAIL on HEAD: Codex pass22 restructured
CEngineSubPrimitiveAnimatedMesh / CEngineInternalPrimitiveAnimatedMesh / CEngineInternalPrimitiveMorphInfo (FABLE_RETAIL_PASS22_*), and
6 of the wfE7 hdrdiffs no longer apply (fuzz-applying them inserts duplicates of blocks HEAD already has).
Re-based (overlay hdr/, hdr.diff, add-only, asserts pinned: pin_asserts reports 0 moved):
- ported by hand: C3DMeshMaterial GETDECALID; CEngineInternalPrimitiveAnimatedMesh ACCESSORS (inside the PASS22 class); CEngineInternalPrimitiveBase
  ALPHAFADE; CEngineLightingManager SPOTLIGHT_REFLECTION (moved inside the class, next to SetupShaderSpotLight(const CMatrix3x4&)) + FADEDSHADOW_MODE_CHECK;
  CEnginePrimitiveRenderUtil ALPHAMASKTEXTUREMODE; CEngineScreenEffectOutlineGlow DEPTHTRANSFORM; CGraphicInfo GETTRANSPARENCYTYPE + IsAlpha; CGraphicInfoFrame ISALPHA.
- NEW opt-ins on HEAD's pass22 headers (replacing acctC's NEW_ headers, which conflict): CEngineSubPrimitiveAnimatedMesh.h
  FABLE_RETAIL_CENGINESUBPRIMITIVEANIMATEDMESH_ACCESSORS (GetParent / PeekParent / PeekObjectTransform decl; FableWin names),
  CEngineInternalPrimitiveMorphInfo.h FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_GETTEXTUREDECAL (+ _FORCEINLINE; wfE7 text, FableWin 60i/204b).
- row: + FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER, CENGINESUBPRIMITIVEANIMATEDMESH_ACCESSORS, CENGINESUBPRIMITIVEMESHBASE_ACCESSORS (HEAD's GetScale body
  replaces the row-local one, dropped), _MESHBASE_PUBLICMEMBERS, _MORPHINFO_GETTEXTUREDECAL.
Grades (default flags):
- b3.cpp (acctC text + the re-base, 16 fitted asserts kept): DIFFER(11288v11288), 36 g.py hunks; the same tie set acctC reported (342a/3491/34fc/3567
  load order, 4c30 test/or, 5003, 5712/5718/5889 SIB) + 32f4 / 3960 / 44f0 / 5ad8 x87 and order hunks (the jump-table / reloc-mask hunks are artefacts).
- b4.cpp (the 16 fitted asserts REMOVED, as the 2026-11-21 ruling requires): DIFFER(11256v11288), 250 hunks.
- b5.cpp (3 asserts, the count FableWin's size allows): DIFFER(11256v11288), 252 hunks.
  Without the fitted asserts the length drops 32 B at one place: retail 0x00BD467E duplicates the AddStateToUpdateList tail
  (mov eax,[g]; mov edx,[eax+2948h]; test; lea ecx,[eax+2944h]; je; ...; jmp) where ours cross-jumps (`jmp L1cb0`). Every other hunk is a
  numbering tie (test/or orientation, load order, x87 order). The cross-jump also flips on the assert count alone (0..3 -> 11256, 18..24 ->
  11288 per wfE7), so it is probably a numbering effect, not a structural difference.
- b6.cpp = RULE-COMPLIANT BASE for a successor: b4 (no row asserts) + the LANDED HEAD opt-in FABLE_RETAIL_CARRAY_INDEX_ASSERT (dead bounds
  asserts in CArray's operator[], FableWin's out-of-line debug indexers; wfE7's H0 idea with HEAD's text): DIFFER(11288v11288), 62 g.py hunks
  (fitted-assert b3: 36). Equal length, so the 0x00BD467E cross-jump is gone. Work the remaining ties from b6, not b3.
- The overlay's assert lines are unchanged from HEAD (`diff HEAD hdr7 | grep ASSERT` is empty).
Still open beyond the ties: ~15 row-local FABLE_FORCEINLINE definitions of members that shared headers model (PeekGraphic, SetTextureFactor,
SelectVertexBuffer, FlushRender, ApplyPixelShader, GetCurrent2DAlphaMaskInfo, DisableTextureFiltering, Set2DAlphaMaskTextureMode,
GetAlphaIncludingDistanceFade, GetAppropriateMipmap, SetupAlphaBlendingStatesFromRenderFlags, GetTexture) must move into those headers as add-only
opt-ins. Moving them changes c1 symbol creation order, so expect the ties to move.
