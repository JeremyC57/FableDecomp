#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A01F80: exercises the same logic through a copy of the model (compiled separately from the source)
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
    MakeFree(a, 600, 0); MakeFree(b, 900, 0); MakeFree(c, 3000, 0);
    pool.InsertFreeArea(&a); pool.InsertFreeArea(&b); pool.InsertFreeArea(&c);
    pool.FreeLists[5] = 0;                             // the 3000-byte area was taken, LargestFreeArea is stale
    pool.UpdateLargestFreeArea();
    if (pool.LargestFreeArea != 900) return false;
    pool.FreeLists[1] = 0;
    pool.UpdateLargestFreeArea();
    return pool.LargestFreeArea == 0;
}
int main()
{
    if (!Run()) { std::printf("00a01f80_TEST FAIL\n"); return 1; }
    std::printf("00a01f80_TEST PASS\n");
    return 0;
}
