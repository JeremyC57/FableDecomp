// Behaviour test for 009e6d10 (std::__unguarded_insertion_sort_aux over pair<CCharString, CSymbolMap::CSymbolInfo>): the range comes out ordered by key, every element moved whole, copies balanced.
#include <cstdio>
#include <new>
static int g_live = 0;
static const char* const g_names[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
class CCharString { public:
    void* m_pData;
    explicit CCharString(int k) : m_pData((void*)g_names[k]) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }   // stands in for 0x0099EC30
    ~CCharString() { --g_live; }                                                   // stands in for 0x0099EAE0
    const CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
    int Key() const { return ((const char*)m_pData)[0] - 'a'; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
class CSymbolMap { public:
    class CSymbolInfo { public:
        CCharString GroupName;
        CCharString EntryName;
        long        Val;
        CSymbolInfo(int k) : GroupName(k), EntryName(k), Val(k * 10) { }
    };
};
typedef pair<CCharString, CSymbolMap::CSymbolInfo> SSymbolEntry;
struct SSymbolEntryLess {
    bool operator()(const SSymbolEntry& a, const SSymbolEntry& b) const { return a.first.Key() < b.first.Key(); }
};
typedef SSymbolEntry T;
typedef SSymbolEntryLess C;
static int KeyOf(const T& x) { return x.first.Key(); }
static bool Consistent(const T& x) { return x.second.GroupName.Key() == x.first.Key() && x.second.EntryName.Key() == x.first.Key() && x.second.Val == 10 * x.first.Key(); }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee)
{
    T* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
void __unguarded_insertion_sort_aux(T* first, T* last, T*, C comp)
{
    for (T* i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
int main()
{
    static const int keys[8] = { 0, 6, 3, 4, 1, 7, 2, 5 };
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(CCharString(keys[i]), CSymbolMap::CSymbolInfo(keys[i]));
    C comp = C();
    __unguarded_insertion_sort_aux(items + 1, items + 1, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) { std::printf("009e6d10_TEST FAIL\n"); return 1; }
    __unguarded_insertion_sort_aux(items + 1, items + 8, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) { std::printf("009e6d10_TEST FAIL\n"); return 1; }
    if (!(g_live == 3 * 8)) { std::printf("009e6d10_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    std::printf("009e6d10_TEST PASS\n");
    return 0;
}