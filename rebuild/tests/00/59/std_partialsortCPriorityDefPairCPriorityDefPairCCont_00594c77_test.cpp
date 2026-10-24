// Behaviour test for 0x00594C77 std::__partial_sort (priority-compare pairs, model copy with STLport heap helpers)
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
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    push_heap(first, hole, top, val, comp);
}
static void make_heap(Entry* first, Entry* last, PriorityCompare comp)
{
    int len = int(last - first);
    if (len < 2) return;
    for (int parent = (len - 2) / 2; ; --parent) { adjust_heap(first, parent, len, first[parent], comp); if (parent == 0) break; }
}
static void pop_heap_into(Entry* first, Entry* last, Entry* result, Entry val, PriorityCompare comp, int*)
{
    *result = *first;
    adjust_heap(first, 0, int(last - first), val, comp);
}
static void sort_heap(Entry* first, Entry* last, PriorityCompare comp)
{
    while (last - first > 1) { --last; Entry v = *last; pop_heap_into(first, last, last, v, comp, 0); }
}
static void partial_sort(Entry* first, Entry* middle, Entry* last, Entry*, PriorityCompare comp)
{
    make_heap(first, middle, comp);
    for (Entry* i = middle; i < last; ++i)
        if (comp(*i, *first)) pop_heap_into(first, middle, i, Entry(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
int main()
{
    int pr[10] = { 4, 15, 8, 1, 16, 23, 42, 7, 3, 9 };
    Entry v[10];
    for (int i = 0; i < 10; ++i) { v[i].id = i; v[i].priority = pr[i]; }
    partial_sort(v, v + 4, v + 10, (Entry*)0, PriorityCompare());
    int want[4] = { 42, 23, 16, 15 };
    for (int i = 0; i < 4; ++i) if (v[i].priority != want[i]) { std::printf("00594c77_TEST FAIL\n"); return 1; }
    int sum = 0; for (int i = 0; i < 10; ++i) sum += v[i].priority;
    if (sum != 128) { std::printf("00594c77_TEST FAIL\n"); return 1; }
    std::printf("00594c77_TEST PASS\n");
    return 0;
}