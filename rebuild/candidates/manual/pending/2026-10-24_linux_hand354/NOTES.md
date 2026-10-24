# Hand chunk 354 (_Destroy_range .. _Find<list<NUISystem::CComponent*>>): 40 of 40 byte-exact

18 MATCH + 22 RELOCATION_MATCH; every test prints PASS under g++ (tall.sh) and VC7.1 (tall.py). Every `// Leaf:` was
checked against the object's symbol table (exactly one defined symbol each), and the call targets that RELOCATION_MATCH
masks were checked where an implicit operator= sits next to the graded body (0x0067C720 calls both vector operator=s).

## What matched (40)

- **_Destroy_range (6 + 1 renamed)** -- STLport `_Destroy_range(first, last, const allocator&)` running the implicit
  element destructor. The inlined CCountedPointer release (decrement, re-read, destroy hook [block+4] with the object in
  ecx, delete the block, zero both words) sits on a count-block second induction register (`lea esi,[edi+0Ch|8]`):
  - 0x00A4A2A0: 16-byte {CCharString, .., CCountedPointer}.
  - 0x00A70EE0: pair<CCharString, C3DMeshVolume>; second is destroyed before first.
  - 0x00A80E40: pair<long, CCountedPointer<CRegionConnectionNode>>.
  - 0x00CBB1B0: a 16-byte counted record.
  - 0x00CBB200: 12-byte {CCharString, CCountedPointer}.
  - 0x00AF6F90: pair<EPSCType, CComponentInstance>. The base dtor call is on the vptr word with no vtable reset,
    because the polymorphic member's destructor is compiler-generated.
  - 0x00A96810 is not a _Destroy_range: it is DestructArray<CAnimatedBlock>(blocks, count), with a null guard, an
    unsigned count, and a `dec edi ; jne` countdown over the CIVCountedPointer member.
- **_Equal_range (1)** -- 0x00817E30 is STLport `__equal_range` over THitLocationIntersectionElement with a by-value
  predicate and a `const long&` key. Retail calls __upper_bound (0x008174F0) BEFORE __lower_bound (0x008174B0) and never
  advances `first` in place. The source form is `return pair(__lower_bound(first, middle, ..), __upper_bound(middle + 1,
  first + len, ..))`, and VC's right-to-left argument order produces the retail call order. 2 grades.
- **fill (13)** -- STLport `fill(first, last, const T&)`, ret 4, the value reference in edi:
  - Trivial block copies (`mov esi/edi`, words through ebx): 0x009D1BB0 pair<ulong, CSoundPair> and 0x00819230
    pair<EHeroMorphType, CTextureMorphEntry>.
  - Memberwise copies, with the source member addresses hoisted out of the loop and `last` re-read from the
    `push ecx` frame word:
    - 0x00613180 CUIMiniMapMarker (CCharString operator=, second IV at Position).
    - 0x005664B0 {3 CCharString, 2 bytes}.
    - 0x00A85810 {3 vectors, Dirty}.
    - 0x00A335E0 pair<long, C3DMeshVolume> (CCharString op= plus the inlined CCountedPointer operator=, both source
      words read first).
    - 0x008895F0 CEnvironmentThemePriorityStackEntry (CArray op= 0x00888EE0, the double through fld/fstp qword).
  - Intrusive-pointer operator= (self test, release through a local copy, store, count bump):
    - 0x00A43890 CDataBankTextEntry.
    - 0x00AF1C70 CParticleEmitterInfo (12 bytes).
    - 0x005E1820 pair<ulong, CAnimationEntry> (second IV at Components).
  - Polymorphic elements, whose implicit operator= skips the vptr:
    - 0x007D1420 CShoveHaPennyRegionDef: three 1-bit flags copied by read-modify-write.
    - 0x00AF6D90 pair<EPSCType, CComponentInstance>.
    - 0x0067C720 CBlurredAttitudesDef: the retail def hierarchy is CDefClassBase pack(1) 0x25, then CParentDefClassBase
      pack(1) with InstantiationName +0x25 and a 16-byte CVectorMap +0x29, so sizeof is 0x39 and Attitudes sits at
      +0x3C.
