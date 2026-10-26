#include <cstdio>

class CUserProfileManager { public:
    char pad_0[0xc];
    char m_flag;      /* +0xc */
    char pad_d[3];    /* +0xd..0xf */
    char CameraYZInverted;   /* +0x10 */
    char pad_11[0xbf];/* +0x11..0xcf */
    char CameraYZInvertedNewProfile;   /* +0xd0 */
    void Notify();
    void Apply();
    void SetCameraYZInverted(char value, char doApply);
};

static int g_notify = 0;
static int g_apply = 0;

void CUserProfileManager::Notify() { ++g_notify; }
void CUserProfileManager::Apply()  { ++g_apply; }

void CUserProfileManager::SetCameraYZInverted(char value, char doApply)
{
    if (this->m_flag != 0)
        this->CameraYZInvertedNewProfile = value;
    this->CameraYZInverted = value;
    this->Notify();
    if (doApply != 0)
        this->Apply();
}

int main()
{
    CUserProfileManager o;
    for (int i = 0; i < (int)sizeof(o); ++i) ((char*)&o)[i] = 0;

    o.m_flag = 0;
    o.SetCameraYZInverted(5, 0);
    if (o.CameraYZInverted != 5 || o.CameraYZInvertedNewProfile != 0 || g_notify != 1 || g_apply != 0) {
        std::printf("FAIL case1\n"); return 1;
    }

    o.m_flag = 1;
    o.SetCameraYZInverted(7, 1);
    if (o.CameraYZInverted != 7 || o.CameraYZInvertedNewProfile != 7 || g_notify != 2 || g_apply != 1) {
        std::printf("FAIL case2\n"); return 1;
    }

    std::printf("CUserProfileManager_0040b050_TEST PASS\n");
    return 0;
}