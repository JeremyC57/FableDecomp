# Hand chunk 372 (std::vector<CCharString> .. std::iter_swap<pair<float,CCameraTag>*>): 37 of 40 byte-exact

All 40 rows are `_global` catalog rows. 37 grade MATCH (8) or RELOCATION_MATCH (29) and pass their behaviour tests under
g++ (tall.sh) and VC7.1/Wine (tall.py). 3 are parked in differ/ and none were skipped. Bounds: all 40 rows are single
functions; trailing `cc` padding trimmed (lengths in STATUS.tsv).

## What matched (37)
- **std::vector<CCharString>** (0x4B83E0 `_M_allocate_and_copy`, 0x4B8770 `erase(pos)`, 0x4BA7A0 `reserve`): STLport
  bodies with malloc-backed `allocator`, the copy loop as `_Construct` placement-new. reserve moves by copy-and-clear.
- **_Rb_tree<CCharString, pair<const CCharString,long>>** (0x4B9C10 `erase(first,last)`, 0x4BAF10 `equal_range`):
  erase(range) keeps the `[begin,end) -> clear()` early path with the node walk's `_M_increment` inlined.
- **GetRenderPositionForThing** (0x4BBF00, file-static __fastcall): C3DVector operators must be value-constructing
  (`return C3DVector(a.X-b.X, ...)`) for the x87 schedule. A local CTCPhysicsBase model with virtuals is used, because the
  engine header has none.
- **CTCGraphicAppearance** (0x4BE4E0 `Draw(info)`, 0x4C0050 `AddPersistentPrimitive`): Draw needs the parameter types
  declared `class` so the leaf mangles with V. AddPersistentPrimitive matched only with a declared `_List_iterator` copy ctor.
- **CMeshInfo sort** (0x4C1EC0 / 0x4C1F10 `__unguarded_linear_insert` First/Last, 0x4C2B90 `__unguarded_insertion_sort_aux`):
  the comparators read only the left operand, so the loop has no exit test (`while (comp(value, *next))`).
  The aux function is __fastcall with the 4-byte comparator by value.
- **Map erase(iterator)** (0x4C43B0 `<long, CIVCountedPointer>`, 0x4C4C70 MeshEffects): `_Rebalance_for_erase`, then the
  counted-pointer release inline (dec, virtual slot 1 at zero, null the slot), then free.
- **vector<CMeshInfo>::reserve** (0x4C46C0): /3 by the 0xAAAAAAAB multiply, `__uninitialized_copy` out of line (0x4C2AD0).
- **_Rb_tree::_M_insert** (0x4C4890 TintColours, 0x4C4F70 MeshEffects, 0x4C6960 BulletTimeInfo) and `_M_create_node`
  (0x4C4A20 MeshEffects): in 0x4C4890 and 0x4C4F70, the left site (`_S_left(y) = z`) inlines `_M_create_node` and the
  right site calls it out of line (a `__forceinline` copy for the left site plus the declared member for the right).
  0x4C6960 inlines both.
  CRGBColour copies through `Set(R,G,B,A)` (byte shuffle through a frame byte).
- **_List_base<CPrimitive>::clear** (0x4C5690): the CEnginePrimitiveHandle dtor modelled as the landed 0x4C4160.
- **CBulletTimeManager** (0x4C5F10 `RecalculateSoundPitch`, 0x4C5FA0 `GetBulletTimeSlowdownMultiplier`, 0x4C60F0 ctor):
  - The iterator ++ must be STLport's static by-value `_Rb_global<bool>::_M_increment(_M_node)`, assigned back.
  - The ctor needs the `_Rb_tree_base` / `_Rb_tree` split: the header is malloc'ed in the base ctor, before `_M_node_count(0)`.
  - Local models are used because the engine header's map is 16 bytes and retail's STLport map is 12.
- **CThing** (0x4C7380 `operator new(size,bool)`, 0x4C81F0 static `SerialiseThingInfo`, 0x4CA1D0 `Create`): straight
  C++; SerialiseThingInfo's three name/default transfers keep their CCharString temporaries.
