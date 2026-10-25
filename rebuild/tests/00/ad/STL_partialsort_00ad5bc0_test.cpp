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

// __partial_sort row: make_heap(first, middle); swap in every later element that sorts before the heap top; sort_heap
static void partial_sort_(CGroup* first, CGroup* middle, CGroup* last, PFNGroupCompare comp)
{
    make_heap_(first, middle, comp);
    for (CGroup* i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            CGroup v = *i;
            *i = *first;
            adjust_heap(first, 0, (int)(middle - first), v, comp);
        }
    sort_heap_(first, middle, comp);
}
int main()
{
    const float w[10] = { 0.15f, 0.85f, 0.35f, 0.95f, 0.25f, 0.55f, 0.75f, 0.45f, 0.65f, 0.05f };
    CGroup g[10];
    for (int i = 0; i < 10; ++i) g[i] = group_(i, w[i]);
    partial_sort_(g, g + 4, g + 10, CompareWeight);            // the four heaviest groups, heaviest first
    const unsigned int want[4] = { 3, 1, 6, 8 };
    for (int i = 0; i < 4; ++i) if (g[i].BoneIndex != want[i]) { std::printf("00ad5bc0_TEST FAIL\n"); return 1; }
    unsigned int idx = 0;
    for (int i = 0; i < 10; ++i) idx += g[i].BoneIndex;
    if (idx != 45) { std::printf("00ad5bc0_TEST FAIL\n"); return 1; }
    std::printf("00ad5bc0_TEST PASS\n");
    return 0;
}