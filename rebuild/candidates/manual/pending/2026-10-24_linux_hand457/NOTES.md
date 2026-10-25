# Hand chunk 457 (pair<unsigned_long .. std): 40 of 40 byte-exact

All 40 rows grade MATCH (5) or RELOCATION_MATCH (35) through `gall.sh` (with the Leaf/Flags lines). Every leaf was
checked against `objdump -t`. All 40 behaviour tests print `<addr>_TEST PASS` under g++ (`tall.sh`) and VC7.1 (`tall.py`).
`calls.py` shows every call going to retail's target: malloc 0xBFEA0E, free 0xBFEA14, operator delete 0xBFE9BC, the
CCharString imports, and the out-of-line STLport helpers at their retail addresses. No row needed `/Gr` or `/GS` flags;
`__fastcall` is written on the template definitions.

## What matched (40)

**pair<unsigned_long (3 rows): constructors**
- 0x00A327E0 `_STL::pair<unsigned long,C3DMeshVolume>::pair()` default-constructs a C3DMeshStats volume entry.
  - Shape: STLport `pair() : first(_T1()), second(_T2())`; the key zero store comes first.
- 0x00C108A0 `SPKPlaybackRequest::SPKPlaybackRequest()` and 0x00C2D1A0 `SPKVoiceSettings::SPKVoiceSettings()` are libpk
  record constructors (descriptive names; libpk has no PDB or RTTI).
  - Shape: a member-init list in declaration order around the embedded member's out-of-line constructor.

**remove_if<CAnimSet (2 rows): list<CAnimSet>::remove_if**
- 0x00711800 `remove_if<CIsType>` (the Type byte) and 0x007118C0 `remove_if<CHasTypeFlags>` (TypeFlags & Flags).
  - Shape: STLport 4.6 `remove_if` + `erase`. The 0x54-byte retail CAnimSet carries two 12-byte STLport vectors whose
    `_Vector_base` destructors free their buffers.

**remove_if<CMiniMapDisplay (3 rows)**
- 0x0076D7B0 is really `list<CIntelligentPointer<CThing> >::remove(const&)`.
  - Shape: the `__fastcall` template `operator==` compares the two `GetPItem()` results.
- 0x0082C6E0 `list<CMarkerInfo>::remove_if<CIsMarkerAutoMarker>` and 0x0082C770 `remove_if<CIsMarkerBoundToThing>`.
  - Shape: the implicit ~CMarkerInfo calls the out-of-line ~CBaseIntelligentPointer 0xA01C10. CIntelligentPointer
    declares no destructor.

**remove_if<CTCCreatureModeManager::CModeBase (3 rows)**
- 0x0073E790 `list<CCountedPointer<CModeBase> >::remove_if<CIsType>` (the predicate makes the vcall to GetModeType).
- 0x008112D0 is really `CTCEntityEvents::RemoveEvent(CEventBase&)`. 0x00A77710 is really
  `CNavRegion::RemoveConnectionFromConnectionList(const CNavRegionConnection*)`.
  - Shape: a `for` loop with `erase(it); return;` inside.
  - CNavRegion's member had to be `private` to match FableWin's `AAE` mangling.

**remove_if<NEntityEvents (1 row)**
- 0x00812030 `list<CCountedPointer<CEventBase> >::remove_if<CPred_EventExpired>`.
  - Shape: `IsExpired` reads `WorldFrame` into a named local before the virtual `GetLifeTime()`. Without it, VC loads
    the frame after the call.

**remove_if<NEntityEvents::CConvFilterToCP<NEntityEvents (6 rows)**
- 0x008AC890 is the real `remove_if<CConvFilterToCP<CIsEventOfType> >`.
- 0x004BA390 `list<CCountedPointer<CQuestManager::CActiveQuest> >::remove(const&)` and 0x009C6E50
  `list<CCountedPointer<const CFileInstaller::CFileInfo> >::remove(const&)`.
- 0x009F89B0, 0x00A0A720 and 0x00A17390 are `_List_base<T>::clear()` for CGraphicDataBankInfo, CMeshDataBankInfo and
  CNavQuadTreeMap.
  - Shape: the STLport clear advances the cursor before destroying the node, then relinks the sentinel.

**remove_if<std::unary_negate<NCameraModes (1 row)**
- 0x008FBDD0 `list<const CThing*>::remove_if<unary_negate<CGetThingsWithCollisionMesh> >`.
  - Shape: going through the inline const accessor `GetPhysicsTC()` puts the vtable in edx; the bare member chain
    reuses eax.

