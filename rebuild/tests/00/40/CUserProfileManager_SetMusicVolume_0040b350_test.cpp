// behaviour test for 0x0040B350 CUserProfileManager::SetMusicVolume: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x1B];
    float         MusicVolume;               // +0x28
    unsigned char _pad2C[0xBC];
    float         MusicVolumeNewProfile;     // +0xE8
    void SetMusicVolume(float value, bool save);
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
    m.MusicVolume = 0.25f; m.MusicVolumeNewProfile = 0.25f;
    m.SetMusicVolume(0.75f, false);
    if (m.MusicVolume != 0.75f || m.MusicVolumeNewProfile != 0.25f || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetMusicVolume(1.5f, true);
    if (m.MusicVolume != 1.5f || m.MusicVolumeNewProfile != 1.5f || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xEC) fails |= 4;
    if (fails) { std::printf("0040b350_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040b350_TEST PASS\n");
    return 0;
}