- **__uninitialized_fill_n (11)** -- STLport `__uninitialized_fill_n(first, n, x, const __false_type&)`. The placement
  new keeps its null check, and n counts down. The copy constructor's form decides the shape:
  - One block, so the implicit trivial copy: 0x007183E0 and 0x00F3B220 (24-byte morph record).
  - STLport pair's memberwise "VC++ needs this" copy constructor: 0x0081A200 (key, then a 20-byte block) and
    0x004CFA70 pair<float, CCameraTag> (key, then a 24-byte block).
  - A flat record with a user copy constructor, each scalar copied through esi with no second IV: 0x0040EAC0 and
    0x00B8E930 (7 words), 0x00F37AD0 (11 words).
  - Memberwise through CCharString: 0x008EBAE0 CHoverState; 0x00613E30 CUIMiniMapMarker (C2DVector needs a user copy
    ctor, otherwise a second IV appears).
  - Nested user copy constructors keep the second IV: 0x00CB6160 pair<CCharString, CScriptInfo> and 0x006BE770
    pair<long, CRegisteredMusicEntry>.
- **__unguarded_insertion_sort_aux (1)** -- 0x00470D77 over pair<ulong, CSoundPair> with CKeyPairCompareLess. The
  frame form needs `optimize("y", off)` + `optimize("s", on)`, a __forceinline __unguarded_linear_insert, and STLport's
  pair copy constructor (the by-value `val` copied word by word into the 12-byte frame local). 3 grades.
- **MemCopy (1)** -- 0x009A0020: the /O2 inline memcpy (`rep movsd` of size >> 2, then `rep movsb` of size & 3); one
  stack word.
- **remove_copy (1)** -- 0x00806D80 STLport remove_copy over NPlayerTargeting::CTargetPoint, with CTargetPoint's
  operator== (resolved things, then Index) and its assignment inlined; second IV at TargetingPosition.
- **find / __find / find_if / __find_if (5)**:
  - 0x00535F38: `__find(first, last, val, input_iterator_tag)` over list<CComponent*>.
  - 0x006C4FC0: `find(first, last, val)` with __find inlined (`ret 8`, no edx tag).
  - 0x0069CAF0: `__find` over list<CIntelligentPointer<const CThing>>. The value is the intelligent pointer, and both
    sides go through GetPItem.
  - 0x0076D810: `find_if(first, last, pred)` with a by-value one-pointer predicate in the third stack slot and edx
    unused.
  - 0x004B91F0: `__find_if(first, last, CStringIs pred, tag)`. The wanted string's block is hoisted across Compare,
    whose body is kept out of line in the TU under `auto_inline(off)`.

## Parked for Fable 5.1 (0)

None.

## Skipped

None.

## Label corrections

The catalog labels in this chunk are almost all propagated across byte-identical bodies. Real identities:

- 0x00A4A2A0 `_Destroy_range<pair<long,CCompositeSurfaceInfo>>` -> _Destroy_range of a 16-byte record with a
  CCharString at +0 (not a long) and a CCountedPointer.
- 0x00AF6F90 `_Destroy_range<pair<long,CCompositeSurfaceInfo>>` -> _Destroy_range<pair<NParticleEngine::EPSCType,
  CComponentInstance>> (24 bytes).
- 0x00CBB200 `_Destroy_range<pair<long,CCompositeSurfaceInfo>>` -> _Destroy_range of a 12-byte {CCharString,
  CCountedPointer} element.
- 0x00CBB1B0 `_Destroy_range<pair<long,CCountedPointer<CRegionConnectionNode>>>` -> stride is 16, not 12 (a 16-byte
  counted record).
- 0x00A96810 `_Destroy_range<pair<EAnimComponent,CIVCountedPointer<..>>>` -> DestructArray<CAnimatedBlock>(CAnimatedBlock*,
  unsigned long), a __fastcall free function.
