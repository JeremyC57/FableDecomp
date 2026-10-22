#include <cstdio>

struct CUserProfileManager {
  char pad0[0x24];
  float SoundVolume;
  float MusicVolume;
  float DialogueVolume;
  unsigned char UsingHeadPhones;
  unsigned char SoundsMuted;
};

static int g_helper_called = 0;

void __fastcall CUserProfileManager_Helper(CUserProfileManager* self)
{
  g_helper_called = 1;
  (void)self;
}

void __fastcall CUserProfileManager_SetDefaultValuesGameplay(CUserProfileManager* self)
{
  self->SoundVolume = 0.8f;
  self->MusicVolume = 0.6f;
  self->DialogueVolume = 0.9f;
  self->SoundsMuted = 0;
  self->UsingHeadPhones = 0;
  CUserProfileManager_Helper(self);
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