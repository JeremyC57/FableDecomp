#include <cstdio>

class C3DSkeletalMorph { public:
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static C3DSkeletalMorph* g_last_dtor = 0;

void __fastcall C3DSkeletalMorph_dtor(C3DSkeletalMorph* morph)
{
    g_last_dtor = morph;
    morph->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall C3DSkeletalMorph_scalar_deleting_destructor(C3DSkeletalMorph* morph, int edx_unused, unsigned int flags)
{
    C3DSkeletalMorph_dtor(morph);
    if (flags & 1)
        operator delete(morph);
    return morph;
}

int main()
{
    C3DSkeletalMorph a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = C3DSkeletalMorph_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    C3DSkeletalMorph b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = C3DSkeletalMorph_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00a47020_TEST PASS\n");
    return 0;
}