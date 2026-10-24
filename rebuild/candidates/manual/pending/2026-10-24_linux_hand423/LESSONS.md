# Chunk 423 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Address exposure through declaration-only callees.** A local, by-value parameter or pair temporary whose address
  goes to an out-of-line callee that is only *declared* is treated by VC7.1 as address-exposed for the whole function.
  The effects are:
  - words re-read from the frame instead of kept in registers;
  - dead proxy zero stores that stay in;
  - an STLport tag that gets its own frame byte, or lands in the wrong slot;
  - a count block re-read after `dec`.

  The fix is a same-TU `#pragma auto_inline(off)` + `__declspec(noinline)` body for that callee. Where the body sits
  (before or after the caller) does not matter. Proven on:
  - 00a837e0 (`_M_insert_overflow`)
  - 00aa65a0 (the tag's `push ecx` frame byte)
  - 00a843f0 (pair copy constructor + `list::operator=`)
  - 00a84090 (`__destroy_aux` tag in the live `first` slot)
  - 00a85240 (~list + `_M_fill_insert`)
  - 00a9b930 (`AddChunkType`'s hidden-return temp)
- **`__forceinline __copy`.** When the copy loop calls an out-of-line element `operator=` (list<C2DLineF>), VC7.1
  emits `__copy` as its own cdecl call. `__forceinline` restores retail's inline loop (00a84090
  `vector<list>::erase`).
- **C3DVector pointer pattern.** A pack(1) C3DVector plus a component-wise copy (`C3DVector p(m.X, m.Y, m.Z)`) gives
  retail's `lea ebp,[esi+8]; mov edx,ebp` member-pointer shape. A plain `= m_position` copy does not (00aabbb0
  CSoundEmitterLH slot 15).
- **CVectorMap::DeallocateMemory.** The shape is `clear(); vector empty; swap(empty); Dirty = false;`. Retail
  `vector::swap` is three `_STL::swap` calls, one of them on the whole `_STLP_alloc_proxy`; `__copy` and
  `__destroy_aux` stay out of line (00a83fd0).
- **vector(n) for PODs.** `vector<T>(n)` for T = unsigned short or unsigned long uses the real `_Vector_base(n, a)` +
  `uninitialized_fill_n(start, n, T())` with `__true_type`, which becomes `fill_n` and compiles to `rep stos`.
  For unsigned short it is `rep stosd` + `rep stosw` (00a8d750, 00a96100). The copy constructor is
  `__copy_trivial` / memmove (00a87090).
- **vector::resize(n).** It is `resize(n, _Tp())` (erase or insert, then `_M_fill_insert`), and the `_Tp()`
  temporary's destructor runs at both exits (00a85240).
- **AddChunkType and no NRVO.** VC7.1 has no NRVO: `CCountedPointer chunk; chunk.Reset(proto.CreateNewInstance());
  parent->RegisterSubChunkType(chunk); return chunk;` copy-constructs the local into the hidden result. The copy
  shares one null test with the local's destructor (00a9ad00).
- **Identities.**
  - 00aa84f0 / 00aa87e0 are CChunkedFileSuperChunk::AddSubChunk / RegisterSubChunkType. Retail layout: SubChunks
    +0x10, RegisteredSubChunkTypes +0x1C, CurrentChunkIndex +0x28, size 0x2C.
  - 00aa87e0 is ICF-shared (GUI var-transfer Add helpers, C3DMeshFileXLoader constructor).
  - C3DMeshFileXUVTangentChunk: UTangents +0x10, VTangents +0x1C.
  - C3DMeshFileXAnimatedPrimitiveBlockChunk: BonesPerVertex +0x44, VertexList +0x48, BoneWeights +0x54.
  - CNavQuadTree::CNavigationLayer is 0x28 bytes. CWall is 24 bytes.
- **CCountedPointer release.** On a non-exposed register value, the CCountedPointer release folds to `dec [esi];
  jne`. A re-read of the count block after the decrement means the pointer's address escaped (see the first lesson).

Review at landing:
- Landed 0x00A9BA10 / 0x00A9BA40 (CreatePhysicsFacesSegment / ...FromPhysicsChunks, "MakeSegment") are
  C3DMeshFileLoader::RegisterMapSubChunkType (+0x88) and RegisterHelpersSubChunkType (+0x98). The callers' landed
  "C3DMeshFileRootChunk" naming of AddChunkType's parent is also wrong.
- The landed CSBCCorridor::Reset 0x00A88080 models CWall as 16 bytes; it is 24 (Ego_r, and 00a88b10 copies 6 dwords).
- The landed 0x0056D490 and std_fill 0x00A85810 model STLport in namespace `std`, not `_STL`, so their mangled names
  differ from retail.
- The landed `CVectorMap_T_00a84ff0_Sort` models a TransferToGui call. Re-check what that function is.
- The engine headers `rebuild/include/engine/CChunkedFileSuperChunk.h` and `C3DMeshFileXUVTangentChunk.h` carry Ego_r
  16-byte debug vectors. Retail uses 12-byte vectors (see the offsets above), so these rows use local models next to
  the existing headers.
- 00a83770 and 00a852b0 (parked, 4 grades each): try the now-proven same-TU noinline stand-ins first:
  - 00a83770: `_M_insert_overflow` 0x00A82380.
  - 00a852b0: pair copy constructor 0x00A825D0, `_M_insert_overflow` 0x00A84C20, ~list.
- Rows that needed 4 grades before matching: 00a83570, 00a83cc0.
