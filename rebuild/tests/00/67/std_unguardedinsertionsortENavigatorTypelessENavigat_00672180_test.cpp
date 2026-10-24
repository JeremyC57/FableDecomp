// Behaviour test for 0x00672180 std::__unguarded_insertion_sort (model copy)
#include <cstdio>
enum ENav { N0, N1, N2, N3, N4, N5, N6, N7 };
struct Less { bool operator()(const ENav& a, const ENav& b) const { return a < b; } };
static void UnguardedLinearInsert(ENav* last, ENav val, Less comp)
{
    ENav* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void UnguardedInsertionSort(ENav* first, ENav* last, Less comp)
{
    for (ENav* i = first; i != last; ++i) UnguardedLinearInsert(i, *i, comp);
}
static void LinearInsert(ENav* first, ENav* last, ENav val, Less comp)
{
    if (comp(val, *first)) { for (ENav* p = last; p != first; --p) *p = *(p - 1); *first = val; }
    else UnguardedLinearInsert(last, val, comp);
}
static void InsertionSort(ENav* first, ENav* last, Less comp)
{
    if (first == last) return;
    for (ENav* i = first + 1; i != last; ++i) LinearInsert(first, i, *i, comp);
}
static void FinalInsertionSort(ENav* first, ENav* last, Less comp)
{
    if (last - first > 16) { InsertionSort(first, first + 16, comp); UnguardedInsertionSort(first + 16, last, comp); }
    else InsertionSort(first, last, comp);
}
static int Lg(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
static void Sort(ENav* first, ENav* last)
{
    if (first != last) { /* the introsort pass is out of scope: the final insertion sort alone must sort */ (void)Lg((int)(last - first)); FinalInsertionSort(first, last, Less()); }
}
static bool Sorted(const ENav* a, int n) { for (int k = 1; k < n; ++k) if (a[k - 1] > a[k]) return false; return true; }
static void Fill(ENav* a, int n, unsigned int seed) { for (int k = 0; k < n; ++k) { seed = seed * 1103515245u + 12345u; a[k] = (ENav)((seed >> 16) % 8); } a[0] = N0; }   // the minimum up front: the unguarded pass needs a guard (introsort guarantees one)
int main()
{
    ENav a[12] = { N0, N5, N3, N7, N1, N1, N6, N2, N4, N0, N2, N3 };   // a[0] is the minimum: the guard
    UnguardedInsertionSort(a + 1, a + 12, Less());
    bool ok = Sorted(a, 12) && a[0] == N0 && a[11] == N7;
    std::printf(ok ? "00672180_TEST PASS\n" : "00672180_TEST FAIL\n");
    return ok ? 0 : 1;
}