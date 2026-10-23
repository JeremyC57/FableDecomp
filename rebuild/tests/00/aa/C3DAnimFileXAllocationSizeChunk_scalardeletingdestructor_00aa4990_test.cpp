#include <cstdio>

struct C3DAnimFileXAllocationSizeChunk {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DAnimFileXAllocationSizeChunk* g_last_dtor = 0;

void __fastcall C3DAnimFileXAllocationSizeChunk_dtor(C3DAnimFileXAllocationSizeChunk* chunk)
{
    g_last_dtor = chunk;
    chunk->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DAnimFileXAllocationSizeChunk_scalar_deleting_destructor(C3DAnimFileXAllocationSizeChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DAnimFileXAllocationSizeChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}

int main()
{
    C3DAnimFileXAllocationSizeChunk a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DAnimFileXAllocationSizeChunk_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DAnimFileXAllocationSizeChunk b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DAnimFileXAllocationSizeChunk_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00aa4990_TEST PASS\n");
    return 0;
}