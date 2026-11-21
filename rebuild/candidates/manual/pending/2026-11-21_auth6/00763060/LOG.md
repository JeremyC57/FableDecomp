# 0x00763060 NShape::FractureShape -- AUTH6 log (follow-up of AUTH5; g1 / e5 / h1 copied from $S/w22/auth5/00763060)
## Brief 2: iterator model
- Ego_r is a VC10 RELEASE build (ghidra_out/egor_pdb_names.tsv): its FractureShape uses Dinkumware `_Vector_iterator` (class,
  _Iterator_base0, unchecked), and its out-of-line helper is `_Find_if<C3DVector*, CIsPointFinalLinkInTriangleWithMatchingCirculation>`
  (Ego_r 0x00774880: VC10 unwraps to raw pointers). Ego_r therefore cannot show TLC's iterator type; TLC's STLport 4.6.2 (non-debug)
  has `typedef _Tp* iterator` (repo stlport/fable_containers.h:636), the model g1 already uses. No class-iterator model exists for TLC:
  the iterator-model lever is closed.
## Brief 4: B's p25Q decode of 197 (VC10 --stl, AUTH5's step0 f6 frame; v10/q*.cpp)
- q1 195 it_begin=begin(); 197 `iterator it_anchor = duplicate_shape.begin() + 1;` 198 a=*it_begin; 199 b=*it_anchor; 204 it_start=it_anchor:
  197 = 69 (Ego_d 74), 198 = 17 (Ego_d 18). q2 (`it_anchor = it_begin + 1`) 197 = 18. q3 (+ b=*it_anchor++) 197 69, 199 74.
  -> B's decode is refuted by size at 197 AND 198; g1's (`it_anchor = begin()` 16 / `a = *it_anchor++` 74 / `it_start = it_anchor + 1` 18 /
  `b = *it_anchor` 17) is the only text with 195..204 exact and the named frame exact. Size selects; no ruling needed.
## Brief 3: minimal-deviation text = g1 with e5's 195..204 = e5 itself (g1 and e5 differ only at source lines 91-94 / 97 = Ego_d 195..204)
- VC7.1: DIFFER(1146v1146), 2 lines (AUTH5 grade 6; a.Z store `mov [esp+48h],edx` before `mov ecx,eax`; retail after).
- VC10 sizes of that region (v10/x5.cpp): 195 16/16, 197 17 v 74, 198 69 v 18, 199 90 v 17, 204 16/16 (region 208 B v Ego_d 141);
  frame it_begin -52 / found_appropriate -37 v Ego_d -48 / -49. Ego_d 288 (5 B) + 291 (197 B) = an else arm with code retail has none of:
  debug-only (all texts).
## Brief 1: C2X_FORCE readout on g1 (tools/c2prio_x.py, AUTH2's copy + FORCE_ONCE patch)
- g1 priorities (g1_prio.txt): begin = id 10 (`local`, lines 91,111,131,133, prio -63, split: ebp), const 0 = id 1 (prio -33, allowed .....b.
  only, split: ebx). c2 colours const 0 first and it may only take ebx, so begin lands in ebp: the swap is an allowed-set / order effect.
