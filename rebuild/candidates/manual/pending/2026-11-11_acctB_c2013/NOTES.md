# Hand chunk 2013 (CFrontEndManager .. CTCQuickAccessMenu): 4 of 4 byte-exact

All four rows grade RELOCATION_MATCH under VC7.1 RTM (q.py / gall.sh with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2013/hdr`),
pass the read-only vland verifier (parity RELOCATION_MATCH, behaviour PASS, no RELOC_IDENTITY; warnings listed below), and their
tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). Row starts and lengths were checked against the neighbours:
all four catalog lengths are right (no SPLITS.tsv).

## What matched (4)

### NUISystem::CFrontEndManager (size TU, `#pragma optimize("s", on)`, same TU as chunk 2003's 0x00598463)
- 0x00597DF7 GetSavedGameNames(vector<CWideString>&) -- clears the names and the frame's per-save vectors, fetches
  CUserProfileManager::GetSortedSaveNamesAndTimes; debug profile: per name the empty-slot text / modified time + AddRegionAndTimeInfo /
  blank entries; otherwise the autosave first (case-insensitive ToCharString compare), GetMaxSaveSlots slot names, then the
  empty names erased with their times. Shape lesson: retail's `mov ecx,[first_slot] ; lea ebx,[eax+ecx]` (0x005980F6) is the slot
  count written `max_slots + first_slot` at each use (both resizes and the loop bound, c2 CSEs it into ebx); a named
  `count = max + first_slot` leaves first_slot a 1-reference piece that c2 drops (c2prio: `drop .. refs 1 spill -2`) and folds the
  add (`mov ebx,eax ; add ebx,[mem]`). The slot index is `i - first_slot` (strength-reduced, zeroed after the entry test).
- 0x00595CC1 ConstructFileDescription(unsigned long, CComponent*, CComponent*, CWideString&) (/GS) -- the save button's text
  ("<profile> : name\n<date> <time>\n<region> : region\n<time played>") and minimap / default picture. Shape lessons: the
  CIVCountedPointer temporaries need a one-level forced destructor with the null store (size TU calls the plain one); text arm
  first, group arm second; minimap name and map as named references; nested ifs for `valid` / non-empty (an `&&` with the
  GetEmptySlotName temporary adds a conditional-destruction flag); FindChild results tested as CComponent* then cast;
  FABLE_RETAIL_CARRAY for the group's first id (edi).

### CTCQuickAccessMenu (speed TU)
- 0x00626110 ContructItemLists(map<long,long>&, CList*, CComponent*, CCountedPointer<CComponent>&) and
- 0x00626750 ContructItemLists(vector<pair<long,long> >&, CList*, CComponent*, CCountedPointer<CComponent>&) -- per item a menu
  entry registered in CTCInventory::ItemsMenuEntries, its quick-access icon (AssignedIcons), LOAD_ITEM_MENU_ENTRY_NAME (and, for
  expressions, LOAD_QUICK_MENU_EXPRESSIONS_ICON) visible actions, thing param, list child, actions-list child. The bodies are the
  landed CTCInventory::ConstructItemEntry 0x0058A0C7 shapes on the shared headers; the CAction constructor must be visible
  (overlay FABLE_RETAIL_CACTION_INLINE_CTOR). Inline-budget residue (both rows): retail expands the icon's CCountedPointer(T*) only
  in the LEFT arm and calls ~CCountedPointer only in the DOWN arm; the c2 inline trace shows every arm one factor 2 too hot
  (scores ctor 56/65/76/92, dtor 37/43/51/61 against T 74). Handed back with `// Flags: /d2inlT59` (smallest T in 59..61) and an
  `// Inline-budget:` line; the source is otherwise natural C++. Alternative that also matches with no flag: a folded `&& true` in
  the loop condition (body 2.5x instead of 5x; probe/v_andtrue.cpp, probe/00626750.cpp). `while` and guarded do-while loops do not
  (9x / 5x). The coordinator may prefer either device.

## Parked for Fable 5.1 (0)
None.

## Skipped
None.

## Names
All four row names are the Ego_r.pdb / FableWin.pdb spellings (two sources each in LABELS.tsv); every callee binds a PDB / STLport
name. No descriptive names were made up. The two QuickAccess overloads: the map one is 0x00626110 (caller = landed
ConstructItemCategoryList, which passes its std::map<long,long>; tree walk), the vector one 0x00626750 (caller 0x00627400, the
expressions category; 8-byte element index loop); both PDBs list the map overload first, as retail lays them out.

### Callee names (proven, see LABELS.tsv / NAME_FIXES.tsv)
GetSortedSaveNamesAndTimes 0x004091C0 (landed as descriptive LoadFileList), GetMaxSaveSlots 0x004067B0, GetSaveSlotFileName
0x00407360, GetAutoSaveFileName 0x00406690, CAFile::GetFileModifiedTime 0x009992A0, CDateAndTime() 0x009E9AB0 / ToFileTime
0x009E9D40, CXBConfig::FormatDateTime 0x00A0D980, CBankFile::FindIndexBySymbolCRC 0x009CC410, CTCInventoryStats::GetTimeSpentInGame
0x005CCC91, CWideString::operator!=(const CWideString&) 0x0099B0F0, CCharString::operator!=(const char*) 0x0099E960.

## Label corrections (catalog / manifest labels found wrong)
- 0x004091C0 "LoadFileList" -> CUserProfileManager::GetSortedSaveNamesAndTimes (NAME_FIXES.tsv).
- 0x004067B0 "ReturnConst_50_004067b0" -> CUserProfileManager::GetMaxSaveSlots; 0x00407030 "ReturnFalse_ret0_00407030" ->
  CUserProfileManager::IsDebugProfile (chunk 2003); 0x00407360 "NInventory::CTCInventoryBase::GetCategoryWithIndex" ->
  CUserProfileManager::GetSaveSlotFileName.
- 0x00411B00 "vector_push_back_copy" -> vector<CDateAndTime>::erase(first, last); 0x00440590 "Buffer_Relocate" ->
  vector<long>::erase(first, last); 0x00429F43 "CTCMapwho::OnDie" -> CCountedPointer<CAction>::DeleteData; 0x005D9710
  "CTCInventoryItem::GetInventoryCategory" -> GetUseButtonActionType (as the landed 0x0058A0C7 binds it).
- 0x009CC410 seven invented "LookupValue"-style spellings -> CBankFile::FindIndexBySymbolCRC; 0x005CCC91 CTCHeroStats::
  GetTimeSpentInGame(long, bool) -> CTCInventoryStats::GetTimeSpentInGame(unsigned long, const NGameText::CDataBank*);
  0x00A0D980 free FormatDateTime -> CXBConfig::FormatDateTime (all NAME_FIXES.tsv).

## vland warnings (non-blocking; for the landing session)
- RELOC_NAME on the catalog labels above (expected; NAME_FIXES.tsv).
- RELOC_CONFLICT ??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ -> 0x00429C53 here, 0x004291DE in landed 0x00570332:
  0x00429C53 is the `jmp 0x004291DE` destructor COMDAT, 0x004291DE is ReleaseData (engine/CCountedPointer.h evidence; that row's
  own comment says so); the stale 0x00570332 binding is the wrong one (NAME_FIXES.tsv).
- RELOC_ALIAS for the _stricmp / wcscmp import thunks (as usual).

## Header debt
- The hdr/ overlay (hdr/README.md): opt-in blocks in CFrontEndManager.h, CUserProfileManager.h (both on top of chunk 2003's
  pending overlay copies), CDateAndTime.h, CAFile.h, Win32Types.h, CBankFile.h, CTCInventoryStats.h, CIVCountedPointer.h,
  NUISystem_CAction.h, fable_string.h, and the new engine/CXBConfig.h. fable_string.h / fable_definition_table.h are whole-file
  overlays (fable_string.h cannot be overridden through engine/ headers alone): merge the `_NE` opt-in into the repo copy.
- ConstructFileDescription includes engine/CTCInventoryStats.h for one static declaration, which drags in CTCBase /
  CTCInventoryBase (FABLE_RETAIL_CTCBASE / _CTCINVENTORYBASE defined for that only).
