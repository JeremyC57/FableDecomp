#include <cstdio>

struct CShaderResource {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CShaderResource* g_last_dtor = 0;

void __fastcall CShaderResource_dtor(CShaderResource* resource)
{
    g_last_dtor = resource;
    resource->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CShaderResource_scalar_deleting_destructor(CShaderResource* resource, int edx_unused, unsigned int flags)
{
    CShaderResource_dtor(resource);
    if (flags & 1)
        operator delete(resource);
    return resource;
}

int main()
{
    CShaderResource a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CShaderResource_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CShaderResource b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CShaderResource_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}