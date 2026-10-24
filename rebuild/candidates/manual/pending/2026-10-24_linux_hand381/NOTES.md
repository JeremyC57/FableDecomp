# Hand chunk 381 (_global .. _global): 38 of 40 byte-exact

Every row was catalogued as `_global`. The bodies are STLport 4.x library code from two translation units:

- the CTCInventoryAbilities STL TU (0x005F7B76..0x005FA3A5), a size TU;
- the CTCInventoryMap / persist TUs (0x005FC510, 0x00612D30..0x006236C0), speed TUs.

The 38 matched sources (in `src/`) all grade MATCH or RELOCATION_MATCH in `grades.txt`. All 38 behaviour tests pass under g++ (`tests_gcc.txt`) and under the lane's VC7.1 in Wine (`tests_vc.txt`).

## What matched (38)

Sort chains. The lower-address twin of each pair sorts `pair<long, EHeroAbility>` (CTCInventoryAbilities AbilityButtons, CVectorMap at +0x16C). The higher-address twin sorts `pair<long, long>` (ExpressionButtons, +0x1C8).
- 005f7cd0 / 005f7d15 `std::__unguarded_linear_insert`
- 005f84c6 / 005f84f3 `std::__median`
- 005f8648 / 005f86aa `std::__adjust_heap` (calls the landed `__push_heap` 0x5F7D85 / 0x5F7DD4)
- 005f8c9b / 005f8ce2 `std::__unguarded_partition`
- 005f9a98 / 005f9adb `std::__linear_insert`
- 005f9d4e / 005f9da3 `std::__partial_sort`
- 005fa26b / 005fa2f6 `std::__introsort_loop`

Copy algorithms:
- 005f7b76 `std::__copy<EHeroAbility*>`
- 005f7c13 `std::__copy_backward<EHeroAbility*>`
- 005f847f `std::__copy<CDefPointer<const CHeroAbilityDef>*>`
- 00613dd0 `std::__uninitialized_copy<CTCInventoryMap::CUIMiniMapMarker>`
- 006236c0 `std::uninitialized_fill_n<CQuestCardItem*>`

Vector members:
- 005f8ec0 `vector<EHeroAbility>::vector(n)`
- 005f9495 `vector<EHeroAbility>::_M_insert_overflow`
- 005f9633 `vector<EHeroAbility>::_M_fill_insert`
- 005f9bfe `vector<pair<long, CDefPointer<const CHeroAbilityDef> > >::vector(n, value, a)`
- 005f9f5a `vector<CDefPointer<const CHeroAbilityDef> >::operator=`
- 005fa1ba `vector<pair<long, CDefPointer<...> > >::operator=`
- 005fa10a `vector<CDefPointer<...> >::_M_insert_overflow`
- 005fa3a5 `vector<CDefPointer<...> >::push_back`
- 00617370 `vector<unsigned __int64>::_M_insert_overflow` (`__true_type`)
- 006181a0 `vector<unsigned __int64>::push_back`

Trees and lists:
- 00614a10 `_Rb_tree<long, long, _Identity<long>, less<long> >::_M_find`: set<long>, CTCVillage::PlayersCrimes
- 00616f90 `_Rb_tree<long, pair<const long, pair<long,long> >, ...>::_M_insert`
- 00617230 `list<pair<const CRegion*, const CRegion*> >::push_back`: CTCInventoryMap::RegionsCorrespondances
- 00618100 `list<CIntelligentPointer<CThing> >::_M_insert_dispatch`

CTCInventoryMap:
- 005fc510 `CTCInventoryMap::ResetPrimitiveHandles()`

Persist:
- 00612d30 `GFSerialiseVectorBinaryOut<unsigned __int64>`
- 00614700 `GFSerialiseVectorBinaryOut<CTCInventoryMap::CUIRegionInfo>`
- 00619130 `GFSerialiseVectorBinaryIn<unsigned __int64>`
- 00619620 `GFSerialiseVectorBinaryIn<long>`

## Parked for Fable 5.1 (2)

