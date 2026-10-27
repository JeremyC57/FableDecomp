// Behaviour test for std::__final_insertion_sort<pair<unsigned long, CRegisteredModeInfo>*> (0x00743B70): guarded sort of the first 16, unguarded insertion of the rest.
#include <cstdio>
struct Pair { unsigned int first; int second; };
static void insertion_sort(Pair* first, Pair* last)
{
    if (first == last) return;
    for (Pair* i = first + 1; i != last; ++i) {
        Pair val = *i;
        if (val.first < first->first) { for (Pair* p = i; p != first; --p) *p = p[-1]; *first = val; }
        else { Pair* l = i; Pair* n = i - 1; while (val.first < n->first) { *l = *n; l = n; --n; } *l = val; }
    }
}
static void unguarded_insertion_sort(Pair* first, Pair* last)
{
    for (Pair* i = first; i != last; ++i) { Pair val = *i; Pair* l = i; Pair* n = i - 1; while (val.first < n->first) { *l = *n; l = n; --n; } *l = val; }
}
static void final_insertion_sort(Pair* first, Pair* last)
{
    if (last - first > 16) { insertion_sort(first, first + 16); unguarded_insertion_sort(first + 16, last); }
    else insertion_sort(first, last);
}
int main()
{
    Pair a[40];
    // introsort leaves the range partitioned in 16-element blocks: each block's keys exceed the previous block's
    for (int i = 0; i < 40; ++i) { int block = i / 16; a[i].first = (unsigned int)(block * 100 + (37 * i) % 16); a[i].second = i; }
    final_insertion_sort(a, a + 40);
    for (int i = 1; i < 40; ++i) if (a[i - 1].first > a[i].first) { std::printf("00743b70_TEST FAIL\n"); return 1; }
    Pair b[5] = { {5, 0}, {1, 1}, {4, 2}, {2, 3}, {3, 4} };
    final_insertion_sort(b, b + 5);
    for (int i = 0; i < 5; ++i) if (b[i].first != (unsigned int)(i + 1)) { std::printf("00743b70_TEST FAIL\n"); return 1; }
    std::printf("00743b70_TEST PASS\n");
    return 0;
}