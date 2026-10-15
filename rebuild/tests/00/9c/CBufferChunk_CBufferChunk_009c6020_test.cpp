#include <cstdio>
static int g_enter = 0, g_leave = 0, g_freed = 0, g_destroyed = 0;
struct CMemoryAllocator {
    long m_count;
    void InternalDeallocate(void*) { ++g_freed; --m_count; }
    long GetAllocationCount() { return m_count; }
    ~CMemoryAllocator() { ++g_destroyed; }
};
static void GFEnterAllocatorLock() { ++g_enter; }
static void GFLeaveAllocatorLock() { ++g_leave; }
CMemoryAllocator* g_pGlobalAllocator_013caa90 = 0;
bool g_bDestroyAllocatorWhenEmpty_013caa94 = false;
static void GFFreeFromGlobalAllocator(void* block)
{
    GFEnterAllocatorLock();
    if (block && g_pGlobalAllocator_013caa90) {
        g_pGlobalAllocator_013caa90->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty_013caa94) {
            if (g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
    }
    GFLeaveAllocatorLock();
}
struct CBufferChunk {
    unsigned char* Mem; bool Used; bool IsInput;
    ~CBufferChunk() { if (Mem) GFFreeFromGlobalAllocator(Mem); }
};
int main()
{
    unsigned char a, b;
    { CBufferChunk c; c.Mem = 0; }
    if (g_enter != 0) { std::printf("009c6020_TEST FAIL\n"); return 1; }
    g_pGlobalAllocator_013caa90 = new CMemoryAllocator; g_pGlobalAllocator_013caa90->m_count = 2;
    { CBufferChunk c; c.Mem = &a; }
    if (g_enter != 1 || g_leave != 1 || g_freed != 1 || g_destroyed != 0) { std::printf("009c6020_TEST FAIL\n"); return 1; }
    g_bDestroyAllocatorWhenEmpty_013caa94 = true;
    { CBufferChunk c; c.Mem = &b; }
    if (g_freed != 2 || g_destroyed != 1 || g_pGlobalAllocator_013caa90 != 0 || g_leave != 2) { std::printf("009c6020_TEST FAIL\n"); return 1; }
    { CBufferChunk c; c.Mem = &b; }
    if (g_freed != 2 || g_enter != 3 || g_leave != 3) { std::printf("009c6020_TEST FAIL\n"); return 1; }
    std::printf("009c6020_TEST PASS\n");
    return 0;
}