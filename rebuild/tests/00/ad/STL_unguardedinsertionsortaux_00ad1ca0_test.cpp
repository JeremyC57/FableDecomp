// Behaviour test for _STL::__unguarded_insertion_sort_aux<CGroup*, CGroup, comp> @ 0x00AD1CA0
// Runs the unguarded linear insert on every element of [first, last); elements before first act as the guard.
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

static void unguarded_insertion_sort_aux(CGroup* first, CGroup* last, CGroup*, TGroupCompare comp)
{
    for (CGroup* i = first; i != last; ++i)
        unguarded_linear_insert(i, CGroup(*i), comp);
}

int main()
{
    // [0] is the guard (heaviest); sort [1, 7).
    CGroup g[7] = { { 0, 10.0f }, { 1, 0.2f }, { 2, 0.9f }, { 3, 0.4f }, { 4, 0.7f }, { 5, 0.1f }, { 6, 0.5f } };
    unguarded_insertion_sort_aux(g + 1, g + 7, (CGroup*)0, HeavierFirst);
    static const unsigned int order[7] = { 0, 2, 4, 6, 3, 1, 5 };
    int ok = 1;
    for (int i = 0; i < 7; ++i) if (g[i].BoneIndex != order[i]) ok = 0;
    for (int i = 1; i < 7; ++i) if (g[i].BoneWeight > g[i - 1].BoneWeight) ok = 0;
    unguarded_insertion_sort_aux(g + 3, g + 3, (CGroup*)0, HeavierFirst);   // empty range
    if (g[3].BoneIndex != 6) ok = 0;
    if (!ok) { std::printf("00ad1ca0_TEST FAIL\n"); return 1; }
    std::printf("00ad1ca0_TEST PASS\n");
    return 0;
}