# Hand chunk 362 (_Sort_heap<pair<unsigned long,CAnimationEntry>> .. _Sort_heap<pair<unsigned long,CSmallVector<CEvent,32>>>): 40 of 40 byte-exact

All 40 rows grade MATCH (1) or RELOCATION_MATCH (39) with `gall.sh`. Every test prints `<addr>_TEST PASS` under g++
(`tall.sh`) and under VC7.1/Wine (`tall.py`). Every catalog label in the chunk reads `_Sort_heap<...>`, but most of them
were spread across byte-identical skeletons. 33 of the 40 name the wrong instantiation or the wrong function (see Label
corrections). Each row's real element type comes from its callee (__pop_heap / _Adjust_heap / __pop_heap_aux), which in
most cases is already landed, and from the stride divide.

Both shapes are /Gr algorithm TUs (link-time `__fastcall`): `first` is in ecx, `last` in edx, and the comparator is
passed by value on the stack (`ret 4` for an empty comparator).

## What matched (40)

- **std::sort_heap, SGI/STLport form (26 rows).** The template is
  `while (last - first > 1) pop_heap(first, last--, comp)`, with `pop_heap` and `__pop_heap_aux` inline. The value copy
  `T(*(last - 1))` is built in the argument area, then `__fastcall __pop_heap(first, last-1, last-1, value, comp, (int*)0)`
  is called. What made each row match was the element's copy shape:
  - **Counted pointers.** 005e2860 and 005e2fb0 use CIVCountedPointer (`inc [ecx+4]`). 006ba830 and 00a83600 use
    CCountedPointer with a CCPPointerInfo addref. 006a38c0 is CIntelligentPointer<CThing>, whose copy ctor runs the base
    ctor and then `SetPItem(other.GetPItem())`. 00bee990 is the implicit copy of a polymorphic CCompositeSurfaceInfo: one
    vtable store, then the Cache addref. 00cbc580 is a string key with CCountedPointer<CEntityScriptBindingBase>.
  - **String records.** 004bb570, 00624fc0 (CTestQuest), 009e7950, 00a41c20, 00a719a0, 00af3770 and 00cb6f10 are
    copies of CCharString-bearing records. The out-of-line CCharString copy ctor and dtor show up as relocations.
    006bf0e0 is the same with CWideString.
  - **Records with a named counted pointer.** 00a34d80, 00a72aa0 and 00a72b10 carry {CCharString, CCountedPointer<CFaction>}.
  - **Other records.**
    - 00cbb9d0: a u64 key with CPersistInfo.
    - 0081c670: STextureMorphPair. The memberwise user copy ctor splits into the key, then the 5-dword entry as a
      block through its own address.
    - 00a466f0: C3DSkeletalBoneMorph with a `bool (__fastcall*)(const&, const&)` comparator.
  - **Containers inline.**
    - 009b8ca0: CDefClassInfo. The STLport vector copy ctor `_Vector_base(n)` + `__uninitialized_copy` (POD memmove) is
      inline.
    - 00bef350: CEngineTextureDecal.
      - Its vector of CCompositeBlendDesc copies through a same-TU noinline `__uninitialized_copy(..., const __false_type&)`.
      - The pair<long,long> key needs the STLport explicit pair copy ctor, or `lea ebp` is scheduled early.
    - 00a84470: pair<u64, list>. The STLport list copy ctor is `_List_base(a)` (malloc'd sentinel) +
      `insert(begin(), x.begin(), x.end())`. It forwards to a same-TU `__declspec(noinline) _M_insert_dispatch`, and
      _List_const_iterator gets a converting ctor (not a copy ctor).
  - **Out-of-line `__pop_heap_aux`.** In 007fa980 (CThing** / CThingCompare_BestForTargeting2, 52 bytes) and 0080a3f0
    (pair<C3DVector,long> / CCompareTargetedPositionsForFirstPersonTargeting, 44 bytes), only `pop_heap` is inline. The
    big comparator is copied into the argument area member by member, because its C3DVector members have a user copy
    ctor.
- **std::_Sort_heap, Dinkumware form (13 rows).** The template is
  `for (; 1 < last - first; --last) _Pop_heap_0(first, last, pred, (T*)0)`. `_Pop_heap_0` builds the frame temporary
  `T(*(last - 1))`, then `_Pop_heap` does `*dest = *first` and `_Adjust_heap(first, 0, last - first, value, pred)`.
  - **POD or memberwise payloads.**
    - 007590c0 / 0081c700: a long key with a 40- or 36-byte POD payload, via `rep movsd`.
    - 00a339a0: C3DDummyObject.
    - 0076af10: an unsigned long key with a C3DVector.
    - 00a338c0: C3DHelperPoint.
    - 00a81d90 / 00a81e80: a u64 key with a bool or a dword, padded to 16 bytes.
    - 00720d30: CSkeletalMorphs::CEntry with a float.
    - 0086c950: CSkeletalMorph, whose copy ctor is `*this = other`.
  - **String keys.** 009b8170 / 00a41960 are pair<CCharString, unsigned long>. The `and esp,-8` frame for the 8-byte
    temporary comes on its own. 0077f190 is pair<CCharString, C3DVector>.
  - **006fcb90**, pair<ulong, CSmallVector<CEvent,32>>. The CSmallVector copy ctor (zero both members + Copy), the
    operator= self test, and the inline dtor `delete[] Start; Start = Size = 0` produce the two trailing zero stores.
  - Two shape lessons:
    - Do not add pop_heap's `if (1 < last - first)` guard: VC7.1 does not fold it (007590c0 went from 207v201 to a match).
    - Where plain `inline` leaves `_Pop_heap` as a local call (00a338c0, 006fcb90), `__forceinline` on
      `_Pop_heap` / `_Pop_heap_0` is needed.
- **std::__copy (1 row): 005e18b0.** This is STLport's
  `__copy(first, last, result, const random_access_iterator_tag&, int*)` over pair<ulong, CAnimationEntry>:
  `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;`. The count lives in the dead
  tag slot (`ret 0Ch`). The pair assignment is inline (4 dwords + the CIVCountedPointer operator=), and VC7.1 keeps
  `lea esi,[ebp+10h]` / `lea edi,[ebx+10h]` induction registers. It graded MATCH on the first attempt.

## Parked for Fable 5.1 (0)

(none)

## Skipped

(none). No row holds two functions. The scanner extents in the chunk include int3 padding. The trimmed lengths in
each src header were re-checked against `dis_all.txt` (last non-int3 instruction), and all 40 agree.

## Label corrections

The catalog label is propagated across byte-identical skeletons. Below is what each row really is (std:: omitted):

- 005e18b0 `_Sort_heap<pair<unsigned long,CAnimationEntry>*,...>` is not a heap sort at all. It is
  `__copy<pair<unsigned long,CAnimationEntry>*, pair<unsigned long,CAnimationEntry>*, int>` (STLport random-access copy):
  it has no pop or adjust callee and no comparator, it ends in `ret 0Ch`, and it returns `result`.
- **SGI `sort_heap` rows:**
  - 004bb570: `sort_heap<pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*, CKeyPairCompareLess<...>>`
  - 005e2860: `sort_heap<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>*, CKeyPairCompareLess<...>>`
  - 005e2fb0: `sort_heap<pair<unsigned long,CAnimationEntry>*, CKeyPairCompareLess<...>>`
  - 00624fc0: `sort_heap<CTestQuest*, CTestQuestNameCompare>`
  - 006a38c0: `sort_heap<pair<long,CIntelligentPointer<CThing>>*, CKeyPairCompareLess<...>>`
  - 006ba830 and 00a83600: `sort_heap<pair<unsigned long,CCountedPointer<?>>*, CKeyPairCompareLess<...>>` (the pointee
    type is not identified)
  - 006bf0e0: `sort_heap<pair<long,CMusicManager::CRegisteredMusicEntry>*, CKeyPairCompareLess<...>>`
  - 007fa980: `sort_heap<CThing**, CThingCompare_BestForTargeting2>`
  - 0080a3f0: `sort_heap<pair<C3DVector,long>*, (anon)::CCompareTargetedPositionsForFirstPersonTargeting>`
  - 0081c670: `sort_heap<pair<EHeroMorphType,CTextureMorphEntry>*, CKeyPairCompareLess<...>>`
  - 009b8ca0: `sort_heap<pair<unsigned long,CDefinitionManager::CDefClassInfo>*, CKeyPairCompareLess<...>>`
  - 009e7950: `sort_heap<pair<CCharString,CSymbolMap::CSymbolInfo>*, CCompareString<CSymbolMap::CSymbolInfo>>`
  - 00a34d80 and 00a72aa0: `sort_heap<pair<unsigned long,{CCharString, CCountedPointer<CFaction>}>*, CKeyPairCompareLess<...>>`
  - 00a41c20: `sort_heap<CSymbolMap::CSymbolInfo*, <name compare>>`
  - 00a466f0: `sort_heap<C3DSkeletalBoneMorph*, bool (__fastcall*)(const C3DSkeletalBoneMorph&, const C3DSkeletalBoneMorph&)>`
  - 00a719a0: `sort_heap<pair<CCharString,C3DHelperPoint>*, CCompareString<C3DHelperPoint>>`
  - 00a72b10: `sort_heap<pair<CCharString,{CCharString, CCountedPointer<CFaction>}>*, CCompareString<...>>`
  - 00a84470: `sort_heap<pair<unsigned __int64, list<16-byte record>>*, CKeyPairCompareLess<...>>`
  - 00af3770: `sort_heap<pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*, CKeyPairCompareLess<...>>`
  - 00bee990: `sort_heap<pair<long,CCompositeSurfaceInfo>*, CKeyPairCompareLess<...>>`
  - 00bef350: `sort_heap<pair<pair<long,long>,CEngineTextureDecal>*, CKeyPairCompareLess<...>>`
  - 00cb6f10: `sort_heap<pair<CCharString,CScriptInfo>*, CKeyPairCompareLess<...>>`
  - 00cbb9d0: `sort_heap<pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*, CKeyPairCompareLess<...>>`
  - 00cbc580: `sort_heap<pair<CCharString,CCountedPointer<CEntityScriptBindingBase>>*, CCompareString<...>>`
- **Dinkumware `_Sort_heap` rows with the wrong instantiation:**
  - 00720d30: `_Sort_heap<pair<CSkeletalMorphs::CEntry,float>*, CIsSkeletalMorphLowerPriorityThan>`
  - 007590c0: `_Sort_heap<pair<long, 40-byte POD>*, CKeyPairCompareLess<...>>` (the payload type is not identified)
  - 0076af10: `_Sort_heap<pair<unsigned long,C3DVector>*, CKeyPairCompareLess<...>>`
  - 0081c700: `_Sort_heap<pair<long, 36-byte POD>*, CKeyPairCompareLess<...>>` (the payload type is not identified)
  - 00a81d90: `_Sort_heap<pair<unsigned __int64,bool>*, CKeyPairCompareLess<...>>`
  - 00a81e80: `_Sort_heap<pair<unsigned __int64,unsigned long>*, CKeyPairCompareLess<...>>`
- **Catalog labels that are right:** 006fcb90, 0077f190, 0086c950, 009b8170, 00a41960, 00a338c0 and 00a339a0.
- **Callee labels found wrong on the way** (manifest names also propagated; the evidence is in each src header):

  | Callee | Manifest label | Actually |
  |---|---|---|
  | 0x005E1B60 | "pair<long,CDefPointer<...>>::operator=" | the __pop_heap of pair<EAnimComponent,CIVCountedPointer> |
  | 0x006BA180 / 0x00A81F10 | `_Pop_heap<pair<CCharString,CCountedPointer<CEntityScriptBindingBase>>*>` | __pop_heap with a dword key |
  | 0x00758910 | `_Adjust_heap<CDebugBox*>` | _Adjust_heap of pair<long, 40-byte POD> |
  | 0x007FA7B0 | "std_sort_Wrapper" | __pop_heap_aux of the CThing** / CThingCompare_BestForTargeting2 family |
  | 0x00808DB0 | `_Pop_heap_0<pair<unsigned long,CSoundPair>*>` | __pop_heap_aux of pair<C3DVector,long> |
  | 0x0081B910 | `_Pop_heap<pair<unsigned long,CSoundPair>*>` | __pop_heap of pair<EHeroMorphType,CTextureMorphEntry> |
  | 0x0081AB40 | `_Adjust_heap<CDebugCone*>` | _Adjust_heap of pair<long, 36-byte POD> |
  | 0x009B8770 | `_Pop_heap<CTestQuest*>` | __pop_heap of pair<ulong,CDefClassInfo> |
  | 0x00A32500 | `_Adjust_heap<CDebugBox*>` | _Adjust_heap of pair<ulong,C3DDummyObject> |
  | 0x00A41630 | `_Pop_heap<CTestQuest*>` | __pop_heap of CSymbolInfo |
  | 0x00A71BE0 | "std_Adjust_Heap_Pair" | __pop_heap of pair<CCharString,{CCharString,CCountedPointer<CFaction>}> |
  | 0x00A83570 | "Std_Heap_Construct_And_Adjust" | __pop_heap of pair<u64,list> |
  | 0x00BEE070 | "CItem::operator=" | __pop_heap of pair<long,CCompositeSurfaceInfo> |
  | 0x00BEEDA0 | `_Cons_val<...pair<CCharString,C3DVector>>` | __pop_heap of pair<pair<long,long>,CEngineTextureDecal> |
