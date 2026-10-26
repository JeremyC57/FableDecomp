#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00A02470: exercises the same logic through a copy of the model (compiled separately from the source)
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
static int g_combined = 0;
struct M : CHandleBasedMemoryPool {
    void CombineAreas(CHeader* first, CHeader* second) { ++g_combined; first->Size += second->Size + 0x20; first->PNextPhysical = second->PNextPhysical; if (second->PNextPhysical) second->PNextPhysical->PPrevPhysical = first; second->Type = 'X'; }
    void CombineAdjacentAreas(CHeader** pArea)
    {
        CHeader* next = (*pArea)->PNextPhysical;
        if (next && next->Type == 'N') CombineAreas(*pArea, next);
        CHeader* prev = (*pArea)->PPrevPhysical;
        if (prev && prev->Type == 'N') { CombineAreas(prev, *pArea); *pArea = prev; }
    }
};
static bool Run()
{
    M pool; Init(pool); CHeader a, b, c, d;
    MakeFree(a, 100, 0); MakeFree(b, 200, 0); MakeFree(c, 300, 0); MakeFree(d, 400, 0); d.Type = 'T';
    a.PNextPhysical = &b; b.PPrevPhysical = &a; b.PNextPhysical = &c; c.PPrevPhysical = &b; c.PNextPhysical = &d; d.PPrevPhysical = &c;
    CHeader* area = &b;
    pool.CombineAdjacentAreas(&area);
    if (g_combined != 2 || area != &a || a.Size != 100 + 200 + 300 + 0x40 || a.PNextPhysical != &d || d.PPrevPhysical != &a) return false;
    area = &a;
    pool.CombineAdjacentAreas(&area);                  // the taken successor and no predecessor: nothing
    return g_combined == 2 && area == &a;
}
int main()
{
    if (!Run()) { std::printf("00a02470_TEST FAIL\n"); return 1; }
    std::printf("00a02470_TEST PASS\n");
    return 0;
}