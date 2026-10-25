// Behaviour test for 00c3cde0: RecycleGlobalNode -- takes the node off the global pool's active chain, then resets and recycles it, or deletes it when recycling is off
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

static CNodePool g_nodePool = { 0, 0, 0, 0, 0 };

void RecycleGlobalNode(CPooledNode* node)
{
    g_nodePool.RemoveActive(node);
    if (g_nodePool.m_freeCount >= 0) {
        node->Reset();
        g_nodePool.PushFree(node);
    } else
        delete node;
}

static int fail() { std::printf("00c3cde0_TEST FAIL\n"); return 1; }

int main()
{
    CPooledNode* a = new CPooledNode;
    CPooledNode* b = new CPooledNode;
    g_nodePool.PushActive(a); g_nodePool.PushActive(b);
    RecycleGlobalNode(a);                                                   // recycling on: reset + free chain
    if (g_reset != 1 || g_nodePool.m_pFreeHead != a || g_nodePool.m_freeCount != 1 || g_nodePool.m_activeCount != 1) return fail();
    g_nodePool.m_freeCount = -1;                                            // recycling off: deleted
    RecycleGlobalNode(b);
    if (g_deleted != 1 || g_nodePool.m_pActiveHead != 0 || g_nodePool.m_activeCount != 0 || g_reset != 1) return fail();
    delete a;
    std::printf("00c3cde0_TEST PASS\n");
    return 0;
}