# Hand chunk 392 (CTCCreatureModeManager .. CWoundMorphs): 35 of 40 byte-exact

## What matched (35)
- **CTCCreatureModeManager** (0x0073AF00 ctor; 0x0073D720 / 0x00742360 / 0x00743470 / 0x00743EC0 sort of the 8-byte
  mode-registry pair; 0x0073DA50 CVectorMap::Count; 0x007421C0 PModes list::push_front). The ctor nulls the list head through the
  alloc proxy, then mallocs the sentinel. The frame-count store comes after the vector zero stores, so it is a body assignment.
  The STLport sort pieces are link-time `__fastcall`. `__median` returns a reference. The pair has a user copy ctor, so its
  assignment is "not trivial" and copy_backward is the `__copy_backward` count loop. Count is
  `UpperBound(key) - LowerBound(key)`, left operand first. The push row inserts at begin(): the `mov eax,[ecx] ; mov esi,[eax]`
  diff against the landed push_back shows that, so it is push_front.
- **CAnimFilterOfGroup / Of2Groups / Of3Groups** (0x0073DFF0, 0x0073E040, 0x0073E0D0): name assignment, then
  `CRC = Calc(0, name.GetPString(), name.GetLength())`. VC7.1 duplicates the last call's tail into both arms of the
  GetPString test. The 3-group ctor hashes the PARAMETERS, not the copied members.
- **NCreatureMode mode destructors** (0x00742930 ~CModeThunderLightningStorm, 0x00743D40 ~CModeHoverBase):
  `__declspec(novtable)` class plus an empty user dtor, so no own vtable store. Members die in reverse order. Vector element
  destroy is a virtual dtor call through the scalar deleting dtor in slot 0 (`push 0 ; call [eax]`). CDefPointer's dtor needs `Release()` as an INLINE
  MEMBER of the def (`dec [ecx+4]` with the def kept in ecx); a free helper reloads PDef (248v234). The STLport list dtor is
  `clear()` out of line, then `free(_M_node)` behind a null test.
- **CTCPerceiveThing::SetThingToPerceive** (0x007447A0): retail keeps a dropped `GetPerceivedThing()` call in the
  non-null arm. VC duplicates the SetPerceivedThing tail into both arms.
- **NStealth danger-pair family** (0x007485F0 / 0x00748690 `__find_if` with CFindDangerThing, non-const and const;
  0x00748A40 `__uninitialized_copy`; 0x00748AA0 `__uninitialized_fill_n`): random-access find_if is `>> 2` trips unrolled ×4
  plus a `switch (last - first)` fall-through tail. The empty `random_access_iterator_tag` / `__false_type` temporaries need a
  user empty ctor (otherwise VC stores a zero byte). The uninitialised loops use placement new with its null test and the
  inlined pair copy ctor.
- **CThing* CCompare_FurthestFromInitPos2D sort** (0x00748C10, 0x00748C90, 0x00749170, 0x00749530): the by-value empty
  comparator folds onto its argument slot ONLY when the landed operator() body (0x007483E0) is compiled EARLIER in the same TU
  under `#pragma auto_inline(off)`. With an external declaration VC copies the functor into a second slot (+4..+12 bytes).
  The insertion sort's copy_backward of pointers is `__copy_trivial_backward` (memmove).
- **NCreatureHiding::IsThemeHidable** (0x00749D20): `__fastcall`, `!map.IsPosPassable(C2DCoordI((long)X, (long)Y))`, an
  8-byte frame coordinate, `neg al ; sbb ; inc`.
- **CTCShop::AddShopper** (0x0074AC80): the find result must be a NAMED local. With `find(...) == end()` VC evaluates end()
  first and compares against the stale pre-call value.
- **GFGetAngleYZTo** (0x00754E70): the YZ twin of the landed GFGetAngleXYTo (0x00661540).
  `atan2(to.Z - from.Z, sqrt(dy*dy + dx*dx)) / 2pi`, with x and y as from - to and the y square formed first, then the
  landed `__forceinline WrapAngle`. Matched on the first grade.
- **CTCHeroRememberCrimes crime map** (0x007555F0 ctor with the inlined `_Rb_tree` ctor; 0x00756420 `_Rb_tree::erase(first, last)`;
  0x00756540 GFSerialiseMapBinaryOut<pair<long,long>,CCharString>): erase(first++) has the in-order successor inline.
  The serialiser is the landed 0x00632B30 template with the key/value traits roles swapped. This TU inlines
  `_Rb_global::_M_increment` on a COPY of the node.
