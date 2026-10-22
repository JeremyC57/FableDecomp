#include <cstdio>

struct CBankFileAsyncData {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CBankFileAsyncData* g_last_dtor = 0;

void __fastcall CBankFileAsyncData_dtor(CBankFileAsyncData* async)
{
    g_last_dtor = async;
    async->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CBankFileAsyncData_scalar_deleting_destructor(CBankFileAsyncData* async, int edx_unused, unsigned int flags)
{
    CBankFileAsyncData_dtor(async);
    if (flags & 1)
        operator delete(async);
    return async;
}

int main()
{
    CBankFileAsyncData a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CBankFileAsyncData_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CBankFileAsyncData b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CBankFileAsyncData_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}