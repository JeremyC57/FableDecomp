// behaviour test for 0x0040B690 CUserProfileManager::SetResolution: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x3B];
    unsigned long Resolution;                // +0x48
    unsigned char _pad4C[0xB8];
    unsigned long ResolutionNewProfile;      // +0x104
    void SetResolution(unsigned long value, bool save);
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
    m.Resolution = 1; m.ResolutionNewProfile = 1;
    m.SetResolution(5, false);
    if (m.Resolution != 5 || m.ResolutionNewProfile != 1 || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetResolution(7, true);
    if (m.Resolution != 7 || m.ResolutionNewProfile != 7 || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0x108) fails |= 4;
    if (fails) { std::printf("0040b690_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040b690_TEST PASS\n");
    return 0;
}
