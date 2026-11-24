# ESCALATE 0x00763060 NShape::FractureShape (1146 B) -- AUTH6, 2026-11-21 (owner question; every lever left breaks Ego_d)
## Sources
- Fitted match (leads): `$S/w22/auth6/00763060/00763060_fitted.cpp` (= n4) RELOCATION_MATCH /Gr. It is g1 (Ego_d) everywhere except:
    198/199  `const C3DVector& anchor = *it_anchor++; C3DVector b = anchor;`   (g1: `iterator it_start = it_anchor + 1; C3DVector b = *it_anchor;`)
    204+     `iterator it_start = it_anchor;` declared after it_begin, i.e. after the two bool stores (n5: before it_begin, also a match).
  `anchor` is a DESCRIPTIVE name (p25L's): Ego_d has no such local. No behaviour test yet: relocation targets (callees, constants) unchecked.
- Ego_d-exact (line sizes 195..241 AND named frame): `g1.cpp` (= AUTH5 g1) DIFFER(1134v1146), 78 residue lines (nd.py).
- Same deviation without the named reference: `n6.cpp` (`C3DVector b; b = *it_anchor++;`) and `n2.cpp` (it_start before the bools) 1146v1146, 2 lines.
- Older fitted family (superseded): `00763060_fitted.cpp` was k4 before n4; k4 / e5 change 195..204 (VC10 197/198/199 17/69/80 v 74/18/17).
## The deviation, measured (VC10 SP1 --lib fablelib --stl, AUTH5's step0 frame; v10/xn4.cpp)
| line | Ego_d | g1 | fitted (n4) |
|---|---|---|---|
| 195 | 16 | 16 `iterator it_anchor = begin()` | 16 same |
| 197 | 74 | 74 `C3DVector a = *it_anchor++` | 74 same |
| 198 | 18 | 18 `iterator it_start = it_anchor + 1` | 68 `const C3DVector& anchor = *it_anchor++` |
| 199 | 17 | 17 `C3DVector b = *it_anchor` | 12 `C3DVector b = anchor` |
| 204 | 16 | 16 `iterator it_begin = begin()` | 32 it_begin + `iterator it_start = it_anchor` |
Frame: g1 = Ego_d exactly. n4: + `anchor` (4 B reference, -28), every other named local at Ego_d's offset minus 4. All other 55 lines as g1
(56 of 60 exact; 280/283 1-14 B off and 288/291 = an else arm retail has no code for: debug-only, in every text).
Behaviour: g1 and n4 do the same (a = begin[0], b = begin[1], first search at begin+2, it_start++ after each call).
## Why no Ego_d-neutral lever remains
1. Register pair (begin ebp / zero ebx in g1): forced c2 readout (C2X_FORCE 10:1) not obtained, two timeouts (600 s, once-only 300 s).
   Measured instead: every text with retail's preheader (e5, k4, n2, n4) gets begin in ebx / zero in ebp as retail, so the pair follows
   the IL of lines 198/199; it is not an independent lever. The g1 residue is extra instructions (retail `lea eax,[ebx+0Ch] ; mov ecx,eax ;
   b via [ecx] ; add eax,0Ch ; mov esi,eax` + `lea esp,[esp]` pad) that g1 folds (b via [ebp+0Ch], it_start `lea esi,[ebp+18h]`).
2. Iterator model: TLC's are raw pointers (STLport 4.6.2 `typedef _Tp* iterator`; retail passes find_if's range in ecx/edx and steps
   `add esi,0Ch`). Ego_r is a VC10 release build (Dinkumware _Vector_iterator) and cannot show TLC's type. Closed.
3. Line 197: B's p25Q `it_anchor = duplicate_shape.begin() + 1` measures 69 (Ego_d 74) and its 198 `a = *it_begin` 17 (18): refuted.
4. Without a named reference every b read falls into one of two ILs (graded): copy-ctor `b = *it_anchor++` / `(const&)` / `*&*` / direct-init
   6 lines; assign `b; b = *it_anchor++` 2 lines. Only a NAMED reference to the post-incremented element gives retail's early pointer copy,
   and it_start must be copied after the bool stores (n2: 2 lines).
5. Retail needs b read through a copy of a pointer that is then incremented in place and copied to it_start. Ego_d's 198/199 compute
   it_start = it_anchor + 1 BEFORE reading b from it_anchor, so VC7.1 folds both addresses off begin.
## Question for the owner
Ego_d (and FableWin, same 2245 B) record 198/199 as `it_start = it_anchor + 1; b = *it_anchor;`, which VC7.1 cannot turn into retail's
preheader. Accept n4 as an Inferred TLC-revision difference confined to 198/199 + it_start's declaration point (VC10 68/12/32 v 18/17/16;
one descriptive reference local `anchor`, frame otherwise Ego_d's minus 4), or hold the row at g1 (78 lines)? A behaviour test is owed on acceptance.
