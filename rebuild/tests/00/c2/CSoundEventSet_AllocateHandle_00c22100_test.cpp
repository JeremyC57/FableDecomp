// behaviour test for 0x00C22100 CSoundEventSet::AllocateHandle: install a stack failure handler around the pool allocation, restore the default one, return the handle or -1
#include <cstdio>

struct CSoundEventManager { int dummy; };

class CFailedAllocationHandler { public:
    virtual ~CFailedAllocationHandler() {}
    virtual long OnAllocationFailed(long size) = 0;
};

static int g_handlerCalls = 0;

struct CSoundEventFailedAllocationHandler : CFailedAllocationHandler {
    CSoundEventManager* m_pManager;
    long    m_key;
    CSoundEventFailedAllocationHandler(CSoundEventManager* manager, long key) : m_pManager(manager), m_key(key) {}
    virtual long OnAllocationFailed(long) { ++g_handlerCalls; return 0; }
};

struct CDefaultFailedAllocationHandler : CFailedAllocationHandler {
    virtual long OnAllocationFailed(long) { return 0; }
};

class CHandleBasedMemoryPool { public:
    CDefaultFailedAllocationHandler m_defaultHandler;
    CFailedAllocationHandler*       m_pFailureHandler;
    long m_free;
    long m_next;
    long m_seenKey;
    long Allocate(long* handle, long size)                  // stand-in for 0x00C28C20
    {
        if (size > m_free) {
            m_pFailureHandler->OnAllocationFailed(size);
            m_seenKey = static_cast<CSoundEventFailedAllocationHandler*>(m_pFailureHandler)->m_key;
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

struct CSoundEventSet {
    CHandleBasedMemoryPool m_pool;
    long AllocateHandle(CSoundEventManager* manager, long size, long key);
};

long CSoundEventSet::AllocateHandle(CSoundEventManager* manager, long size, long key)
{
    CSoundEventFailedAllocationHandler handler(manager, key);
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
    CSoundEventManager manager;
    CSoundEventSet set;
    set.m_pool.m_pFailureHandler = &set.m_pool.m_defaultHandler;
    set.m_pool.m_free = 100; set.m_pool.m_next = 5; set.m_pool.m_seenKey = 0;
    if (set.AllocateHandle(&manager, 60, 7) != 5 || set.m_pool.m_pFailureHandler != &set.m_pool.m_defaultHandler) { std::printf("00c22100_TEST FAIL\n"); return 1; }
    if (set.AllocateHandle(&manager, 60, 9) != -1 || g_handlerCalls != 1 || set.m_pool.m_seenKey != 9) { std::printf("00c22100_TEST FAIL\n"); return 1; }
    if (set.m_pool.m_pFailureHandler != &set.m_pool.m_defaultHandler) { std::printf("00c22100_TEST FAIL\n"); return 1; }
    if (set.AllocateHandle(&manager, 40, 1) != 6) { std::printf("00c22100_TEST FAIL\n"); return 1; }
    std::printf("00c22100_TEST PASS\n");
    return 0;
}