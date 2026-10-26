#include <cstdio>

class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;       // +0x0c
    char pad1[0x0b];
    char GuildMasterEnabled;         // +0x18
    char pad2[0xbf];
    char GuildMasterEnabledNewProfile;         // +0xd8
};

int g_reset = 0;
int g_second = 0;

void __fastcall CUserProfileManager_OnResetChanged(CUserProfileManager* manager) { (void)manager; g_reset++; }
void __fastcall CUserProfileManager_OnSecondFlag(CUserProfileManager* manager) { (void)manager; g_second++; }

void __fastcall CUserProfileManager_SetGuildMasterEnabled(CUserProfileManager* manager, void* edx, char a, char b)
{
    (void)edx;
    if (manager->UseNewProfileValues)
        manager->GuildMasterEnabledNewProfile = a;
    manager->GuildMasterEnabled = a;
    CUserProfileManager_OnResetChanged(manager);
    if (b)
        CUserProfileManager_OnSecondFlag(manager);
}

int main()
{
    CUserProfileManager m;
    m.UseNewProfileValues = 1; m.GuildMasterEnabled = 0; m.GuildMasterEnabledNewProfile = 0;
    CUserProfileManager_SetGuildMasterEnabled(&m, 0, (char)5, (char)0);
    if (m.GuildMasterEnabledNewProfile != 5 || m.GuildMasterEnabled != 5 || g_reset != 1 || g_second != 0) { std::printf("FAIL 1\n"); return 1; }

    m.UseNewProfileValues = 0; m.GuildMasterEnabled = 0; m.GuildMasterEnabledNewProfile = 9;
    CUserProfileManager_SetGuildMasterEnabled(&m, 0, (char)7, (char)1);
    if (m.GuildMasterEnabledNewProfile != 9 || m.GuildMasterEnabled != 7 || g_reset != 2 || g_second != 1) { std::printf("FAIL 2\n"); return 1; }

    std::printf("CUserProfileManager_0040b250_TEST PASS\n");
    return 0;
}