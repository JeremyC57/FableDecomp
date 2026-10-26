class CUserProfileManager { public:
  char pad0[0xC];
  char UseNewProfileValues;       /* +0x0C */
  char pad0d[0x14-0x0D];
  char CameraPCBowYZInverted;     /* +0x14 */
  char pad15[0xD4-0x15];
  char CameraPCBowYZInvertedNewProfile;     /* +0xD4 */
};

extern void __fastcall CUserProfileManager_Sub1(CUserProfileManager* manager);
extern void __fastcall CUserProfileManager_Sub2(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetCameraPCBowYZInverted(CUserProfileManager* manager, int unused, char a, char b)
{
  if (manager->UseNewProfileValues)
    manager->CameraPCBowYZInvertedNewProfile = a;
  manager->CameraPCBowYZInverted = a;
  CUserProfileManager_Sub1(manager);
  if (b)
    CUserProfileManager_Sub2(manager);
}