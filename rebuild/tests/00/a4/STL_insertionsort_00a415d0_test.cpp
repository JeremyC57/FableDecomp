// Behaviour test for 00a415d0 (_STL::__insertion_sort over pair<pair<CCharString, CCharString>, long> by CKeyPairCompareLess): the range comes out ordered by the comparator, every element
// moved whole (all members still agree with its key), and the by-value copies are balanced.
#include <cstdio>
#include <new>
static int g_live = 0;
static const char* const g_names[] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
struct CCharString {                            // stands in for the engine string (copy 0x0099EC30, dtor 0x0099EAE0)
    const char* m_pData;
    explicit CCharString(int k) : m_pData(g_names[k]) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& other) { m_pData = other.m_pData; return *this; }
    int Key() const { return m_pData[0] - 'a'; }
};
struct C3DVector {                              // memberwise user copy constructor, as retail
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
typedef pair<CCharString, CCharString> SKey;
typedef pair<SKey, long> T;
struct C {                                      // CKeyPairCompareLess<pair<CCharString, CCharString>, long>
    bool operator()(const T& a, const T& b) const { return a.first.first.Key() < b.first.first.Key(); }
};
static T Make(int k) { return T(SKey(CCharString(k), CCharString(k)), (long)(k * 7)); }
static int KeyOf(const T& x) { return x.first.first.Key(); }
static bool Consistent(const T& x) { return x.first.second.Key() == x.first.first.Key() && x.second == 7 * x.first.first.Key(); }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee 0x00A40F50 calls)
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
void __linear_insert(T* first, T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee 0x00A40F50)
{
    if (comp(val, *first)) {
        for (T* p = last; p != first; --p)
            *p = *(p - 1);
        *first = val;
    } else
        __unguarded_linear_insert(last, val, comp);
}
void __insertion_sort(T* first, T* last, C comp)                 // the function under test
{
    if (first == last) return;
    for (T* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
#define FAIL() { std::printf("00a415d0_TEST FAIL\n"); return 1; }
int main()
{
    static const int keys[8] = { 5, 6, 3, 4, 1, 7, 2, 0 };       // no guard: the minimum arrives last and must travel to the front
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(Make(keys[i]));
    C comp = C();
    __insertion_sort(items, items, comp);                       // empty range
    __insertion_sort(items, items + 1, comp);                   // single element
    if (KeyOf(items[0]) != 5) FAIL();
    __insertion_sort(items, items + 8, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) FAIL();
    if (g_live != base + 2 * 8) FAIL();
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    if (g_live != base) FAIL();
    std::printf("00a415d0_TEST PASS\n");
    return 0;
}