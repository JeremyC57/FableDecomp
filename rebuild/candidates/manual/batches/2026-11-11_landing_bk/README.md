# 2026-11-11 landing bk -- agentNF10: 21 CTC component rows moved onto the shared definition-system headers

Source: `naming/2026-11-11_agentNF10/` (STATUS / NOTES / LESSONS / RENAMES / NAME_FIXES). vland --recheck-landed: 21 RELOCATION_MATCH,
tests PASS (20 new tests).
- GetDef<CThingBaseDef> 0x0042AF3C now bound by 20 of these rows, CTCBase::GetDefinitionManager 0x006869D0 by 13.
- Classes: CTCObjectAugmentations 4, CTCInventoryWeapons GetItemDescription x4, CGameScriptInterface IsGiftRomantic / Friendly / Offensive,
  CTCInventoryItem 2, and one each of CTCDummyVillager (0x0072F170: the RELOC_CONFLICT since NF9 is gone, it binds GetDef<CThingBaseDef>),
  CTCDoor ctor, CTCCameraCollision, CTCActionUseBase, CTCDegradableBase, CTCHeroSpecialMovement, CTCCombat::AddMeleeFlourishAbility,
  CTCPerceiveThing. Leaf respellings (RENAMES.tsv): virtual OnCreate / OnInitialActivate (UAE / MAE), CTCDoor(CThing&), full vector<long>
  spelling; member names from Ego_r (TotalTimeToSprintSeconds, CGiftDef::Type).
Headers: base 8f1b0fe48, 13 changed (opt-ins only, hdrcheck2 default=same) + new CGameTimeManager, CGiftDef, CTCDegradableBase,
CTCHeroSpecialMovement. Queued land1011bk: 14 name fixes.
NF9's left list had 12 stale CTCInventoryClothing / CTCInventoryWeapons entries that already bind 0x0042AF3C.
Left: GetSubDef<CAppearanceDef> 0x0042B0A2 (21) and PeekDefinitionManager 0x004C7990 (16) not started; rows needing missing models
(std::remove / vector::erase, CTCDoor animation, hero-stat events, CTCExpression / CTCCoopSpirit headers, CDefClassBase::operator=, CThing
slot 93, CTCBuildingContents, CTCInventoryBase::Categories) -- NOTES.md.
