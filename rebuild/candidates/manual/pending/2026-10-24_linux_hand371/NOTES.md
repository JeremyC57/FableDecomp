# Hand chunk 371 (_global .. _global): 38 of 40 byte-exact

All 40 rows are catalogued `_global`. Through PDB name sets, RTTI and callers they resolve to STLport instantiations
(the CWideString sort family, deques, vectors, lists, trees), CWorld and CQuestManager members, two boost
`checked_delete`s, a counted-pointer deleter and one singleton constructor. All 38 matches pass their behaviour test
under g++ (tall.sh) and VC7.1 (tall.py).

## What matched (38)

### STLport containers and algorithms
- **deque<CSetTextInfo>**
  - **0x494106 `_M_push_back_aux_v` / 0x4941C0 `push_back`.** Size-optimised TU (`#pragma optimize("s", on)`): ebp
    frame, `leave`, `and [m],0`.
  - **Shape:** the allocator needs `__forceinline` malloc. `_M_set_node` is chained, and the slow path takes a by-value
    copy of `__t` first.
- **sort<CWideString*, NGameComponentHelpers::CCaseInsensitiveStringCompare> (/Gr, by-value 4-byte strings and
  comparator)**
  - **Rows:** 0x496EE0 `__median`, 0x497200 `__unguarded_partition`, 0x497350 `__make_heap`, 0x497500
    `__linear_insert`, 0x4975E0 `__insertion_sort`, 0x497620 `__partial_sort`, 0x497740 `__introsort_loop`, 0x4977D0
    `sort`.
  - **Shape:** all are STLport 4.x verbatim.
    - `__median` uses SGI order.
    - `__pop_heap` is inline.
    - `sort` has `__lg` and `__final_insertion_sort` inlined.
    - The empty tag temporaries need a user constructor.
- **random_shuffle<long* / CCharString*, RandFunc> (0x49B1E0 / 0x49B240)**
  - `RandFunc { unsigned long* Seed; long operator()(long n) { return (long)GFFloatRandom((float)n, Seed); } }`, the
    inline `__iter_swap_aux`.
- **vector<CTexture>::erase(first, last) (0x49B3B0)**
  - Needed `__forceinline __copy(first, last, result, const random_access_iterator_tag&, _Distance*)` (76 v 104
    without it) plus the inline `_Destroy`.
- **vector<CCharString>::resize(n) (0x49B760)**
  - `resize(n) { resize(n, CCharString()); }`, with the inner `resize(n, const T&)` taking a const reference (plain
    inline).
    - A by-value inner resize is not inlined (27 v 101).
    - With `__forceinline` it re-forms the address with `lea` (102 v 101).
- **__uninitialized_fill_n<CTestQuest*> (0x4AA0D0, /Gr)**
  - STLport loop with an unsigned `n`.
  - The compiler-generated CTestQuest copy constructor is inlined, giving six CCharString copies.
- **__copy<CIntelligentPointer<CThing>*> (0x4AA610, /Gr)**
  - `for (n = last - first; n > 0; --n)` with an inline `operator=` that calls `SetPItem(other.GetPItem())`.
- **set<CCharString> `_Rb_tree::_M_insert` (0x4ABC70)**
  - STLport verbatim with a `noinline` `CBasicString<char>::operator<` body (0x429950) and a static cdecl
    `_Rb_global<bool>::_Rebalance`.
- **vector<CTestQuest>::_M_insert_overflow (0x4ADB50)**
  - Every `__false_type()` temporary is homed on the live `__atend` argument slot.
  - It matched first try once `__uninitialized_copy` (0x4AA060), `__uninitialized_fill_n` (0x4AA0D0) and
    `__destroy_aux` (0x4ABD90) had same-TU `auto_inline(off)` bodies that never touch the tag.
- **list<CThing*> -> back_inserter<vector<CThing*> > flag filter (0x4ADCB0, /Gr)**
  - A three-argument input-iterator loop, with the element in a named local.
  - The `__true_type()` tag shares the value's live slot because 0x48D0D0 `_M_insert_overflow` has a tag-free same-TU
    stand-in body.
  - The name is descriptive: no PDB name set has this instantiation.
