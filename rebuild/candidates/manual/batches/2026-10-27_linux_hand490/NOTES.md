# Hand chunk 490 (CMap .. CWideString): 36 of 40 byte-exact

11 MATCH + 25 RELOCATION_MATCH, 0 retail_asm; 1 parked (DIFFER); 3 skipped (not function entries).
All 36 tests PASS under g++ (tall.sh) and VC7.1 (tall.py); vland dry run: 36 wins, no RELOC_IDENTITY.

## What matched (36)

- **CMap** 0081e9c0 GetScriptedMapBrush(long): STLport list walk with a counter, `throw;` past the end (/EHsc: `push 0 ; push 0 ; call _CxxThrowException`).
- **CThingBuilding** 0082d0c0 IsEntitySerialisable() const: `!CreatedByOwnerThingCreation`, an unsigned-char bitfield at +0x91 (`shr ; not ; and 1`).
- **CThingAICreature** 0082ff60 GetMaxSprintingSpeed(const C3DVector&) const: forwards to virtual slot 81 GetMaxRunningSpeed; a named float local keeps `call ; ret 4` (a bare `return f(d);` tail-jumps).
- **CThingNoise** 00838570 WorldUpdate(): `CThing::WorldUpdate(); return false;`.
- **CThing** 00838950 IsLineOfSightTo_00838950(const CThing&) const: PeekWorldMap().IsLineOfSight(*this, other, true); the guard form `if (!f()) return false; return true;` gives `test al,al ; setne al`.
- **std (STLport 4.6 algorithms)**:
  - 0084ecd0 __uninitialized_copy<const CCountedPointer<NCameraModes::CSubComponentBase>*, ...> (/Gr): no _STLP_TRY in retail.
  - 0086b4d0 __pop_heap<pair<long, CRandomAppearanceMorph::CSkeletalMorph>*> (/Gr).
  - 0086f9a0 __pop_heap_aux<pair<EExpressionType, CExpressionSet::CEntry>*> (/Gr): the by-value pair is rebuilt through `mov eax,esp`.
  - 00877fd0 __find_if<_List_iterator<const CThingBuilding*>, NCameraModes::CIsPosInBuilding>: /Gr puts the hidden result in ecx and the tag in edx.
  - 00992db0 __pop_heap<pair<long, CSoundBankRecord>*> (/Gr): the by-value pair copy calls the record's out-of-line copy constructor, and the callee destroys the value parameter.
  - 008ee850 _Rb_tree<..long key..>::_M_find const: a copy of the landed 0x0083B170 model, with an explicit instantiation of the private member.
- **std containers**:
  - 0086e9f0 vector<CRandomAppearanceMorph::CBodyPartMesh>::resize(n, x): erase or _M_fill_insert, both declared only.
  - 008fc840 pair<float, list<C3DVector> > implicit copy constructor: it needs the `_List_base` sentinel built in a local. STLport's `_List_iterator(const iterator&)` is a user copy constructor for iterator only, so const_iterators are pushed as plain words.
  - 008fc890 list<CPhysicsMeshObject>::insert(pos, x): _M_create_node inline, with the implicit CPhysicsMeshObject copy (vptr 0x0125D084, IV count bump, `rep movsd` x12).
- **CCreatureAction_BlockRespond** 00855b90 ctor: the base is called as BlockBase(creature, creature, 150, blow). It needs a pack(1) C3DVector with a user copy constructor.
- **CCombatAction_ControlledStrafeJump** 0085bd60 IsOverrideableByActionOfSamePriority(): slot 27 IsActionReadyForInterruption, using the guard-return form.
- **CCombatAction_GenericStrikeResponseBase** 00865b50 IsAlwaysPhysicsPushableBySelfAvoidance(): CCreatureActionBase::IsUsingActionMovement, using the guard-return form.
- **CVillageTaskSecuritySweep** 00860300 IsValid() const: `!HasBecomeInvalid` (engine header donor include).
- **CThingBodyReorienter**:
  - 00863250 IsUpdateNeeded(): `!BodyReorientationDone`.
  - 00863790 dtor: ReorientBodyYZForSlopes(), then the inline STLport vector<CIntelligentPointer<CThing> > destruction.
