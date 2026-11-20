# NF134 header overlay (2026-11-20, round 10, PRIORITY slice 9 third pass)

Base copies of every overlaid header are in ../orig/ (repo working tree at the start of this pass). Every change is an `#ifdef` opt-in
with a member-specific macro; with the macro undefined the header is token-identical to the base copy (the only other difference is the
pin_asserts.py rewrite of FABLE_STATIC_ASSERT to FABLE_STATIC_ASSERT_AT at the base line numbers).

## engine/CVertexShaderConstantLayout.h
- Added `FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT_GETLIGHTARRAY_INLINE` (row 0x0098B5E0 CShaderRenderManager::SetVertexShaderConstantLayout):
  an inline body `CShaderConstantRange GetLightArray() const { return LightArray; }` in place of the declaration (FableWin.pdb 0x030312A0
  ?GetLightArray@CVertexShaderConstantLayout@@QBE?AVCShaderConstantRange@@XZ; retail expands it: the LightArray copy, the
  ??_7CShaderConstantRange@@6B@ store and the ~CBaseClass 0x0099A2E0 call of the temporary in 0x0098B5E0). Pair it with
  FABLE_RETAIL_CSHADERCONSTANTRANGE_IMPLICIT_COPY.
- pin_asserts.py: pinned 28 asserts (every FABLE_STATIC_ASSERT of the header, at its base line number).
- Landed includers: 19 rows in rebuild/src/compiled include this header directly (CEngineOcclusionManager_RenderOcclusionBox_00b8d420.cpp, CEngineVSConstantLayoutBasic_CEngineVSConstantLayoutBasic_00bdbb70.cpp, CShaderRenderManager_SetShadowFadeFactors_00989960.cpp, CShaderRenderManager_SetShadowFadeTransform_00989910.cpp, CShaderRenderManager_SetShadowSpotLightFallloffParameters_009899b0.cpp, CShaderRenderManager_SetShadowTextureTransform_009896b0.cpp, CShaderRenderManager_SetUserVertexShaderConstant_00989a60.cpp, CShaderRenderManager_SetUserVertexShaderConstant_00989ab0.cpp, CShaderRenderManager_SetUserVertexShaderConstant_00989b00.cpp, CShaderRenderManager_SetUserVertexShaderConstant_00989b50.cpp, CShaderRenderManager_SetUserVertexShaderConstants_009896d0.cpp, CShaderRenderManager_UpdateCameraPos_00989700.cpp, CShaderRenderManager_UpdateObjectSpaceFogTransform_00989540.cpp, CShaderRenderManager_UpdateWorldSpaceFogTransform_00989640.cpp, CShaderRenderManager_UpdateWorldSpaceTextureTransform_009894b0.cpp, CShaderRenderManager_UpdateWorldTransform_00988e30.cpp, CVertexShaderConstantLayout_CVertexShaderConstantLayout_0098d490.cpp, CVertexShaderConstantLayout_CVertexShaderConstantLayout_0098d710.cpp, StaticInitialization_InitializeCUnprovenGlobal013CA8A8_01224140.cpp) (more through CShaderRenderManager.h and the
  CEngineVSConstantLayout*.h headers). None defines the new macro (grep of rebuild/ finds GETLIGHTARRAY_INLINE nowhere), so the opt-in
  changes none of them. No other agent (NF132, NF135, NF136) overlays this header.
