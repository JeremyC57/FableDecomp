struct CUserProfileManager {
    char pad0[0xc];
    char UseNewProfileValues;       // +0x0c
    char pad_d[0x12-0x0d];
    char CameraFPBowYZInverted;     // +0x12
    char pad2[0xd2-0x13];
    char CameraFPBowYZInvertedNewProfile;     // +0xd2
};

void __fastcall CUserProfileManager_ResetSub(CUserProfileManager* manager);
void __fastcall CUserProfileManager_ResetSub2(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetCameraFPBowYZInverted(CUserProfileManager* manager, int edx_, char p1, char p2)
{
    if (manager->UseNewProfileValues)
        manager->CameraFPBowYZInvertedNewProfile = p1;
    manager->CameraFPBowYZInverted = p1;
    CUserProfileManager_ResetSub(manager);
    if (p2)
        CUserProfileManager_ResetSub2(manager);
}