- **NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry** 00884d40 implicit copy ctor: emitted through a placement-new helper.
- **NEnvironmentInternal::CEnvironmentThemePriorityStackEntry** 00886430 SetDefaultThemeWeight(float, bool): an FPU clamp with the constants reloaded from the pool, then lock, then an optional rebalance.
- **CCombatSequence_BanditKingSpinAround** 008b0000 StatGetName(): a static __fastcall that returns a CCharString by value through a hidden pointer in ecx.
- **NCreatureMode::CModeStrafe** 008e9b60 OnDeactivate(): `ActiveWeapon = -1`, then the inline base OnDeactivate, then a tail jmp to the reset helper.
- **CUnprovenCountedPointer_008f0000** 008f0000 Assign: reading the old pointer into a local gives retail's load order (other.Object first) and the single register.
- **LHNetworkLib::LTimer** 008f9d00 Start(): write the 1e-5f store before reading SaveSpeedUpFactor. VC hoists the read, but the spill stays after the store.
- **NBalverineCombat** 009054f0 SetLungedThisFrame(): `g_lastLungeFrame = CWorld::GetFrame();`.
- **CAIStateGroup_MinionGuardFollowPlayer** 0090c190 IsStillValid(): `return IsStillValidInternal(true);` (slot 14).
- **CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>** 0090e120 operator(): the out-of-line copy of the landed 0x0090E2E0 inline filter (three GetPItem calls).
- **CIsFreeBench** 00918050 operator(): the landed CIsBenchWithOnePerson model without the in-use count.
- **NEntityEvents::CEventVillagerInteraction** 00982390 GetLifeTime(): `StillWaitingFrame - WorldFrame + 60`.
- **CThreadedFileManager** 0098e590 IsObjectFinished(uid) const:
  - `bool finished; if (...) finished = false; else finished = true;` makes VC7.1 duplicate the unlock into both arms. A pre-initialised local gives one shared unlock instead.
  - The STLport iterator needs user constructors so that find returns through the hidden pointer.
- **CASoundManager** 00991670 FlushLoadingQueue_00991670(): `PSoundBank->ProcessLoadingQueue(false, 0.0)`; the 0.0 argument is `fld qword [pool]`.
- **CBaseClass** 0099a300 ~CBaseClass(): an empty user virtual destructor.
- **CInitBaseClass** 0099a350 Initialise(): `Valid = true; return true;`.
- **CWideString** 0099aed0 CWideString(): `PStringData(0)`, then `++g_CWideStringInstanceCount`.

## Parked for Fable 5.1 (1)

- 008edc70 CUnprovenSequence_008edc70::Step_008edc70 DIFFER(83v85): retail is `mov ecx,[esi+4] ; mov dl,[esi+2Eh] ; test dl,dl ; mov eax,ecx ; mov [esp+4],eax` (index loaded before the bool test, then copied to eax for the spill and compare); built is `mov al,[esi+2Eh] ; test al,al ; mov ecx,[esi+4] ; mov [esp+4],ecx` | tried: post-increment `Table[Index++]`, `++Index; Table[Index-1]`, local index + `Index = index + 1`, two locals (index, position) | next: an inline helper taking the index by value (e.g. `bool IsInRange(long i) const`) so the copy is a parameter, or read the index through a `const long&`.

## Skipped

- 008f92ef Catch@008f92ef: a catch funclet that uses the parent frame's [ebp-0x28]. It is not a standalone function.
- 008f92fd: continuation code after that funclet. It runs on the parent frame (ebp, fs:[0]) and pops registers it never pushed.
- 009277cb: a mid-function fragment of an STL tree routine (pops with no matching pushes). The catalog row is a propagated _Tree<...> label.

## Label corrections

The catalog label of each row is replaced as follows:

| Address | Catalog label | Correct name |
|---|---|---|
| 0081e9c0 | _gapscan::sub_0081e9c0 | CMap::GetScriptedMapBrush(long) |
| 0082d0c0 | CThingBuilding::vfunc_22 | CThingBuilding::IsEntitySerialisable() const |
| 0082ff60 | CThingAICreature::vfunc_82 | CThingAICreature::GetMaxSprintingSpeed |
| 00838570 | CThingNoise::vfunc_19 | CThingNoise::WorldUpdate |
| 00838950 | _gapscan | CThing::IsLineOfSightTo_00838950 (descriptive) |
| 0084ecd0 | _gapscan | std::__uninitialized_copy<CCountedPointer<NCameraModes::CSubComponentBase>> |
| 00855b90 | _gapscan | CCreatureAction_BlockRespond ctor |
| 00863790 | _global::Catch@0047e8cc | CThingBodyReorienter::~CThingBodyReorienter |
| 0086b4d0 | _gapscan | std::__pop_heap<pair<long, CSkeletalMorph>> |
| 0086e9f0 | _gapscan | vector<CBodyPartMesh>::resize |
| 0086f9a0 | _gapscan | std::__pop_heap_aux<pair<EExpressionType, CEntry>> |
| 00877fd0 | _gapscan | std::__find_if<..., CIsPosInBuilding> |
| 00884d40 | _gapscan | CScreenFilterEntry copy ctor |
| 00886430 | _gapscan | CEnvironmentThemePriorityStackEntry::SetDefaultThemeWeight |
| 008b0000 | _gapscan | CCombatSequence_BanditKingSpinAround::StatGetName |
| 008e9b60 | CModeStrafe::vfunc_6 | CModeStrafe::OnDeactivate |
| 008ee850 | _gapscan | _Rb_tree::_M_find |
| 008f0000 | _gapscan | descriptive Assign |
| 008f9d00 | _gapscan | LHNetworkLib::LTimer::Start |
| 008fc840 | _gapscan | pair<float, list<C3DVector>> copy ctor |
| 008fc890 | _gapscan | list<CPhysicsMeshObject>::insert |
| 0090e120 | _gapscan | CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee>::operator() |
| 00918050 | _gapscan | CIsFreeBench::operator() |
| 00982390 | CEventVillagerInteraction::vfunc_3 | GetLifeTime |
| 0098e590 | _gapscan | CThreadedFileManager::IsObjectFinished |
| 00991670 | _global::Catch@0040dc8c | CASoundManager::FlushLoadingQueue_00991670 (descriptive method; class proven by vtable slot 3) |
| 00992db0 | _gapscan | std::__pop_heap<pair<long, CSoundBankRecord>> |
| 0099a300 | CBase::CBase | CBaseClass::~CBaseClass |
| 0099a350 | CXMVPlaybackInfo | CInitBaseClass::Initialise |
| 0099aed0 | CCharString | CWideString::CWideString |
| 008edc70 (parked) | _gapscan | descriptive |

