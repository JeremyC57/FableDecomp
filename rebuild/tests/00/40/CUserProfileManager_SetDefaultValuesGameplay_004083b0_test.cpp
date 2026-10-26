#include <cstdio>

class CUserProfileManager { public:
  char pad0[0x24];
  float SoundVolume;
  float MusicVolume;
  float DialogueVolume;
  unsigned char UsingHeadPhones;
  unsigned char SoundsMuted;
};

static int g_helper_called = 0;

void __fastcall CUserProfileManager_Helper(CUserProfileManager* manager)
{
  g_helper_called = 1;
  (void)manager;
}

void __fastcall CUserProfileManager_SetDefaultValuesGameplay(CUserProfileManager* manager)
{
  manager->SoundVolume = 0.8f;
  manager->MusicVolume = 0.6f;
  manager->DialogueVolume = 0.9f;
  manager->SoundsMuted = 0;
  manager->UsingHeadPhones = 0;
  CUserProfileManager_Helper(manager);
}

int main()
{
  CUserProfileManager m;
  m.SoundVolume = m.MusicVolume = m.DialogueVolume = 1.0f;
  m.UsingHeadPhones = m.SoundsMuted = 0x55;
  CUserProfileManager_SetDefaultValuesGameplay(&m);
  if (m.SoundVolume == 0.8f && m.MusicVolume == 0.6f && m.DialogueVolume == 0.9f &&
      m.UsingHeadPhones == 0 && m.SoundsMuted == 0 && g_helper_called == 1) {
    std::printf("CUserProfileManager_004083b0_TEST PASS\n");
    return 0;
  }
  std::printf("CUserProfileManager_004083b0_TEST FAIL\n");
  return 1;
}