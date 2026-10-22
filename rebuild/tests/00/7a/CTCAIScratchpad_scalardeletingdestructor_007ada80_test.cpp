#include <cstdio>

struct CTCAIScratchpad {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCAIScratchpad* g_last_dtor = 0;

void __fastcall CTCAIScratchpad_dtor(CTCAIScratchpad* scratchpad)
{
    g_last_dtor = scratchpad;
    scratchpad->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCAIScratchpad_scalar_deleting_destructor(CTCAIScratchpad* scratchpad, int edx_unused, unsigned int flags)
{
    CTCAIScratchpad_dtor(scratchpad);
    if (flags & 1)
        operator delete(scratchpad);
    return scratchpad;
}

int main()
{
    CTCAIScratchpad a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCAIScratchpad_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCAIScratchpad b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCAIScratchpad_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}