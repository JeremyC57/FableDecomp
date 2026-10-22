#include <cstdio>

struct CTCVolumeContainmentTracker {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CTCVolumeContainmentTracker* g_last_dtor = 0;

void __fastcall CTCVolumeContainmentTracker_dtor(CTCVolumeContainmentTracker* tracker)
{
    g_last_dtor = tracker;
    tracker->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CTCVolumeContainmentTracker_scalar_deleting_destructor(CTCVolumeContainmentTracker* tracker, int edx_unused, unsigned int flags)
{
    CTCVolumeContainmentTracker_dtor(tracker);
    if (flags & 1)
        operator delete(tracker);
    return tracker;
}

int main()
{
    CTCVolumeContainmentTracker a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CTCVolumeContainmentTracker_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CTCVolumeContainmentTracker b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CTCVolumeContainmentTracker_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}