- **list<CIntelligentPointer<CThing> >::erase(iterator) (0x4B7CA0)**
  - STLport verbatim. The polymorphic element's `~T()` goes through the vtable (`push 0 ; call [eax]`), and VC drops
    deallocate's null test on the already-dereferenced node.
- **__uninitialized_fill_n of pair<CCharString, pair<CCharString, CCharString> > (0x4B8260, /Gr)**
  - The record must be the NESTED pair, with a declared copy constructor.
  - A flat three-string struct keeps the count in a register (69 v 81). The nested inner copy re-bases this/other
    (`lea ebx,[esi+4]` / `lea edi,[ebp+4]`), and the count then spills to the `push ecx` word.

### CWorld
- **0x49D610 ConvertWorldToLevelWADPath / 0x49D770 ConvertWorldToQuestPath**
  - `SplitPath` into a named local.
  - `GetLevelsDirectory() + name + L".wad"/".qst"`.
- **0x49DDD0 ConvertWorldToStaticMapPath / 0x49DEC0 ConvertWorldToISMThingyPath (the 1-argument overloads)**
  - Each is `return F(world, false, g_bRuntimeLevelFiles);`, with the 3-argument overload's body (the landed 0x49D550 /
    0x49D6B0) compiled in the same TU and auto-inlined.
  - The flag is read before SplitPath.
- **0x49E740 DeleteCheckPointSave** (static __fastcall)
  - Profile path, `PathExists`, delete, then the `.hs` sidecar.
- **0x49ED30 ConsoleSetFreeCameraPos**
  - The main player's free camera `MoveTo(C3DVector(arg0, arg1, arg2))` from the CGenericVar floats.
- **0x4A08D0 ClearRegisteredQuests**
  - Three inline `clear()` calls.
  - The CTestQuest erase is inline with an out-of-line `__copy` / `__destroy_aux`.
  - A local retail layout is used: engine/CWorld.h carries the PDB debug offsets.

### Singletons, deleters, constructors
- **0x49BB90 CGameJoystickManager::Get**
  - `if (!PClass.Get()) PClass.Reset(new CGameJoystickManager); return *PClass;`
- **0x4ADD30 / 0x4AE080 boost::checked_delete<CGameTimeManager / CBulletTimeManager> (/Gr)**
  - `delete x;`, with the implicit destructor's inline STLport `~_Rb_tree` (`clear()`, then `_M_header.deallocate`).
  - The explicit instantiation of the inline template emits it.
- **0x4AE2A0 CSkeletalMorphResourceManager::CSkeletalMorphResourceManager()**
  - Identified through the RTTI of vtable 0x01238F84.
  - The inline protected CTBaseSingleton base's vtable store is dead. The STLport `_Rb_tree()` follows:
    `_Rb_tree_base(a) : _M_header(a, 0) { _M_header._M_data = allocate(1); }`, then `_M_empty_initialize()`.
- **0x4AE510 CCountedPointer<CMusicManager>::DeleteData**
  - `static void __fastcall DeleteData(void* p) { delete (T*)p; }`.
  - The implicit destructor runs the out-of-line vector-map destructor, then the const def pointer's
    `if (m_p) { m_p->Release(); m_p = 0; }`.

### CInsertNameInSet / CQuestManager
- **0x4AD3D0 CInsertNameInSet::operator()(const CThing*) const**
  - The physics pointer is a named local, and the name goes through an inline `CDefString::GetString() const` plus the
    inline `set::insert -> _M_t.insert_unique`.
  - That form re-forms the string temporary's address with `lea`:
    - A direct `insert(table.GetString(def))` pushes eax.
    - A named string local shrinks the frame to 0x48.
- **0x4AF610 IsQuestActive**
  - Matched once Compare 0x411570 got a same-TU stand-in body. VC then hoists the searched name's data pointer out of
    the loop, as retail does. With the callee declared only, VC reloads it every pass.
- **0x4B0A30 AddPendingQuestToDeactivate**
  - `push_back(pair<CCharString, unsigned long>(name, flags))`, using STLport insert/_M_create_node verbatim.
