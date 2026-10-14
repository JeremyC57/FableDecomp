#include <cstdio>
// Behaviour model of CAIStateGroup_JackOfBladesBase::IsStillRunnable @ 0x00971B00.
struct CThing { unsigned char _pad[0x91]; unsigned char m_flags91; bool IsDead() const { return (m_flags91 & 1) != 0; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() { return m_pItem; } };
static bool g_stopping = false;
struct CAIStateGroupBase { virtual void v00() {} void* PBrain; bool IsStopping() { return g_stopping; } };
struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase { CBaseIntelligentPointer PSwordEmitter, PArenaCentre, PTargetThing;
    bool IsStillRunnable() { if (PTargetThing.GetPItem() != 0) { if (!PTargetThing.GetPItem()->IsDead()) { if (!IsStopping()) { if (PArenaCentre.GetPItem() != 0) { if (!PArenaCentre.GetPItem()->IsDead()) return true; } } } } return false; } };
int main()
{
    CThing target, arena; target.m_flags91 = 0; arena.m_flags91 = 0;
    CAIStateGroup_JackOfBladesBase g; g.PTargetThing.m_pItem = &target; g.PArenaCentre.m_pItem = &arena;
    bool ok = g.IsStillRunnable() == true;
    g_stopping = true; ok = ok && g.IsStillRunnable() == false; g_stopping = false;
    arena.m_flags91 = 1; ok = ok && g.IsStillRunnable() == false; arena.m_flags91 = 0;
    g.PArenaCentre.m_pItem = 0; ok = ok && g.IsStillRunnable() == false; g.PArenaCentre.m_pItem = &arena;
    target.m_flags91 = 1; ok = ok && g.IsStillRunnable() == false; target.m_flags91 = 0;
    g.PTargetThing.m_pItem = 0; ok = ok && g.IsStillRunnable() == false;
    if (!ok) { std::printf("00971b00_TEST FAIL\n"); return 1; }
    std::printf("00971b00_TEST PASS\n");
    return 0;
}
