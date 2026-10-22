#include <cstdio>

struct CCombatSequence_WillPhysicalShieldNonMelee {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCombatSequence_WillPhysicalShieldNonMelee* g_last_dtor = 0;

void __fastcall CCombatSequence_WillPhysicalShieldNonMelee_dtor(CCombatSequence_WillPhysicalShieldNonMelee* sequence)
{
    g_last_dtor = sequence;
    sequence->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCombatSequence_WillPhysicalShieldNonMelee_scalar_deleting_destructor(CCombatSequence_WillPhysicalShieldNonMelee* sequence, int edx_unused, unsigned int flags)
{
    CCombatSequence_WillPhysicalShieldNonMelee_dtor(sequence);
    if (flags & 1)
        operator delete(sequence);
    return sequence;
}

int main()
{
    CCombatSequence_WillPhysicalShieldNonMelee a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCombatSequence_WillPhysicalShieldNonMelee_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCombatSequence_WillPhysicalShieldNonMelee b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCombatSequence_WillPhysicalShieldNonMelee_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}