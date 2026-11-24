# ESCALATE 0x00763060 NShape::FractureShape (1146 B) -- AUTH5, 2026-11-21 (stall: every idea left changes an Ego_d-exact line)
## Best sources
- Ego_d-exact in line sizes AND named frame: `$S/w22/auth5/00763060/g1.cpp` = DIFFER(1134v1146), 78 nd.py lines, /Gr (advisor's role decode).
- Ego_d line sizes exact but frame NOT (it_begin -52 / found_appropriate -37 v Ego_d -48 / -49): `h1.cpp` = DIFFER(1136v1146), 37 lines.
- Fitted reference (rejected texts, kept as evidence only): `e5.cpp` = DIFFER(1146v1146), 2 lines (a before it_anchor, `C3DVector b; b=*it_anchor++;`).
## What VC10 settled (odall10 --lib fablelib --stl, standalone step0/f5.cpp; see LOG.md)
- 56 of 60 Ego_d lines byte-exact with either role assignment; only g1's matches the frame (step0/f6.cpp: every named local at Ego_d's offset):
  g1: 195 `iterator it_anchor = duplicate_shape.begin();` 16 | 197 `C3DVector a = *it_anchor++;` 74 | 198 `iterator it_start = it_anchor + 1;` 18 |
  199 `C3DVector b = *it_anchor;` 17 | 204 `iterator it_begin = duplicate_shape.begin();` 16 | 241 `std::distance(it_begin, it_result)` 92.
  (h1, frame-inexact: 195 it_begin, 197 `a = *it_begin++`, 198 `it_anchor = it_begin + 1`, 199 `b = *it_begin`, 204 `it_start = it_anchor`.)
- Common to both: 207 `it_result = std::find_if(it_start, end(), F(a,b,circ));` 168 |
  210 `it_start++;` 28 | 241 `long distance = std::distance(duplicate_shape.begin(), it_result);` 92 | found block: iterator decls at 252/253,
  `it_del_start = splice_one.begin(); ++it_del_start;` 84, `it_del_end = splice_one.begin(); it_del_end += distance;` 88,
  `it_del_start = splice_two.begin(); it_del_start += distance + 1;` 91, push_back(*it_result) 20.
- So the p25L/p25Q/AUTH1 "199/204 leave no room for the post-increment" rejection rested on attribution and does NOT survive: Ego_d
  increments after the call (line 210), and the Ego_d text behaves exactly like retail (b = begin[1], first search at begin+2).
- Not exact: 280 (143 v 142), 283 (25 v 11, `else if (circ == ...)`), 288 5 + 291 197 (an else arm retail has no code for: debug-only).
## Remaining diff (g1 and h1 v retail; g1 adds more register/ordering lines after the loop)
- Preheader: retail `lea eax,[ebx+0Ch] ; mov ecx,eax ; b via [ecx] ; add eax,0Ch ; mov esi,eax` and `lea esp,[esp]` at the loop head;
  ours folds b's read to [ebp+0Ch..] and builds it_anchor as `lea eax,[ebp+18h]`, no alignment (needs 10 B, retail pads 7).
- Then a global register swap: begin in ebp / zero in ebx (retail ebx / ebp), 30 of the 37 lines.
## Tried on h1 (all 1136v1146 / 37 unless noted): distance as `it_result - begin()` or a by-value template; b before it_anchor (77);
`*(it_begin++)` / direct-init; `1 + it_begin`; `b=*it_begin++`; C3DVector copy ctor ORIGINAL_COPY_FORMAL / CTCTARGETINGAI_ATTACK body /
CTOR_FORCEINLINE; implicit copy (LIBPK, 1197/129); const a / b. VC10 rules out `C3DVector a; a=*it_begin++;` (90) and `1+it_begin` (49).
## Hypothesis
Retail's IL had b read through a pointer copy whose original is then incremented in place (exactly what `b = *it_anchor++` gives, e5);
under Ego_d's text VC7.1 sees it_begin = begin + 12 as a foldable address and never materialises it. Either the TLC text differs from
Ego_d at 197..199 (size-neutral at /Od? not provable), or a header model (STLport 4.6.2 vector iterator / C3DVector copy ctor) differs.
- g1 (frame-exact): same preheader fold (b read [ebp+0Ch], it_start `lea esi,[ebp+18h]`), no alignment, begin in ebp; 78 lines.
## Question for the advisor
Is there an Ego_d-neutral lever (header model, c2 symbol/priority probe on the g1 text) worth a bounded pass, or does the row take the
e5 family (it contradicts g1 at 195..204) as an Inferred TLC-revision text at 197..199 (with h1's 207/210/241/252..268 spellings, which are retail-neutral)?
