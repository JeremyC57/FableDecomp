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
