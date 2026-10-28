#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif

// Model of the STLport sort instantiation over pair<CCharString, C3DVector> ordered by name (CKeyPairCompareLess).
struct CBasicStringChar {
    char* PData;
    long Compare(const CBasicStringChar& other) const
    {
        const char* b = other.PData;
        const char* a = PData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicStringChar& other) const { return Compare(other) < 0; }
};

static int g_liveStrings = 0;

class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString(CBasicStringChar* p) : PStringData(p) { ++g_liveStrings; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_liveStrings; }
    ~CCharString() { --g_liveStrings; }
    const CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (!PStringData)
            return true;
        if (!other.PStringData)
            return false;
        return *PStringData < *other.PStringData;
    }
};

class C3DVector { public:
    unsigned int X, Y, Z;
    C3DVector(unsigned int x, unsigned int y, unsigned int z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(0), second(0, 0, 0) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<CCharString, C3DVector> SNamedPosition;

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<CCharString, C3DVector> CNamedPositionLess;

// ---- the instantiation's other members (reference STLport bodies) ----
static void __fastcall iter_swap(SNamedPosition* a, SNamedPosition* b)
{
    SNamedPosition tmp = *a;
    *a = *b;
    *b = tmp;
}

static SNamedPosition* __fastcall __copy_backward(SNamedPosition* first, SNamedPosition* last, SNamedPosition* result)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static const SNamedPosition& __fastcall __median(const SNamedPosition& a, const SNamedPosition& b,
                                                 const SNamedPosition& c, CNamedPositionLess comp)
{
    if (comp(a, b)) {
        if (comp(b, c)) return b;
        else if (comp(a, c)) return c;
        else return a;
    }
    else if (comp(a, c)) return a;
    else if (comp(b, c)) return c;
    else return b;
}

static void __fastcall __push_heap(SNamedPosition* first, int hole, int top, SNamedPosition value, CNamedPositionLess comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}

static void __fastcall __adjust_heap(SNamedPosition* first, int hole, int len, SNamedPosition value, CNamedPositionLess comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1]))
            --second;
        first[hole] = first[second];
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) {
        first[hole] = first[second - 1];
        hole = second - 1;
    }
    __push_heap(first, hole, top, value, comp);
}

static void __fastcall __pop_heap(SNamedPosition* first, SNamedPosition* last, SNamedPosition* result,
                                  SNamedPosition val, CNamedPositionLess comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, int(last - first), val, comp);
}

static void __fastcall sort_heap(SNamedPosition* first, SNamedPosition* last, CNamedPositionLess comp)
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, SNamedPosition(*last), comp, (int*)0);
    }
}

// ---- test helpers ----
static CBasicStringChar g_names[64];
static char g_text[64][8];

static CBasicStringChar* Name(int i)
{
    if (i < 0)
        return 0;                          // null payload sorts first
    std::sprintf(g_text[i], "n%02d", i);
    g_names[i].PData = g_text[i];
    return &g_names[i];
}

static int Key(const SNamedPosition& e)
{
    if (!e.first.PStringData)
        return -1;
    return (int)(e.first.PStringData - g_names);
}

static bool CheckPayload(const SNamedPosition* a, int n)
{
    for (int i = 0; i < n; ++i)            // every entry carries its own key in the vector payload
        if ((int)a[i].second.X != Key(a[i]) || a[i].second.Y != a[i].second.X + 1 || a[i].second.Z != a[i].second.X + 2)
            return false;
    return true;
}

static bool IsSorted(const SNamedPosition* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (Key(a[i]) < Key(a[i - 1]))
            return false;
    return true;
}

static SNamedPosition Entry(int key)
{
    unsigned int k = (unsigned int)key;
    return SNamedPosition(CCharString(Name(key)), C3DVector(k, k + 1, k + 2));
}

// ---- the six bodies of chunk 395 (copies of the reconstructed sources) ----
static void __fastcall __unguarded_linear_insert(SNamedPosition* last, SNamedPosition val, CNamedPositionLess comp)
{
    SNamedPosition* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

static SNamedPosition* __fastcall __unguarded_partition(SNamedPosition* first, SNamedPosition* last, SNamedPosition pivot,
                                                        CNamedPositionLess comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}

static void __fastcall __linear_insert(SNamedPosition* first, SNamedPosition* last, SNamedPosition val, CNamedPositionLess comp)
{
    if (comp(val, *first)) {
        __copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

static void __fastcall __make_heap(SNamedPosition* first, SNamedPosition* last, CNamedPositionLess comp, SNamedPosition*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, SNamedPosition(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

static void make_heap(SNamedPosition* first, SNamedPosition* last, CNamedPositionLess comp)
{
    __make_heap(first, last, comp, (SNamedPosition*)0, (int*)0);
}

static void __fastcall __partial_sort(SNamedPosition* first, SNamedPosition* middle, SNamedPosition* last, SNamedPosition*,
                                      CNamedPositionLess comp)
{
    make_heap(first, middle, comp);
    for (SNamedPosition* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, SNamedPosition(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

static void partial_sort(SNamedPosition* first, SNamedPosition* middle, SNamedPosition* last, CNamedPositionLess comp)
{
    __partial_sort(first, middle, last, (SNamedPosition*)0, comp);
}

static void __fastcall __introsort_loop(SNamedPosition* first, SNamedPosition* last, SNamedPosition*, int depth_limit,
                                        CNamedPositionLess comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        SNamedPosition* cut = __unguarded_partition(first, last,
                                                    SNamedPosition(__median(*first, *(first + (last - first) / 2),
                                                                            *(last - 1), comp)),
                                                    comp);
        __introsort_loop(cut, last, (SNamedPosition*)0, depth_limit, comp);
        last = cut;
    }
}

static int Fail(const char* addr)
{
    std::printf("%s_TEST FAIL\n", addr);
    return 1;
}

static int Pass(const char* addr)
{
    std::printf("%s_TEST PASS\n", addr);
    return 0;
}

// a fixed pseudo-random permutation of 0..n-1 (and -1 for a null name at slot `nullAt`)
static void Fill(SNamedPosition* a, int n, int seed, int nullAt)
{
    for (int i = 0; i < n; ++i)
        a[i] = Entry(i);
    unsigned int r = (unsigned int)seed;
    for (int i = n - 1; i > 0; --i) {
        r = r * 1103515245u + 12345u;
        int j = (int)((r >> 8) % (unsigned int)(i + 1));
        SNamedPosition t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    if (nullAt >= 0)
        a[nullAt] = Entry(-1);
}

int main()
{
    for (int n = 0; n < 30; ++n) {
        SNamedPosition a[30];
        Fill(a, n, n * 7 + 3, n > 4 ? 2 : -1);
        int live = g_liveStrings;
        __make_heap(a, a + n, CNamedPositionLess(), (SNamedPosition*)0, (int*)0);
        if (g_liveStrings != live || !CheckPayload(a, n))
            return Fail("0077eda0");
        for (int i = 1; i < n; ++i)        // max-heap on the name order
            if (Key(a[(i - 1) / 2]) < Key(a[i]))
                return Fail("0077eda0");
    }
    return Pass("0077eda0");
}