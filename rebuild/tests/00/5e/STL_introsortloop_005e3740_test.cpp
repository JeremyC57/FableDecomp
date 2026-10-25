// Behaviour test of std::__introsort_loop(CCharString* first, CCharString* last, CCharString*, int depth_limit, less<CCharString> comp) (retail 0x005E3740)
// The model and the function body are the source's own; the retail callees are given test bodies below.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

struct CBasicStringChar {                                  // CBasicString<char>
    char* PData;
    __forceinline long Compare(const CBasicStringChar& other) const
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
    bool operator<(const CBasicStringChar& other) const;   // 0x00429950
};

// The comparator's real body (the landed 0x00429950 source) compiled out of line ABOVE its caller: VC7.1 then knows
// the call writes no memory, so the pivot's payload pointer stays in esi across the compare calls (reloaded only
// after the swap's operator= calls), as retail. Without it the pivot word is re-read from its slot after every compare.
#pragma auto_inline(off)
bool CBasicStringChar::operator<(const CBasicStringChar& other) const
{
    return Compare(other) < 0;
}
#pragma auto_inline(on)

struct CCharString {
    CBasicStringChar* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
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

template <class T> struct less {                           // STLport less<T>: empty, passed by value
    bool operator()(const T& a, const T& b) const { return a < b; }
};

const CCharString& __fastcall __median(const CCharString& a, const CCharString& b, const CCharString& c, less<CCharString> comp);   // 0x005E0580
CCharString* __fastcall __unguarded_partition(CCharString* first, CCharString* last, CCharString pivot, less<CCharString> comp);   // 0x005E0650
void __fastcall __partial_sort(CCharString* first, CCharString* middle, CCharString* last, CCharString*, less<CCharString> comp);   // 0x005E2EB0

inline void partial_sort(CCharString* first, CCharString* middle, CCharString* last, less<CCharString> comp)
{
    __partial_sort(first, middle, last, (CCharString*)0, comp);
}

void __fastcall __introsort_loop(CCharString* first, CCharString* last, CCharString*, int depth_limit, less<CCharString> comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        CCharString* cut = __unguarded_partition(first, last,
                                                 CCharString(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                                 comp);
        __introsort_loop(cut, last, (CCharString*)0, depth_limit, comp);
        last = cut;
    }
}

// ---- test bodies of the retail callees ----
static int g_live = 0;                                     // CCharString copies alive: copy ctor +1, dtor -1
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_live; }
CCharString::~CCharString() { --g_live; }
CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }

static char g_text[64][4];
static CBasicStringChar g_payload[64];
static CBasicStringChar* g_raw[64];                        // viewed as CCharString[64]: one payload pointer each
static CCharString* Arrange(const int* keys, int n)       // "kNN" strings in the given key order
{
    for (int i = 0; i < n; ++i) {
        g_text[i][0] = 'k';
        g_text[i][1] = (char)('0' + keys[i] / 10);
        g_text[i][2] = (char)('0' + keys[i] % 10);
        g_text[i][3] = 0;
        g_payload[i].PData = g_text[i];
        g_raw[i] = &g_payload[i];
    }
    return (CCharString*)g_raw;
}
static CCharString* MakeStrings(int n, int mul)            // a permutation of the keys 0..n-1 (gcd(mul, n) == 1)
{
    int keys[64];
    for (int i = 0; i < n; ++i)
        keys[i] = (i * mul + 5) % n;
    return Arrange(keys, n);
}
static int Key(const CCharString& s) { const char* t = s.PStringData->PData; return (t[1] - '0') * 10 + (t[2] - '0'); }
static bool IsSorted(const CCharString* a, int n) { for (int i = 1; i < n; ++i) if (Key(a[i]) < Key(a[i - 1])) return false; return true; }
static bool IsPerm(const CCharString* a, int n)
{
    int seen[64] = { 0 };
    for (int i = 0; i < n; ++i) {
        int k = Key(a[i]);
        if (k < 0 || k >= n || seen[k])
            return false;
        seen[k] = 1;
    }
    return true;
}
static bool IsHeap(const CCharString* a, int n) { for (int i = 1; i < n; ++i) if (Key(a[(i - 1) / 2]) < Key(a[i])) return false; return true; }

