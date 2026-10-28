// Behaviour test for 00a85050: std::__introsort_loop over pair<unsigned __int64,list<C2DLineF> > with CKeyPairCompareLess (64-bit key compares, heap fallback at depth 0).
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void check(bool ok, const char* what) { if (!ok) { std::printf("check failed: %s\n", what); g_fail = 1; } }

static const int THRESH = 16;
template <class T, class L> static const T& median3(const T& a, const T& b, const T& c, L lt) {
    if (lt(a, b)) { if (lt(b, c)) return b; else if (lt(a, c)) return c; else return a; }
    else if (lt(a, c)) return a; else if (lt(b, c)) return c; else return b;
}
template <class T, class L> static T* upart(T* first, T* last, T pivot, L lt) {
    for (;;) {
        while (lt(*first, pivot)) ++first;
        --last;
        while (lt(pivot, *last)) --last;
        if (!(first < last)) return first;
        T t = *first; *first = *last; *last = t; ++first;
    }
}
template <class T, class L> static void sift(T* b, int hole, int len, T v, L lt) {
    int top = hole, c = 2 * hole + 2;
    while (c < len) { if (lt(b[c], b[c - 1])) c--; b[hole] = b[c]; hole = c; c = 2 * c + 2; }
    if (c == len) { b[hole] = b[c - 1]; hole = c - 1; }
    int p = (hole - 1) / 2;
    while (hole > top && lt(b[p], v)) { b[hole] = b[p]; hole = p; p = (hole - 1) / 2; }
    b[hole] = v;
}
template <class T, class L> static void heapsort(T* f, T* l, L lt) {
    int n = (int)(l - f);
    for (int i = (n - 2) / 2; i >= 0; --i) sift(f, i, n, f[i], lt);
    for (; n > 1; --n) { T v = f[n - 1]; f[n - 1] = f[0]; sift(f, 0, n - 1, v, lt); }
}
template <class T, class L> static void introsort_loop(T* first, T* last, int depth, L lt) {
    while (last - first > THRESH) {
        if (depth == 0) { heapsort(first, last, lt); return; }
        --depth;
        T* cut = upart(first, last, median3(*first, *(first + (last - first) / 2), *(last - 1), lt), lt);
        introsort_loop(cut, last, depth, lt);
        last = cut;
    }
}
template <class T, class L> static void isort(T* f, T* l, L lt) {
    for (T* i = f + 1; i < l; ++i) { T v = *i; T* j = i; while (j != f && lt(v, *(j - 1))) { *j = *(j - 1); --j; } *j = v; }
}
static unsigned g_seed = 12345u;
static unsigned rnd() { g_seed = g_seed * 1103515245u + 12345u; return (g_seed >> 8) & 0xffff; }

struct Pair { u64 first; int second; };   // pair<unsigned __int64, list<C2DLineF> >
struct Less { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };   // CKeyPairCompareLess
static Pair mk(int i) { Pair p; p.first = ((u64)(rnd() % 50) << 32) | rnd(); p.second = i; return p; }
int main() {
    for (int round = 0; round < 3; ++round) {
        const int N = 200;
        Pair data[N]; unsigned sum = 0;
        for (int i = 0; i < N; ++i) { data[i] = mk(i); sum += (unsigned)data[i].second; }
        introsort_loop(data, data + N, round == 2 ? 0 : 2 * 7, Less());   // depth 0 exercises the heap fallback
        // after the loop every element sits at most THRESH-1 places from a block holding only keys <= the next block
        bool blocks = true;
        for (int i = 0; i < N; ++i) for (int j = i + THRESH; j < N; ++j) if (Less()(data[j], data[i])) blocks = false;
        check(blocks, "loop leaves only short unsorted runs");
        isort(data, data + N, Less());
        bool sorted = true; unsigned sum2 = 0;
        for (int i = 0; i < N; ++i) { sum2 += (unsigned)data[i].second; if (i && Less()(data[i], data[i - 1])) sorted = false; }
        check(sorted && sum == sum2, "sorted, same elements");
    }
    if (g_fail) { std::printf("00a85050_TEST FAIL\n"); return 1; }
    std::printf("00a85050_TEST PASS\n");
    return 0;
}