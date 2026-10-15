#include <cstdio>
#include <cstdlib>
static int g_disposal = 0; static bool g_fail = false; static unsigned int g_lastSize = 0;
static void* AllocFromGlobalAllocator(unsigned int size) { g_lastSize = size; return g_fail ? 0 : std::malloc(size); }
struct CIndexBuffer {
    unsigned int Count, Flags; long Format, Pool, Prim;
    CIndexBuffer() : Count(0), Flags(0), Format(0), Pool(0), Prim(0) { }
    void Initialise(unsigned int c, unsigned int f, long fmt, long pool, long prim) { Count = c; Flags = f; Format = fmt; Pool = pool; Prim = prim; }
    static void* operator new(size_t size) throw() { return AllocFromGlobalAllocator((unsigned int)size); }
    static void operator delete(void* p) { std::free(p); }
};
struct CManager {
    void ProcessDisposalLists() { ++g_disposal; }
    CIndexBuffer* AllocateIndexBuffer(long, unsigned int count, unsigned int flags, long format, long prim)
    {
        ProcessDisposalLists();
        CIndexBuffer* buffer = new CIndexBuffer;
        if (!buffer) return 0;
        buffer->Initialise(count, flags, format, 1, prim);
        return buffer;
    }
};
int main()
{
    CManager m;
    CIndexBuffer* b = m.AllocateIndexBuffer(0, 300, 8, 2, 4);
    if (!b || g_disposal != 1 || b->Count != 300 || b->Flags != 8 || b->Format != 2 || b->Pool != 1 || b->Prim != 4) { std::printf("00bda360_TEST FAIL\n"); return 1; }
    delete b;
    g_fail = true;
    if (m.AllocateIndexBuffer(0, 1, 1, 1, 1) != 0 || g_disposal != 2) { std::printf("00bda360_TEST FAIL\n"); return 1; }
    std::printf("00bda360_TEST PASS\n");
    return 0;
}
