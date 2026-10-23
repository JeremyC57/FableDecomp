#include <cstdio>

struct CTCCameraPointScripted {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCCameraPointScripted* g_last_dtor = 0;

void __fastcall CTCCameraPointScripted_dtor(CTCCameraPointScripted* scripted)
{
    g_last_dtor = scripted;
    scripted->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCCameraPointScripted_scalar_deleting_destructor(CTCCameraPointScripted* scripted, int edx_unused, unsigned int flags)
{
    CTCCameraPointScripted_dtor(scripted);
    if (flags & 1)
        operator delete(scripted);
    return scripted;
}

int main()
{
    CTCCameraPointScripted a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCCameraPointScripted_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCCameraPointScripted b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCCameraPointScripted_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0075f290_TEST PASS\n");
    return 0;
}