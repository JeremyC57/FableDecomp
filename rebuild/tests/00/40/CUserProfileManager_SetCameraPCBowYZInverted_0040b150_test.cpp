#include <cstdio>

struct CUserProfileManager {
  char pad0[0xC];
  char UseNewProfileValues;
  char pad0d[0x14-0x0D];
  char CameraPCBowYZInverted;
  char pad15[0xD4-0x15];
  char CameraPCBowYZInvertedNewProfile;
};

static int g_sub1 = 0;
static int g_sub2 = 0;

void __fastcall CUserProfileManager_Sub1(CUserProfileManager* manager) { (void)manager; g_sub1++; }
void __fastcall CUserProfileManager_Sub2(CUserProfileManager* manager) { (void)manager; g_sub2++; }

void __fastcall CUserProfileManager_SetCameraPCBowYZInverted(CUserProfileManager* manager, int unused, char a, char b)
{
  (void)unused;
  if (manager->UseNewProfileValues)
    manager->CameraPCBowYZInvertedNewProfile = a;
  manager->CameraPCBowYZInverted = a;
  CUserProfileManager_Sub1(manager);
  if (b)
    CUserProfileManager_Sub2(manager);
}

int main() {
  CUserProfileManager o;
  o.UseNewProfileValues = 1; o.CameraPCBowYZInverted = 0; o.CameraPCBowYZInvertedNewProfile = 0;
  g_sub1 = g_sub2 = 0;
  CUserProfileManager_SetCameraPCBowYZInverted(&o, 0, 7, 1);
  if (o.CameraPCBowYZInvertedNewProfile != 7) { std::printf("FAIL d4\n"); return 1; }
  if (o.CameraPCBowYZInverted != 7) { std::printf("FAIL 14\n"); return 1; }
  if (g_sub1 != 1) { std::printf("FAIL sub1\n"); return 1; }
  if (g_sub2 != 1) { std::printf("FAIL sub2\n"); return 1; }

  o.UseNewProfileValues = 0; o.CameraPCBowYZInverted = 0; o.CameraPCBowYZInvertedNewProfile = 99;
  g_sub1 = g_sub2 = 0;
  CUserProfileManager_SetCameraPCBowYZInverted(&o, 0, 3, 0);
  if (o.CameraPCBowYZInvertedNewProfile != 99) { std::printf("FAIL d4-skip\n"); return 1; }
  if (o.CameraPCBowYZInverted != 3) { std::printf("FAIL 14-2\n"); return 1; }
  if (g_sub1 != 1) { std::printf("FAIL sub1-2\n"); return 1; }
  if (g_sub2 != 0) { std::printf("FAIL sub2-skip\n"); return 1; }

  std::printf("CUserProfileManager_0040b150_TEST PASS\n");
  return 0;
}