- **Thing-component pairs** (0x4CB9F0 / 0x4CBA30 `for_each<.., CCallOnKill / CCallOnDie>`, 0x4CBB80
  `pair<CIntelligentPointer<CThing>,int>` ctor, 0x4CBE80 `__unguarded_insertion_sort_aux`): STLport's user-declared
  `pair(const pair&)` is required. Without it, for_each keeps the wrong half and the sort shrink-wraps its pushes.
- **CPersistContext** (0x4CC520 `TransferBinary<unsigned __int64>`, 0x4CCA80 `Transfer<unsigned __int64>(name, v)`):
  the empty-key CRC check, then a two-case mode switch (save via WriteUInt64, load via ReadUInt64).
- **list<CTCBase*>::push_back** (0x4CC820): plain STLport insert(end(), x).
- **GFRandom** (0x4CDAE0): `if (range != 0) return *seed % range; return 0;` puts the division on the fall-through path.
- **CSubtitledMessage::Initialise** (0x4CDB10): the `L""` prefix parameter is `const unsigned short*` (no /Zc:wchar_t).
- **pair<float, CCameraTag> swap / iter_swap** (0x4CF450, 0x4CFC50; identical bytes) and
  **__uninitialized_fill_n<pair<float,CCharString>>** (0x4CF990): 28-byte struct copies (`rep movsd` x7); the fill_n
  is __fastcall with the `__false_type` tag.

## Parked for Fable 5.1 (3)
- 004ba420 std::list<pair<CCharString,long>>::push_back DIFFER(63v63): `push ebp` is sunk into the placement-new block (shrink-wrapped) where retail pushes ebp in the prologue (push ebx/ebp/esi/edi) | tried: plain STLport insert(end(),x), memberwise pair copy ctor, no __forceinline, try/catch wrapper, /EHsc, QFE-4035 toolset | next: declared _List_iterator copy ctor (the fix that matched 0x004C0050)
- 004bcf50 NGraphicAppearance::CTCGraphicAppearance::GetPMeshEffect DIFFER(141v141): register swap only -- retail keeps the header (end) in esi and the best entry in edi, ours the reverse | tried: four declaration orders of best / last / it (incl. probe/004bcf50_v3), out-of-line prefix ++ with inline postfix | next: a named `end()` iterator local copied by a declared iterator copy ctor, or `best` as an iterator rather than an entry pointer
- 004c10e0 NGraphicAppearance::CTCGraphicAppearance::GetStaticFocalPos DIFFER(227v227): zero-offset path only -- retail forms &CachedStaticFocalPosOffset in eax (values in ecx/edx) and loads ParentThing before the Z store; ours forms the address in ecx and loads ParentThing after | tried: the flag set once before the branch vs in both branches (hoisted), a C3DVector(0,0,0) value temporary assigned through operator=, named-result operator- | next: three member stores (`Offset.X = 0; ...`) or a `C3DVector zero(0,0,0)` named local before the assignment

## Skipped
None. No row holds two functions.

## Label corrections
Catalog names (propagated) and the real identity:
- 0x4B83E0 Vector_Allocate_And_CopyConstruct -> vector<CCharString>::_M_allocate_and_copy
- 0x4B8770 vector_pop_back_shift -> vector<CCharString>::erase(iterator)
- 0x4B9C10 LTextTreeWalkThrough_Process -> _Rb_tree<CCharString,pair<const CCharString,long>>::erase(first, last)
- 0x4BA420 StdList_PushBack -> list<pair<CCharString,long>>::push_back
- 0x4BA7A0 CCombatWheel_Resize -> vector<CCharString>::reserve
- 0x4BAF10 StdMap_EqualRange -> _Rb_tree<CCharString,...>::equal_range
- 0x4BBF00 Mesh_InterpolatePosition -> GetRenderPositionForThing (file-static, anonymous namespace)
- 0x4BCF50 Appearance_OnAppearanceDrawHelper -> CTCGraphicAppearance::GetPMeshEffect
- 0x4BE4E0 CTCGraphicAppearance_PrepareForDraw -> CTCGraphicAppearance::Draw(const CEnginePrimitivePrepareInfo&)
- 0x4C0050 CArray_InsertAppearanceNode -> CTCGraphicAppearance::AddPersistentPrimitive
- 0x4C10E0 CTCGraphicAppearance_UpdateHelperPointOffset -> CTCGraphicAppearance::GetStaticFocalPos() const
- 0x4C1EC0 / 0x4C1F10 Insertion_MoveBackward_AndInsert(_Inverted) -> __unguarded_linear_insert<CMeshInfo*, CMeshInfo, CSortPrimaryMeshesFirst / Last>
- 0x4C2B90 _Copy_impl<CNavigationPosition*> -> __unguarded_insertion_sort_aux<CMeshInfo*, CSortPrimaryMeshesFirst>
- 0x4C43B0 StdList_Erase / 0x4C4C70 MeshEffect_EraseNode -> _Rb_tree::erase(iterator) (long->CIVCountedPointer map; MeshEffects map)
- 0x4C46C0 VectorReserve -> vector<CMeshInfo>::reserve
- 0x4C4890 ListNode_InsertCombatAnimation -> TintColours _Rb_tree::_M_insert. Neighbours: 0x4C41D0 is its `_M_create_node`;
  0x4C5890 (catalog CombatAnimationList::FindOrInsert) is the TintColours operator[]
