# Hand chunk 383 (_global .. _global): 37 of 40 byte-exact

All 40 rows are catalog `_global` rows, and almost every catalog label was propagated or descriptive. The bodies
fall into six families:
- STLport 4.x containers and algorithms (deque, list, vector<C2DCoordI>, vector<CIntelligentPointer<CParticleEmitter>>,
  vector<long> and vector<ENavigatorType>, plus the sort, adjacent_find and random_shuffle instances);
- CTCVillage members;
- CSoundMap and CTCSoundPlayer;
- the CThingFilter bind operators;
- GF* maths;
- one CTCCoopSpirit physics row.

7 rows are MATCH and 30 are RELOCATION_MATCH. All 37 tests print PASS under g++ (tall.sh) and VC7.1 under Wine (tall.py).

## What matched (37)

### STLport fill, deque and list
- **std::fill<C2DCoordI*,C2DCoordI> 0x00654E67** (MATCH, 30)
  - Function: `__fastcall` fill used by vector<C2DCoordI>::_M_fill_insert.
  - Shape: the verbatim loop. The 8-byte POD assignment copies the two longs through esi, and the value pointer is loaded after the empty-range test. Size TU.
- **_Deque_base<C2DVector>::_M_initialize_map 0x00655516** (RELOC, 150)
  - Function: STLport verbatim; `(max)(8, num_nodes + 2)` goes through the const-reference max into two frame words with a `lea` select.
  - Pragmas: y off + s on, which gives `leave` and `push 8 ; pop ecx`.
- **list<NPlayerGui::CStatBar::CChangeText>::erase 0x00655A52** (RELOC, 44)
  - Function: unlinks the node, `_Destroy(&n->_M_data)` then deallocate.
  - Shape: CChangeText's implicit non-trivial destructor goes through its `??_G` with 0. The iterator (it has user ctors) is returned through the hidden pointer.
- **deque<pair<CCharString,long>>::_M_push_back_aux_v 0x006569DA** (RELOC, 102)
  - Function: STLport verbatim without `_STLP_TRY`. `__t_copy` in the frame gives the /Os ebp frame. _M_reserve_map_at_back 0x0065676F and the pair copy ctor 0x0050B7B0 are out of line.
- **deque<pair<CCharString,long>>::push_back 0x00656E0C** (RELOC, 50)
  - Function: `finish.cur != finish.last - 1` (the `sub eax,8`), with a placement copy, else the aux path. Size TU, no y pragma.

### STLport vectors (CIntelligentPointer<CParticleEmitter>, C2DCoordI, long)
- **vector<CIntelligentPointer<CParticleEmitter>>::_M_insert_overflow 0x006566C3** (RELOC, 172)
  - Shape: STLport verbatim. The empty `__false_type` temporaries sit in the dead high byte of the atEnd slot. That comes from same-TU `auto_inline(off)` noinline template bodies of the tag-taking helpers with the tag parameter unnamed.
- **vector<CIntelligentPointer<CParticleEmitter>>::_M_fill_insert 0x006568F2** (RELOC, 232)
  - Shape: the same empty-tag technique; the tags share the dead high byte of the position slot. `x_copy` is a frame local whose implicit dtor runs at the end.
  - Pragmas: y off + s on.
- **vector<C2DCoordI>::_M_fill_insert 0x00656A40** (RELOC, 219)
  - Shape: the same body over the POD element. VC7.1 tail-merges the two `fill(.., &x_copy)` calls.
  - The catalog called it a CObjectFamilyEntry range insert; it is not.
- **vector<long>::_M_allocate_and_copy<long*> 0x00660213** (RELOC, 45)
  - Shape: the POD `__uninitialized_copy` collapses onto `__copy_trivial` (memmove).
- **vector<long>::reserve 0x00660684** (RELOC, 108)
  - Function: STLport verbatim. n lives in ebp because this TU has no frame pointer.
- **std::__copy(_Rb_tree_iterator<long>, back_insert_iterator<vector<long>>) 0x006611F7** (RELOC, 51)
  - Shape: the by-value inserter (it has a user ctor) is returned through the hidden pointer. Its container pointer is held in ebx.

### CTCVillage, CSoundMap and the list algorithms
- **CTCVillage::GetNearestBuildingWithProperty 0x00658E38** (RELOC, 59)
  - Function: `for_each(begin, end, functor).NearestBuilding`. Arguments are evaluated right to left, so the functor and both iterators are each built in their slots.
  - Layout: a local CTCVillage model at +0x28, because the header's PDB list is 0xC opaque bytes.
