// behaviour test for 0x00C28290 CHandleBasedMemoryPool::ResizeArea: split off a free tail, or keep a small spare as padding
#include <cstdio>
#include <string.h>
#include <stdlib.h>

struct CHeader {
    void*          Resource;
    long           Handle;
    long           Size;
    short          Padding;
    unsigned char  Status;
    unsigned char  Locked;
    CHeader**      RefAvailable;
    CHeader*       NextAvailable;
    CHeader*       PrevContiguous;
    CHeader*       NextContiguous;
};

static int g_inserted = 0, g_removed = 0, g_released = 0, g_moves = 0;
static CHeader* g_lastInserted = 0;
static unsigned long g_lastMoveSize = 0;

struct CTestPool {
    void*    Buffer;
    void*    UnalignedBuffer;
    long     PoolSize;
    long     AllocatedSize;
    long     MaxBlocksToTryOnPartialDefrag;
    CHeader* FreeLists[256];
    CHeader* HandleTable[16];
    void RemoveFreeArea(CHeader* area) { ++g_removed; area->Status = 0; }
    void InsertFreeArea(CHeader* area) { ++g_inserted; g_lastInserted = area; }
    void ReleaseBuffer() { ++g_released; }
    void MoveResource(void* resource, long, void* from, void* to, unsigned long size) { ++g_moves; g_lastMoveSize = size; memmove(to, from, size); (void)resource; }
    CHeader* CreateArea(void* address, unsigned long size, CHeader* prev, CHeader* next);
    CHeader* CombineAreas(CHeader* first, CHeader* second);
    void ClearFreeLists();
    void Initialise(long poolSize, long maxBlocksToTry);
    void ResizeArea(CHeader* area, long newSize);
    void MoveArea(CHeader* from, CHeader* to);
};

static void* PoolAlloc(unsigned long size) { return malloc(size); }

CHeader* CTestPool::CreateArea(void* address, unsigned long size, CHeader* prev, CHeader* next)
{
    CHeader* area = (CHeader*)address;
    area->Size = size - sizeof(CHeader);
    area->Padding = 0;
    area->Status = 'N';
    area->Locked = 0;
    area->PrevContiguous = prev;
    area->NextContiguous = next;
    area->Resource = 0;
    if (prev)
        prev->NextContiguous = area;
    if (next)
        next->PrevContiguous = area;
    return area;
}

CHeader* CTestPool::CombineAreas(CHeader* first, CHeader* second)
{
    RemoveFreeArea(first);
    RemoveFreeArea(second);
    CHeader* next = second->NextContiguous;
    first->NextContiguous = next;
    if (next)
        next->PrevContiguous = first;
    first->Size += second->Size + sizeof(CHeader);
    first->Status = 'N';
    InsertFreeArea(first);
    return first;
}

void CTestPool::ClearFreeLists()
{
    memset(FreeLists, 0, sizeof(FreeLists));
}

void CTestPool::Initialise(long poolSize, long maxBlocksToTry)
{
    ReleaseBuffer();
    PoolSize = poolSize;
    MaxBlocksToTryOnPartialDefrag = maxBlocksToTry;
    UnalignedBuffer = PoolAlloc(poolSize + 16);
    Buffer = (void*)(((size_t)UnalignedBuffer + 15) / 16 * 16);
    InsertFreeArea(CreateArea(Buffer, PoolSize, 0, 0));
}

void CTestPool::ResizeArea(CHeader* area, long newSize)
{
    long spare = area->Size - newSize;
    if (spare > (long)sizeof(CHeader)) {
        InsertFreeArea(CreateArea((char*)area + newSize + sizeof(CHeader), spare, area, area->NextContiguous));
        area->Size = newSize;
    } else {
        area->Size = newSize;
        area->Padding = (short)spare;
    }
}

void CTestPool::MoveArea(CHeader* from, CHeader* to)
{
    long size = from->Size;
    MoveResource(from->Resource, from->Handle, (char*)from + sizeof(CHeader), (char*)to + sizeof(CHeader), size);
    RemoveFreeArea(to);
    ResizeArea(to, size);
    to->Resource = from->Resource;
    to->Status = from->Status;
    to->Handle = from->Handle;
    to->Locked = from->Locked;
    HandleTable[from->Handle] = to;
    AllocatedSize += to->Padding - from->Padding;
    from->Status = 'N';
    InsertFreeArea(from);
}

static unsigned char g_arena[4096];

int main()
{
    CTestPool pool;
    CHeader* a = pool.CreateArea(g_arena, 1024, 0, 0);
    CHeader* z = pool.CreateArea(g_arena + 1024, 512, a, 0);
    long before = a->Size;
    pool.ResizeArea(a, 256);
    CHeader* tail = (CHeader*)((char*)a + 256 + sizeof(CHeader));
    bool ok = a->Size == 256 && g_inserted == 1 && g_lastInserted == tail && tail->Size == (long)(before - 256 - sizeof(CHeader))
           && a->NextContiguous == tail && tail->PrevContiguous == a && tail->NextContiguous == z && z->PrevContiguous == tail;
    pool.ResizeArea(a, 256 - 16);                          // 16 spare: padding only
    ok = ok && a->Size == 240 && a->Padding == 16 && g_inserted == 1;
    if (!ok) { std::printf("00c28290_TEST FAIL\n"); return 1; }
    std::printf("00c28290_TEST PASS\n");
    return 0;
}