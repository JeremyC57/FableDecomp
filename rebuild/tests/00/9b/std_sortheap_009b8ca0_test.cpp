// behaviour test for 009b8ca0: SGI sort_heap over (unsigned long, CDefinitionManager::CDefClassInfo) pairs; names and index arrays must follow
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
    const CCharString& operator=(const CCharString& other) { if (this != &other) Set(other.m_text); return *this; }
    void Set(const char* s) { int i = 0; for (; s[i] && i < 15; ++i) m_text[i] = s[i]; m_text[i] = 0; }
    const char* c_str() const { return m_text; }
};
static CCharString Name(const char* prefix, int k) { char buf[32]; std::sprintf(buf, "%s%03d", prefix, k); return CCharString(buf); }
static int NumOf(const CCharString& s) { const char* p = s.c_str(); while (*p && (*p < '0' || *p > '9')) ++p; return std::atoi(p); }
static bool Same(const CCharString& s, const char* prefix, int k) { char buf[32]; std::sprintf(buf, "%s%03d", prefix, k); return std::strcmp(s.c_str(), buf) == 0; }
static bool StrLess(const CCharString& a, const CCharString& b) { return std::strcmp(a.c_str(), b.c_str()) < 0; }

static int g_arrays = 0;
template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CArray(const CArray& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { Assign(x._M_start, x.size()); }
    ~CArray() { Free(); }
    CArray& operator=(const CArray& x) { if (this != &x) { Free(); Assign(x._M_start, x.size()); } return *this; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void Free() { if (_M_start) { std::free(_M_start); --g_arrays; } _M_start = _M_finish = _M_end_of_storage = 0; }
    void Assign(const T* p, unsigned int n)
    {
        _M_start = n ? (T*)std::malloc(n * sizeof(T)) : 0;
        if (_M_start) { ++g_arrays; std::memmove(_M_start, p, n * sizeof(T)); }
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_finish;
    }
};
class CDefClassBase;
static CDefClassBase* AllocDef() { return 0; }
class CDefinitionManager { public:
    class CDefClassInfo { public:
        CCharString Name;
        CDefClassBase* (*AllocFunc)();
        CArray<unsigned long> GlobalIndices;
        CDefClassInfo() : AllocFunc(0) { }
    };
};
struct SDefClassInfoEntry {
    unsigned long first;
    CDefinitionManager::CDefClassInfo second;
    SDefClassInfoEntry() : first(0) { }
};
struct CKeyPairCompareLess {
    bool operator()(const SDefClassInfoEntry& a, const SDefClassInfoEntry& b) const { return a.first < b.first; }
};
typedef SDefClassInfoEntry ElemT;
typedef CKeyPairCompareLess CompT;
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    unsigned long indices[4];
    unsigned int count = (unsigned int)(k % 4) + 1;
    unsigned int i = 0;
    for (i = 0; i < count; ++i) indices[i] = (unsigned long)(k * 10 + i);
    e.first = (unsigned long)k;
    e.second.Name = Name("Def", k);
    e.second.AllocFunc = &AllocDef;
    e.second.GlobalIndices.Free();
    e.second.GlobalIndices.Assign(indices, count);
}
static int KeyOf(const ElemT& e) { return int(e.first); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    unsigned int i = 0;
    if (!Same(e.second.Name, "Def", k) || e.second.AllocFunc != &AllocDef) return false;
    if (e.second.GlobalIndices.size() != (unsigned int)(k % 4) + 1) return false;
    for (i = 0; i < e.second.GlobalIndices.size(); ++i) if (e.second.GlobalIndices._M_start[i] != (unsigned long)(k * 10 + i)) return false;
    return true;
}
static bool Live(int n) { return g_live == n && g_arrays == n; }
static bool Final() { return g_live == 0 && g_arrays == 0; }

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
    if (!ok) { std::printf("009b8ca0_TEST FAIL\n"); return 1; }
    std::printf("009b8ca0_TEST PASS\n");
    return 0;
}