// Behaviour test for 0x00594637 std::__unguarded_partition (priority-descending pairs, model copy)
#include <cstdio>
struct Entry { int id; int priority; };
struct PriorityCompare { bool operator()(const Entry& l, const Entry& r) const { return l.priority > r.priority; } };
static void iter_swap(Entry* a, Entry* b) { Entry t = *a; *a = *b; *b = t; }
static Entry* unguarded_partition(Entry* first, Entry* last, Entry pivot, PriorityCompare comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        iter_swap(first, last);
        ++first;
    }
}
int main()
{
    int pr[8] = { 3, 9, 1, 7, 5, 2, 8, 4 };
    Entry v[8];
    for (int i = 0; i < 8; ++i) { v[i].id = i; v[i].priority = pr[i]; }
    Entry pivot; pivot.id = -1; pivot.priority = 5;
    Entry* cut = unguarded_partition(v, v + 8, pivot, PriorityCompare());
    if (cut <= v || cut >= v + 8) { std::printf("00594637_TEST FAIL\n"); return 1; }
    for (Entry* p = v; p < cut; ++p) if (p->priority < 5) { std::printf("00594637_TEST FAIL\n"); return 1; }
    for (Entry* p = cut; p < v + 8; ++p) if (p->priority > 5) { std::printf("00594637_TEST FAIL\n"); return 1; }
    int sum = 0; for (int i = 0; i < 8; ++i) sum += v[i].priority;
    if (sum != 39) { std::printf("00594637_TEST FAIL\n"); return 1; }
    std::printf("00594637_TEST PASS\n");
    return 0;
}