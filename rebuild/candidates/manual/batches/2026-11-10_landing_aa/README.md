# 2026-11-10 landing aa -- chunk 1008, the CTCInventoryAbilities translation unit (9 rows)

Source: `pending/2026-11-10_linux_hand1008/`. vland --land: 9 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x005EE77C GetExperienceCostToUpgradeAbility (`long result = cost;` before the morality test; Retail-asm line for the fistp 0x005EE86C),
  0x005F077F ContructItemLists (PDB spelling) / 0x005F1526 ConstructPCItemLists, 0x005F0C5F AssignAbility / 0x005F0DC5 AssignAbilityPC,
  0x005F1320 UnAssignAbilityPC, 0x005F3F7B ConstructExpressionCategoryList (renamed from the PC name, which is 0x005F45B5's: called x4 by the
  console 0x005F4C8F; manifest corrected) / 0x005F45B5 ConstructPCExpressionCategoryList, 0x005F4C8F GetExpressionsListUIComponent.
- 0x005F4C8F: the agent matched it with an invented `StandInBody()` definition of ConstructExpressionCategoryList; the coordinator replaced
  that with the real landed body of 0x005F3F7B (same TU in retail) and regraded RELOCATION_MATCH.
- Headers (opt-in only): CTCInventoryAbilities.h ASSIGN / ITEMLISTS, CTCHeroStats.h MORALITYMAX (GetMoralityMax 0x0057A67E).

Parked (differ/): 0x005F11A9 UnAssignAbility DIFFER(358v375) (retail reloads AssignedAbilities' start in both arms), 0x005F59C7
ConstructPCExpressionsList DIFFER(2649v2652) (frame 0xC8 v 0xD4). Unreached: IsAbleToUseAbility, CycleAssignedAbilities,
CycleAssignedExpressions, SetAbilityAsAvailable, OnSerialise. Queued `land1011aa`: 0x005EDCF0 GetNumAssignableSpecialMoveButtons (landed
callers say GetNumAbilitiesPerRow), 0x005F3F7B.