- **CWoundMorphs 0x2C wound pair** (0x007577F0 Add, 0x00757870 GetPRandomWoundForLocation, 0x00758220 fill,
  0x007588A0 / 0x00758910 / 0x00758B70 / 0x00758C90 / 0x00759420 sort pieces): the pair's memberwise user copy ctor (key dword,
  then the 40-byte second as `rep movsd` ×10) versus the implicit assignment (`rep movsd` ×11). The empty `__false_type` is homed
  in a dead argument slot. The inline GFRandom LCG has its `range ? : 0` folded.

## Parked for Fable 5.1 (5)
- 0x00749620 vector<pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus> >::push_back DIFFER(98v105): retail has `this` in ebx and x in edi, loads end-of-storage into eax before finish (`mov eax,[ebx+8] ; mov esi,[ebx+4] ; cmp esi,eax`) and does a load/add/store `++finish` interleaved with the pops; ours has `this` in edi, x in ebx, `cmp esi,[edi+8]` and `add dword ptr [edi+4],10h` | tried: plain vector members, STLport _Vector_base + _STLP_alloc_proxy end-of-storage, implicit pair copy ctor, direct placement new, reversed compare, base-ctor init list | next: the wave-25 "24i" full _Vector_base/allocator_type() model with the overflow call through a non-inline `_M_insert_overflow(pos, x, __false_type(), 1, true)` taking `const __false_type&`; else lane gap: register allocation
- 0x0074A4E0 CTCCreatureGenerator::DoNavCheck DIFFER(233v234): retail loads the creature into ebx in place (`mov ebx,[esp+20h] ; mov al,[ebx+20h] ; add ebx,44h`), tests the count with `test edi,edi`, does `xor ebx,ebx` after the branch and does `sub edi,0Ch ; lea edx,[edi+eax]` before the copy; ours has `lea ebx,[eax+44h]`, a zeroed-ebx compare and the -0Ch folded into the lea | tried: early return vs block, if/else order (fixed the arm layout), separate loop count (fixed the ebp copy), GetTC with the HasTC test inside | next: a named `CInterfaceMap& map = creature.m_interfaces` pointer local before the HasTC test and `--last` as its own statement before the swap copy
- 0x0074FBB0 std::for_each<CThingObject**,CAddObjectsNotSolidTo> DIFFER(185v185): in the inlined interface lookup retail loads Last into edi before First (`mov edi,[eax+48h] ; mov esi,[eax+44h]`) with the object in eax; ours has the object in edi, First then Last in ecx, and re-reads `mov eax,[edi+48h]` | tried: plain inline operator() (called out of line, 63v185), __forceinline HasTC/GetTC, __forceinline operator() (full size), GetTC sharing one `last` with a free lower_bound helper (not inlined, 157v185) | next: a named `SEntry* last = map.Last;` local read before First in the inlined GetInterface; else lane gap: register allocation
- 0x00752510 CTCLook::GetHeadCreatureAnimFrame DIFFER(159v165): in the XY arm retail has `fmul [1/360] ; fmul [0.5]` on the def range, while VC7.1 folds (range*k)*0.5 into one pre-multiplied constant (the YZ arm and the clamp tail match) | tried: getter `range * k`, getter `k * range`, `range / 2.0f`, `0.5f * range` | next: make the 1/360 or the 0.5 unfoldable (an extern const float / class static for 1/360, or the half factor from a named float), or a non-inline GetHeadTurnRangeXY that is still `__forceinline`d after the fold pass
- 0x00756C60 std::_Rb_tree<CCharString,pair<long,long>>::erase(const key&) DIFFER(92v92): only the frame slots of the two by-value distance() iterator copies are swapped (retail first at +0, last at +4) | tried: const-ref distance (reloads p after the call), const pair, named locals, template distance + iterator_category, base-iterator model, reversed-parameter helper | next: STLport's `equal_range` returning `pair<iterator,iterator>` built by a two-argument pair ctor, then `distance(p.first, p.second, n)` (the 3-argument SGI form with a size_type out-param); else lane gap: frame-slot ordering

## Skipped
None (no row held two functions).

