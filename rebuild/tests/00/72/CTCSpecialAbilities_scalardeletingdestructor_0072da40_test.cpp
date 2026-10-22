#include <cstdio>

struct CTCSpecialAbilities {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCSpecialAbilities* g_last_dtor = 0;

void __fastcall CTCSpecialAbilities_dtor(CTCSpecialAbilities* abilities)
{
    g_last_dtor = abilities;
    abilities->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCSpecialAbilities_scalar_deleting_destructor(CTCSpecialAbilities* abilities, int edx_unused, unsigned int flags)
{
    CTCSpecialAbilities_dtor(abilities);
    if (flags & 1)
        operator delete(abilities);
    return abilities;
}

int main()
{
    CTCSpecialAbilities a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCSpecialAbilities_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCSpecialAbilities b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCSpecialAbilities_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}