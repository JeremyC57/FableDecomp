# Hand chunk 384 (_global: std::__partial_sort<ENavigatorType*> .. vector<CGameCameraManager::CQuake>::_M_insert_overflow): 37 of 40 byte-exact

All 40 rows are module `_global` in the manifest; the real owners are listed per family below. All 37 matched rows
grade MATCH/RELOCATION_MATCH in gall.sh and their tests PASS under g++ (tall.sh) and VC7.1/Wine (tall.py).

## What matched (37)

### STLport algorithms (/Gr TUs written as explicit `__fastcall`)
- 0x00672330 `std::__partial_sort<ENavigatorType*>`, 0x00672390 `__adjust_heap`, 0x00672410 `make_heap` -- the
  sort of CThingCreatureDef::NavigatorTypes. Inner helpers (`__push_heap`, `__make_heap`) must be `__forceinline`.
  `less<>` on an enum is a signed `jge`; the halving `(i - 1) / 2` is `cdq ; sub ; sar`.
- 0x0067E7F0 / 0x00680700 `__unguarded_insertion_sort_aux` (CDegradableInfo, pair<EReactionSpeechType,long>),
  0x0067E8F0 `__adjust_heap`, 0x0067EB70 `__pop_heap_aux`, 0x0067F150 `__partial_sort` (CDegradableInfo, comparator
  CDegradableDef::CIsHealth1GTHealth2) -- `__unguarded_linear_insert` / `__pop_heap` `__forceinline`; the out-of-line
  callees (`__adjust_heap`, `__make_heap`, `sort_heap`) declared with their addresses.
- 0x006727F0 `uninitialized_fill_n<ENavigatorType*>`, 0x00673F90 / 0x00674030 `vector<pair<..>>::_M_allocate_and_copy`
  -- STLport has no `__type_traits` for enums and pairs, so the `__false_type` arm runs (placement-new null test);
  the helper needs `__forceinline` (plain inline stayed a call, 60v86).
- 0x0069C8F0 `__uninitialized_copy<CGameCameraManager::CQuake*>`, 0x0069C970 `__uninitialized_fill_n<CQuake*>` -- the
  out-of-line `__false_type` helpers themselves (`ret 8`: result/x and the tag on the stack); CQuake's memberwise copy
  (C3DVector with a user copy ctor, as the landed `_Construct` 0x0069C860) gives retail's second +0x14-biased induction
  register.
- 0x0069D300 `vector<CQuake>::_M_insert_overflow` -- the landed CSeaBodyDesc twin 0x0051C110 recipe (same-TU
  `__declspec(noinline)` placement loops, tags homed in the atend slot) plus `_Construct` as a same-TU noinline
  `__fastcall` body so the fill_len == 1 arm calls 0x0069C860 out of line.
- 0x00674D80 `__copy<CBodyPartMesh*>`, 0x0067C7E0 / 0x0067C8B0 `__copy` / `__copy_backward<CBlurredAttitudesDef*>` --
  the counted loop with the element's implicit operator= inline (def classes: no vptr copy, flag bits by RMW, the
  unaligned InstantiationName, vector operator= call + Dirty byte).
- 0x00685430 `std::fill<bool*, bool>` -- a byte loop (STLport's char overloads would be memset), so the element is bool.
- 0x0067B5B0 `list<CReactionMatch*>::insert`, 0x0067BCC0 `push_back` (insert inline: end() read before malloc).

### Serialisation
- 0x006724F0 / 0x00672530 `GFSerialiseVectorBinaryOut/In<ENavigatorType>`, 0x0067E720 `GFSerialiseVectorBinaryOut
  <CDegradableInfo>` -- the landed generic template with the traits transfer inline; the In side is `CPersistTraits<T>
  traits; ReadCount; out.resize(count)` with `resize(n) = resize(n, T())` and the inline memory-reader Read.

### Counted pointers / small members
- 0x00674E80 `CCountedPointer<CExpressionSet>::DeleteData` -- `delete static_cast<T*>(data)` (static `__fastcall`).
- 0x00692E50 `CCreatureActionBase::GetPActionFromEnum` -- static `__fastcall` with the hidden CCountedPointer result in
  ecx: `return CCountedPointer<..>()` is the inline two-zero-store default ctor, the clone path constructs into the
  result slot through 0x00695880; the registry map's find is out of line.
- 0x00697A50 anon `IsCameraModeInterruptable(CCountedPointer<const NCameraModes::CBase>)` -- a class passed by value
  goes on the stack in a /Gr TU (`ret 8`); the by-value parameter's release is duplicated at both returns. Leaf is the
  `?Name@?A0x` prefix with a `Call...` wrapper to emit it.
- 0x00689270 `operator!=<const CThing>(const CIntelligentPointer<const CThing>&, const CBaseObject*)` -- free
  `__fastcall` template (Ego_r.pdb symbol); `return p.GetPItem() != o;` = `xor ecx,ecx ; cmp ; setne cl ; mov al,cl`.
- 0x00689370 `float ceil(float)` -- the <math.h> float overload emitted out of line (CRT ceil import 0x00BFEB8A).

### Things / TCs / camera
- 0x006989D0 `NCameraModes::GetPBuildingThingOccupying` -- inline HasTC/GetTC(0x75) then one GetPItem, dead bit and
  thing-type 3 tests; the key temporary is the entry `push ecx` slot.
- 0x0069D090 `CSoundMap::GetPCriteriaString` -- the twin of the landed GetPSoundPair 0x0065FBA4 on CriteriaMap +0x10:
  inline GetCRC (length first), crc in edi and homed in the dead name slot thanks to LowerBound's body in the TU
  (`auto_inline(off)`), end() re-read for the final compare.
- 0x00682140 `CTCCreatureOpinionOfHero::ApplyOpinionTransientOffset` -- the plain offset must be a NAMED local (as one
  nested temporary VC evaluates the traits argument first), and the def pointer assigned inside the condition
  (`(personality = PPersonalityDef) != 0`) to get retail's ebx = this / edi = def (a local initialised before the `if`
  swapped them).
