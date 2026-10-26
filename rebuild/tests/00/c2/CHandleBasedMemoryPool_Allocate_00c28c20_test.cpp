// Behaviour test for CHandleBasedMemoryPool::Allocate @ 0x00C28C20 (copy of the model and logic; callees are fakes).
#include <cstdio>

class CFailedAllocationHandler { public:
    int calls;
    int freesPerCall;
    int budget;                       // how many calls succeed
    struct CPool* pool;
    int HandleFailure(long size);
};
struct CPool {
    long PoolSize;
    long AllocatedSize;
    long LargestFreeArea;
    long MaxFreeSizeBeforeDefrag;
    long MaxFreeSizeBeforeFailure;
    long EnableFullDefragIfPartialFails;
    CFailedAllocationHandler* FailureHandler;
    int partialCalls, partialResult, defragCalls, allocCalls;

    int PartialDefragment(long) { ++partialCalls; return partialResult; }
    void Defragment() { ++defragCalls; LargestFreeArea = PoolSize - AllocatedSize; }
    int InternalAllocate(long* handle, long size) { ++allocCalls; *handle = size; return 1; }

    int Allocate(long* handle, long size)
    {
        if (FailureHandler) {
            while (size > PoolSize - AllocatedSize) {
                if (!FailureHandler->HandleFailure(size))
                    return 0;
            }
            while (LargestFreeArea < size) {
                if (PoolSize - AllocatedSize >= MaxFreeSizeBeforeDefrag + size)
                    break;
                if (!FailureHandler->HandleFailure(size))
                    break;
            }
            if (LargestFreeArea < size && !PartialDefragment(size)) {
                while (LargestFreeArea < size) {
                    if (PoolSize - AllocatedSize >= MaxFreeSizeBeforeFailure + size)
                        break;
                    if (!FailureHandler->HandleFailure(size))
                        break;
                }
                if (LargestFreeArea < size && !PartialDefragment(size)) {
                    if (EnableFullDefragIfPartialFails)
                        Defragment();
                    else
                        return 0;
                }
            }
        }
        return InternalAllocate(handle, size);
    }
};
int CFailedAllocationHandler::HandleFailure(long)
{
    ++calls;
    if (calls > budget)
        return 0;
    pool->AllocatedSize -= freesPerCall;
    return 1;
}

static void Reset(CPool& p, CFailedAllocationHandler& h)
{
    p.PoolSize = 1000; p.AllocatedSize = 0; p.LargestFreeArea = 1000;
    p.MaxFreeSizeBeforeDefrag = 100; p.MaxFreeSizeBeforeFailure = 200; p.EnableFullDefragIfPartialFails = 1;
    p.FailureHandler = &h; p.partialCalls = 0; p.partialResult = 0; p.defragCalls = 0; p.allocCalls = 0;
    h.calls = 0; h.freesPerCall = 50; h.budget = 100; h.pool = &p;
}

int main()
{
    CPool p; CFailedAllocationHandler h; long handle = 0;
    // 1. plenty of room: straight to InternalAllocate, no handler calls
    Reset(p, h);
    if (p.Allocate(&handle, 64) != 1 || handle != 64 || h.calls != 0 || p.allocCalls != 1) { std::printf("00c28c20_TEST FAIL\n"); return 1; }
    // 2. no handler: allocates even when full
    Reset(p, h); p.FailureHandler = 0; p.AllocatedSize = 1000;
    if (p.Allocate(&handle, 64) != 1 || p.allocCalls != 1) { std::printf("00c28c20_TEST FAIL\n"); return 1; }
    // 3. pool full and the handler gives up at once: 0, nothing allocated
    Reset(p, h); p.AllocatedSize = 990; h.budget = 0;
    if (p.Allocate(&handle, 64) != 0 || p.allocCalls != 0 || h.calls != 1) { std::printf("00c28c20_TEST FAIL\n"); return 1; }
    // 4. pool full: the handler frees 30 per call until 64 bytes are free (20 -> 50 -> 80: 2 calls), then allocates
    Reset(p, h); p.AllocatedSize = 980; h.freesPerCall = 30;
    if (p.Allocate(&handle, 64) != 1 || h.calls != 2 || p.allocCalls != 1) { std::printf("00c28c20_TEST FAIL\n"); return 1; }
    // 5. fragmented: room but the largest area is too small, handler refuses, partial defrag fails twice,
    //    full defrag enabled -> Defragment then allocate
    Reset(p, h); p.LargestFreeArea = 10; h.budget = 0;
    if (p.Allocate(&handle, 64) != 1 || p.partialCalls != 2 || p.defragCalls != 1 || p.allocCalls != 1) { std::printf("00c28c20_TEST FAIL\n"); return 1; }
    // 6. same with full defrag disabled -> 0
    Reset(p, h); p.LargestFreeArea = 10; h.budget = 0; p.EnableFullDefragIfPartialFails = 0;
    if (p.Allocate(&handle, 64) != 0 || p.partialCalls != 2 || p.defragCalls != 0 || p.allocCalls != 0) { std::printf("00c28c20_TEST FAIL\n"); return 1; }
    // 7. partial defrag succeeds on the first try -> allocate without full defrag
    Reset(p, h); p.LargestFreeArea = 10; h.budget = 0; p.partialResult = 1;
    if (p.Allocate(&handle, 64) != 1 || p.partialCalls != 1 || p.defragCalls != 0) { std::printf("00c28c20_TEST FAIL\n"); return 1; }
    // 8. free total already above the defrag threshold: no handler calls in the second wait
    Reset(p, h); p.LargestFreeArea = 10; p.partialResult = 1;   // free 1000 >= 100 + 64
    if (p.Allocate(&handle, 64) != 1 || h.calls != 0) { std::printf("00c28c20_TEST FAIL\n"); return 1; }
    std::printf("00c28c20_TEST PASS\n");
    return 0;
}