**sort<... CThing* ... (1 row)**
- 0x007FA680 is really `_STL::__pop_heap<CThing**, CThing*, CThingCompare_BestForTargeting2, int>`.
  - Shape: `__fastcall`, `*result = *first; __adjust_heap(first, 0, last - first, value, comp)`.

**std (20 rows)**

*Def-TU vectors (size-optimised TU)*
- 0x00451C3E `vector<long>::_M_initialize_aux<int>`.
  - Under "s", `allocate`, `fill_n` and `uninitialized_fill_n` must be `__forceinline`; fill_n then becomes `rep stosd`.
- 0x0046EF33 `_Vector_base<pair<unsigned long,CSubDefInfo> >(n, a)` uses "s" alone (no frame).
- 0x0046EECF is that vector's copy constructor, with "y" off + "s" on.

*Sound map sorts (CSoundMap::SoundMap, pair<unsigned long, CSoundPair>, 12 bytes)*
- Rows: 0x00470CF1 `sort`, 0x00470E08 `__linear_insert`, 0x00470E6D `__copy_backward_ptrs`, 0x00470F32 `__median`,
  0x00470F5F `__unguarded_partition`, 0x00470FA5 `__partial_sort`, 0x004710F0 `__make_heap`.
- The CriteriaMap twins (pair<unsigned long, CDefString>, 8 bytes) are 0x004712B2, 0x004713C5, 0x004713F2 and 0x0047142B.
- Shape lessons:
  - Frame bodies use "y" off + "s" on; frameless ones use "s" alone. Adding "y" off to 0x4713F2 adds a frame.
  - The `__linear_insert` copy_backward tag is STLport's `_IsOKToMemCpy(..)._Answer()` returning a POD `__false_type`.
    `T()` zero-initialises it (`xor eax,eax ; stosb`) and the returned byte is copied back onto itself. Giving
    __false_type a user constructor drops 16 bytes.
  - partial_sort, make_heap, __pop_heap, sort_heap, pop_heap and `__lg` are `__forceinline`; the median, the partition
    and the heap helpers stay out of line.

*Quest-persistence sorts (NQuestManager::CRuntimeQuestPersistence's CVectorMap<CCharString, CPersistInfo>,
pair<CCharString, CPersistInfo{QuestName, PersistScript}>, 12 bytes, /Gr TU)*
- Rows: 0x004B9480 `__unguarded_linear_insert`, 0x004BA080 `__unguarded_partition`, 0x004BAB30 `__linear_insert`,
  0x004BAC90 `__make_heap`.
- Shape lessons:
  - The landed 0x429950 `CBasicString<char>::operator<` body sits above the callers under `auto_inline(off)`, and the
    inline CCharString::operator< null checks surround the call.
  - STLport's iterator tags are empty structs with bases (non-POD). `random_access_iterator_tag()` therefore stores
    nothing and is homed in the dead comparator slot, while `__false_type` stays POD.

*Other std rows*
- 0x0048B2F0 `_STL::const_mem_fun1_t<bool,CThing,CCharString>::operator()`, which binds CThing::IsSameScriptName.
  - It is an unreferenced COMDAT (no call or absolute reference in retail).
- 0x004B92D0 `_STL::list<CCountedPointer<CQuestManager::CActiveQuest> >::_M_insert_dispatch<const_iterator>`. It serves
  list::insert 0x4B9EB0, the list copy constructor 0x4BADC0 and CQuestManager 0x4B2AD0 (which copies ActiveQuests).

## Parked for Fable 5.1 (0)

None.

## Skipped

None.

## Label corrections

**Chunk rows whose catalog labels are wrong or propagated**
- 0x00C108A0 and 0x00C2D1A0 carry the propagated `pair<unsigned_long,C3DMeshVolume>`. They are libpk constructors,
  named descriptively: SPKPlaybackRequest and SPKVoiceSettings. Their callees 0xC20390 and 0xC167B0 are catalogued as
  CNavigationLayer and CBaseIntelligentPointer; both are libpk sub-object constructors (SPKHandle, SPKReference).
- 0x008112D0 and 0x00A77710 carry `remove_if<CModeBase::CIsType>`. They are
  `CTCEntityEvents::RemoveEvent(NEntityEvents::CEventBase&)` and
  `CNavRegion::RemoveConnectionFromConnectionList(const CNavRegionConnection*)` (FableWin names).
