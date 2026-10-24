// behaviour test for 006fcb90: Dinkumware _Sort_heap over (unsigned long, CSmallVector<CEvent,32>) pairs; event blocks must follow and balance
#include <cstdio>
#include <cstring>
#include <cstdlib>
namespace NAnimationEvents { struct CEvent { long m_a, m_b; }; }
static int g_blocks = 0;
template<class T, int N> struct CSmallVector {
    T*  Start;
    int Size;
    CSmallVector() : Start(0), Size(0) { }
    void Copy(const CSmallVector& other)
    {
        Clear();
        if (other.Size) {
            Start = new T[other.Size];
            ++g_blocks;
            Size = other.Size;
            for (int i = 0; i < Size; ++i) Start[i] = other.Start[i];
        }
    }
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other) { if (&other != this) Copy(other); return *this; }
    void Clear() { if (Start) --g_blocks; delete[] Start; Start = 0; Size = 0; }
    ~CSmallVector() { Clear(); }
};
template<class K, class V> struct pair {
    K first;
    V second;
    pair() : first() { }
};
typedef pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedEvents;
struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SKeyedEvents& a, const SKeyedEvents& b) const { return a.first < b.first; }
};
typedef SKeyedEvents ElemT;
typedef CKeyPairCompareLess CompT;
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    int count = k % 3 + 1;
    e.first = (unsigned long)k;
    e.second.Clear();
    e.second.Start = new NAnimationEvents::CEvent[count];
    ++g_blocks;
    e.second.Size = count;
    for (int i = 0; i < count; ++i) { e.second.Start[i].m_a = k; e.second.Start[i].m_b = k * 2 + i; }
}
static int KeyOf(const ElemT& e) { return int(e.first); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    if (e.second.Size != k % 3 + 1) return false;
    for (int i = 0; i < e.second.Size; ++i) if (e.second.Start[i].m_a != k || e.second.Start[i].m_b != k * 2 + i) return false;
    return true;
}
static bool Live(int n) { return g_blocks == n; }
static bool Final() { return g_blocks == 0; }

static void PushHeapT(ElemT* first, long hole, long top, ElemT value, CompT comp)
{
    long parent = (hole - 1) / 2;
    while (top < hole && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeapT(ElemT* first, long hole, long len, ElemT value, CompT comp)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < len; idx = 2 * idx + 2) {
        if (comp(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx];
        hole = idx;
    }
    if (idx == len) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeapT(first, hole, top, value, comp);
}

// the out-of-line callee (_Adjust_heap in retail)
static void _Adjust_heap(ElemT* first, long hole, long bottom, ElemT value, CompT pred)
{
    AdjustHeapT(first, hole, bottom, value, pred);
}
// the reconstructed body: Dinkumware _Sort_heap with _Pop_heap_0 / _Pop_heap inline
inline void _Pop_heap(ElemT* first, ElemT* last, ElemT* dest, ElemT value, CompT pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}
inline void _Pop_heap_0(ElemT* first, ElemT* last, CompT pred, ElemT*)
{
    _Pop_heap(first, last - 1, last - 1, ElemT(*(last - 1)), pred, (long*)0);
}
static void _Sort_heap(ElemT* first, ElemT* last, CompT pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (ElemT*)0);
}
#define SORT(a, n, comp) _Sort_heap(a, a + n, comp)

int main()
{
    static const int keys[9] = { 5, 31, 12, 7, 64, 0, 99, 40, 2 };
    const int n = 9;
    int ok = 1;
    {
        ElemT a[9];
        CompT comp = MakeComp();
        int i = 0;
        for (i = 0; i < n; ++i) Fill(a[i], keys[i]);
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        for (i = 0; i < n; ++i) if (!Check(a[i])) ok = 0;
        if (KeyOf(a[0]) != 0 || KeyOf(a[n - 1]) != 99) ok = 0;
        if (!Live(n)) ok = 0;
        // re-heap the sorted output and sort again; a one-element range is left as it is
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        SORT(a, 1, comp);
        if (KeyOf(a[0]) != 0 || !Check(a[0])) ok = 0;
        if (!Live(n)) ok = 0;
    }
    if (!Final()) ok = 0;
    if (!ok) { std::printf("006fcb90_TEST FAIL\n"); return 1; }
    std::printf("006fcb90_TEST PASS\n");
    return 0;
}