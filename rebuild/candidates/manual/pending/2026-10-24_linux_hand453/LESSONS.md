# Chunk 453 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **`__copy_trivial`.** Use the real STLport 4.6 body:
  `return (__last == __first) ? __result : (char*)memmove(__result, __first, __last - __first) + (__last - __first);`.
  It gives retail's `cmp last,first ; je ; sub` (0x00F38440, 0x00F3B640). The tempting `n = last - first; return n ? ... : result;`
  gives `sub ; je` and misses by 2 bytes. Inside `erase`, the `__last == __first` test also sets the operand order of the
  merged `position + 1 != end()` compare (likely fix for parked 0x00F36F40).
- **Same-TU template bodies matter.** When retail homes an empty `__true_type`/`__false_type` temporary on a *live*
  argument slot (`lea edx,[esp+N]`, the same slot as `&x` or `atEnd`) and keeps the argument unaliased afterwards,
  the out-of-line callee (`_M_insert_overflow`, `__uninitialized_copy`) has a visible same-TU template body that never
  touches the tag. Define the real template body, or a noinline stand-in that only reads the value (0x00F38770,
  0x00F38860, 0x00F3B330). A declaration-only callee gives a separate `push ecx` tag slot and argument reloads after
  every store (0x00F36F40 grades 1-3).
- **vector<long> copy ctor: VC7.1 splits it by itself.** With one inline definition, VC7.1 inlines vector<long>'s
  copy ctor into `__uninitialized_copy` / `__uninitialized_fill_n` (0x00F38440, 0x00F384E0) but calls it out of line
  from `_M_insert_overflow` / `push_back` (0x00F38770, 0x00F38860 -> 0x00514160). That is the retail split, so no
  `inline_depth` pragma is needed.
- **Trivial `_Destroy` loops vanish.** STLport `_M_clear()` = `_Destroy(start, finish)` plus `deallocate` inlines to
  `if (p) free(p)` when the element destructor is trivial (0x00F3B330). Keep it out of line (declaration only) when
  the element has a real dtor (0x00F38710).
- **Partial static folding of dynamic initialisers.** A global built through an inline ctor from constant and
  sqrt-derived components: VC7.1 folds the constant words into the static image and the `$E` function writes only the
  computed ones, but only when C3DVector has a memberwise *user* copy ctor. The implicit bitwise copy sends the
  constants through stack temporaries and folds nothing: 115 bytes v 40 (0x0121AD20, 0x0121AD50).
- **Explicit `(float)` cast.** `(float)expr` on an x87 value rounds through memory (`fst [esp] ; fld [esp]`), even
  without /Op. It is not a free way to make two otherwise identical expressions differ for CSE (0x0121ACD0).
- **`__ehvec_ctor` naming.** VC7.1 itself mangles a function named `__ehvec_ctor` as `??_L` (`eh vector constructor
  iterator'). Under `#pragma optimize("s", on)` the CRT body gets `__SEH_prolog` / `__SEH_epilog`, and its `__finally`
  funclet follows `ret 14h` in the same COMDAT. Model the CRT's `__thiscall` function pointers as pointers to members
  of an empty class (0x00F3FB96).
- **`delete` with an inline non-virtual dtor.** `delete p` where the class has an inline dtor that calls an
  out-of-line member gives `test ; je ; mov ecx,p ; call member ; push p ; call operator delete`. Adding `if (p)`
  around it doubles the null test (0x00F36F40).
- **Element identity from callers.** A 0x1c record {long, vector<long>, ptr, long, bool} pushed in a `ret 8` function
  right after the GPU scratch manager dtor is CGPUScratchBufferTextureManager::CResourcePage. A 24-byte all-scalar entry
  pushed from a recursive `ret 14h` function is CRangeCompressor::CScriptEntry (CalcCompressionScript 0x00F3ABF0). Check
  `ret N` against the Ego_r signature before trusting catalog element names.

Review at landing:
- **0x00F36F40 FreeResourceEntry: parked after the full 4-grade budget, one compare operand away.** Try the
  `__last == __first` `__copy_trivial` first. Its same-TU noinline `_M_insert_overflow` stand-in follows the landed
  0x007CB850 / 0x00F37030 device.
- **0x00F3FB96 `__ehvec_ctor`: the row is cut 24 bytes short.** The `__finally` funclet 0x00F3FBE0..0x00F3FBF7 belongs
  to it. Our object matches all 98 bytes, so land it against the 98-byte span (test: differ/00f3fb96.test.cpp).
- **0x00F3FC50 / 0x00F3FD10 / 0x00F3FD50 are CRT MASM helpers.** They are not compiler output and need a lane decision
  (retail_asm, or exclude from coverage).
- **Local models next to existing engine headers:**
  - CDefClassBase (engine/CDefClassBase.h is a pack(1) flat layout with the vptr as data): 0x00F25BB0, 0x00F25D20,
    0x00F26FB0, 0x00F27780, 0x00F33EA0, 0x00F34110, 0x00F342B0, 0x00F345F0.
  - CTexture / CSurface (engine/CTexture.h and engine/CSurface.h are flat data layouts): parked 0x00F36F40.
  - CCutsceneDef: skips the hand-corrected CParentDefClassBase header (0x00F2A1D0).
  - Local CScriptDef models: there is no header (0x00E5FC70, 0x00E7F7D0).
- **Mangled names differ from Ego_r.** The quest-script `Alloc` / ctor models spell the interface as `struct
  CGameScriptInterface&`. Ego_r mangles it as `AAVCGameScriptInterfaceBase@@` with class keys `V`. The bytes are
  unaffected; align the names if the lander checks mangling.
- **Errors in already-landed sources:**
  - 0x00F3B220 is named `std::__uninitialized_fill_n<pair<EHeroMorphType, CTextureMorphEntry>>`. It is
    `_STL::__uninitialized_fill_n<CRangeCompressor::CScriptEntry*>`.
  - 0x00F3B420 "CRetailVector_SRecord24::PushBack" is `vector<CRangeCompressor::CScriptEntry>::push_back`.
  - 0x00F37030 FreeMemoryPage names its callee "FreeTextureSlot". The PDB name is `FreeResourceEntry`.
  - 0x00F3B8A0 / 0x00F3B8F0 "CElement12_00f3b640" is CArray<unsigned short>.
  - The landed 0x009FC550 "~CMemoryBlock" is likely ~CResource.
- **Catalog rows that hold several functions:** none. 0x00E5FC70's row includes its own jump table.
