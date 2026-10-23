#include <cstdio>

struct CMessageOnMorphChanged {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CMessageOnMorphChanged* g_last_dtor = 0;

void __fastcall CMessageOnMorphChanged_dtor(CMessageOnMorphChanged* message)
{
    g_last_dtor = message;
    message->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CMessageOnMorphChanged_scalar_deleting_destructor(CMessageOnMorphChanged* message, int edx_unused, unsigned int flags)
{
    CMessageOnMorphChanged_dtor(message);
    if (flags & 1)
        operator delete(message);
    return message;
}

int main()
{
    CMessageOnMorphChanged a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CMessageOnMorphChanged_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CMessageOnMorphChanged b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CMessageOnMorphChanged_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_0071ee50_TEST PASS\n");
    return 0;
}