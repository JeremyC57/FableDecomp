// behaviour test for 00593e3e: _Adjust_heap over (priority, CDefPointer) pairs with out-of-line pair copy/assign/destroy and an out-of-line priority comparator
// The _Adjust_heap body below is the candidate's; make_heap/sort_heap are built on it and the result
// must come out ordered by the row's comparator with every element's payload intact.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const long kKeys[9] = { 5, -3, 12, 7, 7, 0, 99, -40, 2 };

template<class T, class C> void AdjustHeap(T* first, long hole, long bottom, T value, C pred);

class CContextSensitiveItemDef { public: long m_refCount; long m_priority; };
static CContextSensitiveItemDef g_defs[9];
template<class U> class CDefPointer { public: U* m_pDef; };
struct T {
    long first;
    CDefPointer<CContextSensitiveItemDef> second;
    T() : first(0) { second.m_pDef = 0; }
    T(const T& other) : first(other.first) { second = other.second; if (second.m_pDef) ++second.m_pDef->m_refCount; }
    T& operator=(const T& other)
    {
        if (other.second.m_pDef) ++other.second.m_pDef->m_refCount;
        if (second.m_pDef) --second.m_pDef->m_refCount;
        first = other.first;
        second = other.second;
        return *this;
    }
    ~T() { if (second.m_pDef) --second.m_pDef->m_refCount; }
};
struct Pred {
    char m_tag;
    bool operator()(const T& a, const T& b) const { return a.first < b.first; }
};
static void InitPred(Pred& p) { p.m_tag = 0; }
static void Make(T& e, long i) { T v; g_defs[i].m_refCount = 0; g_defs[i].m_priority = kKeys[i]; v.first = kKeys[i]; v.second.m_pDef = &g_defs[i]; ++g_defs[i].m_refCount; e = v; }
static long Sig(const T& e) { return e.first * 7 + e.second.m_pDef->m_priority; }
static bool Ok(const T& e) { return e.second.m_pDef != 0 && e.second.m_pDef->m_priority == e.first && e.second.m_pDef->m_refCount == 1; }
static bool Extra(const T* a) { return a[0].first == -40 && a[8].first == 99; }

template<class T, class C> void PushHeap(T* first, long hole, long top, T value, C pred)
{
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}

template<class T, class C> void AdjustHeap(T* first, long hole, long bottom, T value, C pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeap(first, hole, top, value, pred);
}

static int RunTest()
{
    long n = 9;
    long i = 0;
    long last = 0;
    long sigBefore = 0;
    long sigAfter = 0;
    int ok = 1;
    T* a = new T[9];
    Pred pred;
    InitPred(pred);
    for (i = 0; i < n; ++i)
        Make(a[i], i);
    for (i = 0; i < n; ++i)
        sigBefore += Sig(a[i]);
    for (i = n / 2; 0 < i; ) {
        --i;
        T v(a[i]);
        AdjustHeap(a, i, n, v, pred);
    }
    for (last = n; 1 < last; --last) {
        T v(a[last - 1]);
        a[last - 1] = a[0];
        AdjustHeap(a, 0, last - 1, v, pred);
    }
    for (i = 1; i < n; ++i) {
        if (pred(a[i], a[i - 1]))
            ok = 0;
    }
    for (i = 0; i < n; ++i) {
        sigAfter += Sig(a[i]);
        if (!Ok(a[i]))
            ok = 0;
    }
    if (sigBefore != sigAfter)
        ok = 0;
    if (!Extra(a))
        ok = 0;
    delete[] a;
    return ok;
}

int main()
{
    if (!RunTest()) {
        std::printf("00593e3e_TEST FAIL\n");
        return 1;
    }
    std::printf("00593e3e_TEST PASS\n");
    return 0;
}