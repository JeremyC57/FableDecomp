# Chunk 462 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `vector::resize(n)` with out-of-line erase and `_M_fill_insert` is the plain `resize(n) { resize(n, _Tp()); }` over the inline two-argument resize. Seven rows matched first try with no `__forceinline` (0x0049B7F0, 0x006253D0, 0x007B9EC0, 0x009C7850, 0x009CB7D0, 0x00AE7D10, 0x00BF97A0). The element's real default ctor decides the temporary's stores:
  - an implicit ctor of `{long, CCharString}` builds only the string (CTextTag, CPaletteEntry, CQuestCardItem);
  - STLport `pair()` zeroes `second` after the compare;
  - CIntelligentPointer's derived vtable store is scheduled after the compare.
- Explicit instantiation of `vector<T>::resize(unsigned int)` also emits the inline two-argument `resize(n, x)` as its own COMDAT, and `?resize@?$vector@` then matches two symbols. Use the full `?resize@?$vector@...@_STL@@QAEXI@Z` leaf (0x0049B7F0).
- The catalog label "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize" covers every 8-byte-element resize(n), across seven element types. Identify each from the caller's class layout plus the `?resize@?$vector@V<elem>` PDB name sets.
- CArray<T> (Lionhead) has no resize in the PDB name sets: `CArray<T>::Resize` callers really call the inherited `vector<T>::resize` (0x009C7850 from CFileInstaller::InitBuffers).
- The byte-vector resize (0x004122D0) is STLport `__copy_trivial` with `__last == __first` both `_M_finish`. VC7.1 keeps the unfolded `cmp eax,eax ; jne` and `mov esi,eax ; sub esi,eax` from the plain template. memmove is a plain `extern "C"` declaration: the call is rel32 to the 0x00BFEAE6 thunk, not `call [__imp_]`.
- In /Os TUs a pair swap keeps the pair copy ctor and operator= out of line (0x004DA6FC calls 0x004D788D / 0x004DA56A). In /O2 TUs they inline into member calls.
- An /Os `swap<pair<long, CDefPointer<T>>>` needs `__forceinline ~CDefPointer()`, so that the pair's out-of-line `~pair` is called on the temporary. A plain inline dtor inlines `~pair` and calls the def-pointer release at ebp-4 (0x005938F1).
- Assigning an intelligent pointer built from a raw pointer needs a NAMED local (`CIntelligentPointer<CThing> p(thing); member = p;`). An unnamed temporary pushes the ctor's returned eax instead of retail's `lea eax,[ebp-8] ; push eax` (0x005BD183, 40v43).
- STLport `iter_swap` is a bare `jmp` to `swap` (0x00CB63F0 -> 0x00CB6230).
- In `vector::erase(pos)` the `__false_type` tag goes by reference into the dead argument slot. `__copy` is declared out of line (0x008E3890).
- In `__unguarded_insertion_sort_aux` the comparator stays declaration-only and the linear insert stays out of line (0x006A32B0, 0x005E1AC0, 0x006BEB80). A `__fastcall` function-pointer comparator with an inline linear insert gives a full MATCH (0x009323D0).

Review at landing:
- No row needed more than 4 grades. The maximum was 3: 0x007B9EC0 and 0x00AE7D10, each with one SRC_FAIL from `>>` in the explicit instantiation.
- Local models next to existing engine headers:
  - engine/CCategory.h (0x005BD183: the header has the Ego_r 16-byte vector layout with the pointer as opaque bytes);
  - engine/CMultiStaticMeshEntryDef.h (0x004DA655: the header models the vptr as data);
  - engine/CTexture.h (0x0049B7F0: the header flattens the ByteLength:28 / AllocationSource:4 bitfields into byte arrays);
  - engine/CContextSensitiveItemDef.h exists, but 0x005938F1 / 0x005946B2 only forward-declare it (pointer use).
- 0x00CD2725's element is a descriptive name (CScriptRecord16, 16 bytes, polymorphic). The TU's element copy 0x00CD2AE2 runs CScriptThing's copy ctor 0x004ABE90 and copies a byte at +0xC, so the real element is probably a CScriptThing-derived class plus a bool. Check the PDB for its name.
- Errors in ALREADY-LANDED sources:
  - 0x00CB67B0 declares iter_swap at 0x00CB6230, but 0x00CB6230 is swap and iter_swap is 0x00CB63F0.
  - The landed 0x00CD2AE2 "CTavernTableDef::operator=" is really the copy of a {CScriptThing, bool} record (it calls CScriptThing's copy 0x004ABE90).
  - The landed CFileInstaller::InitBuffers 0x009C5310 names its callee 0x009C7850 "CArray<CBufferChunk>::Resize(unsigned long)"; it is the inherited `vector<CFileInstaller::CBufferChunk>::resize(unsigned int)`.
  - The landed 0x00A744B0 is named Copy_VectorWord (C3DVectorWord), but its caller 0x00A97A50 is vector<C3DTriangle2>::resize (the landed C3DMeshFileTrianglesChunk::ReadFromFile agrees). The 6-byte POD `__copy` bodies are byte twins, so the label is propagated.
  - The landed 0x004DA636, 0x004129F0 and 0x004110D0 use namespace `std` instead of `_STL`. 0x004110D0 cites `.?AVlength_error@std@@` RTTI. This chunk follows the brief's `_STL` rule; pick one convention at landing.
- No catalog row in this chunk holds two functions.
