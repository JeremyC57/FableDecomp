#include <stdio.h>

class CUserProfileManager { public:
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

int main() {
    CUserProfileManager obj;
    obj.UseNewProfileValues = 1;
    obj.VibrationEnabled = 0x22;
    obj.VibrationEnabledNewProfile = 0x55;
    char r1 = IsVibrationEnabled(&obj);
    if (r1 != 0x55) { printf("FAIL case1 got %d\n", (int)r1); return 1; }

    obj.UseNewProfileValues = 0;
    obj.VibrationEnabled = 0x33;
    obj.VibrationEnabledNewProfile = 0x77;
    char r2 = IsVibrationEnabled(&obj);
    if (r2 != 0x33) { printf("FAIL case2 got %d\n", (int)r2); return 1; }

    printf("OK_0x00548570\n");
    return 0;
}