- 00618de0 std::map<long, pair<long,long> >::operator[] DIFFER(123v123): the two `mov dword ptr [esp+18h],0 ; mov dword ptr [esp+1Ch],0` stores (the pair() mapped half) sit right after `push ecx ; mov eax,esp`; retail sinks them below `mov [eax],edx ; lea edx,[esp+24h] ; push edx`, just before the insert_unique call. Scheduling only. | tried: pair without a user copy ctor (119v123, zero register); STLport explicit pair copy ctor (best, 123v123); direct `_M_t.insert_unique` (121v123); body-assigning `_Rb_tree_base_iterator` ctors (123v123) | next: lane gap: store scheduling across argument pushes (maybe try `pair()` with body assignments, or a named `_Tp` local).
- 00619560 GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo> DIFFER(180v180): the traits object is addressed at `lea ecx,[esp+18h]` (the count word); retail uses `lea ecx,[esp+1Ch]` (over the T() resize temporary). | tried: CUIRegionInfo with a zeroing default ctor (best; a probe lever); POD T (zero fill through ecx, edi opened in the guard); a hoisted `long i = 0`; both together | next: give `CPersistTraits<T>` an uninitialised `T DefaultVal` member (the matched BinaryOut twin 0x00614700 proves the 40-byte traits) with a POD T, so the traits object overlays the temporary like retail.

## Skipped

None. No row spans two functions. Several rows' `length` includes trailing int3 padding: 00613dd0 +12, 00614700 +2, 00616f90 +5, 00617230 +11, 00618100 +14, 00618de0 +5, 00619130 +12, 00619560 +12, 006236c0 +14. These are not mis-cuts, and none is an int3 after a noreturn call.

## Label corrections

