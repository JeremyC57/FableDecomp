# Hand chunk 465 (vector<std::pair<CCharString .. vector<unsigned_int): 38 of 40 byte-exact

Every row in this chunk is a small STLport 4.6 helper: sort, fill, destroy, resize, push_back or operator=. The one exception is
a Dinkumware `_Ufill` from the libpk middleware. Almost every catalog label was propagated from some other body, so each
row below is named from its callers, the Ego_r.pdb name sets and the element layout. 37 rows matched on the first grade and
0x0086D8C0 on the second (a class-name fix only). Every test passes under both g++ and VC7.1 under Wine. calls.py confirms
that every call goes to retail's target address.

## What matched (38)

- **_STL::__uninitialized_fill_n (13)**: 0x004B81D0 CActivateQuestInfo, 0x00526E40
  CSharerParserOutputToDebugContextMapper::CEntry, 0x006E65F0 CConversationLine, 0x009E5FE0 CSymbolMap::CSymbolInfo,
  0x00AD1700 COptimisedBone, 0x00A3FC00 pair<pair<CCharString, CCharString>, long>, 0x00CD4EA0 CMarkerDataEntry
  (descriptive name), 0x00A486B0 CLipSyncPhoneme, 0x007D2370 CShoveHaPennyRegionDef, 0x00709CE0
  pair<NGraphicAppearance::CMeshInfo, CEngineTextureMorphSet>, 0x0086D8C0 CRandomAppearanceMorph::CBodyPartMesh, 0x0051A880
  and 0x00A830A0 pair<unsigned __int64, list<C2DLineF> >.
  Each one copy-constructs n elements into raw storage. Shape: STLport verbatim
  (`for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x)`). `__false_type` needs a user constructor. Placement new keeps
  its `test esi,esi ; je` null test. The element's real copy semantics then produce the rest:
  - CCharString copies go through the out-of-line 0x0099EC30.
  - The CDefClassBase base copy is 0x0046644C, followed by the own-vtable store.
  - The CEngineTextureMorphArray copy is 0x005BBDF1, and the STLport vector copy is 0x00673E10.
  - The list copy is inline: a self-linked `malloc(0x18)` header (0x00BFEA0E), then `_M_insert_dispatch` 0x005152C0. Its
    iterator has a user copy constructor and its const_iterator an implicit one.
- **_STL::fill_n (2)**: 0x006149B0 over `unsigned __int64` and 0x00A5C510 over C4DVector. These assign n values and return
  first + n. They are told apart from the __uninitialized_fill_n rows by the missing null test and `ret 4`.
- **_STL::__unguarded_insertion_sort_aux (8)**: 0x00AF3240 pair<CCharString, NParticleEngine::CRegisteredPSCInfo>, 0x00624330
  CTestQuest / CTestQuestNameCompare, 0x00A45F90 C3DSkeletalBoneMorph with a `__fastcall` function-pointer comparator,
  0x00A6F8B0 pair<CCharString, C3DHelperPoint> / CCompareString<C3DHelperPoint>, 0x00CB6860 pair<CCharString, CScriptInfo>,
  0x00A32340 pair<unsigned long, C3DHelperPoint>, 0x0071FCF0 pair<long, CTCHeroMorph::CTextureLayer>, and 0x0080AB40
  NPlayerTargeting::CTargetPoint with the 52-byte CThingCompare_BestForTargeting2 passed by value.
  Shape: `for (__i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);`. The element is
  copy-constructed in its outgoing argument slot, and the comparator's operator() stays declaration-only.
  - In 0x0071FCF0 the colours are copied byte by byte in R, G, B, A order: that is CRGBColour's
    `Set(o.R, o.G, o.B, o.A)` user copy constructor.
  - In 0x0080AB40 the comparator is copied memberwise, with the bool moved as a byte, because C3DVector has a user copy
    constructor. The CIntelligentPointer<CThing> copy is inline: 0xA01B10, vtable 0x01238C6C, GetPItem 0xA01B50, SetPItem
    0xA01B90.
- **_STL::__insertion_sort (1)**: 0x00A415D0 over pair<pair<CCharString, CCharString>, long>.
  `if (first == last) return; for (i = first + 1; ...) __linear_insert(first, i, *i, comp)`, with the element passed BY VALUE
  to 0x00A40F50 (`ret 4`).
- **_STL::__destroy_aux (1)**: 0x00CDB640 TeleportData*. Four `~CCharString` 0x0099EAE0 calls per element, the last
  declared member first.
- **_STL::vector::resize(n) (5)**: 0x00AE7AA0 C3DQuad2, 0x00AE7B10 C2DVector, 0x00BBDD40
  CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator (`Valid(false)` default), 0x00BCC3B0
  CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry, and 0x00BE0140 C3DBoundingSphere (zeroing default).
  Shape: `resize(n) { resize(n, _Tp()); }`, with `resize(n, x)` inlined and erase / `_M_fill_insert` out of line. The TU also
  emits `resize(n, x)`, so the Leaf names the full `...QAEXI@Z` symbol.
- **_STL::vector::resize(n, x) (5)**:
  - 0x0085E8D0 NThingMap::CThingMapCell: erase inline, `__copy` loop with a long distance.
  - 0x00A2D0E0 CMeshDataBankEntry* and 0x00B2AAB0 CSurface*: erase inline, `__copy_trivial` with the direct memmove call
    0x00BFEAE6.
  - 0x00467686 and 0x00486189 unsigned char: erase and fill out of line, `#pragma optimize("s", on)`.
