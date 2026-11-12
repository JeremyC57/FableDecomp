// Behaviour test for CMemoryAllocator::CMemoryAllocator (0x009F20B0): the constructor's logic through a copy of the model
// (PDB layout of the fields it touches, stand-in sub-allocators that only report their area size, a 30-record table).
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CMemoryAllocatorBase {
    virtual unsigned long GetAllocationSize(void* p) = 0;
    void* SeenHeap;
    bool  Fixed;
};
struct CTestFixed : CMemoryAllocatorBase {
    unsigned long AreaSize;
    CTestFixed(void* heap, unsigned long, unsigned long maxAllocSize, unsigned long alignment, unsigned long, unsigned long)
    {
        SeenHeap = heap; Fixed = true;
        AreaSize = ((maxAllocSize + alignment - 1) / alignment) * alignment;   // the real one rounds the area size up
    }
    virtual unsigned long GetAllocationSize(void*) { return AreaSize; }
};
struct CTestVariable : CMemoryAllocatorBase {
    CTestVariable(void* heap, long, void*, long, unsigned long, unsigned long, unsigned long) { SeenHeap = heap; Fixed = false; }
    virtual unsigned long GetAllocationSize(void*) { return 0; }
};

struct CAllocatorInfo {
    long MaxAllocSize, MinChunkSize, InitialSize, Alignment;
    unsigned long Flags, InternalFlags;
    CMemoryAllocatorBase* PAllocator;
};
struct CAllocSizeToAllocator {
    CMemoryAllocatorBase* PAllocator;
    unsigned char ID;
};

static CAllocatorInfo g_AllocatorInfo[30];
static int g_initLock, g_enterLock, g_leaveLock, g_initSystem, g_mallocs;

static void* TestMalloc(unsigned int size)
{
    ++g_mallocs;
    void* p = std::malloc(size);
    std::memset(p, 0xEE, size);            // sentinel: entries the constructor does not write stay 0xEE
    return p;
}

class CMemoryAllocator { public:
    unsigned long          MemoryAllocatedPerTrackingID[256];
    unsigned long          MemoryAllocated;
    void**                 MemoryAuditBuffer;
    void*                  Heap;
    unsigned long          NoAllocations;
    unsigned long          NoAllocators;
    unsigned char          CurrentTrackingID;
    bool                   AllocatorExists[15];
    long                   MaxFixedAllocationSizes[15];
    CAllocSizeToAllocator* AllocSizeToAllocator[15];

    void Construct(void* heap)
    {
        MemoryAllocated = 0; NoAllocations = 0; CurrentTrackingID = 0;
        ++g_initLock;
        ++g_enterLock;
        ++g_initSystem;
        std::memset(MaxFixedAllocationSizes, 0, sizeof(MaxFixedAllocationSizes));
        std::memset(AllocSizeToAllocator, 0, sizeof(AllocSizeToAllocator));
        std::memset(AllocatorExists, 0, sizeof(AllocatorExists));
        std::memset(MemoryAllocatedPerTrackingID, 0, sizeof(MemoryAllocatedPerTrackingID));
        MemoryAllocated = 0;
        NoAllocators = 30;
        for (unsigned long i = 0; i < NoAllocators; ++i) {
            CAllocatorInfo& info = g_AllocatorInfo[i];
            if (info.MaxAllocSize == 0)
                continue;
            bool fixedSize = ((info.InternalFlags >> 1) & 1) != 0;
            unsigned long allocType = info.Flags;
            AllocatorExists[allocType] = true;
            if (fixedSize) {
                info.PAllocator = new CTestFixed(heap, info.MinChunkSize, info.MaxAllocSize, info.Alignment, allocType,
                                                 info.InternalFlags);
                info.MaxAllocSize = info.PAllocator->GetAllocationSize(0);
                if (info.MaxAllocSize > MaxFixedAllocationSizes[allocType])
                    MaxFixedAllocationSizes[allocType] = info.MaxAllocSize;
            } else {
                info.PAllocator = new CTestVariable(heap, info.MinChunkSize, 0, info.InitialSize, info.Alignment, allocType,
                                                    info.InternalFlags);
            }
        }
        for (long type = 0; type < 15; ++type) {
            long maxSize = MaxFixedAllocationSizes[type];
            if (!AllocatorExists[type])
                continue;
            __int64 tableBytes = ((__int64)maxSize + 2) * sizeof(CAllocSizeToAllocator);
            if (tableBytes < 0 || tableBytes > 0x7fffffff)
                return;
            AllocSizeToAllocator[type] = (CAllocSizeToAllocator*)TestMalloc((maxSize + 2) * sizeof(CAllocSizeToAllocator));
            for (long size = 0; size <= maxSize + 1; ++size) {
                for (long a = 0; (unsigned long)a < NoAllocators; ++a) {
                    CAllocatorInfo& info = g_AllocatorInfo[a];
                    if (info.Flags != (unsigned long)type)
                        continue;
                    bool fixedSize = ((info.InternalFlags >> 1) & 1) != 0;
                    if (fixedSize) {
                        long minSize = 0;
                        if (a > 0)
                            minSize = g_AllocatorInfo[a - 1].MaxAllocSize + 1;
                        if (size >= minSize && size <= info.MaxAllocSize) {
                            AllocSizeToAllocator[type][size].PAllocator = info.PAllocator;
                            AllocSizeToAllocator[type][size].ID = (unsigned char)a;
                            break;
                        }
                    } else {
                        AllocSizeToAllocator[type][size].PAllocator = info.PAllocator;
                        AllocSizeToAllocator[type][size].ID = (unsigned char)a;
                        break;
                    }
                }
            }
        }
        MemoryAuditBuffer = 0;
        ++g_leaveLock;
    }
};

