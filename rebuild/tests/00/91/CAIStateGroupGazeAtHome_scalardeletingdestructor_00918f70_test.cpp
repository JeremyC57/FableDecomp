#include <cstdio>

struct CAIStateGroup_GazeAtHome {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CAIStateGroup_GazeAtHome* g_last_dtor = 0;

void __fastcall CAIStateGroup_GazeAtHome_dtor(CAIStateGroup_GazeAtHome* stateGroup)
{
    g_last_dtor = stateGroup;
    stateGroup->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CAIStateGroup_GazeAtHome_scalar_deleting_destructor(CAIStateGroup_GazeAtHome* stateGroup, int edx_unused, unsigned int flags)
{
    CAIStateGroup_GazeAtHome_dtor(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}

int main()
{
    CAIStateGroup_GazeAtHome a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CAIStateGroup_GazeAtHome_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CAIStateGroup_GazeAtHome b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CAIStateGroup_GazeAtHome_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00918f70_TEST PASS\n");
    return 0;
}