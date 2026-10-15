// CUserProfileManager::SetProfileName(const CWideString&, bool) -- assign the profile name (+0x08), then load that profile or save the current one
// Retail: Fable.exe @ 0x0040D400, 44 bytes, `ret 8`.
// Callees: CWideString::operator=(const CWideString&) @ 0x0099B7D0, CUserProfileManager::Load @ 0x0040D350,
// CUserProfileManager::Save @ 0x0040A740.
// Shape: an if/else of two calls; VC duplicates the `pop esi ; ret 8` epilogue into both arms. Same body as the
// landed 0x0040D400 source (fable_user_profile.h); modelled here with the two leading vtable words of the retail class.
#include "fable_string.h"

struct CUserProfileManager {
    void* _vtables[2];          // +0x00 / +0x04
    CWideString ProfileName;    // +0x08
    void Load();                // 0x0040D350
    void Save();                // 0x0040A740
    void SetProfileName(const CWideString& profileName, bool load);
};

void CUserProfileManager::SetProfileName(const CWideString& profileName, bool load)
{
    ProfileName = profileName;
    if (load)
        Load();
    else
        Save();
}
