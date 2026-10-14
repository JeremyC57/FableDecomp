#include <cstdio>
struct C3DVector { float X, Y, Z; };
static C3DVector* g_points; static int g_baseInit;
struct CTCPhysicsComponent { virtual void v00() { } virtual void GetSwoopPoints(C3DVector* points) { g_points = points; points[2].Z = 9.0f; } };
struct CThingCreatureBase_Retail { unsigned char _pad[0x60]; CTCPhysicsComponent* m_pPhysics; };
struct CBaseIntelligentPointer { virtual void v00() { } CThingCreatureBase_Retail* m_pItem; CThingCreatureBase_Retail* GetPItem() const { return m_pItem; } };
struct CCombatActionBase { virtual void v00() { } virtual void Initialise() { ++g_baseInit; } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0x114 - 0x10]; };
struct CCreatureAction_DragonSwoop : CCombatActionBase { C3DVector SwoopPoints[3]; virtual void Initialise(); };
void CCreatureAction_DragonSwoop::Initialise() { CCombatActionBase::Initialise(); CThingCreatureBase_Retail* pCreature = PCreature.GetPItem(); pCreature->m_pPhysics->GetSwoopPoints(SwoopPoints); }
int main()
{
    CTCPhysicsComponent phys; CThingCreatureBase_Retail creature; creature.m_pPhysics = &phys;
    CCreatureAction_DragonSwoop a; a.PCreature.m_pItem = &creature; a.SwoopPoints[2].Z = 0.0f; a.Initialise();
    if (g_baseInit != 1 || g_points != a.SwoopPoints || a.SwoopPoints[2].Z != 9.0f || (char*)a.SwoopPoints - (char*)&a != 0x114) { std::printf("008f2f00_TEST FAIL\n"); return 1; }
    std::printf("008f2f00_TEST PASS\n");
    return 0;
}