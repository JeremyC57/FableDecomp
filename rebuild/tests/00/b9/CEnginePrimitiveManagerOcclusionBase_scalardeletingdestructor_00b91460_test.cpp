#include <cstdio>

struct CEnginePrimitiveManagerOcclusionBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveManagerOcclusionBase* g_last_dtor = 0;

void __fastcall CEnginePrimitiveManagerOcclusionBase_dtor(CEnginePrimitiveManagerOcclusionBase* occlusion)
{
    g_last_dtor = occlusion;
    occlusion->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveManagerOcclusionBase_scalar_deleting_destructor(CEnginePrimitiveManagerOcclusionBase* occlusion, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveManagerOcclusionBase_dtor(occlusion);
    if (flags & 1)
        operator delete(occlusion);
    return occlusion;
}

int main()
{
    CEnginePrimitiveManagerOcclusionBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveManagerOcclusionBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveManagerOcclusionBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveManagerOcclusionBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}