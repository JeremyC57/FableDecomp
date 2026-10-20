// CWorld::SaveGameStateManual(const CWideString&) -- manual save: between the quest start and finish screens copy the quest-start (or plain) autosave next to the named slot and save only the hero state, otherwise save the whole game; then record the manual save
// Retail: Fable.exe @ 0x004A0780, 200 bytes. PDB ?SaveGameStateManual@CWorld@@QAEXABVCWideString@@@Z
// Callees: quest manager singleton 0x013B89FC IsBetweenQuestStartAndFinishScreens() 0x004AF290;
// CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0; CUserProfileManager::GetPathNameFromFileName(const CWideString&,
// bool) const 0x00408120, GetAutoSavePathName() const 0x00406F70, GetAutoSaveQuestStartPathName() const 0x00407FD0
// (manifest: "GetAutoSaveCheckPointPathName"); __fastcall PathExists(const CWideString&) 0x00999230;
// CWorld::CopyFileInternal 0x0049DD50, SaveHeroStateInternal 0x004A00E0, SaveGameState(const CWideString&, bool, bool)
// 0x004A05C0; ~CWideString 0x0099B510; CWideString::operator=(const CWideString&) 0x0099B7D0.
// Retail CWorld offsets (the RegionLoad block's vector is 12 bytes, so everything after it sits 0x10 below the PDB):
// SerialisingNonPersistentQuestItems +0x103 (PDB +0x10F), MostRecentSaveType +0x134 (PDB +0x144),
// MostRecentSaveTypeBeforeManualSave +0x138 (PDB +0x148), MostRecentManualSaveName +0x13C (PDB +0x14C).
// Grade: DIFFER(ours v retail) -- 205v200: the arms of the inline source-path helper push the hidden result pointer through
// ecx / edx, so VC7.1 does not hoist retail's shared `lea ecx ; push ecx` above the jne; the CopyFileInternal
// argument leas are scheduled lea/push/lea/push where retail does lea/lea/push/push
// Shape: the destination path is a named local; the autosave source chosen by an inline if/return helper is built in the dead `name`
// argument slot inside its own block; the trailing `false` of GetPathNameFromFileName and of SaveGameState is one
// shared `push 0` hoisted above the branch.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                          // 0x0099B510
    CWideString& operator=(const CWideString& other);        // 0x0099B7D0
};

struct CUserProfileManager {
    static CUserProfileManager& __fastcall Get();            // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;   // 0x00408120
    CWideString GetAutoSavePathName() const;                 // 0x00406F70 ("AutoSave" path)
    CWideString GetAutoSaveQuestStartPathName() const;       // 0x00407FD0 ("AutoSave" + ".qs" path)
};
bool __fastcall PathExists(const CWideString& path);          // 0x00999230

struct CQuestManager { bool IsBetweenQuestStartAndFinishScreens() const; };   // 0x004AF290
extern CQuestManager* g_pQuestManager;                        // 0x013B89FC

struct CWorld {
    unsigned char _pad0[0x103];
    bool          SerialisingNonPersistentQuestItems;         // +0x103 (PDB +0x10F)
    unsigned char _pad104[0x30];
    long          MostRecentSaveType;                         // +0x134 ESaveType (PDB +0x144)
    long          MostRecentSaveTypeBeforeManualSave;         // +0x138 ESaveType (PDB +0x148)
    CWideString   MostRecentManualSaveName;                   // +0x13C (PDB +0x14C)

    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;   // 0x0049DD50
    bool SaveHeroStateInternal(const CWideString& name, bool b);                            // 0x004A00E0
    void SaveGameState(const CWideString& name, bool questScreens, bool b);                 // 0x004A05C0
    void SaveGameStateManual(const CWideString& name);
    CWideString GetManualSaveSourcePathName() const
    {
        if (MostRecentSaveTypeBeforeManualSave == 2)
            return CUserProfileManager::Get().GetAutoSavePathName();
        return CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
    }
};

void CWorld::SaveGameStateManual(const CWideString& name)
{
    if (g_pQuestManager->IsBetweenQuestStartAndFinishScreens())
    {
        CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(name, false);
        {
            CWideString source = GetManualSaveSourcePathName();
            if (PathExists(source))
                CopyFileInternal(source, path);
        }
        SerialisingNonPersistentQuestItems = false;
        SaveHeroStateInternal(name, false);
        SerialisingNonPersistentQuestItems = true;
    }
    else
        SaveGameState(name, false, false);
    MostRecentSaveType = 1;
    MostRecentManualSaveName = name;
}
