// behaviour test for 005e2860: SGI sort_heap over (EAnimComponent, CIVCountedPointer<CAnimComponentBase>) pairs; reference counts must balance
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_destroyed = 0;
class CAnimComponentBase { public:                       // intrusively counted: vptr, count at +4
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }   // slot 1
    long m_count;
    long m_id;
    CAnimComponentBase() : m_count(0), m_id(0) { }
    virtual ~CAnimComponentBase() { }
};
struct CIVCountedPointer {
    CAnimComponentBase* m_p;
    CIVCountedPointer() : m_p(0) { }
    CIVCountedPointer(const CIVCountedPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CIVCountedPointer() { Release(); }
    void Release() { if (m_p) { if (--m_p->m_count == 0) m_p->Destroy(); m_p = 0; } }
    CIVCountedPointer& operator=(const CIVCountedPointer& o)
    {
        if (m_p != o.m_p) { Release(); m_p = o.m_p; if (m_p) ++m_p->m_count; }
        return *this;
    }
    void Attach(CAnimComponentBase* p) { Release(); m_p = p; ++p->m_count; }
};
static CAnimComponentBase g_objects[100];

struct SAnimComponentPair {
    long first;
    CIVCountedPointer second;
    SAnimComponentPair() : first(0) { }
};
struct CKeyPairCompareLess {
    bool operator()(const SAnimComponentPair& a, const SAnimComponentPair& b) const { return a.first < b.first; }
};
typedef SAnimComponentPair ElemT;
typedef CKeyPairCompareLess CompT;
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    e.first = k;
    g_objects[k].m_id = k;
    e.second.Attach(&g_objects[k]);
}
static int KeyOf(const ElemT& e) { return int(e.first); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    return e.second.m_p == &g_objects[k] && e.second.m_p->m_count == 1;
}
static bool Live(int n) { return g_destroyed == 0; }
static bool Final() { return g_destroyed == 9; }

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

// the out-of-line callee (__pop_heap in retail), modelled with the standard hole algorithm
static void __pop_heap(ElemT* first, ElemT* last, ElemT* result, ElemT value, CompT comp, int*)
{
    *result = *first;
    AdjustHeapT(first, 0, long(last - first), value, comp);
}
// the reconstructed body: SGI sort_heap with pop_heap / __pop_heap_aux inline
inline void __pop_heap_aux(ElemT* first, ElemT* last, ElemT*, CompT comp)
{
    __pop_heap(first, last - 1, last - 1, ElemT(*(last - 1)), comp, (int*)0);
}
inline void pop_heap(ElemT* first, ElemT* last, CompT comp)
{
    __pop_heap_aux(first, last, (ElemT*)0, comp);
}
static void sort_heap(ElemT* first, ElemT* last, CompT comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}
#define SORT(a, n, comp) sort_heap(a, a + n, comp)

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
    if (!ok) { std::printf("005e2860_TEST FAIL\n"); return 1; }
    std::printf("005e2860_TEST PASS\n");
    return 0;
}