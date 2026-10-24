// Behaviour test for 0x009F2BE0 CMemoryAllocator::AllocateAligned: the allocator is created on first use, size + alignment + 3
// bytes are taken under the lock, the result is aligned and the real block is stored in the dword just below it.
#include <cstdio>

static unsigned char gArena[4096];
static unsigned long gOffset = 1;
static int gCreates = 0, gEnter = 0, gLeave = 0;
static unsigned long gLastSize = 0, gLastType = 99;

class CMemoryAllocator {
public:
    void* InternalAllocate(long size, unsigned long type, bool)
    {
        gLastSize = (unsigned long)size; gLastType = type;
        void* p = gArena + gOffset;
        gOffset += (unsigned long)size;
        return p;
    }
    static CMemoryAllocator* Allocator;
    static void Create() { static CMemoryAllocator instance; Allocator = &instance; ++gCreates; }
    static void EnterSynchronisation() { ++gEnter; }
    static void LeaveSynchronisation() { ++gLeave; }
    static void* Allocate(unsigned long size, unsigned long type)
    {
        if (!Allocator)
            Create();
        EnterSynchronisation();
        void* block = Allocator->InternalAllocate(size, type, false);
        LeaveSynchronisation();
        return block;
    }
    static void* AllocateAligned(unsigned long size, long alignment)
    {
        void* block = Allocate(size + alignment + 3, 0);
        void** aligned = (void**)(((size_t)block + alignment + 3) & ~(size_t)(alignment - 1));
        aligned[-1] = block;
        return aligned;
    }
};
CMemoryAllocator* CMemoryAllocator::Allocator = 0;

int main()
{
    bool ok = true;
    for (int i = 0; i < 3; ++i) {
        void* block = gArena + gOffset;
        void* p = CMemoryAllocator::AllocateAligned(100, 16);
        ok = ok && ((size_t)p & 15) == 0 && ((void**)p)[-1] == block && gLastSize == 119 && gLastType == 0
                && (unsigned char*)p >= (unsigned char*)block + 4 && (unsigned char*)p + 100 <= (unsigned char*)block + 119;
    }
    ok = ok && gCreates == 1 && gEnter == 3 && gLeave == 3;
    if (!ok) { std::printf("009f2be0_TEST FAIL\n"); return 1; }
    std::printf("009f2be0_TEST PASS\n");
    return 0;
}