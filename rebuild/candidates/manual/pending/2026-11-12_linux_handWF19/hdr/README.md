## 0x006B3FF0

engine/CEnvironment.h: FABLE_RETAIL_CENVIRONMENT_UPDATE -- nested CEnvironmentGameTickState typed (Ego_r members, retail 0xc20, Time +0xbb8) for GameTickState/PrevGameTickState, PRainSplashParticle as CIntelligentPointer<CThing>, Update + private UpdateSunAndMoonPositions/UpdateWind/LightingFlash + GetRainStrength decls (row 0x006B3FF0).
engine/CEnvironmentSpellEffectInterpolator.h: FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FRAMEUPDATE -- FrameUpdate() decl, 0x006B8640 (row 0x006B3FF0).
engine/CGameDefinitionManager.h: FABLE_RETAIL_CGAMEDEFINITIONMANAGER_ENVIRONMENTDEF -- PeekEnvironmentDef() (FableWin 0x02455340) over descriptive PEnvironmentDef +0xd8 (row 0x006B3FF0).

## 0x00727D00

engine/NGraphicAppearance_CTCGraphicAppearance.h: rebased on the current repo header (keeps _REMOVETINT and _FULLSCALE); adds FABLE_RETAIL_CTCGRAPHICAPPEARANCE_RANDOMMORPH -- GetAlpha / PeekLightingChannel (inline), GetTintColour/GetPMeshEffect/RefreshPrimitiveAnimationAndPos decls (row 0x00727D00). GetFullScale comes from the existing _FULLSCALE opt-in, which the row defines.

## 0x007BF790

engine/NGraphicAppearance_CTCGraphicAppearance.h: FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETFORCERESEND -- inline SetForceResendEveryRenderFrame(bool) (row 0x007BF790)
engine/CTCWeaponTrail.h: FABLE_RETAIL_CTCWEAPONTRAIL_UPDATEWEAPONTRAILS -- inline StatGetInterfaceType + UpdateWeaponTrails decl in the non-UPDATE branch (row 0x007BF790)
engine/CTCObjectAugmentations.h: FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_COPYFROMTHING -- CopyAugmentationDataFromThing(const CThing&) decl in the lite class (row 0x007BF790)

## 0x006BFCD0

engine/CTCBuyableHouse.h: FABLE_RETAIL_CTCBUYABLEHOUSE_GETSIGNPOSTPOS -- declares private GetSignPostPos(bool, C3DVector&, CRightHandedSet*) 0x006BFCD0 (Ego_r + FableWin).

## 0x0055D180

# hdr overlay (wf19, row 0x0055D180) -- copies of the current repo headers (re-based 2026-11-13; CSurface.h includes agentNF32's FABLE_RETAIL_CSURFACE_GETBYTELENGTH block) plus these opt-ins and the two #if rewrites noted below
- engine/CAFile.h: FABLE_RETAIL_CAFILE_ISROOMFORFILE -- declares static __fastcall CAFile::IsRoomForFile(long, const CWideString&) (0x00999AB0; Ego_r + FableWin).
- engine/CDateAndTime.h: FABLE_RETAIL_CDATEANDTIME_SYSTEMTIME -- declares CDateAndTime(const _SYSTEMTIME&) (0x009E9A30) and ToFileTime() const (0x009E9D40).
  Rewrite: repo line 4 `#ifndef FABLE_RETAIL_CDATEANDTIME_CTOR` becomes `#if defined(..._SYSTEMTIME) && !defined(..._CTOR)` (the new class body) then
  `#elif !defined(..._CTOR)` (the unchanged default body); the class is a one-line definition per opt-in, so the new opt-in needs its own branch.
  With neither macro the preprocessor picks the same default body as the repo; _CTOR alone still picks the repo's _CTOR branch.