static void SetInfo(int index, long maxAlloc, long alignment, unsigned long type, bool fixedSize)
{
    CAllocatorInfo& info = g_AllocatorInfo[index];
    info.MaxAllocSize = maxAlloc; info.MinChunkSize = 0x1000; info.InitialSize = 0; info.Alignment = alignment;
    info.Flags = type; info.InternalFlags = fixedSize ? 3 : 1;      // bit 1 = fixed size; bit 0 is unrelated noise
    info.PAllocator = 0;
}
static void ResetTable()
{
    std::memset(g_AllocatorInfo, 0, sizeof(g_AllocatorInfo));
    for (int i = 0; i < 30; ++i) g_AllocatorInfo[i].Flags = 14;   // unused records: type 14, MaxAllocSize 0
    g_initLock = g_enterLock = g_leaveLock = g_initSystem = g_mallocs = 0;
}
static bool Unwritten(const CAllocSizeToAllocator& e)
{
    static const unsigned char sentinel[sizeof(void*)] = { 0xEE, 0xEE, 0xEE, 0xEE };
    return std::memcmp(&e.PAllocator, sentinel, sizeof(void*)) == 0 && e.ID == 0xEE;
}
static int Fail(const char* what)
{
    std::printf("009f20b0_TEST FAIL (%s)\n", what);
    return 1;
}

static CMemoryAllocator g_model;

