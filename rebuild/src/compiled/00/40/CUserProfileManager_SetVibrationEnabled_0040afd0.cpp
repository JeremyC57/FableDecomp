struct CUserProfileManager {
    char pad0[0xc];
    char UseNewProfileValues;       // +0xc
    char SubtitlingEnabled;        // +0xd
    char VibrationEnabled;      // +0xe
    char pad2[0xce - 0xf];
    char VibrationEnabledNewProfile;     // +0xce
};

extern void __fastcall CUPM_Helper1(CUserProfileManager* manager);
extern void __fastcall CUPM_Helper2(CUserProfileManager* manager);

void __fastcall CUserProfileManager_SetVibrationEnabled(CUserProfileManager* manager, void* edx, char p1, char p2)
{
    if (manager->UseNewProfileValues)
        manager->VibrationEnabledNewProfile = p1;
    manager->VibrationEnabled = p1;
    CUPM_Helper1(manager);
    if (p2)
        CUPM_Helper2(manager);
}