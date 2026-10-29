// CWorld::AutoSaveQuestStart() -- quest-start autosave: save the game as the ".qs" quest-start file, then copy it over the plain autosave, delete the checkpoint save and clear the checkpoint flag
// Retail: Fable.exe @ 0x004A0CD0, 176 bytes. PDB ?AutoSaveQuestStart@CWorld@@QAE_NXZ
// Callees: CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0; CUserProfileManager::GetAutoSaveQuestStartFileName()
// const 0x004066B0 (manifest: "GetAutoSaveCheckPointFileName"; its body is "AutoSave" 0x0122DE9C + ".qs" 0x0122DEB0),
// GetAutoSaveQuestStartPathName() const 0x00407FD0 (manifest: "GetAutoSaveCheckPointPathName"), GetAutoSavePathName()
// const 0x00406F70; CWorld::AutoSaveInternal(const CWideString&, bool) 0x004A0850, StartAutoSaveProgress 0x0049E7C0
// (manifest: "AutoSave_StartProgress"), CopyFileInternal 0x0049DD50, static __fastcall DeleteCheckPointSave 0x0049E740,
// StopAutoSaveProgress 0x0049E8C0; ~CWideString 0x0099B510.
// Retail CWorld offsets: MostRecentSaveType +0x134 (PDB +0x144), MostRecentSaveTypeBeforeManualSave +0x138 (PDB +0x148),
// AutoSaveCheckPointExists +0x140 (PDB +0x150).
// Shape: the save result is held in bl across the file-name temporary's destructor; both paths are named locals.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                          // 0x0099B510
};

class CUserProfileManager { public:
    static CUserProfileManager& __fastcall Get();            // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    CWideString GetAutoSaveQuestStartFileName() const;       // 0x004066B0 ("AutoSave" + ".qs")
    CWideString GetAutoSavePathName() const;                 // 0x00406F70 ("AutoSave" path)
    CWideString GetAutoSaveQuestStartPathName() const;       // 0x00407FD0 ("AutoSave" + ".qs" path)
};

class CWorld { public:
    unsigned char _pad0[0x134];
    long          MostRecentSaveType;                         // +0x134 ESaveType (PDB +0x144)
    long          MostRecentSaveTypeBeforeManualSave;         // +0x138 ESaveType (PDB +0x148)
    unsigned char _pad13c[4];                                 // +0x13C MostRecentManualSaveName
    bool          AutoSaveCheckPointExists;                   // +0x140 (PDB +0x150)

protected:
    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;   // 0x0049DD50
public:
    void StartAutoSaveProgress();                                                           // 0x0049E7C0
    void StopAutoSaveProgress();                                                            // 0x0049E8C0
    static void __fastcall DeleteCheckPointSave();                                          // 0x0049E740
    bool AutoSaveInternal(const CWideString& name, bool b);                                 // 0x004A0850
    bool AutoSaveQuestStart();
};

bool CWorld::AutoSaveQuestStart()
{
    MostRecentSaveType = 3;
    MostRecentSaveTypeBeforeManualSave = 3;
    bool saved = AutoSaveInternal(CUserProfileManager::Get().GetAutoSaveQuestStartFileName(), false);
    if (saved)
    {
        StartAutoSaveProgress();
        CWideString questStart = CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
        CWideString autoSave = CUserProfileManager::Get().GetAutoSavePathName();
        CopyFileInternal(questStart, autoSave);
        DeleteCheckPointSave();
        AutoSaveCheckPointExists = false;
        StopAutoSaveProgress();
        return true;
    }
    return false;
}