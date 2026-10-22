#include <cstdio>

struct CThingCreatureDef {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CThingCreatureDef* g_last_dtor = 0;

void __fastcall CThingCreatureDef_dtor(CThingCreatureDef* def)
{
    g_last_dtor = def;
    def->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CThingCreatureDef_scalar_deleting_destructor(CThingCreatureDef* def, int edx_unused, unsigned int flags)
{
    CThingCreatureDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}

int main()
{
    CThingCreatureDef a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CThingCreatureDef_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CThingCreatureDef b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CThingCreatureDef_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}