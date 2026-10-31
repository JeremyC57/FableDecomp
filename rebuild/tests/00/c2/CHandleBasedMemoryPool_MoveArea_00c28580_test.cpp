// behaviour test for 0x00C28580 CHandleBasedMemoryPool_012b4098::MoveArea: move the bytes, resize the target, transfer the header, repoint the handle
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
    CTestPool pool; pool.AllocatedSize = 1000;
    CHeader* from = pool.CreateArea(g_arena, 256, 0, 0);
    CHeader* to = pool.CreateArea(g_arena + 1024, 1024, 0, 0);
    from->Size = 100; from->Padding = 4; from->Resource = (void*)g_arena; from->Handle = 3; from->Status = 'T'; from->Locked = 1;
    memset((char*)from + sizeof(CHeader), 0x5A, 100);
    pool.HandleTable[3] = from;
    pool.MoveArea(from, to);
    bool ok = g_moves == 1 && g_lastMoveSize == 100 && ((unsigned char*)to)[sizeof(CHeader) + 99] == 0x5A
           && g_removed == 1 && to->Size == 100 && to->Resource == (void*)g_arena && to->Handle == 3 && to->Status == 'T'
           && to->Locked == 1 && pool.HandleTable[3] == to && pool.AllocatedSize == 1000 + 0 - 4
           && from->Status == 'N' && g_lastInserted == from && g_inserted == 2;
    if (!ok) { std::printf("00c28580_TEST FAIL\n"); return 1; }
    std::printf("00c28580_TEST PASS\n");
    return 0;
}