#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A01F50: exercises the same logic through a copy of the model (compiled separately from the source)
struct CHeader { long Handle; long _unused04; long Size; short Padding; char Type; char Flags; CHeader** PPrevFreeLink; CHeader* PNextFree; CHeader* PPrevPhysical; CHeader* PNextPhysical; };
class CHandleBasedMemoryPool { public:
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
    CHandleBasedMemoryPool pool; Init(pool); CHeader a, b;
    MakeFree(a, 1024, 0); MakeFree(b, 1500, 0); pool.InsertFreeArea(&a); pool.InsertFreeArea(&b);
    if (pool.FindLargestFreeArea() != &b) return false;
    pool.LargestFreeArea = 1024;
    if (pool.FindLargestFreeArea() != &a) return false;
    pool.LargestFreeArea = 1300;                       // nothing of that exact size
    return pool.FindLargestFreeArea() == 0;
}
int main()
{
    if (!Run()) { std::printf("00a01f50_TEST FAIL\n"); return 1; }
    std::printf("00a01f50_TEST PASS\n");
    return 0;
}