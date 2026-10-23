// CWorld::LoadGameState(const CWideString& name, bool loadNow) -- resolve a save name to a file (profile path, else the profile-folder search, else the U:\ directory search) and either load it now (with the hero state) or queue it as the pending save-game load
// Leaf: ?LoadGameState@CWorld@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A3200, 740 bytes, `ret 8` (/GS cookie; FableWin.pdb ?LoadGameState@CWorld@@QAE_NABVCWideString@@_N@Z).
// Callees: CUserProfileManager::Get 0x0040D2A0 + GetPathNameFromFileName 0x00408120, CWideString::operator!=(const wchar16*)
//   0x0099B150, CWideString() 0x0099AED0, ~CWideString 0x0099B510, operator=(const CWideString&) 0x0099B7D0,
//   operator=(const wchar16*) 0x0099B800, the profile-directory getter 0x0041A1A0 (__fastcall, wraps 0x00406780),
//   operator+(const CWideString&, const CWideString&) 0x0099BE70, PathExists 0x00999230, IsPathFile 0x00999A30,
//   FindFirstFile 0x00999760, CWorld::GetPathNameForFileName 0x0049EF50, IsHeroStateFileValid 0x0049D4E0,
//   LoadGameStateInternal 0x004A21F0, LoadHeroStateInternal 0x004A2D70; the console flag byte 0x013B8646; L"" 0x0122D70C.
// Layout: retail CWorld is 0xc below the Ego_r.pdb view here (as the landed LoadHeroStateInternal 0x004A2D70):
//   SaveGameLoadStatus +0xf8 (PDB +0x104), SaveGamePathName +0xfc (+0x108), SerialisingAboutToLoadHeroState +0x101 (+0x10d).
// Shape: the whole body sits under `if (SaveGameLoadStatus == 0 || loadNow)` (one shared false tail); the found flag is
//   declared before the profile-path call; the search buffer is 262 wchars (fixes the /GS frame to 0x228); the third arm
//   loads the profile path (as retail) but queues fullPath.
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
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;   // 0x00408120
};
extern bool g_bLoadFromAnyDirectory;                    // 0x013B8646 (console variable)

struct CWorld {
    unsigned char _pad[0xf8];
    long          SaveGameLoadStatus;                   // +0xf8
    CWideString   SaveGamePathName;                     // +0xfc
    bool          AutoSaveLocked;                       // +0x100
    bool          SerialisingAboutToLoadHeroState;      // +0x101
    CWideString GetPathNameForFileName(const CWideString& fileName) const;   // 0x0049EF50
    bool IsHeroStateFileValid(const CWideString& name) const;   // 0x0049D4E0
    bool LoadGameStateInternal(const CWideString& path, bool b);   // 0x004A21F0
    bool LoadHeroStateInternal(const CWideString& name);   // 0x004A2D70
    bool LoadGameState(const CWideString& name, bool loadNow);
};

bool CWorld::LoadGameState(const CWideString& name, bool loadNow)
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
