// Behaviour test for 0x00594946 std::__linear_insert (priority-descending pairs, model copy)
#include <cstdio>
struct Entry { int id; int priority; };
struct PriorityCompare { bool operator()(const Entry& l, const Entry& r) const { return l.priority > r.priority; } };
static Entry* copy_backward(Entry* first, Entry* last, Entry* result)
{
    for (int n = int(last - first); n > 0; --n) *--result = *--last;
    return result;
}
static void unguarded_linear_insert(Entry* last, Entry val, PriorityCompare comp)
{
    Entry* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void linear_insert(Entry* first, Entry* last, Entry val, PriorityCompare comp)
{
    if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; }
    else unguarded_linear_insert(last, val, comp);
}
int main()
{
    Entry v[6];
    int pr[6] = { 9, 6, 4, 2, 12, 5 };
    for (int i = 0; i < 6; ++i) { v[i].id = i; v[i].priority = pr[i]; }
    linear_insert(v, v + 4, v[4], PriorityCompare());   // 12 goes to the front
    int want1[5] = { 12, 9, 6, 4, 2 };
    for (int i = 0; i < 5; ++i) if (v[i].priority != want1[i]) { std::printf("00594946_TEST FAIL\n"); return 1; }
    linear_insert(v, v + 5, v[5], PriorityCompare());   // 5 goes between 6 and 4
    int want2[6] = { 12, 9, 6, 5, 4, 2 };
    for (int i = 0; i < 6; ++i) if (v[i].priority != want2[i]) { std::printf("00594946_TEST FAIL\n"); return 1; }
    std::printf("00594946_TEST PASS\n");
    return 0;
}