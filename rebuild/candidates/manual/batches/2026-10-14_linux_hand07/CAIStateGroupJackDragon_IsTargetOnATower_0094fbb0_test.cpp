#include <cstdio>
// Behaviour model of CAIStateGroup_JackDragon::IsTargetOnATower @ 0x0094FBB0.
struct C3DVector { float X, Y, Z; };
struct CThing { C3DVector m_pos; C3DVector* GetPosition() { return &m_pos; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
struct CAIStateGroup_JackDragon { CBaseIntelligentPointer PEnemy, PLandedMarker;
    bool IsTargetOnATower() { CThing* m = PLandedMarker.GetPItem(); CThing* e = PEnemy.GetPItem(); if (e->GetPosition()->Z > m->GetPosition()->Z + 1.5f) return true; return false; } };
int main()
{
    CThing marker, enemy; marker.m_pos.Z = 10.0f; enemy.m_pos.Z = 12.0f;
    CAIStateGroup_JackDragon g; g.PEnemy.m_pItem = &enemy; g.PLandedMarker.m_pItem = &marker;
    bool ok = g.IsTargetOnATower() == true;
    enemy.m_pos.Z = 11.5f; ok = ok && g.IsTargetOnATower() == false;
    enemy.m_pos.Z = 5.0f; ok = ok && g.IsTargetOnATower() == false;
    if (!ok) { std::printf("0094fbb0_TEST FAIL\n"); return 1; }
    std::printf("0094fbb0_TEST PASS\n");
    return 0;
}
