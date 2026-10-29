#include <cstdio>
// behaviour test for CThingCreatureBase::UpdateLastAttackingCreature (0x006624A0)
class CBaseObject { public: int dummy; };
static long g_frame = 0;
long g_GameFramesPerSecond = 30;
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() { return m_pItem; }
};
class CWorld { public:
    static long __fastcall GetFrame() { return g_frame; }
    unsigned long GetFramesSince(unsigned long frame) { return GetFrame() - frame; }
};
static CWorld g_world;
class CThingCreatureBase { public:
    CBaseIntelligentPointer PLastAttackedByCreature;
    unsigned long WFLastAttackedByCreature;
    int clears;
    CWorld* GetWorld() { return &g_world; }
    void ClearLastAttackedByCreature() { ++clears; PLastAttackedByCreature.m_pItem = 0; }
    void UpdateLastAttackingCreature();
};
void CThingCreatureBase::UpdateLastAttackingCreature()
{
    if (PLastAttackedByCreature.GetPItem()) {
        CWorld* world = GetWorld();
        if (world->GetFramesSince(WFLastAttackedByCreature) > (unsigned long)(g_GameFramesPerSecond * 300))
            ClearLastAttackedByCreature();
    }
}
int main()
{
    CBaseObject attacker;
    CThingCreatureBase c; c.clears = 0;
    c.PLastAttackedByCreature.m_pItem = 0; c.WFLastAttackedByCreature = 0; g_frame = 100000;
    c.UpdateLastAttackingCreature();
    if (c.clears != 0) { std::printf("006624a0_TEST FAIL\n"); return 1; }
    c.PLastAttackedByCreature.m_pItem = &attacker; c.WFLastAttackedByCreature = 1000; g_frame = 1000 + 9000;
    c.UpdateLastAttackingCreature();
    if (c.clears != 0 || c.PLastAttackedByCreature.m_pItem != &attacker) { std::printf("006624a0_TEST FAIL\n"); return 1; }
    g_frame = 1000 + 9001;
    c.UpdateLastAttackingCreature();
    if (c.clears != 1 || c.PLastAttackedByCreature.m_pItem != 0) { std::printf("006624a0_TEST FAIL\n"); return 1; }
    std::printf("006624a0_TEST PASS\n");
    return 0;
}