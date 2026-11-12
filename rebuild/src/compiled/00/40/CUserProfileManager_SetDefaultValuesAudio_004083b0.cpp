// CUserProfileManager::SetDefaultValuesAudio() -- restore the audio options (sound 0.8, music 0.6, dialogue 0.9, not muted,
//   no headphones) and apply them
// Leaf: ?SetDefaultValuesAudio@CUserProfileManager@@QAEXXZ
// NF28 (2026-11-12, queue land1011eg; repair 2): Ego_r.pdb 0x00406350 / FableWin.pdb 0x01881DC0 ?SetDefaultValuesAudio@CUserProfileManager@@QAEXXZ
//   (the SETTERS block puts it at 0x004083B0; NUISystem::CManager::Action 0x0041E6D3 already binds it here; SetDefaultValuesGameplay
//   is 0x00408360). Was the free __fastcall stand-in CUserProfileManager_SetDefaultValuesGameplay calling CUserProfileManager_Helper,
//   now the protected ?ApplyValues@CUserProfileManager@@IAEXXZ 0x00407370. Members through the shared header's AUDIODEFAULTS opt-in.
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETTERS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUDIODEFAULTS
#include "engine/CUserProfileManager.h"

void CUserProfileManager::SetDefaultValuesAudio()
{
  SoundVolume = 0.8f;
  MusicVolume = 0.6f;
  DialogueVolume = 0.9f;
  SoundsMuted = false;
  UsingHeadPhones = false;
  ApplyValues();
}
