// Behaviour test for 0x00A80100 std::__pop_heap_aux<pair<unsigned __int64,bool>*, ..., CKeyPairCompareLess<unsigned __int64,bool> >
// Model: STLport __pop_heap_aux -> __pop_heap: save the last value, move the top to the last slot, sift the saved value down.
#include <cstdio>
typedef unsigned long long u64;
struct Pair { u64 first; bool second; };
struct Less { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };
static void push_heap(Pair* first, int hole, int top, Pair val, Less comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void adjust_heap(Pair* first, int hole, int len, Pair val, Less comp)
{
    int top = hole;
    int child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child];
        hole = child;
        child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    push_heap(first, hole, top, val, comp);
}
static void pop_heap_aux(Pair* first, Pair* last, Less comp)
{
    Pair val = *(last - 1);
    *(last - 1) = *first;
    adjust_heap(first, 0, (int)((last - 1) - first), val, comp);
}
int main()
{
    Pair h[6];
    u64 keys[6] = { 90, 70, 80, 10, 60, 50 };              // a valid max-heap
    for (int i = 0; i < 6; ++i) { h[i].first = keys[i]; h[i].second = (i & 1) != 0; }
    u64 expect[6] = { 90, 80, 70, 60, 50, 10 };
    Pair* last = h + 6;
    for (int n = 0; n < 6; ++n) {
        pop_heap_aux(h, last, Less());
        --last;
        if (last->first != expect[n]) { std::printf("00a80100_TEST FAIL\n"); return 1; }
    }
    if (h[5].second != false || h[0].first != 10) { std::printf("00a80100_TEST FAIL\n"); return 1; }
    std::printf("00a80100_TEST PASS\n");
    return 0;
}