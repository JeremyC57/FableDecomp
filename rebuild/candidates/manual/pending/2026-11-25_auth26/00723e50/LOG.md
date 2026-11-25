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
