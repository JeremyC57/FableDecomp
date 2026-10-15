#include <cstdio>
// behaviour test for CThingCreatureBase::SetAttackedByCreature (0x006624F0)
struct CBaseObject { int dummy; };
static long g_frame = 0;
static int g_live = 0;
struct CBaseIntelligentPointer {
    CBaseObject* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_live; }
    virtual ~CBaseIntelligentPointer() { --g_live; }
    CBaseObject* GetPItem() { return m_pItem; }
    void SetPItem(CBaseObject* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};
struct CWorld { static long GetWorldFrame() { return g_frame; } };
struct CThingCreatureBase {
    CBaseIntelligentPointer PLastAttackedByCreature;
    unsigned long WFLastAttackedByCreature;
    void SetAttackedByCreature(CThingCreatureBase* attacker);
};
void CThingCreatureBase::SetAttackedByCreature(CThingCreatureBase* attacker)
{
    WFLastAttackedByCreature = CWorld::GetWorldFrame();
    CIntelligentPointer pointer;
    pointer.SetPItem((CBaseObject*)attacker);
    PLastAttackedByCreature.SetPItem(pointer.GetPItem());
}
int main()
{
    CThingCreatureBase c, attacker;
    int before = g_live;
    g_frame = 777;
    c.SetAttackedByCreature(&attacker);
    if (c.WFLastAttackedByCreature != 777 || c.PLastAttackedByCreature.m_pItem != (CBaseObject*)&attacker || g_live != before) { std::printf("006624f0_TEST FAIL\n"); return 1; }
    g_frame = 800;
    c.SetAttackedByCreature(0);
    if (c.WFLastAttackedByCreature != 800 || c.PLastAttackedByCreature.m_pItem != 0) { std::printf("006624f0_TEST FAIL\n"); return 1; }
    std::printf("006624f0_TEST PASS\n");
    return 0;
}
