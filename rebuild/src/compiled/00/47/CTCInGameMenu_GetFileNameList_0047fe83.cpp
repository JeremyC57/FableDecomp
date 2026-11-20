// CTCInGameMenu::GetFileNameList() -- rebuild the in-game menu's save list: clear the names, times, region names, minimap names and play times, fetch the sorted names and times from the profile manager, then for a debug profile fill each listed save's region / time (empty slots get the empty-slot name) and append an empty slot; otherwise (load or main menu screen) look for the autosave, size the list to the save slots (+1 for the autosave) and fill the autosave and every slot's name, region and time
// Inline-budget: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI (shared header contract) -- retail expands CCharString::EqualsCaseInsensitive in the autosave search (both GetAsCharArray ternaries, `call _stricmp`, neg/sbb/inc) as the twin 0x00597DF7 does; plain inline: DIFFER(1580v1627).
// Leaf: ?GetFileNameList@CTCInGameMenu@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047FE83, 1627 bytes, thiscall, plain `ret`; size TU (`#pragma optimize("s", on)`, as the TU's landed
//   rows, e.g. AddRegionAndTimeInfo 0x0047F58B / the ctors 0x0047DF8A / 0x0047E07F; without it DIFFER(2810v1627)).
// Identity: Ego_r.pdb / FableWin.pdb ?GetFileNameList@CTCInGameMenu@@AAEXXZ (private; Ego_r 0x0048C6D5), declared in the shared
//   engine/CTCInGameMenu.h (FABLE_RETAIL_CTCINGAMEMENU_OPENMENU); callers OpenInGameMenu 0x004830A1 (landed) and the save / load set-ups.
//   Locals are Ego_d.pdb's (i, savepath; autosave_exists, i_offset, max_slots, autosave_name, it, `const CWideString& filename`);
//   ActiveScreen's values are the nested Ego_d enum CTCInGameMenu::EScreenType (SCREEN_LOAD_GAME 3, SCREEN_IN_GAME_MENU 0).
// Twin: NUISystem::CFrontEndManager::GetSavedGameNames 0x00597DF7 (landed, same statements on the front end's members, without the
//   screen test and the debug-profile empty slot); its shape notes hold here: the slot count is written `max_slots + i_offset` at each
//   use (both resizes and the loop bound) and the slot index is `i - i_offset` (the codex draft's one residue, the count sum).
// Callees: vector<CWideString> erase(first, last) 0x00412450 / push_back 0x004128A0 / resize 0x00412B70, vector<CDateAndTime>
//   erase 0x00411B00 / push_back 0x00412970 / resize 0x0048623F, vector<CCharString> erase 0x0043336A / push_back 0x0044BFF0,
//   vector<long> erase 0x00440590 / push_back 0x00485FF3; CUserProfileManager::Get 0x0040D2A0 / GetSortedSaveNamesAndTimes
//   0x004091C0 / IsDebugProfile 0x00407030 / GetEmptySlotName 0x00406A80 / GetAutoSaveFileName 0x00406690 / GetMaxSaveSlots
//   0x004067B0 / GetSaveSlotFileName 0x00407360 / GetPathNameFromFileName 0x00408120; CAFile::GetFileModifiedTime 0x009992A0;
//   CDateAndTime() 0x009E9AB0; CWideString != 0x0099B150 / = 0x0099B7D0 / ToCharString 0x0099B2C0 / ~ 0x0099B510;
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0; _stricmp 0x00BFEBA8 (EqualsCaseInsensitive expanded);
//   CTCInGameMenu::AddRegionAndTimeInfo 0x0047F58B (landed).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_SAVELOAD
#define FABLE_RETAIL_CTCINGAMEMENU_VIDEODIALOGS
#define FABLE_RETAIL_CTCINGAMEMENU_SAVEGAME
#define FABLE_RETAIL_CTCINGAMEMENU_OPENMENU
#define FABLE_RETAIL_CTCINGAMEMENU_ACTIVESCREEN
#define FABLE_RETAIL_CAFILE_MODIFIEDTIME
#define FABLE_RETAIL_CDATEANDTIME_CTOR
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVELIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CDateAndTime.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInGameMenu.h"
#include "engine/CAFile.h"
#include "engine/CUserProfileManager.h"

