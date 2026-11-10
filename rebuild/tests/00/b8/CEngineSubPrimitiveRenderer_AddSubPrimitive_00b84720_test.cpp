// Behaviour test for CEngineSubPrimitiveRenderer::AddSubPrimitive (0x00B84720): host model of the sort-key computation
// (scaled distance rounded to nearest, clamped to [0, 0x3FFFFFFF]), the push onto the type's list and the manager store.
#include <stdio.h>
#include <vector>

struct SubPrim { int manager; };
struct SortElement { unsigned long key; SubPrim* sub; };
struct Renderer {
    float MaxSortDistance, SortDistanceMultiplier;
    std::vector<SortElement> lists[15];
};

static long RoundNear(float v)        // the x87 fistp under the default round-to-nearest mode (inputs here are off .5)
{
    return v >= 0.0f ? (long)(v + 0.5f) : -(long)(-v + 0.5f);
}

static void AddSubPrimitive(Renderer& r, int type, SubPrim* sub, float distance, int manager)
{
    float sortDistance = (r.MaxSortDistance - distance) * r.SortDistanceMultiplier;
    long sortValue = RoundNear(sortDistance);
    if (sortValue < 0)
        sortValue = 0;
    else if (sortValue > 0x3FFFFFFF)
        sortValue = 0x3FFFFFFF;
    SortElement e = { (unsigned long)sortValue, sub };
    r.lists[type].push_back(e);
    sub->manager = manager;
}

int main()
{
    int fails = 0;
    Renderer r; r.MaxSortDistance = 100.0f; r.SortDistanceMultiplier = 10.0f;
    SubPrim a = { 0 }, b = { 0 }, c = { 0 };
    AddSubPrimitive(r, 3, &a, 40.2f, 7);          // (100 - 40.2) * 10 = 598
    AddSubPrimitive(r, 3, &b, 150.0f, 8);         // negative -> 0
    r.SortDistanceMultiplier = 1.5e7f;
    AddSubPrimitive(r, 14, &c, 0.0f, 9);          // 1.5e9 -> clamped
    if (r.lists[3].size() != 2 || r.lists[14].size() != 1) ++fails;
    if (r.lists[3][0].key != 598 || r.lists[3][0].sub != &a) ++fails;
    if (r.lists[3][1].key != 0 || r.lists[3][1].sub != &b) ++fails;
    if (r.lists[14][0].key != 0x3FFFFFFF) ++fails;
    if (a.manager != 7 || b.manager != 8 || c.manager != 9) ++fails;
    if (fails == 0) printf("00b84720_TEST PASS\n"); else printf("00b84720_TEST FAIL %d\n", fails);
    return fails;
}