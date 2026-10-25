# Chunk 330 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport resize, zero stores after the size computation (0x007B5310, 0x006C9DE0).** In a TransferableVector
  Resize, the default temporary's zero stores come after `sar edi,3` and TransVector stays in ecx. That is
  `resize(n) { resize(n, T()); }` with BOTH overloads `__forceinline`. A named default local, or a direct
  `resize(count, T())`, puts the stores first. A plain inline leaves resize(n, x) out of line (8-byte jmp body).
- **Two members read in one body (0x004671CA, 0x007B42D0, 0x00755D00).** Retail homes both values in the same dead
  stream-argument slot. To get that, read each through an inline `CPersistTraits<long>::TransferBinaryIn(stream,
  &member) { long n; Read(&n, 4); *member = n; }`. Two block locals in the body add a `push ecx` frame word, and one
  reused local swaps eax/ecx in the first member store.
- **CMemoryDataInputStream::Read position update.** A module whose Read stores `position = (long)newPosition` gives
  `add eax,4 ; xor ecx,ecx ... mov [ebx+4],eax`. The `position += size` form gives `lea ecx,[eax+4]`, so pick the form
  per module (0x004671CA uses the first, 0x0046B0D6 next to it uses the second).
- **Default value whose temporary outlives the member stores (0x004670F5).** If a traits constructor's "" CCharString
  temporary is destroyed after the member zero stores, it is the mem-initializer argument `DefaultVal("")`; the
  temporary lives to the end of that full expression. In a size TU, the element constructors it inlines must be
  `__forceinline`.
- **By-value iterator results (0x00661287).** A by-value iterator / inserter result of a __fastcall STL helper goes
  through the hidden pointer (ecx) only if the class has a user constructor (STLport's
  `explicit back_insert_iterator(C&)`). As an aggregate it comes back in eax.
- **Memberwise argument copy (0x00A33800, 0x00758E00, 0x0081BE30).** In __linear_insert, retail copies `val` into
  the argument slot memberwise: `mov [eax],ecx` (the key loaded for the compare), then `rep movsd` of the rest. That
  needs pair's user copy constructor `first(p.first), second(p.second)`. The implicit one block-copies everything and
  reloads the key in eax. `*first = val` stays the implicit block operator=.
- **Store before the vptr in a constructor (0x00C70120 / 0x00C70B00).** An aligned-buffer pointer store before the
  derived vptr store is a second, non-polymorphic base with an inline constructor (confirms wave 24o). Members that a
  derived constructor zeroes with `memset` and then points into are addressed off `this` (esi). Moving that code into
  the member's own constructor addresses it off edx/ebx.
- **Void setter versus constructor (0x009D5060).** A thiscall callee storing into `[esi]` with no `mov eax,esi` is a
  void setter (CDefString::SetString), not a constructor. Writing `*value = CDefString(text)` builds a temporary and
  copies it; `value->SetString(text)` matches.
- **Element type from the stride.** Most propagated `CPersistContext_TransferableVector<long>` rows are other
  element types. The stride in the TransferOut/In body (`imul 1Ch`, `lea [eax+eax*4] ; lea [eax+edx*8]` = 40,
  `lea [eax+eax*2] ; lea [ecx+eax*8]` = 24) plus Ego_r.pdb sizes names them (CActionInputControl, CUIRegionInfo,
  CHeroMarriageInfo).

Review at landing:
- Local models next to an existing engine header:
  - 0x005C0E41 / 0x005C20D8 use a local NInventory::CItem. engine/CItem.h flattens the CCountedPointer into two raw
    words, so it cannot carry the counted pointer's out-of-line destructor 0x005C04BF. Offsets agree with the header.
    The verifier will warn SHARED_HEADER_AVAILABLE CItem.
  - 0x0083EB90 uses a local LHHttp2. engine/LHHttp2.h is the Ego_r.pdb layout (0x1C-byte strings) and does not match
    retail (CurrentHeaderPart +0x70 v +0xB8). The verifier will warn SHARED_HEADER_AVAILABLE LHHttp2.
- CMemoryDataInputStream and CStringParser are used through derived method views (`struct CMemoryDataInputStreamReader
  : CMemoryDataInputStream`, `struct CStringParserReader : CStringParser`) over the engine headers, as in the landed
  0x007565E0.
- `__forceinline` is used in 0x007B5310, 0x006C9DE0 and 0x006D4AA0 (STLport resize) and in 0x004670F5 (element
  constructors in a size TU).
- Errors in already-landed sources:
  - rebuild/src/compiled/00/5c/unattributed_ClearEntry_005c2a5c.cpp is CPersistContext_TransferableVector<
    NInventory::CItem>::Resize (vtable 0x01253CA4 slot 1), not a "CSoundPairTable::ClearEntry". Its 0x14-byte
    {id, CSoundPair} model is really CItem.
  - CRegion_SetMiniMapRegionExitTextOffsetX_009d5060.cpp is CDefString::SetString(const CCharString&).
  - global_ConstructString_0066124b.cpp is std::__copy_aux(first, last, result, const __false_type&) (__fastcall,
    hidden result), not a CWideString constructor.
  - CPersistContextTransferableVectorSPersistRecord1C_Resize_006193b0.cpp: the vtable 0x01257164 element is
    CTCInventoryMap::CUIMiniMapMarker (long Region, CCharString, C2DVector, long, long, float Angle), not
    {float, CCharString, 5 floats}. The bytes match because every member is zeroed.
- Catalog rows that are not functions: 0x007FAE34 and 0x0080BFEC are the `depth_limit == 0` arms of the
  __introsort_loop bodies at 0x007FAC80 (468 bytes) and 0x0080BD30 (738 bytes).
- No row needed more than 4 grades. 0x0057F70F is parked after 4.
