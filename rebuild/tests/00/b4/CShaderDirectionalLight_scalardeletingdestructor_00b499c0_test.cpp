#include <cstdio>

struct CShaderDirectionalLight {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CShaderDirectionalLight* g_last_dtor = 0;

void __fastcall CShaderDirectionalLight_dtor(CShaderDirectionalLight* light)
{
    g_last_dtor = light;
    light->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CShaderDirectionalLight_scalar_deleting_destructor(CShaderDirectionalLight* light, int edx_unused, unsigned int flags)
{
    CShaderDirectionalLight_dtor(light);
    if (flags & 1)
        operator delete(light);
    return light;
}

int main()
{
    CShaderDirectionalLight a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CShaderDirectionalLight_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CShaderDirectionalLight b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CShaderDirectionalLight_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}