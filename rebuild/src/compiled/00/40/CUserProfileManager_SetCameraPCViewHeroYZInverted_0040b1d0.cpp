class CUserProfileManager { public:
    char pad0[0xc];
    unsigned char UseNewProfileValues;   // +0xc
    char pad1[0x9];
    unsigned char CameraPCViewHeroYZInverted;  // +0x16
    char pad2[0xbf];
    unsigned char CameraPCViewHeroYZInvertedNewProfile;  // +0xd6
};

extern void __fastcall UPM_helperA(CUserProfileManager* manager);
extern void __fastcall UPM_helperB(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetCameraPCViewHeroYZInverted(CUserProfileManager* manager, void* /*edx*/, char p1, char p2)
{
    if (manager->UseNewProfileValues)
        manager->CameraPCViewHeroYZInvertedNewProfile = (unsigned char)p1;
    manager->CameraPCViewHeroYZInverted = (unsigned char)p1;
    UPM_helperA(manager);
    if (p2)
        UPM_helperB(manager);
}