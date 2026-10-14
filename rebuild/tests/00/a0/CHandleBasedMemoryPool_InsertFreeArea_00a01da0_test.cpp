#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A01DA0: exercises the same logic through a copy of the model (compiled separately from the source)
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
static bool Run()
{
    CHandleBasedMemoryPool pool; Init(pool); CHeader a, b, c;
    MakeFree(a, 1000, 24); pool.InsertFreeArea(&a);      // 1024 -> bucket 2
    if (a.Size != 1024 || a.Padding != 0 || pool.FreeLists[2] != &a || a.PNextFree != 0 || a.PPrevFreeLink != &pool.FreeLists[2] || pool.LargestFreeArea != 1024) return false;
    MakeFree(b, 1100, 0); pool.InsertFreeArea(&b);       // same bucket: linked in front of a
    if (pool.FreeLists[2] != &b || b.PNextFree != &a || a.PPrevFreeLink != &b.PNextFree || pool.LargestFreeArea != 1100) return false;
    MakeFree(c, 200000, 0); pool.InsertFreeArea(&c);     // huge: the last bucket
    return pool.FreeLists[255] == &c && pool.LargestFreeArea == 200000;
}
int main()
{
    if (!Run()) { std::printf("00a01da0_TEST FAIL\n"); return 1; }
    std::printf("00a01da0_TEST PASS\n");
    return 0;
}