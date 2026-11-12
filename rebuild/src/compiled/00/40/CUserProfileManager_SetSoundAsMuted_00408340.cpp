// CUserProfileManager::SetSoundAsMuted(bool muted) -- store the muted flag when it changes and re-apply the profile's values
// Leaf: ?SetSoundAsMuted@CUserProfileManager@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00408340, `ret 4`.
// Flags: none
// Name-fix pass (2026-11-12): Ego_r.pdb 0x004062F0 + FableWin.pdb 0x0187E2D0 ?SetSoundAsMuted@CUserProfileManager@@QAEX_N@Z (bool, not
//   char); the row now uses the shared engine/CUserProfileManager.h (AUDIODEFAULTS opt-in: SoundsMuted +0x31 and the protected
//   ApplyValues 0x00407370; the declaration-only SETSOUNDASMUTED opt-in, hdr/README.md) in place of its local CUserProfileManager model.
// Callees: CUserProfileManager::ApplyValues 0x00407370 (protected).
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETTERS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETSOUNDASMUTED
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUDIODEFAULTS
#include "engine/CUserProfileManager.h"

void CUserProfileManager::SetSoundAsMuted(bool muted)
{
    if (SoundsMuted != muted) {
        SoundsMuted = muted;
        ApplyValues();
    }
}
