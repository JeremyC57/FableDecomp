// Behaviour test for 0x009F2870 CMemoryAllocator::Create: the allocator is built in VirtualAlloc'd pages (size, MEM_COMMIT |
// MEM_RESERVE, PAGE_READWRITE) on the process heap, and the physical memory in use (total - available) lands in both globals.
#include <cstdio>
#include <cstdlib>

static unsigned long gAllocSize = 0, gAllocType = 0, gAllocProtect = 0;
static void* gHeap = (void*)0x1234;
static unsigned long gTotal = 0, gAvail = 0;

static void* VirtualAlloc(void*, unsigned long size, unsigned long type, unsigned long protect)
{
    gAllocSize = size; gAllocType = type; gAllocProtect = protect;
    return std::malloc(size);
}
static void* GetProcessHeap() { return gHeap; }
struct SMemoryStatus {
    unsigned long dwLength, dwMemoryLoad, dwTotalPhys, dwAvailPhys;
    unsigned long dwTotalPageFile, dwAvailPageFile, dwTotalVirtual, dwAvailVirtual;
};
static void GlobalMemoryStatus(SMemoryStatus* status) { status->dwTotalPhys = gTotal; status->dwAvailPhys = gAvail; }

static unsigned long g_physicalMemoryInUseAtStart = 0;
static unsigned long g_physicalMemoryInUseAtStartCopy = 0;

class CMemoryAllocator {
public:
    unsigned char _body[0x49c];
    void* m_heap;
    CMemoryAllocator(void* heap) : m_heap(heap) {}
    static void* operator new(size_t size) { return VirtualAlloc(0, (unsigned long)size, 0x3000, 4); }
    static void operator delete(void* p) { std::free(p); }
    static CMemoryAllocator* Allocator;
    static void Create();
};
CMemoryAllocator* CMemoryAllocator::Allocator = 0;

void CMemoryAllocator::Create()
{
    Allocator = new CMemoryAllocator(GetProcessHeap());
    SMemoryStatus status;
    GlobalMemoryStatus(&status);
    unsigned long inUse = status.dwTotalPhys - status.dwAvailPhys;
    g_physicalMemoryInUseAtStart = inUse;
    g_physicalMemoryInUseAtStartCopy = inUse;
}

int main()
{
    gTotal = 1000000; gAvail = 250000;
    CMemoryAllocator::Create();
    bool ok = CMemoryAllocator::Allocator != 0 && CMemoryAllocator::Allocator->m_heap == gHeap
           && gAllocSize == (unsigned long)sizeof(CMemoryAllocator) && gAllocType == 0x3000 && gAllocProtect == 4
           && g_physicalMemoryInUseAtStart == 750000 && g_physicalMemoryInUseAtStartCopy == 750000;
    delete CMemoryAllocator::Allocator;
    if (!ok) { std::printf("009f2870_TEST FAIL\n"); return 1; }
    std::printf("009f2870_TEST PASS\n");
    return 0;
}