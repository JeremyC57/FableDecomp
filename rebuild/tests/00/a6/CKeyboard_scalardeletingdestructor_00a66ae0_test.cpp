#include <cstdio>

class CKeyboard { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CKeyboard* g_last_dtor = 0;

void __fastcall CKeyboard_dtor(CKeyboard* keyboard)
{
    g_last_dtor = keyboard;
    keyboard->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CKeyboard_scalar_deleting_destructor(CKeyboard* keyboard, int edx_unused, unsigned int flags)
{
    CKeyboard_dtor(keyboard);
    if (flags & 1)
        operator delete(keyboard);
    return keyboard;
}

int main()
{
    CKeyboard a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CKeyboard_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CKeyboard b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CKeyboard_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a66ae0_TEST PASS\n");
    return 0;
}