// behaviour test for 00a7f1a0: _Adjust_heap over 16-byte (unsigned __int64 key, dword) pairs by 64-bit key
// The _Adjust_heap body below is the candidate's; make_heap/sort_heap are built on it and the result
// must come out ordered by the row's comparator with every element's payload intact.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static const long kKeys[9] = { 5, -3, 12, 7, 7, 0, 99, -40, 2 };

template<class T, class C> void AdjustHeap(T* first, long hole, long bottom, T value, C pred);

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct T {
    u64 m_key;
    unsigned long m_value;
    T() : m_key(0), m_value(0) { }
    T(const T& other) : m_key(other.m_key), m_value(other.m_value) { }
};
struct Pred {
    char m_tag;
    bool operator()(const T& a, const T& b) const { return a.m_key < b.m_key; }
};
static const unsigned long kHigh[9] = { 1, 0, 1, 3, 0, 2, 0, 1, 2 };
static void InitPred(Pred& p) { p.m_tag = 0; }
static u64 KeyOf(long i) { return (((u64)kHigh[i]) << 32) | (u64)(unsigned int)kKeys[i]; }
static void Make(T& e, long i) { e.m_key = KeyOf(i); e.m_value = kHigh[i] * 10 + 5; }
static long Sig(const T& e) { return (long)(unsigned int)e.m_key + (long)(e.m_key >> 32) * 1000; }
static bool Ok(const T& e) { return e.m_value == (unsigned long)(e.m_key >> 32) * 10 + 5; }
static bool Extra(const T* a) { return (unsigned long)(a[0].m_key >> 32) == 0 && (unsigned int)a[0].m_key == 7 && (unsigned long)(a[8].m_key >> 32) == 3; }

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
        std::printf("00a7f1a0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a7f1a0_TEST PASS\n");
    return 0;
}