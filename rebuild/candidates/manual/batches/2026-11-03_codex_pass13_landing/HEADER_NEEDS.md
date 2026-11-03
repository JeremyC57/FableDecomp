# Codex pass 13 landing: header needs of the held rows and open header debt

Four of the 51 Codex pass 13 rows are held. Each one matches (RELOCATION_MATCH) in its own Codex header context
(`../../pending/2026-11-02_codex_pass13/verified_pending/<addr>/`, `hdr_manifest.json`). None of them matches in a header form
we can make into one retail model in this landing. The sources and Codex headers stay in that pending package.

## 0x00501450 CWorldMap::GenerateRegionConnectivityGraph (held)
- **CVectorMap::LowerBound.** The row needs a LowerBound body that tests `size()` first, compares with the Compare functor
  and is expanded inline (no noinline). The tree model in `engine/CVectorMap.h` tests `end()` first, compares with
  `operator<`, and is `__declspec(noinline)`. Every landed CThing includer uses the tree model. The row had per-feature
  macros `FABLE_RETAIL_CVECTORMAP_SIZE_LOWERBOUND` and `_ORDINARY_LOWERBOUND`. Two bodies cannot be one model, so this
  needs a study of which retail TUs expand which form.
- **CThingFilter_HasTC.** The row needs it out of line (`FABLE_RETAIL_CTHINGFILTER_HASTC_EXTERNAL`).
- **CThing.** The row needs the navigation virtuals (Codex CTHING_NAVIGATION, which becomes the FABLE_RETAIL_CTHING
  branch).
- **Headers Codex wrote that are not installed.** The row needs `CThingSearchTools.h` (PeekClassList and the
  GetAllThings template), `CDiskFileWin32.h` (Codex class branch) and the STLport `set` `empty/begin/end`
  (`FABLE_RETAIL_STLP_SET` in `stlport/fable_map.h`). It also needs these new headers: CEventPackageFileHeader,
  CFrameRateSmoother, CGameEventPackage, CInterpolationInfo, CNetworkServer, CRoughFrameCounter, CTCActionUseScriptedHook,
  CTCDRegionExit, CThingFilter_HasTCOfLeafType, ECombatAnimationHandedness, EControlledMovementType, EGameAction and
  EMovementBandType.
- **Grades.** The row grades DIFFER(1313v1334) on the landing headers and RELOCATION_MATCH in the Codex context.

## 0x00989CB0 CShaderRenderManager::UpdateObjectSpaceLighting, 0x0098ACF0, 0x009DA9F0 CRenderManager2D::Render2DDrawList (held)
These three rows need a render header set that overlaps chunk 723 (round2/round3 overlays, still pending):
- **CShaderRenderManager.h (new).** Codex and 723 each wrote one. Merge them into one class; 723's round3 version is
  the newer one.
- **CShaderConstantRange.h, CRGBFloatColour.h, CVertexShaderConstant.h (new in both).** Codex and 723 each wrote these
  too. Merge them.
- **CRenderStateManager.h.**
  - The tree has a generated model.
  - Codex adds a RENDER2D_TYPES branch with its own guard.
  - Codex also adds the seven D3D*Table statics (names accepted from Codex's PDB DATA citation; cvdump list in
    NAME_DECISIONS.tsv).
  - 723 overlays the class too.
  - Needed: one FABLE_RETAIL_CRENDERSTATEMANAGER branch that holds the members both sides prove.
- **CEngineLightingManager.h.** 723 overlays it. Codex's 0x00989CB0 reads the light lists through
  CShaderGenericLight (new).
- **C4DVector.h.** 723 uses FABLE_RETAIL_C4DVECTOR with `Initialise(float,float,float,float)` and a CRGBFloatColour
  ctor. Codex's rows need the same class plus vector<C4DVector> erase/_M_fill_insert (queued REVIEW, cx13).
- **CMatrix4x4.h (new), CVertexShaderConstantLayout.h, CVertexShaderConstantLayoutEmpty.h (new),
  CScopedVertexShaderConstantLayout.h, CScopedShadersEnable.h.** Codex only.
- **Codex only, 0x009DA9F0.** CBankStateBlock, CShaderBankStateBlock, CStateBlockFunctionBase,
  CRenderStateCaptureBlock, CQuickDrawTriInfo, CTVertexRHWColSpecTex1, CSystemFont, CSystemFontCharacter,
  CIndexedArray, CPackedUIntArray, CDateAndTime, CChecksumCacheFile, Direct3D9Device, EPrimitiveType, EVertexType and
  RenderStateEnums.
- **C2DExtentsF.h.** Codex has a second model of the class; use the tree's.
- **CDisplayManager.h.** Codex's form conflicts with the tree's FABLE_RETAIL_CDISPLAYMANAGER branch.

**What chunk 723 needs from this landing.** 723's sources include three headers this landing changed: C3DVector.h,
CRGBColour.h and fable_string.h. Every change in them is additive and sits under a class macro, so the default path is
unchanged:
- FABLE_RETAIL_C3DVECTOR gained `GetAngleXY() const`.
- FABLE_RETAIL_CRGBCOLOUR gained a three-channel ctor that calls `Initialise(r,g,b,255)`.
- FABLE_RETAIL_CCHARSTRING gained SaveToFile(CAFile&) const, EqualsCaseInsensitive, operator< and the free
  `operator+(const char*, const CCharString&)`.

Regrade 723 on the landed headers before it lands. The new CRGBColour(r,g,b) overload can change overload resolution
where 723 passes three channels. Apart from that, 723's overlays can land on their own.
When they land, fold Codex's render needs above into the same classes. Do not land a second CShaderRenderManager /
CRenderStateManager model. After 723 lands, regrade the three held rows on its headers.

## Header debt noticed, not changed here
- **CSpawnedFunc / SuspendableProcess.** Retail sets `StopRunning=true` (landed 0x00D1CF90 / 0x00E479A0). The tree
  header does not model that store. Codex pass 13 had an edit for it; it was reverted in this landing because no landing
  row needs it and it changes the default path.
- **CEntityScriptBinding.** FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR has no counter-example among the pass 13
  rows (forced on in the 9 rows without it: 9 RELOCATION_MATCH). Folding it into FABLE_RETAIL_NSCRIPT changes every
  landed NScript binder TU, so it stays a per-TU opt-in until those TUs are rechecked.
- **Default path changed without a branch macro.** Three headers are retail-only class headers with no macro to keep the
  old path, and every includer was rechecked:
  - CQ_SunnyvaleMasterData.h: typed members, same layout.
  - CThingManager.h: inline GetCurrentThingUID.
  - NHeroInformationScreens_CBase.h: includes EPlayerMode.h instead of declaring an empty `enum EPlayerMode {}`.
