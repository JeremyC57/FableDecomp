// Behaviour test for 0x00A03870 CScopedMemoryHandle::Clear: a held handle is freed back to its pool and the pair reset to
// (no pool, -1); an empty handle does nothing.
#include <cstdio>

static int gFrees = 0;
static long gLastFreed = 0;

class CHandleBasedMemoryPool { public: void Free(long handle) { ++gFrees; gLastFreed = handle; } };

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
};

int main()
{
    CHandleBasedMemoryPool pool;
    CScopedMemoryHandle h;
    h.Pool = &pool; h.Handle = 17;
    h.Clear();
    bool ok = gFrees == 1 && gLastFreed == 17 && h.Pool == 0 && h.Handle == -1;
    h.Handle = 5;
    h.Clear();
    ok = ok && gFrees == 1 && h.Handle == 5;
    if (!ok) { std::printf("00a03870_TEST FAIL\n"); return 1; }
    std::printf("00a03870_TEST PASS\n");
    return 0;
}