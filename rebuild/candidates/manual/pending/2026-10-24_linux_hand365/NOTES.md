# Hand chunk 365 (_Uninit_copy .. _Uninit_fill_n): 40 of 40 byte-exact

All 40 rows grade MATCH (24) or RELOCATION_MATCH (16) on the exact file in `src/`. All 40 tests print PASS under g++
(`tall.sh`) and VC7.1 /Od (`tall.py`). Every catalog label in this chunk is a propagated VC8 name
(`_Uninit_copy<_Vector_const_iterator<..unsigned int..>>` / `_Uninit_fill_n<pair<u64, CCountedPointer<CActiveEntityScriptBase>>>`)
over STLport 4.x bodies in /Gr TUs. The sources therefore use descriptive, address-suffixed `_global` names, with the
element type named from vtables, callee addresses and landed callers. No row looked mis-cut: every oracle extent ends on
the `ret imm16` and the next 16-byte boundary is `int3` padding.

## What matched (40)

### STLport `__uninitialized_copy(first, last, result, const __false_type&)`: `__fastcall`, `ret 8` (25 rows)
- **Trivial or POD-ish elements, looping directly on `result`.**
  - Rows: 0082c420 (dword), 009ca4c0 (pair<long, CCountedPointer>), 00a43630 (NGameText::CDataBankTextEntry),
    00a7e990 (12-byte record with a memberwise user copy ctor), 00a7e630 / 00a7e5c0 (pair<u64, long> / pair<u64, bool>,
    16-byte stride copying 12 / 9 bytes), 00a6e350 / 00a51c60 (C3DTriangle2), 00a52040 (C3DVectorWord),
    008002f0 (pair<C2DVector, uchar>, 9-byte stride: C2DVector really is packed), 00bbd520 (pair<uchar, long>),
    00b0f670 (pair<ulong, CParticleUpdateParamDesc>, vtable 0x0125FEE4), 00a31fd0 ({long, 12-byte record, long}) and
    0082c690 (CRGBColour, copied through Set(r,g,b,a) over the B,G,R,A bytes).
  - Shape: the `cmp ecx,edx` empty test is hoisted above the loop, the placement-new null test sits on the destination,
    and the advanced result is returned in eax.
- **`T* cur = result;` form.**
  - Rows: 0086abf0 (pair<long, 12-byte record>), 0081a1b0 (pair<EHeroMorphType, CTextureMorphEntry>),
    00786e40 ({long, CCharString, long}), 0086d870 ({long, long, CVectorMap}), 00ad6ae0 ({uchar, set, set}),
    00ab4450 / 00a5e7f0 (CVertexShaderInput::CStreamInfo) and 00a82730 / 00a831d0 / 00865650 (list copies).
  - Why: the named local keeps result loaded before the empty test and recomputes the inner block address
    (`lea edi,[eax+4]`) per iteration. Looping on `result` itself strength-reduces it into a second induction register
    and spills `last` (0086abf0 DIFFER(78v65), 0081a1b0 DIFFER(90v77)), or for CStreamInfo adds a second epilogue that
    reloads result (DIFFER(70v61)).
- **CStreamInfo (00ab4450 / 00a5e7f0):** {CBaseClass vptr, CVertexStreamFormat (vtable 0x0129D578),
  CArray copy 0xA5E0A0}. Its copy ctors need `__forceinline` to stay inline under the outer loop.
- **List copies (00a82730, 00a831d0, 00865650):** the retail STLport list is one header-node pointer. The copy ctor runs
  `_List_base` (proxy zeroed, `malloc` header 0xBFEA0E, self-link) and then `insert(begin, x.begin, x.end)` into the
  out-of-line `_M_insert_dispatch`: 0x5152C0 for the 0x18-byte node, 0x76FD40 for 0865650's 0x24-byte node.
- **Dinkumware `_Uninit_copy(first, last, dest, _Alloc&, _Nonscalar_ptr_iterator_tag)` (00c29340):** the tag is passed by
  value, giving `ret 0Ch`. It is the same body as 0082c420, constructed through `allocator::construct`.

