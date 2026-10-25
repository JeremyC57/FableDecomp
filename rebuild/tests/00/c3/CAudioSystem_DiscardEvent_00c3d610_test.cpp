// Behaviour test for 00c3d610: CAudioSystem::DiscardEvent -- notify, pick the pool, remove from active, recycle or delete
#include <cstdio>
static int g_deleted = 0, g_discards = 0;
struct CPooledNode {
    virtual ~CPooledNode() { ++g_deleted; }
    CPooledNode* m_pPrev; CPooledNode* m_pNext;
    CPooledNode() : m_pPrev(0), m_pNext(0) {}
};
struct CNodePool {
    void* m_pOwner; CPooledNode* m_pActiveHead; CPooledNode* m_pFreeHead; long m_freeCount; long m_activeCount;
    void RemoveActive(CPooledNode* n) { if (m_pActiveHead == n) m_pActiveHead = n->m_pNext; --m_activeCount; }
    void PushFree(CPooledNode* n) { n->m_pNext = m_pFreeHead; m_pFreeHead = n; ++m_freeCount; }
};
struct CAudioEvent : CPooledNode {
    bool m_secondary;
    virtual bool UsesSecondaryPool() { return m_secondary; }
    virtual void OnDiscard() { ++g_discards; }
};
struct CAudioSystem {
    CNodePool m_primaryPool, m_secondaryPool;
    void DiscardEvent(CAudioEvent* e)
    {
        e->OnDiscard();
        CNodePool* pool = e->UsesSecondaryPool() ? &m_secondaryPool : &m_primaryPool;
        pool->RemoveActive(e);
        if (pool->m_freeCount >= 0) pool->PushFree(e); else delete e;
    }
};
int main()
{
    CAudioSystem s = { { 0, 0, 0, 0, 1 }, { 0, 0, 0, -1, 1 } };
    CAudioEvent* a = new CAudioEvent; a->m_secondary = false; s.m_primaryPool.m_pActiveHead = a;
    CAudioEvent* b = new CAudioEvent; b->m_secondary = true; s.m_secondaryPool.m_pActiveHead = b;
    s.DiscardEvent(a); s.DiscardEvent(b);
    bool ok = s.m_primaryPool.m_pFreeHead == a && s.m_primaryPool.m_freeCount == 1 && s.m_primaryPool.m_activeCount == 0 &&
              s.m_secondaryPool.m_activeCount == 0 && g_deleted == 1 && g_discards == 2;
    delete a;
    if (!ok) { std::printf("00c3d610_TEST FAIL\n"); return 1; }
    std::printf("00c3d610_TEST PASS\n");
    return 0;
}
