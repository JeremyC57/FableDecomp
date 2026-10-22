#include <cstdio>

struct C3DMeshInfo {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DMeshInfo* g_last_dtor = 0;

void __fastcall C3DMeshInfo_dtor(C3DMeshInfo* info)
{
    g_last_dtor = info;
    info->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DMeshInfo_scalar_deleting_destructor(C3DMeshInfo* info, int edx_unused, unsigned int flags)
{
    C3DMeshInfo_dtor(info);
    if (flags & 1)
        operator delete(info);
    return info;
}

int main()
{
    C3DMeshInfo a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DMeshInfo_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DMeshInfo b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DMeshInfo_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}