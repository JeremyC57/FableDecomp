struct CUserProfileManager {
    char pad0[0xc];
    char UseNewProfileValues;
    char pad1[0x13 - 0xd];
    char CameraPCBowXYInverted;
    char pad2[0xd3 - 0x14];
    char CameraPCBowXYInvertedNewProfile;
};

extern void __fastcall UPM_Update(CUserProfileManager* manager);
extern void __fastcall UPM_Refresh(CUserProfileManager* manager);

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