- 0x4C4A20 ListNode_AllocAbilityData -> MeshEffects _Rb_tree::_M_create_node
- 0x4C4F70 ListNode_InsertAbilityData -> MeshEffects _Rb_tree::_M_insert. Neighbours: 0x4C5040 / 0x4C5480 (catalog
  AbilityDataList) are its insert_unique overloads
- 0x4C5690 LinkedList_ClearAndDestroy -> _List_base<CPrimitive>::clear
- 0x4C5F10 BulletTimeController_UpdateMinMultiplier -> CBulletTimeManager::RecalculateSoundPitch
- 0x4C5FA0 CAnimationList_GetTotalTime -> CBulletTimeManager::GetBulletTimeSlowdownMultiplier. The landed 0x4C6010
  calls it "GetTimeScale"
- 0x4C60F0 LTextBinTree_ctor -> CBulletTimeManager::CBulletTimeManager(CWorld&, CPlayerManager&)
- 0x4C6960 ListNode_InsertSplineData -> BulletTimeInfo _Rb_tree::_M_insert. Neighbours: 0x4C6A30 / 0x4C6BC0 (catalog
  SplineDataList) are its insert_unique overloads
- 0x4C7380 operator_new -> CThing::operator new(unsigned int, bool)
- 0x4C81F0 SerialiseThingInfo -> CThing::SerialiseThingInfo (static)
- 0x4CA1D0 Create -> CThing::Create
- 0x4CB9F0 / 0x4CBA30 _Copy_backward<CPixelFormat*> -> for_each<pair<ETCInterfaceType,CTCBase*>*, CCallOnKill / CCallOnDie>
- 0x4CBB80 make_pair<CIntelligentPointer<CThing>,int> -> the pair constructor (not make_pair)
- 0x4CBE80 _Copy_impl<CNavigationPosition*> -> __unguarded_insertion_sort_aux<pair<ETCInterfaceType,CTCBase*>*>
- 0x4CC520 CPersistTraits_uint64_TransferBinary -> CPersistContext::TransferBinary<unsigned __int64>
- 0x4CC820 ListNode_InsertTCNode -> list<CTCBase*>::push_back
- 0x4CCA80 CPersistTraits_uint64_TransferBinary_WithArg -> probably CPersistContext::Transfer<unsigned __int64>(const char*, v)
  (inferred)
- 0x4CDB10 Initialise -> CSubtitledMessage::Initialise
- 0x4CF450 / 0x4CFC50 swap<CDebugSphere> -> swap / iter_swap of pair<float, CSubtitledMessage::CCameraTag>. CDebugSphere is
  76 bytes and these move 28. Which address is swap and which is iter_swap is inferred: the bytes are identical.
- 0x4CF990 _Fill_n<unsigned long*,...> -> __uninitialized_fill_n<pair<float,CCharString>*>
- Data: 0xA36F70 is a folded `mov al,1; ret 4` thing-manager query; 0x13B8A54 is a CCRCSymbolMap; 0x1378D00 is a long
  holding 4 (the default bullet-time multiplier).
