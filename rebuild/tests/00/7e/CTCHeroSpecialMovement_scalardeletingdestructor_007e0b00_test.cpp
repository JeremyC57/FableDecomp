#include <cstdio>

struct CTCHeroSpecialMovement {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHeroSpecialMovement* g_last_dtor = 0;

void __fastcall CTCHeroSpecialMovement_dtor(CTCHeroSpecialMovement* movement)
{
    g_last_dtor = movement;
    movement->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHeroSpecialMovement_scalar_deleting_destructor(CTCHeroSpecialMovement* movement, int edx_unused, unsigned int flags)
{
    CTCHeroSpecialMovement_dtor(movement);
    if (flags & 1)
        operator delete(movement);
    return movement;
}

int main()
{
    CTCHeroSpecialMovement a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHeroSpecialMovement_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHeroSpecialMovement b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHeroSpecialMovement_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}