# Chunk 521 lessons

## GOTCHAS one-liners
- VC7.1 leaves STLport's plain-inline loop helpers (__uninitialized_copy / _fill_n, __copy, __destroy_aux, pop_heap / __pop_heap_aux / __pop_heap) out of line; mark them `__forceinline` to match retail inlining, and leave inline only the ones retail calls out of line (e.g. 0x00433D8A).
- `allocator::allocate` must be `__forceinline` too, or the malloc in `_M_insert_overflow` stays a call (0x0046C18C / 0x004701CE).
- The biased size frame `lea ebp,[esp-N]` comes from `#pragma optimize("s", on)` alone; adding `optimize("y", off)` turns it into `mov ebp,esp` (0x004EC8C2).
- A `new T` rvalue passed to push_back is homed in the dead argument slot only when the real `_M_insert_overflow` body is a same-TU noinline callee; if it is only declared, VC7.1 gives the temporary its own frame word (0x0044A1A0).
- An empty traits object can share a spilled pointer's frame word only if its real callee body (one that never reads `this`) is visible as a same-TU noinline function (CPersistTraits<EObjectAugmentationType>::TransferBinaryIn 0x00452D8E in 0x004EC8C2).
- An out-of-line retail ctor (pair<K,V>() 0x004EC9B5) must be declared only; defining it lets VC7.1 inline it.
- CWideString::operator==(const wchar_t*) inline: write the null case as `if (wcscmp(text, L"") == 0) return true; return false;`. `return wcscmp(..) == 0` materialises the bool (`neg; sbb; inc`), and the caller-side two-test goto form swaps ebx/edi (0x0048F939).
- On opaque engine/ header members, `reinterpret_cast` at each use; a reference local hoists the `lea` and reallocates registers (0x0048F939, 0x0044A1A0).
- To satisfy the SHARED_HEADER policy while adding methods, use `#define X XLayout` / `#include "engine/X.h"` / `#undef X` / `class X : public XLayout { ... }`.
- x87 accumulators are kept in reverse declaration order: declare the sums a, b, g, r to get retail's R, G, B, A load order (0x004BD040).
- `sum / count` with a float `count` local compiles to one stored reciprocal (`fld1; fdiv st(1); fstp`) reloaded per channel; an explicit `1.0f / count` scale keeps it in a register (`fdivr`) instead (0x004BD040).
- A 2D length's x87 order follows the statement form: `r = Y*Y; r += X*X` gives retail's `fld Y; fmul st0; fld X; fmul st0; faddp` (0x0049ED80).
- The order of factors inside a C3DVector cross product does not change VC7.1 output (all 64 permutations compile the same), so a cross-product operand-order residue needs a different body shape, not factor swaps.
- The STLport 4.6 count loop in GFSerialiseMapBinaryIn is `for (; count > 0; --count)`; an `i < count` loop changes the register allocation.
- The CPersistContext::TransferVectorOfSubComponents load arm has a 1-byte register tie in the inline Read (`lea edx,[eax+4]` vs `add eax,4`). It depends on the element size (sar vs idiv). Don't spend grades on more source forms (20+ tried); see lane parks 00476E3E / 00477CD2.

## Review at landing
- 0x00433D8A: the landed `std_uninitializedcopyCWideStringCWideString_00433d8a.cpp` uses a `<CWideString*,CWideString*>` (PAV) spelling. Retail operator= calls it with const_pointer first / last, so the real symbol is the PBV form in NAME_FIXES.tsv. Respell the landed callers.
- 0x004D0660 is landed with the bare leaf `__adjust_heap` and the SCameraTagEntry element; 0x004D0B50 (global_popheap_004d0b50.cpp) calls it as `?__adjust_heap@@YIXPAUSKeyedRecord24@@JJU1@UCKeyPairCompareLess@@@Z`. Both should use the pair<float, CSubtitledMessage::CCameraTag> template spelling (NAME_FIXES.tsv). 0x004D0B50 itself is probably __pop_heap over the same pair (not proven here).
- 0x00452D8E: the landed stdcall `TransferBinaryIn_00452d8e` should become the thiscall CPersistTraits<EObjectAugmentationType>::TransferBinaryIn. 0x004EC8C2 carries its real body as a same-TU noinline callee. This duplicates the landed body's symbol only if the landed file is respelled to the same name, so check RELOC_DUPLICATE after the respell.
- 0x004E8212 / 0x004EC9D3 are probable names (_Rb_tree::clear / insert_unique via inlined map forwarders); an alternative is that the out-of-line symbols are map::clear / map::insert.
- The landed TransferVectorOfSubComponents bodies 0x00632E00 / 0x0067D6B0 / 0x00472A8D / 0x004ED953 / 0x007D2740 use non-std `vector` spellings; they should take the std::vector<T, allocator<T>> template form.
- 0x0048F939 uses the inline `??8CWideString@@QBE_NPBG@Z` (PBG because VC7.1 wchar_t is unsigned short here; the PDB prints `PB_W`). It is inline only, so it emits no relocation.
- engine/CMemoryDataInputStream.h / CDataInputStream.h need the Ego_r methods (inline Read / ReadType / ReadSLONG, private ReadWithSrcChunkOverflow) and the inheritance before the CPersistContext family can use shared headers.

## Why earlier attempts stalled
- The CPersistContext family (9 rows) hits a register-allocation tie in the inline stream Read that no source form tried moves; earlier attempts also used the propagated `<CAbilityButtonGuiDef>` label and non-template `Transfer(vector&)` spellings, so even near matches relocated against the wrong names.
- Several rows had wrong or behavioural labels and Dinkumware spellings (`_Sort_heap<CDebugCone*>`, `GFSerialiseMapBinaryIn<CCharString,long>`, `Appearance_FloatToCoord`), so the element types and callees could not be modelled until the identities were re-derived from the PDB, strides and vtables.
- The InitPlayerMode, GetTintColour and console-command identities were unknown; once the PDB names and the +0x10 STLport deque offset shift were established, the bodies followed.
- Shape traps that only fixed-form source resolves: forced inlining of the STLport helpers and allocate, same-TU noinline real callees for homing, the y-off frame, and the operator== form.
