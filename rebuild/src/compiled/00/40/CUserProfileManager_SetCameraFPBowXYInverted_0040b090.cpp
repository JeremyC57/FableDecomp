class CUserProfileManager { public:
    char pad0[0xc];
    char UseNewProfileValues;
    char pad_d[0x11-0xc-1];
    char CameraFPBowXYInverted;
    char pad_11[0xd1-0x11-1];
    char CameraFPBowXYInvertedNewProfile;
};

extern void __fastcall CUserProfileManager_Sub1(CUserProfileManager* manager);
extern void __fastcall CUserProfileManager_Sub2(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetCameraFPBowXYInverted(CUserProfileManager* manager, void* edx, char a, char b)
{
    if (manager->UseNewProfileValues)
        manager->CameraFPBowXYInvertedNewProfile = a;
    manager->CameraFPBowXYInverted = a;
    CUserProfileManager_Sub1(manager);
    if (b)
        CUserProfileManager_Sub2(manager);
}