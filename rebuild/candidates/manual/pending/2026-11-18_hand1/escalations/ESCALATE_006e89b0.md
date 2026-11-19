# ESCALATE 0x006E89B0 CDefendingCombatantInfo::CCombatWheel::GetAttackerPlacementVector -- DIFFER(344v344), 2 hunks

Constraint (dag.sh, join-block region of the `pring->Index < DefenderWheelOuterRingStartIndex` arm, 16 tuples
`fadd fld fmul mov fstp ... fstp _epop x4`): cyc0 fadd [esp+38h] + mov eax,[esp+2Ch]; cyc1 fld [esp+10h] + _epop#13
(pop edi, prio 0x2800); cyc2 _epop#14 (pop esi) because fmul is not ready until cyc3 (fadd latency 3). Retail has
`pop edi ; fmul st(1) ; pop esi`, i.e. the epilogue chain one cycle later: one more u0 tuple ready at cyc1 with prio >
0x2800 (or fmul ready at cyc2). Ego_d locals match the draft (prong, inner_ring_rad, outer_ring_rad, pring, {pref_dist};
helper: separation, distance, combat_def, {far_range, middle_range}).
Measured, all the same 2 hunks: (sep+dist) paren, prong*(pref_dist), distance+separation, distance+=separation;return,
const pref_dist, FABLE_FASTCALL helper (Ego_d params are register params), helper returns in both arms (c2 sinks the
fadd), C3DVector SCALE_X_PAREN; SCALE_NAMED_RESULT / INITIALISE_SCALE worse (334); `prong *= pref_dist` worse (354).
