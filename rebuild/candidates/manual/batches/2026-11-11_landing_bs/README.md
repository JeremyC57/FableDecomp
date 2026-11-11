# 2026-11-11 landing bs -- agentNF14: 62 landed rows on proven names (definition-system remainder + re-identified rows)

Source: `naming/2026-11-11_agentNF14/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed: 62 RELOCATION_MATCH,
VC7.1 tests PASS (0x006AF620 prints its catalog pattern DRAWISDRAWABLE_FORWARD_OK, checked by running the exe). g++ PREFAIL on 21 rows is
pre-existing (list in the hand-back NOTES).
- All 48 GetDef<CThingBaseDef> 0x0042AF3C / CTCBase::GetDefinitionManager 0x006869D0 binders moved (CTCInventory* family 18, CThing 3,
  CTCHeroAttachableAppearanceModifiers 3, CTCHeroMorph / CTCCombat / CTCShop / CTCInventoryTrade 2 each, 16 more classes 1 each).
- Own names re-identified (RENAMES): 0x006585EE CTCVillage::NotifyHasApologised, 0x00658604 GetIsHeroEnemyOfGuards (/Os TU), 0x006CB8F0
  CTCCreatureNavigation::IsCurrentlyNavigating, 0x006CEAE0 CTCCreatureOpinionOfHero::ResetLastOpinionReactionFrame, 0x007236E0
  CTCPhysicsStandard::ResetInterpolation, 0x00811450 CTCEntityEvents::FrameUpdate, 0x006AF620 / 0x006AF630 CTCPhysicsBase::GetWorldMap
  (IAE / IBE), 0x008957C0 CGameScriptInterface::SendEntityEvent, 0x006C8780 CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn
  (invented namespace removed), 0x0071C360 CTCHeroMorph::InitialiseCustomTattoos (invented CTCHeroMorph_Retail removed).
- 0x0065C1FC / 0x006B0C10 / 0x007BE0F9 / 0x007E2790 rebased on retail headers (007E2790's +0x28/+0xBC is PeekDisplayEngine().IsScreenFadeOut()).
Headers (base 2e8e72e93, opt-ins only, hdrcheck2 default=same): CTCVillage, CTCCreatureNavigation, CTCPhysicsStandard, CTCPhysicsBase,
CTCEntityEvents, CMainGameComponent, CDisplayEngine, CFadeInFadeOutBase, CWorld, CThingManager.
Bindings: 220 removed, 117 added. Queued land1011bs: 96 proven callee spellings.
Left: 0x006A23E0 and 0x008AF850 (invented namespaces; need container / sound-bank models).
