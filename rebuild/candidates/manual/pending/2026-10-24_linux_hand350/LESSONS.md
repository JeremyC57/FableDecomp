# Chunk 350 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A `__copy_backward` with `ret 8` (result + ONE reference argument), `mov eax,[esp+4]` as its first instruction and the
  source formed as result + (last - result) (`lea esi,[edx+eax]` / `[ecx+eax]`) is STLport `__copy_backward_ptrs(first,
  last, result, const __false_type&)` with the SGI `__copy_backward(.., random_access_iterator_tag(), (ptrdiff_t*)0)`
  inlined into it; an out-of-line 4/5-argument loop keeps two pointer registers (0x00A95A70, 0x0047130E, 0x004ED2F1).
- In a size-optimised (`optimize("s")`, odd-address, `push N ; pop ecx`) TU VC7.1 inlines neither a plain `inline` helper
  nor an element's implicit operator=: make the inner loop and the element's memberwise operator= `__forceinline`; an
  ebp frame with `leave` adds `#pragma optimize("y", off)` (0x004ED2F1, 0x0047130E).
- An element copy that skips the word at +4 (copies +0 and +8..) is a pair whose second is polymorphic (its vtable is
  never assigned); check the PDB for a vtable `_padding_` at +0 (0x00769EA0 CParticleUpdateParamDesc, 0x00AF6E10
  CComponentInstance, 0x00A5E830).
- Three words copied by plain offsets are separate scalar members; a C3DVector member (user copy ctor) copies through
  pointer registers (`mov eax,esi ; mov ecx,edi`) (0x00769B90 vs 0x00722C90).
- A byte followed by a C3DVector at +1 (`lea ecx,[esi+1]`) is a `#pragma pack(1)` block {bool, C3DVector} with a
  memberwise user operator=; its implicit operator= block-copies 13 bytes as 3 dwords + 1 byte (0x006F0640).
- An output iterator returned through a hidden pointer (__fastcall: hidden pointer in ecx, first in edx, `ret 8`) needs a
  user constructor (back_insert_iterator's `explicit (C&)`); write the copy loop in the out-of-line function -- an inlined
  `__copy(.., input_iterator_tag, Distance*)` wrapper adds a self-copy of the by-value iterator (0x008ED4D0).
- `_Copy_backward` labels on `ret 0Ch`/`ret 3Ch` bodies that load `[esp+8]` as a key and loop `while (comp(val, *next))`
  are SGI `__unguarded_linear_insert(last, T val, Compare comp)`; with a by-value intrusive pointer the callee destroys
  `val` at the end (0x005E07F0, 0x008074A0).
- STLport `__uninitialized_copy(first, last, result, const __false_type&)` is a pointer-compare loop with the placement
  new null test (`test ebp,ebp ; je`) and STLport pair's user copy constructor (0x00CB60F0).

Review at landing:
- 0x00A95A70 took 6 grades (default, /Oa, /Ow, tag by reference, /Os, then the __copy_backward_ptrs wrapper) and
  0x0047130E took 5 (the wrapper needed /Os + __forceinline) -- both over the 4-grade budget; the final forms are the
  family lesson above.
- 0x00564D5D is a catalog row that starts mid-function (real start 0x00564D50, 42 bytes); fix the catalog bounds.
- All element types are local models: no engine headers exist for C3DMeshVolume, CScriptInfo, CRegisteredMusicEntry,
  CUIMiniMapMarker, CGenerationPos, CComponentInstance, CParticleEmitterInfo or CDataBankTextEntry (checked
  rebuild/include/engine).
- 0x00CB60F0 includes `<new>` for placement new (the landed 0x007BB660 declares its own operator new instead).
- 0x006C1ED0 (parked) is the only row with a next idea left; see NOTES.md.
