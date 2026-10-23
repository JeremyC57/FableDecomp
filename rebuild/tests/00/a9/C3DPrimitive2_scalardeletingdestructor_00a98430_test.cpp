#include <cstdio>

struct C3DPrimitive2 {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DPrimitive2* g_last_dtor = 0;

void __fastcall C3DPrimitive2_dtor(C3DPrimitive2* primitive2)
{
    g_last_dtor = primitive2;
    primitive2->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DPrimitive2_scalar_deleting_destructor(C3DPrimitive2* primitive2, int edx_unused, unsigned int flags)
{
    C3DPrimitive2_dtor(primitive2);
    if (flags & 1)
        operator delete(primitive2);
    return primitive2;
}

int main()
{
    C3DPrimitive2 a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DPrimitive2_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DPrimitive2 b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DPrimitive2_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a98430_TEST PASS\n");
    return 0;
}