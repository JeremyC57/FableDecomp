struct CUserProfileManager {
  char pad0[0x24];
  float SoundVolume;
  float MusicVolume;
  float DialogueVolume;
  unsigned char UsingHeadPhones;
  unsigned char SoundsMuted;
};

extern void __fastcall CUserProfileManager_Helper(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetDefaultValuesGameplay(CUserProfileManager* self)
{
  self->SoundVolume = 0.8f;
  self->MusicVolume = 0.6f;
  self->DialogueVolume = 0.9f;
  self->SoundsMuted = 0;
  self->UsingHeadPhones = 0;
  CUserProfileManager_Helper(self);
}