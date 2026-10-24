// behaviour test for 006fbd60: _Adjust_heap over pair<unsigned long, CSmallVector<CEvent,32>> by unsigned key (vector deep-copied by Copy, freed by delete[])
// The _Adjust_heap body below is the candidate's; make_heap/sort_heap are built on it and the result
// must come out ordered by the row's comparator with every element's payload intact.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const long kKeys[9] = { 5, -3, 12, 7, 7, 0, 99, -40, 2 };

template<class T, class C> void AdjustHeap(T* first, long hole, long bottom, T value, C pred);

struct CEvent { long m_a, m_b; };
struct CSmallVector {
    CEvent* Start;
    int Size;
    CSmallVector() : Start(0), Size(0) { }
    void Copy(const CSmallVector& other)
    {
        int i = 0;
        delete[] Start;
        Start = new CEvent[other.Size];
        for (i = 0; i < other.Size; ++i)
            Start[i] = other.Start[i];
        Size = other.Size;
    }
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    ~CSmallVector() { delete[] Start; }
};
struct T {
    unsigned long first;
    CSmallVector second;
    T() : first(0) { }
};
struct Pred {
    char m_tag;
    bool operator()(const T& a, const T& b) const { return a.first < b.first; }
};
static void InitPred(Pred& p) { p.m_tag = 0; }
static void Make(T& e, long i)
{
    CSmallVector v;
    int j = 0;
    v.Size = (int)(i % 3) + 1;
    v.Start = new CEvent[v.Size];
    for (j = 0; j < v.Size; ++j) { v.Start[j].m_a = kKeys[i]; v.Start[j].m_b = j; }
    e.first = (unsigned long)kKeys[i];
    e.second = v;
}
static long Sig(const T& e) { return (long)e.first * 7 + e.second.Size; }
static bool Ok(const T& e) { int j = 0; if (e.second.Size < 1) return false; for (j = 0; j < e.second.Size; ++j) { if (e.second.Start[j].m_a != (long)e.first || e.second.Start[j].m_b != j) return false; } return true; }
static bool Extra(const T* a) { return a[0].first == 0 && a[8].first == (unsigned long)-3; }

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
        std::printf("006fbd60_TEST FAIL\n");
        return 1;
    }
    std::printf("006fbd60_TEST PASS\n");
    return 0;
}