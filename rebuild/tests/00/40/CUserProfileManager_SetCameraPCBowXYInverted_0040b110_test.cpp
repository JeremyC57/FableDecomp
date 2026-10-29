// behaviour test for 0x0040B110 CUserProfileManager::SetCameraPCBowXYInverted: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x6];
    bool          CameraPCBowXYInverted;     // +0x13
    unsigned char _pad14[0xBF];
    bool          CameraPCBowXYInvertedNewProfile; // +0xD3
    void SetCameraPCBowXYInverted(bool value, bool save);
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
    m.CameraPCBowXYInverted = false; m.CameraPCBowXYInvertedNewProfile = false;
    m.SetCameraPCBowXYInverted(true, false);
    if (m.CameraPCBowXYInverted != true || m.CameraPCBowXYInvertedNewProfile != false || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetCameraPCBowXYInverted(false, true);
    if (m.CameraPCBowXYInverted != false || m.CameraPCBowXYInvertedNewProfile != false || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xD4) fails |= 4;
    if (fails) { std::printf("0040b110_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040b110_TEST PASS\n");
    return 0;
}
