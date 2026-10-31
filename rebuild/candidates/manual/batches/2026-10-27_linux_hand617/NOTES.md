# Chunk 617 -- notes (10 same-length parked rows)

Result: 2 RELOCATION_MATCH (0x0096BCB0, 0x00919780; tests PASS under g++ and VC tall.py; read-only vland WINS: 2),
8 re-parked same-length in differ/ (2 improved: 0x00F3ABF0, 0x006AD700; 0x006A4FB0/0x0063E6F0/0x00965640/0x00621A20 respelled).

## Matched rows
- **0x0096BCB0** `CThingSearchTools::GetAllThingsInRoughArea<CThing, std::vector<CThing*>, CThingFilter_HasDefGlobalIndex>` -- the
  catalog/parked identity (GetPBestThingInRoughArea<HasTrackName,...>, FindThingsInRoughArea) was wrong. The body is the landed
  GetAllThingsInRoughArea sibling (rebuild/src/compiled/00/8e/..._008e3eb0.cpp) re-instantiated: no type mask, filter 0x006A70B0
  (bound `??RCThingFilter_HasDefGlobalIndex@@QBE_NABVCThing@@@Z`), same-TU `vector<CThing*>::_M_insert_overflow` 0x00436AC0.
  Leaf in both PDB name sets.
- **0x00919780** same template, `<CThingGameObject, std::vector<CThingGameObject*>, CThingFilter_HasTC>`: TypeMask 7, filter 0x006A80A0
  (`CThingFilter_HasTC::Type` is the Ego_r member), overflow 0x00788FFC. Caller CAIStateGroup_LookAtInterestingThings 0x00919240.

## Parked rows (line 2 of each differ file carries residue | tried | next)
- 0x0072C430 IsAbilityAvailable: 15 `!HasTC` arms; ours rotates ecx/edx/eax for ParentThing, retail reuses the dead `this` (esi).
- 0x00555A90 UpdateVisibleList: colouring in the child loops, and the by-value C2DVector copy order.
- 0x0063E6F0 IsLineOfSight: retail caches the list head in esi across the search call (whole-program mod/ref info). Respelled own
  symbol to references (`?IsLineOfSight@CWorldMap@@QBE_NABVCThing@@0_N@Z`; the parked PBV spelling was wrong),
  PeekThingManager slot 4, PeekThingSearchTools, GetFocalPos.
- 0x00621A20 InitialiseQuestCardList: colouring. Found but not yet applied: CTCInventoryBase +0x20 = Categories
  (vector<NInventory::CCategory>, 0x2c-byte elements); secondary vtable 0x012585A0 slot 3 = IsInventoryScreen (CTCInventoryQuests
  0x00619FD0 returns false); primary slot 75 = GetInventoryGuildValidity (0x0061A0C0, IsGuildValid +0x14C); +0x150 = QuestCardCount;
  CQuestManager +0x5c = ActiveQuestCards (list<CIntelligentPointer<CThing>>); AddCategory
  `?AddCategory@CTCInventoryBase@NInventory@@IAEXPAV12@JPAVCThing@@@Z`; NInventory::CCategory::SetAsSelected.
- 0x00F3ABF0 CalcCompressionScript: guard plus do-while in the value loop fixed that loop; one pair is left: `mov ebx,edi` before the
  split-loop `jle`.
- 0x00965640 OnReplace: this vs &PStealableItem ebx/ebp swap; every callee respelled from the PDB.
- 0x006AD700 ControlledMove: Y,Z,X named compound dot product fixed the term order; the Y product operand form is left.
- 0x006A4FB0 GetFirstCollision: final Normalise X store scheduled before the pops (retail interleaves).

## Names: proven vs descriptive
Proven (PDB name set and a second source: binding, vtable slot, donor order, or RELOCATION_MATCH): all own symbols of the 10 rows,
plus every entry in NAME_FIXES.tsv.
Descriptive / probable (left unchanged or flagged):
- 0x00A442E0 `GetSquaredDistanceToInfiniteLine` (__fastcall, returns squared distance): this is the existing binding, and the PDB
  has no match. The differ declares it as ClosestPointOnLineSegment (descriptive).
- 0x006B13B0 CTCPhysicsBase slot 93: probably `GetPhysicsMeshInfo() const` (the CTCPhysicsStandard and CTCPhysicsRigidBody
  overrides at slot 93 are PDB `?GetPhysicsMeshInfo@...@@UBEPBVCThingPhysicsMeshInfo@@XZ`); there is no PDB base entry.
- 0x0063F360 out-of-line dtor of list<const CThingGameObject*>. FableWin has `??1?$list@PBVCThingGameObject@@...QAE@XZ`, but the
  differ declares `~_List_base` and has to choose between them. Catalog `SortTreeRecursively`/StdList_Clear is wrong.
- 0x006AD700 ComputeAcceleration 0x006AD260, GetType 0x00697AB0, the CTargeting/CLockTarget stand-ins, CTCPhysicsBase filler slots,
  ApplyAcceleration slot 81 (the PDB has a non-virtual CTCPhysicsBase::ApplyAcceleration QAE, which conflicts).

## Header debt
- src rows: the inline class model is copied from the landed 0x008E3EB0 sibling (vland WARN SHARED_HEADER_AVAILABLE CBaseClass;
  the same warning applies to the landed file). engine/CThingSearchTools.h models a 16-byte vector.
- 0x0063E6F0: CWorldMap local model.
- 0x00621A20: engine/CTCQuestCard.h is vptr-as-data, so it is modelled locally.
- 0x00965640: engine/CAIStateGroup_RecoverStealableItem.h flattens pointers; CCarryableDef needs `mutable long RefCount` and a
  const Destroy for the const GetSubDef path (CIVCountedPointeeBase refcount note).
- 0x006A4FB0: engine/CTCPhysicsBase.h is vptr-as-data with a flattened C3DVector, so it is modelled locally (Position +0x0c,
  Radius +0x34 use the real member names).