- **0x4B3760 LoadMasterScriptDataFromGameState**
  - The CScriptBase::DoScriptDataPersist loading arm over `GetQuestMasterDataPersistScript(name)`.
  - The copied text is homed in the dead name slot.
  - `&CGameDefinitionManager::Get()` is evaluated after the two outer pushes.
- **0x4B4A10 ActivateQuest**
  - `CArray<CCharString> quests; quests.push_back(name); return ActivateMultipleQuests(quests, ...);`
  - Retail keeps start/finish in edi/ebp across the element destructors. That needs:
    - a by-value STLport `_Destroy(first, last)` (which fixes finish);
    - a same-TU `ActivateMultipleQuests` stand-in that never keeps the array's address (which fixes start, so VC knows
      the local does not escape);
    - the tag-free `_M_insert_overflow` stand-in.

## Parked for Fable 5.1 (2)
- 0x00493ff8 vector<CSubtitleRenderer::CEmbeddedGraphic>::_M_insert_overflow DIFFER(181v180): the three `__false_type()` tag temporaries get their own frame word (`sub esp,8`, `lea [ebp-1]`) where retail homes them in the padding of the bool `__atend` slot (`push ecx` frame, `lea [ebp+1Bh]`) | tried: plain STLport form, `_STLP_alloc_proxy` end of storage, named tag local, tag with a member function (4 grades) | next: give `__uninitialized_copy` 0x493959 and `__uninitialized_fill_n` 0x493984 same-TU `auto_inline(off)` tag-free bodies (exactly what made the O2 twin 0x4ADB50 match first try)
- 0x004978a0 GFFloatRandom DIFFER(113v113): scheduling only. Retail issues `fmul [65536.0]` + `fstp [esp+4]` after the LCG `imul/add` and the first seed store, and `mov ecx,eax` after the double reload; the lane issues them straight after the `fld` | tried: `65536.0 * range` and `range * 65536.0` (2 grades) | next: a same-TU `auto_inline(off)` body of GFROR13 0x497890 (it directly precedes the row, so it is probably the same TU in retail), else lane gap: x87 scheduling

## Skipped
None. Every row is one function; the bounds match the bodies (no row runs into a second function).

