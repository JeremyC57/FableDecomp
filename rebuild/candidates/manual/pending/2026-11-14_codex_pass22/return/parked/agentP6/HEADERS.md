# agentP6 header overlay (w22, 2026-11-15) -- mirrors rebuild/include

## What it is
Codex pass 22's six parked rows were graded by Codex against frozen PRIVATE header profiles (codex zip
`history/header_profiles/<BUILD.json header_profile>.json`) that were never merged into the repo ("The six unfinished rows have
additional private changes; do not blindly merge them"). Under the repo headers at a53eb7a898 their sources therefore SRC_FAIL.
This overlay is those private changes merged onto the current repo headers, for 0x00B6FA40, 0x00B70230, 0x00BF0B20,
0x00F3E120 and 0x0064BFBA (0x00675E20 is NOT merged: its profile predates Codex overlay45 in several files -- merging it would
delete accepted text -- and its CCreatureDef.h / CCreatureStatsDef.h / NHeroInformationScreens_CBase.h / NInventory_CTCInventoryBase.h /
CEnginePrimitiveRenderUtil.h / fable_containers.h hunks conflict; its row still needs that work, see the agentP6 report).

How it was built (reproducible, tools in ../tools/):
- `mkovl.py <row> <out>`: for every file of the row's frozen profile that differs from Codex overlay45 (or from Codex's
  reference/include), `diff3 -m -E <current overlay or repo file> <base> <profile file>`, base = whichever of overlay45 /
  reference is closer to the profile (so a profile older than overlay45 never deletes accepted overlay45 text).
