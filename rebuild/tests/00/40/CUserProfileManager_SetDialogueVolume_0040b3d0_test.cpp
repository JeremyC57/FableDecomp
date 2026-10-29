// behaviour test for 0x0040B3D0 CUserProfileManager::SetDialogueVolume: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x1F];
    float         DialogueVolume;            // +0x2C
    unsigned char _pad30[0xBC];
    float         DialogueVolumeNewProfile;  // +0xEC
    void SetDialogueVolume(float value, bool save);
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
    m.DialogueVolume = 0.25f; m.DialogueVolumeNewProfile = 0.25f;
    m.SetDialogueVolume(0.75f, false);
    if (m.DialogueVolume != 0.75f || m.DialogueVolumeNewProfile != 0.25f || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetDialogueVolume(1.5f, true);
    if (m.DialogueVolume != 1.5f || m.DialogueVolumeNewProfile != 1.5f || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0xF0) fails |= 4;
    if (fails) { std::printf("0040b3d0_TEST FAIL %d\n", fails); return 1; }
    std::printf("CUserProfileManager_0040b3d0_TEST PASS\n");
    return 0;
}
