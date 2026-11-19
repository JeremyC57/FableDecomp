# hand4f batch 4 (2026-11-19): 0x00554030, 0x005698A0, 0x004C5B90 -- all RELOCATION_MATCH + test PASS in the strict read-only vland
- The std::map operator[] family: the shared stlport/fable_map.h body with the existing opt-ins FABLE_RETAIL_STLP_MAP +
  _STLP_TREE_INSERT_UNIQUE_HINT_BODY + _STLP_MAP_ORIGINAL_INSERT_BODY (the landed 0x00553E60 set); explicit instantiation; owner classes
  from their shared headers. The parked local models were the residue. No flags, hdr/ empty.
- Names (NAME_FIXES.tsv): 0x005698A0 = map<long,CCountedPointer<CTexture> >::operator[] (catalog made-up _global::RBTree_SearchOrCreate);
  0x004C5B90 = map<void*,CTCGraphicAppearance::CMeshEffectPriorityEntry>::operator[] (both FableWin.pdb). vland WARN STANDIN_LEAF = catalog label.
- 0x004C5B90: defines CMeshEffectPriorityEntry's default ctor body `{}` (FableWin 0x01D07550 only constructs PEffect; header
  EFFECT_ENTRY_PASS23 declares it; retail inlines it) and names the protected entry through a derived-class typedef (VC7.1 checks access in
  explicit instantiations; no code). Coordinator: decide whether that `// Access:` device is acceptable.
- Tests: real STLport maps; a test-only no-rotation stand-in for the library's out-of-line _Rb_global<bool>::_Rebalance (0x0042971D).
- Hint insert_unique body graded RELOCATION_MATCH at its own copy 0x00553C50.
