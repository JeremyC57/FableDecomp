// Behaviour test for CHandleBasedMemoryPool::BeginShutdown @ 0x00A02220 (copy of the model with stub time/lock/allocator).
#include <cstdio>
#include <cstring>
static double g_time; static int g_enter, g_leave, g_sleeps, g_freed, g_destroyed;
static double GFGetTime() { return g_time; }
static void Sleep(unsigned long ms) { g_time += ms / 1000.0; ++g_sleeps; }
struct CMemoryAllocator { long count; };
static CMemoryAllocator g_alloc; static CMemoryAllocator* g_pGlobalAllocator; static bool g_bDestroyWhenEmpty;
static void GFFreeFromGlobalAllocator(void* block)
{
    if (block && g_pGlobalAllocator) {
        --g_pGlobalAllocator->count; ++g_freed;
        if (g_bDestroyWhenEmpty && g_pGlobalAllocator->count == 0) { ++g_destroyed; g_pGlobalAllocator = 0; }
    }
}
struct CFailedAllocationHandler { long freePerCall; bool HandleFailure(unsigned long); struct CHandleBasedMemoryPool* pool; };
struct CHandleBasedMemoryPool {
    void* Buffer; void* UnalignedBuffer; long AllocatedSize; bool MultiThreaded, ShutdownFlag;
    CFailedAllocationHandler* FailureHandler; void* FreeLists[256];
    void BeginCritical(bool) { if (MultiThreaded) ++g_enter; }
    void EndCritical() { if (MultiThreaded) ++g_leave; }
    void ClearFreeLists() { std::memset(FreeLists, 0, sizeof(FreeLists)); }
    bool BeginShutdown(float timeout)
    {
        BeginCritical(false);
        double startTime = GFGetTime();
        for (;;) {
            while (AllocatedSize > 0) { if (!FailureHandler) break; if (!FailureHandler->HandleFailure(AllocatedSize)) break; }
            if (AllocatedSize == 0) break;
            EndCritical(); Sleep(10); BeginCritical(false);
            if (!(GFGetTime() <= startTime + timeout)) { EndCritical(); return false; }
        }
        GFFreeFromGlobalAllocator(UnalignedBuffer);
        UnalignedBuffer = 0; Buffer = 0; ShutdownFlag = true;
        ClearFreeLists();
        EndCritical();
        return true;
    }
};
bool CFailedAllocationHandler::HandleFailure(unsigned long) { if (freePerCall <= 0) return false; pool->AllocatedSize -= freePerCall; if (pool->AllocatedSize < 0) pool->AllocatedSize = 0; return true; }
static char g_buf[16];
static void reset(CHandleBasedMemoryPool& p, CFailedAllocationHandler& h, long alloc, long freePer)
{
    g_time = 100.0; g_enter = g_leave = g_sleeps = g_freed = g_destroyed = 0; g_alloc.count = 1; g_pGlobalAllocator = &g_alloc; g_bDestroyWhenEmpty = true;
    p.Buffer = g_buf; p.UnalignedBuffer = g_buf; p.AllocatedSize = alloc; p.MultiThreaded = true; p.ShutdownFlag = false;
    p.FailureHandler = &h; h.freePerCall = freePer; h.pool = &p; for (int i = 0; i < 256; ++i) p.FreeLists[i] = g_buf;
}
int main()
{
    int bad = 0; CHandleBasedMemoryPool p; CFailedAllocationHandler h;
    reset(p, h, 300, 100); if (!p.BeginShutdown(1.0f) || !p.ShutdownFlag || p.Buffer || p.UnalignedBuffer || p.FreeLists[7] || g_freed != 1 || g_destroyed != 1 || g_enter != g_leave) bad++;
    reset(p, h, 300, 0); if (p.BeginShutdown(0.5f) || p.ShutdownFlag || g_sleeps < 50 || g_enter != g_leave) bad++;   // nothing freed: times out
    reset(p, h, 300, 0); p.FailureHandler = 0; if (p.BeginShutdown(0.05f) || g_enter != g_leave) bad++;
    if (bad) { std::printf("00a02220_TEST FAIL\n"); return 1; }
    std::printf("00a02220_TEST PASS\n");
    return 0;
}