### Callee and neighbour labels found wrong

- 0x009FC410 is labelled std::_Cons_val<..CAnimationEntry..>; it is CResourceBank::ProcessLoadingQueue(bool, double).
- 0x0086E5C0 is labelled std::vector::Resize_CSharerParserEntry_24byte; it is vector<CRandomAppearanceMorph::CBodyPartMesh>::_M_fill_insert. 0x0086E150 is its erase.
- LTimer methods, pinned by body against the FableWin.pdb method set and the Ego_r.pdb layout:

  | Address | Current label | Correct name |
  |---|---|---|
  | 0x008F8030 | _Dest_val<..CMusicManager..> | LHNetworkLib::LTimer::MSeconds |
  | 0x008F8080 | (none) | LTimer::Running |
  | 0x008F80A0 | (none) | LTimer::GetSpeedUpFactor |
  | 0x008F80D0 | CMusicManager_OnMusicEntryTimeout | LTimer::SetSpeedUpFactor(float) |
  | 0x008F9CA0 | MusicTimer_CheckAndTrigger | LTimer::Stop |
  | 0x008FA000 | CAIStateGroup_MinionHasInformation::OnActivate | LTimer::LTimer() |
- 0x008FBF10 is labelled LinkedList_CloneNodes; it is std::list<C3DVector>::_M_insert_dispatch<const_iterator>(..., const __false_type&).
- 0x008FC900 (unlabelled) is std::pair<float, list<C3DVector> >::pair(const float&, const list&).
- 0x008FC960 is a list<CPhysicsMeshObject> push_back-shaped insert(end(), x) caller.
- 0x008EE890 is labelled BinarySearchTree_FindLE; it is map<long, CBlendedParticleEffectSet>::find, which the landed 0x008EE790 already says.
- 0x0098E5F0 is CThreadedFileManager::ObjectExists(unsigned long) const. 0x0098E630 is its static forwarder on the singleton 0x013BC870, which tail-jumps to IsObjectFinished.
- 0x00990320 is the const STLport _Rb_tree::find of CThreadedFileManager::Objects (map<unsigned long, CFileObject*>).
- 0x009922F0 is labelled SRecord::SRecord / GFPredicateAnd<..>, and 0x00992630 is labelled CCamera::UpdateCleanupFlags (landed under those names). Both are members of the sound-bank record that the landed siblings call CSoundBankRecord: its copy constructor and its operator=.
- 0x0090DFB0 is labelled CThingFilter_IsValidGossipee in the manifest. The landed 0x0090E2E0 says it is the out-of-line CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing>::operator().
- 0x007C85B0 / 0x007C85A0 sit among CTCFireballSpell methods but act on the parked row's 0x34-byte sequence object (+0x2E flag, +0x30 count). Their class is unproven.

## Header debt

These rows use local models even though an engine header exists. In each case the header carries the vptr as data or lacks the needed method, so a virtual call or static method needs the local model:

