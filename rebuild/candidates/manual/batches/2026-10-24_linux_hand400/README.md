# Hand chunk 400 (CParticleMorphs .. CThingAICreature): 38 of 40 byte-exact

## What matched (38)

- **CParticleMorphs / CVectorMap** (0x0081B490 CParticleMorphs::Add, 0x0081D820 CVectorMap<EHeroMorphType,CParticleMorphs::CEntry>::Add):
  both append a (key, 36-byte entry) pair to the map's vector, set Dirty and re-sort the map. Shape: the landed
  CVectorMap::Add 0x00759570 form. The pair temporary is built FIRST in the frame (the key dword, then `rep movsd` x9).
  STLport `push_back` is inline, and its empty `__false_type` is homed in a dead argument slot.
- **STLport sort/heap helpers over keyed pairs** (12-, 24- and 40-byte (key, payload) elements of the morph maps):
  - 0x0081B730 and 0x0081B800 `__unguarded_partition` (both MATCH): the inlined `__iter_swap` temporary sits in the
    frame or the dead pivot slot.
  - 0x0081BAD0, 0x0081BB50 and 0x0081BBD0 `__make_heap`: the landed 0x00758C90 form.
  - 0x0081BC90 `__pop_heap_aux`, needing `__forceinline __pop_heap`.
  - 0x0081BE80 `__linear_insert`.
  - 0x0081D450 and 0x0081D4D0 `__partial_sort`: `__pop_heap` is a call for the 24-byte pair and inline for the
    40-byte one.

  The lever common to all of them is STLport `pair`'s memberwise user copy constructor. It builds every by-value
  element in the argument area: the key dword, then the payload as `rep movsd`, or through its own address when the
  payload is trivially copyable.
- **vector<SRecord20>::_M_insert_overflow** (0x0081C270): STLport 4.x verbatim, as the landed CQuake twin 0x0069D300
  but with the `fill_len == 1` `_Construct` inline. The uninitialized copy and fill helpers are same-TU noinline bodies,
  and the `__false_type` temporaries are homed in the `atend` slot.
