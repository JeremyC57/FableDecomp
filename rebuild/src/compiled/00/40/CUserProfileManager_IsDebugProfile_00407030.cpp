// CUserProfileManager::IsDebugProfile() const -- false in the retail build (`xor al,al ; ret`). Ego_r.pdb / FableWin.pdb ?IsDebugProfile@CUserProfileManager@@QBE_NXZ (FableWin lays it right before DeleteProfile, retail 0x00407030 right before DeleteProfile 0x00407040); the header SAVENAMES opt-in names it; all 8 landed call sites are CUserProfileManager users (0x0047E196, 0x004804DE, 0x004830A1, 0x00597DF7, 0x00598463) and 0x00598463 binds this spelling.
// Leaf: ?IsDebugProfile@CUserProfileManager@@QBE_NXZ
// NF37 (2026-11-16): moved off its stand-in onto the shared engine/CUserProfileManager.h; include chain = the preamble of the landed 00/40/CUserProfileManager_GetManualSaveDisplayName_00406ae0.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK_INITIALISED
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVENAMES
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/CUserProfileManager.h"

bool CUserProfileManager::IsDebugProfile() const
{
    return false;
}
