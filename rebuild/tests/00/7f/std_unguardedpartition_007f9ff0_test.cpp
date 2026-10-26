#include <cstdio>
// Behaviour test: std::__unguarded_partition over the targeting comparator -- everything left of the cut ranks at least as well as the pivot, everything right of it no better.
#define CHECK(c) if (!(c)) { std::printf("007f9ff0_TEST FAIL\n"); return 1; }
// Test model: candidates carry a compare value; the comparator ranks a before b when a's value is greater (the inlined
// CThingCompare_BestForTargeting2::operator() test `valueA > valueB`), so the SGI algorithms order best first.
class CThing { public: float Value; };
struct CThingCompare { int calls; CThingCompare() : calls(0) { } bool operator()(CThing* a, CThing* b) { ++calls; return a->Value > b->Value; } };
static void IterSwap(CThing** a, CThing** b) { CThing* t = *a; *a = *b; *b = t; }
static CThing** UnguardedPartition(CThing** first, CThing** last, CThing* pivot, CThingCompare comp)
{
    while (true) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        IterSwap(first, last);
        ++first;
    }
}
static void UnguardedLinearInsert(CThing** last, CThing* val, CThingCompare comp)
{
    CThing** next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void LinearInsert(CThing** first, CThing** last, CThing* val, CThingCompare comp)
{
    if (comp(val, *first)) { for (CThing** p = last; p != first; --p) *p = *(p - 1); *first = val; }
    else UnguardedLinearInsert(last, val, comp);
}
static void InsertionSort(CThing** first, CThing** last, CThingCompare comp)
{
    if (first == last) return;
    for (CThing** i = first + 1; i != last; ++i) LinearInsert(first, i, *i, comp);
}
static void UnguardedInsertionSort(CThing** first, CThing** last, CThingCompare comp)
{
    for (CThing** i = first; i != last; ++i) UnguardedLinearInsert(i, *i, comp);
}
static void FinalInsertionSort(CThing** first, CThing** last, CThingCompare comp)
{
    if (last - first > 16) { InsertionSort(first, first + 16, comp); UnguardedInsertionSort(first + 16, last, comp); }
    else InsertionSort(first, last, comp);
}
static void PushHeap(CThing** first, int holeIndex, int topIndex, CThing* value, CThingCompare comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent); holeIndex = parent; parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}
static void AdjustHeap(CThing** first, int holeIndex, int len, CThing* value, CThingCompare comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1)))) secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { *(first + holeIndex) = *(first + (secondChild - 1)); holeIndex = secondChild - 1; }
    PushHeap(first, holeIndex, topIndex, value, comp);
}
static void MakeHeapImpl(CThing** first, CThing** last, CThingCompare comp, CThing**, int*)
{
    if (last - first < 2) return;
    int len = last - first;
    int parent = (len - 2) / 2;
    while (true) {
        AdjustHeap(first, parent, len, *(first + parent), comp);
        if (parent == 0) return;
        parent--;
    }
}
static void MakeHeap(CThing** first, CThing** last, CThingCompare comp) { MakeHeapImpl(first, last, comp, (CThing**)0, (int*)0); }
static void PopHeap(CThing** first, CThing** last, CThing** result, CThing* value, CThingCompare comp, int*)
{
    *result = *first;
    AdjustHeap(first, 0, last - first, value, comp);
}
static void PopHeapAux(CThing** first, CThing** last, CThing**, CThingCompare comp)
{
    PopHeap(first, last - 1, last - 1, *(last - 1), comp, (int*)0);
}
static void PartialSortImpl(CThing** first, CThing** middle, CThing** last, CThing**, CThingCompare comp)
{
    MakeHeap(first, middle, comp);
    for (CThing** i = middle; i < last; ++i)
        if (comp(*i, *first)) PopHeap(first, middle, i, *i, comp, (int*)0);
    while (middle - first > 1) { PopHeapAux(first, middle, (CThing**)0, comp); --middle; }
}
static void PartialSort(CThing** first, CThing** middle, CThing** last, CThingCompare comp) { PartialSortImpl(first, middle, last, (CThing**)0, comp); }
static CThing* Median(CThing* a, CThing* b, CThing* c, CThingCompare comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a; else if (comp(b, c)) return c; else return b;
}
static void IntrosortLoop(CThing** first, CThing** last, CThing**, int depth, CThingCompare comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last, last, comp); return; }
        --depth;
        CThing** cut = UnguardedPartition(first, last, Median(*first, *(first + (last - first) / 2), *(last - 1), comp), comp);
        IntrosortLoop(cut, last, (CThing**)0, depth, comp);
        last = cut;
    }
}
static int Lg(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
static void Sort(CThing** first, CThing** last, CThingCompare comp)
{
    if (first != last) { IntrosortLoop(first, last, (CThing**)0, Lg(last - first) * 2, comp); FinalInsertionSort(first, last, comp); }
}
static CThing g_things[40];
static CThing* g_ptrs[40];
static void Fill(int n)
{
    unsigned long seed = 12345;
    for (int i = 0; i < n; ++i) { seed = seed * 1103515245UL + 12345UL; g_things[i].Value = (float)((seed >> 8) % 1000); g_ptrs[i] = &g_things[i]; }
}
static bool SortedBestFirst(CThing** first, CThing** last)
{
    for (CThing** p = first; p + 1 < last; ++p) if (p[0]->Value < p[1]->Value) return false;
    return true;
}
static bool IsHeap(CThing** first, int n)
{
    for (int i = 1; i < n; ++i) if (first[(i - 1) / 2]->Value > first[i]->Value) return false;   // root = worst-ranked (smallest value)
    return true;
}
static float Sum(int n) { float s = 0.0f; for (int i = 0; i < n; ++i) s += g_ptrs[i]->Value; return s; }
int main()
{
    Fill(30);
    CThing pivot; pivot.Value = 500.0f;
    CThing* lo = &g_things[0]; lo->Value = 999.0f; CThing* hi = &g_things[29]; hi->Value = 0.0f;   // sentinels both ends
    float before = Sum(30);
    CThing** cut = UnguardedPartition(g_ptrs, g_ptrs + 30, &pivot, CThingCompare());
    CHECK(cut > g_ptrs && cut < g_ptrs + 30)
    for (CThing** p = g_ptrs; p < cut; ++p) CHECK((*p)->Value >= 500.0f)
    for (CThing** p = cut; p < g_ptrs + 30; ++p) CHECK((*p)->Value <= 500.0f)
    CHECK(Sum(30) == before)
    std::printf("007f9ff0_TEST PASS\n");
    return 0;
}