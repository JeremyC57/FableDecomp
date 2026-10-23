#include <cstdio>

struct CEngineInternalAttachedPrimitive {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineInternalAttachedPrimitive* g_last_dtor = 0;

void __fastcall CEngineInternalAttachedPrimitive_dtor(CEngineInternalAttachedPrimitive* primitive)
{
    g_last_dtor = primitive;
    primitive->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineInternalAttachedPrimitive_scalar_deleting_destructor(CEngineInternalAttachedPrimitive* primitive, int edx_unused, unsigned int flags)
{
    CEngineInternalAttachedPrimitive_dtor(primitive);
    if (flags & 1)
        operator delete(primitive);
    return primitive;
}

int main()
{
    CEngineInternalAttachedPrimitive a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineInternalAttachedPrimitive_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineInternalAttachedPrimitive b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineInternalAttachedPrimitive_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b3acd0_TEST PASS\n");
    return 0;
}