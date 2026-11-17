# NF52 header overlays (round 7)

Each file is a copy of the CURRENT repo header with `#ifdef` opt-ins only. Without the opt-in macro, every overlay is
token-identical to the repo header (re-checked at hand-back with `diff` against the current rebuild/include copy: every overlay
only ADDS lines -- the `#ifdef ... #endif` blocks below; where a body becomes declaration-only under an opt-in (CScopedShadersEnable
ctor/dtor, CSegmentList dtor, CSkeletalMorphResourceManager ctor) the original line stays unchanged under `#else`).

| Overlay | Repo header | Opt-in(s) | What it adds |
|---|---|---|---|
| engine/CScopedShadersEnable.h | rebuild/include/engine/CScopedShadersEnable.h | `FABLE_RETAIL_CSCOPEDSHADERSENABLE_CTOR_OUTOFLINE`, `FABLE_RETAIL_CSCOPEDSHADERSENABLE_DTOR_OUTOFLINE` | declaration-only ctor / dtor for the TU that defines the out-of-line ??0 (0x009D8AD0) / ??1 (0x009D8B40); default path keeps the inline bodies under `#else` |
| engine/CMainGameComponent.h | rebuild/include/engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKEVENTDISPATCHTABLEENTRY` (nested in `_EVENTDISPATCH`) | FableWin.pdb public inline `PeekEventDispatchTableEntry(long) const` over the protected static EventDispatch; includes engine/CGameEventDispatch.h at the top when on |
| engine/CGameEventPackage.h | rebuild/include/engine/CGameEventPackage.h | `FABLE_RETAIL_CGAMEEVENTPACKAGE_GETEVENTS` (nested in `FABLE_RETAIL_CGAMEEVENTPACKAGE`) | PDB const `GetNoEvents()` 0x009F16E0 / `GetEvent(unsigned long)` 0x009F16D0, declarations only |
| engine/CWorld.h | rebuild/include/engine/CWorld.h | `FABLE_RETAIL_CWORLD_PROCESSEVENT` | PDB protected `ProcessEvent(const CGameEvent*)` 0x0049D980, declaration only |
| engine/COpinionReactionManager.h | rebuild/include/engine/COpinionReactionManager.h | `FABLE_RETAIL_COPINIONREACTIONMANAGER_ATTITUDETOPLAYERASSTRING` | PDB `GetThingAttitudeToPlayerAsString(const CThing&) const` 0x007018B0 and (unless `FABLE_RETAIL_CTCWIFE_MARRY`, which already has it) the private `GetAttitudeOrReactionType` 0x00701010, declarations only |
| engine/CGameScriptThing.h | rebuild/include/engine/CGameScriptThing.h | `FABLE_RETAIL_CGAMESCRIPTTHING_ISNULL` | PDB `virtual bool IsNull() const` (slot 77, 0x008D0110), declared before PThing; declaration only |
| engine/CWorldMap.h | rebuild/include/engine/CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_FINDMAXZINCELL` | public `IsPosInMap(const C2DCoordI&) const`, `GetGroundSizeZAtBlock(const C2DCoordI&) const`; protected `FindMaxGroundZInCell(const C3DVector*) const` 0x0063B750 / `FindMaxThemeZInCell(const C3DVector*) const` 0x0063B7F0 (Ego_r IBE); declarations only |
| engine/CCreatureActionBase.h | rebuild/include/engine/CCreatureActionBase.h | `FABLE_RETAIL_CCREATUREACTIONBASE_SETCREATURE` | protected `SetCreature(CThingCreatureBase&)` 0x006928C0, declaration only |
| engine/CCombatActionBase.h | rebuild/include/engine/CCombatActionBase.h | `FABLE_RETAIL_CCOMBATACTIONBASE_SETTARGET` | public `SetTarget(CThing&)` 0x0085AA90, declaration only |
| engine/CTCInventoryMagic.h | rebuild/include/engine/CTCInventoryMagic.h | `FABLE_RETAIL_CTCINVENTORYMAGIC_CTOR` | PDB `CTCInventoryMagic(CThing&)` 0x0072DB90 (??0CTCInventoryMagic@@QAE@AAVCThing@@@Z), declaration only |
| libpk/CSegmentFile.h | rebuild/include/libpk/CSegmentFile.h | `FABLE_RETAIL_CSEGMENTLIST_CTOR_ALLOCATOR`, `FABLE_RETAIL_CSEGMENTLIST_DTOR_OUTOFLINE` | `CSegmentList(PKCTRAllocator*)` 0x00C2DFE0 (declaration; the inline default ctor is kept) and the out-of-line copy of the inline virtual `~CSegmentList()` 0x00C2E030 (declaration; inline body under `#else`). Descriptive names (libpk ships no PDB) |
| engine/CCard.h | rebuild/include/engine/CCard.h | `FABLE_RETAIL_CCARD_FRAMEUPDATE` | forward `class CRightHandedSet;` + Ego_r `FrameUpdate(const C3DVector&, const CRightHandedSet&)` 0x008F06F0, declaration only. Note: CCard.h's layout asserts need `FABLE_RETAIL_C3DVECTOR_PACKED` (as its other users define) |
| engine/CSkeletalMorphResourceManager.h | rebuild/include/engine/CSkeletalMorphResourceManager.h | `FABLE_RETAIL_CSKELETALMORPHRESOURCEMANAGER_INLINE_CTOR`, `FABLE_RETAIL_CSKELETALMORPHRESOURCEMANAGER_GETMORPHTARGET` | the constructor inline in the class (CTCSkeletalMorph::CalculateSkeletalMorph 0x00786700 expands it through CTBaseSingleton::Get(); 0x004AE2A0 is its COMDAT; default keeps the out-of-line declaration under `#else`) and PDB `CCountedPointer<C3DSkeletalMorphTarget>& GetMorphTarget(const CCharString&)` 0x006C3620 (non-const return, Ego_r 0x0071B6C0 / FableWin 0x023E36E0), declaration only |

Rows that define each opt-in (grep of rows/): CScopedShadersEnable CTOR (0x009D8AD0) / DTOR (0x009D8B40); CMainGameComponent PEEK (0x00434A30,
0x00434A60, 0x0049D8C0, 0x0049D8E0, 0x0049D980, 0x0049DFB0, 0x0049E1D0); CGameEventPackage GETEVENTS (0x00434A60, 0x0049DFB0); CWorld
PROCESSEVENT (0x0049D980, 0x0049DFB0); COpinionReactionManager ATTITUDETOPLAYERASSTRING (0x007018B0); CGameScriptThing ISNULL (0x008D0110);
CWorldMap FINDMAXZINCELL (0x0063B750, 0x0063B7F0); CCreatureActionBase SETCREATURE (0x00692E50, 0x00857430); CCombatActionBase SETTARGET
(0x00857430); CTCInventoryMagic CTOR (0x0072DB90); CSegmentList CTOR_ALLOCATOR (0x00C2DFE0) / DTOR_OUTOFLINE (0x00C2E030) (0x00C2DBE0 and its
three EH funclets compile against the overlay file on the default path); CCard FRAMEUPDATE (0x008F06F0); CSkeletalMorphResourceManager
INLINE_CTOR + GETMORPHTARGET (0x00786700).

Macro check (hand-back): none of the 16 opt-in macros above occurs anywhere in rebuild/ or in agentNF51/ (grep -w); NF51 overlays
CTCInventory.h / CAIStateGroup_ApprenticeHero.h / CFrontEndManager.h / CAIStateGroupBase.h / CTCInventoryAbilities.h, none of which NF52 overlays.
