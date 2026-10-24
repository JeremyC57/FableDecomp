// Behaviour test for 0x00593E10 std::__unguarded_insertion_sort_aux (priority-descending pairs, model copy)
#include <cstdio>
struct Entry { int id; int priority; };
struct PriorityCompare { bool operator()(const Entry& l, const Entry& r) const { return l.priority > r.priority; } };
static void unguarded_linear_insert(Entry* last, Entry val, PriorityCompare comp)
{
    Entry* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void unguarded_insertion_sort_aux(Entry* first, Entry* last, Entry*, PriorityCompare comp)
{
    for (Entry* i = first; i != last; ++i) unguarded_linear_insert(i, Entry(*i), comp);
}
int main()
{
    // v[0] is a sentinel already >= every later element (the introsort guarantees one before `first`)
    Entry v[7];
    int pr[7] = { 100, 3, 8, 1, 8, 5, 2 };
    for (int i = 0; i < 7; ++i) { v[i].id = i; v[i].priority = pr[i]; }
    unguarded_insertion_sort_aux(v + 1, v + 7, (Entry*)0, PriorityCompare());
    int want[7] = { 100, 8, 8, 5, 3, 2, 1 };
    for (int i = 0; i < 7; ++i) if (v[i].priority != want[i]) { std::printf("00593e10_TEST FAIL\n"); return 1; }
    if (v[1].id != 2 || v[2].id != 4) { std::printf("00593e10_TEST FAIL\n"); return 1; }   // stable for equal keys
    std::printf("00593e10_TEST PASS\n");
    return 0;
}