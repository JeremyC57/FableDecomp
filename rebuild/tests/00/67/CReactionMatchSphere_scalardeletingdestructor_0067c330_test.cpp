#include <cstdio>

struct CReactionMatch_Sphere {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CReactionMatch_Sphere* g_last_dtor = 0;

void __fastcall CReactionMatch_Sphere_dtor(CReactionMatch_Sphere* sphere)
{
    g_last_dtor = sphere;
    sphere->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CReactionMatch_Sphere_scalar_deleting_destructor(CReactionMatch_Sphere* sphere, int edx_unused, unsigned int flags)
{
    CReactionMatch_Sphere_dtor(sphere);
    if (flags & 1)
        operator delete(sphere);
    return sphere;
}

int main()
{
    CReactionMatch_Sphere a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CReactionMatch_Sphere_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CReactionMatch_Sphere b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CReactionMatch_Sphere_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}