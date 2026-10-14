#include <cstdio>
struct CPooledCombatAction {
    virtual void Release(long) { } virtual bool IsSecondaryAction() { return secondary; } virtual void v02() { } virtual void v03() { } virtual void OnHandled() { ++handled; }
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

int main()
{
    CCombatActionPool pool = { 0, 0, 0, 0, 0 }; CPooledCombatAction a, b; a.m_pPrev = a.m_pNext = b.m_pPrev = b.m_pNext = 0;
    pool.PushActive(&a); pool.PushActive(&b);
    if (pool.m_pActiveHead != &b || b.m_pNext != &a || a.m_pPrev != &b || pool.m_activeCount != 2 || pool.m_freeCount != 0) { std::printf("00c3c9b0_TEST FAIL\n"); return 1; }
    std::printf("00c3c9b0_TEST PASS\n");
    return 0;
}