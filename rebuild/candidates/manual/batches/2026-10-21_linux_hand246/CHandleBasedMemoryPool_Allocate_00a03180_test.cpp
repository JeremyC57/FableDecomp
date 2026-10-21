// Behaviour test for CHandleBasedMemoryPool::Allocate @ 0x00A03180 (copy of the model with stub lock/handler/allocator).
#include <cstdio>
static int g_enter, g_leave, g_tryFail, g_handled, g_defrags;
struct CFailedAllocationHandler {
    long freePerCall; int budget; struct CHandleBasedMemoryPool* pool;
    bool HandleFailure(unsigned long size);
};
struct CHandleBasedMemoryPool {
    long PoolSize, AllocatedSize, LargestFreeArea; bool MultiThreaded, ShutdownFlag;
    unsigned long MaxFreeSizeBeforeDefrag, MaxFreeSizeBeforeFailure; CFailedAllocationHandler* FailureHandler; bool defragWorks;
    bool BeginCritical(bool tryOnly) { if (!MultiThreaded) return true; if (tryOnly && g_tryFail) return false; ++g_enter; return true; }
    void EndCritical() { if (MultiThreaded) ++g_leave; }
    bool PartialDefragment(long) { ++g_defrags; if (defragWorks) LargestFreeArea = PoolSize - AllocatedSize; return defragWorks; }
    bool InternalAllocate(long* handle, unsigned long size) { if ((unsigned long)LargestFreeArea < size) return false; AllocatedSize += (long)size; *handle = 7; return true; }
    bool Allocate(long* handle, unsigned long size, unsigned long flags);
};
bool CFailedAllocationHandler::HandleFailure(unsigned long) { ++g_handled; if (budget-- <= 0) return false; pool->AllocatedSize -= freePerCall; return true; }
struct CScopedCritical {
    CHandleBasedMemoryPool* m_pPool; bool m_locked;
    CScopedCritical(CHandleBasedMemoryPool* pool, bool tryOnly) : m_pPool(pool) { m_locked = pool->BeginCritical(tryOnly); }
    ~CScopedCritical() { if (m_locked) m_pPool->EndCritical(); }
};
bool CHandleBasedMemoryPool::Allocate(long* handle, unsigned long size, unsigned long flags)
{
    CScopedCritical lock(this, (flags & 1) != 0);
    if (!lock.m_locked) return false;
    if (ShutdownFlag) return false;
    if (FailureHandler && ((unsigned char)~(flags >> 1) & 1)) {
        while (size > (unsigned long)(PoolSize - AllocatedSize)) { if (!FailureHandler->HandleFailure(size)) return false; }
        while ((unsigned long)LargestFreeArea < size) {
            if ((unsigned long)(PoolSize - AllocatedSize) >= MaxFreeSizeBeforeDefrag + size) break;
            if (!FailureHandler->HandleFailure(size)) break;
        }
        if ((unsigned long)LargestFreeArea < size && !PartialDefragment(size)) {
            while ((unsigned long)LargestFreeArea < size) {
                if ((unsigned long)(PoolSize - AllocatedSize) >= MaxFreeSizeBeforeFailure + size) break;
                if (!FailureHandler->HandleFailure(size)) break;
            }
            if ((unsigned long)LargestFreeArea < size && !PartialDefragment(size)) return false;
        }
    }
    bool result = InternalAllocate(handle, size);
    return result;
}
static void reset(CHandleBasedMemoryPool& p, CFailedAllocationHandler& h)
{
    p.PoolSize = 1000; p.AllocatedSize = 900; p.LargestFreeArea = 100; p.MultiThreaded = true; p.ShutdownFlag = false;
    p.MaxFreeSizeBeforeDefrag = 50; p.MaxFreeSizeBeforeFailure = 100; p.FailureHandler = &h; p.defragWorks = true;
    h.freePerCall = 100; h.budget = 10; h.pool = &p; g_enter = g_leave = g_tryFail = g_handled = g_defrags = 0;
}
int main()
{
    int bad = 0; CHandleBasedMemoryPool p; CFailedAllocationHandler h; long handle;
    reset(p, h); handle = 0; if (!p.Allocate(&handle, 80, 0) || handle != 7 || g_enter != 1 || g_leave != 1 || g_handled) bad++;
    reset(p, h); if (!p.Allocate(&handle, 250, 0) || g_handled < 2 || g_defrags != 1 || g_leave != 1) bad++;   // handler frees, defrag merges
    reset(p, h); h.budget = 0; if (p.Allocate(&handle, 250, 0) || g_leave != 1) bad++;                         // handler gives up
    reset(p, h); if (p.Allocate(&handle, 250, 2) || g_handled || g_leave != 1) bad++;                            // no-handler flag
    reset(p, h); p.ShutdownFlag = true; if (p.Allocate(&handle, 10, 0) || g_leave != 1) bad++;
    reset(p, h); g_tryFail = 1; if (p.Allocate(&handle, 10, 1) || g_enter || g_leave) bad++;
    reset(p, h); p.defragWorks = false; h.budget = 2; if (p.Allocate(&handle, 250, 0) || g_defrags != 2 || g_leave != 1) bad++;
    if (bad) { std::printf("00a03180_TEST FAIL\n"); return 1; }
    std::printf("00a03180_TEST PASS\n");
    return 0;
}
