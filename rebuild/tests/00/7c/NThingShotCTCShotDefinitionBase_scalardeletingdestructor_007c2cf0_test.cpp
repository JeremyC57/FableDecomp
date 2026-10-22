#include <cstdio>

struct CTCShotDefinitionBase {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCShotDefinitionBase* g_last_dtor = 0;

void __fastcall CTCShotDefinitionBase_dtor(CTCShotDefinitionBase* definition)
{
    g_last_dtor = definition;
    definition->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCShotDefinitionBase_scalar_deleting_destructor(CTCShotDefinitionBase* definition, int edx_unused, unsigned int flags)
{
    CTCShotDefinitionBase_dtor(definition);
    if (flags & 1)
        operator delete(definition);
    return definition;
}

int main()
{
    CTCShotDefinitionBase a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCShotDefinitionBase_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCShotDefinitionBase b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCShotDefinitionBase_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}