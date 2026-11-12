## 0x00931200

engine/CAIStateGroup_MinionReactToCriminal.h: FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL -- polymorphic retail class over CAIStateGroupBase (typed PHero / PHome, virtual IsStillValid() const, private IsValid() const, mutable *InsideHouse flags); default flat layout unchanged.

## 0x009DC6E0

(no overlay headers: the row uses only opt-ins already in the repo headers)

## 0x00761980

# Header overlay (row 0x00761980) -- full copies of the current repo headers plus opt-ins only
- engine/CTCShapeManager.h: `FABLE_RETAIL_CTCSHAPEMANAGER_DRAWSHAPES` -- types PersistantShapes / SelectedShape as the STLport vector of CCountedPointer<NShape::CShape> and its iterator (same 12 + 4 bytes) and declares the private DrawShapes 0x00761980 (PDB: AAE, const refs).
- engine/CTCShapeManager.h: `FABLE_RETAIL_CTCSHAPEMANAGER_GETPRINCIPALVECTOR` -- declares the public GetPrincipalVector(C3DVector&) const 0x00760910 (used only by the evidence probe probe2/gpv.cpp).
- engine/NShape_CClosedShapeBase.h: `FABLE_RETAIL_NSHAPE_CSHAPE_EDITORDRAWHIGHLIGHTPOINTS` -- declares the non-virtual NShape::CShape::EditorDrawHighlightPoints 0x007617A0 with its PDB const-reference signature (no layout change).
- engine/NShape_CClosedShapeBase.h: `FABLE_RETAIL_NSHAPE_CSHAPE_GETSTARTPOINT` -- the inline `bool GetStartPoint(C3DVector& pos) const` (FableWin.pdb name; body `if (Points.empty()) return false; pos = *Points.begin(); return true;`, proven by the byte-exact sibling probe below).
- engine/CEnginePrimitiveLine.h: `FABLE_RETAIL_CENGINEPRIMITIVELINE_CTOR_VISIBLE_OUTOFLINE` (attempt 2) -- the inline constructor's real body (landed 0x004C3090) visible to the caller but `__declspec(noinline)`: retail's DrawShapes calls 0x004C3090 while the same TU expands it in 0x007617A0; with it the red colour stores are scheduled after `push &red` as in retail. Default path unchanged.

## 0x00A89740

# Header overlay for row 0x00A89740 (wf3) -- full copies of the current repo headers plus `#ifdef` opt-ins only
| header | opt-in | what it adds | row |
|---|---|---|---|
| engine/C3DMesh2.h | FABLE_RETAIL_C3DMESH2_HELPERPOINTS | retail branch `class C3DMesh2 : public C3DMeshStats` (UnmodelledBase line split under `#ifndef`, same offsets) declaring the virtual overrides RelocateData 0x00A8B4C0 and vtable 0x0129DEF0 slots 4..9 (GetHelperPointPositionAnimated x2, GetDummyObjectPositionAnimated x2, GetDummyObjectTransformAnimated x2); needs FABLE_RETAIL_CMOVABLERESOURCE + FABLE_RETAIL_C3DMESHSTATS_DUMMYOBJECTS | 00a89740 |
| engine/C3DVector.h | FABLE_RETAIL_C3DVECTOR_GETSCALED | inline FableWin.pdb `?GetScaled@C3DVector@@QBE?BV1@ABV1@@Z` (componentwise product `C3DVector(X*scale.X,Y*scale.Y,Z*scale.Z)`) | 00a89740 |
| engine/CMatrix3x4.h | FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS | nested TransformPoint(C3DVector&) body (under FABLE_RETAIL_CMATRIX3X4): rows as plain left-to-right sums `E3k*zz+E2k*yy+E1k*xx+E4k`; default path unchanged | 00a89740 |

## 0x009122A0

engine/CBezierCurve.h: FABLE_RETAIL_CBEZIERCURVE_GETPOINTONCURVE declares `C3DVector GetPointOnCurve(float) const` (retail 0x009122A0); default path unchanged.
engine/C3DVector.h: not copied; the row uses the repo header's existing FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER (GFSqr).

## 0x00BE7BE0

engine/CLandscapeBackgroundPatch.h: FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_RENDER -- on the retail class: Render(bool,long&,bool&) 0x00BE7BE0, private Render(bool) 0x00BE6F70 and RenderWithOcclusion(long&,bool&) 0x00BE79D0 (declarations), inline HasWater() (FableWin.pdb name).
engine/CEngineWaterBackgroundSubPatch.h: FABLE_RETAIL_CENGINEWATERBACKGROUNDSUBPATCH_ACCESSORS -- WaterType typed EWaterType (enum defined under FABLE_EWATERTYPE_DEFINED), inline IsSea() / IsEmpty() (FableWin.pdb names).
engine/CEngineWaterRenderer.h: FABLE_RETAIL_CENGINEWATERRENDERER_ADDTORENDERLIST -- inline AddMidgroundSeaPatchToRenderList / AddBackgroundWaterPatchToRenderList (FableWin.pdb names, FABLE_FORCEINLINE bodies after the class; switches the sub-patch accessors on; needs FABLE_RETAIL_CARRAY_PUSHBACK).

## 0x0081F240

# Header overlay for row 0x0081F240 CMap::CopyPos (full copies of the current repo headers plus opt-ins)
- engine/CHeightMapCell.h: `FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS` -- FableWin.pdb inline accessors Get/SetEngineThemePaletteIndex, Get/SetEngineBlend (no layout change).
- engine/CMap.h: `FABLE_RETAIL_CMAP_ENGINEACCESSORS` (nested in FABLE_RETAIL_CMAP_THEMEPALETTE; includes engine/CHeightMap.h above the class) -- protected decls SetEngineThemeAtIncludingExtraCells(long,long,unsigned long,unsigned long) 0x0081EF30 and CopyPos 0x0081F240, inline GetEngineThemeAtIncludingExtraCells / GetEngineBlendAtIncludingExtraCells / SetEngineBlendAtIncludingExtraCells (no layout change).

## 0x00646580

# Header overlay for row 0x00646580 (full copies of the current repo headers plus opt-ins; default paths unchanged)
- engine/NPlayerGui_CDrawBase.h: `FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS` declares the static `__fastcall` CDrawBase::ClearEffects(vector<CIntelligentPointer<CParticleEmitter> >&) (0x006445BA, both PDBs).
- engine/CPlayerGui.h: `FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS` (with FABLE_RETAIL_CPLAYERGUI_MEMBERS) adds the inline FableWin accessor `bool IsShowSpecialAbilityButtons() const` (reads ShowSpecialAbilityButtons +0x250).
- engine/CPlayerGuiDef.h: `FABLE_RETAIL_CPLAYERGUIDEF_FLOURISH` types AbilityButtonFlourishEffectPos (+0x788) and AbilityButtonFlourishGraphicOffset (+0x794) as the C2DVector members Ego_r.pdb names (same layout as the split _X/_Y floats).

## 0x0052C920

# Header overlay for row 0x0052C920
No overlay headers: the row matches on the CURRENT repo headers with existing opt-ins only
(FABLE_RETAIL_C2DVECTOR, FABLE_RETAIL_C2DVECTOR_NORMALISE, FABLE_RETAIL_CRGBCOLOUR). No new opt-in was added.

## 0x00848B70

(no header opt-ins needed for 0x00848B70: the row grades against the repo headers as they are)

