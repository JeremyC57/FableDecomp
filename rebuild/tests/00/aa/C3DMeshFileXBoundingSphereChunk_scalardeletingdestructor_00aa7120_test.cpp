#include <cstdio>

struct C3DMeshFileXBoundingSphereChunk {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DMeshFileXBoundingSphereChunk* g_last_dtor = 0;

void __fastcall C3DMeshFileXBoundingSphereChunk_dtor(C3DMeshFileXBoundingSphereChunk* chunk)
{
    g_last_dtor = chunk;
    chunk->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DMeshFileXBoundingSphereChunk_scalar_deleting_destructor(C3DMeshFileXBoundingSphereChunk* chunk, int edx_unused, unsigned int flags)
{
    C3DMeshFileXBoundingSphereChunk_dtor(chunk);
    if (flags & 1)
        operator delete(chunk);
    return chunk;
}

int main()
{
    C3DMeshFileXBoundingSphereChunk a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DMeshFileXBoundingSphereChunk_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DMeshFileXBoundingSphereChunk b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DMeshFileXBoundingSphereChunk_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00aa7120_TEST PASS\n");
    return 0;
}