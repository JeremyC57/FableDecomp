// Behaviour test for 0x0080B050 STLport __insertion_sort over CTargetPoint.
#include <cstdio>

// Deterministic generator for test inputs.
static unsigned int g_seed = 12345u;
static int NextValue() { g_seed = g_seed * 1103515245u + 12345u; return (int)((g_seed >> 16) % 1000u); }

class CThing { public: int id; };
struct STargetPoint { CThing* PThing; long Index; float X, Y, Z; long FrameRejected; };   // NPlayerTargeting::CTargetPoint
struct CCompare {                                           // stand-in for CThingCompare_BestForTargeting2: larger X first
    bool operator()(const STargetPoint& a, const STargetPoint& b) const { return a.X > b.X; }
};
typedef STargetPoint T;
static CThing g_things[64];
static void Fill(T* v, int n) { for (int i = 0; i < n; ++i) { g_things[i].id = i; v[i].PThing = &g_things[i]; v[i].Index = i; v[i].X = (float)NextValue(); v[i].Y = 0.0f; v[i].Z = 0.0f; v[i].FrameRejected = -i; } }
static bool Ordered(const T* v, int n) { CCompare c; for (int i = 1; i < n; ++i) if (c(v[i], v[i - 1])) return false; return true; }
static bool IsHeap(const T* v, int n) { CCompare c; for (int i = 1; i < n; ++i) if (c(v[(i - 1) / 2], v[i])) return false; return true; }
static float Sum(const T* v, int n) { float s = 0.0f; for (int i = 0; i < n; ++i) s += v[i].X; return s; }
static bool Consistent(const T* v, int n) { for (int i = 0; i < n; ++i) if (v[i].PThing->id != v[i].Index || v[i].FrameRejected != -v[i].Index) return false; return true; }

// STLport 4.x heap / sort algorithms (reference behaviour of the retail instantiations).
template <class I, class T, class C> void PushHeap(I first, int hole, int top, T val, C comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(*(first + parent), val)) { *(first + hole) = *(first + parent); hole = parent; parent = (hole - 1) / 2; }
    *(first + hole) = val;
}
template <class I, class T, class C> void AdjustHeap(I first, int hole, int len, T val, C comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(*(first + child), *(first + (child - 1)))) child--;
        *(first + hole) = *(first + child); hole = child; child = 2 * (child + 1);
    }
    if (child == len) { *(first + hole) = *(first + (child - 1)); hole = child - 1; }
    PushHeap(first, hole, top, val, comp);
}
template <class I, class T, class C> void MakeHeap(I first, I last, C comp, T*)
{
    if (last - first < 2) return;
    int len = (int)(last - first), parent = (len - 2) / 2;
    for (;;) { AdjustHeap(first, parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }
}
template <class I, class T, class C> void PopHeapAux(I first, I last, T*, C comp)
{
    T val = *(last - 1);
    *(last - 1) = *first;
    AdjustHeap(first, 0, (int)((last - 1) - first), val, comp);
}
template <class I, class T, class C> void SortHeap(I first, I last, C comp, T* t)
{
    while (last - first > 1) { PopHeapAux(first, last, t, comp); --last; }
}
template <class I, class T, class C> void PartialSort(I first, I middle, I last, T* t, C comp)
{
    MakeHeap(first, middle, comp, t);
    for (I i = middle; i < last; ++i)
        if (comp(*i, *first)) { T val = *i; *i = *first; AdjustHeap(first, 0, (int)(middle - first), val, comp); }
    SortHeap(first, middle, comp, t);
}
template <class T, class C> const T& Median(const T& a, const T& b, const T& c, C comp)
{
    if (comp(a, b)) { if (comp(b, c)) return b; else if (comp(a, c)) return c; else return a; }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}
template <class I, class T, class C> I UnguardedPartition(I first, I last, T pivot, C comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        T tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}
template <class I, class T, class C> void UnguardedLinearInsert(I last, T val, C comp)
{
    I next = last; --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class I, class T, class C> void LinearInsert(I first, I last, T val, C comp)
{
    if (comp(val, *first)) { for (I p = last; p != first; --p) *p = *(p - 1); *first = val; }
    else UnguardedLinearInsert(last, val, comp);
}
template <class I, class T, class C> void InsertionSort(I first, I last, C comp, T*)
{
    if (first == last) return;
    for (I i = first + 1; i != last; ++i) LinearInsert(first, i, T(*i), comp);
}
template <class I, class T, class C> void UnguardedInsertionSort(I first, I last, C comp, T*)
{
    for (I i = first; i != last; ++i) UnguardedLinearInsert(i, T(*i), comp);
}
template <class I, class T, class C> void FinalInsertionSort(I first, I last, C comp, T* t)
{
    if (last - first > 16) { InsertionSort(first, first + 16, comp, t); UnguardedInsertionSort(first + 16, last, comp, t); }
    else InsertionSort(first, last, comp, t);
}
template <class I, class T, class C> void IntrosortLoop(I first, I last, T* t, int depth, C comp)
{
    while (last - first > 16) {
        if (depth == 0) { PartialSort(first, last, last, t, comp); return; }
        --depth;
        I cut = UnguardedPartition(first, last, T(Median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        IntrosortLoop(cut, last, t, depth, comp);
        last = cut;
    }
}
inline int Lg(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
template <class I, class T, class C> void Sort(I first, I last, C comp, T* t)
{
    if (first != last) { IntrosortLoop(first, last, t, Lg((int)(last - first)) * 2, comp); FinalInsertionSort(first, last, comp, t); }
}

int main()
{
    bool ok = true;
    for (int n = 0; ok && n <= 30; n += 3) {
        T v[30]; Fill(v, n); float before = Sum(v, n);
        InsertionSort(v, v + n, CCompare(), (T*)0);
        ok = Ordered(v, n) && Sum(v, n) == before && Consistent(v, n);
    }
    if (!ok) { std::printf("0080b050_TEST FAIL\n"); return 1; }
    std::printf("0080b050_TEST PASS\n");
    return 0;
}