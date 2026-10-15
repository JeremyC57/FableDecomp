// Behaviour test for CThingCreatureBase::NewCreatureInteraction (0x00662970)
#include <cstdio>
struct CThingCreatureBase;
struct CCreatureInteraction {
    CThingCreatureBase* m_a; CThingCreatureBase* m_b;
    CCreatureInteraction(int type, CThingCreatureBase& first) : m_a((CThingCreatureBase*)(long)type), m_b(&first) {}
};
static int g_resets = 0;
struct CInteractionPointer {
    CCreatureInteraction* m_pObject;
    void Reset(CCreatureInteraction* object) { delete m_pObject; m_pObject = object; ++g_resets; }
};
struct CThingCreatureBase {
    CInteractionPointer m_creatureInteraction;
    void NewCreatureInteraction(int type)
    {
        m_creatureInteraction.Reset(new CCreatureInteraction(type, *this));
    }
};
int main()
{
    CThingCreatureBase a, b; a.m_creatureInteraction.m_pObject = 0;
    a.NewCreatureInteraction(5);
    CCreatureInteraction* i = a.m_creatureInteraction.m_pObject;
    if (g_resets != 1 || i == 0 || i->m_a != (CThingCreatureBase*)5 || i->m_b != &a) { std::printf("00662970_TEST FAIL\n"); return 1; }
    a.NewCreatureInteraction(0);
    i = a.m_creatureInteraction.m_pObject;
    if (g_resets != 2 || i == 0 || i->m_a != 0 || i->m_b != &a) { std::printf("00662970_TEST FAIL\n"); return 1; }
    delete i;
    std::printf("00662970_TEST PASS\n");
    return 0;
}