- **CTCVillage::ClearCrimes 0x0065C2D0** (RELOC, 158)
  - Function: the GetTCIfPresent idiom (the deed log local is left unset when absent), then a set walk with end() cached in a named iterator. The handle is read before `++it`.
  - Pragmas: y off + s on.
- **std::__advance<_List_iterator<..>,int> 0x0065E8B8** (MATCH, 41)
  - Function: the STLport bidirectional __advance. Both loops compile to `dec ; jne` counters.
- **std::__find_if<list<CCountedPointer<CVillageTaskTorch>>::iterator, CIsThingInvolvedWithTask> 0x0065EA75** (RELOC, 97)
  - Shape: the inline predicate takes `const CCountedPointer<CVillageTaskBase>&`. The up-cast temporary of the `&&` condition gives the conditional destruction flag (`or [ebp-4],1`).
- **CSoundMap::GetPSoundPair 0x0065FBA4** (RELOC, 61)
  - Function: Find inline.
  - Shape: LowerBound's body must be in the same TU (`auto_inline(off)`). Otherwise VC re-reads the escaped key. The `!=` test order gives the `je` to the null return.
- **CTCVillage::IsThingInTaskList<CVillageTaskTorch> 0x0065FF5E** (RELOC, 48)
  - Function: `find_if(...) != end()`, with end() kept in esi and setne for the bool. y off + s on, like the landed sibling 0x0065FF8E.

### GF maths and creature physics
- **GFWorld3DDistanceSmallerThan 0x006614F0** (MATCH, 68)
  - Shape: three named float deltas in X, Y, Z order give retail's Z, Y, X squares. An inline C3DVector operator- temporary gives X, Z, Y instead.
- **GFGetAngleXYTo 0x00661540** (RELOC, 91)
  - Function: intrinsic atan2 times 1/2pi, stored once, then the landed `__forceinline WrapAngle`.
- **CThingCreatureBase::GetCurrentSpeed 0x00661CD0** (MATCH, 46)
  - Function: `(Position - OldPosition).GetLength()` through the landed inline C3DVector ops, with fsqrt.
- **CThingCreatureBase::AddQueuedAction 0x00664490** (RELOC, 85)
  - Function: `PQueuedActions.push_back(CCountedPointer<..>()); PQueuedActions.back().Reset(action.Clone());`
  - Shape: the zero-initialised counted-pointer temporary folds into immediate stores in the new node.

### CThingFilter bind operators
- **CFilter_Bind3<SolidToThing, PhysicsFlagsSolidToThing, OverlappingCircle>::operator()<CThingPhysical> 0x00669040** (RELOC, 183)
  - Shape: `(*F1)(t) && (*F2)(t) && (*F3)(t)`, with every filter operator `__forceinline`. Plain `inline` leaves calls (59v183).
  - SolidToThing reads only SourceThing.
  - PhysicsFlags checks both physics components, then vtable slot 57 (`+0xE4`, GetCollidesWithThings) or slot 58 (`+0xE8`, OneWay) on the source, then slot 57 on the thing.
- **CFilter_Bind2<OverlappingSphere, PosInContainmentVolume>::operator()<CThingBuilding> 0x00669110** (RELOC, 134)
  - Shape: the sphere operator is `__forceinline`; PosInContainmentVolume is called out of line at 0x006A7B20.
- **std::random_shuffle<CThing**, RandFunc> 0x00669850** (RELOC, 113)
  - Function: STLport `__fastcall`. RandFunc is in an anonymous namespace and holds a seed pointer.
  - Shape: the LCG must go through `__forceinline GFRandom(range, seed)` with the seed pointer passed by value. A member-seed form re-reads the seed after each store (124v113).

### CTCSoundPlayer (sound-event and sample players)
- **InternalPlaySoundEventAtPos 0x0066A920** (RELOC, 169)
  - Function: `if (g_pGameState) { emitter = CThingSoundEmitter::Create(pos, 0, true); if (emitter && bank.IsSoundEventValid(name)) { CSoundInit init; init.GainFaderBits = 0x8010; return emitter->PlaySoundEvent(...); } } return invalid;`
  - Shape: the CSoundInit default ctor (initialiser list for Volume, Pitch, Pan, NoPlays, PitchFaderBits and OverrideID; body sets PlaybackType=1, GainFaderBits=0) gives the eight immediate stores.
- **PlayUnattachedSample 0x0066ACE0** (RELOC, 176)
  - Function: an early `!sample` return.
  - Shape: named `thing` and `physics` locals give `lea ecx,[eax+0Ch]`; an inline GetPosition gives `add ecx,0Ch`.
  - The init is copied and set to 0x8010 when GainFaderBits == 0.
