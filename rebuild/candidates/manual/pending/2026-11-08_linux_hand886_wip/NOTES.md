# Hand chunk 886 (NUISystem::CManager .. NUISystem::CManager): 0 of 1 byte-exact

## What matched (0)

## Parked for Fable 5.1 (1)
- 0x0041E6D3 NUISystem::CManager::Action DIFFER(35224v35651): all 213 cases are written (loose per-case match 93.9%; v14 also fixed the ?: FindChild arms). Session 3 solved the frame blocker: CDeletion's ctor / copy ctor must stay IMPLICIT (retail 0x42BED4 / 0x42CD84 are the compiler-generated members); the FABLE_RETAIL_CDELETION user declarations made every deletion address-exposed, and above c2's 10000-tuple limit (Action is 14593; retail is over it too) exposed objects never share slots. Now every deletion sits at retail's [ebp-248h] and the frame is 0x804 vs 0x800 | tried: 14 grades (ATTEMPTS.log); /d2inlS10..16 do not reproduce the late begin() calls; c2 debugged with gdb (WORKLOG.md Session 3) | next: (1) the slot ORDER of unpacked locals (sorted by refs/size, ties scrambled; one extra 4-byte symbol in ours), (2) retail calls postfix -- 0x42899B and begin 0x429A85 out of line in later cases, (3) block placement of the GetTC(0x6F) block, (4) entry register swap.

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

Session 2 label corrections (LABELS.tsv / NAME_FIXES.tsv):
- 0x00401490, landed as `CEngineMap::PeekWaterHasType`, is `?GFStringsAreEqual@@YI_NPB_W0@Z` (CWideString == const wchar_t* on a
  null block; FableWin name set).
- 0x005BE8D3 catalog `CTCInventoryBase::GetBoughtPrice` = NInventory::CTCInventoryBase::GetPerItemDataForItem (wave-1 decision
  already says so; it returns the CPerItemData whose Augmentations vector LOAD_WEAPON_MENU_ENTRY_NAME walks).
- 0x0042D405 = CTCInventoryWeapons::SetWeaponMappingToMenuEntry; 0x0042D41E = CTCInventoryClothing::SetClothingMappingToMenuEntry;
  0x0042BEAD = CTCInventoryWeapons::AddComponentToAvailableWeaponsEntries (catalog Forward_Std_List_PushFront...);
  0x0042CD3E / 0x0042CD61 = CTCInventoryTrade::GetNumberOfSellItems / GetNumberOfBuyItems (catalog FID_conflict:max_size);
  0x0042D437 / 0x0042D44A = CTCInventoryTrade::RemoveFromItemSellList / RemoveFromItemBuyList (catalog
  Forward_CompareAndCountAllocations...).

## Case map (for the successor)
t2/order.txt lists every case body by retail address with its EActionType (= source order). Six jump tables:
0x426E0E (58), 0x426EF6 (34), 0x426F7E (12), 0x426FAE (107), 0x42715A (23), 0x4271B6 (24); the dispatch tree sits in the
middle of the code (e.g. 0x42095C `sub eax,0C9h` and 0x420F02 `cmp eax,7D0h`).

## Parked (session 4, 2026-11-08)
- 0x0041E6D3 NUISystem::CManager::Action: DIFFER(35593v35651), loose per-case 96.1%, calls 98.4%, frame 0x800 = retail,
  best source differ/0041e6d3.cpp (= src/0041e6d3.cpp + grade lines). Inline split done with Inline-budget devices (line 3..8 of the
  source, hdr/README.md). Next: entry register roles (retail edi = CAction*, esi = game), cross-jump tails of CLOSE_MENU /
  OPEN_ABILITIES / CLOSE_ABILITIES, block placement of the expanded CWideString text == (2006/2013) and GetTC(0x6F), GetTC 75 v 73.

## Parked (session 5, 2026-11-08)
- 0x0041E6D3 NUISystem::CManager::Action: DIFFER(35697v35651), difflib 92.7% (9277/10004), loose per-case 96.6%, frame 0x808
  v 0x800. Best source differ/0041e6d3.cpp (= src/0041e6d3.cpp + 6 grade lines). Next: entry register roles (c2 colours the
  action.Ptr CSE temp #1395 before game; WORKLOG Session 5 has the cost-function reading), cross-jump tail direction
  (CLOSE_TELEPORT_MENU keeps the shared tail in retail), CHOOSE_CLAN `*action->UIActionParams.begin()`, LOAD_PHOTO homes, frame +8.

## Parked (session 7, 2026-11-08)
- 0x0041E6D3 NUISystem::CManager::Action DIFFER(35702v35651): v23 + Codex's fixes a..e merged one grade each (code identical to
  Codex's checkpoint); frame 0x800 = retail; t7/m3.py exact 79.3% / regs-masked 83.6% / regs+frame-masked 98.6% | tried: a visible
  noinline CCountedPointer(T*) ctor (frame), trade receiver locals, CActionParamSLONG* count, unsigned `<= 0`, named CWideString |
  next: entry register roles (c2 colours the action.Ptr CSE temp before game, esi by table order), regime-1 slot tie order,
  cross-jump tail direction.
