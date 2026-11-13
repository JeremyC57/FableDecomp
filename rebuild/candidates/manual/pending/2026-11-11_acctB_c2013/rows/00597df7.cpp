// NUISystem::CFrontEndManager::GetSavedGameNames(std::vector<CWideString>& names) -- rebuild the front end's save list for the current profile: clear the names and the per-save vectors (file times, region names, region minimap names, play times), fetch the sorted save names and times; for the debug profile check every listed name (empty names become the empty-slot text, existing saves get their modified time and AddRegionAndTimeInfo, missing ones blank entries); otherwise put the autosave (if listed) first, then one entry per save slot (GetMaxSaveSlots), and finally drop the empty names with their times
// Leaf: ?GetSavedGameNames@CFrontEndManager@NUISystem@@IAEXAAV?$vector@VCWideString@@V?$allocator@VCWideString@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00597DF7, 1644 bytes, thiscall, `ret 4` at 0x00598435 plus the out-of-line erase arm after it
//   (0x00598438..0x00598462, ends with `jmp 0x00598424`; RefreshAvailableSavedGamesForProfile starts at 0x00598463). Unpadded size
//   TU: the previous function (GotoPreviousScreen 0x00597BF2, landed) ends with `leave ; ret 4` at 0x00597DF4, so 0x00597DF7 is
//   the first byte after it.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetSavedGameNames@CFrontEndManager@NUISystem@@IAEXAAV?$vector@VCWideString@@...@Z
//   (protected); Ego_r donor order ... GotoPreviousScreen, GetSavedGameNames, RefreshAvailableSavedGamesForProfile matches retail;
//   callers 0x0059848B (RefreshAvailableSavedGamesForProfile, landed spelling in chunk 2003) and 0x005989CF (LoadProfile); it fills
//   exactly the PDB members m_vFileTimes +0x68 (28-byte CDateAndTime), FileRegionNames +0x74, FileRegionMiniMapNames +0x80,
//   FilePlayTimes +0x8C and calls AddRegionAndTimeInfo 0x00597228 (landed), which appends to those three vectors.
// Callees: vector<CWideString>::erase(first, last) 0x00412450 / erase(position) 0x0059AE20 / resize(n) 0x00412B70,
//   vector<CDateAndTime>::erase(first, last) 0x00411B00 / erase(position) 0x0059AEAF / resize(n) 0x0048623F,
//   vector<CCharString>::erase 0x0043336A / push_back 0x0044BFF0, vector<long>::erase 0x00440590 / push_back 0x00485FF3,
//   CUserProfileManager::Get 0x0040D2A0 / GetSortedSaveNamesAndTimes 0x004091C0 / IsDebugProfile 0x00407030 / GetAutoSaveFileName
//   0x00406690 / GetMaxSaveSlots 0x004067B0 / GetSaveSlotFileName 0x00407360 / GetEmptySlotName 0x00406A80 / GetPathNameFromFileName
//   0x00408120, CAFile::GetFileModifiedTime 0x009992A0, CDateAndTime() 0x009E9AB0, CWideString != 0x0099B150 / = 0x0099B7D0 / ~
//   0x0099B510 / ToCharString 0x0099B2C0, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, _stricmp 0x00BFEBA8 (the
//   expanded EqualsCaseInsensitive), wcscmp 0x00BFE94C + STLport basic_string<unsigned short> == 0x00404280 (the expanded
//   CWideString == L""), AddRegionAndTimeInfo 0x00597228.
// Shape (chunk 2013): shared headers only (hdr/ overlay opt-ins: CDATEANDTIME_CTOR, CAFILE_MODIFIEDTIME, CUSERPROFILEMANAGER_SAVELIST,
//   CFRONTENDMANAGER_ADDREGION; SAVEDGAMES / SAVENAMES from chunk 2003). Size TU (`optimize("s", on)`: here a plain `mov ebp,esp`
//   frame, 0x90 bytes). The slot count is written `max_slots + first_slot` at each use (both resizes and the loop bound): c2 then
//   CSEs it into ebx after a 2-reference first_slot piece in ecx (retail `mov ecx,[ebp-10h] ; lea ebx,[eax+ecx]`, 0x005980F6); one
//   named `count = max + first_slot` drops that piece (1 reference) and folds the add (`mov ebx,eax ; add ebx,[ebp-10h]`). The slot
//   index is `i - first_slot` (strength-reduced to [ebp-28h], zeroed after the loop entry test; a separate counter is zeroed before
//   it). The empty-slot sweep at the end is one `for` over two iterators with the `&&` bound (retail tests names' end first).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQUALSCI
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_SAVEDGAMES
#define FABLE_RETAIL_CFRONTENDMANAGER_ADDREGION
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_MODIFIEDTIME
#define FABLE_RETAIL_CDATEANDTIME_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVELIST
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CDateAndTime.h"
#include "engine/CFrontEndManager.h"
#include "engine/CAFile.h"
#include "engine/CUserProfileManager.h"