- **InternalPlayAttachedSample 0x0066B080** (RELOC, 148)
  - Function: CreatePAttachedEmitter(flag) plus the same copy-and-0x8010 rule.
- **InternalPlaySoundEvent 0x0066B120** (RELOC, 163)
  - Function: 0x0066A920 with the attached emitter and `init.OverrideID = overrideID`.
- **PlayAttachedSample(const CSoundPair&, ...) 0x0066B280** (RELOC, 116)
  - Function: `FirstSound + Random(Last - First + 1)`.
  - Shape: GetSampleBank goes into a named reference BEFORE the argument pushes. Otherwise the call lands after them.
- **NHeroInformationScreens::CManager::PlayScreenUpdateSound 0x0066E742** (RELOC, 87)
  - Function: a one-case switch (value 2 reads CPlayerGuiDef::InventoryNoMovementSound +0x3c), then PlaySample with a CSoundInit temporary.
  - Shape: /Os with locals gives the ebp frame.
  - The enumerator name for value 2 is a model name.

### vector<ENavigatorType> and its STLport algorithms
- **vector<ENavigatorType>::operator= 0x006714B0** (RELOC, 233)
  - Shape: the enum is non-POD to STLport, so __false_type gives counted `__copy` loops and placement-new `__uninitialized_copy` loops. All three copy helpers must be `__forceinline` (plain inline: 200v233).
- **vector<ENavigatorType>::_M_allocate_and_copy<const ENavigatorType*> 0x006715A0** (RELOC, 72)
  - Shape: the placement-new null-test copy loop, with plain inline helpers.
- **std::adjacent_find<ENavigatorType*, equal_to<ENavigatorType>> 0x00671BB0** (MATCH, 41)
  - Function: `__fastcall`, with the predicate by value.
- **vector<ENavigatorType>::erase(first, last) 0x00672070** (MATCH, 52)
  - Function: the counted __copy tail, then `_M_finish = new end`.
- **std::sort<ENavigatorType*> 0x006720B0** (RELOC, 77)
  - Function: `__fastcall`, with __lg inline and __introsort_loop / __final_insertion_sort declared only.
  - Shape: `less` needs a user ctor `less() { }`. Otherwise the empty temporaries emit `mov byte [esp+N],0`.
- **std::__final_insertion_sort<ENavigatorType*, less> 0x00672100** (RELOC, 123)
  - Shape: `__forceinline __insertion_sort` (inlined twice). __linear_insert and __unguarded_insertion_sort are out of line.
- **std::__unguarded_insertion_sort<ENavigatorType*, less> 0x00672180** (MATCH, 50)
  - Shape: `__forceinline` aux and __unguarded_linear_insert (signed `jge`/`jl`).

## Parked for Fable 5.1 (3)
- 0066bc10 CTCSoundPlayer::GetPotentialCriteriaStrings DIFFER(199v232): the first `names.push_back(id + ";CS_" + criteria)` stays a call (`push eax ; mov ecx,esi ; call`) where retail inlines both push_backs (null-tested copy ctor 0x0099EC30, `_M_insert_overflow` 0x00433530 with the tag in the dead argument slot), and the CDefOut slot is zeroed by an immediate where retail shares `xor edi,edi` with the holder | tried: `found = def && GetSubDef(out)` (249v232), `found = false; if (def) found = ...` (240v232), ternary (242v232), holder(0) assigned inside `if (def)` (199v232) | next: a same-TU `__forceinline` STLport push_back (out-of-line `_M_insert_overflow` declared) and build the holder and the out slot from one zero local
- 0066bdf0 CTCSoundPlayer::PlayUnattachedCriteria DIFFER(227v223): the lane keeps the CSE'd 0 in ebp to the end (`xor ebp,ebp` after the loop, `cmp edi,ebp` for the _M_start null test, end in eax/ebx), while retail zeroes edi only for the state test and the vector zero stores, keeps end in ebp, and tests `test edi,edi`; the mixed inline/out-of-line vector destructor already matches | tried: plain inline dtors (267v223), `__forceinline` dtors + `#pragma inline_depth(0)` around the found-handle return (227v223), plain _Vector_base dtor under the forceinline vector dtor (227v223), full STLport `_Vector_base(const allocator&)` / `_STLP_alloc_proxy` / deallocate null test (227v223) | next: lane gap: zero-register CSE live range (same gap as the landed sibling PlayAttachedCriteria 0x0066BD00)
- 00670470 CTCCoopSpirit::UpdateHeightAboveGround DIFFER(222v228): x87 only: retail stores the float gap (`fst dword [esp+10h]`, frame 14h v 10h), loads the int sign first (`fild`), and re-reads the gap from memory for the square, while the lane keeps the gap on the x87 stack across the sign branch and loads the sign after the clamp; a double clamp helper makes the lane load 1.0 as a qword where retail uses a dword | tried: sign/clamp as statements (222v228), clamp in an inline double helper (222v228), if/else sign + float-limit helper (222v228), sign ternary as the left operand with a float GFMin (224v228) | next: `--flags /Op` (the store and the memory re-read of the float gap look like float-consistency code) or a float-typed helper that takes the sign

