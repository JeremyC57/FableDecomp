#include <cstdio>

struct CFrontendGameComponent {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CFrontendGameComponent* g_last_dtor = 0;

void __fastcall CFrontendGameComponent_dtor(CFrontendGameComponent* component)
{
    g_last_dtor = component;
    component->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CFrontendGameComponent_scalar_deleting_destructor(CFrontendGameComponent* component, int edx_unused, unsigned int flags)
{
    CFrontendGameComponent_dtor(component);
    if (flags & 1)
        operator delete(component);
    return component;
}

int main()
{
    CFrontendGameComponent a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CFrontendGameComponent_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CFrontendGameComponent b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CFrontendGameComponent_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}