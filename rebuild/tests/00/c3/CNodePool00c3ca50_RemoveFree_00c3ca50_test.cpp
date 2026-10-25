// Behaviour test for 00c3ca50: CNodePool::RemoveFree -- advances the free head past the node when it is the head, unlinks it and decrements the free count
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

static int fail() { std::printf("00c3ca50_TEST FAIL\n"); return 1; }

int main()
{
    CNodePool pool = { 0, 0, 0, 0, 0 };
    CPooledNode a, b;
    pool.PushFree(&a); pool.PushFree(&b);                                   // b -> a
    pool.RemoveFree(&a);                                                    // tail
    if (pool.m_pFreeHead != &b || b.m_pNext != 0 || pool.m_freeCount != 1) return fail();
    pool.RemoveFree(&b);                                                    // head
    if (pool.m_pFreeHead != 0 || pool.m_freeCount != 0) return fail();
    std::printf("00c3ca50_TEST PASS\n");
    return 0;
}