# Hand chunk 357 (_Move<CMapDesc*> .. _Move<pair<long,CCountedPointer<CParticleEmitter>>*>): 40 of 40 byte-exact

Every catalog row in this chunk carries a propagated Dinkumware-style `_Move<T*>` label. None of the 40 bodies is
a Dinkumware `_Move`. They are SGI/STLport algorithm instances in five shapes: the counted `__copy`,
`__copy_backward`, STLport `__uninitialized_copy`, `__insertion_sort` and `for_each`. All are `__fastcall`
free functions, so each is named `_global::<Algorithm>_<Element>_<addr>` with a matching `// Leaf:`.
Graded with q.py (all MATCH / RELOCATION_MATCH, see grades.txt). The tests PASS under both g++ (tall.sh) and the
VC7.1 lane (tall.py).

## What matched (40)

- **STLport `__uninitialized_copy(first, last, result, const __false_type&)`** (`ret 8`, null-tested placement copy), 13 rows:
  - 00776b60, 007770c0 (0x24 key-camera record), 009f6c00 (0x30 record with an out-of-line array copy ctor 0x009F66A0),
    00a95a10, 00b31a30 (0x20), 00bf5b50 (0x1c), 00b28ed0 (0x38 with two flag bytes). Shape lesson: the element has a
    memberwise USER copy ctor around a 3-long record that has its own user copy ctor. VC7.1 keeps a second
    induction pair at `&record + 8` (`lea edi,[eax+N] ; lea esi,[ecx+N]`, stores `[edi-k]`), so N - 8 gives the
    record offset. The models are reused from the landed fill_n twins 0x00776BC0 / 0x009F6C90.
  - 00a2af20 `__uninitialized_copy<C3DAnimationInfo*>`. The vtable stores 0x0129CD28 (CMeshDataBankEntry) and
    0x0129CD64 (C3DAnimationInfo) plus the 0x28 stride fix the identity. A local polymorphic base/derived model
    gives the implicit copy ctor: base vtable, Bank/Index, derived vtable, a CIVCountedPointer AddRef, the floats.
  - 0081a650, 008247e0, 00b3e340 (20-byte POD, implicit copy ctor: `mov esi,ecx ; mov edi,eax` + ebx moves) and 0085db50
    (the same record with an explicit memberwise copy ctor, which copies directly through `[ecx+N]` with only esi).
  - 009e6270 `__uninitialized_copy<pair<CCharString, CSymbolMap::CSymbolInfo>*>`: three CCharString copy ctors + Val.
  - Lesson for 009f6c00 / 009e6270, where the loop calls out of line: `T* cur = result;` puts result in ebp before
    the empty test and returns it from the shared epilogue. The `++result` form adds a separate early-out.
- **SGI counted `__copy(first, last, result, const random_access_iterator_tag&, int*)`** (`ret 0Ch`), 16 rows:
  - 00841de0: STLport strings (12-byte). The inline `operator=` self-test plus `_M_assign_dispatch` 0x00569140 comes
    from the landed LAddress 0x00841F60, and the tag temporary is homed in the dead `result` slot.
  - 00708c20 CModifierGraphic and 00af1ce0 CParticleEmitterInfo (byte twins), plus 009ca660 / 00a438e0
    CDataBankTextEntry. Each uses the landed intrusive-pointer `operator=` from 0x00A43720 (release through
    slot 1 + clear, then adopt).
  - 00612f70 CUIMiniMapMarker (exact Ego_r.pdb layout), 00a45990 {CCharString, long, 12-byte POD}, 009e6600
    pair<CCharString,CSymbolInfo>, 004b84f0 pair<CCharString,CPersistInfo>, 004aa580 / 006237c0 (0x1c flat
    {2 strings, long, 4 strings}), 00566780 (16-byte flat {3 strings, 2 flags}). Lesson: when retail addresses every
    member as [esi+N] the record is FLAT. A nested `second` holding strings + a word adds an induction pair, as in
    009e6600, where retail has one.
  - 00a85870: three different out-of-line vector `operator=`s plus a flag at +0x25. The never-copied byte at +0x24 is
    an empty struct member.
  - 008e34c0 `__copy<CIcicleData*>`: two `SetPItem(GetPItem())` assignments, the timers, and a C3DVector block. The
    identity comes from the landed caller 0x008D9650 and Ego_r.pdb.
  - 00af6ed0 pair<EPSCType, CComponentInstance> and 00828cd0 pair<long, CCountedPointer>. Both use the landed
    CCountedPointer `operator=` (0x0084F130 / 0x00AF6E10 models). The CComponentInstance vtable word is not copied.
- **SGI counted `__copy_backward`**, 1 row: 0080dff0 over 12-byte pair<CIntelligentPointer<CThing>, int>.
- **SGI/STLport `__insertion_sort(first, last, comp)`** (`ret 4`, 4-byte functor), 8 rows: 0076aec0, 00a340f0, 0086c830,
  0081c490, 004d1530, 008183e0, 00721360, 006bf2c0. STLport passes `*i` BY VALUE to the out-of-line
  `__linear_insert(first, i, val, comp)` (`push comp ; sub esp,N ; mov eax,esp` + the inlined copy ctor). Shape
  lessons:
  - STLport `pair`'s user copy ctor (`first(p.first), second(p.second)`) gives "key word, then
    `add eax,4 ; mov edx,edi` block copy of the POD second" (0086c830 / 0081c490 / 004d1530). The implicit pair copy
    ctor would copy one block instead.
  - A nested user-copy record gives the `&record + 8` induction register (0076aec0 / 00a340f0 / 008183e0).
  - The landed CRGBColour copy ctor `Set(other.R, other.G, other.B, other.A)` gives 00721360's scrambled byte
    shuffle with the bl/cl/dl + frame-spill byte.
