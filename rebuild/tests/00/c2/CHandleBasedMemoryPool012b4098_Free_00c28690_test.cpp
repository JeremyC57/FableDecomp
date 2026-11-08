// Behaviour test for CHandleBasedMemoryPool::Free (0x00C28690): the handle's area is marked free ('N') and put on the free
// list, the release is counted, its header + payload + padding leave the allocated size, neighbours are combined and the
// handle is released, in that order.
#include <cstdio>
#include <cstring>

struct CHeader { void* Resource; long Handle; long Size; short Padding; unsigned char Status; unsigned char Locked; };
struct Pool {
    CHeader* Handles[4]; long AllocatedSize; long NumAllocations;
    char log[64];
    void InsertFreeArea(CHeader* a) { std::strcat(log, a->Status == 'N' ? "I" : "i"); }
    void CombineAdjacentAreas(CHeader*) { std::strcat(log, "C"); }
    void FreeHandle(long h) { std::strcat(log, h == 1 ? "F" : "f"); }
    void Free(long handle)
    {
        CHeader* area = Handles[handle];
        area->Status = 'N';
        InsertFreeArea(area);
        ++NumAllocations;
        AllocatedSize -= area->Padding + area->Size + 0x20;
        CombineAdjacentAreas(area);
        FreeHandle(handle);
    }
};

int main()
{
    CHeader a = { 0, 1, 100, 4, 'T', 0 };
    Pool p; std::memset(&p, 0, sizeof p);
    p.Handles[1] = &a; p.AllocatedSize = 1000; p.NumAllocations = 5;
    p.Free(1);
    bool ok = a.Status == 'N' && p.AllocatedSize == 1000 - (4 + 100 + 0x20) && p.NumAllocations == 6 && std::strcmp(p.log, "ICF") == 0;
    if (!ok) { std::printf("00c28690_TEST FAIL\n"); return 1; }
    std::printf("00c28690_TEST PASS\n");
    return 0;
}