- CThing: 0082d0c0, 00838950 and 00918050. The 00918050 model is the landed 0x0094B9B0 one: slot 48 GetStateFlags plus the interface map.
- CThingAICreature / CThingCreatureBase: 0082ff60. The chunk lists rebuild/include/engine/CThingAICreature.h, but slot 81 needs a polymorphic model.
- CCombatAction_ControlledStrafeJump / CCreatureActionBase: 0085bd60 (header listed by the chunk) and 00865b50. Both need virtual slot 27.
- CCreatureAction_BlockBase: 00855b90 (base ctor call).
- CWorldMap: 00838950. CScriptedMapBrush: 0081e9c0 (opaque element). CWorld: 009054f0 (static GetFrame not in the header).
- CCombatSequence_BanditKingSpinAround: 008b0000. The header has no static StatGetName.
- CIVCountedPointeeBase: 008fc890 includes engine/CIVCountedPointeeBase.h but uses only IVRefCount; the vptr is data there.
- CVillageTaskSecuritySweep: 00860300 uses engine/CVillageTaskSecuritySweep.h through a donor include.

These classes have no engine header (local models from the PDB): CThingBodyReorienter, LHNetworkLib::LTimer, CPhysicsMeshObject, CThreadedFileManager (+CFileObject), CMessageEvent / CMessageEventFilter_ThingSearchFilter, CASoundManager, CModeStrafe/CModeBase, CEnvironmentThemePriorityStackEntry, CScreenFilterEntry, CSoundBankRecord.

## Callee names used

| Address | Name |
|---|---|
| 0x0063E6F0 | CWorldMap::IsLineOfSight(const CThing&, const CThing&, bool) const |
| 0x004C79E0 | CThing::PeekWorldMap() const |
| 0x00692A50 | CCreatureActionBase::IsUsingActionMovement() const |
| 0x00855AF0 | CCreatureAction_BlockBase ctor |
| 0x008633B0 | CThingBodyReorienter::ReorientBodyYZForSlopes |
| 0x0086B070 | __adjust_heap<pair<long, CSkeletalMorph>> |
| 0x0086F450 | __adjust_heap<pair<EExpressionType, CEntry>> |
| 0x0082D000 | CThingBuilding::IsPointInside |
| 0x00884A40 | map<CThing*, CThingGraphicByPassFlagState> copy ctor |
| 0x00886200 | RebalanceWeights |
| 0x0099EBF0 | CCharString(const char*, long) |
| 0x008E97B0 | ResetStrafeAnims_008e97b0 (descriptive) |
| 0x0049D870 | CWorld::GetFrame |
| 0x009FC410 | CResourceBank::ProcessLoadingQueue(bool, double) |
| 0x008F8030 | LTimer::MSeconds |
| 0x008FBF10 | list<C3DVector>::_M_insert_dispatch |
| 0x00A01B50 | CBaseIntelligentPointer::GetPItem |
| 0x0090C900 | CThingFilter_IsValidGossipee::operator() |
| 0x004365B0 | const CVectorMap LowerBound |
| 0x007FEBF0 | CTCUserActionPointBase::IsActionPointFree |
| 0x00990320 | _Rb_tree::find const |
| 0x009922F0 | CSoundBankRecord copy ctor |
| 0x00992630 | CSoundBankRecord::operator= |
| 0x00992B80 | __adjust_heap<pair<long, CSoundBankRecord>> |
| 0x00992510 | ~CSoundBankRecord |

## Names

### Proven

- 0081e9c0
- 0082d0c0
- 0082ff60
- 00838570
- 0084ecd0
- 00855b90
- 0085bd60
- 00860300
- 00863250
- 00863790
- 00865b50
- 0086b4d0
- 0086e9f0
- 0086f9a0
- 00877fd0
- 00884d40
- 00886430
- 008b0000
- 008e9b60
- 008f9d00
- 008fc840
- 008fc890
- 009054f0
- 0090c190
- 0090e120
- 00918050
- 00982390
- 0098e590
- 0099a300
- 0099a350
- 0099aed0

### Probable

- 008ee850: the STLport _Rb_tree::_M_find algorithm is proven. The instantiation is probably CMaterialDef's map<long, CBlendedParticleEffectSet>, which is next to its find 0x008EE890, but it is unreferenced and 47 byte-identical copies exist. The types therefore stay CUnprovenKey/Value_008ee850.
- 00992db0: the STLport __pop_heap algorithm and element layout are proven. The element's record name CSoundBankRecord is descriptive, following the landed siblings, because the sound module is absent from the Ego PDB.

### Descriptive

- 00838950: CThing::IsLineOfSightTo_00838950.
- 00991670: CASoundManager::FlushLoadingQueue_00991670. The class is proven by vtable slot 3; the method name is descriptive.
- 008f0000: CUnprovenCountedPointer_008f0000::Assign_008f0000. The pointee releases through slot 1, which fits neither CIVCountedPointeeBase nor CCountedPointer.
- 008edc70 (parked): CUnprovenSequence_008edc70::Step_008edc70.
