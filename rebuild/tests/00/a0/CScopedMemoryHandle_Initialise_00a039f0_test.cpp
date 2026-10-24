// Behaviour test for 0x00A039F0 CScopedMemoryHandle::Initialise: any handle already held is freed (Clear), then the new pool
// and handle are taken.
#include <cstdio>

static int gFrees = 0;
static long gLastFreed = 0;
static void* gLastPool = 0;

class CHandleBasedMemoryPool { public: void Free(long handle) { ++gFrees; gLastFreed = handle; gLastPool = this; } };

class CScopedMemoryHandle {
public:
    CHandleBasedMemoryPool* Pool;
    long Handle;
    void Clear()
    {
        if (Pool) {
            Pool->Free(Handle);
            Pool = 0;
            Handle = -1;
        }
    }
    void Initialise(CHandleBasedMemoryPool* pool, long handle)
    {
        Clear();
        Pool = pool;
        Handle = handle;
    }
};

int main()
{
    CHandleBasedMemoryPool a, b;
    CScopedMemoryHandle h;
    h.Pool = 0; h.Handle = -1;
    h.Initialise(&a, 3);
    bool ok = gFrees == 0 && h.Pool == &a && h.Handle == 3;
    h.Initialise(&b, 9);
    ok = ok && gFrees == 1 && gLastFreed == 3 && gLastPool == &a && h.Pool == &b && h.Handle == 9;
    h.Initialise(0, -1);
    ok = ok && gFrees == 2 && gLastFreed == 9 && gLastPool == &b && h.Pool == 0 && h.Handle == -1;
    if (!ok) { std::printf("00a039f0_TEST FAIL\n"); return 1; }
    std::printf("00a039f0_TEST PASS\n");
    return 0;
}