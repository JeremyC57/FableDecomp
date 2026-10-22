#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char UseNewProfileValues;
    char pad1[0x50-0x0d];
    char resetFlag50;
    char pad2[0x10c-0x51];
    char VerticalSyncEnabledNewProfile;
};

static int g_apply = 0;
static int g_notify = 0;
void __fastcall CUserProfileManager_Apply(CUserProfileManager* manager) { (void)manager; g_apply++; }
void __fastcall CUserProfileManager_Notify(CUserProfileManager* manager) { (void)manager; g_notify++; }

void __fastcall CUserProfileManager_SetVerticalSyncEnabled(CUserProfileManager* manager, void* /*edx*/, char resetting, char notify)
{
    if (manager->UseNewProfileValues)
        manager->VerticalSyncEnabledNewProfile = resetting;
    manager->resetFlag50 = resetting;
    CUserProfileManager_Apply(manager);
    if (notify)
        CUserProfileManager_Notify(manager);
}

int main() {
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;
    o.UseNewProfileValues = 1;
    g_apply = g_notify = 0;
    CUserProfileManager_SetVerticalSyncEnabled(&o, 0, (char)7, (char)1);
    if (o.VerticalSyncEnabledNewProfile != 7 || o.resetFlag50 != 7 || g_apply != 1 || g_notify != 1) { std::printf("FAIL case1\n"); return 1; }

    CUserProfileManager o2;
    for (int i = 0; i < (int)sizeof(o2); ++i) ((char*)&o2)[i] = 0;
    o2.UseNewProfileValues = 0;
    o2.VerticalSyncEnabledNewProfile = 99;
    g_apply = g_notify = 0;
    CUserProfileManager_SetVerticalSyncEnabled(&o2, 0, (char)3, (char)0);
    if (o2.VerticalSyncEnabledNewProfile != 99 || o2.resetFlag50 != 3 || g_apply != 1 || g_notify != 0) { std::printf("FAIL case2\n"); return 1; }

    std::printf("CUserProfileManager_0040b750_TEST PASS\n");
    return 0;
}