void __fastcall __push_heap(CCharString* first, int holeIndex, int topIndex, CCharString val, less<CCharString> comp)   // 0x005E08C0
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], val)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = val;
}

void __fastcall __adjust_heap(CCharString* first, int holeIndex, int len, CCharString val, less<CCharString> comp)   // 0x005E1170
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1]))
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, val, comp);
}

void __fastcall __make_heap(CCharString* first, CCharString* last, less<CCharString> comp, CCharString*, int*)   // 0x005E1BE0
{
    if (last - first < 2)
        return;
    int len = last - first;
    for (int parent = (len - 2) / 2; parent >= 0; --parent)
        __adjust_heap(first, parent, len, CCharString(first[parent]), comp);
}

void __fastcall sort_heap(CCharString* first, CCharString* last, less<CCharString> comp)   // 0x005E27F0
{
    while (last - first > 1) {
        --last;
        CCharString val(*last);
        *last = *first;
        __adjust_heap(first, 0, last - first, val, comp);
    }
}

void __fastcall __partial_sort(CCharString* first, CCharString* middle, CCharString* last, CCharString*, less<CCharString> comp)   // 0x005E2EB0
{
    __make_heap(first, middle, comp, (CCharString*)0, (int*)0);
    for (CCharString* i = middle; i < last; ++i)
        if (comp(*i, *first)) {
            CCharString val(*i);
            *i = *first;
            __adjust_heap(first, 0, middle - first, val, comp);
        }
    sort_heap(first, middle, comp);
}

const CCharString& __fastcall __median(const CCharString& a, const CCharString& b, const CCharString& c, less<CCharString> comp)   // 0x005E0580
{
    if (comp(a, b))
        return comp(b, c) ? b : (comp(a, c) ? c : a);
    return comp(a, c) ? a : (comp(b, c) ? c : b);
}

CCharString* __fastcall __unguarded_partition(CCharString* first, CCharString* last, CCharString pivot, less<CCharString> comp)   // 0x005E0650
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        CCharString tmp = *first;
        *first = *last;
        *last = tmp;
        ++first;
    }
}

CCharString* __fastcall copy_backward(CCharString* first, CCharString* last, CCharString* result)   // 0x005E0FE0
{
    while (first != last)
        *--result = *--last;
    return result;
}

void __fastcall __unguarded_linear_insert(CCharString* last, CCharString val, less<CCharString> comp)   // 0x005E0710
{
    CCharString* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}

void __fastcall __linear_insert(CCharString* first, CCharString* last, CCharString val, less<CCharString> comp)   // 0x005E1A40
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

void __fastcall __insertion_sort(CCharString* first, CCharString* last, less<CCharString> comp)   // 0x005E20C0
{
    if (first == last)
        return;
    for (CCharString* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    CCharString* a = MakeStrings(40, 7);
    __introsort_loop(a, a + 40, (CCharString*)0, 10, less<CCharString>());
    bool ok = IsPerm(a, 40) && g_live == 0;
    __insertion_sort(a, a + 40, less<CCharString>());       // the loop leaves ordered runs of at most 16
    ok = ok && IsSorted(a, 40) && g_live == 0;
    a = MakeStrings(40, 3);                                 // depth exhausted: partial_sort sorts the whole range
    __introsort_loop(a, a + 40, (CCharString*)0, 0, less<CCharString>());
    ok = ok && IsSorted(a, 40) && IsPerm(a, 40) && g_live == 0;
    if (!ok) { std::printf("005e3740_TEST FAIL\n"); return 1; }
    std::printf("005e3740_TEST PASS\n");
    return 0;
}