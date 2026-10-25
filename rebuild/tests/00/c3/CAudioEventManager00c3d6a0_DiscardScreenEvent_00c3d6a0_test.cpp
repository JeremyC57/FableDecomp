// Behaviour test for 00c3d6a0: CAudioEventManager::DiscardScreenEvent -- releases the event's resource, takes it off the screen pool's active chain and recycles or deletes it
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

struct CAudioEventManager {
    long m_pad[22];
    CNodePool m_screenPool;
    void DiscardScreenEvent(CPooledNode* event)
    {
        event->ReleaseResource();
        m_screenPool.RemoveActive(event);
        if (m_screenPool.m_freeCount >= 0)
            m_screenPool.PushFree(event);
        else
            delete event;
    }
};

static int fail() { std::printf("00c3d6a0_TEST FAIL\n"); return 1; }

int main()
{
    CAudioEventManager m;
    m.m_screenPool.m_pActiveHead = 0; m.m_screenPool.m_pFreeHead = 0;
    m.m_screenPool.m_freeCount = 2; m.m_screenPool.m_activeCount = 0;
    CPooledNode* a = new CPooledNode;
    CPooledNode* b = new CPooledNode;
    m.m_screenPool.PushActive(a); m.m_screenPool.PushActive(b);
    m.DiscardScreenEvent(b);
    if (b->m_released != 1 || m.m_screenPool.m_pFreeHead != b || m.m_screenPool.m_freeCount != 3 || m.m_screenPool.m_pActiveHead != a) return fail();
    m.m_screenPool.m_freeCount = -1;
    m.DiscardScreenEvent(a);
    if (g_deleted != 1 || m.m_screenPool.m_activeCount != 0) return fail();
    delete b;
    std::printf("00c3d6a0_TEST PASS\n");
    return 0;
}