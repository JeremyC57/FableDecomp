# Chunk 368 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Hoisted key load across a call (0x004636CF insert_equal).**
  - Retail hoists the value key's `PStringData` into ebx for the whole tree walk, even across a call to the
    `__fastcall` Compare 0x00411570. VC7.1 does that only when Compare's BODY is in the TU (it then knows Compare only
    reads memory).
  - With a bare declaration it re-reads the key per node and the loop shape changes too (`jmp` to the test vs guard +
    do-while, ebx pushed inside the guard).
  - Paste the landed callee body when retail hoists a load across a call.
- **Operand order in pointer equality (0x004636CF).** `cmp eax(node),ebx(key)` comes from `other.PStringData ==
  PStringData` inside `CCharString::operator<`. The landed operator!= at 0x0099E900 writes it this-first.
- **Size TUs keep inline helpers out of line.** Retail inlined these, so each needed `__forceinline`:
  - `CCharString::operator<` / `less::operator()` (0x004636CF).
  - `list::insert(pos, x)` inside `_M_insert_dispatch` (0x004638C8).
  - STLport's one-argument `vector::resize(n)` (0x00465852 / 0x00466BEB / 0x0046712B).
  - Three-call `CPersistTraits<T>::TransferBinaryOut` (0x00466B93) and the traits loop of 0x0045E808.
  - Look for "no call to it in retail" before suspecting the pragma.
- **STLport one-argument resize (0x0046712B, 0x00466BEB, 0x00465852).** `resize(n)` is
  `resize(n, T())` with the value temporary in the frame: `or [ebp-x],-1` for CDefString, zero stores, `fldz/fstp` for
  float, `mov byte [ebp-1],0` for bool. It is a visible two-argument call `push &tmp ; push n`. The landed 0x005C2AD8's
  one-argument resize is a different module's out-of-line instance.
- **STLport allocator value (0x00463871).** `allocator_type()` on an aggregate (empty) allocator is value-initialised:
  `xor eax,eax ; stosb` plus a byte copy. Give the allocator STLport's user-declared `allocator() {}` /
  `allocator(const allocator&) {}` and the temporary is free.
- **STLport list iterators (0x00463871).** `_List_iterator(const iterator&)` is user-declared. So iterator copies are built
  in the reserved argument slot (`push ecx ; mov eax,esp ; mov [eax],ecx`), while const_iterator copies are trivial and
  pushed directly.
- **Unsigned loop index (0x0045E808).** A fixed-count loop (`i < 5` over 0x24-byte entries) strength-reduced to a byte
  offset compares with `jb` only when the index and bound are unsigned; a signed `long` index gives `jl`.
- **CPersistContext one-argument binary Transfer<T>(T&) (0x0045C8D3, 0x0045E808).** The shape is CheckCRC(""),
  `CPersistTraits<T> traits;` (PDB: `{ T DefaultVal; }`), then `switch (Mode) { case 2: traits.TransferBinaryIn(PLoadStream,
  &value); case 3: traits.TransferBinaryOut(PSaveStream, &value); }`, which lowers to `dec/dec/je, dec/jne`.
  - A traits whose DefaultVal has a non-trivial ctor/dtor keeps its frame slot and its out-of-line ctor/dtor calls.
  - A trivial one folds away.
- **Def class mangling.** Element types in these templates are `class` in the PDB (`V`): CBrainBehaviour,
  CBrainUpdateZone, CSpecialEffectsStringMap, COpinionPersonalityTraits, CStringParserCommentBoundaries,
  CInventoryItemDef, CPhysicalPrimitiveInit, CDefString. A `struct` model mangles `U`, so a leaf copied from the PDB
  (`??$GFSerialiseVectorBinaryIn@VCBrainBehaviour@@@@`) matches no symbol and the grader silently picks the largest
  function.
- **Member-template leaf.** A member template's symbol starts `??$` (`??$_M_insert_dispatch@...`), not `?_M_insert_dispatch@`.
- **Packed def members without `#pragma pack` (0x0045370A).** CDefClassBase is 0x25 packed bytes and CEngineGraphic is
  14 packed bytes (PDB). Modelled as `unsigned char[0x25]` / `unsigned char[14]` (alignment 1), the derived members land
  on the PDB offsets naturally, and the 14-byte member copy is `movsd x3 / movsw`.
- **`__destroy_aux` (0x00440C00).** `__destroy_aux(first, last, const __false_type&)` over deque iterators is an
  out-of-line `__fastcall`: the tag reference rides in ecx, the two 16-byte iterators are on the stack, `ret 20h`.
- **Implicit destructors (0x00441870 / 0x00441C20 / 0x00443D70 / 0x00444ED0).** A compiler-generated destructor of a
  polymorphic class stores no vtable. A `__fastcall` `delete p` helper (the CCountedPointer deleter shape) makes VC emit
  the `??1` out of line.
- **List destructors (0x00443D70).** When one `list<long>` destructor is called out of line (0x0042AB73) while another
  list's is inlined, declare `template <> _List_base<long>::~_List_base();` as an explicit specialisation.
- **POD tag by value (0x00462E78, parked).** A `__true_type` returned by value from an inline `_Is_POD<T>::_Answer()`
  reproduces retail's `xor eax,eax ; stosb` plus the tag self-copy.
- **Def-TU pragmas.** STLport vector members in the def TUs (operator=, erase, `_M_insert_overflow`, fill ctor) are
  y-off + s. `_M_allocate_and_copy` is plain "s".
- **Slot-19 def thunks (0x00453705).** A def vtable's slot 19 is a `jmp` thunk to the def's `operator=`. Use
  rtti-vtables.tsv to name a propagated "CThingBuildingDef_CopyAssign" row; 0x004F9145 CHeroExperienceDef is the same.

Review at landing:
- Rows that needed more than 4 grades:
  - 0x00458833: about 9 probes, parked. The scheduling of the hoisted traits `lea` is a likely lane gap.
  - 0x00464D23: 8 grades/probes, parked. Its bool twin 0x004675F6 was graded once and parked on the same frame-slot gap.
  - 0x004636CF matched on the 4th grade.
  - 0x00440C00 matched on the 4th grade (recorded before this hand-over).
- Local base models next to an existing engine header:
  - 0x0045370A models CDefClassBase / CInventoryItemDef locally: engine/CInventoryItemDef.h flattens the base (no
    base operator= to call) and sizes Graphic 0xf where retail copies 14 bytes.
  - 0x00441870 / 0x00444ED0 model CFadeInFadeOutBase locally.
  - Parked 0x00459AF3 models CParentDefClassBase locally.
- Errors noticed in already-landed sources:
  - 0x00442770 `vector<unsigned int>::resize(unsigned int, unsigned int x)` takes the value BY VALUE. Its caller
    0x00464D23 passes `&tmp`: STLport's `resize(size_type, const T&)` / `_M_fill_insert(..., const T&)` forward the
    pointer with the same bytes. The model is byte-equal but semantically wrong.
  - 0x004518E1 is landed as `std::list<T_004518e1>::~list`. 0x0045C8D3 calls it as the destructor of
    CSpecialEffectsStringMap's map<unsigned long,long> (clear 0x00451046 + free of the header), so the body is at least
    shared with a tree destructor.
  - 0x00463334 TransferMap<CCharString,long> calls 0x004636CF as "map insert". That body is `insert_equal` (no
    predecessor check), which is multimap semantics.
  - 0x004670F5's leaf `??0?$CPersistTraits@UCBrainBehaviour` uses the struct key; the PDB has `VCBrainBehaviour`.
- Catalog rows that hold several functions: none. All 40 extents are single functions.
