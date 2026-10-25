// Behaviour test of std::__linear_insert(CCharString* first, CCharString* last, CCharString val, less<CCharString> comp) (retail 0x005E1A40)
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

CCharString* __fastcall copy_backward(CCharString* first, CCharString* last, CCharString* result);   // 0x005E0FE0
void __fastcall __unguarded_linear_insert(CCharString* last, CCharString val, less<CCharString> comp);   // 0x005E0710

void __fastcall __linear_insert(CCharString* first, CCharString* last, CCharString val, less<CCharString> comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
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

int main()
{
    static const int keys[4] = { 1, 2, 3, 0 };             // smaller than the first: shift the run up
    CCharString* a = Arrange(keys, 4);
    __linear_insert(a, a + 3, a[3], less<CCharString>());
    bool ok = IsSorted(a, 4) && IsPerm(a, 4) && g_live == 0;
    static const int keys2[4] = { 0, 2, 3, 1 };            // otherwise the unguarded insert
    a = Arrange(keys2, 4);
    __linear_insert(a, a + 3, a[3], less<CCharString>());
    ok = ok && IsSorted(a, 4) && IsPerm(a, 4) && g_live == 0;
    if (!ok) { std::printf("005e1a40_TEST FAIL\n"); return 1; }
    std::printf("005e1a40_TEST PASS\n");
    return 0;
}