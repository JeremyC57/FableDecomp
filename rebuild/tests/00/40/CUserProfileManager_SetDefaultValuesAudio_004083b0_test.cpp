#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SETTERS
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUDIODEFAULTS
#include "engine/CUserProfileManager.h"

static int g_apply_called = 0;

void CUserProfileManager::ApplyValues()
{
  g_apply_called = 1;
}

void CUserProfileManager::SetDefaultValuesAudio()
{
  SoundVolume = 0.8f;
  MusicVolume = 0.6f;
  DialogueVolume = 0.9f;
  SoundsMuted = false;
  UsingHeadPhones = false;
  ApplyValues();
}

int main()
{
  static unsigned char storage[0x140];
  std::memset(storage, 0x55, sizeof(storage));
  CUserProfileManager& m = *reinterpret_cast<CUserProfileManager*>(storage);
  m.SoundVolume = m.MusicVolume = m.DialogueVolume = 1.0f;
  m.SetDefaultValuesAudio();
  if (m.SoundVolume == 0.8f && m.MusicVolume == 0.6f && m.DialogueVolume == 0.9f &&
      storage[0x30] == 0 && storage[0x31] == 0 && storage[0x32] == 0x55 && storage[0x23] == 0x55 &&
      g_apply_called == 1) {
    std::printf("CUserProfileManager_004083b0_TEST PASS\n");
    return 0;
  }
  std::printf("CUserProfileManager_004083b0_TEST FAIL\n");
  return 1;
}
