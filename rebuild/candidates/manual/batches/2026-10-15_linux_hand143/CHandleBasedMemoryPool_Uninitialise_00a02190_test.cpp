// Behaviour test for 0x00a02190 (CHandleBasedMemoryPool): the reconstructed pool routine over a host model of the pool and the global allocator.
#include <cstdio>
#include <cstdlib>
// Host model of the pool bookkeeping: a fake global allocator (malloc-backed, counting live blocks), the
// critical-section calls counted, and the pool fields the four routines touch.
struct Allocator { int live; unsigned int lastFlags; long lastSize; };
static Allocator* g_alloc = 0;
static bool g_destroyWhenEmpty = false;
static int g_csInit = 0, g_csDelete = 0, g_csEnter = 0, g_csLeave = 0;
static void* AllocFromGlobal(long size, unsigned int flags)
{
    if (!g_alloc) { g_alloc = new Allocator; g_alloc->live = 0; }
    g_alloc->live++; g_alloc->lastFlags = flags; g_alloc->lastSize = size;
    return std::malloc(size);
}
static void FreeToGlobal(void* block)
{
    if (block && g_alloc) {
        std::free(block); g_alloc->live--;
        if (g_destroyWhenEmpty && g_alloc->live == 0) { delete g_alloc; g_alloc = 0; }
    }
}
struct CHeader {
    int Handle; int _unused04; int Size; short Padding; char Type; char Flags;
    CHeader** PPrevFreeLink; CHeader* PNextFree; CHeader* PPrevPhysical; CHeader* PNextPhysical;
};
struct Pool {
    void* Buffer; void* UnalignedBuffer; int PoolSize; int AllocatedSize; int NumAllocations; int LockCount; int LargestFreeArea;
    unsigned int MemoryFlags; bool MultiThreaded; bool ShutdownFlag;
    CHeader* FreeLists[256];
    Pool() : Buffer(0), UnalignedBuffer(0), PoolSize(0), AllocatedSize(5), NumAllocations(6), LockCount(7), LargestFreeArea(0), MemoryFlags(0), MultiThreaded(false), ShutdownFlag(false)
    { for (int i = 0; i < 256; ++i) FreeLists[i] = 0; }
    void InsertFreeArea(CHeader* area)
    {
        area->Size += area->Padding; area->Padding = 0;
        unsigned int bucket = (unsigned int)area->Size >> 9; if (bucket >= 255) bucket = 255;
        area->PPrevFreeLink = &FreeLists[bucket]; area->PNextFree = FreeLists[bucket];
        if (area->PNextFree) area->PNextFree->PPrevFreeLink = &area->PNextFree;
        FreeLists[bucket] = area;
        if (LargestFreeArea < area->Size) LargestFreeArea = area->Size;
    }
    void CreateInitialFreeArea()
    {
        CHeader* area = (CHeader*)Buffer;
        area->Size = PoolSize - 32; area->Padding = 0; area->Type = 'N'; area->Flags = 0;
        area->PPrevPhysical = 0; area->PNextPhysical = 0; area->Handle = 0;
        InsertFreeArea(area);
    }
    static void* Align(void* p) { return (void*)((((size_t)p) + 15) / 16 * 16); }
    void Uninitialise()
    {
        if (UnalignedBuffer) { FreeToGlobal(UnalignedBuffer); UnalignedBuffer = 0; }
        Buffer = 0; PoolSize = 0;
        if (MultiThreaded) { ++g_csDelete; MultiThreaded = false; }
        AllocatedSize = 0; NumAllocations = 0; LockCount = 0; LargestFreeArea = 0;
        for (int i = 0; i < 256; ++i) FreeLists[i] = 0;    // host-side reset so the free lists don't dangle
    }
    void Initialise(int poolSize, bool mt, unsigned int flags)
    {
        Uninitialise(); MultiThreaded = mt; if (mt) ++g_csInit;
        PoolSize = poolSize; MemoryFlags = flags;
        UnalignedBuffer = AllocFromGlobal(PoolSize + 16, MemoryFlags);
        Buffer = Align(UnalignedBuffer);
        CreateInitialFreeArea();
    }
    void InitialiseFromPreallocatedBuffer(void* buffer, int size, bool mt)
    {
        Uninitialise(); MultiThreaded = mt; if (mt) ++g_csInit;
        size_t start = (size_t)buffer; size_t end = start + size;
        start = (start + 15) / 16 * 16; end = end / 16 * 16;
        Buffer = (void*)start; PoolSize = (int)(end - start); MemoryFlags = 0;
        CreateInitialFreeArea();
    }
    void EndShutdown()
    {
        if (MultiThreaded) ++g_csEnter;
        if (ShutdownFlag) {
            ShutdownFlag = false;
            UnalignedBuffer = AllocFromGlobal(PoolSize + 16, MemoryFlags);
            Buffer = Align(UnalignedBuffer);
            CreateInitialFreeArea();
        }
        if (MultiThreaded) ++g_csLeave;
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
int main()
{
    Pool p; p.Initialise(4096, true, 3);
    CHECK(g_alloc && g_alloc->live == 1 && g_csInit == 1);
    g_destroyWhenEmpty = true;
    p.Uninitialise();
    CHECK(p.UnalignedBuffer == 0 && p.Buffer == 0 && p.PoolSize == 0 && !p.MultiThreaded && g_csDelete == 1);
    CHECK(p.AllocatedSize == 0 && p.NumAllocations == 0 && p.LockCount == 0 && p.LargestFreeArea == 0);
    CHECK(g_alloc == 0);                                   // destroyed when its last block went back
    p.Uninitialise(); CHECK(g_csDelete == 1);              // a second reset frees nothing
    if (g_fail) { std::printf("00a02190_TEST FAIL\n"); return 1; }
    std::printf("00a02190_TEST PASS\n");
    return 0;
}
