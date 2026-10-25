// Behaviour test for 00c3ca00: the libpk two-chain node pool (Link / Unlink / GetFree / PushActive / PushFree / PopFree)
#include <cstdio>
struct CPooledNode {
    virtual ~CPooledNode() {}
    CPooledNode* m_pPrev; CPooledNode* m_pNext;
    CPooledNode() : m_pPrev(0), m_pNext(0) {}
    void Link(CPooledNode* prev, CPooledNode* next) { if (prev) prev->m_pNext = this; if (next) next->m_pPrev = this; m_pPrev = prev; m_pNext = next; }
    void Unlink() { if (m_pPrev) m_pPrev->m_pNext = m_pNext; if (m_pNext) m_pNext->m_pPrev = m_pPrev; m_pPrev = 0; m_pNext = 0; }
};
struct CNodePool {
    void* m_pOwner; CPooledNode* m_pActiveHead; CPooledNode* m_pFreeHead; long m_freeCount; long m_activeCount;
    CPooledNode* GetFree() { return m_pFreeHead; }
    void PushActive(CPooledNode* n) { n->Link(0, m_pActiveHead); m_pActiveHead = n; ++m_activeCount; }
    void PushFree(CPooledNode* n) { n->Link(0, m_pFreeHead); m_pFreeHead = n; ++m_freeCount; }
    void PopFree() { CPooledNode* n = m_pFreeHead; m_pFreeHead = n->m_pNext; n->Unlink(); --m_freeCount; }
};
int main()
{
    CNodePool p = { 0, 0, 0, 0, 0 }; CPooledNode a, b, c;
    p.PushFree(&a); p.PushFree(&b);                       // free: b, a
    bool ok = p.GetFree() == &b && p.m_freeCount == 2 && b.m_pNext == &a && a.m_pPrev == &b;
    CPooledNode* n = p.GetFree(); p.PopFree(); p.PushActive(n);
    ok = ok && p.GetFree() == &a && a.m_pPrev == 0 && p.m_freeCount == 1 && p.m_pActiveHead == &b && p.m_activeCount == 1;
    p.PushActive(&c);                                     // active: c, b
    ok = ok && c.m_pNext == &b && b.m_pPrev == &c && p.m_activeCount == 2;
    b.Unlink(); ok = ok && c.m_pNext == 0 && b.m_pPrev == 0 && b.m_pNext == 0;
    if (!ok) { std::printf("00c3ca00_TEST FAIL\n"); return 1; }
    std::printf("00c3ca00_TEST PASS\n");
    return 0;
}
