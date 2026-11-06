# Landing 2026-11-05v: chunk 795 (2 of 6 rows landed, 2 parked, 2 not attempted)

One expert agent authored the rows (sharing the lane with the chunk 794, then 796, agent). Both landed rows are plain C++ and grade
`genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x004B5500 | CQuestManager::LoadGameState(CStringParser&) | RELOCATION_MATCH PASS (7 grades) |
| 0x0055FEC0 | CTCHeroOnlineScoreboard::ChooseAClan | RELOCATION_MATCH PASS (4 grades) |

- **Key spellings:**
  - 0x004B5500: the UID remap and the thing lookup are two statements (a named `remapped` local; nested it compiled to 2567 bytes
    against 2525). A same-TU noinline body stands in for ActivateMultipleQuests 0x004B4260.
  - 0x0055FEC0: the CWideString temporaries are named locals in their own blocks; the frame is found through
    `PScoreboardScreen->FindChild(...)`.
- **Headers** (`hdr_README.md`): 20 headers gain opt-in blocks (CQuestManager LOADGAMESTATE, CMainGameComponent TEXTBANK,
  CAIStateGroupBase SPEECH, CIntelligentPointer CONVERT, CSubtitleRenderer SETTEXT, CRegion PREGIONDEF, CTCWife / CTCCreatureOpinionOfHero
  GREET, CTCShopKeeper SHOP, CTCShop AVAILABLE, CTCVillage / CTCHeroStats BRIBE, CTCHeroStats BOASTS, ...). hdrcheck2 reads
  `default=same`; every DIFF macro is new (no landed users).
  - **Coordinator merge:** the agent's CTCHeroStats.h copy predated chunk 794's landing and would have dropped 794's MORALITY branch.
    The coordinator three-way merged it (MORALITY / BOASTS / BRIBE as one #ifdef / #elif chain); every other overlay header was
    additive against the current tree.
  - Header debt: CAIStateGroup_MinionReactToPlayerInteraction.h is flattened (needs a CAIStateGroupBase-derived retail branch);
    CTCInventory.h needs a polymorphic branch (slot 82 GetComponents, CurrentOpenMenu +0x21C); CTCHeroOnlineScoreboard, CTCDrunkenness,
    CTCBoastUI and CRegionDef are local models.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05v.tsv` (the four worked rows, plus 0x0092F310 GetGossipPhrase).
  - 18 queue lines (tag `land0511v`): GetGossipPhrase, CTCShopKeeper::GetHint 0x00798D30, CTCVillage::GetCostToBribe 0x006588CF,
    CTCCreatureOpinionOfHero::ResetLastOpinionReactionFrame 0x006CEAE0, NUISystem::CList::ClearPreviousSelected 0x0048573D,
    vector<CComponent*>::clear 0x00485D25, the two `_M_insert_overflow` instantiations 0x00433530 / 0x00485DBC (vland RELOC_NAME),
    CStringParser::ReadNextItemAsQuotedString returning bool 0x009BA1D0 (0x005079C0 declares it void), plus two CHECK lines for
    folded STLport list clears (0x004B8B70, 0x004B8BB0) bound under stand-in names.
  - One-source only, not queued: CManager vtable slot 42 may be GetMiscThingsDef (landed as GetIconsDef); check the Ego_r vtable.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0.

## Parked (`pending/2026-11-05_linux_hand795/`)
| Row | Grade | Residue |
|---|---|---|
| 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate | DIFFER(2340v2340), 4 lines, 13 attempts | x87 scheduling in the experience digit-count block (TargetExperience spill-store vs the Gold load around fmul/fdivp) |
| 0x0092FAB0 CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction | DIFFER(2508v2508), 311 lines, 9 attempts | frame offsets only: retail's frame is 0x24, ours 0x1C (two extra slots: the empty CWideString, which VC7.1 here puts in `player`'s slot, and the case 1/2 temporaries) |

Not attempted (time): 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate and 0x0087F3A0 CCombatAction_KickThingOnGround::PerformAction.
