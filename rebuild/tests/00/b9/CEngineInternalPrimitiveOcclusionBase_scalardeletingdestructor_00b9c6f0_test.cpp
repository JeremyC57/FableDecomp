#include <cstdio>

class CEngineInternalPrimitiveOcclusionBase { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineInternalPrimitiveOcclusionBase* g_last_dtor = 0;

void __fastcall CEngineInternalPrimitiveOcclusionBase_dtor(CEngineInternalPrimitiveOcclusionBase* occlusion)
{
    g_last_dtor = occlusion;
    occlusion->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineInternalPrimitiveOcclusionBase_scalar_deleting_destructor(CEngineInternalPrimitiveOcclusionBase* occlusion, int edx_unused, unsigned int flags)
{
    CEngineInternalPrimitiveOcclusionBase_dtor(occlusion);
    if (flags & 1)
        operator delete(occlusion);
    return occlusion;
}

int main()
{
    CEngineInternalPrimitiveOcclusionBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineInternalPrimitiveOcclusionBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineInternalPrimitiveOcclusionBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineInternalPrimitiveOcclusionBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b9c6f0_TEST PASS\n");
    return 0;
}