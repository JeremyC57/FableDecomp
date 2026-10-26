#include <cstdio>

class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;       // +0x0c
    char pad_d[0x12-0x0d];
    char CameraFPBowYZInverted;     // +0x12
    char pad2[0xd2-0x13];
    char CameraFPBowYZInvertedNewProfile;     // +0xd2
};

static int g_sub1 = 0;
static int g_sub2 = 0;

void __fastcall CUserProfileManager_ResetSub(CUserProfileManager* manager) { g_sub1++; }
void __fastcall CUserProfileManager_ResetSub2(CUserProfileManager* manager) { g_sub2++; }

void __fastcall CUserProfileManager_SetCameraFPBowYZInverted(CUserProfileManager* manager, int edx_, char p1, char p2)
{
    if (manager->UseNewProfileValues)
        manager->CameraFPBowYZInvertedNewProfile = p1;
    manager->CameraFPBowYZInverted = p1;
    CUserProfileManager_ResetSub(manager);
    if (p2)
        CUserProfileManager_ResetSub2(manager);
}

int main() {
    CUserProfileManager o;
    // case A: flag_c set, p2 set
    o.UseNewProfileValues = 1; o.CameraFPBowYZInverted = 0; o.CameraFPBowYZInvertedNewProfile = 0;
    g_sub1 = g_sub2 = 0;
    CUserProfileManager_SetCameraFPBowYZInverted(&o, 0, (char)7, (char)1);
    if (o.CameraFPBowYZInvertedNewProfile != 7 || o.CameraFPBowYZInverted != 7 || g_sub1 != 1 || g_sub2 != 1) {
        std::printf("FAIL A\n"); return 1;
    }
    // case B: flag_c clear, p2 clear
    o.UseNewProfileValues = 0; o.CameraFPBowYZInverted = 0; o.CameraFPBowYZInvertedNewProfile = 55;
    g_sub1 = g_sub2 = 0;
    CUserProfileManager_SetCameraFPBowYZInverted(&o, 0, (char)9, (char)0);
    if (o.CameraFPBowYZInvertedNewProfile != 55 || o.CameraFPBowYZInverted != 9 || g_sub1 != 1 || g_sub2 != 0) {
        std::printf("FAIL B\n"); return 1;
    }
    std::printf("CUserProfileManager_0040b0d0_TEST PASS\n");
    return 0;
}