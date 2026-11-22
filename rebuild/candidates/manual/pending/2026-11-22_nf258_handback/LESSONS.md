# NF258 lessons
- A stand-in container member is often the PDB's own type, not a bare vector: check the PDB layout first. CDefClassInfo::GlobalIndices
  and CVertexGroupBlock::Groups are CArray<unsigned long>, and the map behind 0x009D4810 is map<CCharString, CArray<unsigned long> >
  (both PDBs). The vector copy constructor binding is the same either way, but the self leaf and the callee spellings are not.
- A row that indexes a header container through any inline member (operator[], begin()) can pick up a dead `add reg, off` for the
  member's `this`, which shifts register allocation, where the HEAD stand-in read the field directly. Three spellings that all reach the
  same inline call cost three forms with identical output: try a different shape (the siblings' header port), not another operator.
- The catalog's Dinkumware `_Cons_val<...>` labels on unreferenced 31-byte rows are propagated across byte-identical shapes. Name each one
  from its callees' offsets (what is copied at +4 v +0xC) and the PDB construct helpers; the STLport name is `_Construct<T, T>`
  (0x007AE0C0 recipe, FABLE_RETAIL_STLP_CONSTRUCT_FASTCALL).
- A header class with an out-of-line destructor still makes VC7.1 emit its members' inline destructors (and the tree's `_M_erase` /
  `clear`) as unreferenced pick-any COMDATs when the implicit copy constructor is used. Compare them with a landed sibling's HEAD object
  (cmpobj) before calling them new; the existing FABLE_RETAIL_STLP_TREE_DTOR_DECL_ONLY (NF178b) removes them.
- Link control: duplicating an object whose leaf is an explicit instantiation (a COMDAT) gives no LNK4006; duplicate a row with a strong leaf.
- clang cannot parse VC7.1's own <cstdio> + <vector>: a precheck "error" in a test that includes only VC7.1 STL headers is that, not a
  missing include. Check with a two-line control file.
