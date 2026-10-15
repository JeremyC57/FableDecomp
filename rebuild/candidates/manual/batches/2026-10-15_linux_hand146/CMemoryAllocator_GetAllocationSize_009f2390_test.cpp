// Behaviour test for 0x009F2390 CMemoryAllocator::GetAllocationSize: ID from the region offset or the byte at block-5, size from that sub-allocator.
#include <cstdio>
static unsigned char g_region[64];
static unsigned char* g_pRegionStart = g_region;
static unsigned char* g_pRegionEnd = g_region + sizeof(g_region);
static int g_lockDepth = 0;
struct Sub { unsigned int size; void* lastArea; unsigned int GetAllocationSize(void* area) { lastArea = area; return size; } };
struct Info { Sub* PAllocator; };
static Sub g_subs[2];
static Info g_info[2] = { { &g_subs[0] }, { &g_subs[1] } };
static unsigned char GetAllocatorID(void* block)
{
    if ((unsigned char*)block >= g_pRegionStart && (unsigned char*)block < g_pRegionEnd)
        return (unsigned char)((unsigned long)((unsigned char*)block - g_pRegionStart) >> 24);
    return ((unsigned char*)block)[-5];
}
struct M {
    unsigned int NoAllocators;
    unsigned int GetAllocationSize(void* block)
    {
        ++g_lockDepth;
        if (block == 0) return 0;
        unsigned int id = GetAllocatorID(block);
        if (id >= NoAllocators) return 0;
        unsigned int size = g_info[id].PAllocator->GetAllocationSize((unsigned char*)block - 4);
        --g_lockDepth;
        return size;
    }
};
int main()
{
    static unsigned char outside[16];
    g_subs[0].size = 40; g_subs[1].size = 77;
    M m; m.NoAllocators = 2;
    int bad = !(m.GetAllocationSize(g_region + 8) == 40 && g_subs[0].lastArea == g_region + 4 && g_lockDepth == 0);
    outside[3] = 1;
    bad |= !(m.GetAllocationSize(outside + 8) == 77 && g_subs[1].lastArea == outside + 4);
    outside[3] = 5;
    bad |= !(m.GetAllocationSize(outside + 8) == 0 && m.GetAllocationSize(0) == 0);
    if (bad) { std::printf("009f2390_TEST FAIL\n"); return 1; }
    std::printf("009f2390_TEST PASS\n");
    return 0;
}
