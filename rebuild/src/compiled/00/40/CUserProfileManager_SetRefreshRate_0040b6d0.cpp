class CUserProfileManager { public:
    char pad0[0x0c];
    char UseNewProfileValues;
    char pad1[0x4c - 0x0d];
    int RefreshRate;
    char pad2[0x108 - 0x50];
    int RefreshRateNewProfile;
};

extern void __fastcall CUserProfileManager_Apply(CUserProfileManager* manager);
extern void __fastcall CUserProfileManager_Notify(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetRefreshRate(CUserProfileManager* manager, void* edx, int value, char doNotify)
{
    if (manager->UseNewProfileValues)
        manager->RefreshRateNewProfile = value;
    manager->RefreshRate = value;
    CUserProfileManager_Apply(manager);
    if (doNotify)
        CUserProfileManager_Notify(manager);
}