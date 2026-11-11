# agentNF10 notes

21 rows moved onto shared headers, all RELOCATION_MATCH with tests passing (g++ -std=c++11 and VC7.1 /Od), final regrade in
work/final.out. 0x0072F170 keeps its landed test (it passes against the new source); the other 20 rows have new tests in rows/.
Self-symbol and member spelling changes: RENAMES.tsv. Callee spellings proven wrong elsewhere in the landed tree: NAME_FIXES.tsv.

## Stale entries in the NF9 left list
12 CTCInventoryClothing / CTCInventoryWeapons rows already include CTCBase.h with FABLE_RETAIL_CTCBASE and a
CDefPointer<const CThingBaseDef> handle, so they bind 0x0042AF3C correctly today (005c66b2 and 005b48a2 confirmed by compile with
--all): 005b48a2 005b76d1 005b8743 005c47d5 005c4f42 005c574e 005c5f11 005c66b2 005c8176 005c8459 005c9a95 005cacb9.

## Left (with reason)
- 0x0042B0A2 GetSubDef<CAppearanceDef> (21 rows) and 0x004C7990 CThing::PeekDefinitionManager (16 rows): not started; the
  CThing-derived rows (CThingAICreature, CThingDeadCreature, CThingPhysicalSwitch ...) and the animation component rows need the
  CThing/CTCGraphicAppearance/animation headers completed first. Biggest classes were done first within the time.
- 007366c0 CTCCombat::RemoveMeleeFlourishAbility: needs std::remove / remove_copy / __find + vector::erase in the STLport header.
- 007306c0 / 00730c00 / 00730dd0 / 007310c0 CTCDoor: animation models (CAnimation, CTCGraphicAppearance) not in shared headers.
- 00811470 / 008115a0 NEntityEvents::CEvent*::OnAdd: hero-stat event models + GetTC chain.
- 007eefe0 / 007ef070 CTCExpression: no shared header for the class.
- 006700f0 CTCCoopSpirit::OnCreate: no header; C3DVector members.
- 007498e0 CTCNoise::OnCreate: copies a def via CDefClassBase::operator= (not modelled in the shared header).
- 006b0780 CTCPhysicsBase::OnCreate: calls virtual slot 93 (+0x174) of CThing, which the CThing header does not name yet.
- 006bf9e0 CTCBuyableHouse::OnCreate: CTCBuildingContents list model.
- 005bd82e / 005bdfb1 (CItem / CTCInventoryBase): Categories member opaque in CTCInventoryBase.h; CItem not shared.
- 00890f40 CGameScriptInterface::EntityResetCombatTypeToDefault: the landed local model is unsalvageable; needs a full rewrite.
- All other rows in work/left_all.tsv: not attempted (local models of CTCInventory*, CThing, CTCHero*, CTCShop ...).

## Header debt
- CTCInventoryBase Categories is still opaque; CDegradableInfo kept const in CTCDegradableBase (PCurrentDegradableInfo) by the
  Ego_r type; EGiftType / EPerceivedThingType enumerators are unnamed (no name source on disk); StatGetTypeName literals follow the
  class-name convention (FableWin.pdb names the functions, the literal itself is by convention).
- CGameTimeManager is modelled only up to DayCount (+0x14): the retail size of the STLport set at +0x2C is unproven.
