# Hand chunk 386 (std::fill<CThemeChannel*> .. CTCHeroOpinionDeedLog): 39 of 40 byte-exact

## What matched (39)

- **STLport algorithms over the environment-theme types (006b6560, 006b6590, 006b6610, 006b6710, 006b67a0):**
  - `fill<CThemeChannel*>` uses the 0x1F0-byte element and op= 0x006B6180.
  - `uninitialized_fill_n<CThemeChannel*>` is the 3-arg `ret 4` front end over a `__forceinline` inner
    `__uninitialized_fill_n`, with copy ctor 0x006B6010.
  - Two `__destroy_aux` instantiations: CThemeChannel (CArray dtor `if (_M_start) free`) and
    CBlendedEnvironmentTheme (0x1F4 bytes).
  - `__uninitialized_copy<CEnvironmentThemePriorityStackEntry*>`: implicit copy ctor plus CThemeWeightArray copy
    0x006B58F0.
- **STLport over CGameMapCell (7 bytes) (006b78e0, 006b7920, 006b79a0, 006b7c10, 006b7cc0, 006b7d60, 006b7f70):**
  - `fill`, the two random-access `__copy` twins, `__uninitialized_fill_n` (`ret 8`) and `uninitialized_fill_n`
    (`ret 4`, `__forceinline` inner). The element has an empty user ctor.
  - `vector(n)` uses `_Vector_base(n, a)` inline and `uninitialized_fill_n` 0x006B7CC0.
  - `_M_insert_overflow` matches only with same-TU `auto_inline(off)` bodies of `__uninitialized_copy` 0x006B7BB0 and
    `__uninitialized_fill_n` 0x006B7C10. Then the tag temporaries take a dead argument slot (declared-only gives
    `sub esp,8`).
- **CFaction / CFactionManager (006b8f40, 006b93a0, 006b9460, 006ba100, 006ba3b0, 006ba7b0, 006badb0):**
  - Sort helpers over `pair<unsigned long, CCountedPointer<CFaction>>`:
    - `__unguarded_linear_insert`: the counted-pointer op= reads Info and Data first.
    - `__unguarded_partition`: `iter_swap` 0x006B8E70 is declared `__fastcall`.
    - `__linear_insert`: needs a same-TU `__declspec(noinline)` memberwise pair op= 0x006B8E20. A declared-only op=
      gives 128v113.
    - `__partial_sort`: `__make_heap` 0x006BA230, `__pop_heap` 0x006BA180, `sort_heap` 0x006BA830.
  - `GetPFaction`: const LowerBound 0x006B8AE0 plus the inline Find ternary.
  - `IsAlliedWithFaction`: `return faction.Name == Name || AlliedFactions.count(faction.Name) != 0;`. STLport `count`
    is `_M_find(k) == header ? 0 : 1`. CCharString `==` takes references, with the Compare 0x00411570 body under
    `auto_inline(off)`.
  - `CFaction(const CCharString&)` uses the `_Rb_tree_base`/`_Rb_tree` model of landed 0x004C60F0.
- **CGameTimeManager::GetTimeAs24HourClock (006bb300):** the VC7.1 `<math.h>` float overload
  `floor(float) = (float)floor((double)x)` gives the float-typed `fsubr`/`fmul dword`. The body is
  `floor(h) * 100 + (long)((h - floor(h)) * 60)`.
- **CRegion (006bc290, 006bc350):**
  - `TransferGameState` is three `context.Transfer("", bool)` calls (CheckCRC 0x00404500, mode +0x18).
  - `GetMiniMapRegionExitTextOffset` needs the found-arm-first layout
    `if (it != end()) { offset = it->second; return true; } return false;`.
- **CMusicManager (006bd8a0, 006be870, 006be930, 006bed50, 006bee80, 006bf020, 006bf340, 006bf410):**
  - `UpdateCurrentMusicOffset`: the sound-system global goes into a local, with virtual slot 49 GetMusicPosition.
  - Sort and heap helpers over `pair<long, CRegisteredMusicEntry>`: `iter_swap` needs `__forceinline __iter_swap`.
  - `CArray<..>::push_back` must hold the STLport `vector::push_back` body itself. A CArray member calling the
    in-class vector member is not inlined (a 5-byte `jmp`).
