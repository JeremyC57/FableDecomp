// CUserProfileManager::SetSaveSlotFileName(long slot, const CWideString& fileName) -- store the save slot's file name and
// save the profile
// Leaf: ?SetSaveSlotFileName@CUserProfileManager@@QAEXJABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0040BC50, 37 bytes, thiscall, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetSaveSlotFileName@CUserProfileManager@@QAEXJABVCWideString@@@Z (the only
//   CUserProfileManager member taking (long, const CWideString&)); the body assigns SaveGameNames[slot] (vector<CWideString>
//   at retail +0xC0: the STLport tutorial queue ends there, see 0x0040BB90) through CWideString::operator=(const
//   CWideString&) 0x0099B7D0 and then calls Save 0x0040A740; Ego_r.pdb places it next to Apply, as retail does (0x0040BC80).
//   Was landed as a CEngine::GetEditorAnimation stand-in (propagated catalog label).
#include "fable_string.h"
class CUserProfileManager {
public:
    unsigned char _pad00[0xC0];
    CWideString*  SaveGameNames;                // +0xC0 SaveGameNames._M_start (vector<CWideString>)
    void Save();                                // 0x0040A740
    void SetSaveSlotFileName(long slot, const CWideString& fileName);
};

void CUserProfileManager::SetSaveSlotFileName(long slot, const CWideString& fileName)
{
    SaveGameNames[slot] = fileName;
    Save();
}
