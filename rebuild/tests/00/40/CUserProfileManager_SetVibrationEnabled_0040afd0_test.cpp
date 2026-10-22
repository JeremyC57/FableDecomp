#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char UseNewProfileValues;       // +0xc
    char SubtitlingEnabled;        // +0xd
    char VibrationEnabled;      // +0xe
    char pad2[0xce - 0xf];
    char VibrationEnabledNewProfile;     // +0xce
};

static int g_h1 = 0;
static int g_h2 = 0;

void __fastcall CUPM_Helper1(CUserProfileManager* manager) { (void)manager; g_h1++; }
void __fastcall CUPM_Helper2(CUserProfileManager* manager) { (void)manager; g_h2++; }

void __fastcall CUserProfileManager_SetVibrationEnabled(CUserProfileManager* manager, void* edx, char p1, char p2)
{
    (void)edx;
    if (manager->UseNewProfileValues)
        manager->VibrationEnabledNewProfile = p1;
    manager->VibrationEnabled = p1;
    CUPM_Helper1(manager);
    if (p2)
        CUPM_Helper2(manager);
}

int main()
{
    CUserProfileManager o;
    // case: flag_c set, p1=1, p2=1
    o.UseNewProfileValues = 1; o.VibrationEnabled = 0; o.VibrationEnabledNewProfile = 0; g_h1 = g_h2 = 0;
    CUserProfileManager_SetVibrationEnabled(&o, 0, 1, 1);
    if (o.VibrationEnabledNewProfile != 1 || o.VibrationEnabled != 1 || g_h1 != 1 || g_h2 != 1) {
        std::printf("FAIL case1\n"); return 1;
    }
    // case: flag_c clear, p1=7, p2=0 -> field_ce untouched, no helper2
    o.UseNewProfileValues = 0; o.VibrationEnabled = 0; o.VibrationEnabledNewProfile = 55; g_h1 = g_h2 = 0;
    CUserProfileManager_SetVibrationEnabled(&o, 0, 7, 0);
    if (o.VibrationEnabledNewProfile != 55 || o.VibrationEnabled != 7 || g_h1 != 1 || g_h2 != 0) {
        std::printf("FAIL case2\n"); return 1;
    }
    std::printf("CUserProfileManager_0040afd0_TEST PASS\n");
    return 0;
}