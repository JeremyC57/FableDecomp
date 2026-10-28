// Test: CHandleBasedMemoryPool::Free -- the area is marked free, put on the free list, the counters move (allocation count up, allocated size down by size+padding+header), neighbours are merged and the handle is released, in that order.
#include <cstdio>
#include <cstring>
static char g_seq[8];
struct CHeader { int Resource; int Handle; int Size; short Padding; unsigned char Status; unsigned char Locked; int pad[4]; };
struct CPool { CHeader** Handles; int NumAllocations; int AllocatedSize;
    void InsertFreeArea(CHeader* h) { std::strcat(g_seq, h->Status == 'N' ? "I" : "x"); }
    CHeader* CombineAdjacentAreas(CHeader* h) { std::strcat(g_seq, "C"); return h; }
    void InternalFreeHandle(int) { std::strcat(g_seq, "H"); }
    void Free(int handle) { CHeader* header = Handles[handle]; header->Status = 'N'; InsertFreeArea(header); NumAllocations++;
        AllocatedSize -= header->Size + header->Padding + (int)sizeof(CHeader); CombineAdjacentAreas(header); InternalFreeHandle(handle); } };
int main()
{
    CHeader h; h.Status = 'T'; h.Size = 100; h.Padding = 4; CHeader* table[1] = { &h };
    CPool p; p.Handles = table; p.NumAllocations = 5; p.AllocatedSize = 1000;
    p.Free(0);
    bool ok = sizeof(CHeader) == 32 && h.Status == 'N' && p.NumAllocations == 6 && p.AllocatedSize == 1000 - 136 && !std::strcmp(g_seq, "ICH");
    std::printf(ok ? "00c28690_TEST PASS\n" : "00c28690_TEST FAIL\n");
    return ok ? 0 : 1;
}