int main()
{
    int heapToken = 0;
    void* heap = &heapToken;
    void* auditSentinel[1];

    // Scenario 1: three fixed ranges + one variable allocator of type 0, a variable-only type 1, a fixed type 2 whose
    // range starts above the previous record's maximum, an unused record in between.
    ResetTable();
    SetInfo(0, 8, 8, 0, true);
    SetInfo(1, 16, 8, 0, true);
    SetInfo(2, 32, 8, 0, true);
    SetInfo(3, 0x7fffffff, 8, 0, false);
    SetInfo(4, 0, 8, 0, true);              // MaxAllocSize 0: skipped entirely
    SetInfo(5, 5, 8, 1, false);
    SetInfo(6, 10, 4, 2, true);             // reports 12 (rounded up to its alignment)
    std::memset(&g_model, 0xCC, sizeof(g_model));
    g_model.Heap = auditSentinel;
    g_model.Construct(heap);

    if (g_initLock != 1 || g_enterLock != 1 || g_leaveLock != 1 || g_initSystem != 1) return Fail("lock/init calls");
    if (g_model.MemoryAllocated != 0 || g_model.NoAllocations != 0 || g_model.CurrentTrackingID != 0) return Fail("counters");
    if (g_model.NoAllocators != 30 || g_model.MemoryAuditBuffer != 0) return Fail("NoAllocators/MemoryAuditBuffer");
    if (g_model.Heap != auditSentinel) return Fail("Heap must not be written");
    for (int t = 0; t < 256; ++t) if (g_model.MemoryAllocatedPerTrackingID[t] != 0) return Fail("tracking table");
    for (int t = 0; t < 15; ++t) {
        bool expectExists = t < 3;
        if (g_model.AllocatorExists[t] != expectExists) return Fail("AllocatorExists");
        if ((g_model.AllocSizeToAllocator[t] != 0) != expectExists) return Fail("lookup table presence");
    }
    if (g_model.MaxFixedAllocationSizes[0] != 32 || g_model.MaxFixedAllocationSizes[1] != 0 ||
        g_model.MaxFixedAllocationSizes[2] != 12) return Fail("MaxFixedAllocationSizes");
    if (g_mallocs != 3) return Fail("malloc count");
    if (g_AllocatorInfo[4].PAllocator != 0 || g_AllocatorInfo[7].PAllocator != 0) return Fail("unused record got an allocator");
    if (g_AllocatorInfo[6].MaxAllocSize != 12 || g_AllocatorInfo[0].MaxAllocSize != 8) return Fail("fixed MaxAllocSize update");
    if (g_AllocatorInfo[3].MaxAllocSize != 0x7fffffff || g_AllocatorInfo[5].MaxAllocSize != 5) return Fail("variable MaxAllocSize kept");
    for (int r = 0; r < 7; ++r) {
        if (r == 4) continue;
        CMemoryAllocatorBase* p = g_AllocatorInfo[r].PAllocator;
        if (!p || p->SeenHeap != heap || p->Fixed != (r != 3 && r != 5)) return Fail("sub-allocator kind/heap");
    }
    // type 0: sizes 0..8 -> record 0, 9..16 -> record 1, 17..32 -> record 2, 33 -> record 3 (variable)
    for (long size = 0; size <= 33; ++size) {
        int expect = size <= 8 ? 0 : size <= 16 ? 1 : size <= 32 ? 2 : 3;
        const CAllocSizeToAllocator& e = g_model.AllocSizeToAllocator[0][size];
        if (e.ID != expect || e.PAllocator != g_AllocatorInfo[expect].PAllocator) return Fail("type 0 lookup");
    }
    // type 1: no fixed allocator -> two entries, both the variable record 5
    for (long size1 = 0; size1 <= 1; ++size1) {
        const CAllocSizeToAllocator& e = g_model.AllocSizeToAllocator[1][size1];
        if (e.ID != 5 || e.PAllocator != g_AllocatorInfo[5].PAllocator) return Fail("type 1 lookup");
    }
    // type 2: record 6 covers [record 5's max + 1, 12] = 6..12; sizes 0..5 and 13 find nothing and stay unwritten
    for (long size2 = 0; size2 <= 13; ++size2) {
        const CAllocSizeToAllocator& e = g_model.AllocSizeToAllocator[2][size2];
        if (size2 >= 6 && size2 <= 12) {
            if (e.ID != 6 || e.PAllocator != g_AllocatorInfo[6].PAllocator) return Fail("type 2 lookup");
        } else if (!Unwritten(e)) {
            return Fail("type 2 entry written without a match");
        }
    }

    // Scenario 2: a fixed size so large that the table's byte count leaves [0, 0x7fffffff]: the constructor returns at
    // once -- no table, MemoryAuditBuffer untouched, lock not released.
    ResetTable();
    SetInfo(0, 0x10000000, 8, 0, true);     // (0x10000000 + 2) * 8 = 0x80000010
    SetInfo(1, 8, 8, 1, true);
    std::memset(&g_model, 0xCC, sizeof(g_model));
    g_model.MemoryAuditBuffer = auditSentinel;
    g_model.Construct(heap);
    if (g_enterLock != 1 || g_leaveLock != 0) return Fail("early return must keep the lock");
    if (g_mallocs != 0 || g_model.AllocSizeToAllocator[0] != 0 || g_model.AllocSizeToAllocator[1] != 0) return Fail("early return tables");
    if (g_model.MemoryAuditBuffer != auditSentinel) return Fail("early return MemoryAuditBuffer");
    if (!g_model.AllocatorExists[0] || !g_model.AllocatorExists[1] || g_model.MaxFixedAllocationSizes[0] != 0x10000000)
        return Fail("early return loop-1 state");

    // Scenario 3: the largest size that still passes the range check is accepted (no early return).
    ResetTable();
    SetInfo(0, 0x0ffffffd, 1, 0, true);     // (0x0ffffffd + 2) * 8 = 0x7ffffff8 <= 0x7fffffff
    {
        __int64 bytes = ((__int64)0x0ffffffd + 2) * sizeof(CAllocSizeToAllocator);
        if (bytes < 0 || bytes > 0x7fffffff) return Fail("boundary arithmetic");
        __int64 over = ((__int64)0x0ffffffe + 2) * sizeof(CAllocSizeToAllocator);
        if (!(over < 0 || over > 0x7fffffff)) return Fail("boundary arithmetic (over)");
    }

    std::printf("009f20b0_TEST PASS\n");
    return 0;
}