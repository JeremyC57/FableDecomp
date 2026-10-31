# Hand chunk 606 (NUISystem::CComponent .. std::__push_heap): 1 of 10 byte-exact

Chunk 606 = 10 PARKED same-length rows from the parked-row retry sweep. Each was re-graded from its best attempt (all
reproduced), then given one lever per grade. 1 matched, 9 parked again (4 of them improved, 5 held).

## What matched (1)
- CAIStateGroup_CreatureUseCoverBase::DuckBehindCover 0x009713E0 (RELOCATION_MATCH, vland RELOCATION_MATCH + PASS under g++ and
  VC7.1). The parked attempt sat at 18 lines with invented accessors (`GetPWorld`, `CCombatManager::GetDef`, `long` param).
  What made it match:
  1. The Ego_r.pdb inline accessor chain instead of stand-ins: `GetWorld().GetCombatManager().GetCombatDialogueManager()
     .GetPDialogueDef()->Hiding`, `PBrain->GetCreature()`, `*PTargetThing` (CIntelligentPointer::operator*). With the shared
     engine/CDefPointer.h the by-value def copy gives retail's AddRef null test and the release WITHOUT one (VC7.1 folds the
     second test), and the register rotation the old model showed disappeared.
  2. The EAIActionReturn result in a NAMED local, compared after the action temporary's destructor (`mov esi,eax` ...
     `cmp esi,2`); comparing in the return expression keeps a bool in bl.
  3. `return result == 2 ? true : false;` gives `sete al` without the `xor eax,eax` of the plain `==` return.
  Models: engine/CDefClassBase.h + CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE), engine/CWorld.h, engine/CAIBrain.h and
  engine/CAIStateGroupBase.h through the `#define X XLayout` rename (headers lack the methods); RTTI 0x0128D31C proves
  CreatureUseCoverBase : EnemyCreatureBase (PTargetThing +0x14) : CAIStateGroupBase : CBaseClass.

