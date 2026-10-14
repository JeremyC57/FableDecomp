#include <cstdio>
static int g_released = 0;
struct CPooledCombatAction {
    virtual ~CPooledCombatAction() { ++g_released; } virtual bool IsSecondaryAction() { return secondary; } virtual void v02() { } virtual void v03() { } virtual void OnHandled() { ++handled; }
    CPooledCombatAction* m_pPrev; CPooledCombatAction* m_pNext; bool secondary; int handled;
    void Link(CPooledCombatAction* prev, CPooledCombatAction* next) { if (prev) prev->m_pNext = this; if (next) next->m_pPrev = this; m_pPrev = prev; m_pNext = next; }
    void Unlink() { if (m_pPrev) m_pPrev->m_pNext = m_pNext; if (m_pNext) m_pNext->m_pPrev = m_pPrev; m_pPrev = 0; m_pNext = 0; }
};
struct CCombatActionPool {
    void* m_pOwner; CPooledCombatAction* m_pActiveHead; CPooledCombatAction* m_pFreeHead; long m_freeCount; long m_activeCount;
    void PushActive(CPooledCombatAction* a) { a->Link(0, m_pActiveHead); m_pActiveHead = a; ++m_activeCount; }
    void PushFree(CPooledCombatAction* a) { a->Link(0, m_pFreeHead); m_pFreeHead = a; ++m_freeCount; }
    void RemoveActive(CPooledCombatAction* a) { if (a == m_pActiveHead) m_pActiveHead = a->m_pNext; a->Unlink(); --m_activeCount; }
};

struct CManager {
    unsigned char _pad[0x1c]; CCombatActionPool m_primaryPool; CCombatActionPool m_secondaryPool;
    void RecycleAction(CPooledCombatAction* action)
    {
        action->OnHandled();
        CCombatActionPool* pool; if (action->IsSecondaryAction()) pool = &m_secondaryPool; else pool = &m_primaryPool;
        pool->RemoveActive(action);
        if (pool->m_freeCount >= 0) pool->PushFree(action); else delete action;
    }
};
int main()
{
    CManager m; CCombatActionPool z = { 0, 0, 0, 0, 0 }; m.m_primaryPool = z; m.m_secondaryPool = z; m.m_secondaryPool.m_freeCount = -1;
    CPooledCombatAction* a = new CPooledCombatAction; a->m_pPrev = a->m_pNext = 0; a->secondary = false; a->handled = 0;
    CPooledCombatAction* b = new CPooledCombatAction; b->m_pPrev = b->m_pNext = 0; b->secondary = true; b->handled = 0;
    m.m_primaryPool.PushActive(a); m.m_secondaryPool.PushActive(b);
    m.RecycleAction(a);
    if (a->handled != 1 || m.m_primaryPool.m_pActiveHead != 0 || m.m_primaryPool.m_pFreeHead != a || m.m_primaryPool.m_freeCount != 1 || m.m_primaryPool.m_activeCount != 0 || g_released != 0) { std::printf("00c3d610_TEST FAIL\n"); return 1; }
    m.RecycleAction(b);
    if (g_released != 1 || m.m_secondaryPool.m_pActiveHead != 0 || m.m_secondaryPool.m_pFreeHead != 0 || m.m_secondaryPool.m_activeCount != 0) { std::printf("00c3d610_TEST FAIL\n"); return 1; }
    delete a;
    std::printf("00c3d610_TEST PASS\n");
    return 0;
}
