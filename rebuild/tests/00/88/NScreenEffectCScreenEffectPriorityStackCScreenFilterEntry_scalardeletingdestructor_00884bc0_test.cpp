#include <cstdio>

struct CScreenFilterEntry {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CScreenFilterEntry* g_last_dtor = 0;

void __fastcall CScreenFilterEntry_dtor(CScreenFilterEntry* entry)
{
    g_last_dtor = entry;
    entry->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CScreenFilterEntry_scalar_deleting_destructor(CScreenFilterEntry* entry, int edx_unused, unsigned int flags)
{
    CScreenFilterEntry_dtor(entry);
    if (flags & 1)
        operator delete(entry);
    return entry;
}

int main()
{
    CScreenFilterEntry a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CScreenFilterEntry_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CScreenFilterEntry b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CScreenFilterEntry_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00884bc0_TEST PASS\n");
    return 0;
}