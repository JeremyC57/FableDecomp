#include <cstdio>

struct CTCExplodingObject {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCExplodingObject* g_last_dtor = 0;

void __fastcall CTCExplodingObject_dtor(CTCExplodingObject* explodingObject)
{
    g_last_dtor = explodingObject;
    explodingObject->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCExplodingObject_scalar_deleting_destructor(CTCExplodingObject* explodingObject, int edx_unused, unsigned int flags)
{
    CTCExplodingObject_dtor(explodingObject);
    if (flags & 1)
        operator delete(explodingObject);
    return explodingObject;
}

int main()
{
    CTCExplodingObject a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCExplodingObject_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCExplodingObject b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCExplodingObject_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007512a0_TEST PASS\n");
    return 0;
}