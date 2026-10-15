// Behaviour test for 0x00A65F40 CMemoryAllocatorFixedSize::InternalAlloc: slack added, overflow rejected, heap failure gives 0.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif
static unsigned int g_lastSize = 0; static int g_fail = 0; static char g_block[64];
static void* HeapAllocStub(unsigned int size) { g_lastSize = size; return g_fail ? 0 : g_block; }
static unsigned int HeapSizeStub(const void*) { return g_lastSize + 3; }
struct A {
    unsigned int Alignment;
    unsigned int InternalAlloc(unsigned int size, void** out)
    {
        unsigned int extra = (Alignment - 1) & ~7u;
        U64 total = (U64)extra + size;
        if (total <= (U64)0xFFFFFFFFu) {
            *out = HeapAllocStub(extra + size);
            if (*out) return HeapSizeStub(*out);
        }
        return 0;
    }
};
int main()
{
    A a; a.Alignment = 32; void* p = 0;
    int bad = !(a.InternalAlloc(100, &p) == 124 + 3 && g_lastSize == 124 && p == g_block);
    p = (void*)1; g_lastSize = 0;
    bad |= !(a.InternalAlloc(0xFFFFFFF0u, &p) == 0 && g_lastSize == 0 && p == (void*)1);
    g_fail = 1;
    bad |= !(a.InternalAlloc(8, &p) == 0 && p == 0);
    if (bad) { std::printf("00a65f40_TEST FAIL\n"); return 1; }
    std::printf("00a65f40_TEST PASS\n");
    return 0;
}
