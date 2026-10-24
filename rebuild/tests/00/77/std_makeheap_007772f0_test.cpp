// Behaviour test for 0x007772f0 (STLport sort<long*, less<long> > family member)
#include <cstdio>
#include <cstring>

typedef int Elem;    // 32-bit signed (long in retail)
struct Less { bool operator()(const Elem& x, const Elem& y) const { return x < y; } };

static Elem* UnguardedPartition(Elem* first, Elem* last, Elem pivot, Less comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        Elem t = *first; *first = *last; *last = t;
        ++first;
    }
}
static void UnguardedLinearInsert(Elem* last, Elem val, Less comp)
{
    Elem* next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void UnguardedInsertionSortAux(Elem* first, Elem* last, Elem*, Less comp)
{
    for (Elem* i = first; i != last; ++i) UnguardedLinearInsert(i, *i, comp);
}
static void InsertionSort(Elem* first, Elem* last, Less comp)
{
    if (first == last) return;
    for (Elem* i = first + 1; i != last; ++i) {
        Elem val = *i;
        if (comp(val, *first)) {
            int n = (int)((char*)i - (char*)first);
            if (n > 0) std::memmove((char*)(i + 1) - n, first, n);
            *first = val;
        } else
            UnguardedLinearInsert(i, val, comp);
    }
}
static void PushHeap(Elem* first, int hole, int top, Elem val, Less comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void AdjustHeap(Elem* first, int hole, int len, Elem val, Less comp)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, val, comp);
}
static void MakeHeap(Elem* first, Elem* last, Less comp, Elem*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, first[parent], comp);
        if (parent == 0) return;
        parent--;
    }
}
static void SortHeap(Elem* first, Elem* last, Less comp)
{
    while (last - first > 1) { --last; Elem v = *last; *last = *first; AdjustHeap(first, 0, (int)(last - first), v, comp); }
}
static void PartialSort(Elem* first, Elem* middle, Elem* last, Elem*, Less comp)
{
    MakeHeap(first, middle, comp, 0, 0);
    for (Elem* i = middle; i < last; ++i)
        if (comp(*i, *first)) { Elem v = *i; *i = *first; AdjustHeap(first, 0, (int)(middle - first), v, comp); }
    SortHeap(first, middle, comp);
}
static const Elem& Median(const Elem& a, const Elem& b, const Elem& c, Less comp)
{
    if (comp(a, b))
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
static void IntrosortLoop(Elem* first, Elem* last, Elem*, int depth, Less comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last, last, 0, comp); return; }
        --depth;
        Elem* cut = UnguardedPartition(first, last, Elem(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, 0, depth, comp);
        last = cut;
    }
}
static unsigned int g_seed = 777;
static int Rand() { g_seed = g_seed * 1103515245u + 12345u; return (int)((g_seed >> 8) % 2001u) - 1000; }
static void Fill(Elem* v, int n) { for (int i = 0; i < n; ++i) v[i] = Rand(); }
static bool Sorted(const Elem* v, int n) { for (int i = 1; i < n; ++i) if (v[i - 1] > v[i]) return false; return true; }
static bool IsHeap(const Elem* v, int n) { for (int i = 1; i < n; ++i) if (v[(i - 1) / 2] < v[i]) return false; return true; }
static int Sum(const Elem* v, int n) { int s = 0; for (int i = 0; i < n; ++i) s += v[i] * (v[i] & 7); return s; }
int main()
{
    static Elem v[57];
    for (int r = 0; r < 10; ++r) {
        Fill(v, 57);
        int before = Sum(v, 57);
        MakeHeap(v, v + 57, Less(), 0, 0);
        if (!IsHeap(v, 57) || Sum(v, 57) != before) { std::printf("007772f0_TEST FAIL\n"); return 1; }
    }
    Elem one = 5;
    MakeHeap(&one, &one + 1, Less(), 0, 0);
    if (one != 5) { std::printf("007772f0_TEST FAIL\n"); return 1; }
    std::printf("007772f0_TEST PASS\n");
    return 0;
}