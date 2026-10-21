// CWorld::LoadHeroStateInternal(const CWideString& name) -- load "<name>.hs" from the profile directory (a missing file is fine) with SerialisingHeroState latched for the load; a loaded region-leaving autosave companion is then deleted from the drive
// Leaf: ?LoadHeroStateInternal@CWorld@@IAE_NABVCWideString@@@Z (Ego_r.pdb, protected; modelled public)
// Retail: Fable.exe @ 0x004A2D70, 277 bytes, `ret 4`.
// Body: file = name + L".hs" (__fastcall operator+ 0x0099BF30, L".hs" 0x01236A74); path = CUserProfileManager::Get()
// (0x0040D2A0).GetPathNameFromFileName(file, false) (0x00408120); !PathExists(path) (__fastcall 0x00999230) -> true.
// Else SerialisingHeroState = true; loaded = LoadGameStateInternal(path, false) (0x004A21F0); latch false; if loaded:
// autosave = Get().GetAutoSaveRegionLeavingPathName() (0x004080B0, builds "AutoSave" + ".rl") + L".hs"; inline
// CWideString == (same block, null tests, STLport size compare, MemCmp_Unsigned16 0x00403C60); equal ->
// g_pDriveManager->DeleteFile(autosave) (0x009E4BC0, static counted pointer 0x013CAA28). ~CWideString 0x0099B510.
// Layout: CWorld SerialisingHeroState +0x102 (PDB +0x10E), as the landed SaveHeroStateInternal 0x004A00E0.
// Names: 0x004080B0 is GetAutoSaveRegionLeavingPathName (".rl" literal 0x0122DEC0); the landed 0x004A00E0 header calls it
// GetAutoSaveCheckPointPathName (wrong).
// Shape: the inline == is the STLport `size() == size() && compare(..., size())` form (a named length or a
// separate size test rotates esi/ecx); path reuses the dead `name` argument slot; the load result stays in bl across the three destructors; the
// missing-file exit (nested success path) returns true with its own destructor pair at the end.
// Opt: /O2 (16-aligned entry).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

int __cdecl MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count);   // 0x00403C60
struct CWideStringData {                                // STLport basic_string<wchar_t> + NoRefs
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    long            NoRefs;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                     // 0x0099B510
    __forceinline bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return PStringData->size() == other.PStringData->size()
            && MemCmp_Unsigned16(PStringData->_M_start, other.PStringData->_M_start, PStringData->size()) == 0;
    }
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30
bool __fastcall PathExists(const CWideString& path);   // 0x00999230

struct CUserProfileManager {
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;   // 0x00408120
    CWideString GetAutoSaveRegionLeavingPathName() const;   // 0x004080B0
};
struct CDriveManager {
    long DeleteFile(const CWideString& path);           // 0x009E4BC0
};
struct CDriveManagerPointer {                           // static CCountedPointer<CDriveManager>
    CDriveManager* Object;
    void*          Info;
    CDriveManager* operator->() const { return Object; }
};
extern CDriveManagerPointer g_pDriveManager;            // 0x013CAA28

struct CWorld {
    unsigned char _pad[0x102];
    bool          SerialisingHeroState;                 // +0x102 (PDB +0x10E)
    bool LoadGameStateInternal(const CWideString& path, bool b);   // 0x004A21F0
    bool LoadHeroStateInternal(const CWideString& name);
};

bool CWorld::LoadHeroStateInternal(const CWideString& name)
{
    CWideString file = name + L".hs";
    CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(file, false);
    if (PathExists(path))
    {
        SerialisingHeroState = true;
        bool loaded = LoadGameStateInternal(path, false);
        SerialisingHeroState = false;
        if (loaded)
        {
            CWideString autosave = CUserProfileManager::Get().GetAutoSaveRegionLeavingPathName() + L".hs";
            if (path == autosave)
                g_pDriveManager->DeleteFile(autosave);
        }
        return loaded;
    }
    return true;
}