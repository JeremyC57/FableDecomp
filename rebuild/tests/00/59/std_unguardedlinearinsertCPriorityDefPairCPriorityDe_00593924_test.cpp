// Behaviour test for 0x00593924 std::__unguarded_linear_insert (priority-descending pairs, model copy)
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
int main()
{
    // sorted descending prefix [9, 7, 5, 3]; insert 6 (the slot at index 4 holds it)
    Entry v[5];
    int pr[5] = { 9, 7, 5, 3, 6 };
    for (int i = 0; i < 5; ++i) { v[i].id = i; v[i].priority = pr[i]; }
    unguarded_linear_insert(v + 4, v[4], PriorityCompare());
    int want[5] = { 9, 7, 6, 5, 3 };
    for (int i = 0; i < 5; ++i) if (v[i].priority != want[i]) { std::printf("00593924_TEST FAIL\n"); return 1; }
    if (v[2].id != 4 || v[3].id != 2) { std::printf("00593924_TEST FAIL\n"); return 1; }
    std::printf("00593924_TEST PASS\n");
    return 0;
}