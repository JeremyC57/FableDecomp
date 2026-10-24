// Behaviour test for 0x009F2B20 CMemoryAllocator::DeallocateAligned: a null pointer does nothing; otherwise the real block
// (the dword just below p) is deallocated under the lock, and the allocator is destroyed when it empties with DestroyWhenEmpty.
#include <cstdio>
#include <cstdlib>

static int gEnter = 0, gLeave = 0, gFreed = 0;
static void* gLastBlock = 0;

class CMemoryAllocator {
public:
    unsigned long NoAllocations;
    void InternalDeallocate(void* block) { gLastBlock = block; --NoAllocations; }
    static void operator delete(void* p) { ++gFreed; std::free(p); }
    static CMemoryAllocator* Allocator;
    static bool DestroyWhenEmpty;
    static void EnterSynchronisation() { ++gEnter; }
    static void LeaveSynchronisation() { ++gLeave; }
    unsigned long GetNoAllocations() { return NoAllocations; }
    static void Destroy() { delete Allocator; Allocator = 0; }
    static void Deallocate(void* block)
    {
        EnterSynchronisation();
        if (block && Allocator) {
            Allocator->InternalDeallocate(block);
            if (DestroyWhenEmpty && Allocator->GetNoAllocations() == 0)
                Destroy();
        }
        LeaveSynchronisation();
    }
    static void DeallocateAligned(void* p)
    {
        if (p)
            Deallocate(((void**)p)[-1]);
    }
};
CMemoryAllocator* CMemoryAllocator::Allocator = 0;
bool CMemoryAllocator::DestroyWhenEmpty = false;

int main()
{
    CMemoryAllocator* a = (CMemoryAllocator*)std::malloc(sizeof(CMemoryAllocator));
    a->NoAllocations = 2;
    CMemoryAllocator::Allocator = a;
    CMemoryAllocator::DestroyWhenEmpty = true;
    int block1 = 0, block2 = 0;
    void* slots[4];
    slots[0] = &block1; slots[2] = &block2;
    CMemoryAllocator::DeallocateAligned(0);
    bool ok = gEnter == 0 && gLeave == 0;
    CMemoryAllocator::DeallocateAligned(&slots[1]);
    ok = ok && gLastBlock == &block1 && gEnter == 1 && gLeave == 1 && CMemoryAllocator::Allocator == a && gFreed == 0;
    CMemoryAllocator::DeallocateAligned(&slots[3]);
    ok = ok && gLastBlock == &block2 && gEnter == 2 && gLeave == 2 && CMemoryAllocator::Allocator == 0 && gFreed == 1;
    CMemoryAllocator::DeallocateAligned(&slots[1]);            // no allocator left: lock taken, nothing freed
    ok = ok && gEnter == 3 && gLeave == 3 && gFreed == 1;
    if (!ok) { std::printf("009f2b20_TEST FAIL\n"); return 1; }
    std::printf("009f2b20_TEST PASS\n");
    return 0;
}