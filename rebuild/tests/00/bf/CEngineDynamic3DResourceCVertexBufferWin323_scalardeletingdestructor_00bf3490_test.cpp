#include <cstdio>

struct CEngineDynamic3DResource {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CEngineDynamic3DResource* g_last_dtor = 0;

void __fastcall CEngineDynamic3DResource_dtor(CEngineDynamic3DResource* resource)
{
    g_last_dtor = resource;
    resource->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CEngineDynamic3DResource_scalar_deleting_destructor(CEngineDynamic3DResource* resource, int edx_unused, unsigned int flags)
{
    CEngineDynamic3DResource_dtor(resource);
    if (flags & 1)
        operator delete(resource);
    return resource;
}

int main()
{
    CEngineDynamic3DResource a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CEngineDynamic3DResource_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CEngineDynamic3DResource b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CEngineDynamic3DResource_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00bf3490_TEST PASS\n");
    return 0;
}