- Force 10:1 (every SORTED event): did not finish in 600 s (rc 124, no object). Retry with FORCE_ONCE: see below.
- Retry with C2X_FORCE_ONCE=1 (force applied at the first SORTED only): rc 124 again after 300 s, no object. Two failures: stopped.
  READOUT NOT OBTAINED (two timeouts). Measured instead: the pair is not an independent lever. Every text that changes source
  lines 93-97 to retail's preheader (e5, k4, n2, n4) comes out with begin in ebx and zero in ebp as retail; g1's swap follows its IL.
  (The `allowed .....b.` reading for const 0 is g1's state only, not a rule.) The preheader residue is extra instructions, not renamings.
## Brief 2 addendum: retail evidence for raw-pointer iterators
- Retail 0x007630F8..0x0076310B: find_if (0x00763F50) gets `mov ecx, esi` (first) and `mov edx, [esp+50h]` (last = the vector's finish
  word) in registers under __fastcall, its result is compared straight with the finish word (`cmp edi, eax`), and it_start++ is
  `add esi, 0Ch`. A class iterator passed by value would go on the stack: TLC's iterators are raw C3DVector* (STLport 4.6.2).
## VC7.1 grades (q.py /Gr; residue = nd.py residue_lines, AUTH5's metric: g1 78, e5 2). objgrade.py's own count (g1 134) is a different metric, unused.
1. e5 (regrade): DIFFER(1146v1146) 2.
2. k1 = p25L bracket region (it_anchor=it_begin+1; `const C3DVector& anchor=*it_anchor++; C3DVector b=anchor;`) on e5's body (Ego_d's
   207 find_if(it_start,..) / 210 it_start++ / 241 distance(it_begin,..)): RELOCATION_MATCH.
3. k2 e5 + `C3DVector b=(const C3DVector&)*it_anchor++;` 1137 / 6.   4. k3 e5 + `C3DVector b;b=(const C3DVector&)*it_anchor++;` 1146 / 2.
5. k4 = e5 with ONLY the named reference (`const C3DVector& anchor=*it_anchor++; C3DVector b=anchor;`, it_anchor=begin()+1 kept): RELOCATION_MATCH.
6. k5 k1 + `C3DVector b; b=anchor;` RELOCATION_MATCH.   7. k6 e5 + `C3DVector b(*it_anchor++);` 1137 / 6.
8. k7 `C3DVector b=*&*it_anchor++;` 1137 / 6.   9. k8 `C3DVector b;b=*&*it_anchor++;` 1146 / 2.
   -> without a named reference every form falls into e5's IL (2) or the copy-ctor IL (6): the one token separating e5 from retail is
   a NAMED reference to the post-incremented element. 10. 00763060_fitted.cpp (= k4 + header) RELOCATION_MATCH.
- VC10 of k4's region (v10/x4k.cpp): 195 16/16, 197 17 v 74, 198 69 v 18, 199 80 v 17, 204 16/16; frame gains `anchor` (-28) and shifts
  it_anchor -40 (Ego_d -36), it_begin -56 (-48), found_appropriate -41 (-49), b -104 (-100).
-> MATCHES_PENDING_RULING.tsv + ESCALATE.md (fitted match contradicts Ego_d at 197..199 and the frame). No behaviour test written yet.
## Bounded pass on g1 with source lines 91/92/97 untouched (advisor; AUTH1/AUTH5 logs hold no such grade)
11. n1 g1 93-94 -> `C3DVector b;b=*it_anchor++; iterator it_start=it_anchor;` 1137 / 6.  12. n2 same with `const C3DVector& anchor=*it_anchor++;
    C3DVector b=anchor;` 1146 / 2 (only `mov esi,eax` before the two bool stores; retail after).  13. n3 `C3DVector b=*it_anchor++;` + it_start 1134 / 78.
14. n4 = n2 with it_start declared AFTER it_begin (after the bools): RELOCATION_MATCH.  15. n5 it_start before it_begin (after the bools): RELOCATION_MATCH.
16. n6 = n4 in assign form (`C3DVector b;b=*it_anchor++;`): 1146 / 2.  17. 00763060_fitted.cpp (= n4 + header): RELOCATION_MATCH.
- VC10 of n4 (v10/xn4.cpp): 195 16/16, 197 74/74, 198 68 v 18, 199 12 v 17, 204 32 v 16 (it_begin + it_start on one line); every other
  line as g1. Frame: + `anchor` (4 B ref, -28); every other named local is Ego_d's offset minus 4 (it_anchor -40, it_begin -52,
  found_appropriate -53, it_start -88, b -104, it_result -116, quit -117). n4 supersedes k4 as the fitted source (k4 changed 195..204).
