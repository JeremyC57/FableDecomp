#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CMessageEvent;
struct CThing { C3DVector m_pos; const C3DVector& GetPosition_006a4d00() const { return m_pos; } };
static long g_type, g_flags, g_prio; static const C3DVector* g_pos; static CThing *g_t1, *g_t2; static float g_a, g_b; static int g_baseInit;
struct CMessageEventManager { CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2, float a, float b, long priority)
    { g_type = type; g_flags = flags; g_pos = position; g_t1 = thing1; g_t2 = thing2; g_a = a; g_b = b; g_prio = priority; return 0; } };
struct CWorld { unsigned char _pad[0x60]; CMessageEventManager* m_pMessageEventManager; };
struct CBaseIntelligentPointer { virtual void v00() { } CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static CWorld g_world; static CMessageEventManager g_manager;
struct CCreatureActionBase { virtual void v00() { } virtual void Initialise() { ++g_baseInit; } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xa8 - 0x10];
    CWorld* GetPWorld() const { return &g_world; } };
struct CCreatureAction_GiveItemToThing : CCreatureActionBase { CBaseIntelligentPointer PReceivingThing; CBaseIntelligentPointer PItemBeingGiven; virtual void Initialise(); };
void CCreatureAction_GiveItemToThing::Initialise()
{
    CCreatureActionBase::Initialise();
    CThing* pCreatureThing = PCreature.GetPItem();
    CThing* pCreature = PCreature.GetPItem();
    CMessageEventManager* pManager = GetPWorld()->m_pMessageEventManager;
    pManager->AddMessage(0x14, 0x10, &pCreature->GetPosition_006a4d00(), pCreatureThing, PReceivingThing.GetPItem(), 0.0f, 0.0f, 5);
}
int main()
{
    CThing creature, receiver; g_world.m_pMessageEventManager = &g_manager;
    CCreatureAction_GiveItemToThing a; a.PCreature.m_pItem = &creature; a.PReceivingThing.m_pItem = &receiver; a.Initialise();
    if (g_baseInit != 1 || g_type != 0x14 || g_flags != 0x10 || g_pos != &creature.m_pos || g_t1 != &creature || g_t2 != &receiver || g_a != 0.0f || g_b != 0.0f || g_prio != 5
        || (char*)&a.PReceivingThing - (char*)&a != 0xa8) { std::printf("00842f70_TEST FAIL\n"); return 1; }
    std::printf("00842f70_TEST PASS\n");
    return 0;
}