### STLport `__destroy_aux(first, last, const __false_type&)`: `ret 4` (2 rows)
- **009f6e00:** CInputEventPackage (0x30 bytes, holds a vector<CInputEvent>). The inlined ~vector walks
  ~CInputEvent 0xA03B30 (empty, out of line) and then `free` 0xBFEA14.
- **00a43f80:** a vector of {CCharString (~ 0x99EAE0), long, long}. The body is ICF-folded across 12 callers.

### STLport `__uninitialized_fill_n(first, n, x, const __false_type&)`: `ret 8` (11 rows)
- **`T* cur = first; for (; n > 0; --n, ++cur) new (cur) T(x); return cur;`**
  - Rows: 00889230 (CEnvironmentThemePriorityStackEntry, vector copy 0x6B58F0), 009ee100 (CGenericVar,
    vtable 0x0129C3E4), 005e0ca0 (pair<long, CIVCountedPointer>), 008ac180 (CScriptThing, vtable 0x01238C8C),
    006b8de0 (pair<long, CCountedPointer>), 005e1740 (pair<ulong, CAnimationEntry>),
    00769810 ({CCharString x2, long x3}, copy 0x99EC30) and 008181e0 (28-byte record with two CIVCountedPointers).
- **005b149f / 005f7a1d:** CIVCountedPointer in unaligned TUs. Each needs `#pragma optimize("s", on)`: &x is reloaded every
  iteration and no esi frame is opened.
- **00513570:** a flat `{long first; CCountedPointer m_a, m_b, m_c;}` with an implicit copy ctor. The second induction
  register sits at +0x10 as in retail. The pair<long, triple> nesting put it at +0x18, and {CCP, pair<CCP,CCP>} at +8.

### STLport 3-arg `uninitialized_fill_n(first, n, x)`: `ret 4` (2 rows)
- **00440180 (CEnginePrimitiveHandle, vtable 0x0122F598) and 007688b0 (CIVCountedPointer):** the inner
  `__uninitialized_fill_n` must be `__forceinline`. With plain `inline` it stays a call (00440180 DIFFER(23v49)).

## Parked for Fable 5.1 (0)

## Skipped
- none (no row holds two functions).

## Label corrections
- 00889230: `_Uninit_fill_n<CCamera*,...>` is wrong. The element is CEnvironmentThemePriorityStackEntry (its vector copy
  is 0x6B58F0, and the PDB CCamera is 56 bytes).
- 009ee100: `_Uninit_fill_n<pair<CCharString,CScriptInfo>*>` is wrong. The element is CGenericVar (vtable 0x0129C3E4).
- 00440180: the 3-arg `uninitialized_fill_n<CEnginePrimitiveHandle*>` front end (vtable 0x0122F598, `ret 4`).
- 008ac180: `__uninitialized_fill_n<CScriptThing*>` (vtable 0x01238C8C).
- 009f6e00: `__destroy_aux<CInputEventPackage*>`, not `_Uninit_copy` (as the landed ~CInputManager declares).
- 00a43f80: `__destroy_aux<vector<{CCharString, long, long}>*>`, not `_Uninit_copy`.
- 00c29340: the Dinkumware `_Uninit_copy(.., _Alloc&, _Nonscalar_ptr_iterator_tag)`, `ret 0Ch`.
- 0081a1b0: pair<EHeroMorphType, CTextureMorphEntry>.
- 00a43630: NGameText::CDataBankTextEntry.
- 00b0f670: pair<ulong, CParticleUpdateParamDesc>.
- 00ab4450 / 00a5e7f0: CVertexShaderInput::CStreamInfo.
- 005e1740: pair<ulong, CAnimationEntry>.
- 0082c690: CRGBColour.
- 00a6e350 / 00a51c60: C3DTriangle2. 00a52040: C3DVectorWord.
- 00a82730 / 00a831d0 / 00865650: copies of list elements (vector<list<T>> growth), not unsigned-int copies.
- Callee names confirmed along the way: 0x99EAE0 = ~CCharString, 0x99EC30 = CCharString copy ctor,
  0xA03B30 = ~CInputEvent (empty, out of line), 0xBFEA0E = malloc, 0xBFEA14 = free.
