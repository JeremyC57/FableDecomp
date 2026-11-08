// Behaviour test for CHandleBasedMemoryPool::Defragment (0x00C28400): a locked pool is left alone; taken areas slide down
// over free ones (resource bytes moved through the owner's MoveResource, handle table updated, header rebuilt), the
// allocated size becomes the packed size and the rest of the pool becomes one free area.
#include <cstdio>
#include <cstring>

struct CHeader {
    void* Resource; long Handle; long Size; short Padding; unsigned char Status; unsigned char Locked;
    CHeader** RefAvailable; CHeader* NextAvailable; CHeader* PrevContiguous; CHeader* NextContiguous;
};
struct Pool {
    void* Buffer; long PoolSize; long AllocatedSize; long LockCount;
    CHeader* Handles[8];
    int moves, clears; CHeader* created; long createdSize; CHeader* createdPrev; CHeader* inserted;
    void MoveResource(void*, long, void* from, void* to, unsigned long size) { std::memmove(to, from, size); ++moves; }
    void ClearFreeLists() { ++clears; }
    CHeader* CreateArea(void* at, unsigned long size, CHeader* prev, CHeader*) { created = (CHeader*)at; createdSize = (long)size; createdPrev = prev; return created; }
    void InsertFreeArea(CHeader* a) { inserted = a; }
    void Defragment()
    {
        if (LockCount > 0) return;
        CHeader* prev = 0;
        unsigned long offset = 0;
        CHeader* area = (CHeader*)Buffer;
        while (area) {
            CHeader* next = area->NextContiguous;
            if (!area->RefAvailable) {
                CHeader* dest = (CHeader*)((char*)Buffer + offset);
                long size = area->Size;
                unsigned char locked = area->Locked;
                if (area != dest || area->Padding != 0) {
                    long handle = area->Handle;
                    void* resource = area->Resource;
                    Handles[handle] = 0;
                    MoveResource(resource, handle, area + 1, dest + 1, size);
                    Handles[handle] = dest;
                    dest->Resource = resource; dest->Handle = handle; dest->Size = size; dest->Padding = 0;
                    dest->Status = 'T'; dest->Locked = locked; dest->RefAvailable = 0; dest->NextAvailable = 0;
                    dest->PrevContiguous = prev; dest->NextContiguous = 0;
                    if (prev) prev->NextContiguous = dest;
                    prev = dest;
                }
                offset += size + sizeof(CHeader);
            }
            area = next;
        }
        AllocatedSize = offset;
        ClearFreeLists();
        long remaining = PoolSize - (long)offset;
        if (remaining > (long)sizeof(CHeader)) {
            CHeader* tail = (CHeader*)((char*)Buffer + offset);
            InsertFreeArea(CreateArea(tail, remaining, prev, 0));
        }
    }
};

int main()
{
    static long long storage[64];
    char* base = (char*)storage;
    const long H = (long)sizeof(CHeader);
    CHeader* free1 = (CHeader*)base;                       // free area of 16 bytes at 0
    CHeader* a = (CHeader*)(base + H + 16);                // taken area, 16 bytes payload
    CHeader* fl = 0;
    std::memset(storage, 0, sizeof storage);
    free1->Size = 16; free1->Status = 'N'; free1->RefAvailable = &fl; free1->NextContiguous = a;
    a->Resource = (void*)0x77; a->Handle = 2; a->Size = 16; a->Status = 'T'; a->Locked = 1; a->PrevContiguous = free1;
    std::memcpy(a + 1, "payload-16bytes", 16);
    Pool p; std::memset(&p, 0, sizeof p);
    p.Buffer = base; p.PoolSize = (long)sizeof storage; p.Handles[2] = a; p.LockCount = 1;
    p.Defragment();
    bool ok = p.moves == 0 && p.clears == 0;
    p.LockCount = 0;
    p.Defragment();
    CHeader* d = (CHeader*)base;
    ok = ok && p.moves == 1 && p.Handles[2] == d && d->Handle == 2 && d->Resource == (void*)0x77 && d->Size == 16
            && d->Status == 'T' && d->Locked == 1 && d->PrevContiguous == 0 && d->NextContiguous == 0
            && std::memcmp(d + 1, "payload-16bytes", 16) == 0;
    ok = ok && p.AllocatedSize == H + 16 && p.clears == 1 && p.created == (CHeader*)(base + H + 16)
            && p.createdSize == p.PoolSize - (H + 16) && p.createdPrev == d && p.inserted == p.created;
    if (!ok) { std::printf("00c28400_TEST FAIL\n"); return 1; }
    std::printf("00c28400_TEST PASS\n");
    return 0;
}