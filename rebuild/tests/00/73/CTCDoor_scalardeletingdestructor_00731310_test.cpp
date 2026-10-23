#include <cstdio>

struct CTCDoor {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCDoor* g_last_dtor = 0;

void __fastcall CTCDoor_dtor(CTCDoor* door)
{
    g_last_dtor = door;
    door->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCDoor_scalar_deleting_destructor(CTCDoor* door, int edx_unused, unsigned int flags)
{
    CTCDoor_dtor(door);
    if (flags & 1)
        operator delete(door);
    return door;
}

int main()
{
    CTCDoor a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCDoor_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCDoor b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCDoor_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00731310_TEST PASS\n");
    return 0;
}