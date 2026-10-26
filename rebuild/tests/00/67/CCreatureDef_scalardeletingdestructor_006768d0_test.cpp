#include <cstdio>

class CCreatureDef { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCreatureDef* g_last_dtor = 0;

void __fastcall CCreatureDef_dtor(CCreatureDef* def)
{
    g_last_dtor = def;
    def->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCreatureDef_scalar_deleting_destructor(CCreatureDef* def, int edx_unused, unsigned int flags)
{
    CCreatureDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}

int main()
{
    CCreatureDef a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCreatureDef_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCreatureDef b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCreatureDef_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_006768d0_TEST PASS\n");
    return 0;
}