## Skipped
None. Every row is a single function. The rows whose catalog length is longer than ours/retail are int3 alignment padding after a `ret`, not a second function. No row ends in a noreturn call, so no int3-after-noreturn was needed.

## Label corrections

| Address | Catalog label | Correct name |
|---|---|---|
| 0x00654E67 | `_Fill<CObjectFamilyEntry*,CObjectFamilyEntry>` | `std::fill<C2DCoordI*,C2DCoordI>` (STLport) |
| 0x00655516 | `CEngineInterpolationInfo_Initialize` | `std::_Deque_base<C2DVector>::_M_initialize_map` |
| 0x00655A52 | `CLandscapeBackground_RemoveNode` | `std::list<NPlayerGui::CStatBar::CChangeText>::erase` |
| 0x006566C3 | `std_vector_insert_IntelligentPointer_range` | `vector<CIntelligentPointer<CParticleEmitter>>::_M_insert_overflow` |
| 0x006568F2 | `std_vector_insert_IntelligentPointer_n` | `vector<CIntelligentPointer<CParticleEmitter>>::_M_fill_insert` |
| 0x006569DA | `CTCUserActionPointBase_InsertActionPoint` | `deque<pair<CCharString,long>>::_M_push_back_aux_v` |
| 0x00656A40 | `std_vector_insert_CObjectFamilyEntry_range` | `vector<C2DCoordI>::_M_fill_insert` |
| 0x00656E0C | `ComboMultiplierData_ConstructOrAppend` | `deque<pair<CCharString,long>>::push_back` |
| 0x00658E38 | `AIStateGroup_FindNearestBuildingWithProperty` | `CTCVillage::GetNearestBuildingWithProperty` |
| 0x0065C2D0 | `CTCHeroOpinionDeedLog_ProcessCrimeDeeds` | `CTCVillage::ClearCrimes` |
| 0x0065E8B8 | `BinaryTree_Navigate` | `std::__advance<_List_iterator<CIntelligentPointer<CThingBuilding>>,int>` |
| 0x0065EA75 | `Container_FindEntry` | `std::__find_if<list<CCountedPointer<CVillageTaskTorch>>::iterator,CIsThingInvolvedWithTask>` |
| 0x0065FBA4 | `CVectorMap_LowerBound_Faction` | `CSoundMap::GetPSoundPair` |
| 0x0065FF5E | `CPersistTraits_IsModified` | `CTCVillage::IsThingInTaskList<CVillageTaskTorch>` |
| 0x00660213 | `Vector_AllocateElements` | `vector<long>::_M_allocate_and_copy<long*>` |
| 0x00660684 | `CVector_Reserve` | `vector<long>::reserve` |
| 0x006611F7 | `MemoryAllocator_IterateAllocatedAreas` | `std::__copy<_Rb_tree_iterator<long>,back_insert_iterator<vector<long>>,int>` |
| 0x00661540 | `AngleBetweenPositions` | `GFGetAngleXYTo` |
| 0x00661CD0 | `Entity_GetDistance` | `CThingCreatureBase::GetCurrentSpeed` |
| 0x00664490 | `PhysicsSegment_AssignSegment` | `CThingCreatureBase::AddQueuedAction` |
| 0x00669040 | `CThingFilter_PhysicsDistance_Test` | `CFilter_Bind3<CThingFilter_SolidToThing,CThingFilter_PhysicsFlagsSolidToThing,CThingFilter_OverlappingCircle>::operator()<CThingPhysical>` |
| 0x00669110 | `CThingFilter_BoundingBox_Test` | `CFilter_Bind2<CThingFilter_OverlappingSphere,CThingFilter_PosInContainmentVolume>::operator()<CThingBuilding>` |
| 0x00669850 | `Shuffle_LinearCongruentialRNG` | `std::random_shuffle<CThing**,RandFunc>` |
| 0x0066A920 | `GetAIStateGroupTypeConditional` | `CTCSoundPlayer::InternalPlaySoundEventAtPos` |
| 0x0066ACE0 | `AI_PlaySoundAtCreatureWithStateGroup` | `CTCSoundPlayer::PlayUnattachedSample` |
| 0x0066B080 | `Sound_PlayAttached` | `CTCSoundPlayer::InternalPlayAttachedSample` |
| 0x0066B120 | `CTCSoundPlayer_GetSoundState` | `CTCSoundPlayer::InternalPlaySoundEvent` |
| 0x0066B280 | `CAnimateThing_PlayRandomSound` | `CTCSoundPlayer::PlayAttachedSample(const CSoundPair&, ...)` |
| 0x0066BC10 | `Sound_AppendAbilityNameToDescription` | `CTCSoundPlayer::GetPotentialCriteriaStrings` |
| 0x0066BDF0 | `CParticleMorphs_LoadAndProcessMorphs` | `CTCSoundPlayer::PlayUnattachedCriteria` |
| 0x0066E742 | `FrontEnd_PlaySoundEffect` | `NHeroInformationScreens::CManager::PlayScreenUpdateSound` |
| 0x00670470 | `CThingCreatureBase_UpdateGravity` | `CTCCoopSpirit::UpdateHeightAboveGround` |
| 0x006714B0 | `CActiveFile_Assign` | `vector<ENavigatorType>::operator=` |
| 0x006715A0 | `Vector_MallocAndCopy` | `vector<ENavigatorType>::_M_allocate_and_copy<const ENavigatorType*>` |
| 0x00671BB0 | `_Unique<ENavigatorType*>` | `std::adjacent_find<ENavigatorType*,equal_to<ENavigatorType>>` |
| 0x00672070 | `Vector_Erase` | `vector<ENavigatorType>::erase(first, last)` |
| 0x006720B0 | `Vector_SortIntroSort` | `std::sort<ENavigatorType*>` |
| 0x00672100 | `Vector_InsertSorted` | `std::__final_insertion_sort<ENavigatorType*,less<ENavigatorType>>` |
| 0x00672180 | `_Copy_impl<CComplexNumber*,CComplexNumber*>` | `std::__unguarded_insertion_sort<ENavigatorType*,less<ENavigatorType>>` |

