#include <cstdio>

struct CGraphicDataBank {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CGraphicDataBank* g_last_dtor = 0;

void __fastcall CGraphicDataBank_dtor(CGraphicDataBank* bank)
{
    g_last_dtor = bank;
    bank->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CGraphicDataBank_scalar_deleting_destructor(CGraphicDataBank* bank, int edx_unused, unsigned int flags)
{
    CGraphicDataBank_dtor(bank);
    if (flags & 1)
        operator delete(bank);
    return bank;
}

int main()
{
    CGraphicDataBank a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CGraphicDataBank_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CGraphicDataBank b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CGraphicDataBank_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}