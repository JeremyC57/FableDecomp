// Behaviour test for CThingCreatureBase::RemoveFromCreatureInteraction (0x006629B0)
#include <cstdio>
class CThingCreatureBase;
static int g_destroyed = 0;
class CCreatureInteraction { public:
    CThingCreatureBase* m_removed;
    void RemoveParticipant(CThingCreatureBase& creature) { m_removed = &creature; }
};
struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
static void DestroyInteraction(void*) { ++g_destroyed; }
struct CInteractionPointer {
    CCreatureInteraction* m_pObject;
    SCountBlock*          m_pCount;
    void Clear()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
    CCreatureInteraction* operator->() const { return m_pObject; }
};
class CThingCreatureBase { public:
    CInteractionPointer m_creatureInteraction;
    void RemoveFromCreatureInteraction()
    {
        m_creatureInteraction->RemoveParticipant(*this);
        m_creatureInteraction.Clear();
    }
};
int main()
{
    CCreatureInteraction inter; inter.m_removed = 0;
    SCountBlock* blk = new SCountBlock; blk->m_references = 2; blk->m_pfnDestroy = DestroyInteraction; blk->m_pObject = &inter;
    CThingCreatureBase a, b;
    a.m_creatureInteraction.m_pObject = &inter; a.m_creatureInteraction.m_pCount = blk;
    b.m_creatureInteraction.m_pObject = &inter; b.m_creatureInteraction.m_pCount = blk;
    a.RemoveFromCreatureInteraction();
    if (inter.m_removed != &a || blk->m_references != 1 || g_destroyed != 0 || a.m_creatureInteraction.m_pObject != 0 || a.m_creatureInteraction.m_pCount != 0)
    { std::printf("006629b0_TEST FAIL\n"); return 1; }
    b.RemoveFromCreatureInteraction();
    if (inter.m_removed != &b || g_destroyed != 1 || b.m_creatureInteraction.m_pObject != 0 || b.m_creatureInteraction.m_pCount != 0)
    { std::printf("006629b0_TEST FAIL\n"); return 1; }
    std::printf("006629b0_TEST PASS\n");
    return 0;
}