- **SGI `for_each`**, 2 rows: 00a72ee0 and 00a72f40, over CPhysicsMesh helper points (20 bytes) and dummy objects (56
  bytes). They use the landed callers' `CAddName` functor (0x00A6D050 / 0x00A6D090) whose `operator()` is the inlined
  STLport `push_back` (placement copy or `_M_insert_overflow(finish, x, __false_type(), 1, true)` 0x00433530). The
  functor result is returned through the hidden pointer in ecx.

## Parked for Fable 5.1 (0)

None.

## Skipped

None. Every row is a single function at its scanner-verified extent: the trailing bytes are int3 padding only,
and the one in-body `nop` at 0x00612F9F is loop alignment.

## Label corrections

All catalog names are propagated `_Move<...>` labels. Corrected identities:

- 00841de0 `_Move<CStaticMapGenerationVisibiilityInfo::CMapDesc*>` -> `__copy<CRetailString*>`. CMapDesc is 16
  bytes (a set<long>); the body strides 12 through basic_string::_M_assign_dispatch.
- 00776b60, 007770c0, 009f6c00, 00a95a10, 00b31a30, 00b28ed0 `_Move<CTCCameraPointScriptedSpline::CKeyCamera*>` ->
  `__uninitialized_copy` of 0x24 / 0x24 / 0x30 / 0x20 / 0x20 / 0x38-byte records. 00b28ed0 has CKeyCamera's 0x38
  size and an InsertKeyCamera caller, but the Ego_r.pdb layout disagrees.
- 00a2af20 `_Move<CKeyCamera*>` -> `__uninitialized_copy<C3DAnimationInfo*>` (RTTI vtables).
- 00af1ce0 `_Move<CTCHeroAttachableAppearanceModifiers::CModifierGraphic*>` ->
  `__copy<NParticleEngine::CParticleEmitterInfo*>` (landed caller 0x00AF15B0). 00708c20 keeps its element and
  becomes `__copy`.
- 004d1530, 00721360, 0076aec0, 008183e0, 0081c490, 0086c830, 00a340f0 `_Move<CTCInventoryMap::CUIMiniMapMarker*>` and
  006bf2c0 `_Move<pair<CCharString,CSymbolInfo>*>` -> `__insertion_sort`. The landed sort / __final_insertion_sort
  callers already name them.
- 00a45990 `_Move<CUIMiniMapMarker*>` -> `__copy` of a 20-byte {CCharString, long, 12-byte} record.
- 00bf5b50 `_Move<CUIMiniMapMarker*>` -> `__uninitialized_copy` of a 28-byte plain record. The real marker has a
  CCharString at +4.
- 00612f70 (CUIMiniMapMarker), 009ca660 / 00a438e0 (CDataBankTextEntry), 009e6600 (pair<CCharString,CSymbolInfo>),
  004b84f0 (pair<CCharString,CPersistInfo>), 00af6ed0, 00828cd0: the element is right, and `_Move` -> `__copy`.
- 0081a650, 008247e0, 00b3e340, 0085db50 `_Move<NThingMap::CMapwhoLevelInfo*>` -> `__uninitialized_copy` of 20-byte
  records. The element stays unproven.
- 00a72ee0 and 00a72f40 `_Move<_List_const_iterator<...CAnimSet...>*, _Temp_iterator<...>>` -> `for_each<const
  SHelperPoint*, CAddName>` and `for_each<const SDummyObject*, CAddName>`.
- 009e6270 `_Move<pair<CCharString,CSymbolInfo>*>` -> `__uninitialized_copy` (the element is right).
- 004aa580, 006237c0, 00566780, 00a85870 `_Move<pair<CCharString,CPersistInfo>*>` -> `__copy` of other records
  (0x1c / 0x1c / 16 / 0x28 bytes).
- 0080dff0 `_Move<pair<CIntelligentPointer<CThing const>,NStealth::CDangerStatus>*>` ->
  `__copy_backward<pair<CIntelligentPointer<CThing>, int>*>`. The caller is labelled make_pair<CIntelligentPointer<CThing>,int>.
- 008e34c0 `_Move<pair<CIntelligentPointer<CThing const>,CDangerStatus>*>` -> `__copy<CIcicleData*>`.
- Callee labels:
  - The `__linear_insert` bodies carry unrelated labels: 0x004D1000 (CJoystick::EventScanStart), 0x00720CB0,
    0x0081BDA0 and 0x00A33780 (CDateAndTime ctor), 0x0076AAE0 and 0x0086C620 (CCompressorZlib::SetupStream),
    0x00818240 (std_copy_BackwardLarge), 0x006BF020 (Vector_InsertWideStringPair).
  - 0x00433530 "std::vector::Reallocate" is vector<CCharString>::_M_insert_overflow.
  - 0x009F66A0 "Array_CopyConstruct" is a 12-byte array copy ctor (fine).
