g0: baseline = Codex pass 26 final source (record 310v310, 96/98)
g1: FableWin 0x02526FA3 subtracts a DOUBLE 0.25 (fsub qword [0x0414C208] = 0.25) and converts on the float parameter: source is xy - 0.25 (double); the extra double->float node may delay fsub one slot (retail issues the normal-pointer load first)
g2: g0 without the PORTABLE_SEED_FIELDS memcpy device: the FableWin/Ego_d GFFastOneOverSqrt (locals n, seed TFloatIntUnion, r only; default ORIGINAL_UNION body). Extra memcpy locals change c2 symbol numbering
g3: GFFastOneOverSqrt with the Newton step as two statements (overlay opt-in NEWTON_TWO_STATEMENTS): same-TU landed GetRHSet 0x00723C80 needed exactly this to schedule mov 0x17c one instruction earlier; our 0x17c load is one slot late
g4: g0 with the natural unparenthesised heading argument (the parens were a fitted PAREN tuple, spell07)
results: g0 DIFFER 310v310 front residue only (retail issues `mov eax,[esp+1Ch]` before fsub and `mov edi,17Ch` before fcos; ours one slot later each)
         g1 (double 0.25) worse: retail's fsub is dword, so retail TLC used a float quarter turn (FableWin is a debug build whose fsub qword + fstp/fld round trip may be its compiler's float handling, not source text; g1 stands as a negative only)
         g2 (no memcpy seed device) front unchanged, tail frame-slot residue added
         g3 (two-statement Newton, overlay) front unchanged, one tail fmul swap
         g4 (no parens on the heading) front unchanged, same tail fmul swap as g3
--- advisor round 2 (base = g2; candidates only from the header spelling of Ego_d lines 453-454) ---
pre-registered:
 C1 (b, identity): CTCPhysicsStandard declares its own overrides GetFacingAngleXY() const (slot 63) and protected UpdateOldRHSet()
    (slot 95), declarations only (Ego_r + FableWin publics name both CTCPhysicsStandard overrides); today the calls bind the base
    CTCPhysicsBase declarations. Codex flush55-60 measured the overrides WITH visible bodies only.
 (a) ctor form: no candidate -- the header's `C3DVector() {}` (in-class, user-declared) is the only form consistent with the PDBs
    (??0C3DVector@@QAE@XZ exists, FableWin calls it, body empty) and C3DVector has other ctors, so it cannot be implicit.
 (b) return type float const matches the PDB; no inline wrapper exists in the header: no further candidate.
g5: C1 on g2
g5 result: identical to g2 (front unchanged, same tail) -> stop condition; TERMINAL.md; probe overlay deleted
--- owner round (2026-11-25): call-site grouping / ordering only, Ego_d locals exact (xy, right_2d, forward; n, seed, r). Base g2.
residue key: FRONT = mov eax / mov edi one slot late; TAIL = Normalise store block (fstp [esp+0Ch] first v retail fstp [esp+8]).
pre-registered: F1 `xy -= 0.25f;` then InitialiseFromAngles(xy, ..); F2 arg `-0.25f + xy`; F3 right_2d declared before xy;
  T1 SetUp(normal) before SetForward(forward); then combinations of whatever moves.
