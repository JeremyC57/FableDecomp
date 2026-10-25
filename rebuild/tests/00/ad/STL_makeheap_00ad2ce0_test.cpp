#include <cstdio>

// Host model of the CGroup heap/sort family (COptimisedVertex::CGroup {BoneIndex, BoneWeight}) under a __fastcall-style
// function-pointer comparator (here: heavier weight first, as CompareWeight orders the bone groups).
struct CGroup {
    unsigned int BoneIndex;
    float        BoneWeight;
};
typedef bool (*PFNGroupCompare)(const CGroup&, const CGroup&);
static bool CompareWeight(const CGroup& a, const CGroup& b) { return a.BoneWeight > b.BoneWeight; }

static void push_heap_(CGroup* first, int hole, int top, CGroup value, PFNGroupCompare comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void adjust_heap(CGroup* first, int hole, int len, CGroup value, PFNGroupCompare comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1]))
            second--;
        first[hole] = first[second];
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) {
        first[hole] = first[second - 1];
        hole = second - 1;
    }
    push_heap_(first, hole, top, value, comp);
}
static void make_heap_(CGroup* first, CGroup* last, PFNGroupCompare comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, first[parent], comp);
        if (parent == 0) return;
        parent--;
    }
}
static void sort_heap_(CGroup* first, CGroup* last, PFNGroupCompare comp)
{
    while (last - first > 1) {
        --last;
        CGroup v = *last;
        *last = *first;
        adjust_heap(first, 0, (int)(last - first), v, comp);
    }
}
static bool is_heap_(const CGroup* first, int len, PFNGroupCompare comp)
{
    for (int i = 1; i < len; ++i)
        if (comp(first[(i - 1) / 2], first[i])) return false;
    return true;
}
static void unguarded_linear_insert(CGroup* last, CGroup val, PFNGroupCompare comp)
{
    CGroup* next = last - 1;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static CGroup group_(unsigned int index, float weight) { CGroup g; g.BoneIndex = index; g.BoneWeight = weight; return g; }

// make_heap row: STLport __make_heap(first, last, comp, (CGroup*)0, (int*)0)
static void __make_heap_row(CGroup* first, CGroup* last, PFNGroupCompare comp)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, CGroup(first[parent]), comp);
        if (parent == 0) return;
        parent--;
    }
}
int main()
{
    const float w[9] = { 0.1f, 0.7f, 0.3f, 0.9f, 0.2f, 0.5f, 0.8f, 0.4f, 0.6f };
    CGroup g[9];
    for (int i = 0; i < 9; ++i) g[i] = group_(i, w[i]);
    __make_heap_row(g, g + 9, CompareWeight);
    if (!is_heap_(g, 9, CompareWeight)) { std::printf("00ad2ce0_TEST FAIL\n"); return 1; }
    if (g[0].BoneIndex != 0 || g[0].BoneWeight > 0.15f) { std::printf("00ad2ce0_TEST FAIL\n"); return 1; }   // lightest on top
    float sum = 0.0f; unsigned int idx = 0;
    for (int i = 0; i < 9; ++i) { sum += g[i].BoneWeight; idx += g[i].BoneIndex; }
    if (idx != 36 || sum < 4.49f || sum > 4.51f) { std::printf("00ad2ce0_TEST FAIL\n"); return 1; }        // a permutation
    CGroup one[1]; one[0] = group_(7, 1.0f);
    __make_heap_row(one, one + 1, CompareWeight);
    if (one[0].BoneIndex != 7) { std::printf("00ad2ce0_TEST FAIL\n"); return 1; }
    std::printf("00ad2ce0_TEST PASS\n");
    return 0;
}