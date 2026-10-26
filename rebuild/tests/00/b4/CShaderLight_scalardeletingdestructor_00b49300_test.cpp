#include <cstdio>

class CShaderLight { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CShaderLight* g_last_dtor = 0;

void __fastcall CShaderLight_dtor(CShaderLight* light)
{
    g_last_dtor = light;
    light->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CShaderLight_scalar_deleting_destructor(CShaderLight* light, int edx_unused, unsigned int flags)
{
    CShaderLight_dtor(light);
    if (flags & 1)
        operator delete(light);
    return light;
}

int main()
{
    CShaderLight a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CShaderLight_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CShaderLight b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CShaderLight_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00b49300_TEST PASS\n");
    return 0;
}