namespace NUISystem {
void CFrontEndManager::GetSavedGameNames(std::vector<CWideString>& names)
{
    names.clear();
    m_vFileTimes.clear();
    FileRegionNames.clear();
    FileRegionMiniMapNames.clear();
    FilePlayTimes.clear();
    CUserProfileManager::Get().GetSortedSaveNamesAndTimes(names, m_vFileTimes);
    if (CUserProfileManager::Get().IsDebugProfile()) {
        for (long i = 0; i < (long)names.size(); ++i) {
            if (names[i] != L"") {
                CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(names[i], false);
                if (path != L"") {
                    AddRegionAndTimeInfo(path);
                    m_vFileTimes[i] = CAFile::GetFileModifiedTime(path);
                } else {
                    m_vFileTimes[i] = CDateAndTime();
                    FileRegionNames.push_back("");
                    FileRegionMiniMapNames.push_back("");
                    FilePlayTimes.push_back(0);
                }
            } else {
                names[i] = CUserProfileManager::Get().GetEmptySlotName();
                m_vFileTimes[i] = CDateAndTime();
                FileRegionNames.push_back("");
                FileRegionMiniMapNames.push_back("");
                FilePlayTimes.push_back(0);
            }
        }
    } else {
        bool found_autosave = false;
        CWideString autosave_name = CUserProfileManager::Get().GetAutoSaveFileName();
        for (std::vector<CWideString>::iterator it = names.begin(); it != names.end(); ++it) {
            if (it->ToCharString().EqualsCaseInsensitive(autosave_name.ToCharString())) {
                found_autosave = true;
                break;
            }
        }
        long first_slot = 0;
        if (found_autosave)
            first_slot = 1;
        long max_slots = CUserProfileManager::Get().GetMaxSaveSlots();
        names.resize(max_slots + first_slot);
        m_vFileTimes.resize(max_slots + first_slot);
        if (found_autosave) {
            names[0] = CUserProfileManager::Get().GetAutoSaveFileName();
            CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(CUserProfileManager::Get().GetAutoSaveFileName(), false);
            if (path != L"") {
                m_vFileTimes[0] = CAFile::GetFileModifiedTime(path);
                AddRegionAndTimeInfo(path);
            } else {
                m_vFileTimes[0] = CDateAndTime();
                FileRegionNames.push_back("");
                FileRegionMiniMapNames.push_back("");
                FilePlayTimes.push_back(0);
            }
        }
        for (long i = first_slot; i < max_slots + first_slot; ++i) {
            names[i] = CUserProfileManager::Get().GetSaveSlotFileName(i - first_slot);
            if (names[i] != L"") {
                CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(names[i], false);
                if (path != L"") {
                    m_vFileTimes[i] = CAFile::GetFileModifiedTime(path);
                    AddRegionAndTimeInfo(path);
                } else {
                    m_vFileTimes[i] = CDateAndTime();
                    FileRegionNames.push_back("");
                    FileRegionMiniMapNames.push_back("");
                    FilePlayTimes.push_back(0);
                }
            } else {
                names[i] = CUserProfileManager::Get().GetEmptySlotName();
                m_vFileTimes[i] = CDateAndTime();
                FileRegionNames.push_back("");
                FileRegionMiniMapNames.push_back("");
                FilePlayTimes.push_back(0);
            }
        }
        std::vector<CDateAndTime>::iterator time_it = m_vFileTimes.begin();
        for (std::vector<CWideString>::iterator name_it = names.begin(); name_it != names.end() && time_it != m_vFileTimes.end(); ) {
            if (*name_it == L"") {
                name_it = names.erase(name_it);
                time_it = m_vFileTimes.erase(time_it);
            } else {
                ++name_it;
                ++time_it;
            }
        }
    }
}
}
