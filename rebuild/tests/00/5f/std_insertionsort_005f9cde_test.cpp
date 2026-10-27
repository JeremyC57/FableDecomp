// behaviour test for 0x005F9CDE: the insertion sort orders the pairs by key, stable for equal keys
#include <cstdio>
struct P { long k; long v; };
static void LinearInsert(P* first, P* last, P val) { P* next = last; while (next != first && val.k < (next - 1)->k) { *next = *(next - 1); --next; } *next = val; }
static void InsertionSort(P* first, P* last) { if (first == last) return; for (P* i = first + 1; i != last; ++i) LinearInsert(first, i, *i); }
int main()
{
    P v[5] = { { 4, 0 }, { 1, 1 }, { 3, 2 }, { 1, 3 }, { 0, 4 } };
    InsertionSort(v, v + 5);
    if (v[0].k != 0 || v[1].v != 1 || v[2].v != 3 || v[4].k != 4) { std::printf("005f9cde_TEST FAIL\n"); return 1; }
    std::printf("005f9cde_TEST PASS\n");
    return 0;
}