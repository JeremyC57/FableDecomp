# 0x00723E50 CTCPhysicsStandard::SetRHSetAsFlushOnNormal: TERMINAL (AUTH26, 2026-11-25; for the owner's decision)

Self: ?SetRHSetAsFlushOnNormal@CTCPhysicsStandard@@UAEXABVC3DVector@@@Z (Ego_r + FableWin publics; vtable slot 77).
Legal base (advisor Q1): g2.cpp = Codex pass 26's source without the PORTABLE_SEED_FIELDS memcpy device (its extra `bits` local
contradicts Ego_d's GFFastOneOverSqrt locals n / seed / r). g2: DIFFER(310v310) = the front residue + one tail frame-slot residue.
g0 (with the device) is an oracle only: front residue alone. Grades by AUTH26: 6 (g0..g5); Codex pass 26: 156+.

## The residue
Retail 0x00723E61..: call [edx+0FCh] ; mov eax,[esp+1Ch] ; fsub [0.25f] ; fld [0.0] ; mov edi,17Ch ; fcos
Ours:                call [edx+0FCh] ; fsub [0.25f] ; fld [0.0] ; mov eax,[esp+1Ch] ; fcos ; mov edi,17Ch

## Proof that it is one cause, located before the fsub
1. Codex's ready-list scores (PROBES.md line 51, native RTM trace of our build): fsub 507904, fld 450560, fcos 468992, the
   normal-pointer load 148480, mov edi 34816. Best-ready-first with the x87 chain eligible in slot 1 gives exactly OUR order; the
   same rule with the x87 chain eligible one slot later gives exactly RETAIL's order (pointer load in slot 1, mov edi filling the wait
   before fcos). So both moves are one event: in retail the first x87 op after the call is not eligible in the first slot.
2. Everything after the subtraction (cross product, Normalise, Newton step, seed, parentheses, literal widths) cannot change when the
   first x87 op becomes eligible; Codex's 156+ downstream respellings and AUTH26's g1 / g3 / g4 all leave the front unchanged, as
   this predicts.
3. Binary-wide scan of retail .text: an FP-returning indirect call followed by `mov r32,[esp+d8]` and then an x87 memory op occurs
   exactly ONCE in Fable.exe (this row, 0x00723E63); the "ours" shape (call, then the x87 memory op immediately) occurs 73 times,
   40+ of them inside landed rows. VC7.1 normally issues the x87 op first; retail's order here is unique, with no twin to copy.

## The window (Ego_d lines 453-454) and each candidate
- `float xy = GetFacingAngleXY();`:
  - return type: the header (CTCPhysicsBase.h:121 `virtual float GetFacingAngleXY() const;`) matches the PDB spelling
    ?GetFacingAngleXY@...@@UBEMXZ (float, const). No inline wrapper around the call exists in any header.
  - identity (candidate C1, g5): CTCPhysicsStandard's own overrides declared (?GetFacingAngleXY@CTCPhysicsStandard@@UBEMXZ,
    ?UpdateOldRHSet@CTCPhysicsStandard@@MAEXXZ, both PDBs) -- output identical to g2. With visible bodies: Codex flush55-60, same.
- `C3DVector right_2d;`: FableWin's ctor 0x0179318C is empty; ??0C3DVector@@QAE@XZ is user-declared (the class has other ctors,
  so it cannot be implicit); the header's in-class `C3DVector() {}` is the only form consistent with both. No candidate.
- The subtraction itself: g1 (double 0.25) emits `fsub qword`, retail `fsub dword`, so the literal is float.
No candidate backed by evidence remains in the window. A match found outside it would either contradict Ego_d (no `xy`, an extra
local) or be a fitted device, and would be reported, not landed.

## Files
g0.cpp (oracle), g2.cpp (legal base), g1/g3/g4/g5.cpp, LOG.md (every hypothesis), NOTES.md, ESCALATE.md (round 1).

## Owner round (2026-11-25): call-site grouping and ordering, Ego_d locals kept exactly
Constraint kept in every candidate: locals xy (float), right_2d, forward (C3DVector) and GFFastOneOverSqrt's n / seed / r only; no
added named locals, references or temporaries. Base g2 = FRONT (mov eax / mov edi one slot late) + TAIL (retail issues `add esi,50h`
and `pop edi` before the first normalised store, so it writes X*r back through [esp+8] after the pop; ours pops later and stores
through [esp+0Ch]). Each candidate was written in LOG.md before its grade; one source per q.py call.

