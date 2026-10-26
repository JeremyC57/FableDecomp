// CWorld::AutoSaveRegionLeaving() -- region-leaving autosave: if a quest-start autosave exists, copy it to the region-leaving slot and save the hero state next to it
// Retail: Fable.exe @ 0x004A0180, 151 bytes. PDB ?AutoSaveRegionLeaving@CWorld@@QAE_NXZ
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0; CUserProfileManager::GetAutoSaveQuestStartPathName
// 0x00407FD0, GetAutoSaveRegionLeavingPathName 0x004080B0 and GetAutoSaveRegionLeavingFileName 0x00406730 (LABEL
// CORRECTIONS: the manifest calls the first two "GetAutoSaveCheckPointPathName" and the third
// "GetAutoSaveCheckPointFileName"; their bodies build "AutoSave" (0x0122DE9C) + ".qs" (0x0122DEB0) / ".rl"
// (0x0122DEC0)); __fastcall PathExists(const CWideString&) 0x00999230; CWorld::CopyFileInternal 0x0049DD50;
// CWorld::SaveHeroStateInternal 0x004A00E0; ~CWideString 0x0099B510.
// Shape: the region-leaving path is a named local in its own block (its address is re-formed with lea) and the file
// name a full-expression temporary, both in one frame slot; the save result stays in bl
// across the temporary's and the local's destructors.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
    bool operator!=(const wchar_t* text) const;             // 0x0099B150
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30

class CUserProfileManager { public:
    static CUserProfileManager& __fastcall Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;   // 0x00408120
    CWideString GetAutoSaveQuestStartPathName() const;      // 0x00407FD0 ("AutoSave" + ".qs" path)
    CWideString GetAutoSaveRegionLeavingPathName() const;   // 0x004080B0 ("AutoSave" + ".rl" path)
    CWideString GetAutoSaveRegionLeavingFileName() const;   // 0x00406730 ("AutoSave" + ".rl")
};
bool __fastcall PathExists(const CWideString& path);         // 0x00999230

class CWorld { public:
    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;   // 0x0049DD50
    bool SaveHeroStateInternal(const CWideString& name, bool b);                            // 0x004A00E0
    bool AutoSaveRegionLeaving();
};

bool CWorld::AutoSaveRegionLeaving()
{
    CWideString questStart = CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
    if (PathExists(questStart))
    {
        {
            CWideString regionLeaving = CUserProfileManager::Get().GetAutoSaveRegionLeavingPathName();
            CopyFileInternal(questStart, regionLeaving);
        }
        return SaveHeroStateInternal(CUserProfileManager::Get().GetAutoSaveRegionLeavingFileName(), false);
    }
    return false;
}