- 0x004BA390 and 0x009C6E50 carry `remove_if<CConvFilterToCP<..> >`. They are `list<CCountedPointer<CActiveQuest> >::remove`
  and `list<CCountedPointer<const CFileInfo> >::remove`.
- 0x009F89B0, 0x00A0A720 and 0x00A17390 carry `remove_if<CConvFilterToCP<..> >`. They are `_List_base<T>::clear()`, and
  their callers are ~CGraphicBankManager, ~CMeshBankManager and the navigator manager's QuadTrees ~list bodies.
- 0x0076D7B0 carries `remove_if<CIsMarkerBoundToThing>`. It is `list<CIntelligentPointer<CThing> >::remove`.
- 0x0082C6E0 carries `remove_if<CIsMarkerBoundToThing>`. It is `remove_if<CIsMarkerAutoMarker>`.
- 0x007FA680 carries `sort<..., CThingCompare_BestForTargeting2>`. It is `__pop_heap<CThing**, CThing*,
  CThingCompare_BestForTargeting2, int>`.
- The size-TU vector and sort rows carry descriptive catalog names (`std::vector_Fill`, `std::sort_insertion_12byte`, ...).
  Their real STLport names are `_M_initialize_aux`, `vector(const vector&)`, `_Vector_base(n, a)`, `sort`,
  `__linear_insert`, `__copy_backward_ptrs`, `__median`, `__unguarded_partition`, `__partial_sort` and `__make_heap`, over
  pair<unsigned long, CSoundPair> (SoundMap) and pair<unsigned long, CDefString> (CriteriaMap).
- 0x0048B2F0 `std::allocator_traits_Construct` (and the autoname's pair<EHeroMorphType, CParticleMorphs::CEntry>) is
  `const_mem_fun1_t<bool,CThing,CCharString>::operator()`.
- Quest-persistence sort rows:
  - 0x004B9480 `std::set_Insert_KeyValue` is `__unguarded_linear_insert`.
  - 0x004BAB30 `std::Set_Insert_OrReplace` is `__linear_insert`.
  - 0x004BA080 and 0x004BAC90 are `__unguarded_partition` and `__make_heap`.
  - All four are over pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>.

**Neighbours outside the chunk (catalog or landed names)**
- 0x00470D36 is `__final_insertion_sort` (catalogued as make_heap<..>).
- 0x00471150 is the CSoundPair `__pop_heap_aux` (catalogued `_Pop_heap_0<pair<float,CSubtitledMessage..> >`, landed with
  a CTextureMorph element).
- 0x00471576 is the CDefString `__pop_heap_aux` (catalogued `_Pop_heap_0<pair<long,EHeroAbility> >`, landed with that
  element).
- 0x0047130E is the CDefString `__copy_backward_ptrs` (catalogued `_Copy_backward<pair<long,float> >`, landed under the
  placeholder `CopyBackwardPtrs_PairULongWord_0047130e`).
- 0x00470CD8 is `CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<..> >::Sort`. It was landed as
  `global_TransferToGui_00470cd8`, with a CGuiVarTransferBool model.
- 0x0047119A is the CDefString CVectorMap::Sort (landed placeholder `CVectorMap_T_0047119a`).
- 0x00471239 is the CDefString `__unguarded_insertion_sort_aux` (landed placeholder).
- The CQuestManager sort chain over pair<CCharString, CPersistInfo>:
  - 0x004BBB40 `sort` (catalogued Heap_BuildFromRange / Sort_SElement12)
  - 0x004BBBC0 / 0x004BBBE0 CVectorMap::Sort / SortIfDirty ("T unresolved" in the 2026-10-24 names: T is CPersistInfo)
  - 0x004BBA40 `__introsort_loop`
  - 0x004BB520 `__final_insertion_sort`
  - 0x004BAFB0 `__insertion_sort`
  - 0x004BA100 `__unguarded_insertion_sort_aux`
  - 0x004BB810 `__partial_sort`
  - 0x004BB010 the unreferenced `make_heap`
  - 0x004BA180 `__adjust_heap`
  - 0x004B7800 `iter_swap` (catalogued swap<CPersistInfo>)
  - 0x004B78D0 SGI `__copy_backward` (catalogued _Move_backward)
- 0x0099EAE0 is `~CCharString` (the call-check catalogue names it `_Cons_val<allocator<pair<EHeroMorphType,..> > >`).
