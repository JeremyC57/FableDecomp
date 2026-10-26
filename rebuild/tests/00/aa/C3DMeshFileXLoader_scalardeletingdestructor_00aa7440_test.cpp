#include <cstdio>

class C3DMeshFileXLoader { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DMeshFileXLoader* g_last_dtor = 0;

void __fastcall C3DMeshFileXLoader_dtor(C3DMeshFileXLoader* loader)
{
    g_last_dtor = loader;
    loader->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DMeshFileXLoader_scalar_deleting_destructor(C3DMeshFileXLoader* loader, int edx_unused, unsigned int flags)
{
    C3DMeshFileXLoader_dtor(loader);
    if (flags & 1)
        operator delete(loader);
    return loader;
}

int main()
{
    C3DMeshFileXLoader a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DMeshFileXLoader_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DMeshFileXLoader b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DMeshFileXLoader_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00aa7440_TEST PASS\n");
    return 0;
}