# Landing hand4a b7 (2026-11-19): CCombatManager::IsCreatureInAttackRingSector (0x006E94D0, 426 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS (22 cases, 14/14 host mutants); 21 grades; no header changes, no Inferred notes.
Name proven (catalog Combat_CheckRingRangeToTarget). The parked 0x24 v 0x1C frame came from the declaration-only
FABLE_RETAIL_STLP_TREE_MFIND_DECL hiding that `_M_find` only reads its key; the existing FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE (visible body,
noinline; retail calls 0x006F0250 out of line) lets both key temporaries share [esp+14h] (`// Inline-budget:`). Same-TU helpers GetRing,
GetRingRadius, GetAttackerProngVector, GetProngSeparationHalfAngle defined before the row (FableWin combat_manager.obj order; Ego_d locals) --
without them an ecx reload differs (same-TU callee lesson).
Name fixes (`NAME_FIXES.tsv`): 0x006E8810 is ?GetAttackerProngVector@CCombatRing@CDefendingCombatantInfo@@.. and 0x006E7FD0 is
?GetProngVector@CProng@CDefendingCombatantInfo@@.. (Ego_r + FableWin) -- this settles the 0x006E7FD0 double binding left by the 006e89b0 landing
(the CDefendingCombatantInfo spelling is right; GetRotatedPosition@CProng is the wrong one); 0x0129BA3C DELTA_NOTIONAL_ZERO.
