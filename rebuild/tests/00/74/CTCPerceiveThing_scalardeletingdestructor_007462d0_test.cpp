#include <cstdio>

struct CTCPerceiveThing {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCPerceiveThing* g_last_dtor = 0;

void __fastcall CTCPerceiveThing_dtor(CTCPerceiveThing* thing)
{
    g_last_dtor = thing;
    thing->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCPerceiveThing_scalar_deleting_destructor(CTCPerceiveThing* thing, int edx_unused, unsigned int flags)
{
    CTCPerceiveThing_dtor(thing);
    if (flags & 1)
        operator delete(thing);
    return thing;
}

int main()
{
    CTCPerceiveThing a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCPerceiveThing_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCPerceiveThing b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCPerceiveThing_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007462d0_TEST PASS\n");
    return 0;
}