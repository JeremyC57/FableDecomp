#include <cstdio>
class CCharString { public: const char* m_pData; };
static CCharString g_RightHandAttachmentName = { "right" };
static CCharString g_LeftHandAttachmentName = { "left" };
class CTCFireballSpell { public:
    long WeaponState; bool LeftHandAnim; CCharString FireballAttachmentName;
    void SetWeaponState(long state)
    {
        WeaponState = state;
        switch (state) {
        case 0: FireballAttachmentName = LeftHandAnim ? g_LeftHandAttachmentName : g_RightHandAttachmentName; break;
        case 1: FireballAttachmentName = g_LeftHandAttachmentName; break;
        case 2: FireballAttachmentName = g_RightHandAttachmentName; break;
        }
    }
};
int main()
{
    CTCFireballSpell s; s.LeftHandAnim = true; s.FireballAttachmentName.m_pData = 0;
    s.SetWeaponState(0); bool a = s.FireballAttachmentName.m_pData == g_LeftHandAttachmentName.m_pData;
    s.LeftHandAnim = false; s.SetWeaponState(0); bool b = s.FireballAttachmentName.m_pData == g_RightHandAttachmentName.m_pData;
    s.SetWeaponState(1); bool c = s.FireballAttachmentName.m_pData == g_LeftHandAttachmentName.m_pData;
    s.SetWeaponState(2); bool d = s.FireballAttachmentName.m_pData == g_RightHandAttachmentName.m_pData;
    s.FireballAttachmentName.m_pData = 0; s.SetWeaponState(3); bool e = s.FireballAttachmentName.m_pData == 0 && s.WeaponState == 3;
    if (!a || !b || !c || !d || !e) { std::printf("007c8660_TEST FAIL\n"); return 1; }
    std::printf("007c8660_TEST PASS\n");
    return 0;
}