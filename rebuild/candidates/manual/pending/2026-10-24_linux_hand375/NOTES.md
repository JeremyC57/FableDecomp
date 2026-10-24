# Hand chunk 375 (_global .. _global): 39 of 40 byte-exact

All 40 rows are catalog `_global`; most turned out to be STLport 4.x instances and class members compiled in the
0x00516xxx-0x00529xxx thing-manager / map-header TUs. 39 grade MATCH/RELOCATION_MATCH, 1 parked, 0 skipped.
Every matched row has a behaviour test that passes under g++ (64-bit) and VC7.1 (`tall.sh`, `tall.py`).

## What matched (39)

**STLport tree instances**
- 0x00528610 / 0x005289C0 CThingManager::NPCNameMap (map<u64, pair<CCharString, CWideString> >) `_M_lower_bound` /
  `_M_create_node`: the plain STLport bodies; no /EHsc, so `_M_create_node` has no `_STLP_UNWIND`; the placement new keeps its null test.
- 0x00519970 / 0x0051AA00 map<CThingBuilding*, list<CThingAICreature*> > `_M_erase` / `_M_insert`: `_M_erase` inlines
  the list destructor (with `allocator::deallocate`'s null test). `_M_insert` declares the pair copy constructor and
  `_Rb_global<bool>::_Rebalance` (`__cdecl`).
- 0x00516F80 map<CCharString, C2DVector> (CRegion::MiniMapRegionExitTextOffsets) `_M_copy`: STLport verbatim, with
  `_M_clone_node`/`_M_create_node` inline. It matched first try.
- 0x00527FC0 / 0x00528140 / 0x00528090: `__uninitialized_copy`, `__copy(input_iterator_tag)` and `advance` over
  set<CCharString> iterators, all for vector<CCharString>::_M_range_insert (0x0052B6A0).
  - The iterator step is STLport 4.5's static `_Rb_global<bool>::_M_increment(node)`, inlined through
    `_M_node = _M_increment(_M_node)`. That copy-in/assign-back gives the eax temporary. An in-class 4.0
    `_M_increment()` edits `_M_node` in place (99 v 103).
  - `advance` needs `__forceinline` on `__advance`; plain inline gives a 25-byte forwarder. Its unsigned distance
    leaves the dead decrement arm behind the second `je`.

**STLport list / pair / vector / algorithm instances**
- 0x00517590 / 0x005199E0 pair copy constructors (map<CThingBuilding*, list<...>> and the CNavigationLayer
  u64 -> list<C2DLineF> vector map): STLport list copy = `insert(begin(), x.begin(), x.end())`.
  - list, `_List_base` and allocator must be `class` (mangled `V`), or the leaf silently grades the largest function.
  - C2DVector needs its user copy constructor.
- 0x00517130 list<CIntelligentPointer<CThingAICreature> >::_M_insert_dispatch: the insert must cache the neighbours
  (`n = pos._M_node; prev = n->_M_prev; ...`); the chained form gives 101 v 98.
- 0x00517480 `__copy` into back_insert_iterator<list<C2DLineF> > and 0x00519FF0 `__copy` over pair<u64, list<C2DLineF> >
  (list::operator= declared): the C2DVector user copy constructor is needed (138 v 122 without it).
- 0x005176D0 `__make_heap`, 0x0051ACF0 `__partial_sort` and 0x00517500 `__insertion_sort` over CThingBuilding** with
  CIsHouseLargerThan, all explicit `__fastcall` (/Gr TU).
  - `__partial_sort` and `__insertion_sort` need a same-TU `__declspec(noinline)` comparator body (the landed
    0x00512410 model). With it the comparator object stays enregistered (108 v 104 and 125 v 113 without it).
- CSeaBodyDesc rows (CStaticMapGenerationVisibiilityInfo, {CCharString Identifier; CArray<long> Maps}):
  - 0x00519EB0 `__destroy_aux`, 0x00518C00 `__uninitialized_copy` and 0x00518C90 `__uninitialized_fill_n`.
  - 0x0051C110 vector<CSeaBodyDesc>::_M_insert_overflow: same-TU noinline bodies for its three callees, with unnamed tags.
- 0x0051C1F0 vector<pair<u64, list<C2DLineF> > >::_M_insert_overflow (pair copy constructor declared, 0x005199E0).
- vector constructors and members:
  - 0x005179C0 vector<CRGBColour>(n): same-TU noinline `__uninitialized_fill_n` (80 v 78 without it).
  - 0x0051BEC0 vector<CRegion>(n): the fill is inline here.
  - 0x005290E0 vector<unsigned char> copy constructor.
  - 0x00519800 vector<CMapInfo>::reserve: needs the descriptive 72-byte Ego_r CMapInfo, not a byte body.

**CReplaceSerialise (map/level file headers)**
- 0x00519510 / 0x0051C890 ConvertToThisVersion: `*(T*)data = *ConvertPreviousUnknownVersionUp(data);` on the
  temporary (a named local reads `mov esi,[esp+8]`).
- 0x00519630 / 0x0051ADF0 GetMaxSize: the early-return `if (version == GetClassVersionNumber()) return MAX(size, sizeof(T));`
  form, with the whole chain Header1..MapHeader defined in the TU. VC then reuses the dominated duplicate call;
  a named-local form or a declared-only chain gives 43 or 131 v 110.
- 0x00518040 <COldFileFormatMapHeader4, COldFileFormatMapHeader3>::ConvertPreviousUnknownVersionUp: the landed 0x0051C2F0
  template unchanged. Version 3 has no fields, so ConvertFromOld is empty and inlined away: no call, and only held's
  count block survives the inline operator=.

**CThingManager / CEntityRuntimePersistence / misc**
- 0x0051F980 SetSuspendAllAI and 0x0051FA00 SetAllBossesToBossPhase (private static __fastcall script commands):
  - GetAllThings, push_back and the real STLport `_M_insert_overflow` are written out in the TU (their instances
    sit at 0x0052B270/0x0052B300). VC then sees the found vector never escapes, so begin/end stay in registers
    (124 v 120 with GetAllThings declared).
  - The boss component uses the landed `T* tc; if (HasTC) tc = GetTC; return tc;` idiom.
- 0x00523790 / 0x005237F0 / 0x00523910 NThingManager::CEntityRuntimePersistence::SetLevelPersistentScript /
  SetGamePersistentScript / SetGamePersistentSaveDataCompressed (24-byte CEntityScriptInfo records):
  - The Level variant reads size() once into a local, compared as `(unsigned)(index + 1) > count`.
  - The two Game variants use `MAX(index + 1, size())` (two divides), as the landed 0x00523860 does.
  - SaveDataCompressed: SaveData.resize(n) passes a zero temporary homed in the dead index slot, then memcpy
    into `data()`.
- 0x00527200 CCharString::ReplaceAll (name unproven): `GetString()` is spelled `PStringData == 0 ? "" : ...`. The first
  strstr comes before the loop and the next at its bottom, on `hit += strlen(key)` (the `while ((hit = strstr(...)))`
  form gives 221 v 224).
- 0x0051D990 GFCalcThingUID (`mapUID * 0x10000000000 + thingID`, `__fastcall` u64), 0x00527320 CThingObject::Alloc,
  0x0051A040 the set+array default constructor (unidentified, no xrefs; descriptive local class).

## Parked for Fable 5.1 (1)
- 0x00518dc0 CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>::operator() DIFFER(189v181): `mov al,[eax+38h] ; sub esp,8 ; test cl,4` and the 0xC2 lookup key in its own frame word, where retail does `push ecx ; test byte [eax+38h],4` with the key and the "CTCActionUseScriptedHook" literal both homed in the dead `thing` argument slot | tried: GetTCOfLeafType plain inline (stays out of line, 17v181); __forceinline + landed GetTC named key copy (189); key as a temporary bound to `const ETCInterfaceType&` (arg slot, but the compare reloads it, 189); by-value Find converting to `const long&` (constant compare, key still own slot, 189) | next: the Ego_r helper chain `GetTC<T>(ETCInterfaceType, T**)` called from GetTCOfLeafType, so the key temporary dies before the literal is built

## Skipped
None. All row extents check out: each matched body ends exactly at the trimmed oracle length.

## Label corrections
- 0x0051D990 "Multiply64Bit" -> GFCalcThingUID (FableWin `?GFCalcThingUID@@YI_K_K0@Z`; the landed caller calls it MakeUID_0051d990).
- 0x00519EB0 "_Copy_backward<CPixelFormat*>" -> `__destroy_aux<CSeaBodyDesc*>`.
- 0x00519FF0 "_Move<CEngineTextureMorph*>" -> `__copy` over pair<u64, list<C2DLineF> >.
- 0x005290E0 "Vector_CopyAssignFrom" -> vector<unsigned char> copy constructor.
- 0x005179C0 "std_vector_ulong_Initialize" -> vector<CRGBColour>(n).
- 0x005289C0 "CSplineEntry_Create" -> NPCNameMap `_M_create_node`.
- 0x00528610 "BinaryTree_LowerBound_Uint64" -> NPCNameMap `_M_lower_bound`.
- 0x005199E0 "std_vector_from_vector" -> pair<u64, list<C2DLineF> > copy constructor.
- 0x00517590 "DoublyLinkedListNode_CreateAndCopy" -> pair<CThingBuilding* const, list<CThingAICreature*> > copy constructor.
- 0x00517130 "DoublyLinkedList_InitializeFromRange" -> list<CIntelligentPointer<CThingAICreature> >::_M_insert_dispatch.
- 0x00517480 "LinkedList_InsertRangeFromArray" -> `__copy`(C2DLineF*, back_insert_iterator).
- 0x0051AA00 "ListNode_InsertWithClone" -> `_Rb_tree::_M_insert`; 0x00519970 "TreeNode_DestroyRecursive" -> `_Rb_tree::_M_erase`.
- 0x00518C90 "CCharString_InitializeArrayRange" -> `__uninitialized_fill_n<CSeaBodyDesc*>`.
- 0x00519510 "CEngineWorldMap_CopyFields_Type3" -> CReplaceSerialise<CFileFormatLevel,COldFileFormatLevel3>::ConvertToThisVersion.
- 0x0051C890 "ResourceData_CopyIfDifferentBuffer" -> CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertToThisVersion.
- 0x00519630 / 0x0051ADF0 "GetMaxSize" -> CReplaceSerialise<Header7,Header6> / <CFileFormatMapHeader,Header8>::GetMaxSize.
- 0x00518040 "CEngineLandscapeMeshBuilder_CLayer_CopyConstructor_Type1" -> CReplaceSerialise<COldFileFormatMapHeader4,COldFileFormatMapHeader3>::ConvertPreviousUnknownVersionUp.
- By the call chain (bodies not graded):
  - 0x00516010 "CEngineWorldMap_MoveConstructor_Type2" -> the <Header3,Header2> link.
  - 0x005196E0 "..._CopyConstructor_Type2" -> the <Header5,Header4> link.
- 0x0051C1F0 "Vector_InsertRangeGeneric_CMusicSetEntry" -> vector<pair<u64, list<C2DLineF> > >::_M_insert_overflow;
  0x0051C110 "Vector_InsertRangeGeneric_CCharString" -> vector<CSeaBodyDesc>::_M_insert_overflow.
- 0x0051F980 / 0x0051FA00 -> CThingManager:: (private static __fastcall, Ego_r/FableWin names).
- 0x00523790 "CThingDeadCreature_SetName" / 0x005237F0 "CThingDeadCreature_UpdateBulletTimeAndName" /
  0x00523910 "CThingDeadCreature_SetAnimationData" -> NThingManager::CEntityRuntimePersistence::SetLevelPersistentScript /
  SetGamePersistentScript / SetGamePersistentSaveDataCompressed.
- 0x0052C6A0 "CThingDeadCreature::WorldUpdateBulletTime" (landed as CMarkerRecordVector::resize) -> vector<CEntityScriptInfo>::resize(size_type).
- 0x00527FC0 "Tree_CopyElements_CCharString" / 0x00528140 "Tree_AssignElements_CCharString" / 0x00528090
  "BinaryTree_AdvanceIterator" -> STLport `__uninitialized_copy` / `__copy` / `advance` over set<CCharString> iterators.
- 0x0052B6A0 "CCombatWheel::Insert" -> vector<CCharString>::_M_range_insert(pos, set iterators, forward_iterator_tag).
- 0x00518DC0 "CTCActionUseScriptedHook_CheckType" -> CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>::operator() (Ego_r name set).
- 0x00516F80 "CCharStringTreeNode_DeepClone_Extended" -> `_Rb_tree::_M_copy` of map<CCharString, C2DVector>.
- 0x00833A10 "CThingAnimatable::SetSuspendAnimation" -> CThingAICreature::SuspendAI(bool, void*) (Ego_r `?SuspendAI@CThingAICreature@@QAEX_NPAX@Z`).
- 0x00527320 "Alloc" -> CThingObject::Alloc; 0x0051A040 stays unidentified (no xrefs).
