// behaviour test for 0x0040B0D0 CUserProfileManager::SetCameraFPBowYZInverted: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x5];
    bool          CameraFPBowYZInverted;     // +0x12
    unsigned char _pad13[0xBF];
    bool          CameraFPBowYZInvertedNewProfile; // +0xD2
    void SetCameraFPBowYZInverted(bool value, bool save);
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
    m.CameraFPBowYZInverted = false; m.CameraFPBowYZInvertedNewProfile = false;
    m.SetCameraFPBowYZInverted(true, false);
    if (m.CameraFPBowYZInverted != true || m.CameraFPBowYZInvertedNewProfile != false || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetCameraFPBowYZInverted(false, true);
    if (m.CameraFPBowYZInverted != false || m.CameraFPBowYZInvertedNewProfile != false || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xD3) fails |= 4;
    if (fails) { std::printf("0040b0d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040b0d0_TEST PASS\n");
    return 0;
}