## Label corrections
- 0x0073AF00 CEngineSubPrimitive2DClockSprite_ctor2 -> CTCCreatureModeManager::CTCCreatureModeManager(CThing*)
- 0x0073D720 _Copy_impl<CNavigationPosition*,...> -> std::__unguarded_insertion_sort_aux (mode-registry pair)
- 0x0073DA50 VectorMap_GetDistance -> CVectorMap<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>::Count
- 0x0073DFF0 CRCHashedString_SetAndHash -> CAnimFilterOfGroup::SetCurrentGroupName
- 0x0073E040 CAnimFilterOf2Groups_Blend -> CAnimFilterOf2Groups::SetCurrentGroupNames
- 0x0073E0D0 CAnimFilterOf3Groups_Init -> CAnimFilterOf3Groups::CAnimFilterOf3Groups (ctor)
- 0x007421C0 ModeList_PushBack -> list<CCountedPointer<CModeBase> >::push_front
- 0x00742360 InsertionSort_Pair_Step -> std::__linear_insert
- 0x00742930 CModeStandBase_Destructor -> NCreatureMode::CModeThunderLightningStorm::~CModeThunderLightningStorm
- 0x00743470 PriorityQueue_Heap_Insert -> std::__partial_sort (mode-registry pair)
- 0x00743D40 NEntityEvents_CEventContainer_Destructor -> NCreatureMode::CModeHoverBase::~CModeHoverBase
- 0x00743EC0 Sort_IntrosortPhase -> std::__introsort_loop
- 0x007447A0 DeleteData -> CTCPerceiveThing::SetThingToPerceive
- 0x007485F0 / 0x00748690 CVector_Find_LinearProbe -> std::__find_if (CFindDangerThing; non-const / const)
- 0x00748A40 _Copy_backward<CIntelligentPointer<CThing>*,...> -> std::__uninitialized_copy (danger pair)
- 0x00748AA0 CVector_Construct_Range -> std::__uninitialized_fill_n (danger pair)
- 0x00748C10 Insertion_MoveElement -> std::__unguarded_insertion_sort_aux (CThing*)
- 0x00748C90 _Adjust_heap<CThingBuilding**,...,CIsHouseLargerThan> -> std::__adjust_heap<CThing**,...,CCompare_FurthestFromInitPos2D>
- 0x00749170 Vector_InsertSorted -> std::__insertion_sort (CThing*)
- 0x00749530 IntrosortPhase_FinalHeapSort -> std::__partial_sort (CThing*)
- 0x00749620 CTargetPoint_Vector_PushBack -> vector<pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus> >::push_back
- 0x00749D20 IsThemeHidable -> NCreatureHiding::IsThemeHidable (namespace)
- 0x0074A4E0 VertexBuffer_FilterNavigableVertices -> CTCCreatureGenerator::DoNavCheck
- 0x0074FBB0 _For_each<CThingObject**,CAddObjectsNotSolidTo> -> std::for_each (STLport spelling)
- 0x00752510 CTCLook_ComputeAnimationPlaybackRatio -> CTCLook::GetHeadCreatureAnimFrame(ECreatureAnim) const
- 0x00754E70 Math_AngleFromPoints -> GFGetAngleYZTo(const C3DVector&, const C3DVector&)
- 0x007555F0 CEngineSubPrimitive2DClockSprite_Constructor -> CTCHeroRememberCrimes::CTCHeroRememberCrimes(CThing*)
- 0x00756420 LTextBinTree_DeleteRange -> std::_Rb_tree<CCharString,pair<long,long>>::erase(iterator, iterator)
- 0x00756540 LTextBinTree_WriteBinary -> GFSerialiseMapBinaryOut<pair<long,long>,CCharString,less<CCharString> >
- 0x00756C60 StdMap_Erase -> std::_Rb_tree<CCharString,pair<long,long>>::erase(const CCharString&)
- 0x007577F0 _Pop_heap_0<CDebugSphere*,CDebugSphere> -> CWoundMorphs::Add
- 0x00757870 CVectorMap_FindRandomEntry -> CWoundMorphs::GetPRandomWoundForLocation
- 0x00758220 _Fill<CHeroMarriageInfo*,...> -> std::fill (0x2C wound pair)
- 0x007588A0 _Fill<CHeroMarriageInfo*,...> -> std::__unguarded_insertion_sort_aux (0x2C wound pair)
- 0x00758910 _Adjust_heap<CDebugBox*,...> -> std::__adjust_heap (0x2C wound pair)
- 0x00758B70 Sort_HeapsortPartition -> std::__unguarded_partition (0x2C wound pair)
- 0x00758C90 Heap_BuildHeap_From -> std::__make_heap (0x2C wound pair)
- 0x00759420 swap<CDebugSphere> -> std::__partial_sort (0x2C wound pair)
