// Behaviour test for 00af3240 (_STL::__unguarded_insertion_sort_aux over pair<CCharString, NParticleEngine::CRegisteredPSCInfo> by CKeyPairCompareLess): the range comes out ordered by the comparator, every element
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
struct CPSCBase;
typedef CPSCBase* (*TAllocFunc)();
enum EPSCType { PSC_TYPE_0, PSC_TYPE_7 = 7 };
struct CRegisteredPSCInfo {
    TAllocFunc  AllocFunc;
    EPSCType    ComponentType;
    CCharString GUIName;
    CRegisteredPSCInfo(TAllocFunc f, EPSCType t, const CCharString& n) : AllocFunc(f), ComponentType(t), GUIName(n) { }
};
typedef pair<CCharString, CRegisteredPSCInfo> T;
struct C {                                      // CKeyPairCompareLess<CCharString, CRegisteredPSCInfo>
    bool operator()(const T& a, const T& b) const { return a.first.Key() < b.first.Key(); }
};
static CPSCBase* AllocA() { return 0; }
static T Make(int k) { return T(CCharString(k), CRegisteredPSCInfo(&AllocA, (EPSCType)k, CCharString(k))); }
static int KeyOf(const T& x) { return x.first.Key(); }
static bool Consistent(const T& x) { return x.second.GUIName.Key() == x.first.Key() && (int)x.second.ComponentType == x.first.Key() && x.second.AllocFunc == &AllocA; }
void __unguarded_linear_insert(T* last, T val, C comp)          // the SGI algorithm (stands in for the retail callee 0x00AF2E70)
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
void __unguarded_insertion_sort_aux(T* first, T* last, T*, C comp)   // the function under test
{
    for (T* i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 8]; };
#define FAIL() { std::printf("00af3240_TEST FAIL\n"); return 1; }
int main()
{
    static const int keys[8] = { 0, 6, 3, 4, 1, 7, 2, 5 };       // keys[0] is the minimum: the guard the unguarded insert relies on
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    for (int i = 0; i < 8; ++i)
        new (&items[i]) T(Make(keys[i]));
    C comp = C();
    __unguarded_insertion_sort_aux(items + 1, items + 1, items, comp);   // an empty range changes nothing
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != keys[i]) FAIL();
    __unguarded_insertion_sort_aux(items + 1, items + 8, items, comp);
    for (int i = 0; i < 8; ++i)
        if (KeyOf(items[i]) != i || !Consistent(items[i])) FAIL();
    if (g_live != base + 2 * 8) FAIL();
    for (int i = 0; i < 8; ++i)
        items[i].~T();
    if (g_live != base) FAIL();
    std::printf("00af3240_TEST PASS\n");
    return 0;
}