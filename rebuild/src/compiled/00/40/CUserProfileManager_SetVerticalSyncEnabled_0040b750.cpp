class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;
    char pad1[0x50-0x0d];
    char resetFlag50;
    char pad2[0x10c-0x51];
    char VerticalSyncEnabledNewProfile;
};

extern void __fastcall CUserProfileManager_Apply(CUserProfileManager* manager);
extern void __fastcall CUserProfileManager_Notify(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetVerticalSyncEnabled(CUserProfileManager* manager, void* /*edx*/, char resetting, char notify)
{
    if (manager->UseNewProfileValues)
        manager->VerticalSyncEnabledNewProfile = resetting;
    manager->resetFlag50 = resetting;
    CUserProfileManager_Apply(manager);
    if (notify)
        CUserProfileManager_Notify(manager);
}