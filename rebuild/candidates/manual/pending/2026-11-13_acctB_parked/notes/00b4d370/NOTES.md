## 0x00B4D370 progress (author 1)
- grades ~25: 6601 -> 7130v7176. Moved: explicit 4 corner blocks; PerspectiveTransformPoint = const x,y,z,w sums from point.X.. then point.X = x / w (FableWin 87i/241b SAME by fwsize); 26-loop as `C3DVector p = points[k]; local_to_base.TransformPoint(p); light_points[k] = p` with accumulate TransformPoint (FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ACCUMULATE); frustum aggregate initialiser.
- Did not move: SetInverse E41 operand order (canonicalised); PostScale/PostTranslate loops fine; ZYX TransformPoint worse than accumulate.
- grades ~60: best 6986v7176, sim noframe 1411/1760 (v/base.cpp, tools v/q.sh + v/sim.py + v/sd.py + v/bins.py). Moved: TransformPoint single-expression rows with X parenthesised (FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_XZY: X=E11*xx+(E21*yy+E31*zz)+E41, Y=E12*xx+E32*zz+E22*yy+E42, Z=E13*xx+E33*zz+E23*yy+E43) for BOTH the frustum and the 26-loop; view.SetTranslateOnly(C3DVector(0,0,0)); grid row pointer (`C3DVector* row=&points[1]; point=row; row+=5; *point++=p`).
- Did not move: PTP operand spellings (V8..V11 identical: canonicalised), weights u*v vs v*u, SetInverse det/E41 orders, SI_ORDER, /G5 /G7 /Op, declaring rot/q earlier. Open: retail emits `fld E ; fmul st(i)` (matrix element first) in every PTP product, ours `fld st(i) ; fmul E`; ours CSEs the whole corner w sum across passes, retail only the products.
- grades ~95: best DIFFER(6986v7176) noframe 1436/1760 (v/base.cpp = differ). Moved: else-branch `x_axis.X = ..; x_axis.Y = ..;` member stores (retail builds both then stores, no temp), half_extent C2DVector(0.5f*length, MSD) named + C2DVector::GetMagnitude FableWin text (FABLE_RETAIL_C2DVECTOR_MAGNITUDE_SQUARED_LOCAL, fwsize SAME 21i/52b).
- Did not move / worse: dir or axes from 3D difference (FABLE_RETAIL_C2DVECTOR_FROM3D) -> perturbs the frustum CSE (chaotic: ANY added statement, even `float p=0.0f;(void)p;` at the end, flips the frustum block d600-d800 to a worse shape; unused declarations do not).