- 0x00470D77 `_Fill<CEngineCamera::CDebugScriptElement*>` -> std::__unguarded_insertion_sort_aux<pair<unsigned long,
  CSoundPair>*, .., CKeyPairCompareLess>.
- 0x009A0020 `_Fill<CEngineCamera::CDebugScriptElement*>` -> MemCopy(void*, const void*, unsigned long), an inline
  memcpy wrapper. The manifest's three-pointer prototype is wrong.
- 0x00AF1C70 `_Fill<NGameText::CDataBankTextEntry*>` -> fill<NParticleEngine::CParticleEmitterInfo*> (12-byte stride;
  the name comes from the 8-byte twin 0x00A43890).
- 0x005664B0 `_Fill<pair<CCharString,CRuntimeQuestPersistence::CPersistInfo>*>` -> fill of a 16-byte {3 CCharString,
  2 bytes} record.
- 0x00A85810 (same catalog name) -> fill of a 40-byte {3 vectors, empty comparator, Dirty} record.
- 0x008EBAE0 `_Fill<pair<EHeroMorphType,CTextureMorphEntry>*>` -> __uninitialized_fill_n<NCreatureMode::CModeHoverBase::
  CHoverState*>.
- 0x007183E0, 0x0081A200, 0x00F3B220 (same catalog name) -> __uninitialized_fill_n, not _Fill.
- 0x00613E30 `_Fill<pair<float,CCameraTag>*>` -> __uninitialized_fill_n<CTCInventoryMap::CUIMiniMapMarker*>.
- 0x007D1420 (same catalog name) -> fill<CShoveHaPennyRegionDef*>.
- 0x00F37AD0 (same catalog name) -> __uninitialized_fill_n of a 44-byte flat record. The neighbouring C3DDummyObject
  label does not fit either (52 bytes in the PDB).
- 0x0040EAC0, 0x00B8E930, 0x004CFA70 (same catalog name) -> __uninitialized_fill_n, not _Fill. The first two are flat
  7-word records, not the nested pair.
- 0x00CB6160 `_Fill<pair<CCharString,CScriptInfo>*>` and 0x006BE770 `_Fill<pair<long,CRegisteredMusicEntry>*>` ->
  __uninitialized_fill_n over those pairs.
- 0x00AF6D90 `_Fill<pair<long,CCountedPointer<CContainmentVolume>>*>` -> fill<pair<EPSCType, CComponentInstance>*>
  (24-byte stride, vptr at +4 untouched).
- 0x0067C720 `_Fill<pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>` ->
  fill<CBlurredAttitudesDef*>. It is the `fill` callee of the landed 0x0067CC50.
- 0x008895F0 (same catalog name) -> fill<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry*>. It is the `fill`
  callee of the landed 0x00889DA0.
- 0x00806D80 `_Find<NPlayerTargeting::CTargetPoint*>` -> std::remove_copy<CTargetPoint*, CTargetPoint*, CTargetPoint>.
- 0x004B91F0 `_Find<list<CCharString>::iterator, CCharString>` -> __find_if with a CCharString-holding predicate (plus
  the input_iterator_tag).
- 0x0069CAF0 `_Find<list<CIntelligentPointer<const CThing>>, CThing*>` -> __find with a `const CIntelligentPointer<const
  CThing>&` value (not CThing*).
- 0x0076D810 (same catalog name) -> find_if(first, last, pred) with a by-value predicate and no tag.
- 0x006C4FC0 `_Find<list<CComponent*>>` -> plain std::find (the __find body inlined).
- 0x00817E30 `_Equal_range<..>`: identity correct; it is STLport's `__equal_range` (callees __upper_bound 0x008174F0,
  __lower_bound 0x008174B0).

No row looked cut wrong: every graded body ends at its final `ret` inside the scanner extent. In 17 rows the chunk
length also counts the trailing int3 alignment pad (STATUS.tsv bytes and oracle.tsv give the trimmed body length).
