// behaviour test for 0x0040B710 CUserProfileManager::SetScreenAspectRatio: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x67];
    unsigned long ScreenAspectRatio;         // +0x74
    unsigned char _pad78[0xB0];
    unsigned long ScreenAspectRatioNewProfile; // +0x128
    void SetScreenAspectRatio(unsigned long value, bool save);
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
    m.ScreenAspectRatio = 1; m.ScreenAspectRatioNewProfile = 1;
    m.SetScreenAspectRatio(5, false);
    if (m.ScreenAspectRatio != 5 || m.ScreenAspectRatioNewProfile != 1 || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetScreenAspectRatio(7, true);
    if (m.ScreenAspectRatio != 7 || m.ScreenAspectRatioNewProfile != 7 || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0x12C) fails |= 4;
    if (fails) { std::printf("0040b710_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040b710_TEST PASS\n");
    return 0;
}