- rows merged in order water (00B6FA40 = 00B70230, identical profiles), 00BF0B20, 00F3E120, 0064BFBA, each on top of the last.
- conflicts resolved by hand (`resolve.py`, `fixf3.py`): C4DVector.h / CGraphicDataBank.h (an `#endif` before the repo text),
  CRGBFloatColour.h (Codex's formal-name branch nested in the repo's `#elif defined(FABLE_RETAIL_C4DVECTOR_ORIGINAL_COLOUR_CHAIN)`),
  CShaderRenderManager.h (Codex's PIXEL_CONSTANT_AT branch, spelled FABLE_FORCEINLINE), CArray.h (water's INDEX_VECTOR_CHAIN
  and 00F3E120's INDEX_BASE as one #ifdef/#elif chain ahead of the repo's FABLE_RETAIL_CARRAY branch), CTriangleStripifier2.h
  (00F3E120's complete-declaration class as `#ifdef FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS ... #else <repo text> #endif`).
- No bare `__forceinline`, `__asm`, `volatile` or `register` is added (checked line by line against the repo).
- Every FABLE_STATIC_ASSERT keeps its typedef name: `pincheck.py` (every assert's effective line / `_AT` pin equals the repo's) = 0
  mismatches; `pin_asserts.py` pinned 0 (Codex had pre-pinned the moved ones with FABLE_STATIC_ASSERT_AT).
- Default path: lane_merge/equiv_check.py over every landed row (gcc cpp of source + test, old tree = rebuild/include, new tree =
  rebuild/include + this overlay): see ../work/equiv.tsv (result in the agentP6 report).
- Grading: `FABLE_HDR_OVERLAY=<this dir>`; the two matched rows (final/00b6fa40.cpp, final/00b70230.cpp) are RELOCATION_MATCH +
  PASS under vland with LANDVERIFY_STRICT_HEADERS=1; the other three reproduce Codex's recorded grade exactly.

## Files (repo -> overlay line delta, opt-ins introduced)
- engine/C2DBoxF.h|+7 -0|FABLE_RETAIL_C2DBOXF_SETTLPOS 
- engine/C2DExtentsI.h|+15 -0|FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22 
- engine/C4DVector.h|+8 -0|FABLE_RETAIL_PASS22_WATER_ORIGINAL_COPY_COLOUR_FORMALS 
- engine/CArray.h|+58 -2|FABLE_RETAIL_CARRAY FABLE_RETAIL_CARRAY_AT FABLE_RETAIL_CARRAY_ORIGINAL_AT_VECTOR_INDEX_PASS22 FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22 FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22 FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_VECTOR_CHAIN_PASS22 FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22 
- engine/CBaseClassNonCopyable.h|+5 -1|FABLE_RETAIL_CBASECLASSNONCOPYABLE_ORIGINAL_ASSIGNMENT_RESULT_PASS22 
- engine/CEngineLightingManager.h|+4 -0|FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALDIFFUSE 
- engine/CEngineMeshEffect.h|+36 -0|FABLE_RETAIL_CENGINEMESHEFFECT_ORIGINAL_DECLARATIONS_PASS22 FABLE_RETAIL_PASS22_STATIC_MESH_RENDER 
- engine/CEngineOcclusionManager.h|+27 -1|FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22 
- engine/CEnginePrimitive2DSprite.h|+20 -1|FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_HIGHLIGHT_BODY 
- engine/CEngineScreenEffectOutlineGlow.h|+67 -39|FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_ORIGINAL_COMPLETE_PASS22 
- engine/CEngineWaterBackgroundSubPatch.h|+42 -21|FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22 
- engine/CEngineWaterRenderer.h|+350 -0|FABLE_RETAIL_CENGINEWATERRENDERER_SEABACKGROUND_BODIES FABLE_RETAIL_CENGINEWATERRENDERER_SEAMIDGROUND_BODIES FABLE_RETAIL_PASS22_WATER_ORIGINAL_COMPLETE_OWNER FABLE_RETAIL_WATER_OCCLUSION_ORIGINAL_CHAIN_PASS22 
- engine/CEngineWaterSettings.h|+79 -77|FABLE_RETAIL_PASS22_RESOURCE_ORIGINAL_SPECIAL_MEMBERS 
- engine/CGraphicDataBank.h|+80 -0|FABLE_RETAIL_CGRAPHICDATABANK_LOADGRAPHICFRAME FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22 
- engine/CGraphicFrame.h|+22 -0|FABLE_RETAIL_CGRAPHICFRAMEDATA_ORIGINAL_TEXTURE_GET_PASS22 FABLE_RETAIL_PASS22_WATER_ORIGINAL_COPY_COLOUR_FORMALS 
- engine/CGraphicInfo.h|+103 -1|FABLE_RETAIL_CFRAMEDATA_ISLOADING FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME_EARLYRETURN FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_FORCED_SUCCESS_SCOPE_PASS22 FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_GETFRAME_POINTER_GUARD_PASS22 FABLE_RETAIL_CGRAPHICINFOFRAME_ORIGINAL_TOUCHFRAME_POINTER_GUARD_PASS22 FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME FABLE_RETAIL_PASS22_RESOURCE_ORIGINAL_SPECIAL_MEMBERS 
- engine/CIVCountedPointer.h|+12 -1|FABLE_RETAIL_CIVCOUNTEDPOINTER_ORIGINAL_CASTTOBASE_PASS22 FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE 
- engine/CManagedTexture.h|+10 -0|FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME FABLE_RETAIL_PASS22_RESOURCE_ORIGINAL_SPECIAL_MEMBERS 
- engine/CManagedTextureData.h|+5 -0|FABLE_RETAIL_PASS22_RESOURCE_ORIGINAL_SPECIAL_MEMBERS 
- engine/CManagedTextureOwner.h|+3 -1|FABLE_RETAIL_PASS22_RESOURCE_ORIGINAL_SPECIAL_MEMBERS 
- engine/CMatrix3x4.h|+14 -0|FABLE_RETAIL_CMATRIX3X4_ENUM_CTOR 
- engine/CRGBColour.h|+40 -0|FABLE_RETAIL_CRGBCOLOUR_CURSOR_COPY_INLINE_PASS22 FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22 
- engine/CRGBFloatColour.h|+55 -0|FABLE_RETAIL_CRGBFLOATCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22 FABLE_RETAIL_PASS22_WATER_ORIGINAL_COPY_COLOUR_FORMALS 
- engine/CRenderManagerCore.h|+11 -0|FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES 
- engine/CRenderStateManager.h|+50 -0|FABLE_RETAIL_CRENDERSTATEMANAGER_ORIGINAL_FORMALS_PASS22 FABLE_RETAIL_PASS22_STATE_ORIGINAL_CAPTURE_STORE_SEQUENCE 
- engine/CShaderRenderManager.h|+21 -1|FABLE_RETAIL_PASS22_SHADER_ORIGINAL_PIXEL_CONSTANT_AT 
- engine/CTCBattleCharge.h|+7 -1|FABLE_RETAIL_CTCBATTLECHARGE_HIGHLIGHT_NAMES 
- engine/CTCTavernGameMaster.h|+5 -1|FABLE_RETAIL_CTCTAVERNGAMEMASTER_GETGAME 
- engine/CTriangleStripifier2.h|+29 -0|FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS 
- engine/CUserProfileManager.h|+4 -0|FABLE_RETAIL_CUSERPROFILEMANAGER_ORIGINAL_TARGETING_QUERY 
- engine/CWorld.h|+8 -0|FABLE_RETAIL_CWORLD_MAP_TABLE_QUERY 
- engine/Direct3DTexture9.h|NEW|
- engine/EMatrix34Init.h|NEW|
- engine/NGraphicAppearance_CTCGraphicAppearance.h|+7 -0|FABLE_RETAIL_CTCGRAPHICAPPEARANCE_ORIGINAL_CURSOR_HELPERS 
- engine/NPlayerGui_CDrawBase.h|+46 -2|FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_GETPLAYER FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES FABLE_RETAIL_NPLAYERGUI_CURSOR_ORIGINAL_DECLARATIONS_PASS22 
- engine/NPlayerGui_CDrawHighlightingTargetingCursor.h|+26 -1|FABLE_RETAIL_NPLAYERGUI_CURSOR_ORIGINAL_DECLARATIONS_PASS22 FABLE_RETAIL_NPLAYERGUI_HIGHLIGHT_ORIGINAL_NAMES 
- stlport/fable_algorithm.h|+19 -0|FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22 
- stlport/fable_containers.h|+82 -1|FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22 FABLE_RETAIL_PASS22_SCOREBOARD_COPY_BACKWARD_ORIGINAL FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22 FABLE_RETAIL_STLP_ORIGINAL_COPY_TRIVIAL_BACKWARD_PASS22 FABLE_RETAIL_STLP_ORIGINAL_TRUE_UNINIT_COPY_PASS22 FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22 FABLE_RETAIL_STLP_VECTOR_C FABLE_RETAIL_STLP_VECTOR_ORIGINAL_RESIZE_INSERT_CHAIN_PASS22 

The two NEW headers (Direct3DTexture9.h, EMatrix34Init.h) come verbatim from Codex's profile (water rows; included only under
FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES / the CMatrix3x4 ENUM_CTOR opt-in). All opt-in comments and evidence lines are
Codex's (pass 22 rows/<addr>/NAMING_EVIDENCE.md, PROBES.md); agentP6 added no header body of its own -- the water match lever
is in the row sources (FABLE_RETAIL_ASSERT dead arms from the existing fable_assert.h).

