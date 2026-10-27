// Test for 0x00A32BC0 std::__pop_heap<pair<unsigned long, C3DDummyObject>*, ...>: moves the heap top into *result and
// re-heaps [first, last) with the saved value via __adjust_heap(first, 0, last - first, val, comp).
#include <stdio.h>
struct Entry { unsigned int first; int second; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };
static void adjust_heap(Entry* first, int hole, int len, Entry val, Less comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) { if (comp(first[child], first[child - 1])) --child; first[hole] = first[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void pop_heap_aux(Entry* first, Entry* last, Entry* result, Entry val, Less comp)
{
    *result = *first;
    adjust_heap(first, 0, (int)(last - first), val, comp);
}
int main()
{
    Entry h[5] = { { 9, 0 }, { 7, 1 }, { 8, 2 }, { 3, 3 }, { 5, 4 } };
    Entry val = h[4];
    pop_heap_aux(h, h + 4, h + 4, val, Less());
    bool ok = h[4].first == 9 && h[0].first == 8;
    for (int i = 1; i < 4; ++i) if (h[(i - 1) / 2].first < h[i].first) ok = false;
    puts(ok ? "00a32bc0_TEST PASS" : "00a32bc0_TEST FAIL");
    return ok ? 0 : 1;
}