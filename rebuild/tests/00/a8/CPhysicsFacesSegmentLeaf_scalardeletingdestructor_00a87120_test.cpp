#include <cstdio>

struct CPhysicsFacesSegmentLeaf {
    void* vftable;
    int destroyed;
};

static int g_freed = 0;
static CPhysicsFacesSegmentLeaf* g_last_dtor = 0;

void __fastcall CPhysicsFacesSegmentLeaf_dtor(CPhysicsFacesSegmentLeaf* leaf)
{
    g_last_dtor = leaf;
    leaf->destroyed = 1;
}

void operator delete(void* p)
{
    g_freed = 1;
    (void)p;
}

void* __fastcall CPhysicsFacesSegmentLeaf_scalar_deleting_destructor(CPhysicsFacesSegmentLeaf* leaf, int edx_unused, unsigned int flags)
{
    CPhysicsFacesSegmentLeaf_dtor(leaf);
    if (flags & 1)
        operator delete(leaf);
    return leaf;
}

int main()
{
    CPhysicsFacesSegmentLeaf a;
    a.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r = CPhysicsFacesSegmentLeaf_scalar_deleting_destructor(&a, 0, 0);
    if (r != &a || a.destroyed != 1 || g_freed != 0 || g_last_dtor != &a) {
        std::printf("FAIL no-free path\n");
        return 1;
    }

    CPhysicsFacesSegmentLeaf b;
    b.destroyed = 0;
    g_freed = 0;
    g_last_dtor = 0;
    void* r2 = CPhysicsFacesSegmentLeaf_scalar_deleting_destructor(&b, 0, 1);
    if (r2 != &b || b.destroyed != 1 || g_freed != 1 || g_last_dtor != &b) {
        std::printf("FAIL free path\n");
        return 1;
    }

    std::printf("CLandscapeBackgroundPatch_00442980_TEST PASS\n");
    return 0;
}