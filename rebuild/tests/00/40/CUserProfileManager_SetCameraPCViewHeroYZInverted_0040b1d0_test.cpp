#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    unsigned char UseNewProfileValues;
    char pad1[0x9];
    unsigned char CameraPCViewHeroYZInverted;
    char pad2[0xbf];
    unsigned char CameraPCViewHeroYZInvertedNewProfile;
};

int g_a = 0;
int g_b = 0;
void __fastcall UPM_helperA(CUserProfileManager*) { g_a++; }
void __fastcall UPM_helperB(CUserProfileManager*) { g_b++; }

void __fastcall CUserProfileManager_SetCameraPCViewHeroYZInverted(CUserProfileManager* manager, void* /*edx*/, char p1, char p2)
{
    if (manager->UseNewProfileValues)
        manager->CameraPCViewHeroYZInvertedNewProfile = (unsigned char)p1;
    manager->CameraPCViewHeroYZInverted = (unsigned char)p1;
    UPM_helperA(manager);
    if (p2)
        UPM_helperB(manager);
}

int main()
{
    CUserProfileManager o;
    o.UseNewProfileValues = 1; o.CameraPCViewHeroYZInverted = 0; o.CameraPCViewHeroYZInvertedNewProfile = 0;
    g_a = 0; g_b = 0;
    CUserProfileManager_SetCameraPCViewHeroYZInverted(&o, 0, (char)7, (char)0);
    if (o.CameraPCViewHeroYZInvertedNewProfile != 7 || o.CameraPCViewHeroYZInverted != 7 || g_a != 1 || g_b != 0) { std::printf("FAIL case1\n"); return 1; }

    o.UseNewProfileValues = 0; o.CameraPCViewHeroYZInverted = 0; o.CameraPCViewHeroYZInvertedNewProfile = 99;
    g_a = 0; g_b = 0;
    CUserProfileManager_SetCameraPCViewHeroYZInverted(&o, 0, (char)5, (char)1);
    if (o.CameraPCViewHeroYZInvertedNewProfile != 99 || o.CameraPCViewHeroYZInverted != 5 || g_a != 1 || g_b != 1) { std::printf("FAIL case2\n"); return 1; }

    std::printf("CUserProfileManager_0040b1d0_TEST PASS\n");
    return 0;
}