// behaviour test for 0x0040B610 CUserProfileManager::SetMeshDetail: the live value is always stored, the new-profile copy only
// while UseNewProfileValues; ApplyValues runs every time, Save only when asked (links the row's object)
#include <cstdio>
#include <cstring>
class CUserProfileManager {
public:
    unsigned char _pad00[0x0C];
    bool          UseNewProfileValues;      // +0x0C
    unsigned char _pad0D[0x37];
    float         MeshDetail;                // +0x44
    unsigned char _pad48[0xB8];
    float         MeshDetailNewProfile;      // +0x100
    void SetMeshDetail(float value, bool save);
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
    m.MeshDetail = 0.25f; m.MeshDetailNewProfile = 0.25f;
    m.SetMeshDetail(0.75f, false);
    if (m.MeshDetail != 0.75f || m.MeshDetailNewProfile != 0.25f || g_applies != 1 || g_saves != 0) fails |= 1;
    m.UseNewProfileValues = true;
    m.SetMeshDetail(1.5f, true);
    if (m.MeshDetail != 1.5f || m.MeshDetailNewProfile != 1.5f || g_applies != 2 || g_saves != 1) fails |= 2;
    if (sizeof(m) != 0x104) fails |= 4;
    if (fails) { std::printf("0040b610_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040b610_TEST PASS\n");
    return 0;
}
