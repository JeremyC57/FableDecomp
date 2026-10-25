// behaviour test for 0x00C22100 CPanelItemSet::AllocateHandle: install a stack failure handler around the pool allocation, restore the default one, return the handle or -1
#include <cstdio>

struct CPanel { int dummy; };

struct CFailedAllocationHandler {
    virtual ~CFailedAllocationHandler() {}
    virtual long OnAllocationFailed(long size) = 0;
};

static int g_handlerCalls = 0;

struct CPanelFailedAllocationHandler : CFailedAllocationHandler {
    CPanel* m_pPanel;
    long    m_key;
    CPanelFailedAllocationHandler(CPanel* panel, long key) : m_pPanel(panel), m_key(key) {}
    virtual long OnAllocationFailed(long) { ++g_handlerCalls; return 0; }
};

struct CDefaultFailedAllocationHandler : CFailedAllocationHandler {
    virtual long OnAllocationFailed(long) { return 0; }
};

struct CHandleBasedMemoryPool {
    CDefaultFailedAllocationHandler m_defaultHandler;
    CFailedAllocationHandler*       m_pFailureHandler;
    long m_free;
    long m_next;
    long m_seenKey;
    long Allocate(long* handle, long size)                  // stand-in for 0x00C28C20
    {
        if (size > m_free) {
            m_pFailureHandler->OnAllocationFailed(size);
            m_seenKey = static_cast<CPanelFailedAllocationHandler*>(m_pFailureHandler)->m_key;
            return 0;
        }
        m_free -= size;
        *handle = m_next++;
        return 1;
    }
    void SetFailureHandler(CFailedAllocationHandler* handler)
    {
        if (handler)
            m_pFailureHandler = handler;
        else
            m_pFailureHandler = &m_defaultHandler;
    }
};

struct CPanelItemSet {
    CHandleBasedMemoryPool m_pool;
    long AllocateHandle(CPanel* panel, long size, long key);
};

long CPanelItemSet::AllocateHandle(CPanel* panel, long size, long key)
{
    CPanelFailedAllocationHandler handler(panel, key);
    m_pool.SetFailureHandler(&handler);
    long handle;
    long ok = m_pool.Allocate(&handle, size);
    m_pool.SetFailureHandler(0);
    if (!ok)
        return -1;
    return handle;
}

int main()
{
    CPanel panel;
    CPanelItemSet set;
    set.m_pool.m_pFailureHandler = &set.m_pool.m_defaultHandler;
    set.m_pool.m_free = 100; set.m_pool.m_next = 5; set.m_pool.m_seenKey = 0;
    if (set.AllocateHandle(&panel, 60, 7) != 5 || set.m_pool.m_pFailureHandler != &set.m_pool.m_defaultHandler) { std::printf("00c22100_TEST FAIL\n"); return 1; }
    if (set.AllocateHandle(&panel, 60, 9) != -1 || g_handlerCalls != 1 || set.m_pool.m_seenKey != 9) { std::printf("00c22100_TEST FAIL\n"); return 1; }
    if (set.m_pool.m_pFailureHandler != &set.m_pool.m_defaultHandler) { std::printf("00c22100_TEST FAIL\n"); return 1; }
    if (set.AllocateHandle(&panel, 40, 1) != 6) { std::printf("00c22100_TEST FAIL\n"); return 1; }
    std::printf("00c22100_TEST PASS\n");
    return 0;
}