## 0x00B4D370 hand-back (author 1, 2026-11-12) -- DIFFER(6986v7176), best source differ/00b4d370.cpp (= v/final.cpp)
Status: whole function written from OUTLINE/dis_all, every block, the 5 retail calls exactly (no extra calls). ~110 grades.
Best: DIFFER(6986v7176); masked instruction alignment (v/sim.py, esp offsets masked) 1436 of 1760 retail instructions equal.
Headers / flags: hdr/ overlay (README there); `// Flags: /d2inlT314` (PostMultiply / Initialise(3x4) expansion, as r3004).
What moved it (in order): explicit 4-corner blocks (a 2x2 loop is not unrolled); PerspectiveTransformPoint FableWin text (x,y,z,w sums from
point.X.., then point.X = x / w; fwsize SAME); the 26-loop as `C3DVector p = points[k]; local_to_base.TransformPoint(p); light_points[k] = p;`;
TransformPoint XZY rows (FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_XZY) for frustum + 26-loop; `view.SetTranslateOnly(C3DVector(0,0,0))`
(retail inverse translation = -(E31+E21+E11)*0 => the view copy's translation is zeroed); frustum aggregate initialiser; grid row pointer
(`C3DVector* row = &points[1]; point = row; row += 5; *point++ = p`, retail ebp/esi); `p *= MSD`; non-perspective branch: half_extent
C2DVector(0.5f*length, MSD).GetMagnitude() (FableWin GetMagnitude text) and `x_axis.X = ..; x_axis.Y = ..;` member stores; first x_axis built
straight from y_axis (C2DVector x_axis(y_axis.Y, -y_axis.X)).
Residue (exact):
 1. frame: ours sub esp,3B0h v retail 3D0h; retail has 6 more scratch slots between light_points (0x158) and points (0x2A8), puts the last
    PostMultiply result at [esp+100h] (local_to_base's slot) and light_vector below local_to_base; ours shares [esp+8Ch] for all three results.
 2. x87 operand order: retail loads the matrix element first (`fld E ; fmul st(i)`) in every PTP / PostTranslate / grid-weight product, ours
    `fld st(i) ; fmul E`; not moved by any operand spelling (PTP V8..V11, u*v v v*u: canonicalised), declaration order, /G5 /G7.
 3. corner passes (0x00B4DD1C..0x00B4E480): retail keeps the products X*0 / Y*0 / Z*0 separate ([d8] [ec] [20]), divides with fdivp in pass 1
    and re-sums w in pass 2; ours factors (Y+X)*0, stores w and reuses it in pass 2 (CSE of the whole sum).
 4. SetInverse det order (retail E31*c13 + E21*c12 + c11*E11, matrix element loaded first) and row 1 of the inverse translation not factored in
    ours ((E31+E21+E11)*0 in retail); CHAOTIC: any added statement anywhere (even `float p = 0.0f; (void)p;` at the end) flips the frustum
    block 0x00B4D600..0x00B4D8C4 into a worse shape (1436 -> ~1330), unused declarations do not -- tune it LAST.
 5. small: dir built as 3D difference would give retail's `fld;fsub;fld;fsub;fxch;fstp;fstp` (FABLE_RETAIL_C2DVECTOR_FROM3D) but trips (4);
    x_max/y_max -1e30 stores scheduled early in ours; PostTranslate(0.5) last column order; GetSquaredMagnitude Y-first in retail.
Tried without gain: TransformPoint ZYX / SUMS / PARTICLE_POINT / accumulate (accumulate stores frustum rows term by term); PTP E-form, temp
result C3DVector, reciprocal; SetInverse E41 operand order, element order (SI_ORDER), det spellings; /G5 /G7 /Op; probes of symbol count;
`q` declared at function scope (worse); rot declared first (no change); x_min init before PostScale (no change); weights v*u.
Best next ideas: (a) fix the frame first by finding the missing temporaries (6 floats near light_points; maybe light_points/points are
C3DVector arrays of a struct, or the grid sum is written with C3DVector temporaries `a*w0 + b*w1` held in named locals); (b) find the
PostScale / PostTranslate FableWin texts (fwsize 48i/140b, 60i/189b) -- they also feed the operand order; (c) for the corner CSE, try the
perspective pass as a separate helper-free block reading rot through a `const CMatrix4x4&` or copying q per pass from points[] instead of
light_points[]; (d) only then tune the frustum block (chaotic).
Tools left in v/: q.sh (grade + marker table), sim.py (masked equality count), sd.py (aligned side-by-side window), bins.py (per-0x100 residue),
fmap.py (retail->ours frame-slot map), pc.py (probe compile /FAs at the lane flags). Probes under probe/.
Name fix (from DOSSIER, re-checked against Ego_r/FableWin mangled names): landed caller 0x00B4F390 declares UpdateShadowSpace(CEngineCamera*);
PDB is `const CEngineCamera&` (?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z) -- same ABI.

## attempt 2 progress (2026-11-12)
- ~45 grades: best DIFFER(7162v7176), frame 0x3cc v 0x3d0, prologue + SetInverse nearly exact. Moved: C3DVector binary ops as `C3DVector result(...); return result;` (FABLE_RETAIL_C3DVECTOR_OPERATORS_RESULT_LOCAL: retail grid stores partial vectors to memory 0x00B4D970..; +176 bytes, frame +0x1c), C3DVector::GetMagnitude squared-local (fwsize SAME), SetTranslateOnly(0,0,0) float overload (FableWin 0x02D78EA0 in this TU's COMDAT cluster), `float start/end` locals for the t interpolation (retail keeps PerspectiveInterpStart in st).
- Did not move: PTP row orders/parenthesisation (canonicalised: X/Y/Z/W term orders give identical code), rot declaration position, renaming locals (names are irrelevant), identity-ctor rot (kills const-prop), `if(false) DrawFrustrumDebugScreen` (frame 0x3e0, worse). PTP `w` accumulate form separates X*0/Y*0 like retail but fwsize says FableWin PTP is the one-line form (91i v 87i) -- not kept.
- ~65 grades: still best DIFFER(7162v7176) = differ/00b4d370.cpp (noframe 1400; the noframe count UNDER-rates it: difflib misaligns the
  frustum block, bins.py shows 40/59/35-v-101 at d600..d800 = one shifted block; by line position ours/retail now agree within 5
  lines through 0x00B4E149). Final-loop operand order: `C3DVector q; q = light_points[k];` (decl then assign) gives retail's exact
  `fld E32 ; fmul st(1) ; fld E22 ; fmul st(3)` matrix-first order in the 2nd unrolled half, but the 1st half keeps q in memory
  (q1.cpp); member-wise copy (u2) gives a mixed order; `C3DVector q(light_points[k])` = default. So operand order follows how q's
  fields were defined (SRA temp ids), not declaration order or names (renaming rot: identical bytes).
- y2/h2: axes as C2DVector(light_points[a] - light_points[b]) (FABLE_RETAIL_C2DVECTOR_FROM3D, both sites): retail's `fld;lea;fsub;fld;fsub;fxch;fstp;fstp` exact at 0x00B4DCD1; noframe 1412 (best), size 7192 v 7176.

## 0x00B4D370 hand-back (author 2, 2026-11-12) -- DIFFER(7176v7176), best source differ/00b4d370.cpp (= a2/h3.cpp)
~80 grades this attempt (scratch in a2/, every grade in ATTEMPTS.log; a2/qq.sh = q.py + noframe count with an overlay, a2/hdr = experiment
overlay with PTP_*/X* macros -- NOT for landing; the real opt-ins are in hdr/, README updated).
Best: same LENGTH as retail (7176), 1752 v 1760 instructions, frame sub esp,3C0h v 3D0h. The noframe difflib count (1390) UNDER-rates it:
blocks now sit at retail's positions (e.g. grid, axes, ForegroundBufferSpan fdiv within 5 lines); do not use same= as the only judge.
What moved it (all kept, each with retail or FableWin evidence):
 1. C3DVector operator*(float) as `C3DVector result(...); return result;` (hdr C3DVector.h FABLE_RETAIL_C3DVECTOR_SCALE_RESULT_LOCAL) with the
    DEFAULT (RVO) operator+: retail's grid sum keeps each scaled vector in memory and builds the last + straight into p (0x00B4D95E..
    0x00B4DA90 now instruction-identical but for slots). Result-local + as well (OPERATORS_RESULT_LOCAL) copies the final sum (worse).
 2. Axes as `C2DVector y_axis(light_points[13] - light_points[0]);` and in the else branch `x_axis = C2DVector(light_points[15] -
    light_points[11]); y_axis = C2DVector(light_points[23] - light_points[3]);` (FABLE_RETAIL_C2DVECTOR_FROM3D): retail's
    `fld;lea;fsub;fld;fsub;fxch;fstp;fstp` exact.
 3. view.SetTranslateOnly(0.0f, 0.0f, 0.0f): FableWin ?SetTranslateOnly@CMatrix3x4@@QAEXMMM@Z 0x02D78EA0 sits in this TU's COMDAT cluster
    (hdr CMatrix3x4.h FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_FLOATS, fwsize SAME).
 4. `float start = PerspectiveInterpStart; float end = PerspectiveInterpEnd; t = (near_y - start) / (end - start);`: retail keeps start in
    st (`fld [2b0]; fxch; fsub st(1); fld [2b4]; fsub st(2)`), exact with a local.
 5. C3DVector::GetMagnitude `float squared=X*X+Y*Y+Z*Z; return sqrtf(squared);` (FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQUARED_LOCAL, fwsize SAME).
Residue (exact, h3):
 a. SetInverse (0x00B4D454..): c11 second product and det products in the other operand order (retail E23*E32, `fld E31 ; fmul st(2)`).
 b. inverse translation / frustum[0] block 0x00B4D5F7..0x00B4D8C4 (retail keeps (E11+E21+E31)*0 in st for frustum[0]).
 c. corner passes: retail CSEs the products X*0 / Y*0 / Z*0 ([d8] [ec] [20], computed first) and re-sums w in pass 2; ours factors
    (Y+X)*0 and CSEs the whole w. A `w` accumulate PTP text (float w=E44; w+=X*E14; ...) gives separate products but still CSEs w and
    fwsize says FableWin's PTP is the one-line form (91i v 87i) -- not the answer.
 d. PTP / PostTranslate operand order: retail loads the matrix element first. In the final 26-loop `C3DVector q; q = light_points[k];`
    gives retail's exact `fld E32 ; fmul st(1) ; fld E22 ; fmul st(3)` in one unrolled half (q1.cpp), memory q in the other.
 e. frame 0x3c0 v 0x3d0 (4 floats).
Tried without gain this attempt: PTP term orders / parentheses (canonicalised: identical bytes), PTP reciprocal form, rot declared earlier
(identical), renaming locals (identical: names irrelevant), identity-ctor or SetIdentity rot (kills constant propagation), `if (false)
DrawFrustrumDebugScreen(...)` (address-taken rot: frame 0x3e0), q declared outside loops / at function scope / decl+assign at every PTP site
(worse), result-local + and - (worse), member-wise q copy, grid sum parenthesisations / `C3DVector p; p = ...`, inline GetDeterminant
(FableWin 0x01B41130 follows SetInverse in its cluster, so SetInverse calls it; but every spelling breaks the cofactor CSE: 7016 bytes).
FableWin /Od notes: float-returning helpers differ from ours by a constant +2i/+8b (GetSquaredMagnitude x2, GetDeterminant 43-44i) -- a
named result local in FableWin's texts; C3DVector -,+,* are +6i over `return C3DVector(...)`, +2i over result-local; FableWin
SetInverse is 332i (ours 172-205i): it calls GetDeterminant and has much more text (asserts?) -- unexplained.
Best next ideas: (1) SetInverse's real text: GetDeterminant call kept in the TEXT but find the spelling that keeps det's cofactors CSE'd
with E11/E12/E13 (e.g. det from the already computed elements: `E11 = c11; E12 = ..; det = m.E11*E11 + m.E21*E12 + m.E31*E13;`
then scale by 1/det -- a two-pass SetInverse would also explain FableWin's 332i); (2) the final-loop q decl+assign idea combined with the
right block structure; (3) frame: find the 4 missing float slots (likely product temps of the corner passes once (c) is right).
- (late) two-pass SetInverse (first-row cofactors stored, det from them): DIFFER(7032v7176) -- breaks the translation/frustum CSE like GetDeterminant; not kept.

## 0x00B4D370 attempt 3 (2026-11-12) -- DIFFER(7160v7176), best source differ/00b4d370.cpp (= a3/f3.cpp), flags /d2inlT314, overlay hdr/
~35 grades (scratch a3/, each in ATTEMPTS.log; grade with `--flags /d2inlT314` -- fg.py does NOT read the `// Flags:` line: without it the
same source grades 6887v7176). Noframe 1495/1760 (attempt 2's h3 = 1390); frame sub esp,3B4h v 3D0h.
What moved it:
 1. Perspective scale pass (4 corner blocks): `C2DVector projected(q.X / (n + q.Y), q.Y / (n + q.Y));` then fabs(projected.X) / projected.Y
    (VC7.1 turns the two divisions into retail's one reciprocal). Retail computes py before the test and stores it ([esp+D0h]); with it
    the whole region 0x00B4DD00..0x00B4E300 (both corner passes, the x_axis temp at [78h]/[7Ch], the w product CSE) aligns instruction for
    instruction (masked) -- the old "corner-pass w CSE" residue is gone. `C2DVector projected(q.X * r, q.Y * r)` with r = 1/(n+q.Y) is the
    same; `C2DVector(q.X,q.Y) / (n+q.Y)` (operator/) and `* r` operator forms are worse.
 2. Final 26-loop as a pointer walk `C3DVector* point = light_points; for (k..; ++k, ++point) { C3DVector q = *point; ... }` (+9 noframe).
Tried without gain (do not repeat): PTP with Data[r][c] indexing (identical bytes); PTP products E-first `E1c*point.X` (identical:
operand order inside a product is canonicalised); PTP sums parenthesised `X + (Y + Z)` either operand order (breaks the corner passes;
the final loop gets Y,Z,X not retail's Z,Y,X); single `C3DVector q` declared before rot / before y_axis / after light_points (all three
identical, worse); corner copies made before the PostScale branch or before rot (copy or reference; operand order unchanged -- the
"stored most recently" rule does not flip it); final-loop `C3DVector q; q = *p;` (7210), member-ctor copy, in-place reference on
light_points (probe: then E-first appears, but stores back -- not retail); FABLE_FORCEINLINE -> plain inline for all shadow helpers
(identical); compiling with QFE 13.10.4035 (FABLE_VC71=$VC71_QFE: identical to RTM); else-branch axes as member stores / float ctors /
FROM3D mixes (e1..e6: local gains, global losses; retail stores x_axis straight to [18h]/[1Ch] keeping both in st, y via temp [24h]/[28h]
then copy to y_axis); non-perspective scale loop `light_points[k] *= scale` (fixes e700 locally, perturbs x_axis temp elsewhere); named
far-edge difference; min/max through C2DVector or a named y; C2DVector GetSquaredMagnitude local (fwsize 17i v 18i) / existing
FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX (fwsize 18i/44b v FableWin 18i/43b; release bytes identical here); PostTranslate/PostScale
texts t1 (row pointer / w local), t2 (`a = a + b*c`), t3 (j loop): fwsize none SAME (FableWin 60i/189b, 48i/140b; loops 59i/182b, 45i/134b).
Facts found: FableWin UpdateShadowSpace is 1202i/4922b at /Od, ours 1081i/4729b (+121i: asserts or more text -- unexplained).
FableWin COMDAT cluster after the TU (0x02D78C10..0x02D79A10): CArray<C3DVector>(unsigned), GFLimit<float,int,int>, render-state setters,
CPlane(3 vectors), CMatrix3x4 PreTranslate / SetTranslateOnly(MMM) / PostTranslate(MMM) / PostScale(C3DVector) / PostScale(MMM),
C3DBoundingBox::CalcFurthestDistanceToPointSquared, CMatrix4x4 ctor() / ctor(EMatrix34Init) / ctor(CMatrix3x4) / SetIdentity /
Initialise(3x4) / Initialise(16) / PostMultiply / PerspectiveTransformPoint / PostTranslate / PostScale.
Residue (exact, f3):
 a. operand order everywhere the retail loads the MATRIX element first (`fld E ; fmul st(i)` / `fld E ; fmul [pt]` / `fld E_i1 ; fadd p`):
    SetInverse cofactor products, corner pass 2 (E22*lp.Y), final 26-loop (also Z,Y,X sum order), PostTranslate rows; ours loads the
    point / product first. Not text-driven (see tried list); c2 candidate ids decide it (MATCHING_PLAYBOOK "higher id goes first") --
    needs a c2prio trace (hand892 tools/c2prio_op.py) rather than more spellings.
 b. SetInverse + frustum block 0x00B4D454..0x00B4D8C4 (chaotic, unchanged from attempt 2).
 c. else-branch axes 0x00B4E589..0x00B4E660 (x_axis kept in st and stored to x_axis directly), GetMagnitude of frustum[2]-frustum[1]
    sum order (retail X*X in place, then Z*Z, Y*Y), the 13-iteration scale loop starts at X in retail.
 d. scheduling: ours hoists the x_max/y_max -1e30 stores and `lea ecx,[lp+8]` into the PostScale(0.5) block (retail after PostTranslate);
    the PostTranslate/PostScale FableWin texts (+1i/+3i at /Od) may move that region cut.
 e. frame 0x3B4 v 0x3D0 (28 bytes; retail light_points at 0x158, ours 0x148; retail points at 0x298).
Best next ideas: (1) run the c2 candidate-id trace (hand892 tools: c2.sh / c2prio_op.py) on one PostTranslate row and one SetInverse
product to learn which source fact gives the matrix element the higher id (it is global: one fix likely flips SetInverse, pass 2, final
loop and PostTranslate together); (2) find the PostScale/PostTranslate FableWin texts (fwsize SAME) -- region cuts; (3) the else-branch
x_axis materialisation; (4) frustum block last.