- **CReplaceSerialise version chain** (the scripted-brush and height-map-cell records):
  - 0x00825590 and 0x00825700 `ConvertPreviousUnknownVersionUp`, from the landed 0x00518040 template. A user default
    constructor on the record decides the registers. The brush (C2DBoxI's empty inline ctor) keeps `held` in ebx/ebp,
    while the ctor-less cell keeps `result.object` in ebx.
  - 0x008256A0 and 0x00826560 `GetMaxSize`, from the landed 0x00519630 template. MAX is the evaluate-twice macro and
    the whole version chain is defined in the TU, so the inline depth matches. The compares are unsigned because they
    involve `sizeof`.
  - 0x00825D40 and 0x008273D0 `ConvertToThisVersion`, from the landed 0x00519510 template. The implicit operator= is
    `rep movsd` for 73 bytes and register moves for 21 bytes.
- **STLport _Rb_tree for CScriptedMapBrush's thing sets and move map**:
  - 0x008259C0 and 0x008260C0 `_M_copy`. In the second, VC keeps the first clone as a call and inlines the clone in
    the loop.
  - 0x00825A80 `_M_clone_node`.
  - 0x0082F150 and 0x0082F7B0 `_M_insert`. The u64 set's node copy is inline. For the 32-byte pair, `_M_create_node`
    (0x00824FB0) is a same-TU noinline call.

  C3DVector needs a memberwise user copy constructor to get the direct `[ecx+N]` stores.
- **CScriptedMapBrush**:
  - 0x0082E500 RemoveThing does three find/erase blocks. Lever: STLport 4.6 `find` is an inline wrapper over the
    out-of-line `_M_find` (0x0082EEB0 / 0x0082EF20), which returns the node in eax. A real iterator return goes
    through a hidden pointer and gave 216v183.
  - 0x0082E600 is the init constructor: `Cells(width * height)`, the three trees' inline constructors, C2DBoxI's
    memberwise copy.
  - 0x0082E6E0 is the default constructor. It needs C2DBoxI's empty user ctor.
  - 0x0082F630 is `vector<CScriptedMapCell>(n)`: `_Vector_base(n, a)`, with `_Tp()` as five zero dwords and a same-TU
    noinline fill.
- **Navigation regions**:
  - 0x008285D0 CRegionConnectionNode::SetAsConnectedToRegion is find + push_back, or remove + range erase. It uses the
    landed 0x007CB850 model, with STLport remove as find plus an inline remove_copy, and `erase(first, last)` through
    `__copy_trivial`.
  - 0x008286A0 CRegionConnectionGraph::SetRegionsAsConnected needs a same-TU noinline LowerBound (0x00827B00) body.
    That keeps the region ids and the flag in ebp/edi/ebx.
- **Minimap**:
  - 0x0082A500 CMiniMapDisplay::AddAutoMarker is `if (graphicIndex > 0) Markers.push_back(CMarkerInfo(&thing, idx,
    true))` with the list insert noinline.
  - 0x0082C920 is `list<CMarkerInfo>::insert`, as an explicit instantiation. CMarkerInfo is a nested `class`.
  - 0x0082C980 is `vector<CMinimapData::MinimapTheme>::operator=`. `__copy` and `__copy_ptrs` must be
    `__forceinline`; with plain inline it was 206v222.
  - 0x0082CB70 and 0x0082CC40 are `__uninitialized_copy` and `__uninitialized_fill_n` of CMinimapData. C2DExtentsI
    needs a memberwise user copy ctor to get the strength-reduced cursor addressing. An explicit-specialisation
    declaration keeps the MinimapTheme vector copy ctor (0x0082C620) out of line.
- **CThingBuilding::Create** (0x0082D920): the landed CThingDeadCreature::Create 0x00835C80 form, which needed these
  levers:
  - `T* result = 0;` declared AFTER `Alloc()`.
  - CRightHandedSet with an assignment-body ctor, and C3DVector as a POD.
  - A same-TU noinline CTCParamsPhysicsBase ctor.
- **CThingAICreature**:
  - 0x00833230 IsMessageEventWithinSightRange and 0x008332E0 IsMessageEventWithinHearingRange are STLport `min` of the
    component and event radius, then a squared-distance compare. Lever: the squared length is accumulated Z*Z, then
    Y*Y, then X*X.
  - 0x00833800 Create uses `new (false) CThingAICreature` and `InitialPos` (+0xE8), with `result = 0` after the
    allocation. It is a sibling of 0x0082D920.

## Parked for Fable 5.1 (2)

- 0081e550 FtoL DIFFER(25v29): retail rounds with a bare `fistp dword ptr [edx]` through a result pointer (an x87 store VC7.1 cannot emit from C++: `(long)x` calls `__ftol2` and `/QIfist` emits `fistp qword`) | tried: a C++ reference body using the 1.5*2^52 magic-number rounding (graded once, as the brief allows) | next: lane gap: retail_asm candidate, the same helper as the landed retail_asm CSprite_00b1b5e0::GetHeight 0x00B1B5E0; land as `retail_asm` with a minimal `fistp` `__asm` and a `// Retail-asm:` line
- 0082fb90 map<u64,CScriptedMapBrush::CThingPosAndOrientation>::operator[] DIFFER(192v192): scheduling only; the six-dword copy of the default value / pair loads in a different order (retail +4 before +0, with the key loads in between), and its stores interleave differently with the pushes | tried: C3DVector with a memberwise user copy ctor (192v192), POD C3DVector (186v192) | next: an explicit `CThingPosAndOrientation` member copy order, or a named intermediate `_Tp` local before `insert(i, value_type(k, _Tp()))`. Model: lower_bound -> `_M_lower_bound` 0x0082F0D0, `__default_constructed` 0x0082F5C0 (__fastcall, ecx = 0, hidden return), `insert_unique(iterator, v)` 0x0082F930

## Skipped

None. No row holds two functions, and no row is cut wrong (every row is one body ending in `ret` plus int3 padding).

## Label corrections

Catalog label -> real identity:

- **Propagated `_Pop_heap_0<CDebugSphere*,CDebugSphere>`:**
  - 0x0081B490 -> CParticleMorphs::Add
  - 0x0081BC90 -> STLport `__pop_heap_aux` (40-byte keyed pair)
  - 0x0081D820 -> CVectorMap<EHeroMorphType,CParticleMorphs::CEntry>::Add
- **`swap<CDebugSphere>`:** 0x0081D4D0 -> STLport `__partial_sort` (40-byte keyed pair)
- **Descriptive names, now STLport identities:**
  - 0x0081B730 / 0x0081B800 -> `__unguarded_partition`
  - 0x0081BAD0 / 0x0081BB50 / 0x0081BBD0 -> `__make_heap`
  - 0x0081BE80 -> `__linear_insert`
  - 0x0081C270 -> `vector::_M_insert_overflow`
  - 0x0081D450 -> `__partial_sort`
- **CAFile_* labels, really CReplaceSerialise<CFileFormatBrush,COldFileFormatBrush>:**
  - 0x00825590 CAFile_ConstructRigidBody -> ConvertPreviousUnknownVersionUp
  - 0x00825D40 CAFile_ReadRigidBody -> ConvertToThisVersion
- **0x00825700 CAFile_ConstructPrimitive:** -> CReplaceSerialise<COldFileFormatHeightMapCell2,COldFileFormatHeightMapCell1>::ConvertPreviousUnknownVersionUp
- **Rb-tree internals:**
  - 0x008259C0 Linked_CloneNode -> `_Rb_tree<u64>::_M_copy`
  - 0x00825A80 Linked_CloneNode -> `_Rb_tree<u64, pair<const u64, CThingPosAndOrientation>>::_M_clone_node`
  - 0x008260C0 Linked_CloneNodeLarge -> the same map's `_M_copy`
- **Navigation:**
  - 0x008285D0 StdVector_RemoveIf -> CRegionConnectionNode::SetAsConnectedToRegion
  - 0x008286A0 Navigation_ConnectRegionsInGraph -> CRegionConnectionGraph::SetRegionsAsConnected
- **Minimap:**
  - 0x0082A500 Map_InsertMarker -> CMiniMapDisplay::AddAutoMarker (`labels_` already had it)
  - 0x0082C920 Map_InsertMarker -> `list<CMiniMapDisplay::CMarkerInfo>::insert`
  - 0x0082C980 std_Move_UintVector -> `vector<CMinimapData::MinimapTheme>::operator=`
  - 0x0082CB70 StdVector_CopyDeep -> `__uninitialized_copy<CMinimapData>`
  - 0x0082CC40 CEditPrimitiveStats_CopyPrimitiveArray -> `__uninitialized_fill_n<CMinimapData>`
- **Bare `Create` labels:**
  - 0x0082D920 -> CThingBuilding::Create
  - 0x00833800 -> CThingAICreature::Create
- **CScriptedMapBrush:**
  - 0x0082E500 RBTree_EraseMultiple -> CScriptedMapBrush::RemoveThing(unsigned __int64)
  - 0x0082E600 CScriptedMapBrush_CreateFromDefinition -> CScriptedMapBrush::CScriptedMapBrush(const CScriptedMapBrushInit&)
  - 0x0082E6E0 CBaseIntelligentPointer_ctor_WithVectors -> CScriptedMapBrush::CScriptedMapBrush()
  - 0x0082F150 CMemoryAllocatorVariableSize_InsertNode -> `_Rb_tree<u64>::_M_insert` (set)
  - 0x0082F630 CTextureManagerPoolInfo_Vector_Constructor -> `vector<CScriptedMapBrush::CScriptedMapCell>(n)`
  - 0x0082F7B0 RBTree_Insert -> `_Rb_tree<u64, pair<const u64, CThingPosAndOrientation>>::_M_insert`
  - 0x0082FB90 CThing_CalculateMovementTiming -> `map<u64, CThingPosAndOrientation>::operator[]`
- **CThingAICreature:**
  - 0x00833230 CVertexBufferWin32_DistanceCheck -> CThingAICreature::IsMessageEventWithinSightRange
  - 0x008332E0 CVertexBufferWin32_ComputeWaterSurfaceMetrics -> CThingAICreature::IsMessageEventWithinHearingRange
- **Neighbours identified while modelling (outside the chunk):**
  - 0x0081E4F0 (catalog CRigidBodyFileRecord::CopyFrom) -> CFileFormatBrush::ConvertFromOld
  - 0x00824C50 / 0x00824D30 (CEngineWorldMap::CreateNewBankMap) -> CBaseSerialise ConvertPreviousUnknownVersionUp
  - 0x008271E0 (CAFile_ConvertNavConnectorNode) -> CReplaceSerialise<CFileFormatHeightMapCell,COldFileFormatHeightMapCell6>::ConvertPreviousUnknownVersionUp
  - 0x00824CE0 -> CReplaceSerialise<Cell3,Cell2>::GetMaxSize
  - 0x0082C620 Vector_CopyInit -> the `vector<MinimapTheme>` copy constructor
  - 0x0082C450 -> `vector<MinimapTheme>::_M_allocate_and_copy`
  - 0x0082EEB0 / 0x0082EF20 -> `_M_find`
  - 0x0082F0D0 -> `_M_lower_bound`
  - 0x0082F5C0 -> `__default_constructed<CThingPosAndOrientation>`
  - 0x0082F930 -> `insert_unique(iterator, const value_type&)`
  - 0x00824FB0 -> the map `_M_create_node` (landed as "BuyNode")
  - 0x00744440 -> CTCPerceiveThing::GetSoundRadius (SoundRadius at +0x3C)