- **back_insert_iterator<list<CIntelligentPointer<CThingAICreature>>>::operator= (006c1e70):**
  - Body: `container->push_back(v); return *this;`, where push_back is STLport `insert(end(), x)` (end read before the
    malloc'd node).
  - The CIntelligentPointer copy ctor must go through typed inline `T* GetPItem() const` / `SetPItem(T*)` wrappers.
    Calling the base members directly swaps this/data between ebp and ebx (82v82).
- **CLevelLoader (006c2120, 006c2940):**
  - `Add(const CCountedPointer<CObject>&)`: `object->OnAdd(*this); ObjectList.push_back(object);` (inline list push and
    counted copy).
  - `CObject::CLevelFiles::operator=` is the implicit memberwise copy assignment (LevelID + three CCountedPointer op=).
    It is emitted by taking its address in a NON-const global member pointer.
- **CTCFadeOutAndIn (006c3df0, 006c3e80):**
  - `AddThingToFade`: `if (find(begin, end, CIntelligentPointer<CThing>(&thing)) == end) push_back(CIntelligentPointer<CThing>(&thing));`.
  - `UpdateFadingThings`:
    - Starts with `if (!empty()) erase(remove_if(.., CIsThingDead()), end());`, with STLport `remove_if` inline:
      find_if 0x006C4270, remove_copy_if 0x006C4210.
    - Then `AddCarriedThingsOfThingToFadeList` 0x006C3CA0 runs on the parent and on each entry of a local copy of the
      list.
    - Needs same-TU `auto_inline(off)` bodies of the vector copy ctor and `_Vector_base(n, a)`, plus a user ctor on
      CIsThingDead.
- **std::remove_copy_if<CIntelligentPointer<CThing>*, CIsThingDead> (006c4210):** STLport verbatim. The predicate is
  `GetPItem() == 0 || GetPItem()->IsDead()` (state bit +0x91 & 1); the assignment is `SetPItem(other.GetPItem())`.
- **CTCHeroOpinionDeedLog (006c5710, 006c5780, 006c5800):**
  - These three in-order map walks return `it->first` or -1:
    - `FindWitnessDeed(long)`: type 0x35 with ExtraData == handle.
    - `FindDeed(type, target)`.
    - `FindCriminalDeed(type, target)`: also checks IsCrime +0x23, re-read through `it->second`.
  - STLport 4.5 `_M_node = _Rb_global<bool>::_M_increment(_M_node)` is inline.
  - With a call in the loop, `end()` must be a named local.

## Parked for Fable 5.1 (1)

- 006bd900 CMusicManager::PlayMusic DIFFER(170v169): register permutation only. Retail has g_pSoundSystem ebx / this esi / musicID ebp / entry edi; ours has ebp / edi / ebx / esi. The +1 byte is the disp8 of `mov edx,[ebp+0]` | tried: entry reference with a same-TU noinline LowerBound 0x006BE5E0 body (kept), a local sound-system pointer, `it->second.` everywhere (177v169), an entry pointer + else-if | next: the 0x006C1E70 lever (an extra typed inline accessor layer on the entry or the sound system raises one variable's weight), or a `const CRegisteredMusicEntry&` alias plus a named `end` local; else lane gap: register-weight tie.

## Skipped

None. All 40 rows are single functions. No int3-after-noreturn rows, no jump tables, no retail `fistp`.

## Label corrections

Catalog labels that were propagated or descriptive (Ego_r.pdb / STLport identities):

| address | catalog label | real identity |
|---|---|---|
| 006b6560 | fill<CArray<unsigned short>*> | std::fill<CThemeChannel*> |
| 006b6590 | _Fill_n | uninitialized_fill_n<CThemeChannel*> |
| 006b6610 | _Copy_backward<C3DVertex2*> | __destroy_aux<CThemeChannel*> |
| 006b67a0 | _Copy_backward<C3DVertex2*> | __destroy_aux<CBlendedEnvironmentTheme*> |
| 006b6710 | _Move<CMusicEntry*> | __uninitialized_copy<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*> |
| 006b78e0 | fill<C3DTriangle2*> | fill<CGameMapCell*> |
| 006b7920, 006b79a0 | _Copy_impl | STLport __copy<const CGameMapCell*, CGameMapCell*, int> |
| 006b7c10 | _Fill_n | __uninitialized_fill_n<CGameMapCell*> |
| 006b7cc0 | _Fill_n | uninitialized_fill_n<CGameMapCell*> |
| 006b7d60 | Vector_AllocateAndInit | vector<CGameMapCell>::vector(n) |
| 006b7f70 | Vector_InsertN | vector<CGameMapCell>::_M_insert_overflow |
| 006b8f40 | Vector_CopyWithRefCount | __unguarded_linear_insert<faction pair*> |
| 006b93a0 | CVectorMap_Lookup | CFactionManager::GetPFaction(unsigned long) const |
| 006b9460 | FactionMap_FindAndCompare | CFaction::IsAlliedWithFaction |
| 006ba100 | Heap_PartitionByKey | __unguarded_partition |
| 006ba3b0 | CFaction_InitInternalLists | CFaction::CFaction(const CCharString&) |
| 006ba7b0 | Vector_InsertOrShift | __linear_insert |
| 006badb0 | Heap_SiftDown | __partial_sort |
| 006bb300 | TimeOfDay_GetRoundedHour | CGameTimeManager::GetTimeAs24HourClock |
| 006bc290 | CGameState_TransferBoolFlags | CRegion::TransferGameState |
| 006bc350 | MusicEntry_GetData | CRegion::GetMiniMapRegionExitTextOffset |
| 006bd8a0 | Music_UpdateVolume | CMusicManager::UpdateCurrentMusicOffset |
| 006bd900 | Music_LoadAndStartTrack | CMusicManager::PlayMusic |
| 006be870 | swap<CScriptInfo> | iter_swap<pair<long, CRegisteredMusicEntry>*> |
| 006be930 | Vector_ShiftElementsWideString | __unguarded_linear_insert |
| 006bed50 | Heap_PartitionScriptInfo | __unguarded_partition |
| 006bee80 | Heap_BuildHeapMusic | __make_heap |
| 006bf020 | Vector_InsertWideStringPair | __linear_insert |
| 006bf340 | Heap_SiftDownMusic | __partial_sort |
| 006bf410 | Vector_PushBackMusicEntry | CArray<pair<long, CMusicManager::CRegisteredMusicEntry>>::push_back |
| 006c1e70 | LinkedListNode_Create | back_insert_iterator<list<CIntelligentPointer<CThingAICreature>>>::operator= |
| 006c2120 | LevelLoader_CreateObjectEntry | CLevelLoader::Add(const CCountedPointer<CLevelLoader::CObject>&) |
| 006c2940 | swap<CCompositeSurfaceInfo> | CLevelLoader::CObject::CLevelFiles::operator= |
| 006c3df0 | Region_AddPointerIfNew | CTCFadeOutAndIn::AddThingToFade(CThing&) |
| 006c3e80 | Region_RemoveDeadThings | CTCFadeOutAndIn::UpdateFadingThings() |
| 006c4210 | _Remove_if<..> (Dinkumware donor name) | STLport remove_copy_if<CIntelligentPointer<CThing>*, CIsThingDead> |
| 006c5710 | CrimeList_FindEntryByID | CTCHeroOpinionDeedLog::FindWitnessDeed(long) |
| 006c5780 | BinaryTree_FindNode | CTCHeroOpinionDeedLog::FindDeed(EOpinionDeedType, const CThing*) |
| 006c5800 | CBaseObject_FindInListByID | CTCHeroOpinionDeedLog::FindCriminalDeed(EOpinionDeedType, const CThing*) |

Callee labels that are also wrong:

| address | catalog label | real identity |
|---|---|---|
| 0x006B8E20 | CItem::operator= | pair<unsigned long, CCountedPointer<CFaction>>::operator= |
| 0x006B6010 | CFeatDef_CopySpecializedData | CThemeChannel copy ctor |
| 0x006B6180 | CBlendedEnvironmentTheme::operator= | CThemeChannel::operator= |
| 0x006C3CA0 | COptimisedPrimitive_RemapAndFilter | CTCFadeOutAndIn::AddCarriedThingsOfThingToFadeList(const CThing&) |
| 0x006C4270 | FindFirst_DeadThing | STLport find_if<CIntelligentPointer<CThing>*, CIsThingDead> (random-access) |
| 0x004AC0F0 | std::vector::clear | vector<CIntelligentPointer<CThing>>::erase(first, last), `ret 8` |
| 0x0062ED40 | landed as CRetailVector_CIntelligentPointer_0062ed40 | vector<CIntelligentPointer<CThing>>::vector(const vector&) |
