# 0x0047FE83 CTCInGameMenu::GetFileNameList (p25A)

Grade: RELOCATION_MATCH (1627/1627). Strict vland (LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`) passes and the real-object behaviour test passes. Mutants: 25 of 25 killed.

## Source

`src/0047fe83.cpp` is a full rewrite of the hand324 draft. That draft (DIFFER 1626v1627) used local pad structs, byte-offset FileTimes stores and invented callee names. The rewrite:

- **Headers:** shared headers only. It reuses the TU's existing opt-ins: CTCINGAMEMENU SAVELOAD / VIDEODIALOGS / SAVEGAME / OPENMENU, CUSERPROFILEMANAGER SAVENAMES / SAVELIST, CAFILE_MODIFIEDTIME and CDATEANDTIME_CTOR.
- **Style:** written in the style of the landed twin NUISystem::CFrontEndManager::GetSavedGameNames 0x00597DF7. It has the same statements on the front end's members, minus the screen test and the debug-profile empty slot.
- **The draft's one residue (the count sum):** fixed as in the twin. The slot count is written `max_slots + i_offset` at each use (both resizes and the loop bound), and the slot index is `i - i_offset`.
- **Names:**
  - Locals are Ego_d.pdb's (the 2829-byte GetFileNameList proc): i, savepath; autosave_exists, i_offset, max_slots, autosave_name, it, `const CWideString& filename`.
  - Callees use the PDB spellings already in the shared headers: GetSortedSaveNamesAndTimes, IsDebugProfile, GetEmptySlotName, GetAutoSaveFileName, GetMaxSaveSlots, GetSaveSlotFileName, GetPathNameFromFileName, CAFile::GetFileModifiedTime.
  - ActiveScreen is compared with the Ego_d enumerators SCREEN_LOAD_GAME / SCREEN_IN_GAME_MENU.
- **Compile mode:** size TU via `#pragma optimize("s", on)`, as every landed row of this TU.
- **Inline-budget (line 2):** `FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI`, an existing shared header contract, used by the twin too. Retail expands EqualsCaseInsensitive in the autosave search.

## Header overlay (`hdr/engine/CTCInGameMenu.h`, `hdr.diff`)

The overlay is add-only: 22 diff lines, no `-` lines. It adds the opt-in `FABLE_RETAIL_CTCINGAMEMENU_ACTIVESCREEN`:

- the nested enum `CTCInGameMenu::EScreenType`, from Ego_d.pdb field list 0x3309B: SCREEN_IN_GAME_MENU 0, SCREEN_RETURN_TO_GAME 1, SCREEN_SAVE_GAME 2, SCREEN_LOAD_GAME 3, SCREEN_OPTIONS 4. The Ego_r layout types ActiveScreen as EScreenType.
- `ActiveScreen` at +0x18, splitting the existing `_pad_0x0d`. This is the generated header's offset, and the landed ctors 0x0047DF8A / 0x0047E07F zero it there.

The default layout is unchanged. The header's asserts already use FABLE_STATIC_ASSERT_AT; pin_asserts.py pinned 0.

## Ablation (each removed in turn)

| removed | grade |
|---|---|
| FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI (the inline control) | DIFFER(1580v1627), 57 hunks |
| FABLE_RETAIL_CDATEANDTIME_CTOR | DIFFER(1573v1627), 141 hunks |
| `#pragma optimize("s", on)` | DIFFER(2810v1627) |
| CCHARSTRING / CWIDESTRING / CTCBASE / CTCINGAMEMENU* / ACTIVESCREEN / CAFILE_MODIFIEDTIME / SAVENAMES / SAVELIST | SRC_FAIL (needed for the model) |
| twin opt-ins CWIDESTRING_TEXT_EQ, CWIDESTRING_FORCEINLINE_EQ, STLP_MAP, CAFILE, STLP_VECTOR_ERASE_RANGE_OUTOFLINE | still RELOCATION_MATCH, so dropped |

## Test (`test/0047fe83.cpp`, links the real row object)

- **Fixtures:**
  - The menu object uses the native layout. Its five vectors are STLport, and the row's own vector COMDATs run as compiled.
  - CWideString / CCharString stand-ins have counted heap payloads.
  - CDateAndTime() marks its fields -1.
  - GetFileModifiedTime stamps a hash of the path.
  - The profile manager is scripted: sorted list, debug flag, autosave name, slot names, and a path rule where "x..." names have no file.
  - AddRegionAndTimeInfo appends R:/M:/77.
  - `_M_fill_insert` (resize's grow path) appends.
- **Nine runs:**
  - debug profile, with and without saves;
  - load screen with the autosave listed;
  - in-game-menu screen with a case-insensitive match;
  - save screen (no search);
  - autosave not listed;
  - no match;
  - autosave without a file;
  - autosave matched mid-list.
- **Checks:** every run compares all five vectors element by element against Model(), checks the heap balance, and checks the number of ToCharString calls, which proves the search stops at the first match.
- **Mutants:** `mutants/` holds 25 one-decision mutations of the ROW SOURCE, all killed.

## vland warnings (labels DB, not source names)

All four warnings are label-database issues; the source names are right.

- `RELOC_NAME`: GetMaxSaveSlots 0x004067B0 is labelled `_global::ReturnConst_50_004067b0`. It's an ICF-folded `mov eax,32h ; ret`.
- `RELOC_NAME`: GetSaveSlotFileName 0x00407360 is labelled `NInventory::CTCInventoryBase::GetCategoryWithIndex`, a byte-identical ICF body.
- `RELOC_NAME`: vector<CDateAndTime>::erase(first, last) 0x00411B00 is labelled `vector_push_back_copy` (a forge auto-name).
- `RELOC_ALIAS`: `__stricmp` is the MSVCR71 import thunk.

The spellings are the ones the shared CUserProfileManager.h SAVELIST block and the landed twin use.

## NAME_FIXES

None. The draft's invented names (IsQuery_00407030, GetManualSaveCount_004067b0, ...) were never landed.
