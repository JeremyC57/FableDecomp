## 0x00BAA870 RenderFromSortList -- author session 1 (2026-11-12)
- grades ~60: full draft from OUTLINE -> 4600-byte-class body; best v12 (work/v12.cpp, differ/): ~110 non-frame diff lines left + frame 0x12c v 0x124.
- moved it: transform rows `E1i*x + y*E2i + z*E3i` (x row) / natural (y,z); PostScale(x,y,z)+PostTranslate(x,y,z) FORCEINLINE opt-in (args R-to-L);
  NORMALISE_LOCALS; up = (E31*0 + E21 + E11*0, ...); p0/p1 rows `x + y + (z + w)` (X,Y) and plain (Z); blends `top*wb + side*wa` (operator+ arg first);
  control1_tex as a full vector; half_extent local + explicit sqrt(X*X+Z*Z+Y*Y); clamps `0.0f > g ? 0.0f : g` / `1.0f < h ? 1.0f : h`;
  bool faded = true; if (ShadowMode != 1) faded = false; + enable-first SetEnableFadedShadowFactor body opt-in; window as four float locals;
  CScopedShadersEnable dtor FORCEINLINE opt-in (retail expands it); GFFASTONEOVERSQRT_ORIGINAL_UNION.
- frame tool: work/pk/run.py (copy of agent3002 phook packer trace) `python3 pk/run.py v12.cpp v12 RenderFromSortList --nocmp` lists slots.
  retail slots bottom-up: [10..1c] scalars, [20] side/fade/textures, [2c] tex(48)+box/half_extent/temps, [5c] top/control0, [68] this,
  [6c] up/bottom_tex, [78] sphere, [88] other_side/control1, [94] shaders, [98] mesh/layout, [a0] side_tex, [ac] query, [b0] top_tex/control1_tex,
  [bc] control_tex, [c8] other_side_tex, [d4] inverse, [104] bone (frame 0x124). v12 has 0x12c: tex alone, inverse shares top/box, fade (esc) alone.
- BIG: frame 0x124 reached (work/v13.cpp = differ/): the same-TU callee GetTransitionFade (landed 0x00BAA460) put in the TU with its REAL body,
  __declspec(noinline) -> c2 sees the hidden result does not escape -> fade shares side's slot, inverse/bone apart (agent3002 packer rule).
  Needs hdr opt-ins CENGINEPRIMITIVERENDERERSHAREDDATA_ZSPRITE (Min/MaxZSpriteAsMeshDistance accessors) + CENGINEFADEDISTANCE_INLINE_CTOR.
  Remaining slot ORDER: retail [78] sphere,[88] other_side,[94] shaders,[98] mesh,[a0] side_tex,[ac] query,[b0] top_tex,[bc] control_tex,[c8] other_side_tex;
  ours other_side(w15) before sphere, mesh before shaders, query/top_tex/side_tex tie order, other_side_tex/control_tex swapped.
### Status at hand-back (session 1, ~115 grades): DIFFER(4575v4600), frame 0x124 equal, ~105 masked diff lines (work/dd.sh 1 1200 | grep -vc equal)
Residue list (retail line numbers of work/ret.txt; view with `cd work; ./gs.sh v13.cpp x; ./dd.sh 1 1200`):
 1. 217-237: x column E41 `fld [E41]; fmul st(1)` + pop (ours `fmul [E41]` consumes x); rep movsd of `inverse` after E12's fmul (ours after E22);
    `mov edx,17Ch` before E42 (ours after). Tried: inverse copy at 4 source positions (j1..j4: only moves the movsd), PostScale(const C3DVector&) (worse),
    E = s*E spellings (no change).
 2. 287-295: k stored `fstp [k]` then `fmul [k]` twice (ours `fst` + register); centre z add order (min+max). Both MATCHED in v12 (no
    CENGINEFADEDISTANCE_INLINE_CTOR) and flip with that define alone (i2.cpp) -> record-id effect, not spelling (MIN_FIRST toggle and include order no effect).
 3. 380-382: `mov esi,this` position (scheduling).
 4. 508-517: bbox copy int moves order (retail max.y, max.z, min.z interleaved with control1_tex's fmuls; ours min.z first) and control1_tex
    operand order (retail Y*scale; ours scale*Y). Tried: box before control1_tex / before top, const& box, no box (all worse or same).
 5. 631/646: const 3/4 z multiply consumes the reciprocal (`fmul [Z]`), ours keeps + pops; const 4/5 ecx/push order. Tried `* (1/r)`,
    accessors GetCenter/GetRadius (worse). In tf1 (top reused as control0, frame 0x130) const 3 and push 0xb matched -> also record/IL-order effects.
 6. 722: `push 0xb` before the pop of TLX (ours after). y1 (window by value + C2DBOXF_COPY + GetWidth forms) fixed it but moved BRX/TLX operands.
 7. 805-973: SetRenderState sites 2-5 load order (`mov esi,[edx+8]` first) and sites 6-8 `test eax,esi; or esi,eax` -- agent2059 lesson: record ids
    of earlier IL; EARLY_RETURN body made no difference.
 8. 1108: loop alignment pad (follows code size).
 9. Slot ORDER (not membership): other_side slot weight 15 > retail (sphere must sort before it), shaders/mesh, side_tex/query/top_tex,
    control_tex/other_side_tex tie orders (final qsort ties).
Best next ideas: (a) the record-id class (2, 5, 6, 7) probably needs the retail TU's earlier functions/inline instantiations: try the landed
    same-TU bodies that precede the row (IsInsideFrustum 0x00BAA4E0, CreateInternalPrimitive 0x00BAA820 ...) only if allowed, or write the
    remaining helpers exactly as FableWin lists them (TransformPoint(const C3DVector&, C3DVector&) for p0/p1 instead of the explicit rows,
    C3DBoundingBox::GetCenter/GetDimentions, C2DBoxF accessors); (b) `bool faded = true; if (ShadowMode != 1) faded = false;` is an odd
    spelling that matches -- look for the original (PeekGlobalShadowMode() == ... inside a helper); (c) for 1, try PostScale with the
    x column multiplying through a reference copy of the params.
Headers/flags: Flags none; overlay hdr/ (README lists 13 opt-ins); same-TU GetTransitionFade real body noinline in the row file (landed 0x00BAA460 text).
Names proven: StartOcclusionTest / EndOcclusionTest (NAME_FIXES.tsv); made-up local names only (no new function names).
