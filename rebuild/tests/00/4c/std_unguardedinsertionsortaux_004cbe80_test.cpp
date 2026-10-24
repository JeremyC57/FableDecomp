// Behaviour test for 0x004CBE80 __unguarded_insertion_sort_aux over component pairs: each element of [first, last) is moved
// back past every predecessor with a larger interface-type key (signed), relying on a smaller-or-equal key before first;
// equal keys keep their order.
#include <cstdio>

struct Pair { long first; void* second; };
static void Insert(Pair* last, Pair v) { Pair* next = last - 1; while (v.first < next->first) { *last = *next; last = next; --next; } *last = v; }
static void SortAux(Pair* first, Pair* last) { for (Pair* i = first; i != last; ++i) Insert(i, *i); }

int main()
{
    int tags[6];
    Pair a[7] = { { -100, 0 }, { 5, &tags[0] }, { -3, &tags[1] }, { 9, &tags[2] }, { 5, &tags[3] }, { 0, &tags[4] }, { -3, &tags[5] } };
    SortAux(a + 1, a + 7);                              // a[0] is the guard
    long want[7] = { -100, -3, -3, 0, 5, 5, 9 };
    for (int k = 0; k < 7; ++k) if (a[k].first != want[k]) { std::printf("004cbe80_TEST FAIL\n"); return 1; }
    if (a[1].second != &tags[1] || a[2].second != &tags[5] || a[4].second != &tags[0] || a[5].second != &tags[3]) {
        std::printf("004cbe80_TEST FAIL\n"); return 1;
    }
    SortAux(a + 3, a + 3);                              // empty range: unchanged
    if (a[3].first != 0) { std::printf("004cbe80_TEST FAIL\n"); return 1; }
    std::printf("004cbe80_TEST PASS\n");
    return 0;
}