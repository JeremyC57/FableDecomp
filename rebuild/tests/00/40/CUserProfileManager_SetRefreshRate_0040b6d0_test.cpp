// behaviour test for 0x0040B6D0 CUserProfileManager::SetRefreshRate: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x3F];
    unsigned long RefreshRate;               // +0x4C
    unsigned char _pad50[0xB8];
    unsigned long RefreshRateNewProfile;     // +0x108
    void SetRefreshRate(unsigned long value, bool save);
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
    m.RefreshRate = 1; m.RefreshRateNewProfile = 1;
    m.SetRefreshRate(5, false);
    if (m.RefreshRate != 5 || m.RefreshRateNewProfile != 1 || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetRefreshRate(7, true);
    if (m.RefreshRate != 7 || m.RefreshRateNewProfile != 7 || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0x10C) fails |= 4;
    if (fails) { std::printf("0040b6d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040b6d0_TEST PASS\n");
    return 0;
}
