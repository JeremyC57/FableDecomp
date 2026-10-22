struct CUserProfileManager {
    char pad[0x134];
    int mField;
};

void __fastcall CUserProfileManager_OnPreDeviceReset(CUserProfileManager* manager)
{
    manager->mField = 0;
}