Catalog label -> what the body is (address: old -> new):
- 0x005F7B76 `_Copy_impl<CRGBColour*,CRGBColour*>` -> `std::__copy<EHeroAbility*, EHeroAbility*, int>`
- 0x005F7C13 `_Copy_backward<CDefIndex*,CDefIndex*>` -> `std::__copy_backward<EHeroAbility*, EHeroAbility*, int>`
- 0x005F7CD0 / 0x005F7D15 `std_HeapSiftDownPair` -> `std::__unguarded_linear_insert` (pair<long,EHeroAbility> / pair<long,long>)
- 0x005F847F `_Move<CDefPointer<...>*>` -> `std::__copy<CDefPointer<const CHeroAbilityDef>*, ..., int>`
- 0x005F84C6 `std_MinOfThree` / 0x005F84F3 `std_median_three` -> `std::__median`
- 0x005F8648 / 0x005F86AA `std_PopHeapRebalance` -> `std::__adjust_heap`
- 0x005F8C9B `std_Partition_8Byte` / 0x005F8CE2 `std_partition_Swap` -> `std::__unguarded_partition`
- 0x005F8EC0 `std_Vector_ConstructN_4Byte` -> `std::vector<EHeroAbility>::vector(size_type)`
- 0x005F9495 `Vector_CopyAndReallocate_UnsignedInt` -> `std::vector<EHeroAbility>::_M_insert_overflow`
- 0x005F9633 `std_Vector_InsertSingleRealloc` -> `std::vector<EHeroAbility>::_M_fill_insert`
- 0x005F9A98 / 0x005F9ADB `std_heap_adjust_pair_element` -> `std::__linear_insert`
- 0x005F9BFE `Vector_InitializeContent` -> `std::vector<pair<long, CDefPointer<const CHeroAbilityDef> > >::vector(n, value, a)`
- 0x005F9D4E / 0x005F9DA3 `Heap_ExtractAndSort` -> `std::__partial_sort`
- 0x005F9F5A / 0x005FA1BA `std_vector_resize_grow` -> `vector<CDefPointer<...> >::operator=` / `vector<pair<long, CDefPointer<...> > >::operator=`
- 0x005FA10A `std_vector_resize_grow` -> `std::vector<CDefPointer<const CHeroAbilityDef> >::_M_insert_overflow`
- 0x005FA26B / 0x005FA2F6 `Std_Introsort_8BytePartitionRecurse(_Alt)` -> `std::__introsort_loop`
- 0x005FA3A5 `Vector_PushBack_Refcounted` -> `std::vector<CDefPointer<const CHeroAbilityDef> >::push_back`
- 0x005FC510 `ComboBox_ClearAllLists` -> `CTCInventoryMap::ResetPrimitiveHandles()` (FableWin.pdb `?ResetPrimitiveHandles@CTCInventoryMap@@QAEXXZ`)
- 0x00613DD0 `_Copy_backward<CExplosionRing*,CExplosionRing*>` -> `std::__uninitialized_copy<const CTCInventoryMap::CUIMiniMapMarker*, CUIMiniMapMarker*>`
- 0x00614700 `GFSerialiseVectorBinaryOut<CHeroMarriageInfo>` -> `GFSerialiseVectorBinaryOut<CTCInventoryMap::CUIRegionInfo>`
- 0x00614A10 `RBTree_FindByKey` -> `std::_Rb_tree<long, long, _Identity<long>, less<long>, allocator<long> >::_M_find` (CTCVillage::PlayersCrimes)
- 0x00616F90 `RBTree_Insert` -> `std::_Rb_tree<long, pair<const long, pair<long,long> >, _Select1st, less<long> >::_M_insert`
- 0x00617230 `CList_PushBack` -> `std::list<pair<const CRegion*, const CRegion*> >::push_back`
- 0x00617370 `Vector_InsertPair` -> `std::vector<unsigned __int64>::_M_insert_overflow(..., const __true_type&, ...)`
- 0x00618100 `Campfire_InsertRegionNode` -> `std::list<CIntelligentPointer<CThing> >::_M_insert_dispatch<_List_const_iterator>`
- 0x006181A0 `Vector_PushBack_Pair` -> `std::vector<unsigned __int64>::push_back`
- 0x00618DE0 `RBTree_FindAndInsert` -> `std::map<long, pair<long,long> >::operator[]` (FableWin.pdb + Ego_r.pdb `??A?$map@JU?$pair@JJ@std@@...`)
- 0x00619130 `GFSerialiseVectorBinaryIn<CRGBColour>` -> `GFSerialiseVectorBinaryIn<unsigned __int64>`
- 0x00619560 `GFSerialiseVectorBinaryIn<CBrainUpdateZone>` -> `GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo>`
- 0x00619620 `GFSerialiseVectorBinaryIn<CRGBColour>` -> `GFSerialiseVectorBinaryIn<long>`
- 0x006236C0 `_Fill_n<unsigned long*,unsigned int,unsigned long>` -> `std::uninitialized_fill_n<CQuestCardItem*, unsigned int, CQuestCardItem>`
- Confirmed as catalogued: 0x00612D30 `GFSerialiseVectorBinaryOut<unsigned __int64>`.

Neighbours found on the way (not chunk rows; verify before applying):
- 0x005FA0C3 / 0x005FA0D5 "NGameText::ConstructString" are the `partial_sort` front ends of the two sort chains.
- 0x005F93C1 / 0x005F93E6 "CMemoryAllocator::operator_delete" are `make_heap` front ends.
- 0x005FA03B / 0x005FA07F (landed as "make_heap") are `__final_insertion_sort`.
- 0x005FA3DD / 0x005FA41F are `std::sort`.
- 0x005FA47D / 0x005FA496 are `CVectorMap::Sort`.
- 0x00617060 / 0x00617D50 are the map<long, pair<long,long> > tree's `insert_unique(v)` and `insert_unique(hint, v)`.
- 0x00624E40 is `vector<CQuestCardItem>::_M_fill_insert`, and 0x006252D0 is its `resize(n, x)`.
- The landed 0x00624B90, named `vector<pair<int,CWideString>>::_M_insert_overflow`, has the same 8-byte {long, CWideString} element, which the PDB names CQuestCardItem.
- 0x004FB490 ("CTCInventoryBase::GetCategoryWithIndex", which the PDB says returns NInventory::CCategory*) supplies the region pointers that the persist routine 0x00606300 pushes into RegionsCorrespondances. That label is suspect.
- 0x0065914B ("CBankFileAsyncData::StartRead") runs a set<long> find on this+0x64, like CTCVillage::ReportCrime. It is probably CTCVillage code.
