// Behaviour test for 0x00594706 std::__make_heap (priority-compare pairs, model copy with the STLport sift-down)
#include <cstdio>
struct Entry { int id; int priority; };
struct PriorityCompare { bool operator()(const Entry& l, const Entry& r) const { return l.priority > r.priority; } };
static void push_heap(Entry* first, int hole, int top, Entry val, PriorityCompare comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void adjust_heap(Entry* first, int hole, int len, Entry val, PriorityCompare comp)
{
    int top = hole;
    int child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    push_heap(first, hole, top, val, comp);
}
static void make_heap(Entry* first, Entry* last, PriorityCompare comp, Entry*, int*)
{
    if (last - first < 2) return;
    int len = int(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        adjust_heap(first, parent, len, Entry(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}
int main()
{
    int pr[9] = { 5, 3, 9, 1, 7, 2, 8, 6, 4 };
    Entry v[9];
    for (int i = 0; i < 9; ++i) { v[i].id = i; v[i].priority = pr[i]; }
    make_heap(v, v + 9, PriorityCompare(), (Entry*)0, (int*)0);
    PriorityCompare comp;
    for (int i = 1; i < 9; ++i) if (comp(v[(i - 1) / 2], v[i])) { std::printf("00594706_TEST FAIL\n"); return 1; }
    if (v[0].priority != 1) { std::printf("00594706_TEST FAIL\n"); return 1; }
    Entry one[1]; one[0].id = 7; one[0].priority = 3;
    make_heap(one, one + 1, comp, (Entry*)0, (int*)0);
    if (one[0].id != 7) { std::printf("00594706_TEST FAIL\n"); return 1; }
    std::printf("00594706_TEST PASS\n");
    return 0;
}