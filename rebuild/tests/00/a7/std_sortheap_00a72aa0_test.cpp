// behaviour test for 00a72aa0: SGI sort_heap over (unsigned long, {CCharString, CCountedPointer<CFaction>}) pairs (second instantiation); names and counts must follow
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

static int g_destroyed = 0;                     // objects whose last reference went away
struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
static void DestroyObject(void*) { ++g_destroyed; }
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
    ~CCountedPointer() { Release(); }
    void Release()
    {
        if (m_pCount && --m_pCount->m_references == 0)
            m_pCount->m_pfnDestroy(m_pCount->m_pObject);
        m_pObject = 0;
        m_pCount = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        if (m_pCount != other.m_pCount) {
            if (other.m_pCount) ++other.m_pCount->m_references;
            Release();
            m_pObject = other.m_pObject;
            m_pCount = other.m_pCount;
        }
        return *this;
    }
    void Attach(T* object, SCountBlock* count)
    {
        Release();
        count->m_references = 1; count->m_pfnDestroy = DestroyObject; count->m_pObject = object;
        m_pObject = object; m_pCount = count;
    }
};
static SCountBlock g_counts[100];

struct CFaction { long m_id; };
static CFaction g_factions[100];
struct SNamedPointer {
    CCharString               Name;
    CCountedPointer<CFaction> Pointer;
};
struct SKeyedPair {
    unsigned long first;
    SNamedPointer second;
    SKeyedPair() : first(0) { }
};
struct CKeyPairCompareLess {
    bool operator()(const SKeyedPair& a, const SKeyedPair& b) const { return a.first < b.first; }
};
typedef SKeyedPair ElemT;
typedef CKeyPairCompareLess CompT;
static CompT MakeComp() { return CompT(); }
static void Fill(ElemT& e, int k)
{
    e.first = (unsigned long)k;
    e.second.Name = Name("faction", k);
    g_factions[k].m_id = k;
    e.second.Pointer.Attach(&g_factions[k], &g_counts[k]);
}
static int KeyOf(const ElemT& e) { return int(e.first); }
static bool Check(const ElemT& e)
{
    int k = KeyOf(e);
    return Same(e.second.Name, "faction", k) && e.second.Pointer.m_pObject->m_id == k && g_counts[k].m_references == 1;
}
static bool Live(int n) { return g_live == n && g_destroyed == 0; }
static bool Final() { return g_live == 0 && g_destroyed == 9; }

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
    if (!ok) { std::printf("00a72aa0_TEST FAIL\n"); return 1; }
    std::printf("00a72aa0_TEST PASS\n");
    return 0;
}