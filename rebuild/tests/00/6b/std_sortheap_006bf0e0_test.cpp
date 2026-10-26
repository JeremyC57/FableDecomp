// behaviour test for 006bf0e0: SGI sort_heap over (long, CMusicManager::CRegisteredMusicEntry) pairs
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_wlive = 0;
class CWideString {
public:
    wchar_t m_text[16];
    CWideString() { m_text[0] = 0; ++g_wlive; }
    CWideString(const CWideString& other) { Set(other.m_text); ++g_wlive; }
    ~CWideString() { --g_wlive; }
    CWideString& operator=(const CWideString& other) { if (this != &other) Set(other.m_text); return *this; }
    void Set(const wchar_t* s) { int i = 0; for (; s[i] && i < 15; ++i) m_text[i] = s[i]; m_text[i] = 0; }
    void SetNarrow(const char* s) { int i = 0; for (; s[i] && i < 15; ++i) m_text[i] = (wchar_t)s[i]; m_text[i] = 0; }
    bool Equals(const char* s) const { int i = 0; for (; s[i]; ++i) if (m_text[i] != (wchar_t)s[i]) return false; return m_text[i] == 0; }
};
class CMusicManager { public:
    class CRegisteredMusicEntry { public:
        CWideString SrcFileName;
        CWideString DstFileName;
        float Gain;
        float Offset;
        bool  CacheToHD;
        CRegisteredMusicEntry() : Gain(0), Offset(0), CacheToHD(false) { }
    };
};
struct SMusicEntryPair {
    long first;
    CMusicManager::CRegisteredMusicEntry second;
    SMusicEntryPair() : first(0) { }
};
struct CKeyPairCompareLess {
    bool operator()(const SMusicEntryPair& a, const SMusicEntryPair& b) const { return a.first < b.first; }
};
typedef SMusicEntryPair ElemT;
typedef CKeyPairCompareLess CompT;
static bool NameIs(const CWideString& s, const char* prefix, int k) { char buf[32]; std::sprintf(buf, "%s%03d", prefix, k); return s.Equals(buf); }
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    char buf[32];
    e.first = k;
    std::sprintf(buf, "src%03d", k);
    e.second.SrcFileName.SetNarrow(buf);
    std::sprintf(buf, "dst%03d", k);
    e.second.DstFileName.SetNarrow(buf);
    e.second.Gain = k * 0.5f;
    e.second.Offset = k + 0.25f;
    e.second.CacheToHD = (k % 2) == 0;
}
static int KeyOf(const ElemT& e) { return int(e.first); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    return NameIs(e.second.SrcFileName, "src", k) && NameIs(e.second.DstFileName, "dst", k) && e.second.Gain == k * 0.5f
        && e.second.Offset == k + 0.25f && e.second.CacheToHD == ((k % 2) == 0);
}
static bool Live(int n) { return g_wlive == 2 * n; }
static bool Final() { return g_wlive == 0; }

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

// the out-of-line callee (__pop_heap in retail), modelled with the standard hole algorithm
static void __pop_heap(ElemT* first, ElemT* last, ElemT* result, ElemT value, CompT comp, int*)
{
    *result = *first;
    AdjustHeapT(first, 0, long(last - first), value, comp);
}
// the reconstructed body: SGI sort_heap with pop_heap / __pop_heap_aux inline
inline void __pop_heap_aux(ElemT* first, ElemT* last, ElemT*, CompT comp)
{
    __pop_heap(first, last - 1, last - 1, ElemT(*(last - 1)), comp, (int*)0);
}
inline void pop_heap(ElemT* first, ElemT* last, CompT comp)
{
    __pop_heap_aux(first, last, (ElemT*)0, comp);
}
static void sort_heap(ElemT* first, ElemT* last, CompT comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}
#define SORT(a, n, comp) sort_heap(a, a + n, comp)

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
    if (!ok) { std::printf("006bf0e0_TEST FAIL\n"); return 1; }
    std::printf("006bf0e0_TEST PASS\n");
    return 0;
}