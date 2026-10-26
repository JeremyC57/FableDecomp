#include <cstdio>

class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;
    char pad1[0x13 - 0xd];
    char CameraPCBowXYInverted;
    char pad2[0xd3 - 0x14];
    char CameraPCBowXYInvertedNewProfile;
};

static int g_update = 0;
static int g_refresh = 0;

void __fastcall UPM_Update(CUserProfileManager* manager) { (void)manager; g_update++; }
void __fastcall UPM_Refresh(CUserProfileManager* manager) { (void)manager; g_refresh++; }

void __fastcall CUserProfileManager_SetCameraPCBowXYInverted(CUserProfileManager* manager, int edx_pad, char p1, char p2)
{
    (void)edx_pad;
    if (manager->UseNewProfileValues)
        manager->CameraPCBowXYInvertedNewProfile = p1;
    manager->CameraPCBowXYInverted = p1;
    UPM_Update(manager);
    if (p2)
        UPM_Refresh(manager);
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); i++) ((char*)&o)[i] = 0;

    o.UseNewProfileValues = 1;
    CUserProfileManager_SetCameraPCBowXYInverted(&o, 0, 7, 1);
    if (o.CameraPCBowXYInvertedNewProfile != 7) { std::printf("FAIL bd3\n"); return 1; }
    if (o.CameraPCBowXYInverted != 7) { std::printf("FAIL b13\n"); return 1; }
    if (g_update != 1) { std::printf("FAIL update\n"); return 1; }
    if (g_refresh != 1) { std::printf("FAIL refresh\n"); return 1; }

    CUserProfileManager o2;
    for (int i = 0; i < (int)sizeof(o2); i++) ((char*)&o2)[i] = 0;
    o2.CameraPCBowXYInvertedNewProfile = 99;
    CUserProfileManager_SetCameraPCBowXYInverted(&o2, 0, 3, 0);
    if (o2.CameraPCBowXYInvertedNewProfile != 99) { std::printf("FAIL bd3 unchanged\n"); return 1; }
    if (o2.CameraPCBowXYInverted != 3) { std::printf("FAIL b13_2\n"); return 1; }
    if (g_update != 2) { std::printf("FAIL update2\n"); return 1; }
    if (g_refresh != 1) { std::printf("FAIL refresh2\n"); return 1; }

    std::printf("CUserProfileManager_0040b110_TEST PASS\n");
    return 0;
}