0x006614F0 `GFWorld3DDistanceSmallerThan` was already correct.

Callee labels found along the way:

| Address | Manifest label | Correct name |
|---|---|---|
| 0x0065676F | `CTCUserActionPointBase::GetNumberOfActionPointsInUse` | `deque<pair<CCharString,long>>::_M_reserve_map_at_back` |
| 0x0050B7B0 | `CComboMultiplierData` (identical body) | `pair<CCharString,long>` copy ctor |
| 0x0065605F | `CActiveFile::OnReadFinished` | `vector<CIntelligentPointer<CParticleEmitter>>::_M_clear` |
| 0x00666D40 | — | `CThingCreatureBase::GetHeightZBelowAt(const C3DVector&, bool, bool) const` |
| 0x006715F0 | `CActiveFile_OnReadFinished` (landed stub) | `vector<ENavigatorType>::_M_clear` |

## Errors in already-landed sources (for the landing session)
- **0x0065F8B2:** filed as `CEngineSceneGrid::GetCellBoundingBox`, but it is `std::advance`.
- **0x0065F943:** filed as `unattributed_ConstructCRangeString`, but it is `std::find_if`.
- **0x0065605F:** filed as `CActiveFile_OnReadFinished`, but it is `vector<CIntelligentPointer<CParticleEmitter>>::_M_clear` (destroy [start, finish), then free).
- **0x006715F0:** the landed `CActiveFile_OnReadFinished_006715f0.cpp` is a stand-in stub body. The function is `vector<ENavigatorType>::_M_clear`.
- **0x00654E90, 0x006556A8 and 0x0065F96D:** the comments say Dinkumware. They are STLport (`__uninitialized_fill_n`, `__uninitialized_copy` and the STLport algorithm).
- **0x0066B1D0 PlayAttachedSoundEvent:** passes `CSoundInit* init` as the third argument of InternalPlaySoundEvent. The PDB signature (`_N22J`) says that argument is a bool (flag).
- **0x0066BD00 PlayAttachedCriteria and 0x008A8F20:** both landed with non-retail stand-in bodies. 0x0066BD00 has the same zero-register gap as the parked 0x0066BDF0.
