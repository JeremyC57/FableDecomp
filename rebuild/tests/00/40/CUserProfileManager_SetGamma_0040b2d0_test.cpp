// behaviour test for 0x0040B2D0 CUserProfileManager::SetGamma: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0xF];
    float         Gamma;                     // +0x1C
    unsigned char _pad20[0xBC];
    float         GammaNewProfile;           // +0xDC
    void SetGamma(float value, bool save);
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
    m.Gamma = 0.25f; m.GammaNewProfile = 0.25f;
    m.SetGamma(0.75f, false);
    if (m.Gamma != 0.75f || m.GammaNewProfile != 0.25f || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetGamma(1.5f, true);
    if (m.Gamma != 1.5f || m.GammaNewProfile != 1.5f || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xE0) fails |= 4;
    if (fails) { std::printf("0040b2d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040b2d0_TEST PASS\n");
    return 0;
}
