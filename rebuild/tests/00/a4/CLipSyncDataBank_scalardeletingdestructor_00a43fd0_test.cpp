#include <cstdio>

class CLipSyncDataBank { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CLipSyncDataBank* g_last_dtor = 0;

void __fastcall CLipSyncDataBank_dtor(CLipSyncDataBank* bank)
{
    g_last_dtor = bank;
    bank->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CLipSyncDataBank_scalar_deleting_destructor(CLipSyncDataBank* bank, int edx_unused, unsigned int flags)
{
    CLipSyncDataBank_dtor(bank);
    if (flags & 1)
        operator delete(bank);
    return bank;
}

int main()
{
    CLipSyncDataBank a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CLipSyncDataBank_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CLipSyncDataBank b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CLipSyncDataBank_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a43fd0_TEST PASS\n");
    return 0;
}