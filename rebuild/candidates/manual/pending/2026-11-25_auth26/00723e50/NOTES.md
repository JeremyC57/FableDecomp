# 0x00723E50 CTCPhysicsStandard::SetRHSetAsFlushOnNormal (AUTH26, row 2) -- working notes

Self name: ?SetRHSetAsFlushOnNormal@CTCPhysicsStandard@@UAEXABVC3DVector@@@Z (Ego_r.pdb and FableWin.pdb publics; vtable slot 77,
the catalog's `_global::CCharacter_UpdateOrientation` is a made-up label). Callees: virtual slots 95 UpdateOldRHSet and 63
GetFacingAngleXY (`?GetFacingAngleXY@CTCPhysicsStandard@@UBEMXZ`: float, const -- header CTCPhysicsBase.h:121 agrees).

## State
- g0 (Codex pass 26 final): DIFFER(310v310), front residue only.
- g0 is probably NOT a legal base: its clean tail comes from FABLE_RETAIL_GFFASTONEOVERSQRT_PORTABLE_SEED_FIELDS_PASS26, a memcpy
  device with an extra `bits` local that Ego_d's GFFastOneOverSqrt (n, seed TFloatIntUnion, r) does not list. g2 (the Ego_d helper,
  plain ORIGINAL_UNION body) = front residue + one tail frame-slot residue (PROBES.md already records this pair).
- g1 (double 0.25), g3 (two-statement Newton, overlay; overlay since deleted), g4 (no parens on the heading): front unchanged.

## Front residue: one cause, before the fsub (advisor diagnosis, 2026-11-25)
Retail: call [edx+0FCh] ; mov eax,[esp+1Ch] (normal) ; fsub 0.25f ; fld 0.0 ; mov edi,17Ch ; fcos.
Ours:   call [edx+0FCh] ; fsub ; fld ; mov eax,[esp+1Ch] ; fcos ; mov edi,17Ch.
With Codex's ready-list scores (fsub 507904, fld 450560, fcos 468992, pointer load 148480, mov edi 34816), best-ready-first gives
OUR order when the x87 chain is eligible in the first slot after the call, and RETAIL's order exactly when the x87 chain starts one
slot later (the pointer load takes slot 1, mov edi fills the wait before fcos). So both moves are one cause: in retail the x87 chain
is not eligible in the first slot. Nothing downstream (cross product, Normalise, Newton step, seed, parens, literal width) can affect
that, which fits the 156+ Codex grades that all regrouped downstream.
What lies between the call and the subtraction (Ego_d lines 453-454): `float xy = GetFacingAngleXY();` and `C3DVector right_2d;`.
- GetFacingAngleXY's declared type matches the PDB (float, const).
- FableWin's C3DVector default ctor (0x0179318C) is empty; the header's `C3DVector() {}` agrees.
- FableWin InitialiseFromAngles (0x017F8708): cos_yz = GFCos(angleyz); X = (magnitude * cos_yz) * GFSin(anglexy); Y likewise with
  GFCos; Z = magnitude * GFSin(angleyz) -- the ANGLE_FORMALS_MAGNITUDE_PASS26 body.
- Landed-oracle scan for retail's shape (indirect/direct call, then `mov r32,[esp+d8]`, then an x87 memory op): 4 hits
  (0x0079D7E0, 0x00B51570, 0x0079C880, 0x007B6810), none a structural twin; our shape: 0 hits.
No evidence-backed source candidate for "the x87 chain starts one slot late" is left; see ESCALATE.md.
