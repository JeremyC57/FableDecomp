#include <cstdio>

struct CTCTavernTable {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCTavernTable* g_last_dtor = 0;

void __fastcall CTCTavernTable_dtor(CTCTavernTable* table)
{
    g_last_dtor = table;
    table->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCTavernTable_scalar_deleting_destructor(CTCTavernTable* table, int edx_unused, unsigned int flags)
{
    CTCTavernTable_dtor(table);
    if (flags & 1)
        operator delete(table);
    return table;
}

int main()
{
    CTCTavernTable a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCTavernTable_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCTavernTable b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCTavernTable_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007b9d10_TEST PASS\n");
    return 0;
}