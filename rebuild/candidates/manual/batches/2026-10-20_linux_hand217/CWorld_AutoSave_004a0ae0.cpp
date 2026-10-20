// CWorld::AutoSave(bool) -- autosave: between the quest start and finish screens reuse the quest-start save as the autosave (copied with the progress display), otherwise (or if no quest-start save exists) run a full autosave
// Retail: Fable.exe @ 0x004A0AE0, 195 bytes. PDB ?AutoSave@CWorld@@QAE_N_N@Z
// Callees: quest manager singleton 0x013B89FC IsBetweenQuestStartAndFinishScreens() 0x004AF290;
// CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0; CUserProfileManager::GetAutoSaveQuestStartPathName() const
// 0x00407FD0 (manifest: "GetAutoSaveCheckPointPathName"), GetAutoSavePathName() const 0x00406F70,
// GetAutoSaveFileName() const 0x00406690; __fastcall PathExists(const CWideString&) 0x00999230;
// CWorld::StartAutoSaveProgress 0x0049E7C0 (manifest: "AutoSave_StartProgress"), CopyFileInternal 0x0049DD50,
// StopAutoSaveProgress 0x0049E8C0, AutoSaveInternal(const CWideString&, bool) 0x004A0850; ~CWideString 0x0099B510.
// Retail CWorld offsets: MostRecentSaveType +0x134 (PDB +0x144), MostRecentSaveTypeBeforeManualSave +0x138 (PDB +0x148).
// Shape: the result is a bool local (bl) set inside the copy block before the autosave path's destructor; the
// quest-start path reuses the dead bool argument slot, the autosave path the frame word.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                          // 0x0099B510
};

struct CUserProfileManager {
    static CUserProfileManager& __fastcall Get();            // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    CWideString GetAutoSaveFileName() const;                 // 0x00406690 ("AutoSave")
    CWideString GetAutoSavePathName() const;                 // 0x00406F70 ("AutoSave" path)
    CWideString GetAutoSaveQuestStartPathName() const;       // 0x00407FD0 ("AutoSave" + ".qs" path)
};
bool __fastcall PathExists(const CWideString& path);          // 0x00999230

struct CQuestManager { bool IsBetweenQuestStartAndFinishScreens() const; };   // 0x004AF290
extern CQuestManager* g_pQuestManager;                        // 0x013B89FC

struct CWorld {
    unsigned char _pad0[0x134];
    long          MostRecentSaveType;                         // +0x134 ESaveType (PDB +0x144)
    long          MostRecentSaveTypeBeforeManualSave;         // +0x138 ESaveType (PDB +0x148)

    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;   // 0x0049DD50
    void StartAutoSaveProgress();                                                           // 0x0049E7C0
    void StopAutoSaveProgress();                                                            // 0x0049E8C0
    bool AutoSaveInternal(const CWideString& name, bool b);                                 // 0x004A0850
    bool AutoSave(bool ignoreQuestStart);
};

bool CWorld::AutoSave(bool ignoreQuestStart)
{
    MostRecentSaveType = 2;
    MostRecentSaveTypeBeforeManualSave = 2;
    bool saved = false;
    if (!ignoreQuestStart && g_pQuestManager->IsBetweenQuestStartAndFinishScreens())
    {
        CWideString questStart = CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
        if (PathExists(questStart))
        {
            StartAutoSaveProgress();
            CWideString autoSave = CUserProfileManager::Get().GetAutoSavePathName();
            CopyFileInternal(questStart, autoSave);
            StopAutoSaveProgress();
            saved = true;
        }
    }
    if (!saved)
        saved = AutoSaveInternal(CUserProfileManager::Get().GetAutoSaveFileName(), true);
    return saved;
}
