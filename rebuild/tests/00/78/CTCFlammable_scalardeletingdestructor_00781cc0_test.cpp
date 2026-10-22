#include <cstdio>

struct CTCFlammable {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCFlammable* g_last_dtor = 0;

void __fastcall CTCFlammable_dtor(CTCFlammable* flammable)
{
    g_last_dtor = flammable;
    flammable->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCFlammable_scalar_deleting_destructor(CTCFlammable* flammable, int edx_unused, unsigned int flags)
{
    CTCFlammable_dtor(flammable);
    if (flags & 1)
        operator delete(flammable);
    return flammable;
}

int main()
{
    CTCFlammable a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCFlammable_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCFlammable b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCFlammable_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}