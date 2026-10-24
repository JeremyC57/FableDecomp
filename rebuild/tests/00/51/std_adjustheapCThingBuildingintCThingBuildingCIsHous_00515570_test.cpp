// Behaviour test for 0x00515570 std::__adjust_heap<CThingBuilding**,int,CThingBuilding*,CIsHouseLargerThan>(first,
// holeIndex, len, val, comp): the hole sinks along the child that does NOT compare "larger" (the right child unless
// comp(right, left)), a lone last left child is taken when secondChild == len, and __push_heap then raises val from the
// final hole back toward topIndex. Under the "larger than" comparator the heap keeps the SMALLEST house at the root.
// Re-heaping after replacing the root with any value must leave a valid heap holding the same multiset.
#include <cstdio>

struct House { int size; };

struct IsHouseLargerThan {                                   // stand-in for CIsHouseLargerThan::operator() 0x00512410
    bool operator()(const House* a, const House* b) const { return a->size > b->size; }
};

static void push_heap_model(House** first, int holeIndex, int topIndex, House* val, IsHouseLargerThan comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}

static void adjust_heap_model(House** first, int holeIndex, int len, House* val, IsHouseLargerThan comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    push_heap_model(first, holeIndex, topIndex, val, comp);
}

static bool is_heap(House** v, int n)
{
    IsHouseLargerThan comp;
    for (int i = 1; i < n; ++i)
        if (comp(v[(i - 1) / 2], v[i])) return false;       // a parent never larger than its child
    return true;
}

int main()
{
    House h[9];
    int sizes[9] = { 7, 3, 9, 1, 12, 5, 8, 2, 6 };
    for (int k = 0; k < 9; ++k) h[k].size = sizes[k];
    IsHouseLargerThan comp;
    for (int len = 1; len <= 9; ++len) {                     // odd and even lengths (the secondChild == len tail)
        House* v[9];
        for (int k = 0; k < len; ++k) v[k] = &h[k];
        for (int hole = (len - 2) / 2; hole >= 0; --hole)    // make_heap through adjust_heap
            adjust_heap_model(v, hole, len, v[hole], comp);
        if (!is_heap(v, len)) { std::printf("00515570_TEST FAIL\n"); return 1; }
        for (int k = 0; k < len; ++k) {
            int seen = 0;
            for (int m = 0; m < len; ++m) if (v[m] == &h[k]) ++seen;
            if (seen != 1) { std::printf("00515570_TEST FAIL\n"); return 1; }
        }
        int smallest = h[0].size;
        for (int k = 1; k < len; ++k) if (h[k].size < smallest) smallest = h[k].size;
        if (v[0]->size != smallest) { std::printf("00515570_TEST FAIL\n"); return 1; }
        if (len > 1) {                                       // pop_heap step: move the last into the root's hole
            House* last = v[len - 1];
            v[len - 1] = v[0];
            adjust_heap_model(v, 0, len - 1, last, comp);
            if (!is_heap(v, len - 1) || v[len - 1]->size != smallest) { std::printf("00515570_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("00515570_TEST PASS\n");
    return 0;
}