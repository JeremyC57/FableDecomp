# 0x00BE3970 -- device to confirm before landing (B24C5)
- CEdgeStipVertexArray<T>::operator[] with a release ASSERT dead arm (`FABLE_RETAIL_ASSERT(index < Count)`, fable_assert.h
  FABLE_RETAIL_ASSERT_DEAD_ARM), probe overlay c5/hdr/engine/CEdgeStipVertexArray.h. INFERRED from size only: FableWin.pdb's
  operator[] instances (const and non-const) are 73i/245b, which the plain body + one Lionhead ASSERT gives at /Od (B24C3 probe
  ../00be4660/c3/dbg: `index < Count` 73i/249b -- 4 bytes off, `index < Size()` 73i/251b); the condition text is not readable
  (FableWin.exe not on this machine). Graded effect: 35 -> 25 dd blocks on this row (body from 0x00BE3AC9 on now retail's);
  inert on both water twins (water z2 4 / gba1 16 dd blocks unchanged, sea gba1 still RELOCATION_MATCH, c3/ov_ea + c5 gbaea).
  Same kind of device as CArray.h's FABLE_RETAIL_CARRAY_INDEX_ASSERT (labelled INFERRED there).
- The row is not matched; nothing here is ready to land.
