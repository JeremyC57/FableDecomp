# Chunk 461 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Live-slot STLport tags. Where retail passes `lea reg,[ebp/esp+slot]` of a LIVE argument as an empty tag, give the
  tag-taking callee a same-TU `__declspec(noinline)` stand-in body under `#pragma auto_inline(off)` that never reads
  the tag. Declaration-only callees make VC7.1 home the tag in a fresh frame word or a dead slot. Seen in:
  `_M_insert_overflow` `atend` (0x005E29D0 / 0x0085E200 / 0x008657E0 / 0x0086E1C0), reserve `n` (0x008ACD30 /
  0x00ADB960), push_back `x` (0x0086E7C0), `RemoveMap`'s value (0x006BC160 `__find`), `ClearTextureMorph` index
  (0x006CA850 `__copy`), and the vector(n) `n` slot (0x0057FCDD).
- Dead-slot tags. When the tag goes into a DEAD argument slot (`x` in the copy ctors 0x006B6840 / 0x008899C0,
  `position` in 0x00CD3152, `last` in the erase rows), a declaration-only callee is enough.
- Under `#pragma optimize("s", on)` VC7.1 will not inline STLport's small `inline` helpers (`__copy_ptrs`, the counted
  random-access `__copy`, the `uninitialized_fill_n` forwarder). Mark them `__forceinline` (0x00477E36 / 0x00478C2D /
  0x0057FCDD).
- STLport `_M_clear()` / `_Destroy(first, last)` over a trivially destructible element: an inline
  `__destroy_aux(.., __false_type())` loop folds to nothing, leaving only the null-tested free (0x0085E200 /
  0x00ADB960). For a non-trivial element, keep `__destroy_aux` out of line at its own address. `_M_clear` itself is
  sometimes out of line (declare it: 0x0086E1C0 / 0x0086E7C0 / 0x008ACD30) and sometimes inline (0x005E29D0 /
  0x0085E200 / 0x008657E0): check for a call to it.
- vector.c `_M_insert_overflow(.., const __false_type&, ..)` matches as written in the source with
  `(max)(old_size, fill_len)` taken by reference. That homes old_size in the `push ecx` word. Keep the
  `fill_len == 1` placement copy inline.
- `vector::resize(n)` emits the inline 2-arg `resize(n, x)` too: pin the `// Leaf:` to the full mangled 1-arg name
  (`...QAEXI@Z`), or the leaf prefix matches both (0x00B6CBD0).
- map::find in a guard: the named `iterator it = m.find(k); if (it == m.end())` gives retail's call-before-end()-load
  order. The one-expression `m.find(k) == m.end()` loads the header first (0x008AE510 / 0x008AE970).
- list::operator= over a payload list: `insert(pos, f, l)` is the inline member-template forwarder to the
  out-of-line `_M_insert_dispatch(.., __false_type())`. Its tag lands on the dead `x` slot (0x0076FF90).
- Element identity: check the catalog's propagated element names against PDB class sizes and the landed callees.
  "CFeatDef" (100 bytes in Ego_r) on 0x008899C0 / 0x0088A350 was really the 500-byte
  NEnvironmentInternal::CBlendedEnvironmentTheme (a `class`, mangled V). Retail STLport vectors are 12 bytes and lists
  4, so Ego_r sizes run high in STL-bearing classes.
- A string range ctor that never zeroes the members and returns finish is
  `basic_string::_M_range_initialize(f, l, forward_iterator_tag())`, not the ctor. It ends in the noreturn
  `_M_throw_length_error`, so it is an int3-after-noreturn row (0x005672D0, 90v89).
- Under "s" with a frame pointer, retail placed the `__destroy_aux` tag on the LIVE `first` slot (`[ebp+0Bh]`), and
  none of these placed it there: forceinline helpers, a named tag local, or the full `__value_type` /
  `__iterator_category` chain. They all give the dead `last` slot (`[ebp+0Fh]`). Next idea: a same-TU noinline
  `__destroy_aux` stand-in (0x00476F9B, parked).

Review at landing:
- Grade counts over 4: 0x00478C2D logged 5 grades. One was real; three were my own header-splice edit slips (a
  `#pragma optimize("s", on)` string inside a comment cut the splice); the final body is 0x00477E36's body verbatim.
  0x0057FCDD used exactly 4.
- Local models next to existing engine headers. The verifier may warn about these; each is deliberate:
  - CTCBase (0x00686ED0): engine/CTCBase.h is data-only and models the vptr as a data member, so it cannot declare
    the static list or the method.
  - CVertexShader (0x00B6CBD0): engine/CVertexShader.h is data-only, and the virtual dtor chain is needed.
  - CScriptThing (0x008ACD30 / 0x00CD3152): engine/CScriptThing.h is data-only, and its virtual dtor is needed.
  - CRegion (0x006BC160): engine/CRegion.h carries the Ego_r 16-byte Dinkumware vectors. The retail `Maps` vector
    is 12 bytes.
  - 0x0085E200 does include engine/CPixelFormat.h.
- Already-landed sources that look wrong:
  - 0x006BAB80 names its class `CEntry`; it is CFaction.
  - 0x0086BDE0 "CEntryLayers" is CRandomAppearanceMorph.
  - Landed 0x0085E570 calls its element "Element"; it is CTextureManagerPoolInfo.
  - `CReactionMatch_` has a trailing underscore.
  - Landed 0x0085D7A0 and 0x006F9640 model STLport in namespace `std`, not `std`, so their mangled names may not
    match retail's.
  - Landed 0x006B4900 calls 0x006B6840 the CEnvironmentThemePriorityStack copy ctor. It is byte-identical to the
    vector<CEnvironmentThemePriorityStackEntry> copy ctor (the class holds just that vector), so check which symbol
    the lander binds.
  - Landed 0x00864F80 (the CFlash copy) uses descriptive list structs instead of STLport templates.
  - The landed comment on 0x0050E540 says "Dinkumware _Find"; it is STLport `__find`.
  - The caller note on 0x004CD9D0 says vector<ulong>; it is a folded 4-byte `_Vector_base(n, a)`.
- Callee label: 0x0042A34E "CMemoryAllocatorVariableSize::Erase" is `_Rb_global<bool>::_Rebalance_for_erase`.
- 0x004046B0 is one of about 90 COMDAT-folded list destructors. Its element type (CStringParserCommentBoundaries)
  comes from the catalog, so the lander may bind another twin's name.
- No catalog row in this chunk holds several functions.
