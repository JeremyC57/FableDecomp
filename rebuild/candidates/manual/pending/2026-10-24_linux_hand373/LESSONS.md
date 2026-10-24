# Chunk 373 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport `vector::operator=` (0x004E9AD3 / 0x004EBCA9 / 0x004EBF32)**
  - Write the 4.x text verbatim with `_TrivialAss()` as a POD empty `__false_type`. That gives the single
    `xor eax,eax ; stosb` into the dead x slot.
  - In `__destroy`, the tag must be a NAMED uninitialised local (`_Trivial_destructor tag; __destroy_aux(f, l, tag);`). A
    second `_Trivial_destructor()` adds a second `stosb`.
  - Under "s", `_Destroy`, `__destroy` and `__uninitialized_copy` must be `__forceinline`, or VC emits them as out-of-line
    `__cdecl` calls.
- **One source, two shapes (0x004EBF32).** The same operator= source at a 0x44 stride instead of 0x48 changes the register
  choice: x._M_start is spilled, xlen goes in a frame word, and the frame is `sub esp,0Ch` instead of `push ecx ; push ecx`.
  Both match from one text, so do not chase that difference in source.
- **`__copy_ptrs` / `__copy_backward_ptrs` over def elements (0x004EBDB0, 0x004EAF33, 0x004EDCC2)**
  - The out-of-line `/Gr` `__fastcall` template takes first, last, result and a `const __false_type&`, with `__copy` /
    `__copy_backward(..., random_access_iterator_tag(), (int*)0)` `__forceinline`.
  - The element's implicit operator= is spelled out and `__forceinline`.
  - VC7.1 copies the first float member with `mov` and the second with `fld/fstp` on its own (0x004EAF33). Keep both
    members `float` as the PDB says.
- **STLport vector copy constructor in the def TUs (0x004EBE1A / 0x004EC4B0)**
  - `_Vector_base(size_t, const allocator&)` is out of line. The allocator temporary needs a user default and copy ctor
    (no store), and it is homed at `[ebp+0Bh]`.
  - `__uninitialized_copy` is `__forceinline`.
  - Use `optimize("y", off)` + `("s", on)`.
- **`uninitialized_fill_n` (0x004EA57F)**
  - It is the 3-argument STLport entry (`ret 4`, no tag on the stack) with `__uninitialized_fill_n(..., _Is_POD())`
    inlined.
  - Leaf `??$uninitialized_fill_n`. The catalog's `_Fill<T*,T>` is the propagated Dinkumware spelling.
- **`vector::erase(first, last)` of a non-POD element with a trivial dtor (0x004EA40B)**
  - `__copy_ptrs` → `__copy` → the implicit 14-byte assignment (`movsd x3 ; movsw`) is all inlined.
  - The `__destroy_aux` loop over an empty `~T()` vanishes.
  - It is MATCH with no relocations.
- **GFSerialiseVectorBinaryIn (0x004E9D71 vs 0x004EB219)**
  - When retail calls `resize` with ONE argument (`push count ; call`), declare the one-argument resize out of line. That
    callee builds the `T()` itself.
  - When retail builds the default element inline and calls `resize(n, x)`, make the one-argument resize `__forceinline`.
- **GFSerialise with empty traits whose element writes itself (0x004EB1A8 / 0x004EB219, CExplosionRing)**
  - Use an empty traits specialization with a `__forceinline` TransferBinaryOut/In that forwards to the element's member
    (`ecx = &v[i] ; push stream`).
  - Traits that carry a DefaultVal show their default stores in the prologue (0x004E9CE8 CMusicEntry: `or [..],-1`,
    `fldz/fstp`, the zero byte).
- **GFSerialiseMapBinaryOut (0x004E910F)**: a value traits whose TransferBinaryOut makes two WriteSLONG calls must be
  `__forceinline` under "s". Otherwise VC calls it out of line through a `push ecx` frame word.
- **GFSerialiseMapBinaryIn (0x004E5A76)**: if the count read is inline but the value's long traits are out of line
  (0x0040FE60), combine an inline `ReadCount` helper with the same-TU noinline stand-in for `CPersistTraits<long>`.
  VC7.1 then homes the empty traits on the map spill word (`lea ecx,[ebp-0Ch]`), as retail does.
- **Test lane (g++ x64)**: never give a packed-record test model an `unsigned long` member when it checks `sizeof`, because
  it is 8 bytes on Linux. Use `unsigned int` (the 004EA57F test failed on this at first).

Review at landing:
- **Landed 0x004E59E1 (`CFlammableDef_TransferHelper_004e59e1.cpp`) is misdescribed.**
  - The body reads `[esi+18h]` Mode, `[esi+24h]` / `[esi+28h]` streams, and passes `ecx = stream` and `edx = value + 8`.
    So it is the CPersistContext::Transfer<T> template (cf. 0x0045C8D3 and 0x004EA21E), not a CFlammableDef member with
    "map in ecx, context+8 in edx".
  - Its 20-byte "scope" is the CPersistTraits<T> DefaultVal.
  - Its callees are GFSerialiseMapBinaryIn<long, CDefString, CCompareDefString<long>> 0x004E5A76 (stream ecx, map edx) and
    GFSerialiseMapBinaryOut 0x004E5A39 (not "WriteAllocatedAreasData").
  - The bytes match; the names and comments need a fix.
- **Local base models next to an existing engine header.**
  - CDefClassBase (0x004EAF33, 0x004EDCC2) and CParentDefClassBase (0x004EBCA9, 0x004EBDB0, 0x004EBF32, 0x004EC10F) are
    local polymorphic models, as in the landed 0x004E9BDA / 0x004EBDF1. `engine/CDefClassBase.h` and
    `engine/CParentDefClassBase.h` flatten the vptr into a packed byte body, which cannot express a base operator= or a
    virtual class.
  - CShopItemDef uses its engine header (0x004E9AD3, 0x004ED353).
- **Rows with no engine header** for the element (local models from Ego_r.pdb and retail strides): CCardDef (retail 0x44,
  PDB 72), CCardPositionsDef (retail 0x48, PDB 80), CShoveHaPennyRegionDef, CPrizeScoreDef, CMusicEntry, CExplosionRing,
  CObjectAugmentationParticleSet, CEngineGraphic.
- **Grade counts.** No row needed more than 4 grades. 004dfcea used all 4 and is parked with a concrete next idea
  (auto_inline(off) body of 0x00429950). 004ea21e used 4 and 004d1e20 used 3; both are scheduling-only.
- **Rows holding several functions:** none in this chunk.
