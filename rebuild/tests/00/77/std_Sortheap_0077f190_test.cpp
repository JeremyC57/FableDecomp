// behaviour test for 0077f190: Dinkumware _Sort_heap over (CCharString, C3DVector) pairs ordered by the name
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_live = 0;                          // live CCharString instances
class CCharString {
public:
    char m_text[16];
    CCharString() { m_text[0] = 0; ++g_live; }
    CCharString(const char* s) { Set(s); ++g_live; }
    CCharString(const CCharString& other) { Set(other.m_text); ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& other) { if (this != &other) Set(other.m_text); return *this; }
    void Set(const char* s) { int i = 0; for (; s[i] && i < 15; ++i) m_text[i] = s[i]; m_text[i] = 0; }
    const char* c_str() const { return m_text; }
};
static CCharString Name(const char* prefix, int k) { char buf[32]; std::sprintf(buf, "%s%03d", prefix, k); return CCharString(buf); }
static int NumOf(const CCharString& s) { const char* p = s.c_str(); while (*p && (*p < '0' || *p > '9')) ++p; return std::atoi(p); }
static bool Same(const CCharString& s, const char* prefix, int k) { char buf[32]; std::sprintf(buf, "%s%03d", prefix, k); return std::strcmp(s.c_str(), buf) == 0; }
static bool StrLess(const CCharString& a, const CCharString& b) { return std::strcmp(a.c_str(), b.c_str()) < 0; }

struct C3DVector {
    int data[3];
    C3DVector() { data[0] = data[1] = data[2] = 0; }
    C3DVector(const C3DVector& o) { data[0] = o.data[0]; data[1] = o.data[1]; data[2] = o.data[2]; }
};
struct SNamedVector {
    CCharString first;
    C3DVector   second;
};
struct CCompareNamedVector {
    int state;
    bool operator()(const SNamedVector& a, const SNamedVector& b) const { return StrLess(a.first, b.first); }
};
typedef SNamedVector ElemT;
typedef CCompareNamedVector CompT;
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    e.first = Name("vec", k);
    e.second.data[0] = k; e.second.data[1] = k * 2; e.second.data[2] = -k;
}
static int KeyOf(const ElemT& e) { return NumOf(e.first); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    return e.second.data[0] == k && e.second.data[1] == k * 2 && e.second.data[2] == -k;
}
static bool Live(int n) { return g_live == n; }
static bool Final() { return g_live == 0; }

static void PushHeapT(ElemT* first, long hole, long top, ElemT value, CompT comp)
{
    long parent = (hole - 1) / 2;
    while (top < hole && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeapT(ElemT* first, long hole, long len, ElemT value, CompT comp)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < len; idx = 2 * idx + 2) {
        if (comp(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx];
        hole = idx;
    }
    if (idx == len) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeapT(first, hole, top, value, comp);
}

// the out-of-line callee (_Adjust_heap in retail)
static void _Adjust_heap(ElemT* first, long hole, long bottom, ElemT value, CompT pred)
{
    AdjustHeapT(first, hole, bottom, value, pred);
}
// the reconstructed body: Dinkumware _Sort_heap with _Pop_heap_0 / _Pop_heap inline
inline void _Pop_heap(ElemT* first, ElemT* last, ElemT* dest, ElemT value, CompT pred, long*)
{
    *dest = *first;
    _Adjust_heap(first, 0, long(last - first), value, pred);
}
inline void _Pop_heap_0(ElemT* first, ElemT* last, CompT pred, ElemT*)
{
    _Pop_heap(first, last - 1, last - 1, ElemT(*(last - 1)), pred, (long*)0);
}
static void _Sort_heap(ElemT* first, ElemT* last, CompT pred)
{
    for (; 1 < last - first; --last)
        _Pop_heap_0(first, last, pred, (ElemT*)0);
}
#define SORT(a, n, comp) _Sort_heap(a, a + n, comp)

int main()
{
    static const int keys[9] = { 5, 31, 12, 7, 64, 0, 99, 40, 2 };
    const int n = 9;
    int ok = 1;
    {
        ElemT a[9];
        CompT comp = MakeComp();
        int i = 0;
        for (i = 0; i < n; ++i) Fill(a[i], keys[i]);
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        for (i = 0; i < n; ++i) if (!Check(a[i])) ok = 0;
        if (KeyOf(a[0]) != 0 || KeyOf(a[n - 1]) != 99) ok = 0;
        if (!Live(n)) ok = 0;
        // re-heap the sorted output and sort again; a one-element range is left as it is
        for (i = n / 2; 0 < i; ) { --i; AdjustHeapT(a, i, n, ElemT(a[i]), comp); }
        SORT(a, n, comp);
        for (i = 1; i < n; ++i) if (!(KeyOf(a[i - 1]) < KeyOf(a[i]))) ok = 0;
        SORT(a, 1, comp);
        if (KeyOf(a[0]) != 0 || !Check(a[0])) ok = 0;
        if (!Live(n)) ok = 0;
    }
    if (!Final()) ok = 0;
    if (!ok) { std::printf("0077f190_TEST FAIL\n"); return 1; }
    std::printf("0077f190_TEST PASS\n");
    return 0;
}