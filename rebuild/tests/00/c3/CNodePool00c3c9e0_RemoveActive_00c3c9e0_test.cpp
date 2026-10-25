// Behaviour test for 00c3c9e0: CNodePool::RemoveActive -- advances the active head past the node when it is the head, unlinks it and decrements the active count
#include <cstdio>

static int g_deleted = 0, g_reset = 0;

struct CPooledNode {
    CPooledNode* m_pPrev;
    CPooledNode* m_pNext;
    long m_handle;
    int m_released;
    CPooledNode() : m_pPrev(0), m_pNext(0), m_handle(0), m_released(0) { }
    virtual ~CPooledNode() { ++g_deleted; }
    virtual void Reset() { ++g_reset; }
    void ReleaseResource() { ++m_released; }
    void Link(CPooledNode* prev, CPooledNode* next)
    {
        if (prev) prev->m_pNext = this;
        if (next) next->m_pPrev = this;
        m_pPrev = prev;
        m_pNext = next;
    }
    void Unlink()
    {
        if (m_pPrev) m_pPrev->m_pNext = m_pNext;
        if (m_pNext) m_pNext->m_pPrev = m_pPrev;
        m_pPrev = 0;
        m_pNext = 0;
    }
};

struct CNodePool {
    void* m_pOwner;
    CPooledNode* m_pActiveHead;
    CPooledNode* m_pFreeHead;
    long m_freeCount;
    long m_activeCount;
    void PushActive(CPooledNode* node) { node->Link(0, m_pActiveHead); m_pActiveHead = node; ++m_activeCount; }
    void PushFree(CPooledNode* node) { node->Link(0, m_pFreeHead); m_pFreeHead = node; ++m_freeCount; }
    void RemoveActive(CPooledNode* node)
    {
        if (node == m_pActiveHead)
            m_pActiveHead = m_pActiveHead->m_pNext;
        node->Unlink();
        --m_activeCount;
    }
    void RemoveFree(CPooledNode* node)
    {
        if (m_pFreeHead == node)
            m_pFreeHead = m_pFreeHead->m_pNext;
        node->Unlink();
        --m_freeCount;
    }
};

static int fail() { std::printf("00c3c9e0_TEST FAIL\n"); return 1; }

int main()
{
    CNodePool pool = { 0, 0, 0, 0, 0 };
    CPooledNode a, b, c;
    pool.PushActive(&a); pool.PushActive(&b); pool.PushActive(&c);         // c -> b -> a
    pool.RemoveActive(&b);                                                  // middle
    if (pool.m_pActiveHead != &c || c.m_pNext != &a || a.m_pPrev != &c || pool.m_activeCount != 2) return fail();
    pool.RemoveActive(&c);                                                  // head
    if (pool.m_pActiveHead != &a || a.m_pPrev != 0 || pool.m_activeCount != 1) return fail();
    pool.RemoveActive(&a);
    if (pool.m_pActiveHead != 0 || pool.m_activeCount != 0) return fail();
    std::printf("00c3c9e0_TEST PASS\n");
    return 0;
}