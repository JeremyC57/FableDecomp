#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
static int g_kills; static bool g_inRange; static const C3DVector* g_rangePos; static float g_range; static bool g_silent;
struct CThing { unsigned char _pad[0x91]; unsigned char m_flags91; C3DVector m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
    bool IsInRange(const C3DVector& position, float range) { g_rangePos = &position; g_range = range; return g_inRange; }
    void Kill(bool silent) { ++g_kills; g_silent = silent; } };
struct CBaseIntelligentPointer { virtual void v00() { } CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; } return false; }
struct CCreatureAction_KillCreature { virtual void v00() { } virtual void PerformAction(); unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xa8 - 0x10]; CBaseIntelligentPointer PTargetCreature; };
void CCreatureAction_KillCreature::PerformAction()
{
    if (IS_THING_ALIVE(PTargetCreature)) { CThing* pTarget = PTargetCreature.GetPItem(); CThing* pCreature = PCreature.GetPItem(); if (pTarget->IsInRange(pCreature->GetPosition(), 2.0f)) pTarget->Kill(true); }
}
int main()
{
    CThing creature, target; std::memset(&creature, 0, sizeof(creature)); std::memset(&target, 0, sizeof(target));
    CCreatureAction_KillCreature a; a.PCreature.m_pItem = &creature; a.PTargetCreature.m_pItem = 0;
    a.PerformAction(); if (g_kills != 0) { std::printf("008445a0_TEST FAIL\n"); return 1; }
    a.PTargetCreature.m_pItem = &target; target.m_flags91 = 1; a.PerformAction(); if (g_kills != 0) { std::printf("008445a0_TEST FAIL\n"); return 1; }
    target.m_flags91 = 0; g_inRange = false; a.PerformAction(); if (g_kills != 0 || g_rangePos != &creature.m_pos || g_range != 2.0f) { std::printf("008445a0_TEST FAIL\n"); return 1; }
    g_inRange = true; a.PerformAction(); if (g_kills != 1 || !g_silent || (char*)&a.PTargetCreature - (char*)&a != 0xa8) { std::printf("008445a0_TEST FAIL\n"); return 1; }
    std::printf("008445a0_TEST PASS\n");
    return 0;
}
