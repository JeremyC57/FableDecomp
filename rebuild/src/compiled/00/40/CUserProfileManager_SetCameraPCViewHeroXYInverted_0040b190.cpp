struct CUserProfileManager {
  char pad0[0xc];
  char UseNewProfileValues;    /* +0xc */
  char pad1[0x8];
  char CameraPCViewHeroXYInverted;   /* +0x15 */
  char pad2[0xbf];
  char CameraPCViewHeroXYInvertedNewProfile;   /* +0xd5 */
};
extern void __fastcall CUserProfileManager_HelperA(CUserProfileManager* manager);
extern void __fastcall CUserProfileManager_HelperB(CUserProfileManager* manager);
void __fastcall CUserProfileManager_SetCameraPCViewHeroXYInverted(CUserProfileManager* manager, void* edx, char resetting, char reload)
{
  (void)edx;
  if (manager->UseNewProfileValues) {
    manager->CameraPCViewHeroXYInvertedNewProfile = resetting;
  }
  manager->CameraPCViewHeroXYInverted = resetting;
  CUserProfileManager_HelperA(manager);
  if (reload) {
    CUserProfileManager_HelperB(manager);
  }
}