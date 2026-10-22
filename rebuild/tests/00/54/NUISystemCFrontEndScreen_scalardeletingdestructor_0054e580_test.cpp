#include <cstdio>

struct CFrontEndScreen {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CFrontEndScreen* g_last_dtor = 0;

void __fastcall CFrontEndScreen_dtor(CFrontEndScreen* screen)
{
    g_last_dtor = screen;
    screen->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CFrontEndScreen_scalar_deleting_destructor(CFrontEndScreen* screen, int edx_unused, unsigned int flags)
{
    CFrontEndScreen_dtor(screen);
    if (flags & 1)
        operator delete(screen);
    return screen;
}

int main()
{
    CFrontEndScreen a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CFrontEndScreen_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CFrontEndScreen b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CFrontEndScreen_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}