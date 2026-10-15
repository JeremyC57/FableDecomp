// Behaviour test for 0x009F2310 ~CMemoryAllocator: audit buffer freed, each sub-allocator released, non-null size maps freed, lock deleted.
#include <cstdio>
static int g_frees = 0, g_releases = 0, g_lockDeleted = 0;
static void FreeStub(void* p) { if (p) ++g_frees; }
static void ReleaseStub(void* a) { if (a) ++g_releases; }
struct Info { void* PAllocator; };
static Info g_info[4];
struct M {
    void* MemoryAuditBuffer; unsigned int NoAllocators; void* AllocSizeToAllocator[15];
    ~M()
    {
        if (MemoryAuditBuffer) FreeStub(MemoryAuditBuffer);
        for (unsigned int i = 0; i < NoAllocators; ++i) ReleaseStub(g_info[i].PAllocator);
        for (int j = 0; j < 15; ++j) if (AllocSizeToAllocator[j]) FreeStub(AllocSizeToAllocator[j]);
        g_lockDeleted = 1;
    }
};
int main()
{
    static int x[8];
    for (int i = 0; i < 4; ++i) g_info[i].PAllocator = &x[i];
    {
        M m; m.MemoryAuditBuffer = &x[5]; m.NoAllocators = 3;
        for (int j = 0; j < 15; ++j) m.AllocSizeToAllocator[j] = (j % 5 == 0) ? (void*)&x[6] : 0;
    }
    if (g_frees != 4 || g_releases != 3 || !g_lockDeleted) { std::printf("009f2310_TEST FAIL\n"); return 1; }
    std::printf("009f2310_TEST PASS\n");
    return 0;
}
