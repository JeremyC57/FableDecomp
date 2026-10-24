// Behaviour test for 0x00671BB0 std::adjacent_find (model copy)
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
static ENav* AdjacentFind(ENav* first, ENav* last)
{
    if (first == last) return last;
    ENav* next = first;
    while (++next != last) { if (*first == *next) return first; first = next; }
    return last;
}
int main()
{
    ENav a[6] = { N0, N1, N2, N2, N3, N3 };
    ENav b[3] = { N0, N1, N0 };
    bool ok = AdjacentFind(a, a + 6) == a + 2 && AdjacentFind(b, b + 3) == b + 3 && AdjacentFind(a, a) == a && AdjacentFind(a, a + 1) == a + 1;
    std::printf(ok ? "00671bb0_TEST PASS\n" : "00671bb0_TEST FAIL\n");
    return ok ? 0 : 1;
}