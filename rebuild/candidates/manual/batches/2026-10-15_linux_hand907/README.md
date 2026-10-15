# Chunk 907 (skipped-row ledger: the five functions of catalogue row 00f36ca0)

- 00f36ca0 CDynamicObjectHeapBase::RefCountedRelease(): arm RefCountReleaseEnabled, then slot 1 (FreeAllBlocks) if AllocCount is 0.
  Twenty derived-heap Free() callers. MATCH.
- 00f36cc0 RefCountedReleaseAllHeaps(): walk the global heap list at 0x0143E964 and inline the body above on each heap. No callers.
- 00f36cf0 the scalar deleting destructor (`??_G`) of CDynamicObjectHeapBase, vtable 0x01301CD4 slot 0.
- 00f36d10 a two-word zeroing constructor with no callers; named SHeapCounterPair (descriptive).
- 00f36d20 a one-byte `ret`; named NoOp_00f36d20 (descriptive).
- The chunk 133 oracle still carried the 129-byte multi-row for 00f36ca0 and shadowed the split piece in q.py until it was removed.
