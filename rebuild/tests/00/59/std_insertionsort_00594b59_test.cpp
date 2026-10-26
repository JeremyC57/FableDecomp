// Behaviour test for 00594b59 (std::__insertion_sort over pair<long, CDefPointer<const CHeroAbilityDef> >): the range comes out ordered by key, every element moved whole, copies balanced.
#include <cstdio>
#include <new>
class CHeroAbilityDef { public:
    long Id;
    long RefCount;                          // the def's reference count (+4 in retail)
};
static CHeroAbilityDef g_defs[8];
template <class T> class CDefPointer { public:
    T* m_pDef;
    explicit CDefPointer(T* def) : m_pDef(def) { ++m_pDef->RefCount; }
    CDefPointer(const CDefPointer& o) : m_pDef(o.m_pDef) { ++m_pDef->RefCount; }
    ~CDefPointer() { --m_pDef->RefCount; }
    CDefPointer& operator=(const CDefPointer& o) { ++o.m_pDef->RefCount; --m_pDef->RefCount; m_pDef = o.m_pDef; return *this; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
typedef pair<long, CDefPointer<CHeroAbilityDef> > SAbilityEntry;
struct SAbilityEntryLess {
    bool operator()(const SAbilityEntry& a, const SAbilityEntry& b) const { return a.first < b.first; }
};
static bool RefsAreOne() { for (int i = 0; i < 8; ++i) if (g_defs[i].RefCount != 1) return false; return true; }
typedef SAbilityEntry T;
typedef SAbilityEntryLess C;
static int KeyOf(const T& x) { return (int)x.first; }
static bool Consistent(const T& x) { return x.second.m_pDef == &g_defs[x.first]; }
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
void __linear_insert(T* first, T* last, T val, C comp)
{
    if (comp(val, *first)) {
        for (T* p = last; p != first; --p)
            *p = *(p - 1);
        *first = val;
    } else {
        __unguarded_linear_insert(last, val, comp);
    }
}
void __insertion_sort(T* first, T* last, C comp)
{
    if (first == last)
        return;
    for (T* i = first + 1; i != last; ++i)
        __linear_insert(first, i, T(*i), comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
int main()
{
    static const int keys[8] = { 6, 3, 4, 1, 7, 2, 5, 0 };
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(keys[i], CDefPointer<CHeroAbilityDef>(&g_defs[keys[i]]));
    C comp = C();
    __insertion_sort(items, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) { std::printf("00594b59_TEST FAIL\n"); return 1; }
    __insertion_sort(items, items + 8, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) { std::printf("00594b59_TEST FAIL\n"); return 1; }
    if (!(RefsAreOne())) { std::printf("00594b59_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    std::printf("00594b59_TEST PASS\n");
    return 0;
}