# Chunk 421 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport `__unguarded_partition` over a pair with non-trivial members (0x00A71910).** VC7.1 inlines the implicit
  destructor of the by-value pivot; retail calls the COMDAT out of line (`~pair` 0x00A6F390).
  - Declare `~pair();` in the pair model, which went from 182v110 to 119v110.
  - Then put the same-TU `CBasicString<char>::operator<` body under `auto_inline(off)`. This gives retail's registers
    (first edi, last ebx, the pivot's data reloaded into esi each pass) and a RELOCATION_MATCH.
  - The sibling `__unguarded_linear_insert` 0x00A70540 matched with the destructor inline and operator< only declared:
    each site needs its own decision.
- **Out-of-line vs inline swap is per instance (0x00A6F610 vs 0x00A6F6F0).** Retail calls swap for the 20-byte
  pair<CCharString,C3DHelperPoint>: declare it only. For the 56-byte dummy-object pair, VC inlines the plain STLport
  body. Match the retail call/no-call per row.
- **By-value C3DVector return (0x00A6C9C0, `return map.LowerBound(k)[i].second.Position;`).**
  - Use pack(1) plus a user memberwise copy ctor `C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}`.
  - The implicit copy gives `push esi ; mov ecx,eax` and the wrong store order (51v49).
- **`vector::_M_allocate_and_copy` member template (0x00A6E470).**
  - `__uninitialized_copy` must be `__forceinline`, or it becomes a cdecl call (59v83).
  - STLport placement new is `throw()`, which gives the null test before construction.
- **pair<K, C3DMeshVolume> family.** One model matched 12 rows on the first grade (the 0x00A70540 model):
  - an in-class `C3DMeshVolume::operator=` (string `operator=` then CCountedPointer::ShareData);
  - an implicit pair assignment;
  - a CCountedPointer copy ctor and a dtor that releases then zeroes Data and Info.

  VC itself chooses between inlining the record assignment and calling its 0x00A32960 COMDAT, and matches retail at
  every site in this TU.
- **STLport 4.6 `__insertion_sort` (0x00A73240 / 0x00A732B0).** The signature is `(first, last, comp)` with no `_Tp*`
  tag (`ret 4` for an empty comparator), and it calls `__linear_insert(first, i, *i, comp)`. first and last live in
  two frame words.
- **`__partial_sort` (0x00A6BBB0 / 0x00A72300 / 0x00A72380).** `make_heap` is always inline down to the out-of-line
  `__make_heap(first, middle, comp, (T*)0, (int*)0)` (two `push 0`). `__pop_heap` is inline for the 8-byte pair and
  out of line for the 20- and 56-byte pairs. `sort_heap` is always a call.
- **`vector::push_back` (0x00A72700).** With `_M_insert_overflow(pos, x, const __false_type&, n, bool)` declared only,
  VC7.1 homes the `_IsPODType()` tag in x's argument slot (dead once x is in ecx). No same-TU stand-in is needed for
  a dead slot, unlike the live-slot case.
- **CVectorMap range queries (0x00A6CA20 / 0x00A6CA80).** `last = UpperBound(name)` is a named local evaluated
  before the `for (it = LowerBound(name); it != last; ++it)` loop, which matches retail's call order.
- **??_G of a class that declares no destructor but has an STLport vector member (0x00A6BFC0, parked).** Retail
  inlines the generated destructor with no vtable re-store. VC7.1 either:
  - inlines it but calls ~vector (plain inline dtors), or
  - calls the generated destructor out of line (forceinlined vector chain).

  `__declspec(novtable)` plus a forceinlined user destructor stops ??_G from being emitted at all.

Review at landing:
- 0x00A6BFC0 is parked after 4 grades (DIFFER 30v64). See NOTES.md for the next idea.
- 0x00A6BFC0 models CResourceList locally (`virtual ~CResourceList()`, 0x3C bytes). engine/CResourceList.h exists,
  but it carries the vptr as a data member, so it cannot stand in for a polymorphic base with a real virtual
  destructor. The differ source keeps the local model.
- The CPhysicsMesh rows (0x00A6C9C0..0x00A6CBC0) use a local `CPhysicsMesh { unsigned char _pad00[N]; CVectorMap ... }`
  model, because there is no engine/CPhysicsMesh.h. Retail CVectorMap is 16 bytes, so a generated header from Ego_r
  would put the maps at the wrong offsets. Check this if a header is generated later.
- Already-landed labels:
  - 0x00A6F290 is named for CNavLineMapValue, but its caller 0x00A6C9C0 uses it as the HelperPoints LowerBound (folded
    20-byte bodies).
  - 0x00A70A10 is named `PairULongMeshVolume`, but its caller 0x00A728F0 sorts pair<long, C3DMeshVolume> by a signed
    key.
- 0x00A6B0E0 is byte-identical to the landed 0x00769210 (CDefPointer<CThingBaseDef>). The CIVCountedPointer<CLocalDetailPlacementGrid>
  identity comes from its callers (0x00A6B39C, 0x00A6BD49) only. The pointee is modelled locally, because
  engine/CIVCountedPointeeBase.h models the vptr as data.
- Header byte counts in 0x00A6EE30 and 0x00A703C0 were corrected to the trimmed row lengths (89 and 200) after
  grading. No code changed.
