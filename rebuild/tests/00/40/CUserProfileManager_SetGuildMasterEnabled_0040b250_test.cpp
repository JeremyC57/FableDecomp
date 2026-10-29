// behaviour test for 0x0040B250 CUserProfileManager::SetGuildMasterEnabled: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0xB];
    bool          GuildMasterEnabled;        // +0x18
    unsigned char _pad19[0xBF];
    bool          GuildMasterEnabledNewProfile; // +0xD8
    void SetGuildMasterEnabled(bool value, bool save);
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
    m.GuildMasterEnabled = false; m.GuildMasterEnabledNewProfile = false;
    m.SetGuildMasterEnabled(true, false);
    if (m.GuildMasterEnabled != true || m.GuildMasterEnabledNewProfile != false || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetGuildMasterEnabled(false, true);
    if (m.GuildMasterEnabled != false || m.GuildMasterEnabledNewProfile != false || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xD9) fails |= 4;
    if (fails) { std::printf("0040b250_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040b250_TEST PASS\n");
    return 0;
}
