# NF151 header overlay

## engine/CRenderStateManager.h
Base: the repo working-tree copy at slice start (orig/engine/CRenderStateManager.h, byte-equal).
- Added opt-in `FABLE_RETAIL_CRENDERSTATEMANAGER_ISSOFTWAREPROCESSINGENABLED_INLINE` (retail branch, public block, in place of the bare
  `bool IsSoftwareProcessingEnabled();` declaration, which stays verbatim in the `#else`): the inline body
  `return DeviceState.SoftwareVertexProcessing.CurrentState != 0;`.
  Evidence: FableWin.pdb `?IsSoftwareProcessingEnabled@CRenderStateManager@@QAE_NXZ` (0x03031E10, placed in the CShaderRenderManager group between
  ApplyVertexShader 0x03031510 and DisableVertexShaders 0x030321C0; no Ego_r.pdb or Fable.exe copy, so inline in release) + retail 0x00988042
  `mov edx,[ecx+2908h] ; test edx,edx ; setne dl` (core +0x2908 = DeviceState +0x2818 + SoftwareVertexProcessing +0xF0, CStateInfo::CurrentState +0).
  Used by: rows/00988020.cpp (and its test). No existing landed row expands the +0x2908 read under another spelling (grep).
- Default path token-identical (the opt-in macro is defined nowhere else). pin_asserts.py: pinned 0 (no FABLE_STATIC_ASSERT moved).
- Header users: 30 landed rows include engine/CRenderStateManager.h (`grep -rlE 'CRenderStateManager\.h' rebuild/src/compiled`); none defines the
  new macro, so the opt-in changes none of them.

## engine/CEngineLightingManager.h
Base: repo working-tree copy at slice start (orig/engine/CEngineLightingManager.h, byte-equal to the repo now).
- Added opt-in `FABLE_RETAIL_CENGINELIGHTINGMANAGER_ADDLIGHTPRIMITIVE`: a forward declaration `class CEngineInternalPrimitiveLight;` (after
  `class C3DBoundingBox;`) and, after `UpdateGlobalLightingStates()`, the public declaration
  `void AddLightPrimitive(const CEngineInternalPrimitiveLight& light);` (Ego_r.pdb 0x009BD400 / FableWin.pdb 0x02CEFE10
  `?AddLightPrimitive@CEngineLightingManager@@QAEXABVCEngineInternalPrimitiveLight@@@Z`, retail 0x00B47FD0). Declaration only, no layout change.
  Used by rows/00b47fd0.cpp (+ its test).
- Default path token-identical. pin_asserts.py: pinned 0.
- Header users: 75 landed rows include engine/CEngineLightingManager.h; none defines the new macro, so none changes.

## engine/CEngineInternalPrimitiveLight.h
Base: repo working-tree copy at slice start (orig/engine/CEngineInternalPrimitiveLight.h, byte-equal to the repo now).
- Added opt-in `FABLE_RETAIL_CENGINEINTERNALPRIMITIVELIGHT_TYPED`: includes engine/C3DVector.h + engine/CRGBFloatColour.h, and replaces the
  flattened WorldPos_X..Z / Colour_R..A floats by the Ego_r.pdb typed members `C3DVector WorldPos` (+0x48) and `CRGBFloatColour Colour` (+0x54)
  (same offsets, same size 0x70); the flattened members stay verbatim in the `#else`. The seven flattened-member asserts sit under
  `#ifndef` the macro, with two typed asserts (WorldPos / Colour offsets) in the `#else` arm. Used by rows/00b47fd0.cpp (+ its test).
- pin_asserts.py pinned 12: every bare `FABLE_STATIC_ASSERT(...)` of this header is now `FABLE_STATIC_ASSERT_AT(<its original line>, ...)`
  (45..56), so the opt-in lines move no assert; apart from that pinning the default path is token-identical.
- Header users: 1 landed row includes it (0x00B453D0 `CEngineInternalPrimitiveLight_Update`... grep hit on its name only; it models the class
  locally and does not include the header); none defines the macro.
