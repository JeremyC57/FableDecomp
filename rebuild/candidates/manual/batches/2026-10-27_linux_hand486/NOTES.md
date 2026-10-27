# Hand chunk 486 (CThingSearchTools .. NUISystem::CTextSlider): 35 of 40 byte-exact

14 MATCH, 21 RELOCATION_MATCH, 0 retail_asm; 3 parked (DIFFER), 2 skipped (packed multi-function rows).
Every matched row has a behaviour test that passes under g++ (tall.sh) and VC7.1 (tall.py). The read-only vland run
reports 35 WINS and no RELOC_IDENTITY.

## What matched (35)
- **CTC OnInitialActivate family** (004d51f6 CTCTavernJug, 004d5372 CTCOnHeroPush, 004d8b7c CTCTavernTankard) and
  004d5476 NThingShot::CTCShotDefinitionThrow::GetStatToModifyOnHit: plain member stores / `category = 1; return true`.
  Shape: these sit in /Os TUs (packed odd addresses, `and [x],0` zero stores), so `#pragma optimize("s", on)` is needed.
- **CThing** 004c7ae0 GetPWorldSeed() const: `g_pThingManager->WorldSeed` (+0x2C in the retail manager).
- **CConversationAttitude** 004cd9c0 GetAnimGroupName (static `__fastcall`): `ConversationAttitudeToAnimGroupName[attitude]`
  over an STLport vector<CCharString>.
- **CCodeSectionManager** 004af000 LoadSectionLongName(CCharString): `LoadSection(GetHandleFromLongName(name))`. Shape: the
  by-value CCharString needs a user copy ctor (0x99EC30) + dtor (0x99EAE0) so the argument is built in its stack slot and
  the callee-owned parameter is destroyed last.
- **CWorldMap** 004fc170 GetMapWithIndex(long) const: `Maps[index]`, 72-byte CMapInfo, CArray at +0x20.
- **CThingPhysical** 00529720 IsSolidToThing (slot 46): `thing != this`, via the layout-rename donor include of
  engine/CThingPhysical.h.
- **NUISystem::CComponent** 0052efe0 GetType (slot 65) and 0052f180 IsBastardChild (slot 100): byte loads of Type and
  `(Flags >> 7) & 1`.
- **NUISystem::CTextSlider** 00548780 OnDeactivate: `IgnoreEvent(EEVENT_19)` through the CObserver base at +4 (the primary
  base needs exactly 140 slots, 0..139, for slot 141 to land at +0x234).
- **NUISystem::CChangingStateComponent** 0052cca0 copy ctor: `: CComponent(other)`. The StatesToDo std::list default-constructs
  its self-linked header node in a local before storing it.
- **NUISystem destructors** 00546660 ~CZoomingComponent, 00546c20 its `??_G`, 00545840 ~CSpellContainer:
  `CManager::GetInstance()->RemoveObserver(this)` (slot 5). Shape: the `??_G` is emitted when the virtual dtor is defined in the
  TU. ~CSpellContainer also destroys `vector<CCountedPointer<CComponent>> BaseSpells` (+0x15C). Its `__false_type` needs a
  user ctor, or VC zero-stores the tag byte.
- **CCountedPointer<T>::DeleteData** 0051ccd0 (CRegionConnectionGraph) and 004adc40 (CMessageEventManager):
  `delete (T*)data`, emitted by explicit instantiation. The member destructors are inlined (vector `__destroy_aux` + free;
  list `clear` 0x4A9CF0 + null-tested free).
- **Thing Alloc** 00527300 CThingDeadCreature (0xF0) and 0052ace0 CThingSwitch (0xE8): `new (false) T` via
  `CThing::operator new(size, bool)`.
- **CEnginePrimitivePrepareInfo** 00528200 copy ctor: memberwise. Shape: CCamera has to be modelled with float members
  (4-byte alignment); a `char[0x38]` pad misplaces it at +0x39.
- **NThingManager::CEntityRuntimePersistence** 0051e800 GetLevelSaveStats(unsigned long&, unsigned long&, unsigned long&)
  const. Shape: retail computes the element count ONCE (down-counter loop), so the source keeps `long n = Size()` in a
  local; the inline `i < Size()` re-divides every pass.
