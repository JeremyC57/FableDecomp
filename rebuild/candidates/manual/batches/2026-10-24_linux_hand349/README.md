# Hand chunk 349 (_Cons_val<pair<CCharString,CArray>> .. _Copy_backward<CSymbolMap::CSymbolInfo*>): 38 of 40 byte-exact

## What matched (38)
- **STLport `_Construct(T1* p, const T2& v)` (`new (p) T(v)`), /Gr __fastcall, 15 rows** -- 00d34ec0, 008eba50, 009aa520, 007aca30,
  004e9c39, 0071f4e0, 004cc3c0, 006a30d0, 007ca600, 00748a00, 00a60950, 00769270, 0047c250 (+ the 00a6ee90/005b1b20 parked
  ones). VC null-tests the placement result itself (`test esi,esi ; je`); the implicit copy constructor of the element
  inlines. Shape levers:
  - CIntelligentPointer members: the landed 0x008C7D80 copy-constructor model (base default ctor out of line, vtable store,
    `SetPItem(other.GetPItem())`) matches unchanged.
  - A 28/32-byte POD tail is one `rep movsd` through bumped pointers; a std::pair<long, CDateAndTime> nested value needs a
    user memberwise copy ctor (0071f4e0), otherwise the long and the block merge into one 8-dword move.
  - 008eba50 / 0047c250: retail copies the value's members at fixed offsets from the ELEMENT pointers (or a POD block
    through `lea` pointers, then trailing words off the element pointers). Nesting the value as a struct re-bases the
    pointers in the lane; modelling the value's members flat in the element matches.
  - Def classes (00d34ec0, 004e9c39): CDefClassBase as a 0x25-byte byte-packed polymorphic base with its out-of-line copy
    ctor (landed 0x006250B0 model), derived members 4-aligned from +0x28. 004e9c39 is a size TU (`push 7 ; pop ecx`): under
    "s" the implicit copy ctor is emitted out of line, so the memberwise copy ctor is spelled out `__forceinline`.
- **pair<const unsigned __int64, CGroupMemberData>** -- 00812840 / 008132d0 (thiscall `ret 4`, returns this): the STLport
  converting constructor `pair(const pair<U1,U2>&)` with CGroupMemberData's implicit copy inlined through re-based
  pointers; 00813280: `make_pair` (cdecl, hidden result slot) built with `return pair(a, b);`.
- **SGI `__unguarded_linear_insert(last, value, comp)`** -- 004cfd40 (28-byte float-keyed records, less; reuses the exact
  types the landed caller 0x004D1000 declares), 0067e550 (CDegradableInfo with CDegradableDef::CIsHealth1GTHealth2:
  `test ah,41h ; jne` = greater), 004cfcb0 (8-byte pairs, no saved registers). Plain SGI loop, comparator inlined. MATCH.
- **SGI `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, 14 rows** -- the landed 0x00517430
  family; out-of-line element operator= (00a74d90, 006b6250, 0075f0e0, 008898c0, 00ad7120) or an implicit memberwise
  operator= whose word/byte copies the compiler schedules around `dec ebx` (00526fe0, 00a821c0, 0086d5e0, 0051c710,
  00ad6360, 0052b640, 00623390, 00a37a80, 00a3fe60). 0086d5e0's element holds a CVectorMap (vector + empty comparator +
  Dirty byte at +0x15: the comparator byte copies nothing). The magic-number division and the esi/edi role swap for
  magics >= 2^31 come out of the same source.
- **SGI `__copy` (forward)** -- 00ad6ff0: `*result = *first; ++first; ++result;` (first stepped before result).
- **Small members** -- 005bd328 CTCInventoryBase::GetNumberOfSlotsInCategory (size TU; a named `first` local loads the
  vector pointer before the index); 00643a8e `(long)(value * m_scale)` on an unsigned char (size TU ebp frame from the
  int->float widening in the arg slot); 00c1ba20 EncodeFixed16 (`value * 65535 * 0.0001` with the first product in a
  named local -- the one-expression form folds the two literals, also under /Op); 0098f350 CThreadedFile::AddObject
  forwarding to g_ThreadedFileManager.AddObject(desc, this).

## Parked for Fable 5.1 (2)
- 005b1b20 _global::Construct_PairSkeletalMorphEntryFloat DIFFER(21v21): third word moves through edx (`mov edx,[edx+8] ; mov [ecx+8],edx`) where retail reuses eax | tried: POD pair, CEntry user copy ctor, pair user copy ctor, both | next: lane gap: register choice (likely compiler build), or a different source element type
- 00a6ee90 _global::UninitFillN_CharStringRecord38 DIFFER(73v68): lane strength-reduces `&first->m_data` into ebp (`lea ebp,[ebx+4]` before the loop, `add ebp,38h`) and reloads the value pointer each iteration; retail holds the value pointer in ebp and forms `lea esi,[ebp+4] ; lea edi,[ebx+4]` per iteration | tried: plain loop, inline _Construct helper, value-pointer local, array tail instead of a POD struct | next: a `_Construct` taking the tail by a different path (e.g. element assignment through a non-inline helper), or /O1 vs /O2 sweep at landing

## Skipped
- none (no row holds two functions; all bounds check out).

## Label correction
- 00d34ec0: not pair<CCharString,CArray<unsigned long>> -- a CDefClassBase-derived def (vtable 0x012CCC50) with a vector<unsigned char> at +0x28; its callee 0x00D34F00 ("Map_InsertOrUpdatePair") is that vector's copy ctor.
- 004e9c39: not pair<CCharString,pair<long,CDateAndTime>> -- `_Construct<CShopItemDef>` (RTTI vtable 0x0123B644).
- 00a6ee90: not a _Cons_val -- `_Uninit_fill_n` of 0x38-byte CCharString (0x0099EC30, not CWideString) records.
- 005bd328: CTCInventoryBase::GetNumberOfSlotsInCategory(long) (calls CCategory::GetNumberOfSlots 0x005BD1CF).
- 00643a8e: an unreferenced float-scale helper of an unrecovered class (float +0x2A0).
- 00c1ba20: EncodeFixed16(float), the 16-bit fixed-point encoder of the sound-parameter record (callers 0x00C1BB80/0x00C1BBA0).
- 00813280: std::make_pair<unsigned __int64, CGroupMemberData> (cdecl, hidden result), not a _Cons_val.
- 00812840 / 008132d0: pair<const u64, CGroupMemberData> converting constructors (thiscall, return this).
- 0098f350: CThreadedFile::AddObject(const CThreadedFileObjectDesc&) (forwarder to CThreadedFileManager::AddObject 0x0098F0F0).
- 004cfd40 / 0067e550 / 004cfcb0: SGI __unguarded_linear_insert, not _Copy_backward.
- 00ad6ff0: SGI __copy (forward), not _Copy_backward.
- All other _Copy_backward rows: the element types are not CSeaBodyDesc / CSymbolInfo / CEntry (propagated); modelled by size and callees.
