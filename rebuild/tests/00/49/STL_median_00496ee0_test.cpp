#include <cstdio>
#include <cstdlib>

// Behaviour model of the STLport sort instantiation over CWideString with the case-insensitive comparator
// (NGameComponentHelpers::CCaseInsensitiveStringCompare: _wcsicmp(a, b) < 0). The 4-byte string is modelled as a
// pointer to a narrow string; Wcsicmp stands in for the CRT _wcsicmp.
static int Wcsicmp(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int ca = *a, cb = *b;
        if (ca >= 'A' && ca <= 'Z') ca += 'a' - 'A';
        if (cb >= 'A' && cb <= 'Z') cb += 'a' - 'A';
        if (ca != cb) return ca - cb;
        if (ca == 0) return 0;
    }
}

struct CWideString {
    const char* PStringData;
    CWideString() : PStringData("") {}
    CWideString(const char* s) : PStringData(s) {}
    CWideString(const CWideString& other) : PStringData(other.PStringData) {}
    ~CWideString() {}
    CWideString& operator=(const CWideString& other) { PStringData = other.PStringData; return *this; }
    operator const char*() const { return PStringData; }
};

struct TCompare {
    char m_tag;
    TCompare() : m_tag(0) {}
    bool operator()(const CWideString& a, const CWideString& b) const { return Wcsicmp(a, b) < 0; }
};

static const CWideString& __median(const CWideString& a, const CWideString& b, const CWideString& c, TCompare comp)
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

static CWideString* __unguarded_partition(CWideString* first, CWideString* last, CWideString pivot, TCompare comp)
{
    for (;;) {
        while (comp(*first, pivot)) ++first;
        --last;
        while (comp(pivot, *last)) --last;
        if (!(first < last)) return first;
        CWideString tmp = *first; *first = *last; *last = tmp;
        ++first;
    }
}

static void __unguarded_linear_insert(CWideString* last, CWideString val, TCompare comp)
{
    CWideString* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}

static CWideString* copy_backward(CWideString* first, CWideString* last, CWideString* result)
{
    while (last != first) *--result = *--last;
    return result;
}

static void __linear_insert(CWideString* first, CWideString* last, CWideString val, TCompare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

static void __insertion_sort(CWideString* first, CWideString* last, TCompare comp)
{
    if (first == last) return;
    for (CWideString* i = first + 1; i != last; ++i)
        __linear_insert(first, i, CWideString(*i), comp);
}

static void __unguarded_insertion_sort(CWideString* first, CWideString* last, TCompare comp)
{
    for (CWideString* i = first; i != last; ++i)
        __unguarded_linear_insert(i, CWideString(*i), comp);
}

static void __push_heap(CWideString* first, int holeIndex, int topIndex, CWideString val, TCompare comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}

static void __adjust_heap(CWideString* first, int holeIndex, int len, CWideString val, TCompare comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1)))) secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}

static void __make_heap(CWideString* first, CWideString* last, TCompare comp, CWideString*, int*)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, CWideString(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}

static void __pop_heap(CWideString* first, CWideString* last, CWideString* result, CWideString val, TCompare comp, int*)
{
    *result = *first;
    __adjust_heap(first, 0, int(last - first), val, comp);
}

static void sort_heap(CWideString* first, CWideString* last, TCompare comp)
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, CWideString(*last), comp, (int*)0);
    }
}

static void __partial_sort(CWideString* first, CWideString* middle, CWideString* last, CWideString*, TCompare comp)
{
    __make_heap(first, middle, comp, (CWideString*)0, (int*)0);
    for (CWideString* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, CWideString(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

static void __introsort_loop(CWideString* first, CWideString* last, CWideString*, int depth_limit, TCompare comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            __partial_sort(first, last, last, (CWideString*)0, comp);
            return;
        }
        --depth_limit;
        CWideString* cut = __unguarded_partition(first, last,
            CWideString(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)), comp);
        __introsort_loop(cut, last, (CWideString*)0, depth_limit, comp);
        last = cut;
    }
}

static int __lg(int n)
{
    int k;
    for (k = 0; n != 1; n >>= 1) ++k;
    return k;
}

static void sort(CWideString* first, CWideString* last, TCompare comp)
{
    if (first != last) {
        __introsort_loop(first, last, (CWideString*)0, __lg((int)(last - first)) * 2, comp);
        if (last - first > 16) {
            __insertion_sort(first, first + 16, comp);
            __unguarded_insertion_sort(first + 16, last, comp);
        }
        else
            __insertion_sort(first, last, comp);
    }
}

// --- test helpers ---
static const char* g_words[] = {
    "pear", "Apple", "fig", "BANANA", "cherry", "date", "Elder", "grape", "kiwi", "Lemon", "mango", "Nectarine",
    "olive", "Papaya", "quince", "Raspberry", "satsuma", "Tangerine", "ugli", "Vanilla", "watermelon", "Xigua",
    "yam", "Zucchini", "apricot", "Blueberry", "cantaloupe", "Durian", "eggplant", "Feijoa", "guava", "Honeydew",
    "jackfruit", "Kumquat", "lime", "Mulberry", "nutmeg", "Orange", "peach", "Plum", "apple", "fig"
};
static const int g_nWords = (int)(sizeof(g_words) / sizeof(g_words[0]));

static unsigned long g_seed = 12345;
static int Rand(int n) { g_seed = g_seed * 1103515245UL + 12345UL; return (int)((g_seed >> 16) % (unsigned long)n); }

static void Fill(CWideString* a, int n)
{
    for (int i = 0; i < n; ++i) a[i] = CWideString(g_words[Rand(g_nWords)]);
}

static bool IsSorted(const CWideString* a, int n)
{
    for (int i = 1; i < n; ++i) if (Wcsicmp(a[i], a[i - 1]) < 0) return false;
    return true;
}

// multiset equality by counting pointer identities against the source table
static bool SamePointers(const CWideString* a, const CWideString* b, int n)
{
    for (int w = 0; w < g_nWords; ++w) {
        int ca = 0, cb = 0;
        for (int i = 0; i < n; ++i) { if (a[i].PStringData == g_words[w]) ++ca; if (b[i].PStringData == g_words[w]) ++cb; }
        if (ca != cb) return false;
    }
    return true;
}
int main()
{
    // every permutation of three distinct (case-insensitively ordered) strings yields the middle one
    CWideString lo("apple"), mid("Banana"), hi("CHERRY");
    const CWideString* p[3] = { &lo, &mid, &hi };
    int perm[6][3] = { {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0} };
    TCompare comp;
    for (int i = 0; i < 6; ++i) {
        const CWideString& r = __median(*p[perm[i][0]], *p[perm[i][1]], *p[perm[i][2]], comp);
        if (&r != &mid) { std::printf("00496ee0_TEST FAIL\n"); return 1; }
    }
    // ties: a == b case-insensitively -> !(a<b), a<c -> a
    CWideString a("Fig"), b("fig"), c("zzz");
    const CWideString& r2 = __median(a, b, c, comp);
    if (&r2 != &a) { std::printf("00496ee0_TEST FAIL\n"); return 1; }
    std::printf("00496ee0_TEST PASS\n");
    return 0;
}