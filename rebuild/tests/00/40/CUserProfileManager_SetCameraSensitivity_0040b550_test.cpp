// behaviour test for 0x0040B550 CUserProfileManager::SetCameraSensitivity: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x6F];
    float         CameraSensitivity;         // +0x7C
    unsigned char _pad80[0xB0];
    float         CameraSensitivityNewProfile; // +0x130
    void SetCameraSensitivity(float value, bool save);
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
    m.CameraSensitivity = 0.25f; m.CameraSensitivityNewProfile = 0.25f;
    m.SetCameraSensitivity(0.75f, false);
    if (m.CameraSensitivity != 0.75f || m.CameraSensitivityNewProfile != 0.25f || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetCameraSensitivity(1.5f, true);
    if (m.CameraSensitivity != 1.5f || m.CameraSensitivityNewProfile != 1.5f || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0x134) fails |= 4;
    if (fails) { std::printf("0040b550_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040b550_TEST PASS\n");
    return 0;
}
