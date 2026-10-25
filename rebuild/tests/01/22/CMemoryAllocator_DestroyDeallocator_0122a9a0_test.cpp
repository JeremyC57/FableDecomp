// Behaviour test for 0x0122A9A0 the atexit destructor of CMemoryAllocator::Deallocator: with a live allocator, flag
// shutdown, and destroy it (dtor, page release, pointer cleared) only when no sub-allocators remain
#include <stdio.h>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#endif
static int gAllocatorDtors = 0;
static int gReleases = 0;
static void* gReleased = 0;
class CMemoryAllocator {
public:
    ~CMemoryAllocator();
    long PeekNoAllocators() const { return NoAllocators; }
    class CDeallocator {
    public:
        ~CDeallocator();
    };
    static void __fastcall Destroy();
    static void __fastcall operator delete(void* p) { gReleased = p; ++gReleases; ::operator delete(p); }   // stand-in for VirtualFree(p, 0, MEM_RELEASE)
    long NoAllocators;
    static CMemoryAllocator* PAllocator;
    static bool ShuttingDown;
};
CMemoryAllocator* CMemoryAllocator::PAllocator = 0;
bool CMemoryAllocator::ShuttingDown = false;
CMemoryAllocator::~CMemoryAllocator() { ++gAllocatorDtors; }
void __fastcall CMemoryAllocator::Destroy()
{
    delete PAllocator;
    PAllocator = 0;
}
CMemoryAllocator::CDeallocator::~CDeallocator()
{
    if (PAllocator) {
        ShuttingDown = true;
        if (PAllocator->PeekNoAllocators() == 0)
            Destroy();
    }
}
int main()
{
    bool ok = true;
    { CMemoryAllocator::CDeallocator d; }                 // no allocator: nothing happens
    ok = ok && !CMemoryAllocator::ShuttingDown && gReleases == 0;
    CMemoryAllocator* a = new CMemoryAllocator;
    a->NoAllocators = 2;
    CMemoryAllocator::PAllocator = a;
    { CMemoryAllocator::CDeallocator d; }                 // sub-allocators alive: flag only
    ok = ok && CMemoryAllocator::ShuttingDown && gReleases == 0 && gAllocatorDtors == 0;
    ok = ok && CMemoryAllocator::PAllocator == a;
    CMemoryAllocator::ShuttingDown = false;
    a->NoAllocators = 0;
    { CMemoryAllocator::CDeallocator d; }                 // last one: destroy
    ok = ok && CMemoryAllocator::ShuttingDown && gAllocatorDtors == 1 && gReleases == 1;
    ok = ok && gReleased == (void*)a && CMemoryAllocator::PAllocator == 0;
    if (ok) { printf("0122a9a0_TEST PASS\n"); return 0; }
    printf("0122a9a0_TEST FAIL\n");
    return 1;
}