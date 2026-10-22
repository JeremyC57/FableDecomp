struct CUserProfileManager {
    char pad0[0xc];
    char UseNewProfileValues;    // +0xc
    char SubtitlingEnabled;      // +0xd
    char VibrationEnabled;    // +0xe
    char pad1[0xce - 0xf];
    char VibrationEnabledNewProfile;   // +0xce
};

char __fastcall IsVibrationEnabled(CUserProfileManager* manager) {
    if (manager->UseNewProfileValues)
        return manager->VibrationEnabledNewProfile;
    return manager->VibrationEnabled;
}