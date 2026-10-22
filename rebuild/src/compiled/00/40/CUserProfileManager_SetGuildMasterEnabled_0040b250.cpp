struct CUserProfileManager {
    char pad0[0xc];
    char UseNewProfileValues;       // +0x0c
    char pad1[0x0b];
    char GuildMasterEnabled;         // +0x18
    char pad2[0xbf];
    char GuildMasterEnabledNewProfile;         // +0xd8
};

extern void __fastcall CUserProfileManager_OnResetChanged(CUserProfileManager* manager);
extern void __fastcall CUserProfileManager_OnSecondFlag(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetGuildMasterEnabled(CUserProfileManager* manager, void* edx, char a, char b)
{
    if (manager->UseNewProfileValues)
        manager->GuildMasterEnabledNewProfile = a;
    manager->GuildMasterEnabled = a;
    CUserProfileManager_OnResetChanged(manager);
    if (b)
        CUserProfileManager_OnSecondFlag(manager);
}