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

// __linear_insert row: if comp(val, *first) shift [first, last) up one slot (copy_backward) and store val first,
// else sink it from last (the unguarded insert).
static void copy_backward_(CGroup* first, CGroup* last, CGroup* result)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
}
static void linear_insert(CGroup* first, CGroup* last, CGroup val, PFNGroupCompare comp)
{
    if (comp(val, *first)) {
        copy_backward_(first, last, last + 1);
        *first = val;
    }
    else
        unguarded_linear_insert(last, val, comp);
}
int main()
{
    // sorted heaviest-first prefix [0.9, 0.7, 0.5, 0.3], then insert values at index 4
    CGroup g[5];
    g[0] = group_(0, 0.9f); g[1] = group_(1, 0.7f); g[2] = group_(2, 0.5f); g[3] = group_(3, 0.3f);
    g[4] = group_(4, 0.95f);                                   // heavier than all: goes to the front
    linear_insert(g, g + 4, g[4], CompareWeight);
    const unsigned int want1[5] = { 4, 0, 1, 2, 3 };
    for (int i = 0; i < 5; ++i) if (g[i].BoneIndex != want1[i]) { std::printf("00ad4880_TEST FAIL\n"); return 1; }
    CGroup h[5];
    h[0] = group_(0, 0.9f); h[1] = group_(1, 0.7f); h[2] = group_(2, 0.5f); h[3] = group_(3, 0.3f);
    h[4] = group_(4, 0.6f);                                    // lands between 0.7 and 0.5
    linear_insert(h, h + 4, h[4], CompareWeight);
    const unsigned int want2[5] = { 0, 1, 4, 2, 3 };
    for (int i = 0; i < 5; ++i) if (h[i].BoneIndex != want2[i]) { std::printf("00ad4880_TEST FAIL\n"); return 1; }
    std::printf("00ad4880_TEST PASS\n");
    return 0;
}