// Behaviour test for 00a486b0 (std::__uninitialized_fill_n over CLipSyncPhoneme): n copies of x are copy-constructed into raw storage,
// the returned cursor is first + n, n == 0 constructs nothing, and every copy is balanced by its destructor.
#include <cstdio>
#include <new>
static int g_live = 0;
class CCharString { public:                            // stands in for the engine string (copy 0x0099EC30, dtor 0x0099EAE0)
    const char* m_pData;
    explicit CCharString(const char* s) : m_pData(s) { ++g_live; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
};
static bool SameStr(const CCharString& a, const CCharString& b) { return a.m_pData == b.m_pData; }
class CLipSyncPhoneme { public:
    CCharString  Symbol;
    unsigned int ID;
    float        Weight;
    CLipSyncPhoneme(const char* s, unsigned int i, float w) : Symbol(s), ID(i), Weight(w) { }
};
typedef CLipSyncPhoneme T;
static bool Same(const T& a, const T& b)
{
    return SameStr(a.Symbol, b.Symbol) && a.ID == b.ID && a.Weight == b.Weight;
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
    T x("AA", 0x1234u, 0.5f);
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    T* r = __uninitialized_fill_n(items, 0u, x, __false_type());
    if (r != items || g_live != base) { std::printf("00a486b0_TEST FAIL\n"); return 1; }
    r = __uninitialized_fill_n(items, 5u, x, __false_type());
    if (r != items + 5) { std::printf("00a486b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (!Same(items[i], x)) { std::printf("00a486b0_TEST FAIL\n"); return 1; }
    if (g_live != base + 5 * 1) { std::printf("00a486b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        items[i].~T();
    if (g_live != base) { std::printf("00a486b0_TEST FAIL\n"); return 1; }
    std::printf("00a486b0_TEST PASS\n");
    return 0;
}