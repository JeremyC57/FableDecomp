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
- (B24C12) MECHANISM of the loop-top residue (c2 traces, lane files c12/lk_*.json, lka_n0_nu2.txt, sl/rf0.json): retail's form needs
  vert0's inlined operator[] `_this` (inline symbol s294, the 2nd `_this` of the GetVert expansion: symlog sequence `30:_this 293:_index
  294:_this 295:_index`) to stay UNFORWARDED, so vert0's Data load IND(s294) is a different CSE expression from start's
  IND(strip+8): start's two arm loads then get their own temp, numbered after t in the post-CSE batch (0x107615BC) -> [Data+t] (retail),
  and no Data temp spans vert0 (second load, retail). Ours forwards s294 into strip: the CSE round at n0 sites 2913..2938 (L15) replaces
  vert0's Data by the forwarded temp t143 (key 01030158, wins on count over n*20) -> one Data load, [t+Data].
  Reproduced ONLY by a zero-code statement between vert0 and start that c2 treats as a CSE-region split: `__assume(<anything>)`, even
  `__assume(1)` (c12/na1..na5, nu2: loop top 0x00BE3A50..0x00BE3AA5 exactly retail's) -- but __assume is function-wide (it also stops
  forwarding at FindVert L11, lka base 2939..2969 deleted, and in the push_back expansions: 33 blocks). Volatile reads do the same with a
  load. NOT region splits (inert, 25): do{}while(0), if(0){}, while(0){}, for(;0;){}, a label, a dead arm assigning strip (c12/nz1..nz6);
  40 dead-arm asserts (m40 / n40); class-typed temporaries in the operator[] dead arm (eh1: 135 / 104 with /EHsc); /EHsc or /GX alone.
  _ReadWriteBarrier kills but keeps ONE Data temp redefined after it (t-first by construction). The debug build (FableWin) shows nothing
  between vert0 and start except the unused vert1 reference. No evidenced construct gives the split: the row stays DIFFER(2392v2390).
  Owner question: is there any evidence of Lionhead's release ASSERT or a per-file pragma in Engine_landscape_tesselator.cpp (TLC)?
- (B24C12, corrections after review) The effect is POSITIONAL: the same `__assume(strip != 0)` placed before vert0 (c12/nu1) gives no
  reload; between vert0 and start (nu2, na1..na5) it gives retail's loop top. The unforwarded inline `_this` s294 at vert0's add is a
  SYMPTOM of the two Data loads not being paired by CSE, not a lever. Which inlinee s294 belongs to (operator[] under GetVert) is a
  READING of the symlog allocation order, not a name on the node. The 33-block collateral of na1 is not the operator[] dead arm
  compensating: without FABLE_RETAIL_CEDGESTIPVERTEXARRAY_INDEX_ASSERT both n0 and na1 are 35 (c12/n0x, na1x; na1x's loop top is not
  retail's either). Next step if anyone continues: dump the IL BEFORE CSE (not at lowering) for n0 v na1 and see what __assume(1) leaves
  between the two expansions, then look for an ordinary construct that leaves the same thing.
