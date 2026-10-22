#include <cstdio>

struct CTCHeroExperience {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCHeroExperience* g_last_dtor = 0;

void __fastcall CTCHeroExperience_dtor(CTCHeroExperience* experience)
{
    g_last_dtor = experience;
    experience->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCHeroExperience_scalar_deleting_destructor(CTCHeroExperience* experience, int edx_unused, unsigned int flags)
{
    CTCHeroExperience_dtor(experience);
    if (flags & 1)
        operator delete(experience);
    return experience;
}

int main()
{
    CTCHeroExperience a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCHeroExperience_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCHeroExperience b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCHeroExperience_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}