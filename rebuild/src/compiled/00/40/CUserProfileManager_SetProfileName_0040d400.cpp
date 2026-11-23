#define FABLE_RETAIL_CUSERPROFILEMANAGER_LOAD_PUBLIC   // NF293: Ego_r + FableWin ?Load@CUserProfileManager@@QAEXXZ (public)
#include "fable_user_profile.h"

void CUserProfileManager::SetProfileName(
    const CWideString& profileName,
    bool loadProfile)
{
    profileName_ = profileName;
    if (loadProfile)
        Load();
    else
        Save();
}
