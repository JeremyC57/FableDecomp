#include <cstdio>

struct CGroupEntry {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CGroupEntry* g_last_dtor = 0;

void __fastcall CGroupEntry_dtor(CGroupEntry* entry)
{
    g_last_dtor = entry;
    entry->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CGroupEntry_scalar_deleting_destructor(CGroupEntry* entry, int edx_unused, unsigned int flags)
{
    CGroupEntry_dtor(entry);
    if (flags & 1)
        operator delete(entry);
    return entry;
}

int main()
{
    CGroupEntry a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CGroupEntry_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CGroupEntry b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CGroupEntry_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}