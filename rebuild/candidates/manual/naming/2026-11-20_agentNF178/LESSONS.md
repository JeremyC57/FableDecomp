# NF178 lessons (round 10, cut-scene cast-map family)
- A row-local stand-in for an STLport container can often be swapped for the shared `stlport/fable_map.h` instantiated over the row's OWN
  local key / value classes: the mangled names are the same tokens, so the binders move without porting CCharString or the handles first
  (34 rows, mostly first grade).
- Grade the COMDATs the new object emits against their own retail addresses (q.py with the binder file and the COMDAT leaf): the ctor and
  ~_Rb_tree bodies of the 0x00D29DB0 port matched 0x00CDBF70 / 0x00CDBFB0, a second source for the identity.
- Rows absent from a ctor's / dtor's REFS list expand it in retail: for those, drop the DTOR_DECL_ONLY opt-in (implicit ~map called as a
  COMDAT) or keep the header ctor inline; the plain transform DIFFERed by the ctor call.
- Construct()/Destroy() "view" rows move only where the cast map's destruction can be expressed as a real scope (Destroy before `}` or
  before the hero's Destroy); a view destroyed by hand before another view at early returns needs those views ported first.
- A row with a local `namespace std { vector }` model cannot include the shared map header; port the vector too (VECTOR_DTOR_OUTOFLINE /
  _FORCEINLINE + ERASE_RANGE_OUTOFLINE reproduced the old local shapes first try).
- Adding `stdlib.h` (through the container headers) clashes with a row-local C++-linkage `int __cdecl rand();` (C2732); drop the unused
  declaration.
- A Leaf line with a trailing comment poisons mkpay's leaf: keep any note on its own line.
- (NF178b) VC7.1 emits a member's destructor chain (~_Rb_tree, clear, _M_erase) as pick-any COMDATs from a class's constructor even when
  nothing calls them (only .debug$F refers). Check every new defined symbol against reloc_bindings.tsv, not only the ones the leaf calls;
  removing one body (_M_erase) is not enough when the next one up (clear) also carries a retail name with a different body.
