#include <cstdio>

struct CClass {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CClass* g_last_dtor = 0;

void __fastcall CClass_dtor(CClass* object)
{
    g_last_dtor = object;
    object->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CClass_scalar_deleting_destructor(CClass* object, int edx_unused, unsigned int flags)
{
    CClass_dtor(object);
    if (flags & 1)
        operator delete(object);
    return object;
}

int main()
{
    CClass a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CClass_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CClass b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CClass_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}