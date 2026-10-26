#include <cstdio>

class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;
    char pad_d[0x11-0xc-1];
    char CameraFPBowXYInverted;
    char pad_11[0xd1-0x11-1];
    char CameraFPBowXYInvertedNewProfile;
};

static int g_sub1 = 0;
static int g_sub2 = 0;

void __fastcall CUserProfileManager_Sub1(CUserProfileManager* manager) { (void)manager; g_sub1++; }
void __fastcall CUserProfileManager_Sub2(CUserProfileManager* manager) { (void)manager; g_sub2++; }

void __fastcall CUserProfileManager_SetCameraFPBowXYInverted(CUserProfileManager* manager, void* edx, char a, char b)
{
    if (manager->UseNewProfileValues)
        manager->CameraFPBowXYInvertedNewProfile = a;
    manager->CameraFPBowXYInverted = a;
    CUserProfileManager_Sub1(manager);
    if (b)
        CUserProfileManager_Sub2(manager);
}

int main()
{
    CUserProfileManager m;
    // Case 1: flag set, b set
    m.UseNewProfileValues = 1; m.CameraFPBowXYInvertedNewProfile = 0; m.CameraFPBowXYInverted = 0; g_sub1 = 0; g_sub2 = 0;
    CUserProfileManager_SetCameraFPBowXYInverted(&m, 0, (char)7, (char)1);
    if (m.CameraFPBowXYInvertedNewProfile != 7 || m.CameraFPBowXYInverted != 7 || g_sub1 != 1 || g_sub2 != 1) { std::printf("FAIL case1\n"); return 1; }

    // Case 2: flag clear -> no valD1 store; b clear -> no sub2
    m.UseNewProfileValues = 0; m.CameraFPBowXYInvertedNewProfile = 99; m.CameraFPBowXYInverted = 0; g_sub1 = 0; g_sub2 = 0;
    CUserProfileManager_SetCameraFPBowXYInverted(&m, 0, (char)5, (char)0);
    if (m.CameraFPBowXYInvertedNewProfile != 99 || m.CameraFPBowXYInverted != 5 || g_sub1 != 1 || g_sub2 != 0) { std::printf("FAIL case2\n"); return 1; }

    std::printf("CUserProfileManager_0040b090_TEST PASS\n");
    return 0;
}