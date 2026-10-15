// Behaviour test for 0x00A01E40 (CHandleBasedMemoryPool::CombineAreas): two adjacent free areas merge into one free area of the summed size plus a header.
#include <cstdio>
struct CHeader {
    int Handle; int _unused04; int Size; short Padding; char Type; char Flags;
    CHeader** PPrevFreeLink; CHeader* PNextFree; CHeader* PPrevPhysical; CHeader* PNextPhysical;
};
struct Pool {
    int LargestFreeArea;
    CHeader* FreeLists[256];
    Pool() : LargestFreeArea(0) { for (int i = 0; i < 256; ++i) FreeLists[i] = 0; }
    void InsertFreeArea(CHeader* area)
    {
        area->Size += area->Padding; area->Padding = 0;
        unsigned int bucket = (unsigned int)area->Size >> 9; if (bucket >= 255) bucket = 255;
        CHeader** list = &FreeLists[bucket];
        area->PPrevFreeLink = list; CHeader* head = *list; area->PNextFree = head;
        if (head) head->PPrevFreeLink = &area->PNextFree;
        *list = area;
        if (LargestFreeArea < area->Size) LargestFreeArea = area->Size;
    }
    static void RemoveFreeArea(CHeader* area)
    {
        if (area->PPrevFreeLink && area->Type == 'N') {
            area->Type = 0; *area->PPrevFreeLink = area->PNextFree;
            if (area->PNextFree) area->PNextFree->PPrevFreeLink = area->PPrevFreeLink;
            area->PPrevFreeLink = 0; area->PNextFree = 0;
        }
    }
    void CombineAreas(CHeader* first, CHeader* second)
    {
        RemoveFreeArea(first); RemoveFreeArea(second);
        CHeader* next = second->PNextPhysical; first->PNextPhysical = next;
        if (next) next->PPrevPhysical = first;
        first->Size += second->Size + 32; first->Type = 'N';
        InsertFreeArea(first);
    }
};
static void Init(CHeader& h, int size) { h.Handle = 0; h._unused04 = 0; h.Size = size; h.Padding = 0; h.Type = 'T'; h.Flags = 0; h.PPrevFreeLink = 0; h.PNextFree = 0; h.PPrevPhysical = 0; h.PNextPhysical = 0; }
int main()
{
    Pool pool; CHeader a, b, c; Init(a, 512); Init(b, 1024); Init(c, 64);
    a.PNextPhysical = &b; b.PPrevPhysical = &a; b.PNextPhysical = &c; c.PPrevPhysical = &b;
    a.Type = 'N'; pool.InsertFreeArea(&a); b.Type = 'N'; pool.InsertFreeArea(&b);
    pool.CombineAreas(&a, &b);
    int ok = 1;
    if (a.Size != 512 + 1024 + 32 || a.Type != 'N' || a.PNextPhysical != &c || c.PPrevPhysical != &a) ok = 0;
    if (pool.FreeLists[1] != 0 || pool.FreeLists[2] != 0) ok = 0;              // both old buckets emptied
    if (pool.FreeLists[(512 + 1024 + 32) >> 9] != &a || pool.LargestFreeArea != 1568) ok = 0;
    if (b.PPrevFreeLink != 0 || b.PNextFree != 0 || b.Type != 0) ok = 0;
    if (!ok) { std::printf("00a01e40_TEST FAIL\n"); return 1; }
    std::printf("00a01e40_TEST PASS\n");
    return 0;
}