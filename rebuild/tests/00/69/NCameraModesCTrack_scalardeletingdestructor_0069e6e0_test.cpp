#include <cstdio>

struct CTrack {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTrack* g_last_dtor = 0;

void __fastcall CTrack_dtor(CTrack* track)
{
    g_last_dtor = track;
    track->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTrack_scalar_deleting_destructor(CTrack* track, int edx_unused, unsigned int flags)
{
    CTrack_dtor(track);
    if (flags & 1)
        operator delete(track);
    return track;
}

int main()
{
    CTrack a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTrack_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTrack b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTrack_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}