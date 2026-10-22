#include <cstdio>

struct CTCBed {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCBed* g_last_dtor = 0;

void __fastcall CTCBed_dtor(CTCBed* bed)
{
    g_last_dtor = bed;
    bed->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCBed_scalar_deleting_destructor(CTCBed* bed, int edx_unused, unsigned int flags)
{
    CTCBed_dtor(bed);
    if (flags & 1)
        operator delete(bed);
    return bed;
}

int main()
{
    CTCBed a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCBed_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCBed b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCBed_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}