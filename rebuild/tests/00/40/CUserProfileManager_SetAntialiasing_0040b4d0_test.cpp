// behaviour test for 0x0040B4D0 CUserProfileManager::SetAntialiasing: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x2B];
    unsigned long Antialiasing;              // +0x38
    unsigned char _pad3C[0xB8];
    unsigned long AntialiasingNewProfile;    // +0xF4
    void SetAntialiasing(unsigned long value, bool save);
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
    m.Antialiasing = 1; m.AntialiasingNewProfile = 1;
    m.SetAntialiasing(5, false);
    if (m.Antialiasing != 5 || m.AntialiasingNewProfile != 1 || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetAntialiasing(7, true);
    if (m.Antialiasing != 7 || m.AntialiasingNewProfile != 7 || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xF8) fails |= 4;
    if (fails) { std::printf("0040b4d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040b4d0_TEST PASS\n");
    return 0;
}
