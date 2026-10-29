// behaviour test for 0x0040B290 CUserProfileManager::SetHUDTooltipEnabled: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0xC];
    bool          HUDTooltipEnabled;         // +0x19
    unsigned char _pad1A[0xBF];
    bool          HUDTooltipEnabledNewProfile; // +0xD9
    void SetHUDTooltipEnabled(bool value, bool save);
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
    m.HUDTooltipEnabled = false; m.HUDTooltipEnabledNewProfile = false;
    m.SetHUDTooltipEnabled(true, false);
    if (m.HUDTooltipEnabled != true || m.HUDTooltipEnabledNewProfile != false || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetHUDTooltipEnabled(false, true);
    if (m.HUDTooltipEnabled != false || m.HUDTooltipEnabledNewProfile != false || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xDA) fails |= 4;
    if (fails) { std::printf("0040b290_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040b290_TEST PASS\n");
    return 0;
}
