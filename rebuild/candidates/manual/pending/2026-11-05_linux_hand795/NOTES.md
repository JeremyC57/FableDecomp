# Chunk 795 notes

## 0x004B5500 CQuestManager::LoadGameState(CStringParser&) -- RELOCATION_MATCH (src/004b5500.cpp)
- Shape: clear the lists, then one `if (parser.SkipPastString("START_x"))` block per section, in retail order: quest-card types,
  active quest cards (UID loop), saved quests (persistence script), active quests (CArray<CCharString> + ActivateMultipleQuests),
  completed quest cards (UID loop), CG-disabled quests, between-screens quests, NumberOfQuestsBetweenStartAndFinishScreens.
- Key spelling: in both UID loops the remap and the lookup are two statements
  (`unsigned __int64 remapped = World->GetThingManager().RemapUIDForSerialisation(false, true, 0, uid);` then
  `GetPThingWithUID(remapped)`). Written as one nested expression, the result is 2567 v 2525 bytes, because World is held across
  both calls and the UID pair is spilled.
- ActivateMultipleQuests 0x004B4260 has a same-TU noinline stand-in, because retail compiled it in this TU. Without it, the
  local array's words are not kept in registers.
- The callee names come from the landed rows and from Ego_r. GetPThingWithUID is 0x0051ED80. RemapUIDForSerialisation is
  0x0051E210. The four _List_base::clear instantiations are listed in the header comment.
- Header debt: the FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE members are in the hdr/ overlay (see hdr/README.md).

## 0x0055FEC0 CTCHeroOnlineScoreboard::ChooseAClan -- RELOCATION_MATCH (src/0055fec0.cpp)
- Key spellings: temps built before the argument pushes are named block-scoped locals:
  `{ CWideString title_text(L"TEXT_GUI_MENU_CHOOSE_CLAN"); title->SetValue(title_text, true, true); }` and
  `{ CWideString cache_name(L"ClanData"); image->PTexture = GetCachedImage(cache_name, it->ClanID); }`.
  The frame is found through `PScoreboardScreen->FindChild("PC_UI_SCOREBOARD1_FRAME")`.
- Header debt: CTCInventory and CTCHeroOnlineScoreboard (+CClanInfo) are local models. The generated CTCInventory.h needs a
  polymorphic retail branch: vtable slot 82 is GetComponents, and CurrentOpenMenu is at +0x21C.
  CTCHeroOnlineScoreboard.h needs a retail branch with PScoreboardScreen and the clan list.

## 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate -- PARKED DIFFER(2340v2340), 4 diff lines (differ/005e78f0.cpp)
- Residue: x87 scheduling in the experience digit-count block. Retail orders the TargetExperience spill-store and the Gold
  load differently around fmul/fdivp. I tried 10 spellings (temps, operand order, cast placement, float vs double locals);
  none changed it.
- The callees include NUISystem::CList::ClearPreviousSelected 0x0048573D (static, no this) and
  vector<CComponent*>::clear 0x00485D25 (see NAME_FIXES.tsv).

## 0x0092FAB0 CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction -- PARKED DIFFER(2508v2508), 311 lines (differ/0092fab0.cpp)
- The size matches. The instruction stream matches once [esp+X] offsets are masked; the only other difference is the jump-table
  relocation. The frame is 0x24 in retail and 0x1C here.
  Retail has two extra dword slots:
  - The empty CWideString stays in its own frame slot. VC7.1 here reuses the `player` parameter slot instead.
  - The case 1/2 temps get their own slot.
- Jump table 0x0093046C. Case targets: 0 -> 0x0092FBE8, 1 -> 0x0092FCDF, 2 -> 0x0092FF3F, 3 -> 0x0093002C. The cases fall through.
- Shape: the shopkeeper test, then a fallthrough switch over ReactionType 0..3 (gossip / shop hint / greet / wife), then the
  guard bribe block (CTCVillage::GetCostToBribe vs CTCHeroStats::Money, TEXT_GUI_CRIME_BRIBE_OFFER).
- Callee names proven (NAME_FIXES.tsv):
  - GetGossipPhrase 0x0092F310
  - CTCShopKeeper::GetHint 0x00798D30
  - CTCVillage::GetCostToBribe 0x006588CF
  - CTCCreatureOpinionOfHero::ResetLastOpinionReactionFrame 0x006CEAE0
- DESCRIPTIVE (not proven):
  - CRegionDef::UseCrimes +0x9D
  - CASoundManager vtable slot 9
  - the ReactionType value meanings
  - CTCDrunkenness::Drunk +0xC
  - CTCBoastUI::ActivationRequested +0x2A
- CManager vtable slot 42 looks like GetMiscThingsDef, not the landed "GetIconsDef". This is a single-source reading:
  the caller reads the misc-things def. It is NOT in NAME_FIXES.tsv; confirm it against the Ego_r CManager vtable before respelling. 0x004AFFA0 has the same
  caveat (its landed label is a propagated one; check it with label_trust.py before reuse).
- Header debt:
  - The class is a local packed model (NextReactionFrame[5] +0x14, ReactionType +0x28, WaitingForConfirmation +0x2C,
    DidTraderInteraction +0x2D, Position +0x2E). The generated CAIStateGroup_MinionReactToPlayerInteraction.h is flattened;
    it needs a retail branch derived from CAIStateGroupBase.
  - Local CTCDrunkenness / CTCBoastUI / CRegionDef models.
- Next idea: find what keeps the `player` parameter slot live in retail. Possible causes: a second use of `player` after the
  switch, or a by-value CIntelligentPointer copy.

## 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate -- SKIP (no draft, time)
## 0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction -- SKIP (no draft, time; x87-heavy)