- **CThingManager** 0051ea90 SetUpdateAndRenderAllFlag(bool): store +0x99, then call FillRenderListsWithAllEntities.
- **STLport templates**:
  - 004adaa0 vector<CTestQuest>::clear (erase through `__copy` + `__destroy_aux`).
  - 004ae0d0 `_Destroy<pair<unsigned long, CCountedPointer<CFaction>>*>`.
  - 004b8ac0 list<CQuestManager::CObjective>::_M_create_node (malloc node + null-tested placement copy).
  - 004c2c00 `__unguarded_insertion_sort_aux<CMeshInfo*, CMeshInfo, CSortPrimaryMeshesLast>`.
  - 004c42b0 pop_heap<pair<float, CCameraTag>*, CKeyPairCompareLess> (/Gr: `push comp ; push 0` into the non-inlined
    `__pop_heap_aux`).
  - 0050b550 allocator<unsigned short>::allocate.
  - 00514210 set<unsigned long>::operator=.
  - 0051b410 `_Construct<CRegion, CRegion>` (/Gr).
  - 005269f0 `__uninitialized_fill_n` over a 16-byte POD: the element must be trivially copyable; a user copy ctor
    loses the esi/edi re-basing.
  - 00527680 `_Rb_tree::_M_empty_initialize`.
- **Library** 00534430 `_Pow_int<float>`: the VC7.1 <math.h> template verbatim in a /Gr TU, called by the out-of-line
  `pow(float, int)` 0x5349B0.
- **Descriptive** 00510020 `short ReturnShort2_00510020() { return 2; }`.

## Names: proven vs probable vs descriptive
- Proven (PDB name + structural evidence):
  - 004af000 LoadSectionLongName
  - 004ad810 GetAllThings (parked)
  - 0051e800 GetLevelSaveStats (sole three-reference method; ShowLevelSaveStats caller strings)
  - 0051ea90 SetUpdateAndRenderAllFlag (field +0x99)
  - 00534430 `_Pow_int<float>`
  - 004c7ae0 GetPWorldSeed
  - 004cd9c0 GetAnimGroupName
  - 0052efe0 GetType / 0052f180 IsBastardChild
  - 00548780 OnDeactivate
  - 0052cca0 copy ctor
  - 00546660 / 00546c20 / 00545840 destructors (identified by vtable)
  - 004d51f6 / 004d5372 / 004d8b7c / 004d5476
  - 00527300 / 0052ace0 Alloc (factory strings)
  - 00528200 copy ctor
  - 004adaa0 / 004adc40 / 0051ccd0 / 004ae0d0 (container types from callee chains)
  - 004c2c00 and 004c42b0 (family names in the PDB; the STLport spelling is used)
  - 0051b410 `_Construct<CRegion>`
- Probable:
  - 004b8ac0 list<CQuestManager::CObjective>: the only 12-byte CWideString+8 type kept in a std::list.
  - 00514210 set<unsigned long>: same tree operator= as landed 0x517060.
  - 0050b550: element type.
  - 004fc170 GetMapWithIndex.
  - 00529720 IsSolidToThing.
  - 0051ea30 FillRenderListsWithAllEntities (parked).
- Descriptive:
  - 00510020 ReturnShort2_00510020
  - 004c7870 CThing::DriverQuery_004c7870 (parked)
  - 005269f0 CUnprovenElement16_005269f0
  - 00527680 CUnprovenKey/Value_00527680

