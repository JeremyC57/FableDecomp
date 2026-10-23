#include <cstdio>

struct CCombatSequenceGeneric {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CCombatSequenceGeneric* g_last_dtor = 0;

void __fastcall CCombatSequenceGeneric_dtor(CCombatSequenceGeneric* sequence)
{
    g_last_dtor = sequence;
    sequence->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CCombatSequenceGeneric_scalar_deleting_destructor(CCombatSequenceGeneric* sequence, int edx_unused, unsigned int flags)
{
    CCombatSequenceGeneric_dtor(sequence);
    if (flags & 1)
        operator delete(sequence);
    return sequence;
}

int main()
{
    CCombatSequenceGeneric a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CCombatSequenceGeneric_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CCombatSequenceGeneric b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CCombatSequenceGeneric_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_006f1750_TEST PASS\n");
    return 0;
}