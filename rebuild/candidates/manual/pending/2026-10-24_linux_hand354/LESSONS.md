# Chunk 354 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `__equal_range` whose straddle case calls __upper_bound BEFORE __lower_bound, and never advances `first` in
  place: write `return pair<I,I>(__lower_bound(first, middle, val, comp, (D*)0), __upper_bound(middle + 1, first + len,
  val, comp, (D*)0));`. VC evaluates arguments right to left. The statement-by-statement STLport form calls
  lower_bound first (DIFFER 156v147). Example: 0x00817E30.
- `__uninitialized_fill_n` / `fill` copy shape follows the copy constructor's form:
  - One `movsd`-style block (esi/edi, words through ebx): a trivial (implicit) copy (0x007183E0, 0x00F3B220,
    0x00819230, 0x009D1BB0).
  - Key directly, then the rest as a block through `lea esi,[ecx+4]`: STLport pair's "VC++ needs this" user copy ctor
    (0x0081A200, 0x004CFA70).
  - Every word straight through esi with no second IV: a FLAT record with a user copy ctor (0x0040EAC0, 0x00B8E930,
    0x00F37AD0).
  - Second IV (`lea esi,[ebx+0Ch]`): a user copy ctor over a member that has its own (0x00CB6160, 0x006BE770).
- A C2DVector member copied inside a memberwise copy ctor needs C2DVector's own (memberwise) user copy ctor. A POD
  C2DVector adds a second induction register (DIFFER 86v80). Example: 0x00613E30.
- __unguarded_insertion_sort_aux in the size-optimised frame form needs `optimize("y", off)` + `optimize("s", on)`, a
  __forceinline __unguarded_linear_insert, and STLport pair's user copy ctor. Without that ctor `val` is copied with
  `movsd` and the entry test is duplicated (65v69). Example: 0x00470D77.
- A list search whose compared object sits in the THIRD stack slot with edx unused is `find_if(first, last, pred)`
  with a by-value predicate and NO tag. An out-of-line `__find_if` would take the tag reference in edx. Check a caller:
  if it never sets edx, there is no tag. Example: 0x0076D810.
- A `ret 8` list `find(first, last, val)` with no edx argument is plain std::find with __find inlined into it.
  The `__find(.., const input_iterator_tag&)` form is `ret 8` with edx = val (0x006C4FC0 vs 0x00535F38).
- Retail def hierarchy for derived-member offsets:
  - CDefClassBase is pack(1), 0x25 bytes.
  - CParentDefClassBase is pack(1): InstantiationName at +0x25, then the retail 16-byte CVectorMap (12-byte vector,
    empty comparator, Dirty, 2 pad bytes) at +0x29. So sizeof is 0x39, and the first derived member lands at +0x3C.
  - The PDB's 20-byte CVectorMap gives 0x3D / +0x40, which is wrong for retail. Example: 0x0067C720
    CBlurredAttitudesDef.Attitudes.
- A polymorphic element's implicit operator= never writes the vptr word, and copies each `unsigned char x:1` flag by
  read-modify-write (`xor ; and 1/2/4 ; xor`). The flattened engine headers (`void* __vftable`) cannot express this,
  so use a local polymorphic model (0x007D1420, 0x00AF6D90, 0x0067C720).
- A double member in an implicit copy assignment is copied with `fld/fstp qword`, while float members go through
  integer registers. Example: 0x008895F0 CEnvironmentThemePriorityStackEntry.
- Retail CArray/vector members are 12 bytes, not the PDB's 16, so every later member moves down by 4. Example:
  CEnvironmentThemePriorityStackEntry is 0x30 in retail vs 0x38 in the PDB (0x008895F0, consistent with the landed
  0x00889DA0).
- A catalog `_Fill<..>` / `_Destroy_range<..>` label on a small body is usually propagated. Read the stride and the
  member copies before trusting the element type: 30 of the 40 rows here had a wrong element or function.
  Examples: 0x009A0020 is MemCopy, 0x00A96810 is DestructArray, 0x00806D80 is remove_copy.

Review at landing:
- Landed 0x004B1670 (AddQuestCardEntry) declares its callee 0x004B91F0 as `__find(first, last, tag&, name&)`. The real
  form is `__find_if(first, last, CStringIs pred, const input_iterator_tag&)`. It is a declaration only (no codegen
  impact), but the name and signature should be corrected.
- Landed 0x00CBD510 (CRecordSet) declares the element of 0x00CBB200 as a 16-byte SSurfacePair with a long first. The
  0x00CBB200 body strides 12 bytes over {CCharString, CCountedPointer}. Declaration only.
- 0x0067C720 and 0x007D1420 use local CResource / CDefClassBase / CParentDefClassBase models next to existing engine
  headers. The headers are flattened (vptr as data) and CParentDefClassBase.h uses the PDB layout (0x3D, 20-byte
  CVectorMap), not retail (0x39). Strict-header mode will warn; the local models are required for the bytes.
- 0x005E1820, 0x00A43890, 0x00A96810, 0x00AF1C70 use a local polymorphic CIVCountedPointeeBase (virtual destroy in
  slot 1) next to engine/CIVCountedPointeeBase.h, for the same reason.
- No row needed more than 4 grades. The multi-attempt rows were 0x00470D77 (3), 0x0040EAC0 (flat-record probes),
  0x00613E30 (2) and 0x00817E30 (2).
- 0x007183E0 / 0x00F3B220 (block copy) and 0x0081A200 (STLport pair copy ctor) share a catalog name and layout, but
  retail compiled them differently. Either one module's element is a plain record or its pair lacks the ctor; the
  plain-record form is kept for the first two.
