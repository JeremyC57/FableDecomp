# ESCALATE 00f361d0 NFractal::CPerlinNoise::Get3DNoise -- DIFFER(873v873), 6 diff lines, scheduling only
Best source: differ/00f361d0.cpp (needs hdr/ overlay engine/CPerlinNoise.h, opt-in FABLE_RETAIL_CPERLINNOISE_GETNOISE).
Residue (retail v ours):
- 0x00F36301..0x00F3630D: SCurve(v0_y)'s two `fmul st(2)`: retail `lea esi,[edx+edi] ; mov eax,esi ; fmul st(2) ; and eax,0xff ; lea ; fmul st(2)`,
  ours issues the first fmul before `mov eax,esi` and the second before `and eax,0xff`.
- 0x00F3640F `fsub st(1)` (result_z0's outer Interp, b - a): retail before gradient001's `lea eax,[esi+1] ; and ; lea ; lea`, ours after them.
Levels tried (~300 grades, mostly sweeps): Ego_d locals (all 37) and PDB private inline helpers SCurve/Interp/LookupIndex/LookupGradient
(by value)/LookupScalar from Ego_d's CPerlinNoise class record; C3DVector::Dot opt-ins (default best); statement-block order (v0,v1,b,t best);
gradient/value interleavings; parenthesised lines; helper body groupings (SCurve 9 x Interp 6 x Lookup 12, constrained by the two sibling rows
0x00F36010 / 0x00F36540 which match). Right-assoc SCurve t*(t*(3-2t)) reaches 2 lines (only the fsub) but contradicts SCurve's Ego_d /Od
length (44 bytes = left-assoc t*t*(3-2t)).
Best guess at the original: same as ours; the residue is a c2 scheduling-region / priority tie; maybe a different Interp/SCurve
parameter type (double?) or value lines written as an inline at3-style helper not in the PDB.

## Round 2 (2026-11-19) -- coordinator questions answered

1. Right-assoc SCurve with float literals, graded on retail's own siblings:
   - `t*(t*(3.0f-(2.0f*t)))`: 0x00F36010 DIFFER(442v442), 0x00F36540 DIFFER(321v321), 0x00F361D0 878v873. OUT.
   - `t*((3.0f-(2.0f*t))*t)`: same three results. OUT.
   - `(3.0f-(2.0f*t))*t*t`: both siblings RELOCATION_MATCH, 0x00F361D0 873v873 with the SAME 6-line residue as the baseline
     `t*t*(3.0f-(2.0f*t))` (c1 canonicalises it to the same tree). Baseline kept.
2. Scheduler measurement (c2 DAG dump, tools/sched/t20.py = hand2a's t13 with every region dumped; sim.py reproduces region 1 exactly):
   - Get3DNoise's one basic block is cut into regions of 81 / 81 / 81 / 37 tuples. 0x269 = PAREN no-code tuples: R1 4, R2 6, R3 5, R4 12.
   - Site 2 (0x00F3640F fsub) is a REGION CUT, not a tie: ours cuts R2|R3 between `fadd st(2)` and `fsub st(1)`, so the fsub waits
     the fadd's 3-cycle latency and the lea chain fills it. Retail needs the cut 1 or 2 tuples later => retail has 1-2 FEWER tuples
     in the first 163 (R1+R2).
   - Site 1 (0x00F36303 / 0x00F3630D, SCurve(tx) fmuls) is in R1 (cycle 55: 0x269+fmul pair v `mov eax,esi`). Simulated: removing
     exactly ONE PAREN tuple from R1 (which extends R1 by gradient000's `fld [eax]`) reproduces retail's R1 order exactly ONLY when the
     removed tuple is #62 = SCurve(tx)'s inner `(2.0f*t)` PAREN; removing #64 / #70 / #72 does not. Removing #62 and #70 together
     plus one extra PAREN on `fild x_int` or `fild y_int` also reproduces it. => the deficit is in R1 (before site 1), not between the
     two sites; the between-site code (LookupGradient copies, Dot temporaries, inner Interps) is in R2 and cannot move site 1.
   - Confirmed by compile: header SCurve `t*t*(3.0f-2.0f*t)` (no inner paren): R1 loses 2 PARENs, R1 ends 2 later, R2 ends after
     `fadd fsub` (site 2 cut moves as predicted), site 1 fixed; but R2's start then differs and BOTH siblings break (442v442, 321v321).
   - No-inner-paren SCurve + `float v0_x = point.X - ((float)x_int);` (or `(point.X - x_int)`, or the y twins): BOTH original
     sites fixed; residue moves to R2's first instructions (`fld [eax+4]` v `lea edi,[edx+ebx]`) and R4/epilogue (tz's SCurve lost
     its PAREN too). Siblings still break with that header, so this is not a landable path as is.
3. Include order (owner rule 2026-11-19): six permutations of GFFloatToLongFloor.h / CPerlinNoise.h / C3DVector.h / C2DVector.h:
   all 873v873 with the identical 6-line residue. Expected: the residue is a tuple-count cut, not an ordinal tie.
Conclusion: retail's Get3DNoise has exactly one fewer no-code tuple than ours in its first 81 tuples (consistent with SCurve(tx)
lacking one PAREN), with a header that still matches both siblings. No source spelling found that removes a single PAREN from one
inlined SCurve call only. Row stays parked in differ/.

## Round 3 -- CLOSED (RELOCATION_MATCH)
Sim enumeration over PAREN placements in each fade (R1 count kept at 81): retail R1 order is reproduced by PARENs {after fsubr, after
fmul1} or {after fsubr, after fmul2} for every fade (uniform). Header spellings with those PARENs: `((3.0f-2.0f*t)*t)*t`,
`t*(t*(3.0f-2.0f*t))`, `t*((3.0f-2.0f*t)*t)`, `(t*t*(3.0f-2.0f*t))`, `((3.0f-2.0f*t)*t*t)` -- all keep both siblings matching and fix site 1.
Site 2 (R2|R3 cut) then closed by Interp `(a+(b-a)*t)` (one PAREN fewer per Interp): 0x00F361D0, 0x00F36010, 0x00F36540 all RELOCATION_MATCH.
Chosen: SCurve `(t*t*(3.0f-2.0f*t))` (FableWin's t * t * (3.0 - 2.0 * t) order, bracketed like Interp).
