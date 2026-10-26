#include <cstdio>

class CTCBulletTime { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCBulletTime* g_last_dtor = 0;

void __fastcall CTCBulletTime_dtor(CTCBulletTime* time)
{
    g_last_dtor = time;
    time->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCBulletTime_scalar_deleting_destructor(CTCBulletTime* time, int edx_unused, unsigned int flags)
{
    CTCBulletTime_dtor(time);
    if (flags & 1)
        operator delete(time);
    return time;
}

int main()
{
    CTCBulletTime a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCBulletTime_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCBulletTime b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCBulletTime_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0079dc90_TEST PASS\n");
    return 0;
}