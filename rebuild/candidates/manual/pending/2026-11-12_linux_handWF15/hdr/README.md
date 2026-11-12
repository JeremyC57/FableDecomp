## 0x00BDF100

- engine/CLocalDetailCacheMap.h: FABLE_RETAIL_CLOCALDETAILCACHEMAP_CALCBOUNDINGSPHERE -- declares CQuadTreeElement::CalcBoundingSphereAndFadeDistanceFromChildren and defines nested CObjectCacheGroupCollection (CMovableResource base; Next/BoundingSphere/MaxFadeDistance/PrimitiveTypeMask from Ego_r) (row 0x00BDF100).
- engine/C3DBoundingSphere.h: FABLE_RETAIL_C3DBOUNDINGSPHERE_COPY -- PDB memberwise user copy ctor ??0C3DBoundingSphere@@QAE@ABV0@@Z (row 0x00BDF100).

## 0x005F6BD6

engine/CVectorMap.h: FABLE_RETAIL_CVECTORMAP_VALFIND -- FableWin ValFind<V2>(const V2&) forceinline value search (row 0x005F6BD6)
engine/CTCInventoryAbilities.h: FABLE_RETAIL_CTCINVENTORYABILITIES_AVAILABILITY -- declares IsAbilityAvailable 0x005EDF08 + GetNumAssignedReceivedAbilities 0x005EE146 (row 0x005F6BD6)
stlport/fable_algorithm.h: FABLE_RETAIL_STLP_FIND_NOINLINE_BODY -- real STLport find/__find bodies as same-TU noinline fastcall COMDATs (row 0x005F6BD6)
- (coordinator, at landing) The standalone find block is FABLE_RETAIL_STLP_FIND_NOINLINE_BODY_FORWARDER (renamed: row 0x007BF510 of the same workflow uses FABLE_RETAIL_STLP_FIND_NOINLINE_BODY for a different arm, forwarder inline); trip_count uses the STLport difference_type spelling. Regraded: RELOCATION_MATCH, test PASS.

## 0x00A60D40

engine/CDataInputStream.h: FABLE_RETAIL_CDATAINPUTSTREAM_READDATEANDTIME -- declares CDateAndTime ReadDateAndTime() (0x00995770, out of line).
engine/CDateAndTime.h: FABLE_RETAIL_CDATEANDTIME_CTOR -- declares the public out-of-line ctor ??0CDateAndTime@@QAE@XZ (0x009E9AB0). Added lines only: the original class line is kept verbatim under `#ifndef FABLE_RETAIL_CDATEANDTIME_CTOR`; the ctor-bearing class is the `#else` branch.

## 0x008DB810

- engine/NCameraModes_CTools.h: FABLE_RETAIL_NCAMERAMODES_CTOOLS_LANDSCAPEFROMTO -- const decl of GetLandscapePointIntersectionFromTo 0x00875550 (Ego_r QBE).
- engine/CJackDragonDef.h: FABLE_RETAIL_CJACKDRAGONDEF -- CSubDefClassBase-derived def + inline StatGetTypeName (for CDefPointer / GetSubDef<CJackDragonDef>).
- (coordinator, at landing) The CJackDragonDef.h overlay was a stale copy that conflicted with the landed FABLE_RETAIL_CJACKDRAGONDEF block (wf13, 0x00951D60); it is dropped. The row builds against the repo header (same member offsets) and still grades RELOCATION_MATCH.

## 0x005FBAA0

# hdr overlay: none used for 0x005FBAA0 (all opt-ins already in repo headers)

## 0x007BF510

engine/CTCWeaponTrail.h: FABLE_RETAIL_CTCWEAPONTRAIL_HASPOINTS declares HasWeaponTrailPoints() const (0x0076E4A0, Ego_r ?HasWeaponTrailPoints@CTCWeaponTrail@@QBE_NXZ) inside the COLLISION block (row 0x007BF510).
stlport/fable_algorithm.h: FABLE_RETAIL_STLP_FIND_NOINLINE_BODY (with FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH) gives the random-access __find its real STLport body as __declspec(noinline) (out of line 0x007C2980, visible to the caller), so the empty tag is homed on the live type slot (row 0x007BF510).
engine/CIntelligentPointer.h: FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_MIXED adds operator==(const CIntelligentPointer<T>&, const CIntelligentPointer<U>&) comparing both GetPItem results (needed by the visible __find body: element <const CThing> v value <CThing>, retail 0x007C2980 calls 0x00A01B50 twice). Name made up: the free operator's original spelling was not found.
- (coordinator, at landing) Overlays rebased onto the current repo headers (CIntelligentPointer.h keeps the landed FORCEINLINE_CTOR opt-in; fable_algorithm.h merged with row 0x005F6BD6's block). Regraded: RELOCATION_MATCH, test PASS.

## 0x00972CC0 (not landed)

- engine/CAIStateGroup_JackOfBladesStage1.h: FABLE_RETAIL_JACK_STAGE1_HASHITSCAUSEDRUSH declares protected bool HasHitsCausedRush() (0x00972340, Ego_r+FableWin ?HasHitsCausedRush@CAIStateGroup_JackOfBladesStage1@@IAE_NXZ).
- engine/CCombatManager.h: FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERREADY declares SetAttackerAsReadyToAttack(const CThingCreatureBase&, bool) (0x006EA450).