g6 = F1
g6 (F1) DIFFER 310v310: FRONT same; TAIL changed (different block, not retail)
g7 = F2
g7 (F2) DIFFER 310v310: FRONT same; TAIL = g6's tail
g8 = F3
g8 (F3) DIFFER 310v310: = g2 exactly (front + g2 tail)
g9 = T1
g9 (T1) DIFFER 310v310: FRONT same; TAIL much worse (retail stores forward first) -> rejected
g10 = F5: one statement `C3DVector forward = right_2d.InitialiseFromAngles(xy - 0.25f, 0.0f, 1.0f) ^ normal;` (InitialiseFromAngles returns const C3DVector&; operator^ is a member, so its argument `normal` is evaluated before the left operand's call -- puts the normal-reference load ahead of the angle chain). Ego_d lines 455+456 merge into one statement; locals unchanged
g10 (F5) DIFFER 310v310: = g2 exactly (front unchanged: the DAG, not the statement order, decides)
pre-registered: T2 `RHSet.SetForward(forward.Normalise());` (Normalise returns C3DVector&; lines 457+458 merged); F7 `C3DVector forward; forward = right_2d ^ normal;` (assignment instead of copy-init); F6 int literals 0 / 1 for angleyz / magnitude
g11 = T2
g11 (T2) DIFFER 310v310: = g2
g12 = F7
g12 (F7) DIFFER 340v310: much worse (temporary copied) -> rejected
g13 = F6
g13 (F6) DIFFER 310v310: = g2. Stop: every call-site regrouping / ordering leaves the front residue; TERMINAL.md owner-round section
--- owner round 3 (inlined callees; call site stays g2) ---
PDB: ?InitialiseFromAngles@C3DVector@@QAEABV1@MMM@Z (float by value x3, returns const C3DVector&); ?GFSin@@YINM@Z / ?GFCos@@YINM@Z
  (fastcall, float -> double). Landed matched users of InitialiseFromAngles: 0x006AB820 and 0x0068CB90 both use
  FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN (X = GFSin(a)*(cosYZ*length) ...); no landed row uses Codex's ANGLE_FORMALS_MAGNITUDE_PASS26.
g14 = I1: g2 with ANGLE_FORMALS_MAGNITUDE_PASS26 removed, so the landed INITFROMANGLES_GFSIN body applies
g14 (I1) DIFFER 310v310: identical to g2
g15 = I2: g14 without the row's own '#pragma intrinsic(sin,cos)' (GFSin.h / GFCos.h already declare the intrinsics; the landed 0x006AB820 / 0x0068CB90 have no row pragma)
g15 (I2) DIFFER 310v310: identical to g2
Normalise: landed users -- NORMALISE_SQUARE_HELPER 72 rows, of which only 5 add NORMALISE_GROUPED_SUM (extra parens around the sum, not in FableWin's body). g16 = N1: g2 without NORMALISE_GROUPED_SUM (the 67-row / FableWin spelling GFFastOneOverSqrt(GFSqr(X)+GFSqr(Y)+GFSqr(Z)))
g16 (N1) DIFFER 310v310: FRONT unchanged; TAIL = the g6 tail (not retail)
g17 = N2: g2 + GFFASTONEOVERSQRT_ORIGINAL_UNION_SQUARE_PAREN_PASS26 (Ego_d locals n/seed/r, union seed, '(r*r)*n'; landed matched user 0x0066C900) -- Codex g0's arithmetic without the memcpy bits local
g17 (N2) DIFFER 310v310: FRONT unchanged; TAIL reduced to one swap (retail: fld X ; lea edx,[esi+5Ch] ; fmul st(1) -- ours: fld ; fmul ; lea). New best legal base: g17
--- AUTH27 round 4 (2026-11-25) ---
Measurement (not a grade): c2 list-scheduler trace of g17 ($S/w24/hand2c/tools/sched/t20.py; trace in auth27/tr/g17.json).
 Region 1 = 81 tuples (function start .. the tail `fld X`), region 2 = the rest. 11 no-code 0x269 tuples in region 1.
 Front: cycle 8 = [0x269#9 (xy), fsub], cycle 9 = [fld 0.0, mov eax], cycle 10 = [fcos, mov edi]. Two picks a cycle, one per unit.
 Tail: region 2 starts at `fmul st(1)`; with ONE more tuple in region 1 the cut falls before `fld X` and the model gives retail's
 `fld ; lea ; fmul ; add ; pop ; fstp`. So front and tail are consistent with one extra tuple in retail before the fsub.
 Retail's front needs the first x87 node not ready in cycle 8: an x87-chain no-code tuple that waits for `mov eax,[normal]`
 (cycle 8 [#9], 9 [mov eax], 10 [X, fsub], 11 [fld, mov edi], 12 [fcos]) reproduces it.
 Landed twin 0x007B6810 (call ; mov ebp,[esp+34h] ; fsub) traced: there the load simply has the higher priority (short chain); not our case.
probe P1 (oracle, not a candidate): GetFacingAngleXY declared double -> DAG identical to g17 (a conversion leaves no tuple). Deleted.
probe P2 (oracle, not a candidate; adds a local, contradicts Ego_d): `C3DVector n(normal);` before InitialiseFromAngles, cross with n.
 Tests the model only: do early float reads through the normal pointer delay the fsub.
P2 result: DIFFER 342v310, the copy is real integer moves (not an x87-chain tuple); model test inconclusive. Deleted.
traces T3 / T4 (measurement only): g17 without the heading brackets; g17 with the call result used directly (no xy; contradicts Ego_d, not a candidate).
T3 (no heading brackets): tuple #11 disappears -> #11 is the bracket, #9 is the `xy` local. T4 (call result used directly): no #9; call and fsub share a cycle; emitted order = ours.
 So the front does not respond to the number of no-code tuples before the fsub (a second one in the chain gives cycle 8 [X], 9 [#9, fsub], 10 [fld, mov eax]: ours again).
g18 = g17 with GFFastOneOverSqrt = FABLE_RETAIL_MESH_DUMMY_INVERSE_STEP_PASS26 (Ego_d locals n / seed / r, union seed, `(3.0f - ((r * r) * n)) * r * 0.5f`;
 landed matched user 0x00A364E0) in place of ..._SQUARE_PAREN_PASS26. Evidence: the trace puts the region cut right after the tail `fld X`; one more
 bracket tuple inside region 1 moves the cut before it, which the scheduler model turns into retail's `fld ; lea ; fmul`. Prediction: TAIL closes, FRONT stays.
g18 result: DIFFER 310v310, TAIL closed as predicted; only the FRONT pair is left (mov eax,[esp+1Ch] / mov edi,17Ch one slot late). New best legal base: g18.
T5 (measurement only): `float xy = (GetFacingAngleXY());` -> a second no-code tuple in the chain; picks = cycle 8 [bracket], 9 [xy, fsub], 10 [fld, mov eax]:
 emitted front = ours, as the model predicts. Not graded (same front; the extra tuple only moves the cut like g18's bracket).
Priority readout (g17 trace): prio = depth + successor count (+32 for a memory operand); xy tuple 412, fsub 498, mov eax 147, mov edi 35. A no-code tuple is
 either high (its value feeds the chain) or dead (prio 2, e.g. #44). Hand simulation of every chain of high / dead no-code tuples before the fsub gives
 ours or `mov eax ; mov edi ; fsub`; never retail. Retail's order needs the x87 chain's first node to WAIT FOR `mov eax,[normal]` (or a one-cycle
 latency on the call result), which no tuple kind seen in any trace provides.
Stop: no evidence-backed front candidate left; TERMINAL.md round 4.
Corrections (after review):
- q.py calls this round: 3. (1) g17 re-check before any change: DIFFER 310v310, same three residues as AUTH26 recorded. (2) P2 oracle. (3) g18.
- P1 is UNVERIFIED: I did not confirm the scratch header copy was the one compiled (the probe dir is deleted). Nothing rests on it.
- Trace command (cwd = repo root, lane env sourced, PYTHONDONTWRITEBYTECODE=1):
    export C2PRIO_RUNS=$S/w22/auth27/c2runs FABLE_HDR_OVERLAY=$S/w22/auth27/hdr; T=$S/w24/hand2c/tools/sched
    timeout 900 python3 $T/host.py $T/t20.py <src.cpp> <out.json> '{"target":"<mangled-name substring>"}'
  Sources traced: 00723e50/g17.cpp; T3 = g17 with `InitialiseFromAngles(xy - 0.25f, 0.0f, 1.0f)`; T4 = g17 without the xy line and
  `InitialiseFromAngles(GetFacingAngleXY() - 0.25f, 0.0f, 1.0f)`; T5 = g17 with `float xy = (GetFacingAngleXY());`;
  rebuild/src/compiled/00/7b/CTCCrateStack_FillStack_007b6810.cpp (target FillStack);
  rebuild/src/compiled/00/79/CTCBulletTime_FrameUpdate_0079c880.cpp (target FrameUpdate@CTCBulletTime; trace kept as tr/bt.json).
- Second twin 0x0079C880 traced: the call is the second pick of its cycle (after the argument `fstp`), so the fmul on its result waits a cycle and
  the load (prio 66) goes ahead of it (63). A priority win on a short chain, like 0x007B6810; not our case.
- Repo check: no file under /home/user/FableDecomp was written by this run (the only recent .pyc files there are dated before it started).