## Parked for Fable 5.1 (9)
- 0052eaf0 NUISystem::CComponent::ChangeTargetPositionDelta DIFFER(181v181): delta-copy `mov edx,[esp+8]` after `sub esp,8` (retail both loads before) and converted-X store before the GetUIScale result push | tried: names fixed, same-TU noinline UnConvertVector body, implicit copy (push pair, 173), /Oa, size pragma (+5 earlier copy forms in sec1b) | next: lane gap: argument-copy scheduling (LTCG)
- 008098b0 CThingCompare_BestForTargeting2::operator() DIFFER(183v183): first score `fstp` after `mov ecx,esi` (retail before) | tried: valueB named (187), compare reference local, commuted multiplies (+ PDB const signatures, unnamed score earlier) | next: lane gap: two-call float comparator scheduling
- 00732d40 CTCWeatherVane::FrameUpdate DIFFER(185v185): `mov eax,[eax+18h]; mov edx,[eax]; mov ecx,eax` (retail `mov ecx,[eax+18h]; mov edx,[ecx]`) and no `fxch st(1)` before Y*inv | tried: PDB GetWorld (CWorld&, IBE), engine/CWorld.h, inv*X / inv*Y, scoped_ptr operator-> (187: `add eax,18h`) | next: lane gap: register choice / fxch
- 00bca6b0 CEnginePrimitiveManagerSpriteTrails::RenderPrimitive DIFFER(188v188): head Z copy via eax after `push esi` (retail ecx before), distance loaded to ecx with the renderer after the pushes (retail edx/ecx before) | tried: renderer local, distance as argument (191), camera reference, offset local, CEnginePrimitiveRenderUtil::CalcDistanceFromCamera inline (Ego_r name) with head local / 3-float temporary | next: lane gap: scheduling
- 00774c60 CTCShop::StockDisplay DIFFER(188v188): register colouring only (retail this=ebx, i=ebp, numItems=esi, numPlaces=edi; ours esi/edi/ebx/ebp) | tried: real STLport std::vector model + PDB spellings, for-init index, while loop (+ 4 forms in sec2b) | next: lane gap: register allocation priority
- 00c40590 CRightHandedSet::NormalizeOrientationFrame DIFFER(192v192): the two products with front.X (`[edi]`) load the right vector first (was 6 products' worth, now 2) | tried: named float `-=` locals (win, 12 -> 8 lines), flipped spellings, split products, named front.X, by-value operands, members direct (28), free pointer/reference helpers (8, the b.X products swap instead), cross as a member of front (24, all swapped) | next: lane gap: x87 operand canonicalisation (offset-0 operand rank)
- 00c1c330 TransformPointThroughMirroredFrame_00c1c330 (descriptive) DIFFER(192v192): result.y word stored after the final `fstp` (retail before) | tried: named float `+=` locals (win, 20 -> 2 lines), += translation, aggregate result, user copy ctor (196), pointer store, result declared first | next: lane gap: store scheduling
- 0084cd10 NCameraModes::SetCameraRoll DIFFER(194v194): forward Z dword copied with X/Y (retail after the first fmul) | tried: class key fix, copy ctor from member, len2 before up, named len2 +=, up 3-float ctor (+ decl order, 3-float ctor earlier) | next: lane gap: scheduling
- 007fa160 std::__push_heap<CThing**,int,CThing*,CThingCompare_BestForTargeting2> DIFFER(197v197): first compare value `fstp` before the posB push (retail after) | tried: PDB GetThingCompareMultiplier spelling, return-expression compare, pointer position locals (+ compare forms earlier) | next: lane gap: two-call float comparator scheduling

## Skipped
None.

## Names (two sources each)
- DuckBehindCover: Ego_r.pdb `?DuckBehindCover@CAIStateGroup_CreatureUseCoverBase@@IAE_N_N@Z` (protected, bool param; the parked
  `long` public spelling was wrong) + catalog row. Callees: GetWorld (PDB + 3 bindings), SaySpeech (PDB + binding + landed body),
  CCreatureAction_ProjectileCrouch(CThingCreatureBase&, CThingCreatureBase&, bool) (PDB; the bool overload is the one a bool
  param forwards to), AIAction_PerformActionAndWait (PDB + landed body), ~CCombatActionBase UAE (PDB + landed body), GetPItem.
  Accessors (Ego_r.pdb names): CWorld::GetCombatManager, CCombatManager::GetCombatDialogueManager,
  CombatDialogueManager::GetPDialogueDef (by value), CAIBrain::GetCreature, CThing::IsDead, CIntelligentPointer::Get/->/*.
  Field: CCombatDialogueDef::Hiding at retail +0x50 (Ego_r +0x54; retail def members are PDB -4, landed IsSpeechAllowed 0x008AF5F0).
  Descriptive: the EAIActionReturn enumerator spelling AI_ACTION_DONE (value 2; the type is the PDB's).
- 0052eaf0: the converter is CUIScreenUnitConverter::UnConvertVector 0x0052E530 (landed; retail `call 0x52e530`), not 0x0052E520
  as the parked sibling comments say; slot 31 = ChangePosition 0x0052E9C0 (vtable 0x0124608C read from the exe + landed body +
  PDB); slot 117 0x0052F3B0 (FlagsSetTwo bit 7) is descriptive (IsScreenConverted, as the landed AddPositionChild).
- 00732d40: CTCWeatherVane::FrameUpdate (vtable 0x0123A984 slot 9 + Ego_r donor order after OnInitialActivate); the catalog
  "CViewBase_UpdateLookDirection" is an autoname (label correction).
- 00774c60: Ego_r ?StockDisplay@CTCShop@@QAEXXZ; GetSaleItemsToPlace is const (QBE) in the PDB, the parked model was not.
- 0084cd10: Ego_r ?SetCameraRoll@NCameraModes@@YIXAAVCCamera@@M@Z; C3DVector must be `class` (V) or Rotate/SetView mangle ABU.
- 007fa160: STLport `__push_heap` (the catalog `_Push_heap` is the Ego_r Dinkumware spelling).
- 00c1c330: descriptive (TransformPointThroughMirroredFrame_00c1c330); 00c40590: class unproven (local COrientationFrame_00c40590,
  the catalog CRightHandedSet does not fit the layout) -- both would go to name_review/ if matched.

## Label corrections
- 0x00732D40 catalog "CViewBase_UpdateLookDirection" -> CTCWeatherVane::FrameUpdate.
- 0x00C1C330 catalog "CPhysicsMeshObject::GetHelperPointPositionStatic" disproved (PDB: a by-name lookup on CPhysicsMeshObject).
- 0x007FA160 catalog `_Push_heap<...>` -> std::__push_heap (STLport).
- 0x0052EAF0 catalog "ChangePositionDelta" -> NUISystem::CComponent::ChangeTargetPositionDelta (propagated from 0x0052EA30).
- Callee labels: see NAME_FIXES.tsv (13 addresses, all with landed or bound wrong spellings).

## Header debt
- engine/CWorld.h, engine/CAIBrain.h, engine/CAIStateGroupBase.h: no method declarations (GetCombatManager, GetCreature,
  GetWorld/AIAction_PerformActionAndWait) -- used through `#define X XLayout` + derived class.
- engine/CAIStateGroup_CreatureUseCoverBase.h and CAIStateGroup_EnemyCreatureBase.h: flattened, PTargetThing opaque bytes;
  modelled as a typed CIntelligentPointer<CThingCreatureBase> member of a derived class.
- engine/CCreatureActionBase.h: vptr as a data member, so it cannot carry the virtual destructor chain (retail calls
  ~CCombatActionBase UAE); local polymorphic model in 009713e0.
- engine/CTCBase.h (vptr data member) and engine/CEnvironment.h: CTCWeatherVane::FrameUpdate needs the real virtual (UAE); kept
  a local polymorphic CTCWeatherVane model in the 00732d40 differ.
- No headers for NUISystem::CComponent, CCombatManager, CombatDialogueManager, CCombatDialogueDef (local models).
