#include <cstdio>

struct CUserProfileManager {
    char pad_c[0xc];
    char UseNewProfileValues;
    char SubtitlingEnabled;
    char pad_e[0xcd - 0xe];
    char SubtitlingEnabledNewProfile;
    void ApplyCamera();
    void NotifyReset();
    void SetSubtitlingEnabled(char a1, char a2);
};

static int g_apply = 0;
static int g_notify = 0;

void CUserProfileManager::ApplyCamera() { g_apply++; }
void CUserProfileManager::NotifyReset() { g_notify++; }

void CUserProfileManager::SetSubtitlingEnabled(char a1, char a2)
{
    if (this->UseNewProfileValues)
        this->SubtitlingEnabledNewProfile = a1;
    this->SubtitlingEnabled = a1;
    this->ApplyCamera();
    if (a2)
        this->NotifyReset();
}

int main() {
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); i++) ((char*)&o)[i] = 0;

    o.UseNewProfileValues = 1;
    g_apply = g_notify = 0;
    o.SetSubtitlingEnabled((char)5, (char)1);
    if (o.SubtitlingEnabledNewProfile != 5 || o.SubtitlingEnabled != 5 || g_apply != 1 || g_notify != 1) { std::printf("FAIL case1\n"); return 1; }

    o.UseNewProfileValues = 0;
    o.SubtitlingEnabledNewProfile = 99;
    g_apply = g_notify = 0;
    o.SetSubtitlingEnabled((char)7, (char)0);
    if (o.SubtitlingEnabledNewProfile != 99 || o.SubtitlingEnabled != 7 || g_apply != 1 || g_notify != 0) { std::printf("FAIL case2\n"); return 1; }

    std::printf("CUserProfileManager_0040af90_TEST PASS\n");
    return 0;
}