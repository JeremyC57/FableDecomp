#include <cstdio>
// Behaviour model of CAIStateGroup_JackDragon::IsStillRunnable @ 0x0094FDF0.
struct CThing { unsigned char _pad[0x91]; unsigned char m_flags91; bool IsDead() const { return (m_flags91 & 1) != 0; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
static bool g_stopping = false;
struct CAIStateGroupBase { virtual void v00() {} void* PBrain; bool IsStopping() { return g_stopping; } };
struct CAIStateGroup_JackDragon : CAIStateGroupBase { CBaseIntelligentPointer PEnemy;
    bool IsStillRunnable() { if (PEnemy.GetPItem() == 0) return false; if (PEnemy.GetPItem()->IsDead()) return false; return IsStopping() ? false : true; } };
int main()
{
    CThing enemy; enemy.m_flags91 = 0; CAIStateGroup_JackDragon g; g.PEnemy.m_pItem = &enemy;
    bool ok = g.IsStillRunnable() == true;
    g_stopping = true; ok = ok && g.IsStillRunnable() == false;
    g_stopping = false; enemy.m_flags91 = 1; ok = ok && g.IsStillRunnable() == false;
    enemy.m_flags91 = 0; g.PEnemy.m_pItem = 0; ok = ok && g.IsStillRunnable() == false;
    if (!ok) { std::printf("0094fdf0_TEST FAIL\n"); return 1; }
    std::printf("0094fdf0_TEST PASS\n");
    return 0;
}