## Default-path equivalence (lane_merge/equiv_check.py, 2026-11-15)
rebuild/include vs rebuild/include+this overlay, all 56,367 landed rows (source + test): 1,319 first-pass DIFFs, of which
1,318 were the EH funclet rows whose `#include "../src/compiled/..."` resolves relative to the include dir (a tree-location
artifact; rerun with the new tree beside a `src` symlink: 0 DIFF). The last, 0x00CBFB7D RunCutsceneMacro, differs only in the
`__FILE__` string of an `if (0)` assert arm (the tree's path), i.e. no token change. Default path is unchanged for every landed row.

## Changes after the coordinator's snapshot (landed at 50e1018c95)
Added for 0x00675E20 CCreatureDef::Transfer (Codex pass-22 profile merged onto the repo with tools/mkovl2.py; every addition is
behind FABLE_RETAIL_CCREATUREDEF_TRANSFER or another new opt-in; pincheck 0 mismatches; the two new headers carry only new asserts):
- engine/CCreatureDef.h (+60: PDB member class under FABLE_RETAIL_CCREATUREDEF_TRANSFER; repo asserts kept after the #endif)
- engine/CCreatureStatsDef.h (+17: member class under the same opt-in; the __vftable assert also skipped under it)
- engine/CDefString.h, engine/CFloatRange.h (FABLE_RETAIL_CFLOATRANGE_ORIGINAL_CTOR_PASS22), engine/CHeroAbilityStatWeight.h,
  engine/CPersistContext.h (TransferSubComponent under FABLE_RETAIL_CCREATUREDEF_TRANSFER), engine/CPersistTraits.h (traits
  specialisations under FABLE_RETAIL_CCREATUREDEF_TRANSFER / FABLE_RETAIL_CPERSISTTRAITS_CREATURE_FULL_TYPES_PASS22),
  engine/CRandomAppearanceMorph.h
- NEW engine/CExpressionSet.h, engine/CWoundMorphs.h (only included by the opt-in branches)
Conflict resolutions: CEngineSubPrimitiveBase.h, CTCInventory.h, fable_string.h, CRGBFloatColour.h, fable_containers.h,
NHeroInformationScreens_CBase.h, NInventory_CTCInventoryBase.h, CEnginePrimitiveRenderUtil.h -> repo text (a superset of Codex's);
those files are therefore unchanged and not in hdr/. Codex's *.opaque files dropped.
No other hdr/ file changed after the snapshot (all session-2 probes used separate overlays under pv/).
Equivalence after these additions (equiv_check.py, repo 50e1018c95 vs repo+hdr, tree beside a src symlink): 56,369 rows, one DIFF,
0x00CBFB7D, the `__FILE__` path string only (as before). Default path unchanged.
