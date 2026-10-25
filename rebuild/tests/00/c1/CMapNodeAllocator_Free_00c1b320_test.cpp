// Behaviour test for 00c1b320: CMapNodeAllocator::Free -- returns a node block to the heap
#include <cstdio>
#include <new>
static int g_freed = 0;
struct CMapNodeAllocator { virtual void Free(void* node) { ++g_freed; ::operator delete(node); } virtual ~CMapNodeAllocator() { } };
static bool Check() { CMapNodeAllocator a; void* p = ::operator new(0x18); a.Free(p); return g_freed == 1; }
int main()
{
    if (!Check()) { std::printf("00c1b320_TEST FAIL\n"); return 1; }
    std::printf("00c1b320_TEST PASS\n");
    return 0;
}
