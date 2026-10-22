#include <cstdio>

struct CEnginePrimitiveOcclusionInfo {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEnginePrimitiveOcclusionInfo* g_last_dtor = 0;

void __fastcall CEnginePrimitiveOcclusionInfo_dtor(CEnginePrimitiveOcclusionInfo* info)
{
    g_last_dtor = info;
    info->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEnginePrimitiveOcclusionInfo_scalar_deleting_destructor(CEnginePrimitiveOcclusionInfo* info, int edx_unused, unsigned int flags)
{
    CEnginePrimitiveOcclusionInfo_dtor(info);
    if (flags & 1)
        operator delete(info);
    return info;
}

int main()
{
    CEnginePrimitiveOcclusionInfo a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEnginePrimitiveOcclusionInfo_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEnginePrimitiveOcclusionInfo b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEnginePrimitiveOcclusionInfo_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}