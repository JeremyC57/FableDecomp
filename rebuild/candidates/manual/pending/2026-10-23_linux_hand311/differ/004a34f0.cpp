// Grade: DIFFER(564v564) -- instructions identical; only frame slots swap: retail heroState/fullPath share esp+8 and path takes +0xc, ours the reverse
// CWorld::AutoLoadRegionLeaving() -- region-leaving auto-load: save the region-leaving autosave, delete the quest-start save (and its ".hs" hero state), then queue the "AutoSave.rl" save for loading
// Leaf: ?AutoLoadRegionLeaving@CWorld@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A34F0, 564 bytes, plain `ret` (/GS cookie; FableWin.pdb ?AutoLoadRegionLeaving@CWorld@@QAE_NXZ).
// Callees: CWorld::AutoSaveRegionLeaving 0x004A0180, CUserProfileManager::Get 0x0040D2A0, GetAutoSaveQuestStartPathName
//   0x00407FD0 and GetAutoSaveRegionLeavingFileName 0x00406730 (manifest labels "GetAutoSaveCheckPoint*" are propagated and
//   wrong, as recorded by the landed AutoSaveRegionLeaving 0x004A0180), PathExists 0x00999230, g_pDriveManager 0x013CAA28
//   DeleteFile 0x009E4BC0, operator+(const CWideString&, const wchar16*) 0x0099BF30 with L".hs" 0x01236A74, and the whole
//   of CWorld::LoadGameState(name, false) 0x004A3200 inlined (its callees as listed there).
// Layout: retail CWorld offsets as the landed LoadHeroStateInternal 0x004A2D70 (SaveGameLoadStatus +0xf8, SaveGamePathName +0xfc).
// Shape: LoadGameState is the landed 0x004A3200 body made __forceinline (loadNow folds to false); 262-wchar search buffer.
typedef unsigned short wchar16;
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    const CWideString& operator=(const wchar16* text);  // 0x0099B800
    bool operator!=(const wchar16* text) const;         // 0x0099B150
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
CWideString __fastcall GetProfileDirectory();          // 0x0041A1A0
bool __fastcall PathExists(const CWideString& path);   // 0x00999230
bool __fastcall IsPathFile(const CWideString& path);   // 0x00999A30
struct CFileFinder { void* Handle; unsigned long Flags; };
bool __fastcall FindFirstFile(const CWideString& pattern, CFileFinder* finder, wchar16* fileName);   // 0x00999760
struct CUserProfileManager {
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0
    CWideString GetAutoSaveRegionLeavingFileName() const;   // 0x00406730 ("AutoSave" + ".rl")
    CWideString GetAutoSaveQuestStartPathName() const;  // 0x00407FD0 ("AutoSave" + ".qs" path)
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;   // 0x00408120
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);   // 0x0099BF30
struct CDriveManager {
    long DeleteFile(const CWideString& path);           // 0x009E4BC0
};
struct CDriveManagerPointer {                           // static CCountedPointer<CDriveManager>
    CDriveManager* Object;
    void*          Info;
    CDriveManager* operator->() const { return Object; }
};
extern CDriveManagerPointer g_pDriveManager;            // 0x013CAA28
extern bool g_bLoadFromAnyDirectory;                    // 0x013B8646 (console variable)

struct CWorld {
    unsigned char _pad[0xf8];
    long          SaveGameLoadStatus;                   // +0xf8
    CWideString   SaveGamePathName;                     // +0xfc
    bool          AutoSaveLocked;                       // +0x100
    bool          SerialisingAboutToLoadHeroState;      // +0x101
    CWideString GetPathNameForFileName(const CWideString& fileName) const;   // 0x0049EF50
    bool IsHeroStateFileValid(const CWideString& name) const;   // 0x0049D4E0
    bool AutoSaveRegionLeaving();                       // 0x004A0180
    bool LoadGameStateInternal(const CWideString& path, bool b);   // 0x004A21F0
    bool LoadHeroStateInternal(const CWideString& name);   // 0x004A2D70
    bool LoadGameState(const CWideString& name, bool loadNow);   // 0x004A3200 (inlined here)
    bool AutoLoadRegionLeaving();
};

__forceinline bool CWorld::LoadGameState(const CWideString& name, bool loadNow)
{
    if (SaveGameLoadStatus == 0 || loadNow) {
        bool found = false;
        CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(name, false);
        if (path != L"")
            found = true;
        if (g_bLoadFromAnyDirectory) {
            if (!found)
                return false;
            if (loadNow) {
                SerialisingAboutToLoadHeroState = IsHeroStateFileValid(name);
                bool loaded = LoadGameStateInternal(path, false);
                SerialisingAboutToLoadHeroState = false;
                if (!loaded)
                    return false;
                return LoadHeroStateInternal(name);
            }
            SaveGameLoadStatus = 1;
            SaveGamePathName = path;
            return true;
        }
        if (found && PathExists(path) && IsPathFile(path)) {
            if (loadNow) {
                SerialisingAboutToLoadHeroState = IsHeroStateFileValid(name);
                bool loaded = LoadGameStateInternal(path, false);
                SerialisingAboutToLoadHeroState = false;
                if (!loaded)
                    return false;
                return LoadHeroStateInternal(name);
            }
            SaveGameLoadStatus = 1;
            SaveGamePathName = path;
            return true;
        }
        CWideString fullPath;
        CWideString pattern = GetProfileDirectory() + name;
        CFileFinder finder;
        wchar16 fileName[262];
        if (FindFirstFile(pattern, &finder, fileName))
            fullPath = fileName;
        else
            fullPath = GetPathNameForFileName(name);
        if (fullPath != L"") {
            if (loadNow) {
                SerialisingAboutToLoadHeroState = IsHeroStateFileValid(name);
                bool loaded = LoadGameStateInternal(path, false);   // retail passes the profile path here, not fullPath
                SerialisingAboutToLoadHeroState = false;
                if (!loaded)
                    return false;
                return LoadHeroStateInternal(name);
            }
            SaveGameLoadStatus = 1;
            SaveGamePathName = fullPath;
            return true;
        }
    }
    return false;
}

bool CWorld::AutoLoadRegionLeaving()
{
    AutoSaveRegionLeaving();
    CWideString questStart = CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
    if (PathExists(questStart)) {
        g_pDriveManager->DeleteFile(questStart);
        CWideString heroState = questStart + L".hs";
        if (PathExists(heroState))
            g_pDriveManager->DeleteFile(heroState);
    }
    CWideString name = CUserProfileManager::Get().GetAutoSaveRegionLeavingFileName();
    return LoadGameState(name, false);
}
