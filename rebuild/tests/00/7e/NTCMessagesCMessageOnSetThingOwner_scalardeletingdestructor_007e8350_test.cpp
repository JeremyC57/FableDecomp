#include <cstdio>

struct CMessageOnSetThingOwner {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CMessageOnSetThingOwner* g_last_dtor = 0;

void __fastcall CMessageOnSetThingOwner_dtor(CMessageOnSetThingOwner* message)
{
    g_last_dtor = message;
    message->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CMessageOnSetThingOwner_scalar_deleting_destructor(CMessageOnSetThingOwner* message, int edx_unused, unsigned int flags)
{
    CMessageOnSetThingOwner_dtor(message);
    if (flags & 1)
        operator delete(message);
    return message;
}

int main()
{
    CMessageOnSetThingOwner a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CMessageOnSetThingOwner_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CMessageOnSetThingOwner b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CMessageOnSetThingOwner_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007e8350_TEST PASS\n");
    return 0;
}