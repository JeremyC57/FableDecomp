// Behaviour test for 00c3c930: CNodePool::Clear -- a pooled (recycling) pool just forgets both chains and disables recycling; otherwise every active node is deleted
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

void Clear(CNodePool* pool)
{
    if (pool->m_freeCount >= 0) {
        pool->m_pActiveHead = 0;
        pool->m_pFreeHead = 0;
        pool->m_activeCount = 0;
        pool->m_freeCount = -1;
        return;
    }
    while (pool->m_pActiveHead) {
        CPooledNode* node = pool->m_pActiveHead;
        pool->m_pActiveHead = pool->m_pActiveHead->m_pNext;
        delete node;
        --pool->m_activeCount;
    }
}

static int fail() { std::printf("00c3c930_TEST FAIL\n"); return 1; }

int main()
{
    CPooledNode block[2];
    CNodePool pool = { 0, 0, 0, 0, 0 };
    pool.PushActive(&block[0]); pool.PushFree(&block[1]);
    Clear(&pool);                                                           // pooled storage: just forget the chains
    if (pool.m_pActiveHead || pool.m_pFreeHead || pool.m_activeCount != 0 || pool.m_freeCount != -1 || g_deleted != 0) return fail();
    pool.PushActive(new CPooledNode); pool.PushActive(new CPooledNode); pool.PushActive(new CPooledNode);
    Clear(&pool);                                                           // heap nodes: delete them all
    if (pool.m_pActiveHead != 0 || pool.m_activeCount != 0 || g_deleted != 3) return fail();
    std::printf("00c3c930_TEST PASS\n");
    return 0;
}