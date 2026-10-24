// Behaviour test for 0x00AD4960 STLport sort_heap over 8-byte records with a function-pointer comparator.
#include <cstdio>

struct CGroup { unsigned int BoneIndex; unsigned int BoneWeight; };
typedef bool (*PFNGroupCompare)(const CGroup&, const CGroup&);

static bool CompareWeight(const CGroup& a, const CGroup& b) { return a.BoneWeight < b.BoneWeight; }

// reference __push_heap / __adjust_heap (the out-of-line callee 0x00AD1DD0 and its _Push_heap)
static void push_heap_aux(CGroup* first, int hole, int top, CGroup value, PFNGroupCompare comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
static void adjust_heap(CGroup* first, int hole, int len, CGroup value, PFNGroupCompare comp)
{
    int top = hole; int child = 2 * hole + 2;
    while (child < len) { if (comp(first[child], first[child - 1])) --child; first[hole] = first[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    push_heap_aux(first, hole, top, value, comp);
}

// the reconstructed body (sort_heap -> pop_heap inline)
static void sort_heap(CGroup* first, CGroup* last, PFNGroupCompare comp)
{
    while (last - first > 1) {
        CGroup value = *(last - 1);
        *(last - 1) = *first;
        adjust_heap(first, 0, (int)((last - 1) - first), value, comp);
        --last;
    }
}

int main()
{
    CGroup v[9];
    unsigned int keys[9] = { 5, 1, 9, 3, 7, 2, 8, 6, 4 };
    for (int i = 0; i < 9; ++i) { v[i].BoneIndex = i; v[i].BoneWeight = keys[i]; }
    // make_heap by repeated push
    for (int n = 1; n < 9; ++n) push_heap_aux(v, n, 0, v[n], CompareWeight);
    sort_heap(v, v + 9, CompareWeight);
    for (int i = 0; i < 9; ++i) {
        if (v[i].BoneWeight != (unsigned int)(i + 1)) { std::printf("00ad4960_TEST FAIL\n"); return 1; }
        if (keys[v[i].BoneIndex] != v[i].BoneWeight) { std::printf("00ad4960_TEST FAIL\n"); return 1; }
    }
    // one-element and empty ranges are untouched
    CGroup one = { 7, 42 };
    sort_heap(&one, &one + 1, CompareWeight);
    if (one.BoneIndex != 7 || one.BoneWeight != 42) { std::printf("00ad4960_TEST FAIL\n"); return 1; }
    std::printf("00ad4960_TEST PASS\n");
    return 0;
}