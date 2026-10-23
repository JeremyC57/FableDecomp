#include <cstdio>

struct CShaderBankEntry {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CShaderBankEntry* g_last_dtor = 0;

void __fastcall CShaderBankEntry_dtor(CShaderBankEntry* entry)
{
    g_last_dtor = entry;
    entry->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CShaderBankEntry_scalar_deleting_destructor(CShaderBankEntry* entry, int edx_unused, unsigned int flags)
{
    CShaderBankEntry_dtor(entry);
    if (flags & 1)
        operator delete(entry);
    return entry;
}

int main()
{
    CShaderBankEntry a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CShaderBankEntry_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CShaderBankEntry b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CShaderBankEntry_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a628b0_TEST PASS\n");
    return 0;
}