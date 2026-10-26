#include <cstdio>

class CUserProfileManager { public:
    char pad0[0x0c];
    char UseNewProfileValues;
    char pad1[0x4c - 0x0d];
    int RefreshRate;
    char pad2[0x108 - 0x50];
    int RefreshRateNewProfile;
};

static int g_apply = 0;
static int g_notify = 0;

void __fastcall CUserProfileManager_Apply(CUserProfileManager* manager) { (void)manager; g_apply++; }
void __fastcall CUserProfileManager_Notify(CUserProfileManager* manager) { (void)manager; g_notify++; }

void __fastcall CUserProfileManager_SetRefreshRate(CUserProfileManager* manager, void* edx, int value, char doNotify)
{
    if (manager->UseNewProfileValues)
        manager->RefreshRateNewProfile = value;
    manager->RefreshRate = value;
    CUserProfileManager_Apply(manager);
    if (doNotify)
        CUserProfileManager_Notify(manager);
}

int main()
{
    CUserProfileManager o;
    // case: flag set, notify set
    o.UseNewProfileValues = 1; o.RefreshRate = 0; o.RefreshRateNewProfile = 0; g_apply = 0; g_notify = 0;
    CUserProfileManager_SetRefreshRate(&o, 0, 7, 1);
    if (o.RefreshRateNewProfile != 7 || o.RefreshRate != 7 || g_apply != 1 || g_notify != 1) { std::printf("FAIL a\n"); return 1; }

    // case: flag clear, notify clear
    o.UseNewProfileValues = 0; o.RefreshRate = 0; o.RefreshRateNewProfile = 99; g_apply = 0; g_notify = 0;
    CUserProfileManager_SetRefreshRate(&o, 0, 5, 0);
    if (o.RefreshRateNewProfile != 99 || o.RefreshRate != 5 || g_apply != 1 || g_notify != 0) { std::printf("FAIL b\n"); return 1; }

    std::printf("CUserProfileManager_0040b6d0_TEST PASS\n");
    return 0;
}