## Parked for Fable 5.1 (3)
- 004ad810 CThingSearchTools::GetAllThings<const CThing, std::vector<const CThing*>, CThingFilter_None> DIFFER(122v125): retail has an extra constant `xor al,al ; jne` after the InLimbo test and homes the pushed pointer in the dead `things` argument slot where ours opens a `push ecx` slot | tried: direct flag tests, IsAlive()/IsInLimbo() inline helpers, `__forceinline push_back` (best, probe a4), 4 grades | next: a filter call returning a constant `false` through an inline that VC7.1 cannot fold before scheduling (e.g. `CThingFilter_None::operator()` returning an `unsigned char` local), with push_back taking `things` by reference into a named local
- 004c7870 CThing::DriverQuery_004c7870 DIFFER(15v15): retail `xor al,al` before `test ecx,ecx ; je`, ours after the branch (same instructions, different order) | tried: if/return, ternary, both under pragma s, 4 grades | next: lane gap: scheduling (instruction order only)
- 0051ea30 CThingManager::FillRenderListsWithAllEntities DIFFER(86v88): retail keeps `this` in ecx across the outer loop and re-reads [this+4] for GetTypeList (Used tested through dl); ours keeps the vector base in ecx and reloads `this` from its spill at the loop top | tried: direct unsigned char index over ClassTypeInfo, IsClassTypeRegistered(long) + GetTypeList(unsigned char) inline split (best), const operator[], 3 grades | next: give CClassFactory::GetTypeList / IsClassTypeRegistered their real out-of-class inline definitions (a separate inline member after the class), or test a CThing* local for the forwarder call

## Skipped
- 004ddaa4 (CTCTargetingAI::vfunc_10): the 16-byte catalog row holds 3 packed functions.
- 004e0adc (CTCGatherPointBase::vfunc_6): the 12-byte catalog row holds 4 packed functions.

## Label corrections
- 004c7ae0: catalog "CViewBase::GetDisplayEngine" is CThing::GetPWorldSeed. The landed 0x4C7AA0 / 0x4C7AB0
  "CViewBase::GetDisplayEngine" rows are the CThing::GetWorld / PeekWorld family.
- 00545840: catalog "~CZoomingComponent" is NUISystem::CSpellContainer::~CSpellContainer.
- 004c2c00: catalog "_Copy_impl<CNavigationPosition*>" is `__unguarded_insertion_sort_aux<CMeshInfo*, CMeshInfo,
  CSortPrimaryMeshesLast>`.
- 0052efe0 / 0052f180: "NUISystem::CSprite::vfunc_65 / vfunc_100" are NUISystem::CComponent::GetType / IsBastardChild.
- 004c7870: "CThing::vfunc_22" is CThing's driver query.
- 00548780: "CTextSlider::vfunc_141" is CTextSlider::OnDeactivate.
- 004d51f6 / 004d5372 / 004d8b7c "vfunc_6" are OnInitialActivate; 004d5476 "vfunc_30" is GetStatToModifyOnHit; 00529720
  "CThingPhysical::vfunc_46" is IsSolidToThing.
- Callees:
  - 0x41E5F2 is NUISystem::CManager::GetInstance; it is bound under several names.
  - 0x4C3BF0 "_Pop_heap_0" is STLport `__pop_heap_aux`.
  - 0x512CE0 "LTextTreeWalkThrough_Assign" is `_Rb_tree<unsigned long,...>::operator=`.
  - 0x4C7590 "CRenderTarget::DetachTarget" is the CThing graphic-appearance forwarder (to CTCGraphicAppearance::SetAsChanged
    0x4BF9E0). Landed rows bind it under three different names: UpdateGraphicAppearance, SetGraphicAppearanceChanged and
    ForwardToGraphicAppearance.
  - The landed 0x533720 comment calls 0x52CCA0 "CDraggableInto"; 0x52CCA0 is CChangingStateComponent's copy ctor.

## Header debt
- engine/CThingPhysical.h keeps the vptr as a data member; 00529720 declares a real virtual on top of the donor.
- engine/CWorldMap.h has Ego_r offsets (Maps at +0x24; retail +0x20).
- vland SHARED_HEADER_AVAILABLE (local models kept on purpose):
  - CObserver: 0052cca0, 00545840, 00546660, 00546c20, 00548780.
  - CClass: 00527300, 0052ace0.
  - CRegion: 0051b410.
  - CQuestManager: 004b8ac0.

  These headers are flat data models (vptr as data, no ctors), so they cannot carry the polymorphic bases, the
  `??0CRegion@@QAE@ABV0@@Z` copy ctor or the nested CQuestManager::CObjective. CClass.h is Ego_r-sized (0x11).
- No engine/CThingManager.h exists. 0051ea90 / 0051ea30 model the retail layout from the landed ctor 0x523540, which has
  UpdateAndRenderAllFlag at +0x99 and CClassFactory::ClassTypeInfo at +4 with a 0x14-byte retail CClassTypeInfo.
