// Behaviour test for CHandleBasedMemoryPool::Defragment @ 0x00A02730 (copy of the model over a real byte buffer).
#include <cstdio>
#include <cstring>
struct CHeader {
    void* Resource; long Handle; long Size; short Padding; unsigned char Status, Locked;
    CHeader** RefAvailable; CHeader* NextAvailable; CHeader* PrevContiguous; CHeader* NextContiguous;
};
union CHandleInfo { CHeader* Header; long NextFree; };
static int g_moves, g_inserted;
class CHandleBasedMemoryPool { public:
    CHeader* Buffer; long PoolSize, AllocatedSize, LockCount; bool MultiThreaded, ShutdownFlag, EnableDefrag;
    CHeader* FreeLists[256]; CHandleInfo* Handles; CHeader* lastFree;
    void BeginCritical(bool) { }
    void EndCritical() { }
    void ClearFreeLists() { std::memset(FreeLists, 0, sizeof(FreeLists)); }
    void InsertFreeArea(CHeader* a) { ++g_inserted; lastFree = a; }
    void MoveResource(void*, long, void* from, void* to, unsigned long size) { std::memmove(to, from, size); ++g_moves; }
    void Defragment()
    {
        if (ShutdownFlag || !EnableDefrag) return;
        BeginCritical(false);
        if (LockCount <= 0) {
            CHeader* prev = 0; unsigned long offset = 0; CHeader* area = Buffer;
            while (area) {
                CHeader* next = area->NextContiguous;
                if (!area->RefAvailable) {
                    CHeader* dest = (CHeader*)((char*)Buffer + offset);
                    long size = area->Size; unsigned char locked = area->Locked;
                    if (area != dest || area->Padding != 0) {
                        long handle = area->Handle; void* resource = area->Resource;
                        Handles[handle].Header = 0;
                        MoveResource(resource, handle, area + 1, dest + 1, size);
                        Handles[handle].Header = dest;
                        dest->Size = size; dest->Resource = resource; dest->Handle = handle; dest->Padding = 0; dest->Status = 'T';
                        dest->Locked = locked; dest->RefAvailable = 0; dest->NextAvailable = 0; dest->PrevContiguous = prev; dest->NextContiguous = 0;
                        if (prev) prev->NextContiguous = dest;
                        prev = dest;
                    }
                    offset += size + sizeof(CHeader);
                }
                area = next;
            }
            AllocatedSize = offset;
            ClearFreeLists();
            unsigned long remaining = PoolSize - offset;
            if (remaining > sizeof(CHeader)) {
                CHeader* freeArea = (CHeader*)((char*)Buffer + offset);
                freeArea->Size = remaining - sizeof(CHeader); freeArea->Padding = 0; freeArea->Status = 'N'; freeArea->Locked = 0;
                freeArea->PrevContiguous = prev; freeArea->NextContiguous = 0; freeArea->Resource = 0;
                if (prev) prev->NextContiguous = freeArea;
                InsertFreeArea(freeArea);
            }
        }
        EndCritical();
    }
};
static double g_mem[128];                                  // 1024 bytes, aligned
static CHeader* at(int off) { return (CHeader*)((char*)g_mem + off); }
int main()
{
    int bad = 0; CHandleInfo handles[4]; CHeader* dummyLink = 0; const int H = (int)sizeof(CHeader);
    std::memset(g_mem, 0, sizeof(g_mem));
    // layout: free (32 data) | taken h1 (16 data, "abcd") | taken h2 (32 data, "wxyz") | free tail
    const int o1 = H + 32, o2 = o1 + H + 16, o3 = o2 + H + 32, pool = o3 + H + 64;
    CHeader* f0 = at(0); CHeader* a1 = at(o1); CHeader* a2 = at(o2); CHeader* f3 = at(o3);
    f0->Size = 32; f0->Status = 'N'; f0->RefAvailable = &dummyLink; f0->NextContiguous = a1;
    a1->Size = 16; a1->Status = 'T'; a1->Handle = 1; a1->Locked = 1; a1->PrevContiguous = f0; a1->NextContiguous = a2; std::memcpy(a1 + 1, "abcd", 5);
    a2->Size = 32; a2->Status = 'T'; a2->Handle = 2; a2->PrevContiguous = a1; a2->NextContiguous = f3; std::memcpy(a2 + 1, "wxyz", 5);
    f3->Size = 64; f3->Status = 'N'; f3->RefAvailable = &dummyLink; f3->PrevContiguous = a2;
    handles[1].Header = a1; handles[2].Header = a2;
    CHandleBasedMemoryPool p; p.Buffer = at(0); p.PoolSize = pool; p.AllocatedSize = 0; p.LockCount = 0; p.MultiThreaded = false;
    p.ShutdownFlag = false; p.EnableDefrag = true; p.Handles = handles; p.lastFree = 0; p.FreeLists[3] = f0;
    p.Defragment();
    CHeader* n1 = at(0); CHeader* n2 = at(H + 16);
    if (g_moves != 2 || handles[1].Header != n1 || handles[2].Header != n2) bad++;
    if (std::strcmp((char*)(n1 + 1), "abcd") || std::strcmp((char*)(n2 + 1), "wxyz") || n1->Locked != 1 || n2->PrevContiguous != n1) bad++;
    if (p.AllocatedSize != 2 * H + 48 || g_inserted != 1 || p.lastFree != at(2 * H + 48) || p.lastFree->Size != pool - (2 * H + 48) - H
        || n2->NextContiguous != at(2 * H + 48) || p.FreeLists[3]) bad++;
    p.LockCount = 1; g_moves = 0; p.Defragment(); if (g_moves) bad++;
    if (bad) { std::printf("00a02730_TEST FAIL\n"); return 1; }
    std::printf("00a02730_TEST PASS\n");
    return 0;
}