void CTCInGameMenu::GetFileNameList()
{
    FileNames.clear();
    FileTimes.clear();
    FileRegionNames.clear();
    FileRegionMiniMapNames.clear();
    FilePlayTimes.clear();
    CUserProfileManager::Get().GetSortedSaveNamesAndTimes(FileNames, FileTimes);
    if (CUserProfileManager::Get().IsDebugProfile()) {
        for (long i = 0; i < (long)FileNames.size(); ++i) {
            if (FileNames[i] != L"") {
                CWideString savepath = CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false);
                if (savepath != L"") {
                    AddRegionAndTimeInfo(savepath);
                    FileTimes[i] = CAFile::GetFileModifiedTime(savepath);
                } else {
                    FileTimes[i] = CDateAndTime();
                    FileRegionNames.push_back("");
                    FileRegionMiniMapNames.push_back("");
                    FilePlayTimes.push_back(0);
                }
            } else {
                FileNames[i] = CUserProfileManager::Get().GetEmptySlotName();
                FileTimes[i] = CDateAndTime();
                FileRegionNames.push_back("");
                FileRegionMiniMapNames.push_back("");
                FilePlayTimes.push_back(0);
            }
        }
        FileNames.push_back(CUserProfileManager::Get().GetEmptySlotName());
        FileTimes.push_back(CDateAndTime());
        FileRegionNames.push_back("");
        FileRegionMiniMapNames.push_back("");
        FilePlayTimes.push_back(0);
    } else {
        bool autosave_exists = false;
        if (ActiveScreen == SCREEN_LOAD_GAME || ActiveScreen == SCREEN_IN_GAME_MENU) {
            CWideString autosave_name = CUserProfileManager::Get().GetAutoSaveFileName();
            for (std::vector<CWideString>::iterator it = FileNames.begin(); it != FileNames.end(); ++it) {
                const CWideString& filename = *it;
                if (filename.ToCharString().EqualsCaseInsensitive(autosave_name.ToCharString())) {
                    autosave_exists = true;
                    break;
                }
            }
        }
        long i_offset = 0;
        if (autosave_exists)
            i_offset = 1;
        long max_slots = CUserProfileManager::Get().GetMaxSaveSlots();
        FileNames.resize(max_slots + i_offset);
        FileTimes.resize(max_slots + i_offset);
        if (autosave_exists) {
            FileNames[0] = CUserProfileManager::Get().GetAutoSaveFileName();
            CWideString savepath = CUserProfileManager::Get().GetPathNameFromFileName(CUserProfileManager::Get().GetAutoSaveFileName(), false);
            if (savepath != L"") {
                FileTimes[0] = CAFile::GetFileModifiedTime(savepath);
                AddRegionAndTimeInfo(savepath);
            } else {
                FileTimes[0] = CDateAndTime();
                FileRegionNames.push_back("");
                FileRegionMiniMapNames.push_back("");
                FilePlayTimes.push_back(0);
            }
        }
        for (long i = i_offset; i < max_slots + i_offset; ++i) {
            FileNames[i] = CUserProfileManager::Get().GetSaveSlotFileName(i - i_offset);
            if (FileNames[i] != L"") {
                CWideString savepath = CUserProfileManager::Get().GetPathNameFromFileName(FileNames[i], false);
                if (savepath != L"") {
                    FileTimes[i] = CAFile::GetFileModifiedTime(savepath);
                    AddRegionAndTimeInfo(savepath);
                } else {
                    FileTimes[i] = CDateAndTime();
                    FileRegionNames.push_back("");
                    FileRegionMiniMapNames.push_back("");
                    FilePlayTimes.push_back(0);
                }
            } else {
                FileNames[i] = CUserProfileManager::Get().GetEmptySlotName();
                FileTimes[i] = CDateAndTime();
                FileRegionNames.push_back("");
                FileRegionMiniMapNames.push_back("");
                FilePlayTimes.push_back(0);
            }
        }
    }
}