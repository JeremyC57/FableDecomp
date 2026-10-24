# Chunk 365 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **`T* cur = result;`** in an STLport `__uninitialized_copy` of pair<key, non-POD> (or any element whose copy touches an
  inner block) keeps retail's per-iteration `lea edi,[eax+4]` and loads result before the empty test. Looping on
  `result` strength-reduces into a second induction register and spills `last`.
  Seen at 0086abf0 (78v65 -> 65), 0081a1b0 (90v77), and 00ab4450 / 00a5e7f0 (double epilogue, 70v61).
- **The 3-arg `uninitialized_fill_n(first, n, x)` front end (`ret 4`)** needs a `__forceinline` inner
  `__uninitialized_fill_n(.., __false_type())`. Plain `inline` leaves a cdecl call (00440180 23v49; 007688b0).
- **Unaligned `__uninitialized_fill_n` bodies** (start address not 16-byte aligned) are size-optimised TUs. Use
  `#pragma optimize("s", on)` to get &x reloaded per iteration with no esi push (005b149f, 005f7a1d).
- **The second-induction-register offset in fill_n follows the element nesting.** A flat `{long; CCP a, b, c;}` with
  implicit copy gives +0x10 (retail). pair<long, {CCP,CCP,CCP}> gives +0x18 and {CCP, pair<CCP,CCP>} gives +8.
  Pick the flattest model that keeps the copy order (00513570).
- **STLport list copy-construct inside `__uninitialized_copy`:** `_List_base` zeroes the proxy, mallocs the header node,
  self-links it, then calls the out-of-line `_M_insert_dispatch(begin, x.begin, x.end)`
  (00a82730 / 00a831d0: node 0x18, 0x5152C0; 00865650: node 0x24, 0x76FD40).
- **Dinkumware `_Uninit_copy(first, last, dest, _Alloc&, _Nonscalar_ptr_iterator_tag)`** takes the tag by value in its
  own stack slot (`ret 0Ch`), against STLport's `ret 8` for a `const __false_type&` (00c29340 vs 0082c420).
- **Strides that reveal the pair type:**
  - A 9-byte stride is pair<C2DVector, uchar> with a genuinely packed C2DVector (008002f0).
  - A 16-byte stride copying 12 or 9 bytes is pair<u64, long> or pair<u64, bool>; the u64 forces 8-byte alignment
    (00a7e630 / 00a7e5c0).
- **`__destroy_aux` over vectors** inlines ~vector (element dtor walk + deallocate -> `free` 0xBFEA14), `ret 4`
  (009f6e00, 00a43f80).
- **CVertexShaderInput::CStreamInfo copy** needs `__forceinline` on the member copy ctors (CBaseClass /
  CVertexStreamFormat / CArray 0xA5E0A0) to stay inlined under the uninitialized_copy loop (00ab4450 / 00a5e7f0).
- **Oracle extents run to the end of `ret imm16`.** Hand-counted lengths that stop at the `c2` opcode are 1-2 bytes
  short; take lengths from the oracle row, not from a disassembly eyeball.

Review at landing:
- Landed `rebuild/src/compiled/00/a8/global_uninitializedcopy_00a85420.cpp` models its callee 0x00A82730 as a
  4-byte `SNavCell` int copy. Retail 0x00A82730 copy-constructs an STLport list (this chunk's 00a82730). The stride
  matches and only the frame is affected, but the stand-in model is wrong.
- 008ac180 uses a local CBase / CScriptThing / CCountedPointer model next to `engine/CScriptThing.h`, because that header
  flattens the vptr and declares no ctors.
- 00513570 used 4 grades (the limit), with probes to choose the element nesting.
- 00a43f80 is an ICF-folded body shared by 12 callers. The element type named here
  ({CCharString, long, long} vectors) is one of several byte-identical candidates.
- Twins are kept under distinct address-suffixed names and types: 00a43630 / 009ca4c0; 00a51c60 / 00a52040 / 00a6e350;
  00a5e7f0 / 00ab4450; 00a82730 / 00a831d0; 005b149f / 005f7a1d.
