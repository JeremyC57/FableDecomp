// behaviour test for 00a81e80: Dinkumware _Sort_heap over (unsigned __int64, unsigned long) records
#include <cstdio>
#include <cstring>
#include <cstdlib>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct SKeyedValue {
    u64           m_key;
    unsigned long m_value;
    SKeyedValue() : m_key(0), m_value(0) { }
    SKeyedValue(const SKeyedValue& other) : m_key(other.m_key), m_value(other.m_value) { }
};
struct SKeyLess {
    char m_tag;
    bool operator()(const SKeyedValue& a, const SKeyedValue& b) const { return a.m_key < b.m_key; }
};
typedef SKeyedValue ElemT;
typedef SKeyLess CompT;
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    e.m_key = ((u64)k << 36) + 9u;
    e.m_value = (unsigned long)(k * 4 + 3);
}
static int KeyOf(const ElemT& e) { return int(e.m_key >> 36); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    return (unsigned)(e.m_key & 0xffffffffu) == 9u && e.m_value == (unsigned long)(k * 4 + 3);
}
static bool Live(int) { return true; }
static bool Final() { return true; }

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
    if (!ok) { std::printf("00a81e80_TEST FAIL\n"); return 1; }
    std::printf("00a81e80_TEST PASS\n");
    return 0;
}