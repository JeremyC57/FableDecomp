# 2026-11-11 landing bm -- agentNF11: 12 GetSubDef / PeekDefinitionManager binders moved onto the shared headers

Source: `naming/2026-11-11_agentNF11/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). The agent was stopped by a container restart
and resumed; it regraded all 12 afterwards. vland --recheck-landed here: 12 RELOCATION_MATCH, tests PASS.
- GetSubDef<CAppearanceDef> 0x0042B0A2: 9 of 21 (CTCClock::OnInitialActivate, CTCCreatureHive::Open, CTCAnimationComplex::OnCreate,
  CTCAnimatingObject::PlayDefaultAnimation, CThingDeadCreature Preload / SetupDeadAnimation, CThingPlayerCreature::ConstructFromParams,
  CTCGraphicAppearance::SetAsHighlighted, and 0x00667B30 renamed CThingCreatureBase::GetRandomCombatAnimationFromName from the
  descriptive GetAppropriateCombatAnimation).
- CThing::PeekDefinitionManager 0x004C7990: 3 of 16 (0x008D08A0 CGameScriptThing::IsSummonedCreature, 0x0089E080 / 0x00890F40
  CGameScriptInterface::EntitySetCombatType / EntityResetCombatTypeToDefault -- the last rewritten from a free-function model to the PDB member).
Headers (base be477e123, opt-ins only, hdrcheck2 default=same): CAnimateThing, CTCAnimationComplex, CThingCreatureBase, CThingDeadCreature,
CThingPlayerCreature, CEngineMeshEffect, CTCGraphicAppearance, CGameScriptThing; new CTCClock, CTCCreatureHive, CTCAnimatingObject,
CDeadCreatureAnimPreloader, CThingPlayerCreatureInit, NProgressDisplay, CTCSummonableCreature.
Left (NOTES.md): GetSubDef 12 (0x004BFD10 parked), PeekDefinitionManager 13 (0x00665510 needs CThingCreatureBase slot 72), GetDef /
GetDefinitionManager item not reached (NF10 lists stand). Queued land1011bm: name fixes (0x00662170 GetCombatType, 0x0070EC30 ClearAllAnims,
0x0043FFA0 GetDef<CCreatureAbilityDef>, 0x013BAD3C bound under two names ...).
Note: the agent reported one read-only `git log` it ran before the restart (agents may not run git); nothing was written to the repo.
