#include <cstdio>
// Behaviour of CPooledResource::Release (0x00C265B0).
class CHandleBasedMemoryPool { public: long m_freed; long m_last; void FreeAllocation(long handle) { ++m_freed; m_last = handle; } };
struct CPooledResourceManager { CHandleBasedMemoryPool m_pool; long m_deactivations; };
struct CPooledResource {
    long m_handle; short m_state;
    long IsActive() { return m_state == 1; }
    void Deactivate(CPooledResourceManager* manager) { m_state = 2; ++manager->m_deactivations; }
    void Release(CPooledResourceManager* manager)
    {
        if (IsActive())
            Deactivate(manager);
        long handle = m_handle;
        if (handle >= 0)
            manager->m_pool.FreeAllocation(handle);
    }
};
int main()
{
    CPooledResourceManager m; m.m_pool.m_freed = 0; m.m_pool.m_last = -9; m.m_deactivations = 0;
    CPooledResource r; r.m_handle = 6; r.m_state = 1;
    r.Release(&m);
    int bad = (m.m_deactivations != 1 || r.m_state != 2 || m.m_pool.m_freed != 1 || m.m_pool.m_last != 6);
    r.m_handle = -1; r.Release(&m);
    bad |= (m.m_deactivations != 1 || m.m_pool.m_freed != 1);
    r.m_handle = 0; r.Release(&m); bad |= (m.m_pool.m_freed != 2 || m.m_pool.m_last != 0);
    if (bad) { std::printf("00c265b0_TEST FAIL\n"); return 1; }
    std::printf("00c265b0_TEST PASS\n");
    return 0;
}