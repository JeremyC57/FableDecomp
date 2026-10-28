# Chunk 511 lessons

## GOTCHAS one-liners
- STLport _M_insert_overflow: when retail inlines the fill, declare __uninitialized_fill_n __forceinline. A plain inline goes out as a cdecl call (0xA55070).
- pair<CCharString,T> copies differ (a sub-object pointer adjust) unless T and its members declare the FableWin.pdb user memberwise copy ctors (C3DVector, C3DHelperPoint, C3DMeshVolume).
- Same-TU noinline helper bodies decide which STLport helpers stay out of line; copy the landed model (0x98D360, 0x624B90, 0x9B5310) verbatim.
- Frame-slot sharing: a scoped `{ D3DVOLUME_DESC d; ... }` block lets VC reuse the slot, and a named `sliceStart` local reproduces retail's pointer flow (0xA68910).
- Ego_r.pdb donor order separates byte-identical twins: Swizzle comes before Unswizzle (0xA67440 / 0xA67450).
- VC7.1 canonicalizes x87 product operand order, so swapping a*b in source does nothing (0xA55F90).
- VC keeps an unused std::list size() walk, so do not drop it from the source (0xA618D0).
- A VC7.1 16-byte-aligned frame with a return-address copy (mov ebp,[ebx+4]; mov [esp+4],ebp) does not reproduce in the lane with optimize("y",off) or /Oy- (0xA5C360).
- GFRemoveCommentsLeavingNewlines memmove size is textLength - returns - newlines - end + 1. Write the test expectations from retail behaviour, not from intent.

## Review at landing
- 0x00A68910: switch the local CPixelFormat / CTexture declarations to the engine headers (SHARED_HEADER_AVAILABLE), then regrade.
- 0x00A70CB0: the RELOC_DUPLICATE (0x66DDF0) and RELOC_CONFLICT (__copy<const C3DVector*> 0x74CA20 vs 0x75B560) are identical-COMDAT folds. Pick one binding for each.
- 0x00A67460 ReleaseVolume: the PDB no-argument spelling vs retail's bool argument is unresolved.
- Apply the label corrections in NOTES.md (CMatrix3x4 ctor binding at 0xA13C60, the CVectorMap Adds, the CBusyResourceManager neighbours, the wrong stand-in types).
- 0x00A4B670 and 0x00A5C360 are retail_asm candidates: FloatToLong fistp, and an SSE block that already matches.
