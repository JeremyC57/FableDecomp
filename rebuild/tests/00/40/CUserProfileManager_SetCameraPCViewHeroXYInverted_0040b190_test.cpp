#include <cstdio>
class CUserProfileManager { public:
  char pad0[0xc];
  char UseNewProfileValues;
  char pad1[0x8];
  char CameraPCViewHeroXYInverted;
  char pad2[0xbf];
  char CameraPCViewHeroXYInvertedNewProfile;
};
static int g_a=0, g_b=0;
void CUserProfileManager_HelperA(CUserProfileManager* manager){ (void)manager; g_a++; }
void CUserProfileManager_HelperB(CUserProfileManager* manager){ (void)manager; g_b++; }
void CUserProfileManager_SetCameraPCViewHeroXYInverted(CUserProfileManager* manager, char resetting, char reload)
{
  if (manager->UseNewProfileValues) { manager->CameraPCViewHeroXYInvertedNewProfile = resetting; }
  manager->CameraPCViewHeroXYInverted = resetting;
  CUserProfileManager_HelperA(manager);
  if (reload) { CUserProfileManager_HelperB(manager); }
}
int main(){
  CUserProfileManager o; o.UseNewProfileValues=1; o.CameraPCViewHeroXYInvertedNewProfile=0; o.CameraPCViewHeroXYInverted=0;
  g_a=g_b=0;
  CUserProfileManager_SetCameraPCViewHeroXYInverted(&o, 7, 1);
  if (o.CameraPCViewHeroXYInvertedNewProfile!=7) { std::printf("FAIL flagD5\n"); return 1; }
  if (o.CameraPCViewHeroXYInverted!=7) { std::printf("FAIL flag15\n"); return 1; }
  if (g_a!=1 || g_b!=1) { std::printf("FAIL calls\n"); return 1; }
  CUserProfileManager o2; o2.UseNewProfileValues=0; o2.CameraPCViewHeroXYInvertedNewProfile=3; o2.CameraPCViewHeroXYInverted=0;
  g_a=g_b=0;
  CUserProfileManager_SetCameraPCViewHeroXYInverted(&o2, 9, 0);
  if (o2.CameraPCViewHeroXYInvertedNewProfile!=3) { std::printf("FAIL flagD5 skip\n"); return 1; }
  if (o2.CameraPCViewHeroXYInverted!=9) { std::printf("FAIL flag15 b\n"); return 1; }
  if (g_a!=1 || g_b!=0) { std::printf("FAIL calls b\n"); return 1; }
  std::printf("CUserProfileManager_0040b190_TEST PASS\n");
  return 0;
}