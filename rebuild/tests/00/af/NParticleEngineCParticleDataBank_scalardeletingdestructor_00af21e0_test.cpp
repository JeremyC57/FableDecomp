#include <cstdio>

struct CParticleDataBank {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CParticleDataBank* g_last_dtor = 0;

void __fastcall CParticleDataBank_dtor(CParticleDataBank* bank)
{
    g_last_dtor = bank;
    bank->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CParticleDataBank_scalar_deleting_destructor(CParticleDataBank* bank, int edx_unused, unsigned int flags)
{
    CParticleDataBank_dtor(bank);
    if (flags & 1)
        operator delete(bank);
    return bank;
}

int main()
{
    CParticleDataBank a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CParticleDataBank_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CParticleDataBank b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CParticleDataBank_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00af21e0_TEST PASS\n");
    return 0;
}