| grade | candidate (call-site change) | Ego_d lines affected | result |
|---|---|---|---|
| g6 | `xy -= 0.25f;` then `InitialiseFromAngles(xy, 0.0f, 1.0f)` | 455 split in two | DIFFER 310v310: FRONT unchanged; TAIL changed to another non-retail order |
| g7 | argument `-0.25f + xy` | none | DIFFER 310v310: FRONT unchanged; TAIL = g6's |
| g8 | `C3DVector right_2d;` declared before `float xy = ...` | 453 <-> 454 swapped | DIFFER 310v310: identical to g2 |
| g9 | `RHSet.SetUp(normal);` before `RHSet.SetForward(forward);` | 458 <-> 459 swapped | DIFFER 310v310: FRONT unchanged; TAIL far worse (retail stores Forward first) |
| g10 | one statement `C3DVector forward = right_2d.InitialiseFromAngles((xy - 0.25f), 0.0f, 1.0f) ^ normal;` (operator^'s argument `normal` evaluated before the angle chain) | 455 + 456 merged | DIFFER 310v310: identical to g2 (front unchanged) |
| g11 | `RHSet.SetForward(forward.Normalise());` | 457 + 458 merged | DIFFER 310v310: identical to g2 |
| g12 | `C3DVector forward; forward = right_2d ^ normal;` | 456 split | DIFFER 340v310: the result is copied through a temporary; rejected |
| g13 | int literals `InitialiseFromAngles((xy - 0.25f), 0, 1)` | none | DIFFER 310v310: identical to g2 |

Earlier in this row (same constraint): g1 double quarter turn (front unchanged; literal proven float), g3 two-statement Newton
(front unchanged), g4 unparenthesised heading (front unchanged), g5 CTCPhysicsStandard override declarations (identical to g2).

Reading: no statement order or grouping at the call site moves the FRONT, because the VC7.1 scheduler orders by the dependence DAG,
not by source order (g8 / g10 / g11 are byte-identical to g2 though they reorder or merge Ego_d lines); the only spellings that move
anything (g6 / g7) move the TAIL, not to retail. The TAIL is the same kind of residue as the FRONT: integer instructions (`add esi`,
`pop edi`) that retail issues earlier among the x87 work. Grades by AUTH26 on this row: 14 (g0..g13). Codex's g0 closes the TAIL only
with the memcpy `bits` device (extra local, contradicts Ego_d), which this round did not reuse.

## Owner round 3 (2026-11-25): the inlined callees against landed matched users; call site kept as g2
| grade | check / candidate | evidence | result |
|---|---|---|---|
| - | InitialiseFromAngles signature | Ego_r ?InitialiseFromAngles@C3DVector@@QAEABV1@MMM@Z: three floats BY VALUE, returns const C3DVector&; GFSin / GFCos ?GFSin@@YINM@Z / ?GFCos@@YINM@Z (fastcall float -> double, sin / cos intrinsics; GFSin.h / GFCos.h `#pragma intrinsic`); FableWin 0x017F8708 body: cos_yz = GFCos(angleyz), X = (magnitude*cos_yz)*GFSin(anglexy), Y with GFCos, Z = magnitude*GFSin(angleyz) | header matches |
| g14 | InitialiseFromAngles body: the landed spelling FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN instead of Codex's ANGLE_FORMALS_MAGNITUDE_PASS26 (no landed user) | landed matched 0x006AB820, 0x0068CB90 | identical to g2 |
| g15 | no row-level `#pragma intrinsic(sin,cos)` (the landed users have none; the headers declare it) | same two rows | identical to g2 |
| - | CRightHandedSet::SetForward | FableWin 0x022163A0: struct copy into Forward (+0Ch) as three dword moves = header `Forward = forward` | header matches; nothing to grade |
| g16 | Normalise: NORMALISE_SQUARE_HELPER without NORMALISE_GROUPED_SUM (67 of 72 landed SQUARE_HELPER users; FableWin's body has no extra parens) | landed users; FableWin Normalise | FRONT unchanged; TAIL becomes another non-retail block |
| g17 | GFFastOneOverSqrt: FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION_SQUARE_PAREN_PASS26 (Ego_d locals n / seed / r exactly; `(r*r)*n`) | landed matched user 0x0066C900; Ego_d + FableWin 01ACE0A0 locals | FRONT unchanged; TAIL reduced to ONE swap: retail `fld X ; lea edx,[esi+5Ch] ; fmul st(1)`, ours `fld ; fmul ; lea` |
| - | TU flags | Ego_r module 45 `release\tc_physics_standard.obj` records no VC7.1 flags; the 70+ landed rows of 0x00723xxx-0x00724xxx carry no `// Flags:` override except 0x00724630 (/EHsc only) and no optimize pragma; GetRHSet 0x00723C80 (x87-heavy, same TU) matches at the default /O2 /Oy /W3 | evidence points to the default flags: no flag grade |

New best legal base: g17.cpp (Ego_d locals exact, every opt-in has a landed matched user): DIFFER(310v310) with the FRONT pair plus one
TAIL swap. All three remaining moves are the same kind: retail issues an integer instruction (`mov eax,[esp+1Ch]`, `mov edi,17Ch`,
`lea edx,[esi+5Ch]`) one slot ahead of a dependent x87 op that ours issues first. No call-site spelling (owner round 2) and no
landed-evidenced callee body (this round) moves the front; the evidence-backed flag setting is the default. Grades by AUTH26 on this
row: 18 (g0..g17).

## Round 4 (AUTH27, 2026-11-25): the owner's notes verified one by one, and the scheduler measured
Best: **g18.cpp, DIFFER(310v310), front pair only** (the tail is closed, Ego_d locals exact).
Opt-in evidence in g18: Newton body = landed matched user 0x00A364E0; InitialiseFromAngles body (ANGLE_FORMALS_MAGNITUDE_PASS26) has NO landed
user: it is backed by the FableWin debug body 0x01DCB530 (read again this round: cos_yz first, `magnitude * cos_yz` formed before each
trig call, Z = magnitude * GFSin(angleyz)) and Ego_d's formals; the row-level `#pragma intrinsic(sin,cos)` is removable (g15: identical).
q.py calls by AUTH27: 3 = the g17 re-check (reproduced AUTH26's DIFFER 310v310), the oracle probe P2 (contradicts Ego_d, deleted) and g18.
Scheduler traces (measurements, no grade): g17, T3, T4, T5, P1, landed 0x007B6810, landed 0x0079C880. Whether this readout is allowed was an
open question in ESCALATE.md (Q3); it is disclosed here, and nothing from it went into a source except the choice of g18's landed opt-in.

### What was measured (c2 list-scheduler trace, `$S/w24/hand2c/tools/sched/t20.py`; trace of g17 kept at `auth27/tr/g17.json`)
- The function is two scheduling regions. Region 1 is 81 tuples: function start up to and including the tail `fld X`. Eleven of them are
  no-code tuples (opcode 0x269). Measured: the `xy` local (gone in T4) and the heading bracket (gone in T3). Inferred from the count and
  their positions only: three cross-product brackets, three GFSqr brackets, the sum bracket, `(r * r)` and `(3.0f - ...)`.
- **Tail (closed).** g17's cut fell right after `fld X`, so `lea edx,[esi+5Ch]` (region 2) could not go ahead of `fmul st(1)`. One more
  tuple in region 1 moves the cut before `fld X`; region 2 then schedules `fld ; lea ; fmul ; add ; pop ; fstp` = retail. g18 does this
  with the landed Newton spelling `(3.0f - ((r * r) * n)) * r * 0.5f` (FABLE_RETAIL_MESH_DUMMY_INVERSE_STEP_PASS26, landed matched
  user 0x00A364E0; locals n / seed / r as Ego_d). Predicted before the grade, confirmed by it.
- **Front (open).** Two picks a cycle, one per unit (integer, x87). Ours: cycle 8 [xy tuple, fsub], 9 [fld 0.0, mov eax], 10 [fcos, mov edi].
  Retail's bytes need: [mov eax] alone, then [no-code tuple, fsub], then [fld, mov edi], then [fcos]. Priorities (depth + successors,
  +32 for a memory operand): xy tuple 412, fsub 498, mov eax 147, mov edi 35. So the load can never win on priority; retail's order
  needs the first x87 node to be NOT READY in the first cycle after the call, i.e. an edge from `mov eax,[normal]` into the x87 chain
  before the fsub (or a one-cycle latency on the call's result). No tuple in the traces listed above has either.
- Checked against the model: an extra no-code tuple in the chain (T5, brackets round the call) gives ours; the call result used
  directly (T4, no `xy`) gives ours; a dead no-code tuple (priority 2, by hand simulation) would let `mov edi` through ahead of the fsub.
  P1 (`double` return type in a scratch header copy) gave a DAG identical to g17, but I did not confirm the scratch header was the one
  compiled, so P1 is UNVERIFIED and nothing rests on it.
- The two landed twins with retail's shape, both traced: 0x007B6810 (`call ; mov ebp,[esp+34h] ; fsub`): its x87 chain is short, so the
  load outranks the no-code tuple (0x25000 v 0x1D800). 0x0079C880 (`call ; mov edx,[esp+18h] ; fmul`, result used directly): the call is
  the SECOND pick of its cycle (after the `fstp` that pushes its argument), so the fmul cannot share the call's cycle, and in the next
  cycle the load outranks it (66 v 63). Both are priority wins on short chains; here the chain carries three fsin / fcos (xy tuple 412 v
  load 147), so neither mechanism transfers. The other two shape hits (0x0079D7E0, 0x00B51570) were not traced.

### The owner's hypotheses
| # | hypothesis | verification | grade |
|---|---|---|---|
| 1.1 | GetFacingAngleXY const / virtual / return type | Ego_r + FableWin: `?GetFacingAngleXY@CTCPhysicsStandard@@UBEMXZ` = virtual, const, float; header CTCPhysicsBase.h:121 agrees. FableWin 0x02526F81 calls it through the vtable. | settled, none (g5 earlier: identical) |
| 1.2 | operator^ member v free, argument order | FableWin `??TC3DVector@@QBE?BV0@ABV0@@Z` (0x01B40A20): const member, by const reference, returns const C3DVector; header agrees. FableWin 0x02526FBB..C6 evaluates it as a separate statement after InitialiseFromAngles. The scheduler orders by the DAG: the argument order adds no edge into the x87 chain. | settled (g10 earlier: identical) |
| 1.3 | SetUp return type / RHSet through a getter / CSE of &normal | FableWin `?SetUp@CRightHandedSet@@QAEXABVC3DVector@@@Z`, `?SetForward@...@@QAEXABVC3DVector@@@Z`: both void; FableWin 0x02526FD7..EC reaches RHSet with `add ecx,50h`, no getter call. In the trace `mov eax,[normal]` is ONE node feeding the six cross-product reads and the SetUp copy already; its priority (147) comes from its first reader, which is after the three trig ops. | settled, none |
| 2a | the tail follows from the front | Half right: the tail is a region cut one tuple early, independent of the front's order. | g18: tail closed |
| 2b | C3DVector copy ctor / operator= changes the SetForward / SetUp stores | FableWin has `??0C3DVector@@QAE@ABV0@@Z` (user copy ctor) and no `??4C3DVector`: assignment is the implicit memberwise one; SetForward 0x022163A0 / SetUp 0x02216370 are three dword moves. The header matches, and after g18 the store block is byte-equal to retail. | settled by g18 |

### State
The residue is the two front moves only, and they are one event: retail's x87 chain starts one cycle after the call. Reaching it
needs a dependence of the chain's first node on the `normal` pointer load, which neither Ego_d's statements (lines 453-455 never
touch `normal`; FableWin's debug code confirms the order) nor any landed inline body provides. A fitted device was not tried.
Two bases, for the next reader: g18's extra Newton bracket and a front fix that itself adds a region-1 tuple exclude each other (T5: a tuple
added at the front also moves the cut, regions 83 / 28). Use g17 as the base for a front fix that adds a tuple, g18 for one that adds none.
Files: g18.cpp (best legal base), g17.cpp, LOG.md (round 4, with the trace command), STATUS.tsv, and the traces auth27/tr/g17.json, t3.json,
t4.json, t5.json, bt.json (0x0079C880) with the readers tr/show.py / show2.py. The 0x007B6810 and P1 traces were deleted for disk; the
command in LOG.md reproduces them.

## Owner decision (2026-11-25): set aside
The owner set row 2 aside after round 4 ("Set aside, start row 3"). g18.cpp is the best rule-compliant base, 2 instructions off at the front (`mov eax,[esp+1Ch]` / `mov edi,17Ch` one slot late). The row stays parked in UNMATCHED_ROWS.tsv. A later attempt starts from g18 (no tuple added at the front) or g17 (a front fix that adds a tuple), per the State section above.
