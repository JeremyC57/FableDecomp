// behaviour test for 0x0040AF90 CUserProfileManager::SetSubtitlingEnabled: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    bool          SubtitlingEnabled;         // +0x0D
    unsigned char _pad0E[0xBF];
    bool          SubtitlingEnabledNewProfile; // +0xCD
    void SetSubtitlingEnabled(bool value, bool save);
    void Save();                                 // 0x0040A740
protected:
    void ApplyValues();                          // 0x00407370
};
static int g_applies = 0, g_saves = 0;
void CUserProfileManager::ApplyValues() { ++g_applies; }
void CUserProfileManager::Save() { ++g_saves; }
int main()
{
    int fails = 0;
    CUserProfileManager m;
    std::memset(&m, 0, sizeof(m));
    m.SubtitlingEnabled = false; m.SubtitlingEnabledNewProfile = false;
    m.SetSubtitlingEnabled(true, false);
    if (m.SubtitlingEnabled != true || m.SubtitlingEnabledNewProfile != false || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetSubtitlingEnabled(false, true);
    if (m.SubtitlingEnabled != false || m.SubtitlingEnabledNewProfile != false || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xCE) fails |= 4;
    if (fails) { std::printf("0040af90_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040af90_TEST PASS\n");
    return 0;
}
