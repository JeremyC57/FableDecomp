// Behaviour test for 0x009F2920 CMemoryAllocator::InternalDeallocate: per-ID total reduced, area filled with 0xCD, handed back, count decremented.
#include <cstdio>
#include <cstring>
static unsigned char* g_pRegionStart = 0;
static unsigned char* g_pRegionEnd = 0;
struct Sub { void* freed; unsigned int GetAllocationSize(void*) { return 8; } void Deallocate(void* a) { freed = a; } };
struct Info { Sub* PAllocator; };
static Sub g_sub;
static Info g_info[1] = { { &g_sub } };
struct SDebugInfo { int Guard; unsigned int TrackingID; int Size; };
static unsigned char GetAllocatorID(void* block)
{
    if ((unsigned char*)block >= g_pRegionStart && (unsigned char*)block < g_pRegionEnd)
        return (unsigned char)((unsigned long)((unsigned char*)block - g_pRegionStart) >> 24);
    return ((unsigned char*)block)[-5];
}
struct M {
    unsigned int MemoryAllocatedPerTrackingID[4]; unsigned int NoAllocations, NoAllocators; unsigned int blockSize;
    unsigned int GetAllocationSize(void*) { return blockSize; }
    SDebugInfo* GetDebugInfo(void* block) { GetAllocationSize(block); return (SDebugInfo*)((unsigned char*)block + GetAllocationSize(block) - 0x14); }
    void InternalDeallocate(void* block)
    {
        void* area = (unsigned char*)block - 4;
        unsigned int id = GetAllocatorID(block);
        if (id < NoAllocators) {
            Sub* allocator = g_info[id].PAllocator;
            SDebugInfo* info = GetDebugInfo(block);
            MemoryAllocatedPerTrackingID[info->TrackingID] -= info->Size;
            std::memset(area, 0xcd, allocator->GetAllocationSize(area));
            allocator->Deallocate(area);
            --NoAllocations;
        }
    }
};
int main()
{
    static unsigned int storage[16];
    unsigned char* raw = (unsigned char*)storage;
    unsigned char* block = raw + 8;
    raw[3] = 0;                                    // ID byte at block - 5
    M m; m.NoAllocators = 1; m.NoAllocations = 5; m.blockSize = 0x20; m.MemoryAllocatedPerTrackingID[2] = 100;
    SDebugInfo* info = (SDebugInfo*)(block + 0x20 - 0x14);
    info->Guard = -1; info->TrackingID = 2; info->Size = 30;
    m.InternalDeallocate(block);
    int bad = !(m.MemoryAllocatedPerTrackingID[2] == 70 && m.NoAllocations == 4 && g_sub.freed == raw + 4 && raw[4] == 0xcd && raw[11] == 0xcd);
    raw[3] = 3;                                    // unknown ID: nothing happens
    m.InternalDeallocate(block);
    bad |= !(m.NoAllocations == 4);
    if (bad) { std::printf("009f2920_TEST FAIL\n"); return 1; }
    std::printf("009f2920_TEST PASS\n");
    return 0;
}