- 0x00686DC0 `CTCBase::UpdateGlobalTCs`, 0x00686F60 `CTCBase::GetPTCInfo` (inline CCharString operator<).
- 0x00687E10 `CInputProcessMain::AddManualCameraEvent`, 0x00687E70 `AddFreeCameraEvent` (/GS) -- CGameEvent builder
  with the inline AddUBYTE (the byte homed below the event).
- 0x00677760 `CReactionMatch_MoralityRenownBlob` ctor -- the MRPyramid ctor inlined, its own vtable store dead.
- 0x006864C0 `StdList_InitEmpty_006864c0` -- bare STLport list head init (unreferenced; descriptive name).

## Parked for Fable 5.1 (3)
- 0x0067f430 GFSerialiseVectorBinaryIn<CDegradableInfo> DIFFER(173v173): frame `sub esp,24h` v `20h` -- retail keeps one 28-byte object at [frame+4] that is both the `resize(n, T())` temporary and the loop's traits `this`; ours gives the empty traits its own slot (every later [esp+N] +4) | tried: traits first, traits after resize, traits.DefaultVal as the resize value (frame right, zeroing hoisted above the count read), DefaultVal + traits after resize (0x3C frame) | next: a traits class whose DefaultVal IS the resize value constructed in place (`CPersistTraits<T> traits; out.resize(count, traits.DefaultVal)` with a non-trivial T ctor in the traits ctor), or compare against another landed GFSerialiseVectorBinaryIn<struct> with a non-empty traits
- 0x00698a30 PlayCameraSoundThroughObservedThing DIFFER(239v230): the closing CDefPointer<const CEntitySoundDef> release -- retail `mov ecx,[esp+4] ; test ecx,ecx ; je ; dec dword [ecx+4] ; jne`, ours `mov eax,[esp+4] ; test ; je ; dec [eax+4] ; mov ecx,[esp+4] ; mov eax,[ecx+4] ; test eax,eax ; jne` (the holder's address escaped to GetSubDef, so it is re-read after the dec); everything above matches | tried: `--((CDefBase*)Object)->RefCount == 0`, CDefBase* member + `--Object->RefCount == 0`, register local `object = Object` (load/dec/store, worse), mutable count + inline const Release (worse) | next: release through an out-of-line-shaped inline on the def (`Object->DecRef()` returning the new count, as the landed GetSubDef 0x0065F604 CRefCounted base), or GetSubDef taking the pointer by reference so the holder is not considered escaped
- 0x006991f0 CGameCameraManager::AddCameraPoint DIFFER(170v170): frame layout only (`sub esp,0Ch` v `8`) -- retail homes the find result iterator in the dead `thing` argument slot (shared with the category tag), ours gives the named iterator its own frame word so every temporary offset is +4; instructions and order otherwise identical | tried: one full-expression `find(..) == end()` (compare before the temporary's dtor: wrong order), named iterator, implicit iterator copy (argument copies change), direct-init iterator with `!(it != end())` | next: build the find through a by-value helper returning bool after an inner scope, or assign into an iterator declared before the CIntelligentPointer temporary's scope

## Skipped
None.

## Label corrections
- 0x00672330 Vector_PartialSort_Heap -> std::__partial_sort<ENavigatorType*>; 0x00672390 _Adjust_heap<..> -> std::__adjust_heap<ENavigatorType*>; 0x00672410 Vector_MakeHeap -> std::make_heap<ENavigatorType*>.
- 0x006724F0 GFSerialiseVectorBinaryOut<float> -> <ENavigatorType>; 0x00672530 GFSerialiseVectorBinaryIn<CRGBColour> -> <ENavigatorType>; 0x006724B0 (outside the chunk) manifest "CEnginePrimitiveRenderer2D::AddPrimitive" is the NavigatorTypes binary transfer.
- 0x006727F0 _Fill_n<unsigned long*> -> std::uninitialized_fill_n<ENavigatorType*>.
- 0x00673F90 / 0x00674030 Vector_AllocateTriplets -> vector<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>>::_M_allocate_and_copy / vector<pair<EExpressionType, CExpressionSet::CEntry>>::_M_allocate_and_copy.
- 0x00674D80 _Move<CEngineTextureMorph*> -> std::__copy<CRandomAppearanceMorph::CBodyPartMesh*>.
- 0x00674E80 Object_FreeInnerAndDelete -> CCountedPointer<CExpressionSet>::DeleteData.
- 0x00677760 CReactionMatch_AsymmetricPyramid_Constructor -> CReactionMatch_MoralityRenownBlob::CReactionMatch_MoralityRenownBlob.
- 0x0067B5B0 List_InsertNode -> std::list<CReactionMatch*>::insert; 0x0067BCC0 LinkedList_InsertNode -> push_back.
- 0x0067C7E0 / 0x0067C8B0 _Copy_impl<CBlurredAttitudesDef*> -> std::__copy / std::__copy_backward.
- 0x0067E7F0 _Copy_impl<CActionInputControl*> -> std::__unguarded_insertion_sort_aux<CDegradableInfo*>; 0x0067E8F0 _Adjust_heap<CDebugBox*> -> std::__adjust_heap<CDegradableInfo*>; 0x0067EB70 _Pop_heap_0<CDebugSphere*> -> std::__pop_heap_aux<CDegradableInfo*>; 0x0067F150 swap<CDebugSphere> -> std::__partial_sort<CDegradableInfo*>; 0x0067F430 GFSerialiseVectorBinaryIn<CBrainUpdateZone> -> <CDegradableInfo>.
- 0x00680700 _Copy_impl<CNavigationPosition*> -> std::__unguarded_insertion_sort_aux<pair<EReactionSpeechType,long>*>.
- 0x00682140 _global ApplyOpinionTransientOffset -> CTCCreatureOpinionOfHero::ApplyOpinionTransientOffset (6th parameter a bool).
- 0x00685430 fill<char*,char> -> std::fill<bool*,bool>.
- 0x00686DC0 -> CTCBase::UpdateGlobalTCs; 0x00686F60 CVectorMap_FindMeshVolume -> CTCBase::GetPTCInfo.
- 0x00687E70 AddManualCameraEvent (duplicate label) -> descriptive AddFreeCameraEvent (event 0x0C).
- 0x00689270 SetLungedThisFrame -> operator!=<const CThing>(const CIntelligentPointer<const CThing>&, const CBaseObject*).
- 0x00692E50 GetPCombatActionFromEnum -> CCreatureActionBase::GetPActionFromEnum.
- 0x006989D0 GetPBuildingThingOccupying -> NCameraModes::GetPBuildingThingOccupying.
- 0x006991F0 FindInListAndInsertIfMatches -> CGameCameraManager::AddCameraPoint.
- 0x0069C8F0 _Move<Coin*,Coin*> -> std::__uninitialized_copy<CGameCameraManager::CQuake*>; 0x0069C970 fill<C3DAnimationStateAlpha*> -> std::__uninitialized_fill_n<CGameCameraManager::CQuake*, unsigned int, CQuake>.
- 0x0069D090 SoundMap_FindByCRC -> CSoundMap::GetPCriteriaString; 0x0069D300 CGameCameraManager_ConstructQuakeEntry -> vector<CGameCameraManager::CQuake>::_M_insert_overflow.
- Callees seen on the way (already landed under other names): 0x0041C930 is CSoundInit::CSoundInit(); 0x00415DD0 is CDefString::operator CCharString() const; 0x006928C0 is CCreatureActionBase::SetCreature(CThingCreatureBase&); 0x0065F604 is CDefClassBase::GetSubDef<CEntitySoundDef> (manifest <CAbilityDef>); 0x00696680 is the action registry's map::find (registry @ 0x013BA8C8, static init 0x0121E070).
