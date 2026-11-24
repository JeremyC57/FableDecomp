# 0x007602F0 NShape::IsShapeConvex -- AUTH6 log (follow-up of AUTH5; base = $S/w22/auth5/007602f0/007602f0.cpp, VC10 base = auth5 step0/c3.cpp)
## Ego_d reads AUTH5 did not do (tools ../tools/eglo.py = egl.py with offsets, ../tools/locs2.py = locs.py with block offsets)
- Line table: one contiguous range per line. 313 @0x11B..0x17F (100 B), 342 @0x3C0..0x43A (122 B); total 1096.
- S_BLOCK32 tree: function scope (all 9 named locals) + ONE block @+0x179 len 251 holding only the loop `this_line` (-112).
- Our VC10 text (../tools/objblk.py on a /Z7 obj): same frame, block @+0x170 len 251. So Ego_d's block starts 9 B later and has the
  same length: the 9 extra bytes of 313 are INSIDE the loop test, before the `je` (the block starts at the je in both).
## Lifetime check (advisor lead)
- 342 in the plain text: three iterator dtor groups (4+3+5, 4+3+5, 7+3+5). A fourth group would be exactly +12, but a fourth live
  iterator needs a fourth 12 B object: Ego_d's symbol list has three named iterators and C++ temporaries die at the full expression.
  And frame: the $T temporaries sit above pts (-220) and tv slots below it, so pts = -220 in both pins the $T count; tv slots are free.
  -> the +12 / +9 must be frame-neutral code (tv slots), not object lifetime.
## VC10 measurements (vbatch.sh, 313 / 342 sizes; base 91 / 110)
- b1 a1..a5, b1..b3: a1 `return (float)(result * result) < 0.0025f;` 342 = 122 EXACT (fstp/fld through tv slot), pts -220. a2..a5 110; b1/b2 91; b3 146.
- b2 c1..c8: c3 `while ((it_second != pts.end()) != false)` 313 = 100 EXACT (movzx/neg/sbb/neg); c1 101, c2/c4/c6/c8 91, c5 278, c7 73.
- b3: 313 `!= 0` 100, `!!(..)` 100, `false != (..)` 100 (same IL); `== true` 104, `== 1` 104, `(x == end) == false` 101, `int(..)` 92.
  342 `float(..)` 122, `static_cast<float>(..)` 122, `(float)` on both sides 122; `(..) != false` 116.
- v1 = both: ALL 27 lines exact, total 1096, frame exact, block @+0x179 len 251 (= Ego_d).
## VC7.1 grades (q.py /Gr, overlay ../hdr = AUTH5's)
1. base (AUTH5) RELOCATION_MATCH. 2. g_w (313 only) RELOCATION_MATCH. 3. g_r (342 only) RELOCATION_MATCH. 4. g_wr (both) RELOCATION_MATCH.
5. final 007602f0.cpp RELOCATION_MATCH; vt.py 007602f0 PASS (row.obj from this source).
## Cross-checks (after advisor review)
- AUTH5's var/x4.cpp (`while (it_second != pts.end() != false)`, same text as c3 apart from that line) re-measured: 313 = 100. AUTH5's
  "14 spellings, none 100" was mis-recorded; the exact decode was in its own set. (w2 `!(it_second == pts.end())` = 101, as recorded.)
- Lifetime, measured: b3 `(const_iterator)it_second != pts.end()` adds one iterator temporary in the test and moves pts from -220 to -232
  (313 = 146). An extra iterator object costs a $T slot above pts, which Ego_d's pts = -220 excludes; the two decodes use tv slots only.
- Tool note: early 0x00763060 VC10 runs used AUTH5's step0/cmp.sh, which rewrote AUTH5's scratch step0/_a.txt / _b.txt (5 runs). Since then
  the copies in ../tools (cmp.sh, od.sh, lalign.py) write only under auth6.
