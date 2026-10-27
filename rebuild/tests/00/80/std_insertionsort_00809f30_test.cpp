// Behaviour test for 0x00809F30: __insertion_sort with __linear_insert over thing pointers
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SPtr { int Id; };
static bool Less(const SPtr& a, const SPtr& b) { return a.Id < b.Id; }
static void LinearInsert(SPtr* first, SPtr* last, SPtr val)
{
    if (Less(val, *first)) { for (SPtr* p = last; p != first; --p) *p = *(p - 1); *first = val; }
    else { SPtr* next = last - 1; while (Less(val, *next)) { *last = *next; last = next; --next; } *last = val; }
}
static void InsertionSort(SPtr* first, SPtr* last)
{
    if (first == last) return;
    for (SPtr* i = first + 1; i != last; ++i) LinearInsert(first, i, *i);
}
int main()
{
    int ids[8] = { 5, 3, 9, 1, 5, 0, 7, 2 };
    SPtr v[8]; for (int i = 0; i < 8; ++i) v[i].Id = ids[i];
    InsertionSort(v, v + 8);
    for (int i = 1; i < 8; ++i) CHECK(v[i - 1].Id <= v[i].Id);
    CHECK(v[0].Id == 0 && v[7].Id == 9);
    InsertionSort(v, v);
    if (!g_fail) printf("00809f30_TEST PASS\n");
    return g_fail;
}