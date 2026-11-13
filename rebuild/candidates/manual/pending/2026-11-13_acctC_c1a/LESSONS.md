- (agentC1) Retail keeps a TU's unreferenced inline-member COMDATs (the linker did not drop them): the `mov eax,[ecx+off] ; ret` runs
  next to a TU's functions list every inline getter the TU used, in HEADER DECLARATION order. Use them to prove or refute an inline
  helper (a used helper always gets a COMDAT under VC7.1) and to read the original declaration order. Ex.: 0x005CC58E..0x005CC685.
- (agentC1) VC7.1 numbers an object's inline COMDAT sections in class declaration order, not first-use order (FableWin's run too).
- (agentC1) A SIB base/index flip that dead outer locals and 1..24 discarded inline calls never move is a free-list effect inside one
  operand's inline expansion (an extra local in the callee body), not a record-count wrap.
- (agentC1) equiv_check.py on a copied include tree needs `<tree>/../src` to resolve (rows include "../src/compiled/..."): put the copy
  in a dir with a `src` symlink to rebuild/src, else every such row reports cpp-error-new.
