// Behaviour test for std::__unguarded_linear_insert<CGroup*, CGroup, comp> @ 0x00AD08B0
// Shifts elements up while comp(val, *prev) and stores val in the gap (no lower bound check: a sentinel stops it).
#include <cstdio>

struct CGroup { unsigned int BoneIndex; float BoneWeight; };
typedef bool (*TGroupCompare)(const CGroup& a, const CGroup& b);

static bool HeavierFirst(const CGroup& a, const CGroup& b) { return a.BoneWeight > b.BoneWeight; }

static void unguarded_linear_insert(CGroup* last, CGroup val, TGroupCompare comp)
{
    CGroup* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

int main()
{
    // Sorted descending prefix with a heavy sentinel at [0]; insert the element at [4].
    CGroup g[5] = { { 0, 9.0f }, { 1, 0.8f }, { 2, 0.5f }, { 3, 0.1f }, { 4, 0.6f } };
    unguarded_linear_insert(g + 4, g[4], HeavierFirst);
    static const unsigned int order[5] = { 0, 1, 4, 2, 3 };
    int ok = 1;
    for (int i = 0; i < 5; ++i) if (g[i].BoneIndex != order[i]) ok = 0;
    // Already in place: nothing moves.
    CGroup h[3] = { { 0, 3.0f }, { 1, 2.0f }, { 2, 1.0f } };
    unguarded_linear_insert(h + 2, h[2], HeavierFirst);
    if (h[0].BoneIndex != 0 || h[1].BoneIndex != 1 || h[2].BoneIndex != 2) ok = 0;
    if (!ok) { std::printf("00ad08b0_TEST FAIL\n"); return 1; }
    std::printf("00ad08b0_TEST PASS\n");
    return 0;
}