#include <cstdio>
#include <cstdlib>
// Behaviour test for CHandleBasedMemoryPool scalar deleting destructor (0x00A03AA0): Uninitialise runs first,
// then the name string, then the handle array's block is freed only when allocated, then the CBase root.
static int g_order[8]; static int g_n = 0; static int g_freed = 0;
struct CCharString { void* m_pData; ~CCharString() { g_order[g_n++] = 3; } };
struct CBase { virtual ~CBase() { g_order[g_n++] = 1; } };
struct CHandleInfoArray {
    void* m_pData; long m_size; long m_capacity;
    ~CHandleInfoArray() { g_order[g_n++] = 2; if (m_pData) { std::free(m_pData); ++g_freed; } }
};
struct CHandleBasedMemoryPool : CBase {
    CHandleInfoArray Handles; long FirstUnusedHandle; CCharString Name;
    void Uninitialise() { g_order[g_n++] = 4; }
    virtual ~CHandleBasedMemoryPool() { Uninitialise(); }
};
int main()
{
    CHandleBasedMemoryPool* m = new CHandleBasedMemoryPool;
    m->Handles.m_pData = std::malloc(32);
    CBase* p = m;
    delete p;
    if (g_n != 4 || g_order[0] != 4 || g_order[1] != 3 || g_order[2] != 2 || g_order[3] != 1 || g_freed != 1) { std::printf("00a03aa0_TEST FAIL\n"); return 1; }
    g_n = 0;
    m = new CHandleBasedMemoryPool; m->Handles.m_pData = 0;
    delete m;
    if (g_freed != 1 || g_n != 4) { std::printf("00a03aa0_TEST FAIL\n"); return 1; }
    std::printf("00a03aa0_TEST PASS\n");
    return 0;
}