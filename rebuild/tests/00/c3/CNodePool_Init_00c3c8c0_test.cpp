// Behaviour test for 00c3c8c0: CNodePool::Init -- threads count nodes of a block (stride bytes apart) into the free chain and sets the free count
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

void Init(CNodePool* pool, void* block, long stride, long count)
{
    CPooledNode* prev = 0;
    CPooledNode* node = (CPooledNode*)block;
    CPooledNode* next = (CPooledNode*)((char*)block + stride);
    long n = count;
    while (--n) {
        node->Link(prev, next);
        prev = node;
        node = next;
        next = (CPooledNode*)((char*)next + stride);
    }
    pool->m_pFreeHead = (CPooledNode*)block;
    pool->m_freeCount = count;
}

static int fail() { std::printf("00c3c8c0_TEST FAIL\n"); return 1; }

int main()
{
    CPooledNode nodes[4];
    CNodePool pool = { 0, 0, 0, 0, 0 };
    Init(&pool, nodes, sizeof(CPooledNode), 4);
    if (pool.m_pFreeHead != &nodes[0] || pool.m_freeCount != 4) return fail();
    if (nodes[0].m_pPrev != 0 || nodes[0].m_pNext != &nodes[1]) return fail();
    if (nodes[1].m_pPrev != &nodes[0] || nodes[1].m_pNext != &nodes[2]) return fail();
    if (nodes[2].m_pNext != &nodes[3] || nodes[3].m_pPrev != &nodes[2] || nodes[3].m_pNext != 0) return fail();
    std::printf("00c3c8c0_TEST PASS\n");
    return 0;
}