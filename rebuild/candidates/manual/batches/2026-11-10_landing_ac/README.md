# 2026-11-10 landing ac -- chunk 1008b, retry of the CTCInventoryAbilities rows (1 row)

Source: `pending/2026-11-10_linux_hand1008b/`. vland --land: 1 RELOCATION_MATCH, test PASS (VC7.1 and g++).

- 0x005EE49A CTCInventoryAbilities::IsAbleToUseAbility: plain C++ on shared headers with the inline CThing::PeekTC
  (FABLE_RETAIL_CTHING_GETTC_EXPANDED) and the real CDefinitionManager::GetDef template body (as landed 0x005F4C8F) under auto_inline(off),
  which emits the three GetDef instances it calls. New headers: CTCSummonSpell.h (GetResetCreatureManaCost 0x0079A610),
  CSpecialAbilitiesFireballSpellDef.h, CSpecialAbilitiesDivineWrathDef.h. Opt-ins for the parked 0x005F6BD6: CTCInventoryAbilities.h
  BUTTONS, CGamePlayerInterface.h SPECIALMOVEBUTTONS, stlport/fable_algorithm.h STLP_FIND (declarations only).

Parked (differ/): 0x005F11A9 UnAssignAbility DIFFER(373v375), 0x005F59C7 ConstructPCExpressionsList DIFFER(2649v2652), 0x005F6BD6
SetAbilityAsAvailable DIFFER(617v618) (c2prio diagnoses in NOTES.md). Held (held/): 0x005F64DD CycleAssignedAbilities, 0x005F6793
CycleAssignedExpressions -- PSoundManager slot 9 has only the descriptive name Virtual09_00a3e3a0 (owner decision pending). Unreached:
0x005F6E40 OnSerialise. Queued `land1011ac`: 0x0079A610, 0x00445940, 0x005F8150 find<EHeroAbility*>, 0x005EDCF0, 0x009AD390.