- engine/CUserProfileManager.h: FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME_MEMBER -- layout prefix: 8-byte pad + CWideString ProfileName at +0x08 (struct_layouts_egor).
  Rewrite: repo line 3 `#ifdef FABLE_RETAIL_CUSERPROFILEMANAGER_PROFILENAME` becomes `#if defined(..._PROFILENAME) || defined(..._PROFILENAME_MEMBER)` so the
  new opt-in also gets the existing `#include "fable_string.h"` (CWideString must be complete for the member). Default and _PROFILENAME paths unchanged.
- engine/CPlayerGui.h: FABLE_RETAIL_CPLAYERGUI_SCREENSHOTMESSAGE -- declares Show/Hide/IsShowingScreenshotMessage (0x004379E0/F0/0x00437A00).
- engine/CSurface.h: FABLE_RETAIL_CSURFACE_GETD3DSURFACE -- inline GetD3DSurface() const { return PD3DSurface; } (FableWin ?GetD3DSurface@CSurface@@QBEPAUIDirect3DSurface9@@XZ). Only change vs repo.

## 0x00A8F9E0

- engine/C3DPrimitive2.h: copy of repo header + FABLE_RETAIL_C3DPRIMITIVE2_DATA (Data typed scoped_ptr<N3DPrimitive::CDeviceIndependent>, inside the INDEXBUFFER branch) and FABLE_RETAIL_C3DPRIMITIVE2_UPLOADTOINDEXBUFFER (UploadToIndexBuffer decl + inline InitIndexBuffer / SetIndexBufferStatus / GetRepeatingMeshBatchSize / GetBaseBlock).
- engine/C3DPrimitive2.h is a full copy of the current repo header plus only these two opt-ins.

## 0x00B206B0

engine/CCardinalSpline.h: FABLE_RETAIL_CCARDINALSPLINE_CONTROLPOINT -- inline GetControlPoint(long)/SetControlPoint(long,const C3DVector&) (FableWin.pdb names), for 0x00B206B0.

## 0x007D46D0

- engine/CThingSearchTools.h: full copy of the current repo header plus nested opt-in FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_TYPEMASK (inside ALLTHINGSBODY): GetAllThings body gains the T::StatGetTypeFlags THING_TYPE mask test between the scan test and the filter, as the landed 0x007D4CE0 instance (`and edx,0Fh / cmp dl,0Fh` @0x007D4D1E) and GetPBestThing. Default path unchanged. Used by row 0x007D46D0.

## 0x00D17A80 (not landed)

- engine/CQ_BountyHuntScript.h: FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS declares WatchForBoasts 0x00D17A80 and WatchForQuestInfoDisplayConditions 0x00D17DA0.
- stlport/fable_containers.h (experimental, NOT used by the best source): FABLE_RETAIL_STLP_DESTROY_AUX_FORCEINLINE, FABLE_RETAIL_STLP_DESTROY_RANGE_FORCEINLINE (_Destroy(range)/__destroy), FABLE_RETAIL_STLP_DESTROY_ONE_FORCEINLINE (_Destroy(T*)), FABLE_RETAIL_STLP_VECTOR_DTOR_FORCEINLINE (~vector) -- inline-budget probes for 0x00D17A80; none reproduced retail.
- engine/CSpawnedFunc.h (experimental, NOT used): FABLE_RETAIL_CSPAWNEDFUNC_PLAIN_INLINE_CTOR (plain inline ctor instead of __forceinline) -- no effect on 0x00D17A80.
- stlport/fable_containers.h (attempt 3, experimental, NOT used by the best source): FABLE_RETAIL_STLP_REAL_DEALLOC (allocator::deallocate -> __malloc_alloc<0>::deallocate -> free, one more inline level), FABLE_RETAIL_STLP_VECTOR_DTOR_DEPTH0 / _NOAUTO (#pragma inline_depth(0) / auto_inline(off) round the in-class ~vector), FABLE_RETAIL_STLP_DESTROY_CHAIN_DEPTH0 (#pragma inline_depth(0) round the _construct.h destroy chain) -- none changes 0x00D17A80.

