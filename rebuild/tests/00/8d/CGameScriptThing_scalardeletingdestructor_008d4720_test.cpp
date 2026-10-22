#include <cstdio>

struct CGameScriptThing {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CGameScriptThing* g_last_dtor = 0;

void __fastcall CGameScriptThing_dtor(CGameScriptThing* thing)
{
    g_last_dtor = thing;
    thing->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CGameScriptThing_scalar_deleting_destructor(CGameScriptThing* thing, int edx_unused, unsigned int flags)
{
    CGameScriptThing_dtor(thing);
    if (flags & 1)
        operator delete(thing);
    return thing;
}

int main()
{
    CGameScriptThing a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CGameScriptThing_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CGameScriptThing b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CGameScriptThing_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}