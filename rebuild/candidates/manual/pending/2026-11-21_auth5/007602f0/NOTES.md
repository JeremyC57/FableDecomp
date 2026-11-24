# 0x007602F0 NShape::IsShapeConvex -- byte MATCH, PENDING RULING on Ego_d lines 313 / 342 (AUTH5, 2026-11-21)
The bytes match and the test passes, but under the hard rule this is not a clean match until 313 and 342 are decoded or ruled on
(MATCHES_PENDING_RULING.tsv, not MATCHES.tsv).
Source `007602f0.cpp` (flags /Gr, overlay `../hdr` = hdr.diff), test `007602f0_test.cpp` (vt.py PASS, links the row object).
## Route
1. VC10 SP1 Step 0 (odall10.py --lib fablelib --stl, standalone text; tc_geometric_shapes.obj is a FableLib compiland, -Gr confirmed by
   the YI mangling): Ego_d's frame (S_BPREL32) reproduced exactly; 25 of 27 Ego_d lines byte-exact.
2. The decisive decode (advisor lead): every point read clears Z on the same line (`first = *it_first++; first.Z = 0.0f;` etc.), and the
   edges are Ego_d's `v = second - first; v.Normalise();` (24/44 exact; the three-float ctor is 66/86 under VC10, so the old bracket text
   stays rejected). Z is then a constant 0 and VC7.1 emits retail's two-square Normalise with `Z = 0.0f * scale`.
3. Also decoded exactly: 305 `it_second = it_first; it_second++;`, GFArcCos at all three acos sites (29 B; raw acos 36), closing edges
   `pts.back()` / `pts.front()` reads, closing2's `second = *++it_finish_him;` with it_finish_him = pts.begin() at 334.
4. VC7.1: that text = DIFFER(1126v1126) 12 lines (the two old GFArcCos residues); + PAREN_CAST = MATCH.
## PAREN_CAST contract check (ruling: every landed GFArcCos user regraded with it)
- 0x007C0690, 0x006AB820, 0x00852E10, 0x00A58130 regraded with FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST defined (overlay ../hdr): all
  MATCH, as do their unmodified copies (../gfreg/).
## Opt-ins
- FABLE_RETAIL_GFANGLES_ARCCOS_PAREN_CAST (overlay, `// Inferred:` in the source; ablation 12 lines). Fitted parentheses, /Od-neutral.
- FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER + _SQUARE_GROUPED_PASS23 (existing; ablation: neither 30 lines, HELPER only 15).
- FABLE_RETAIL_STLP_VECTOR_ORIGINAL_FRONT_PASS22 (existing STLport 4.6.2 const front(); Ego_d 327/336 decode as front(), 33 B).
- FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING (existing; GFArcCos is declared under it).
## Open (Ego_d lines this text does not reproduce under VC10)
- 313 `while (it_second != pts.end())`: Ego_d 100 B, this 91; 14 spellings tried (91..171), none 100.
- 342 `result -= 1.0f; return result * result < 0.0025f;`: Ego_d 122 B, this 110; 25 spellings tried (104..152), none 122.
  Retail matches these plain texts; the alternatives were measured in VC10 only, not graded. Neither line is one the earlier
  rejections rested on. Lead (advisor): Ego_d's loop S_BLOCK32 is 251 B but lines 315..322 sum to 250 (FractureShape's three blocks
  equal their line sums exactly), so the loop brace / construct at 313 differs from this text.
## Names
All names from Ego_d (locals pts, result, it_first, it_second, it_finish_him, first, second, last_line, this_line). No NAME_FIXES.
