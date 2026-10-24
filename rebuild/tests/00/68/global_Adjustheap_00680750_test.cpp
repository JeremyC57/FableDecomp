// behaviour test for 00680750: _Adjust_heap over (long key, dword value) pairs, driven through
// make_heap/sort_heap built on it; the result must come out sorted by key with values attached.
#include <cstdio>
struct SPair {
    long m_first;
    unsigned long m_second;
    SPair() : m_first(0), m_second(0) { }
    SPair(const SPair& other) : m_first(other.m_first), m_second(other.m_second) { }
};
struct SLess {
    char m_tag;
    bool operator()(const SPair& a, const SPair& b) const { return a.m_first < b.m_first; }
};
static void PushHeap(SPair* first, long hole, long top, SPair value, SLess pred)
{
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeap(SPair* first, long hole, long bottom, SPair value, SLess pred)
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
int main()
{
    static const long keys[9] = { 5, -3, 12, 7, 7, 0, 99, -40, 2 };
    SPair a[9];
    SLess pred;
    pred.m_tag = 0;
    long i = 0;
    long n = 9;
    long last = 0;
    for (i = 0; i < n; ++i) { a[i].m_first = keys[i]; a[i].m_second = (unsigned long)(keys[i] * 3 + 100); }
    for (i = n / 2; 0 < i; ) { --i; AdjustHeap(a, i, n, a[i], pred); }
    for (last = n; 1 < last; --last) {
        SPair value = a[last - 1];
        a[last - 1] = a[0];
        AdjustHeap(a, 0, last - 1, value, pred);
    }
    int ok = 1;
    for (i = 1; i < n; ++i) if (a[i - 1].m_first > a[i].m_first) ok = 0;
    for (i = 0; i < n; ++i) if (a[i].m_second != (unsigned long)(a[i].m_first * 3 + 100)) ok = 0;
    if (a[0].m_first != -40 || a[8].m_first != 99) ok = 0;
    if (!ok) { std::printf("00680750_TEST FAIL\n"); return 1; }
    std::printf("00680750_TEST PASS\n");
    return 0;
}