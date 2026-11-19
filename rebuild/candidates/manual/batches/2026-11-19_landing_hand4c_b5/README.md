# Landing hand4c b5 (2026-11-19): CThingCreatureBase::GetPossibleCombatAnimations (0x006668A0)

vland --land with strict headers gives RELOCATION_MATCH + PASS, with no flags. New opt-in engine/CThingCreatureBase.h FABLE_RETAIL_CTHINGCREATUREBASE_POSSIBLECOMBATANIMS: a
declaration only, with no layout change (`hdr_README.md`).

Also in this batch: the test of the landed 0x0066A1E0 (ForEachInRoughArea) gains two cases, (4) things-collision and (5) no physics component.
Its pass pattern is unchanged, and it rechecks RELOCATION_MATCH + PASS.
