#include <cstdio>

// CCharString over a CBasicString<char> payload: null means empty; ordering as the landed 0x00429950 + inline operator<.
struct CBasicStringChar {
    const char* PData;
    bool operator<(const CBasicStringChar& other) const
    {
        const char* b = other.PData;
        const char* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0) return false;
            if (*a < *b) return true;
            if (*a > *b) return false;
            ++a;
            ++b;
        }
    }
};
static CBasicStringChar g_names[6] = { { "alpha" }, { "beta" }, { "beta2" }, { "gamma" }, { "a" }, { "zed" } };
class CCharString { public:
    CBasicStringChar* PStringData;
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData) return false;
        if (PStringData == 0) return true;
        if (other.PStringData == 0) return false;
        return *PStringData < *other.PStringData;
    }
};
struct SNamePair {
    CCharString first;
    CCharString second;
};
static bool PairLess(const SNamePair& x, const SNamePair& y)    // STLport pair operator<
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}
struct TEntry {                                                 // pair<pair<CCharString, CCharString>, CSoundBankLH*>
    SNamePair first;
    void*     second;
};
static bool Less(const TEntry& a, const TEntry& b) { return PairLess(a.first, b.first); }   // CKeyPairCompareLess
static TEntry MakeEntry(int k)
{
    TEntry e;
    int i = k % 7, j = (k / 7) % 7;
    e.first.first.PStringData = i == 6 ? 0 : &g_names[i];
    e.first.second.PStringData = j == 6 ? 0 : &g_names[j];
    e.second = 0;
    return e;
}

// STLport 4.6 _heap.c / _algo.c over TEntry with the CKeyPairCompareLess order (Less), as the sources reconstruct them.
static void __push_heap(TEntry* first, int holeIndex, int topIndex, TEntry val)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && Less(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
static void __adjust_heap(TEntry* first, int holeIndex, int len, TEntry val)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (Less(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val);
}
static void __make_heap(TEntry* first, TEntry* last)
{
    int len = (int)(last - first);
    if (len < 2) return;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, TEntry(*(first + parent)));
        if (parent == 0) return;
        parent--;
    }
}
static void __pop_heap(TEntry* first, TEntry* last, TEntry* result, TEntry val)
{
    *result = *first;
    __adjust_heap(first, 0, (int)(last - first), val);
}
static void __partial_sort(TEntry* first, TEntry* middle, TEntry* last)
{
    __make_heap(first, middle);
    for (TEntry* i = middle; i < last; ++i)
        if (Less(*i, *first))
            __pop_heap(first, middle, i, TEntry(*i));
    while (middle - first > 1) {                    // sort_heap
        --middle;
        __pop_heap(first, middle, middle, TEntry(*middle));
    }
}
static const TEntry& __median(const TEntry& a, const TEntry& b, const TEntry& c)
{
    if (Less(a, b))
        if (Less(b, c)) return b;
        else if (Less(a, c)) return c;
        else return a;
    else if (Less(a, c)) return a;
    else if (Less(b, c)) return c;
    else return b;
}
static void Swap(TEntry& a, TEntry& b) { TEntry tmp = a; a = b; b = tmp; }
static TEntry* __unguarded_partition(TEntry* first, TEntry* last, TEntry pivot)
{
    for (;;) {
        while (Less(*first, pivot))
            ++first;
        --last;
        while (Less(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        Swap(*first, *last);
        ++first;
    }
}
static void __introsort_loop(TEntry* first, TEntry* last, int depth_limit)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            __partial_sort(first, last, last);
            return;
        }
        --depth_limit;
        TEntry* cut = __unguarded_partition(first, last,
                                            TEntry(__median(*first, *(first + (last - first) / 2), *(last - 1))));
        __introsort_loop(cut, last, depth_limit);
        last = cut;
    }
}
static void __insertion_sort(TEntry* first, TEntry* last)
{
    if (first == last) return;
    for (TEntry* i = first + 1; i != last; ++i) {
        TEntry val = *i;
        TEntry* j = i;
        while (j != first && Less(val, *(j - 1))) {
            *j = *(j - 1);
            --j;
        }
        *j = val;
    }
}

static unsigned int g_seed = 12345;
static int Rand(int n) { g_seed = g_seed * 1103515245u + 12345u; return (int)((g_seed >> 16) % (unsigned int)n); }

static bool IsSorted(const TEntry* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (Less(a[i], a[i - 1])) return false;
    return true;
}
static bool IsHeap(const TEntry* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (Less(a[(i - 1) / 2], a[i])) return false;
    return true;
}
static int Fail()
{
    std::printf("00a40df0_TEST FAIL\n");
    return 1;
}

int main()
{
    const int N = 200;
    static TEntry data[N];
    for (int round = 0; round < 20; ++round) {
        for (int i = 0; i < N; ++i)
            data[i] = MakeEntry(Rand(50));
        TEntry pivot = __median(data[0], data[N / 2], data[N - 1]);
        TEntry* cut = __unguarded_partition(data, data + N, pivot);
        if (cut <= data || cut >= data + N) return Fail();
        for (TEntry* p = data; p != cut; ++p)
            if (Less(pivot, *p)) return Fail();      // left part: nothing above the pivot
        for (TEntry* q = cut; q != data + N; ++q)
            if (Less(*q, pivot)) return Fail();      // right part: nothing below the pivot
    }
    std::printf("00a40df0_TEST PASS\n");
    return 0;
}