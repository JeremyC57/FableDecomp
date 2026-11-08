# Hand chunk 886 (NUISystem::CManager .. NUISystem::CManager): 0 of 1 byte-exact

## What matched (0)

## Parked for Fable 5.1 (1)
- 0x0041E6D3 NUISystem::CManager::Action DIFFER(20848v35651): the source is incomplete -- ~165 of 213 switch cases are written (WIELD .. RESPAWN, 0x41E752..0x423261); the 2000-2023 description loaders (~12.7 KB) and the 3000-4002 photojournal / scoreboard cases (~3.5 KB) still fall to `default`, so the frame (sub esp,800h) and every register choice still differ from instruction 1 (difflib 47.4%) | tried: 8 grades building the body batch by batch (ATTEMPTS.log) | next: write the missing cases (WORKLOG.md "Decoding rules"), then tune the same-TU inline split (setters 0x41C965..0x41CA52, list begin 0x429A85 called late, inline early) with same-TU out-of-line bodies and /d2inlS<N>.

## Skipped

## Label corrections
See LABELS.tsv (25 rows, each with two sources). Catalog/manifest labels proven wrong:
- 0x00408360 catalog `CPlayerGui::ForceClose` = CUserProfileManager::SetDefaultValuesGameplay; 0x004083B0 catalog
  `SetDefaultValuesGameplay` = CUserProfileManager::SetDefaultValuesAudio (RESTORE_DEFAULTS_GAMEPLAY / AUDIO).
- 0x00625920 `CTCCreditsUI::Deactivate` (folded body) is called here as CTCQuickAccessMenu::Deactivate.
- 0x0058A724 `CTCInventory::GetStyleCardsListUIComponent` = ConstructStyleCardsScreen; 0x006096E0 `ConstructLogbookScreen` =
  CTCInventoryMap::ConstructLogbookListPC; 0x00601310 `ConstructQuestsScreen_MainMenu` = CTCInventoryMap::ConstructQuestsScreenPC.
- 0x005D6748 landed label `?ConstructExperienceList@CTCInventoryStats@@AAEPAVCComponent@NUISystem@@XZ` is the virtual slot 87
  taking the screen (NAME_FIXES.tsv).
- The descriptive setters 0x0041C965 / 72 / 94, 0x0041CA31 / 3E / 52, 0x0041CF1F / 33, 0x005FA900 have PDB names (LABELS.tsv).

## Case map (for the successor)
t2/order.txt lists every case body by retail address with its EActionType (= source order). Six jump tables:
0x426E0E (58), 0x426EF6 (34), 0x426F7E (12), 0x426FAE (107), 0x42715A (23), 0x4271B6 (24); the dispatch tree sits in the
middle of the code (e.g. 0x42095C `sub eax,0C9h` and 0x420F02 `cmp eax,7D0h`).
