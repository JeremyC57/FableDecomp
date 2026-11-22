# 0x00B4D370 LOG (AUTH20; continues AUTH19's LOG in $S/w22/auth19/00b4d370/LOG.md)
- a0 /Od (VC10 odall10 engine preset, lc.py vs FableWin): p7's remaining lines 532/546/590/644 + loop order 408..412.
  - FW 408:18 409:18 411:16 412:16 = weightx0, weighty0, (blank), weightx1, weighty1 (p7 had x0,x1,y0,y1: an `insert` group). Reordered: matches.
  - FW 546:16 = `1.0f + perspective_shift` (fld ps; fld1; faddp; fstp). p7's `perspective_shift + 1.0f` is 18 (fadd qword). Matches.
  - FableWin ?GetMagnitude@C2DVector (52 b) and ?GetSquaredMagnitude (43 b) list NO local but `this`: the MAGNITUDE_SQUARED_LOCAL text (named
    `squared`) contradicts FW; the default `sqrtf(X*X+Y*Y)` is 52 b = FW. Dropped FABLE_RETAIL_C2DVECTOR_MAGNITUDE_SQUARED_LOCAL.
  - 532 (48 v 36): 48 = `Y * max / fabs(X)` (spill across the fabs call), but retail 0x00B4E1FB computes Y/|X| first then *max = p7's text
    (corners region exact). Kept p7's text: FW is the later editor revision there.
  - 590 diag (96 v 68): 7 more C2DVector spellings measured 74..112, none 96. Kept.
  - 644 (39 v 33): not resolved. 652/654: absent from retail (nothing after CalcClipVolume at 0x00B4EF66).
  - /RTC1 changes every line (worse): FW's systematic loop +6/+9 is not /RTC1.
- g1 pre-reg p8 = p7 + the three /Od-evidenced changes above (weights order, `1.0f + perspective_shift`, no C2DVECTOR_MAGNITUDE_SQUARED_LOCAL).
  Expectation: diag (0x00B4E74A) and else GetSquaredMagnitude may turn Y-first like UpdateGoal 0x00A1E030.
- g1 result: p8 DIFFER(7162v7176) nm2 149, byte-identical to p7 (regraded in a fresh work dir QT=_a20; control best.cpp = p5 7176v7176 nm2 110 reproduces).
- g2 pre-reg p9 = p8 + line 532 FW form `Y * max_projected_range_x / fabs(X)` (/Od 48 = FW). Adopt iff corners stays 0+0.
- g3 probe pm1 = p9 + FW-shaped PostMultiply (matrix/temp_matrix/long v,u, braced; probe overlay ovpm, not a candidate).
- g2 result: p9 DIFFER(7162v7176) nm2 147 (persp/else 14+13 -> 13+12; corners 0+0 unchanged: VC7.1 emits the same corner code for both
  spellings, and the text change only flips one later order site in the PostMultiply(perspective_transform) expansion, 0x00B4E556).
  Adopted: best_evidence.cpp = p9 (FW-faithful at 532, 546, 408..412; GetMagnitude default).
- g3 probe pm1 (FW-shaped PostMultiply: matrix / temp_matrix / long v,u, braced; /Od 11,8,28,24,112,5,5,13,8 = FW except loop heads):
  byte-identical to p9. Not a candidate.
- /Od 644: 39 B = the 3x4 read from a `this` member at a 32-bit offset (CameraViewMatrix stand-in measures 39; a frame local 33).
  Retail reads the local's home ([esp+0x100] / [esp+0x124], 0x00B4EDC6..). So FableWin's tail converts a member that retail's text does not
  have (later editor revision).
- Loop heads: long/int/unsigned/++/+=/<= forms are all 36; short 41, char 35. None is FW's 42: the +6/+9 is a build difference, not text.
- Stalled: p7 -> p8 -> p9 = nm2 149 -> 149 -> 147, all remaining sites are x87 operand-order picks inside inline expansions plus one
  stack-slot placement. The c2 lk comparator tools (lkb2 / lka / fsolve, acctC wfE6 scratch) are not on this lane. ESCALATE.md written.
