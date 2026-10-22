struct CUserProfileManager {
  char pad0[0x24];
  float SoundVolume;
  float MusicVolume;
  float DialogueVolume;
  unsigned char UsingHeadPhones;
  unsigned char SoundsMuted;
};

extern void __fastcall CUserProfileManager_Helper(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetDefaultValuesGameplay(CUserProfileManager* manager)
{
  manager->SoundVolume = 0.8f;
  manager->MusicVolume = 0.6f;
  manager->DialogueVolume = 0.9f;
  manager->SoundsMuted = 0;
  manager->UsingHeadPhones = 0;
  CUserProfileManager_Helper(manager);
}