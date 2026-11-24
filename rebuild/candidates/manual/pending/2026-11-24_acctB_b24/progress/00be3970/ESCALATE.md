# 0x00BE3970 -- device to confirm before landing (B24C5)
- CEdgeStipVertexArray<T>::operator[] with a release ASSERT dead arm (`FABLE_RETAIL_ASSERT(index < Count)`, fable_assert.h
  FABLE_RETAIL_ASSERT_DEAD_ARM), probe overlay c5/hdr/engine/CEdgeStipVertexArray.h. INFERRED from size only: FableWin.pdb's
  operator[] instances (const and non-const) are 73i/245b, which the plain body + one Lionhead ASSERT gives at /Od (B24C3 probe
  ../00be4660/c3/dbg: `index < Count` 73i/249b -- 4 bytes off, `index < Size()` 73i/251b); the condition text is not readable
  (FableWin.exe not on this machine). Graded effect: 35 -> 25 dd blocks on this row (body from 0x00BE3AC9 on now retail's);
  inert on both water twins (water z2 4 / gba1 16 dd blocks unchanged, sea gba1 still RELOCATION_MATCH, c3/ov_ea + c5 gbaea).
  Same kind of device as CArray.h's FABLE_RETAIL_CARRAY_INDEX_ASSERT (labelled INFERRED there).
- The row is not matched; nothing here is ready to land.
- (B24C6) The probe is now the real opt-in FABLE_RETAIL_CEDGESTIPVERTEXARRAY_INDEX_ASSERT in hdr/engine/CEdgeStipVertexArray.h (Inferred
  comment with the evidence); differ/00be3970.cpp (= c6/s1.cpp) uses it, byte-identical to the probe build. Still Inferred: not for
  this agent to land. Compile order: no effect in the three-body test (rest of the object untested): the row compiled in one TU before or after the
  water / sea S-form bodies is byte-identical to its one-row compile (c6/ord, ../00be4660/NOTES.md B24C6).
- (B24C11) Debug-build lookup DONE (owner's FableWin.exe): between `GetVert(i)` (0x02CBCE13) and the first GetMainAxisVertexWorldPos
  (0x02CBCE34) FableWin has only an unused `GetVert(i + 1)` reference -- no store, no call. Retail's loop-top shape (Data reloaded for
  start, [Data+t] operand order) is reproduced only by diagnostics that cannot land: a zero-byte `_ReadWriteBarrier()` (u3 / rf0, 4
  blocks: the start SIB pair is still t-first) or `__assume` / a volatile read (exact loop top, but the function-wide mode breaks
  FindVert and the push_back expansions). Nothing landable; the row stays DIFFER(2392v2390).
