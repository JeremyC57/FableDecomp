// behaviour test for CThingCreatureBase::AddToCreatureInteraction (0x00662550)
#include <cstdio>
static int g_destroyed = 0;
struct SCountBlock { int m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void Destroy(void*) { ++g_destroyed; }
template <class T> class CCountedPointer { public:
    T* Data; SCountBlock* m_pCount;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data; SCountBlock* count = other.m_pCount;
        if (m_pCount != count) {
            if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); } }
            Data = data; m_pCount = count;
            if (count) ++count->m_references;
        }
        return *this;
    }
    T* operator->() const { return Data; }
};
class CThingCreatureBase;
class CCreatureInteraction { public: CThingCreatureBase* added[4]; int n; void AddCreature(CThingCreatureBase* c) { added[n++] = c; } };
class CThingCreatureBase { public:
    CCountedPointer<CCreatureInteraction> PCreatureInteraction;
    void AddToCreatureInteraction(const CCountedPointer<CCreatureInteraction>& interaction)
    { PCreatureInteraction = interaction; PCreatureInteraction->AddCreature(this); }
};
int main()
{
    CCreatureInteraction oldI; oldI.n = 0; CCreatureInteraction newI; newI.n = 0;
    SCountBlock ob = { 1, Destroy, &oldI }; SCountBlock nb = { 1, Destroy, &newI };
    CThingCreatureBase c; c.PCreatureInteraction.Data = &oldI; c.PCreatureInteraction.m_pCount = &ob;
    CCountedPointer<CCreatureInteraction> p; p.Data = &newI; p.m_pCount = &nb;
    c.AddToCreatureInteraction(p);
    int bad = g_destroyed != 1 || ob.m_references != 0 || nb.m_references != 2 || c.PCreatureInteraction.Data != &newI || newI.n != 1 || newI.added[0] != &c;
    c.AddToCreatureInteraction(p);
    if (nb.m_references != 2 || newI.n != 2) bad = 1;
    if (bad) { std::printf("00662550_TEST FAIL\n"); return 1; }
    std::printf("00662550_TEST PASS\n");
    return 0;
}