// CWorld::IsHeroStateFileValid(const CWideString& file_name) -- does the hero-state file ("<name>.hs" in the user profile folder) exist
// Leaf: ?IsHeroStateFileValid@CWorld@@IBE_NABVCWideString@@@Z
// Retail: Fable.exe @ 0x0049D4E0, 76 bytes, `ret 4`; callers 0x004A2FFE, 0x004A3280, 0x004A32D6, 0x004A33F9; Ego_r.pdb
// ?IsHeroStateFileValid@CWorld@@IBE_NABVCWideString@@@Z.
// Body: file_name + L".hs" (0x0099BF30, __fastcall operator+ with the hidden result in ecx; literal 0x01236A74), then
// CTBaseSingleton<CUserProfileManager>::Get() (0x0040D2A0, catalog-labelled CCodeSectionManager -- folded/wrong) ->
// GetPathNameFromFileName(that, false) (0x00408120, Ego_r.pdb ?GetPathNameFromFileName@CUserProfileManager@@QBE?AVCWideString@@ABV2@_N@Z;
// the result built in the dead parameter slot), CAFile::PathExists on it (0x00999230, static __fastcall); both temporaries
// destroyed (~CWideString 0x0099B510), the answer held in bl.
// Shape: /O2 (16-aligned entry); the path is a named local (the suffixed temporary dies before PathExists).
#include <stddef.h>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30
class CUserProfileManager {
public:
    CWideString GetPathNameFromFileName(const CWideString& file_name, bool create_folder) const;   // 0x00408120
public:
    static CUserProfileManager& __fastcall Get();   // 0x0040D2A0
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
class CAFile {
public:
    static bool __fastcall PathExists(const CWideString& path);   // 0x00999230
};
class CWorld {
public:
    bool IsHeroStateFileValid(const CWideString& file_name) const;
};
bool CWorld::IsHeroStateFileValid(const CWideString& file_name) const
{
    const CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(file_name + L".hs", false);
    return CAFile::PathExists(path);
}