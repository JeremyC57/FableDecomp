#include <cstdio>

struct C3DAnimFileHelpersChunk {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DAnimFileHelpersChunk* g_last_dtor = 0;

void __fastcall C3DAnimFileHelpersChunk_dtor(C3DAnimFileHelpersChunk* chunk)
{
    g_last_dtor = chunk;
    chunk->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DAnimFileHelpersChunk_scalar_deleting_destructor(C3DAnimFileHelpersChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DAnimFileHelpersChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}

int main()
{
    C3DAnimFileHelpersChunk a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DAnimFileHelpersChunk_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DAnimFileHelpersChunk b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DAnimFileHelpersChunk_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00aaee70_TEST PASS\n");
    return 0;
}