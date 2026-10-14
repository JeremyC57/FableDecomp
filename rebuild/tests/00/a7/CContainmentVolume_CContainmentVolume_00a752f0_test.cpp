#include <cstdio>
struct CPlane { float Normal_X; float Normal_Y; float Normal_Z; float Distance; };
static long g_freed = -1;
struct CMovableResourceMemoryPool_Calls {
    long m_handle;
    long GetHandle(void* block) { return m_handle; }
    void Free(long handle) { g_freed = handle; }
};
struct CMovableResource { virtual void v00() { } ~CMovableResource() { } };
struct CContainmentVolume : CMovableResource {
    CPlane*                           ContainmentPlanes;
    unsigned long                     Size;
    CMovableResourceMemoryPool_Calls* MemoryPool;
    virtual void v00() { }
    ~CContainmentVolume();
};
CContainmentVolume::~CContainmentVolume()
{
    if (ContainmentPlanes) {
        if (MemoryPool) {
            MemoryPool->Free(MemoryPool->GetHandle(ContainmentPlanes));
            MemoryPool = 0;
        } else {
            delete[] ContainmentPlanes;
        }
        ContainmentPlanes = 0;
        Size = 0;
    }
}
int main()
{
    CMovableResourceMemoryPool_Calls pool; pool.m_handle = 31;
    CPlane planes[2];
    {
        CContainmentVolume v; v.ContainmentPlanes = planes; v.Size = 2; v.MemoryPool = &pool;
        v.~CContainmentVolume();
        if (g_freed != 31 || v.MemoryPool != 0 || v.ContainmentPlanes != 0 || v.Size != 0) { std::printf("00a752f0_TEST FAIL\n"); return 1; }
        v.ContainmentPlanes = 0;   // the explicit destructor call above already cleared it; keep the scope exit a no-op
    }
    g_freed = -1;
    {
        CContainmentVolume w; w.ContainmentPlanes = new CPlane[3]; w.Size = 3; w.MemoryPool = 0;
        w.~CContainmentVolume();
        if (g_freed != -1 || w.ContainmentPlanes != 0 || w.Size != 0) { std::printf("00a752f0_TEST FAIL\n"); return 1; }
    }
    std::printf("00a752f0_TEST PASS\n");
    return 0;
}