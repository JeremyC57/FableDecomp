#include <cstdio>
// Behaviour model of CAIStateGroup_JackDragon::Cleanup @ 0x009500A0.
static int g_killed = 0; static bool g_instant = false;
class CThing { public: unsigned char _pad[0x91]; unsigned char m_flags91; bool IsDead() const { return (m_flags91 & 1) != 0; } void Kill(bool instant) { g_killed++; g_instant = instant; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
struct CAIStateGroup_JackDragon { CBaseIntelligentPointer PInvisibleBodgeCreature;
    void Cleanup() { if (PInvisibleBodgeCreature.GetPItem() != 0) { if (!PInvisibleBodgeCreature.GetPItem()->IsDead()) PInvisibleBodgeCreature.GetPItem()->Kill(true); } } };
int main()
{
    CThing c; c.m_flags91 = 0; CAIStateGroup_JackDragon g; g.PInvisibleBodgeCreature.m_pItem = &c;
    g.Cleanup(); bool ok = g_killed == 1 && g_instant == true;
    c.m_flags91 = 1; g.Cleanup(); ok = ok && g_killed == 1;
    g.PInvisibleBodgeCreature.m_pItem = 0; g.Cleanup(); ok = ok && g_killed == 1;
    if (!ok) { std::printf("009500a0_TEST FAIL\n"); return 1; }
    std::printf("009500a0_TEST PASS\n");
    return 0;
}