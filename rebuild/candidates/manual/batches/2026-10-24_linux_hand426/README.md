# Hand chunk 426 (CParticleProgram .. CDynamicObjectHeap): 40 of 40 byte-exact

All 40 rows grade MATCH (7) or RELOCATION_MATCH (33) through gall.sh (grades.txt). Every row's behaviour test prints
`<addr>_TEST PASS` under g++ (tall.sh) and under VC7.1/Wine (tall.py). No row needed more than 3 grades.

## What matched (40)
- **NParticleEngine::CParticleProgram** (4):
  - 0x00AE0680 `WriteGroups` and 0x00AE1210 `GetUsedBones` are the static `__fastcall` helpers (Ego_r PDB names) over
    the retail 16-byte C3DGroup2 = {BoneIndex, CArray<C3DVertexBlend2>}. CArray is an STLport vector of 12 bytes.
  - 0x00AE2510 is `ApplyConstraints(CParticleSimulation&, long iterations, float tolerance) const` (`ret 0Ch`). It
    zeroes MaximumAdjustment (+0x30), runs ExecuteProgram, and stops once the adjustment is within the tolerance.
  - 0x00AE2D90 `AddExportParticle` only matched with `__forceinline` on the in-class `CVectorMap::Add`. VC kept the
    plain inline member out of line (24v137).
- **STLport sort helpers over `pair<long, unsigned long>`** (6): 0x00AE5AB0, 0x00AE5F00, 0x00AE5F60, 0x00AE6760,
  0x00AE7760, plus `__destroy_aux<set<long>*>` 0x00AE76E0.
  - All are explicit `__fastcall` templates in `_STL`, passing the comparator by value.
  - `__iter_swap` goes through `__value_type`.
  - `copy_backward` is inlined as a counted loop.
  - `__pop_heap` is inline over the out-of-line `__adjust_heap` 0xAE5FB0.
  - In `__destroy_aux`, the recursive `_M_erase` body sits in the TU. One level is inlined and the recursion calls
    0x004CF810.
- **STLport vector members**:
  - `vector<set<long> >`: the sized constructor 0x00AE7880.
  - `vector<C3DGroup2>`:
    - `reserve` 0x00AE80F0, which uses `_M_allocate_and_copy`.
    - `_M_insert_overflow` 0x00AE81F0. Same-TU noinline `__uninitialized_copy`/`fill_n` bodies home the `__false_type`
      temporaries.
    - `push_back` 0x00AE83D0. The `_M_insert_overflow` body in the TU is auto-inlined under /O2.
    - `resize(n)` 0x00AE8540 = `resize(n, T())`, over out-of-line erase 0xAE7F30 and insert 0xAE82C0.
  - `vector<pair<CCharString, CRegisteredPSCInfo> >`: `_M_insert_overflow` 0x00AF37F0, and `push_back` 0x00AF39C0,
    which declares the overflow out of line.
- **STLport uninitialized algorithms** (3):
  - `__uninitialized_fill_n<CParticleEmitterInfo*>` 0x00AF1AA0 and `__uninitialized_copy<CParticleEmitterInfo*>`
    0x00AF1C10. The element is Ego_r CParticleEmitterInfo {CIVCountedPointer Data, Bank, Index}.
  - `__uninitialized_copy<pair<EPSCType, CComponentInstance>*>` 0x00AF6920. Its one vtable store (0x0129F478) is
    hoisted into edx.
  - All three are STLport verbatim: unsigned count `jbe`, and the placement-new null test.
- **CPSCPair sort helpers** (6): 0x00AF2E70, 0x00AF31C0, 0x00AF34E0, 0x00AF35A0, 0x00AF3920, 0x00AF3AA0
  (`__unguarded_linear_insert` / `__unguarded_partition` / `__make_heap` / `__linear_insert` / `__partial_sort` /
  `__introsort_loop`).
  - The comparator `CKeyPairCompareLess` compares the CCharString keys, with its operator() as a same-TU noinline body.
  - The partition's swap goes through the declared `swap<CPSCPair>` 0xAF2CD0.
- **NParticleEngine::CParticleEmitterDatabase::DeleteEmitterTemplate** 0x00AE8B50: `Emitters.Erase(handle)` with the
  same-TU noinline LowerBound/UpperBound bodies (the reload control). The erase uses the declared `__copy` 0xAE9CB0 and
  `__destroy_aux` 0xAE9D50.
