# 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable -- Ego_d line 55 (report only, AUTH5 2026-11-21; nothing edited)
Compiland: Ego_d FableLib\debug\ai_state_group_melee_combat_nearby_base.obj (S_ENVBLOCK: FableLib line, -Gr -GR -GS-) = odall10 `--lib fablelib`.
Ego_d table: 43:40 45:377 46:19 47:19 49:49 50:26 54:409 55:76 57:4 61:2 62:14 (1035 B).
Measured (odall10.py --lib fablelib, lane headers; the line holds no STL statement), the `if (ptc_nav->IsAbleToNavigateFromPosToPos(
GetCreature().GetPos(), PEnemy->GetPos(), GetCreature().GetCombatManager().GetProximityForNavigation()))` line:
- all 11 AUTH1 candidates (e1, v01..v06, f_gf, f_op, f_z7, f_zi; copies here): 76 B = Ego_d 55, with the repo's by-value getter.
- same text with the getter returning `const float&` (scratch overlay hdrm/, measurement only): 78 B -> rejected by Ego_d 55 by 2 B
  (and its mangling becomes QBEABMXZ, against FableWin's QBEMXZ). q8.cpp has no such line (older layout).
Sizes only: the candidate's other lines do not reproduce Ego_d's frame (prologue 30 v 40; Ego_d lines 45 = 377 and 54 = 409 not
reproduced), so the 76 / 78 are line sizes, not a verdict on the getter.
