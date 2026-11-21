# 0x007602F0 NShape::IsShapeConvex -- AUTH5 log
## Step 0 (VC10 SP1, fablelib preset --stl, standalone step0/c*.cpp; Ego_d frame S_BPREL32 named locals reproduced exactly:
## pts -220, result -100, it_finish_him -96, it_first -84, last_line -72, this_line -60, second -48, it_second -36, first -24, loop this_line -112)
- Edges 309/317/328/338 (24/24/44/44) = `v = second - first; v.Normalise();` EXACT. Three-float ctor = 66/86 under VC10, `+ v.Z = 0` = 29:
  the old 41/58 rejection was VC7.1-measured, but it SURVIVES under VC10 (rests on size).
- Advisor lead (constant +5 on every point read): the Z clear sits on the READ lines. Exact: 307/308 `C3DVector first = *it_first++;
  first.Z = 0.0f;` 79/79, 315/316 `first = *it_first++; first.Z = 0.0f;` 87/87, 326 `first = pts.back(); first.Z = 0.0f;` 33,
  327 `second = pts.front(); second.Z = 0.0f;` 33, 336 `first = pts.front(); first.Z = 0.0f;` 33 (`*it_finish_him` form 30),
  337 `second = *++it_finish_him; second.Z = 0.0f;` 37; 334 `const_iterator it_finish_him = pts.begin();` 19; 324 `C3DVector this_line;` 8.
- 305 = `const_iterator it_second = it_first; it_second++;` 38 EXACT (`it_first + 1` 18, `pts.begin()+1` 66, `; ++it_second` 24).
- 319/330/340 = `result += GFArcCos(this_line.Dot(last_line));` 29 EXACT at all three (raw `(float)acos(..)*K` = 36).
- Still open: 313 100 (`while (it_second != pts.end())` 91; 6 other spellings 91..171), 342 122 (24 tail spellings 104..152, none 122).
## VC7.1 grades (q.py /Gr)
1-2. r0 (repo) / AUTH1 regrades known: 1126v1126 bracket class; literal op- 1181v1126.
3. z1p = Ego_d-decoded text (reads flattened, op- edges, GFArcCos x3, `*pts.begin()` for front): DIFFER(1126v1126) 12 lines (loop fmul
   order + epilogue pop/fadd = the old GFArcCos-everywhere residues; the [eax*4+IMM] lines are the masked table relocation).
4. z2p = z1p + FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST (p25Q overlay, hdr/engine/GFAngles.h): MATCH.
5. z3 = z2p with Ego_d's `pts.front()` (+ existing opt-in FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22 for const front()): MATCH.
6-8. final 007602f0.cpp MATCH; SIGNED_ROUNDING ablation SRC_FAIL (GFArcCos under it); vt.py 007602f0 PASS.
