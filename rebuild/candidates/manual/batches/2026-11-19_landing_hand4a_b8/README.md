# Landing hand4a b8 (2026-11-19): CCombatManager::SetupDefenderPlacementWheel (0x006ECEA0, 383 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS (11 cases, 9/9 host mutants); 5 grades; plain C++, no Inferred / Inline-budget / Bracket.
IDENTIFIED (catalog "CDefendingCombatantInfo::UpdateCombatState" was descriptive): ?SetupDefenderPlacementWheel@CCombatManager@@AAEXAAUCCombatantInfo@@_N@Z,
Ego_r 0x007167A0 (same `and esp,-8` frame) + FableWin 0x0241AD40 (combat_manager.obj). FableWin debug body, Ego_d locals.
New opt-ins: CCombatManager.h SETUPPLACEMENTWHEEL (private inline GetWorld FableWin 0x024128A0, non-const inline GetCombatantInfo FableWin
0x02415CC0, the declaration), CDefendingCombatantInfo.h CCOMBATWHEEL_UPDATE (declaration; retail 0x006E8560 landed). All 23 landed users rechecked.
Noted (no proof of spelling yet): landed label unattributed_InsertUniqueCRetailTree006f2a40 is the map<CThingCreatureBase*,CAttackingCombatantInfo*>
tree's insert_unique.