- **NParticleEngine globals**:
  - `SetEnvironmentInteraction` 0x00AEAB20 (`__fastcall`, by-value counted pointer into PEnvironmentInteraction
    0x013D2E2C). ReleaseData zeroes Block and Data; the parameter's destructor is the `dec ; jne` form.
  - `Initialise` 0x00AEBE20: three `P.Reset(new T)` calls.
  - `Uninitialise` 0x00AEBEA0: three inline ReleaseData calls in the order Manager, Database, PSC.
- **CParticleEmitterList / CParticleEmitterManager**:
  - List constructor 0x00AEB940: STLport `_List_base` sentinel, then MaxActiveEmitters.
  - Manager constructor 0x00AEBA90: `ActiveEmitters(0x80), PerlinNoise(0), MaxVisibleEmitters(32), ...`, with the
    bit-fields set in the body.
  - `list::push_front` 0x00AEC740 = `insert(begin(), x)` inline.
  - `??_GCParticleEmitterManager` 0x00AEC9F0 and `CCountedPointer<CParticleEmitterManager>::DeleteData` 0x00AECA50 come
    from ONE source. `delete (T*)data` in the static DeleteData, with a user-declared inline destructor, makes VC7.1 emit
    the class's non-virtual `??_G` as its own COMDAT. The two rows differ only in their `// Leaf:`.
- **NParticleEngine::CParticleEmitter::GetFadeAlpha** 0x00AECD70 (FableWin PDB name): the `||` of the two out-of-range
  tests puts the shared `return 0.0f` at the end.
- **NParticleEngine::CPSCManager::AllocatePSC** 0x00AF28C0 took 3 grades:
  - The same-TU noinline `CBasicString<char>::operator<` body (landed 0x00429950) keeps the end pointer in esi.
  - `if (it != Last) return it->second.AllocFunc(); return 0;` fixes the return-block order.
- **NParticleEngine::GFInterpolate(const C3DVector&, const C3DVector&, float)** 0x00AF3CF0 (FableWin PDB name):
  `a * (1.0f - t) + b * t` over inline C3DVector operators. It matched on the first grade; the right operand is
  evaluated first.
- **NParticleEngine::CParticleSystem::GetComponentOfType(EPSCType, unsigned long)** 0x00AF4360 (FableWin PDB name):
  - The equal-range walk over LowerBound 0xAF60C0 and UpperBound 0xAF6120.
  - The positive final test `if (it != last) return ...; return 0;` merges both `return 0`s at the end (2 grades).
- **CDynamicObjectHeap<24,853>::CDynamicObjectHeap(bool)** 0x00AF8700 (FableWin PDB name; RTTI 0x0129F638
  `.?AV?$CDynamicObjectHeap@$0BI@$0DFF@@@`): the real template over CDynamicObjectHeapBase (ctor 0xF36C60), in the
  same body order as the landed <116,529> twin.

## Parked for Fable 5.1 (0)
None.

## Skipped
None. No row holds two functions. 0x00AF4360's trailing 0xCC is padding (79-byte body in an 80-byte row).

