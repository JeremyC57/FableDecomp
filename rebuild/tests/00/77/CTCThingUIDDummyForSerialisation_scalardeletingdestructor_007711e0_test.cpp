#include <cstdio>

struct CTCThingUIDDummyForSerialisation {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCThingUIDDummyForSerialisation* g_last_dtor = 0;

void __fastcall CTCThingUIDDummyForSerialisation_dtor(CTCThingUIDDummyForSerialisation* serialisation)
{
    g_last_dtor = serialisation;
    serialisation->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCThingUIDDummyForSerialisation_scalar_deleting_destructor(CTCThingUIDDummyForSerialisation* serialisation, int edx_unused, unsigned int flags)
{
    CTCThingUIDDummyForSerialisation_dtor(serialisation);
    if (flags & 1)
        operator delete(serialisation);
    return serialisation;
}

int main()
{
    CTCThingUIDDummyForSerialisation a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCThingUIDDummyForSerialisation_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCThingUIDDummyForSerialisation b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCThingUIDDummyForSerialisation_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_007711e0_TEST PASS\n");
    return 0;
}