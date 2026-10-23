#include <cstdio>

struct CVertexShaderConstantLayoutEmpty {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CVertexShaderConstantLayoutEmpty* g_last_dtor = 0;

void __fastcall CVertexShaderConstantLayoutEmpty_dtor(CVertexShaderConstantLayoutEmpty* empty)
{
    g_last_dtor = empty;
    empty->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CVertexShaderConstantLayoutEmpty_scalar_deleting_destructor(CVertexShaderConstantLayoutEmpty* empty, int edx_unused, unsigned int flags)
{
    CVertexShaderConstantLayoutEmpty_dtor(empty);
    if (flags & 1)
        operator delete(empty);
    return empty;
}

int main()
{
    CVertexShaderConstantLayoutEmpty a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CVertexShaderConstantLayoutEmpty_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CVertexShaderConstantLayoutEmpty b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CVertexShaderConstantLayoutEmpty_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_009e1850_TEST PASS\n");
    return 0;
}