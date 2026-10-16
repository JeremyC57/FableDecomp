// CWorld::SaveHeroStateInternal(const CWideString&, bool) -- save only the hero state to "<name>.hs" in the profile directory
// Retail: Fable.exe @ 0x004A00E0, 147 bytes. PDB ?SaveHeroStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z (protected)
// Callees: __fastcall operator+(const CWideString&, const wchar_t*) 0x0099BF30 (L".hs" 0x01236A74),
// CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, CUserProfileManager::GetPathNameFromFileName(const CWideString&,
// bool) const 0x00408120, CWideString::operator!=(const wchar_t*) 0x0099B150 (L"" 0x0122D70C),
// CWorld::SaveGameStateInternal(const CWideString&, bool) 0x0049F4C0, ~CWideString 0x0099B510.
// CWorld: SerialisingHeroState +0x102 (PDB +0x10E; retail containers before it are 12 bytes, not 16).
// Shape: the path string reuses the dead `name` argument slot; the save result is kept in bl across the two dtors.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                         // 0x0099B510
    bool operator!=(const wchar_t* text) const;             // 0x0099B150
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30

struct CUserProfileManager {
    static CUserProfileManager& __fastcall Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;   // 0x00408120
    CWideString GetAutoSaveRegionLeavingPathName() const;   // 0x00407FD0
    CWideString GetAutoSaveCheckPointPathName() const;      // 0x004080B0
    CWideString GetAutoSaveRegionLeavingFileName() const;   // 0x00406730
};

struct CWorld {
    unsigned char _pad[0x102];
    bool          SerialisingHeroState;             // +0x102 (PDB +0x10E)
    bool SaveGameStateInternal(const CWideString& path, bool b);   // 0x0049F4C0
    bool SaveHeroStateInternal(const CWideString& name, bool b);
};

bool CWorld::SaveHeroStateInternal(const CWideString& name, bool b)
{
    CWideString file = name + L".hs";
    CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(file, false);
    if (path != L"")
    {
        SerialisingHeroState = true;
        bool saved = SaveGameStateInternal(path, b);
        SerialisingHeroState = false;
        return saved;
    }
    return false;
}