## Label corrections
- Chunk rows (catalog label -> name). Each name is from the PDB name sets (FableWin / Ego_r) where one exists, otherwise
  descriptive:
  - 0x00AE2510 "ParticleProgram_ExecuteLooped" -> CParticleProgram::ApplyConstraints(CParticleSimulation&, long, float) const (Ego_r PDB name; `ret 0Ch`)
  - 0x00AE2D90 "CParticleMorphs_InsertEntry" -> CParticleProgram::AddExportParticle(const CCharString&, unsigned long)
  - 0x00AE5AB0 "InsertionSort_Insert64Bit" -> _STL::__unguarded_linear_insert<pair<long,unsigned long>*, ...>
  - 0x00AE5F00 "QuickSort_Partition64Bit_Swap" -> _STL::__unguarded_partition<pair<long,unsigned long>*, ...>
  - 0x00AE5F60 "InsertionSort_64Bit" -> _STL::__unguarded_insertion_sort_aux<...>
  - 0x00AE6760 "InsertionSort_InsertBefore" -> _STL::__linear_insert<...>
  - 0x00AE76E0 "LTextTreeWalkThrough_DestroyRange_Range" -> _STL::__destroy_aux<_STL::set<long>*>
  - 0x00AE7760 "Std_PartialSort_Pair64BitMax" -> _STL::__partial_sort<...>
  - 0x00AE7880 "Vector_Construct_Reserve" -> _STL::vector<_STL::set<long> >::vector(unsigned int)
  - 0x00AE80F0 "Container_Reserve" -> _STL::vector<C3DGroup2>::reserve
  - 0x00AE81F0 "Vector_Insert_Pair128BitMultiple" -> _STL::vector<C3DGroup2>::_M_insert_overflow
  - 0x00AE83D0 "Vector_PushBackExpand" -> _STL::vector<C3DGroup2>::push_back
  - 0x00AE8540 "Vector_Resize_Pair128Bit" -> _STL::vector<C3DGroup2>::resize(unsigned int)
  - 0x00AE8B50 "CVectorMap_EraseByRange" -> NParticleEngine::CParticleEmitterDatabase::DeleteEmitterTemplate(long)
  - 0x00AEB940 "StdList_ConstructWithArg_aeb940" -> NParticleEngine::CParticleEmitterList::CParticleEmitterList(long)
  - 0x00AEBA90 "NScript_CPredicate_And_Initialize" -> NParticleEngine::CParticleEmitterManager::CParticleEmitterManager()
  - 0x00AEC740 "List_InsertNode" -> _STL::list<CCountedPointer<CParticleEmitter> >::push_front
  - 0x00AEC9F0 "NEntityEvents_CEventList_Destroy" -> ??_GCParticleEmitterManager@NParticleEngine@@QAEPAXI@Z
  - 0x00AECA50 "NEntityEvents_CEventList_Destroy" -> ?DeleteData@?$CCountedPointer@VCParticleEmitterManager@NParticleEngine@@@@SIXPAX@Z
  - 0x00AECD70 "IsVisible_CalcFadeAlpha" -> NParticleEngine::CParticleEmitter::GetFadeAlpha(float) const
  - 0x00AF1AA0 "_Fill_n<unsigned_long*,...>" -> _STL::__uninitialized_fill_n<NParticleEngine::CParticleEmitterInfo*, unsigned int, ...>
  - 0x00AF1C10 "_Copy_impl<CNavigationPosition*,...>" -> _STL::__uninitialized_copy<NParticleEngine::CParticleEmitterInfo*, ...>
  - 0x00AF28C0 "CVectorMap_FindEntry" -> NParticleEngine::CPSCManager::AllocatePSC(const CCharString&)
  - 0x00AF2E70 "TreeNode_InsertPosition" / 0x00AF31C0 "QuickSort_Partition" / 0x00AF34E0 "Heap_BuildHeap" /
    0x00AF35A0 "TreeNode_InsertSorted" / 0x00AF3920 "HeapSort_Phase" / 0x00AF3AA0 "QuickSort_IntroSort_16Byte" ->
    STLport `__unguarded_linear_insert` / `__unguarded_partition` / `__make_heap` / `__linear_insert` / `__partial_sort`
    / `__introsort_loop` over pair<CCharString, CRegisteredPSCInfo>
  - 0x00AF37F0 "CCombatWheel_ResizeRings" -> _STL::vector<pair<CCharString, CRegisteredPSCInfo> >::_M_insert_overflow
  - 0x00AF39C0 "Vector_PushBackEntry" -> _STL::vector<pair<CCharString, CRegisteredPSCInfo> >::push_back
  - 0x00AF3CF0 "InterpolateRadialBlurPositionParams" -> NParticleEngine::GFInterpolate(const C3DVector&, const C3DVector&, float)
  - 0x00AF4360 "Map_CountRangeEntries" -> NParticleEngine::CParticleSystem::GetComponentOfType(EPSCType, unsigned long)
  - 0x00AF6920 "CopyElements_6DwordStruct" -> _STL::__uninitialized_copy<pair<EPSCType, CComponentInstance>*, ...>
  - 0x00AF8700 "CDynamicObjectHeapBase_Construct_Small" -> CDynamicObjectHeap<24,853>::CDynamicObjectHeap(bool)
- Callees and neighbours seen while working:
  - 0x00AF36C0 "CCombatWheel::ResetRings" = vector<CPSCPair>::_M_clear.
  - 0x004CF810 "LTextTreeWalkThrough::BuildTreeArray" = _Rb_tree<long,...>::_M_erase.
  - 0x00AC40A0 "C3DMeshFileGroupChunk::GetList" = C3DGroup2::PeekVertexBlends.
  - 0x00AF2CD0 = swap<CPSCPair>; 0x00AF2E60 = iter_swap; 0x00F36BE0 = NFractal::CPerlinNoise(unsigned long);
    0x00AD3A10 = the _Rb_tree copy constructor.
  - 0x00AF60C0 = CVectorMap<EPSCType, CComponentInstance>::LowerBound (twin of the landed UpperBound 0x00AF6120).
  - 0x00AF1E90 = vector<CParticleEmitterInfo>::_M_insert_overflow.
  - The only push_front caller (call at 0x00AEB159) is inside 0x00AEB060 (catalog "CParticleEmitterList::Insert"), not
    0x00AEB190.
