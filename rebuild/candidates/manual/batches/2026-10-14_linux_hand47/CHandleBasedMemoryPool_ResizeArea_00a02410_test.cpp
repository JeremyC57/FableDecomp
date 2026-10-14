#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A02410: exercises the same logic through a copy of the model (compiled separately from the source)
struct CHeader { long Handle; long _unused04; long Size; short Padding; char Type; char Flags; CHeader** PPrevFreeLink; CHeader* PNextFree; CHeader* PPrevPhysical; CHeader* PNextPhysical; };
struct CHandleBasedMemoryPool {
    long LargestFreeArea; CHeader* FreeLists[256];
    void InsertFreeArea(CHeader* area)
    {
        area->Size += area->Padding;
        area->Padding = 0;
        unsigned long bucket = (unsigned long)area->Size >> 9;
        if (bucket >= 255) bucket = 255;
        CHeader** list = &FreeLists[bucket];
        area->PPrevFreeLink = list;
        CHeader* head = *list;
        area->PNextFree = head;
        if (head) head->PPrevFreeLink = &area->PNextFree;
        *list = area;
        if (LargestFreeArea < area->Size) LargestFreeArea = area->Size;
    }
    CHeader* FindLargestFreeArea()
    {
        long size = LargestFreeArea;
        unsigned long bucket = (unsigned long)size >> 9;
        if (bucket >= 255) bucket = 255;
        for (CHeader* area = FreeLists[bucket]; area; area = area->PNextFree) if (area->Size == size) return area;
        return 0;
    }
    void UpdateLargestFreeArea()
    {
        unsigned long bucket = (unsigned long)LargestFreeArea >> 9;
        if (bucket >= 255) bucket = 255;
        for (long index = (long)bucket; index >= 0; --index) {
            if (FreeLists[index]) {
                long largest = 0;
                for (CHeader* area = FreeLists[index]; area; area = area->PNextFree) if (largest < area->Size) largest = area->Size;
                LargestFreeArea = largest;
                return;
            }
        }
        LargestFreeArea = 0;
    }
};
static void Init(CHandleBasedMemoryPool& pool) { std::memset(&pool, 0, sizeof(pool)); }
static void MakeFree(CHeader& h, long size, short padding) { std::memset(&h, 0, sizeof(h)); h.Size = size; h.Padding = padding; h.Type = 'N'; }
static int g_inserted = 0; static CHeader* g_insertedArea = 0;
struct M : CHandleBasedMemoryPool {
    void InsertFreeAreaCounted(CHeader* area) { ++g_inserted; g_insertedArea = area; InsertFreeArea(area); }
    void ResizeArea(CHeader* area, long newSize)
    {
        unsigned long spare = area->Size - newSize;
        if (spare > 0x20) {
            CHeader* fresh = (CHeader*)((char*)area + newSize + 0x20);
            CHeader* next = area->PNextPhysical;
            fresh->Size = spare - 0x20; fresh->Padding = 0; fresh->Type = 'N'; fresh->Flags = 0; fresh->PPrevPhysical = area; fresh->PNextPhysical = next; fresh->Handle = 0;
            area->PNextPhysical = fresh;
            if (next) next->PPrevPhysical = fresh;
            InsertFreeAreaCounted(fresh);
            area->Size = newSize;
        } else { area->Size = newSize; area->Padding = (short)spare; }
    }
};
static bool Run()
{
    M pool; Init(pool);
    char* block = (char*)std::malloc(4096); std::memset(block, 0, 4096);
    CHeader* area = (CHeader*)block; CHeader* next = (CHeader*)(block + 2048);
    area->Size = 2000; area->Type = 'T'; area->PNextPhysical = next; next->PPrevPhysical = area;
    pool.ResizeArea(area, 1000);                       // 1000 spare: a new free area at 1032
    CHeader* fresh = (CHeader*)(block + 1000 + 0x20);
    if (g_inserted != 1 || g_insertedArea != fresh || fresh->Size != 1000 - 0x20 || fresh->Type != 'N' || fresh->PPrevPhysical != area || fresh->PNextPhysical != next) return false;
    if (area->Size != 1000 || area->PNextPhysical != fresh || next->PPrevPhysical != fresh || pool.LargestFreeArea != 1000 - 0x20) return false;
    pool.ResizeArea(area, 990);                        // 10 spare: kept as padding
    bool ok = g_inserted == 1 && area->Size == 990 && area->Padding == 10;
    std::free(block);
    return ok;
}
int main()
{
    if (!Run()) { std::printf("00a02410_TEST FAIL\n"); return 1; }
    std::printf("00a02410_TEST PASS\n");
    return 0;
}