## Label corrections
| Address | Catalog label | Actual function | Evidence / note |
|---|---|---|---|
| 0x493FF8 | Std_Vector_Insert_Range_CBodyPartMesh | vector<CSubtitleRenderer::CEmbeddedGraphic>::_M_insert_overflow | |
| 0x494106 | CTCUserActionPointBase_CreateTextInfo | deque<CSetTextInfo>::_M_push_back_aux_v | |
| 0x4941C0 | CSubtitleRenderer_ManageTextStorage | deque<CSetTextInfo>::push_back | |
| 0x496EE0 | CCharString_MedianOfThree | __median<CWideString> | |
| 0x497200 | CWideString_QuicksortPartition | __unguarded_partition | |
| 0x497350 | Heapify_CWideString | __make_heap | |
| 0x497500 | CWideString_PartitionWithMedian | __linear_insert | |
| 0x4975E0 | VectorInsert_ConstructWideStrings | __insertion_sort | |
| 0x497620 | Heapsort_Main_CWideString | __partial_sort | |
| 0x497740 | Heapsort_Recursive | __introsort_loop | |
| 0x4977D0 | StdSort_Introsort | sort | right idea |
| 0x49B1E0 | Shuffle_RandomIndices | random_shuffle<long*, RandFunc> | |
| 0x49B240 | VectorShuffle_Fisher | random_shuffle<CCharString*, RandFunc> | |
| 0x49B3B0 | Vector_EraseRange | vector<CTexture>::erase | |
| 0x49B760 | Vector_InsertElement | vector<CCharString>::resize | |
| 0x49BB90 | Get | CGameJoystickManager::Get | |
| 0x49D610 | GetDefaultVal<CFireEffectCreationSet> | CWorld::ConvertWorldToLevelWADPath | |
| 0x49D770 | GetDefaultVal<CFireEffectCreationSet> | CWorld::ConvertWorldToQuestPath | |
| 0x49DDD0 | GetSaveFileName | CWorld::ConvertWorldToStaticMapPath(const CWideString&) | |
| 0x49DEC0 | Save_ConstructAutoSaveFilename | CWorld::ConvertWorldToISMThingyPath(const CWideString&) | |
| 0x49E740 | DeleteCheckPointSave | CWorld::DeleteCheckPointSave | adds the class |
| 0x49ED30 | ConsoleSetFreeCameraPos | CWorld::ConsoleSetFreeCameraPos | adds the class |
| 0x4A08D0 | NQuestManager_ClearRuntimeState | CWorld::ClearRegisteredQuests | |
| 0x4AA0D0 | Vector_Uninitialized_Fill | __uninitialized_fill_n<CTestQuest*> | |
| 0x4AA610 | CBaseIntelligentPointer_CopyRange | __copy<CIntelligentPointer<CThing>*> | |
| 0x4ABC70 | StdSet_InsertNode | set<CCharString> _Rb_tree::_M_insert | |
| 0x4AD3D0 | Physics_CheckNavigability | CInsertNameInSet::operator()(const CThing*) const | |
| 0x4ADB50 | Vector_Reserve_And_Assign | vector<CTestQuest>::_M_insert_overflow | |
| 0x4ADCB0 | Vector_InsertFiltered | list<CThing*> -> back_inserter<vector<CThing*> > flag filter | real name unknown; caller CWorld::LoadHSP 0x4A0940 |
| 0x4ADD30 | GFCheckTreeNodes | boost::checked_delete<CGameTimeManager> | caller sites at CWorld +0x1C next to `new CGameTimeManager` 0x6BBC30 |
| 0x4AE080 | LTextTreeWalkThrough_DestroyPtr | boost::checked_delete<CBulletTimeManager> | CWorld +0x68, ctor 0x4C60F0 |
| 0x4AE2A0 | LTextTree_InitSentinel | CSkeletalMorphResourceManager::CSkeletalMorphResourceManager() | |
| 0x4AE510 | DeleteData | CCountedPointer<CMusicManager>::DeleteData | stored by Reset 0x4AE650 on CWorld +0x6C |
| 0x4AF610 | CQuest_Array_FindByName | CQuestManager::IsQuestActive | as labels chunk 322 |
| 0x4B0A30 | ListNode_InsertEntry | CQuestManager::AddPendingQuestToDeactivate | |
| 0x4B3760 | CQuestManager_LoadActiveQuest | CQuestManager::LoadMasterScriptDataFromGameState | |
| 0x4B7CA0 | ListNode_RemoveWithCallback | list<CIntelligentPointer<CThing> >::erase(iterator) | |
| 0x4B8260 | _Fill_n<unsigned_long*,unsigned_int,unsigned_long> | __uninitialized_fill_n<pair<CCharString, pair<CCharString, CCharString> >*> | propagated label |

Callee and neighbour labels found along the way:

| Address | Catalog label | Actual function | Note |
|---|---|---|---|
| 0x4AA580 | pair<CCharString,CPersistInfo> (propagated) | __copy<CTestQuest*> | |
| 0x4AA060 | — | __uninitialized_copy<CTestQuest*> | |
| 0x4ABD90 | — | __destroy_aux<CTestQuest*> | |
| 0x4A9CB0 | — | _Rb_tree<long>::_M_erase | |
| 0x4AA650 | — | map<void*, CBulletTimeInfo> _M_erase | |
| 0x4AE040 | — | ~vector<pair<long, CMusicManager::CRegisteredMusicEntry> > | |
| 0x4ADEA0 / 0x4ADEB0 | — | scoped_ptr<CGameTimeManager> dtor / reset | |
| 0x4AE160 / 0x4AE170 | — | scoped_ptr<CBulletTimeManager> dtor / reset | |
| 0x4AE650 | — | CCountedPointer<CMusicManager>::Reset | |
| 0x4B27F0 | LookupPersistentQuestInfo | NQuestManager::CRuntimeQuestPersistence::GetQuestMasterDataPersistScript | its twin GetQuestScriptPersistScript has the same shape; confirm which is which at landing |
| 0x4BB400 | Vector_Reserve_And_Assign | _M_insert_overflow of the nested-pair record vector | |
| 0x4B4260 | — | CQuestManager::ActivateMultipleQuests | |
