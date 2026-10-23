#include <cstdio>

struct CActiveSoundInfo {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CActiveSoundInfo* g_last_dtor = 0;

void __fastcall CActiveSoundInfo_dtor(CActiveSoundInfo* info)
{
    g_last_dtor = info;
    info->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CActiveSoundInfo_scalar_deleting_destructor(CActiveSoundInfo* info, int edx_unused, unsigned int flags)
{
    CActiveSoundInfo_dtor(info);
    if (flags & 1)
        operator delete(info);
    return info;
}

int main()
{
    CActiveSoundInfo a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CActiveSoundInfo_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CActiveSoundInfo b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CActiveSoundInfo_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00aac7d0_TEST PASS\n");
    return 0;
}