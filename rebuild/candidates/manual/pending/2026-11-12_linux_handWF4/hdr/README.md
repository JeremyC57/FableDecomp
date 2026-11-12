## 0x004B3CE0

# Header overlay of row 0x004B3CE0 CQuestManager::ActivateQuests (default paths unchanged; every change is an `#ifdef FABLE_RETAIL_*` opt-in)
- engine/CQuestManager.h: copied from $S/w22/wf1/004b4260/hdr (= the CURRENT repo header + wf1's `FABLE_RETAIL_CQUESTMANAGER_ACTIVATEMULTIPLEQUESTS`, not landed yet), plus `FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUESTS` (nested in it): DayActive +0x8c / NightActive +0x8d typed (for the same-TU real body of LoadResourcesForQuests 0x004B2510) and the inline `long AllocateQuestID() { return CurrentQuestID++; }` (Ego_r.pdb + FableWin.pdb ?AllocateQuestID@CQuestManager@@AAEJXZ).
- engine/CScriptInfo.h: copied from $S/w22/wf1/004b4260/hdr (repo header + wf1's `FABLE_RETAIL_CSCRIPTINFO_FASTCALL_ALLOC`), plus `FABLE_RETAIL_CSCRIPTINFO_ACCESSORS`: both allocator pointers __fastcall, the script allocator's second parameter `CGameScriptInterfaceBase&`, and the inline accessors GetAllocFunc / GetMasterScriptDataAllocFunc / IsMasterScript exactly as both PDBs mangle them (P6I.. = __fastcall pointers). Supersedes _FASTCALL_ALLOC when defined.
- stlport/fable_map.h: full copy of the CURRENT repo header, plus `FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE`: STLport 4.6 _tree.h spelling -- `_M_lower_bound` / `_M_upper_bound` declared (retail 0x004B9D00 / 0x004B9D50, landed), lower_bound / upper_bound / equal_range inline over them, `erase(iterator, iterator)` declared (0x004B9C10, landed), `erase(const key_type&)` = equal_range + distance + erase(first, last); set gains lower_bound / upper_bound / equal_range / erase(first, last). Needs FABLE_RETAIL_STLP_LIST_SIZE_DISTANCE (fable_containers.h distance()). The tree block of FABLE_RETAIL_STLP_TREE_ERASE_KEY is skipped when the new opt-in is defined (do not define both).

## 0x00A90040

# Header overlay for row 0x00A90040 (full copies of the current repo headers plus additions only)
- engine/CGenericVertex.h: `FABLE_RETAIL_CGENERICVERTEX_INLINE_SETTERS` -- inline bodies (out-of-class template definitions after the class) for SetPosition / SetNormal / SetU1 / SetV1; includes engine/C3DVector.h. Bodies only, default path unchanged.
- engine/C3DPrimitive2.h: `FABLE_RETAIL_CANIMATEDBLOCK_DATA` (with FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS) -- CAnimatedBlock::Data typed CIVCountedPointer<CAnimatedBlock::CDeviceIndependent> (+0x2C, still 4 bytes), the nested pointee (: CIVCountedPointeeBase, VertexList +0x08, BoneWeights +0x14), private declarations of ConvertToVBFormatNonBump 0x00A8FF50 and ConvertToVBFormatBump 0x00A90040; includes CIVCountedPointer.h / CArray.h / C3DIndexedBoneWeight.h. No layout change, default path unchanged.

## 0x0064546F

engine/C2DCoordI.h: FABLE_RETAIL_C2DCOORDI_NOTEQUAL -- inline operator!= (FableWin ??9C2DCoordI@@QBE_NABV0@@Z) inside the TOFLOAT branch; row 0x0064546F.
engine/CPlayerGuiDef.h: FABLE_RETAIL_CPLAYERGUIDEF_QUICKACCESSEFFECTOFFSET -- QuickAccessEffectOffset typed C2DVector at +0x6F0 (Ego_r type) instead of split floats; row 0x0064546F.
(repair 2026-11-12) Both overlays rebased on current rebuild/include: CPlayerGuiDef.h = main (incl. wf3 FABLE_RETAIL_CPLAYERGUIDEF_FLOURISH) + only the QUICKACCESSEFFECTOFFSET lines; C2DCoordI.h's `#include "fable_compiler.h"` now sits inside #ifdef FABLE_RETAIL_C2DCOORDI_NOTEQUAL. Default build unchanged.

## 0x008565E0

(no header opt-ins: this row uses the repo headers unchanged)

## 0x00AC8AE0

# Header overlay for row 0x00AC8AE0
- engine/COptimisedPrimitive.h: `FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_JOINADJACENTFACES` -- declares private `bool JoinAdjacentFaces(unsigned long,unsigned long)` (Ego_r 0x00C61CA0 / FableWin `...@@AAE_NKK@Z`) inside the FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE class; declaration only.
- engine/QuadricOptimiserTypes.h: `FABLE_RETAIL_QUADRIC_COPTIMISEDFACE_HASUNIQUEVERTEX` -- inline `bool COptimisedFace::HasUniqueVertex(unsigned long)const` (FableWin `?HasUniqueVertex@COptimisedFace@QuadricOptimiserInternals@@QBE_NK@Z`), U[0]||U[1]||U[2] order from retail's compares.
- engine/QuadricOptimiserTypes.h: `FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE` -- Ego_r COptimisedEdge record (FaceA +0, FaceB +4, UniqueVertex1 +8, UniqueVertex2 +0Ch, 16 bytes).

## 0x008F10B0

- engine/CTavernGameCardBaseDef.h: FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_CARDPACK -- CardPack as std::vector<CCardDef> (Ego_r type) instead of opaque bytes (row 0x008F10B0).
- engine/CTCTavernGame.h: FABLE_RETAIL_CTCTAVERNGAME_HIDEMESH -- declares static __fastcall HideMesh(CThing*) 0x005E9B10 (row 0x008F10B0).
- engine/card_sorting_types.h: FABLE_RETAIL_CTCTAVERNGAMECARDBASE_RHSET -- GamePositionRHSet typed CRightHandedSet (row 0x008F10B0).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG -- declares the two-argument __fastcall std::random_shuffle (row 0x008F10B0).
- engine/CTavernGameCardBaseDef.h: FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_TYPENAME -- inline StatGetTypeName body (literal 'CTavernGameCardBaseDef'), as the landed GetSubDef row 0x008F27A0 shows (row 0x008F10B0).

## 0x00ADDBE0

engine/C3DBoundingSphere.h: FABLE_RETAIL_C3DBOUNDINGSPHERE_BUILDFROMSUBSPHERES -- declares BuildFromSubSpheres(const C3DBoundingSphere*,long) (row 0x00ADDBE0); default path unchanged.

## 0x0065A41C

- engine/CTCVillage.h: `FABLE_RETAIL_CTCVILLAGE_FINEAMOUNT` declares `long GetFineAmountForCurrentCrimes()` (0x0065A41C; Ego_r + FableWin `?GetFineAmountForCurrentCrimes@CTCVillage@@QAEJXZ`), declaration only.

## 0x0065AA6F

# Header overlay for row 0x0065AA6F (CTCVillage::IsTimeToGoToTavern)
- engine/CTCVillage.h: `FABLE_RETAIL_CTCVILLAGE_TAVERNTIMES` (inside FABLE_RETAIL_CTCVILLAGE) -- declares IsTavernTime() const 0x00658C0A, GetTavernTimes(float&, float&) const 0x00658C5B, IsTimeToGoToTavern(CThingAICreature*, const CWorld&, bool, float) 0x0065AA6F (Ego_r.pdb + FableWin.pdb names); declarations only.
- engine/CGameTimeManager.h: `FABLE_RETAIL_CGAMETIMEMANAGER_DAYSPEED` (inside FABLE_RETAIL_CGAMETIMEMANAGER) -- inline `float GetDaySpeed() const { return TimeStep; }` (FableWin.pdb ?GetDaySpeed@CGameTimeManager@@QBEMXZ; pairing with TimeStep +0x10 inferred from the day-speed setter 0x006BB940 writing +0x10); no layout change.
Both are full copies of the current repo headers plus the additions; default paths unchanged.

## 0x00BBCC70

engine/CEngineResourceManager.h: FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE -- inline `CTexture& GetStipleFadeTexture()` (FableWin.pdb ?GetStipleFadeTexture@CEngineResourceManager@@QAEAAVCTexture@@XZ), row 0x00BBCC70; default path unchanged.

## 0x007B8B10

# Header overlay (row 0x007B8B10)
- engine/CTCTavernTable.h: `FABLE_RETAIL_CTCTAVERNTABLE_LAYOUT` (nested in `FABLE_RETAIL_CTCTAVERNTABLE_SEATS`; needs engine/CIntelligentPointer.h with `FABLE_RETAIL_CINTELLIGENTPOINTER` first) -- members after Occupants (Tankards +0x1C, Food +0x28, SeatsCurrentlyUsed +0x34 opaque, NumPeopleSeated +0x40, SomeoneTalking/Laughing, ActionPoint*Strlen +0x48..+0x58, AvailableForUse +0x5C) and the GetEatingPoint declaration (0x007B8B10).
- engine/CTCNamedUserActionPoints.h: `FABLE_RETAIL_CTCUSERACTIONPOINTBASE_SETFREE` -- declares CTCUserActionPointBase::SetActionPointAsFree(long) 0x007FEF40.
- engine/CTCNamedUserActionPoints.h: `FABLE_RETAIL_CTCNAMEDUSERACTIONPOINTS_GETNAME` -- declares CTCNamedUserActionPoints::GetName(long, CCharString&) 0x007FEFC0.

