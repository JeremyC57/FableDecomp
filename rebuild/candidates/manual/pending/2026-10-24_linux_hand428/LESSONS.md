# Chunk 428 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport range `erase` with a non-power-of-two element size.**
  - VC7.1 inlines the plain-inline `copy -> __copy_aux -> __copy_ptrs -> __copy` chain for a 32-byte element (landed 0x00A96A90), but not for 12-, 24- or 56-byte ones. The pointer-difference divide pushes the chain past the inliner budget.
  - What matches retail: the 2-level chain `copy -> __copy_ptrs(__false_type) -> __copy` with all three `__forceinline` (0x00B7FD70, 0x00B80190).
  - The 4-level chain still leaves `__copy_ptrs` out of line even when forced (0x00B6CD80).
- **`1.0f / z * F` is folded to `F / z`.** Compute `float oneOverZ = 1.0f / z;` as its own statement to keep retail's `fdivr` plus `fmul` (0x00B4A6C0).
- **Bool compares.** `return a < b;` in a bool function widens the result through eax. `if (a < b) return true; return false;` gives retail's `mov al,1` / `xor al,al` (0x00B670E0).
- **Nested matrix loops.** VC7.1 fully unrolls the inner loop of a nested i/j loop, so a 4x4 multiply should be written as the nested loop into a local, then `*this = result`. Hand-unrolled sums schedule differently (0x00B507E0).
- **Temporaries and frame slots.**
  - Temporaries inside one full-expression keep distinct frame slots. Split into separate statements, VC7.1 packs them into one slot (0x00B7AD20: frame 0x20 against 0x10). Keep retail's single full-expression, with the inline accessors (`GetWidth()` = `GetDimensions().X`).
  - A small class returned by value (C2DExtentsI) needs a user ctor to go through a hidden pointer.
- **x87 operand order.** For commutative x87 products of member/global floats, VC7.1 picks the operand order whatever the source order. A three-factor product (`SimFadeBegin*SimRadius*Zoom`) did not follow source reordering (0x00B806E0, parked).
- **Compound conditions in a loop.** Putting `p.Get() != 0 && p->IsBusy()` behind a bool-returning inline helper changes register choice: ecx/edx as in retail, against edx/eax when written inline (0x00B81450).
- **Branch-chosen push_back.** If/else-if/else over three STLport list `push_back`s is tail-merged so that only the end-node loads stay in the branches. Choosing the list by reference first gives `lea` in each branch (82v79) (0x00B817D0).
- **`_M_insert_overflow`.** It needs same-TU `#pragma auto_inline(off)` + `__declspec(noinline) __fastcall` bodies of `__uninitialized_copy` and `__uninitialized_fill_n`. Declaration-only callees change the register and stack homing (0x00B3EA70).
- **`??_G` of a two-base engine component with an STLport list member.** Declare the class with an empty out-of-line virtual dtor and model the list's `~_List_base` as `clear(); _M_node.deallocate(p, 1)`, where the allocator does `if (p) free(p)`. Both base vtables are then reset, as in retail (0x00B3ADC0).
- **`CArray<T*>` delete loops.** A trivially destructible element's `delete` is the bare static class `operator delete` with no null test. `clear()` through `__copy_trivial` keeps the `cmp eax,eax` of finish against itself (0x00B49FE0, 0x00B4A060).
- **STLport namespace.** Retail's RTTI strings spell STLport's namespace `std` (12 `@std@@` hits, 0 `@_STL@@` in Fable.exe). The lane convention `_STL` changes only the mangled names, not the bytes. All 22 rows of this chunk that were first written in `std` were renamed to `_STL` and re-graded by gall.sh unchanged (MATCH/RELOCATION_MATCH).
- **Inline-asm float-to-long.** A float-to-long conversion that retail does as `fstp [slot]; fld [slot]; fistp dword [esp]` is an inline-asm helper, since VC7.1 C++ emits `call __ftol` (0x00B84720, parked as a retail_asm candidate).

Review at landing:
- **Local models next to existing engine headers.**
  - These classes have generated `engine/<Class>.h` headers but are modelled locally (no include): CEngineSubPrimitiveRenderer (0x00B842B0, 0x00B84720, 0x00B84B20), CEngineSimulationManager (0x00B817D0, 0x00B806E0), CEngineLandscapeRenderer (0x00B670E0), CEnginePrimitiveRenderer2D (0x00B4A6C0), CVertexShader (0x00B6B540, 0x00B6B610, 0x00B6B790), CEngineWaterRenderer and CSurface (0x00B7AD20), CEnginePrimitiveAttachmentManager and CEngineComponent (0x00B3ADC0), CEnginePrimitiveRenderer and CEnginePersistentPrimitiveBase (0x00B31FC0), CPixelFormat (0x00B3E920).
  - Why: the headers carry Ego_r layouts with 12- or 16-byte Dinkumware containers, while retail STLport lists are 4 bytes and vectors 12. They are also data-only records with the vptr as data, and these rows need real virtuals or member functions.
  - The landed 0x00B849F0 gets around this with `#define <Class> <Class>Donor` + `#include`. Decide whether to apply that convention or regenerate the headers.
- **Already-landed sources that look wrong.**
  - `CTCInventoryBase_SetViewportFloat_00a0b560.cpp` names the wrong class. Per the landed RenderProgress 0x00499880 and this chunk's 0x00B7AD20, it is the render manager's `SetViewport(const C2DBoxF&)`.
  - 0x00B83330 is landed as "_Move" but is STLport `__copy<CCountedPointer<CParticleSimulation>*>`.
  - 0x00B833F0 is landed as "_Destroy_range" but is `__destroy_aux(__false_type)`.
  - 0x00B849F0 RenderType uses a memcpy-PMF trick that should be reviewed.
  - 0x007590C0 `_Sort_heap` is modelled as Dinkumware; retail is STLport `sort_heap`.
  - 0xB6B700 and 0xB6BB10 are landed as Dinkumware `_Uninit_fill_n`; they are STLport `__uninitialized_fill_n`.
  - The DeallocateMemory landings 0x0051A350, 0x006FC620, 0x009D3D90 and 0x009B5F30 use an `Exchange` hack. The real STLport form is `clear(); CArray<T>().swap(*this);` (this chunk's 0x00B83D30).
- **0x00B6CD80.** differ/ holds the ungraded 2-level all-`__forceinline` form; the grade budget was spent on the 4-level chain. The same form matched both siblings, so grade it first.
- **0x00B84720.** This is a candidate for the 2026-10-24 retail_asm owner exception: a C++ body with a minimal `fistp` `__asm` and a `// Retail-asm:` line. This chunk's rules forbade `__asm`.
- **Grade budget.** No row needed more than 4 grades. Seven rows used all 4: 0x00B6B540, 0x00B7AD20, 0x00B80190 and 0x00B81450 matched; 0x00B6CD80, 0x00B806E0 and 0x00B84B20 are parked.
- **Catalog rows holding several functions.** None found; every row's bounds agreed with the built object.