- **_STL::vector::operator= (1)**: 0x005F9074 vector<pair<long, EHeroAbility> > (the AbilityButtons map), STLport verbatim
  under `#pragma optimize("s", on)`. Callees: `_M_allocate_and_copy` 0x005F8BD8, free 0x00BFEA14, fastcall `__copy` 0x005F7C3A,
  `__uninitialized_copy` 0x005F8325.
- **_STL::vector::push_back (1)**: 0x00818DC0 vector<pair<NHitLocations::CHitLocation, long> >. The two CDefPointer copies
  bump RefCount at +4 of the polymorphic def. The overflow path calls `_M_insert_overflow` 0x00818CA0 (fill length 1, at
  end).
- **std::vector::_Ufill (1)**: 0x00C29840, Dinkumware (libpk) vector<CHandleBasedMemoryPool::CHandleInfo>. An inline
  `_Uninitialized_fill_n` + `_Ptr_cat` calls the fastcall `_Uninit_fill_n` 0x00C294B0. `_Vector_val` puts `_Alval` at +0.

## Parked for Fable 5.1 (2)

- 00ab44d0 _STL::vector::resize DIFFER(85v85): retail `xor eax,eax ; mov [esp+8],eax` before `mov edi,esi ; sub edi,edx`, and erase start `lea edx,[edx+eax*8] ; push edx`; ours stores both zero dwords after the sub and uses `lea eax ; push eax` | tried: pair<long,long> with `first(_T1()), second(_T2())`, a struct with ctor-body zero assignments, a CUID64-style struct over an unsigned __int64 member, a scalar unsigned __int64 element (4 grades) | next: lane gap: scheduling (compiler build); the element type is unconfirmed (no callers or address refs), so try a real 8-byte PDB element with a zeroing default ctor once one is found
- 00b6cc90 _STL::vector::resize DIFFER(85v85): the same two differences as 00ab44d0 (first zero store scheduled after `sub edi,edx`; `lea eax / push eax` for `lea edx / push edx`) | tried: pair<long,long>, a struct with ctor-body assignments, /Oa, CEngineLandscapeRenderer::CSubPatchSorterEntry with bitfield inits (3 grades) | next: lane gap: scheduling (compiler build); same body shape as 00ab44d0, so whatever fixes one fixes both

## Skipped

None. No row holds a second function.

## Label corrections

- 0x00AB44D0, 0x00B6CC90 "vector<std::pair<CCharString ...>::resize": `_STL::vector<T>::resize(n)` over an unidentified 8-byte
  element with a zero default.
- 0x00AE7AA0, 0x00AE7B10, 0x00BBDD40, 0x00BCC3B0, 0x00BE0140 "vector<std::pair<CCharString ...>::resize": `vector<T>::resize(n)`
  over C3DQuad2, C2DVector, CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator,
  CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry and C3DBoundingSphere.
- 0x00AF3240 "vector<std::pair<CCharString ...>::_Destroy": `_STL::__unguarded_insertion_sort_aux<pair<CCharString,
  NParticleEngine::CRegisteredPSCInfo>*, ..., CKeyPairCompareLess<...> >`.
- 0x00CDB640 "vector<std::pair<CCharString ...>::_Destroy": `_STL::__destroy_aux<TeleportData*>`.
- 0x00624330, 0x0080AB40, 0x00A45F90, 0x00A6F8B0, 0x00CB6860 "vector<std::pair<long ...>::_Destroy": `__unguarded_insertion_sort_aux`
  over CTestQuest, NPlayerTargeting::CTargetPoint, C3DSkeletalBoneMorph, pair<CCharString, C3DHelperPoint> and
  pair<CCharString, CScriptInfo>.
- 0x00A415D0 "vector<std::pair<long ...>::_Destroy": `_STL::__insertion_sort<pair<pair<CCharString, CCharString>, long>, ...>`.
- 0x0071FCF0, 0x00A32340 "vector<std::pair<unsigned long ...>::_Destroy": `__unguarded_insertion_sort_aux` over
  pair<long, CTCHeroMorph::CTextureLayer> and pair<unsigned long, C3DHelperPoint>.
- 13 "vector<unsigned __int64>::_Ufill" rows are `_STL::__uninitialized_fill_n<T*, unsigned int, T>` (elements above).
  0x006149B0 and 0x00A5C510 are `_STL::fill_n`.
- 0x005F9074 "vector<unsigned int>::operator=": `_STL::vector<pair<long, EHeroAbility> >::operator=`.
- 0x0085E8D0, 0x00A2D0E0, 0x00B2AAB0 "vector<unsigned int>::resize": `vector<NThingMap::CThingMapCell>`,
  `vector<CMeshDataBankEntry*>` and `vector<CSurface*>::resize(n, x)`.
- 0x00C29840 "vector<unsigned int>::_Ufill": `std::vector<CHandleBasedMemoryPool::CHandleInfo>::_Ufill` (Dinkumware).
- Callee and caller labels seen on the way:
  - 0x0099EAE0 (manifest `_Cons_val<...>`) is ~CCharString.
  - 0x005F7C3A (`_Copy_impl<CComplexNumber*, ...>`) is STLport `__copy<const pair<long, EHeroAbility>*, ...>`.
  - 0x005F8325 (`_Uninit_copy<...>`) is `__uninitialized_copy`.
  - 0x00485B18 (`SelectEntryFromData`) is `vector<unsigned char>::erase`.
  - 0x005BBDF1 (`Add`) is the CEngineTextureMorphArray copy constructor.
  - 0x00721FE0 (`std::make_heap_0x18_presort`) is the texture-layer sort that calls the landed `__final_insertion_sort`
    0x007218E0.
  - The function holding 0x0080B196 (manifest `_Insertion_sort<CThing**, CThingCompare_BestForTargeting2>`) sorts
    NPlayerTargeting::CTargetPoint, not CThing**.
