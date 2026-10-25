// Behaviour test for 007d2370 (_STL::__uninitialized_fill_n over CShoveHaPennyRegionDef): n copies of x are copy-constructed into raw storage,
// the returned cursor is first + n, n == 0 constructs nothing, and every copy is balanced by its destructor.
#include <cstdio>
#include <new>
static int g_live = 0;
class CDefinitionManager;
class CDefClassBase {                           // polymorphic definition base (copy constructor 0x0046644C)
public:
    virtual ~CDefClassBase() { --g_live; }
    unsigned char       BaseData[0x18];
    CDefinitionManager* PDefManager;
    unsigned int        GlobalIndex;
    unsigned char       DefaultValsApplied : 1;
    CDefClassBase(CDefinitionManager* m, unsigned int g) : PDefManager(m), GlobalIndex(g), DefaultValsApplied(1)
    {
        for (int i = 0; i < 0x18; ++i) BaseData[i] = (unsigned char)i;
        ++g_live;
    }
    CDefClassBase(const CDefClassBase& other) : PDefManager(other.PDefManager), GlobalIndex(other.GlobalIndex),
                                                DefaultValsApplied(other.DefaultValsApplied)
    {
        for (int i = 0; i < 0x18; ++i) BaseData[i] = other.BaseData[i];
        ++g_live;
    }
};
class CShoveHaPennyRegionDef : public CDefClassBase {
public:
    float Start;
    float Score;
    CShoveHaPennyRegionDef(CDefinitionManager* m, unsigned int g, float s, float sc) : CDefClassBase(m, g), Start(s), Score(sc) { }
};
typedef CShoveHaPennyRegionDef T;
static bool Same(const T& a, const T& b)
{
    for (int i = 0; i < 0x18; ++i) if (a.BaseData[i] != b.BaseData[i]) return false;
    return a.PDefManager == b.PDefManager && a.GlobalIndex == b.GlobalIndex && a.DefaultValsApplied == b.DefaultValsApplied && a.Start == b.Start && a.Score == b.Score;
}
struct __false_type { __false_type() { } };
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 6]; };
int main()
{
    static int dummy;
    T x((CDefinitionManager*)&dummy, 17u, 1.25f, 3.5f);
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    T* r = __uninitialized_fill_n(items, 0u, x, __false_type());
    if (r != items || g_live != base) { std::printf("007d2370_TEST FAIL\n"); return 1; }
    r = __uninitialized_fill_n(items, 5u, x, __false_type());
    if (r != items + 5) { std::printf("007d2370_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (!Same(items[i], x)) { std::printf("007d2370_TEST FAIL\n"); return 1; }
    if (g_live != base + 5 * 1) { std::printf("007d2370_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        items[i].~T();
    if (g_live != base) { std::printf("007d2370_TEST FAIL\n"); return 1; }
    std::printf("007d2370_TEST PASS\n");
    return 0;
}