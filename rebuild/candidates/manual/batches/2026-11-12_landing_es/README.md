# 2026-11-12 landing es -- author workflow wf7: ten of ten rows landed

Source: `pending/2026-11-12_linux_handWF7/` (workflow `lane_tools/workflows/author_rows.js`, 56 agents; launched before the
Opus-only rule, so its authors were Fable). All ten rows byte-matched; nine VERIFIED by both lenses, one with a bookkeeping finding.
Landed (all RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x008B0B60 CCombatSequenceGeneric::OnCreateSpecial,
0x009486D0 CAIStateGroup_GoToPub::FreeSeat, 0x0094F480 CAIStateGroup_BalverineJumpAtFleeingEnemy::IsRunnable, 0x005F6793
CTCInventoryAbilities::CycleAssignedExpressions (/O1 /Oy-), 0x00847DF0 CCreatureAction_UseLamp::Initialise, 0x0097D6F0
CAIStateGroupBase::AIAction_LookAtThing, 0x006AD3C0 CThingPlayerCreature::ApplyRelativeMovementAcceleration, 0x007735D0
CTCShop::GetAttitudeBuyPriceMultiplier, 0x00773790 CTCShop::GetAttitudeSellPriceMultiplier, 0x007FD650
CTCDPhysicalObstruction::FrameUpdate.

**Coordinator actions at landing:** 0x005F6793's notes named a second wrong declaration in landed 0x005F6B47
(CycleAssignedAbilities(long); the PDB has (bool), 0x005F64DD) that was missing from its NAME_FIXES.tsv -- queued. Two rows
(0x007735D0, 0x00773790) added the same GetTradeSkill declaration under the same macro with different comments: merged.
**Mechanical checks:** reserved_check 0 (two rows declare reserved callees by their PDB names; no bodies), purity clean, no local
model of a header class in a row source; 0x005F6793 carries the landed real body of GetPrioritisedExpressionDefs 0x005EED8C.
**Headers** (14; additions under new opt-ins, all unused elsewhere, defaults unchanged per hdrcheck2): CCombatSequenceGeneric.h,
CCombatActionBase.h, CTCTavernTable.h, CAIStateGroup_BalverineJumpAtFleeingEnemy.h, CCreatureActionBase.h, CCreatureAction_UseLamp.h,
CTCActionUseBase.h, CAnimFilterOfGroup.h, CThingCreatureBase.h, CThingPlayerCreature.h, COpinionReactionManager.h, CTCHeroStats.h
(TRADESKILL), CTCDPhysicalObstruction.h, CThingManager.h (an existing `#ifdef` gained a second macro in its condition).
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011es): the rows' name fixes plus the CycleAssignedAbilities one.
