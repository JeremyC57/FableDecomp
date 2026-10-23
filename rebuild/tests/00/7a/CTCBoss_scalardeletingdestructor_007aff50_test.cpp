#include <cstdio>

struct CTCBoss {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCBoss* g_last_dtor = 0;

void __fastcall CTCBoss_dtor(CTCBoss* boss)
{
    g_last_dtor = boss;
    boss->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCBoss_scalar_deleting_destructor(CTCBoss* boss, int edx_unused, unsigned int flags)
{
    CTCBoss_dtor(boss);
    if (flags & 1)
        operator delete(boss);
    return boss;
}

int main()
{
    CTCBoss a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCBoss_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCBoss b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCBoss_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007aff50_TEST PASS\n");
    return 0;
}