// Behaviour test for 00a3fc00 (std::__uninitialized_fill_n over pair<pair<CCharString, CCharString>, long>): n copies of x are copy-constructed into raw storage,
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
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
typedef pair<CCharString, CCharString> SKey;
typedef pair<SKey, long> T;
static bool Same(const T& a, const T& b)
{
    return SameStr(a.first.first, b.first.first) && SameStr(a.first.second, b.first.second) && a.second == b.second;
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
    T x(SKey(CCharString("A"), CCharString("B")), 77L);
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    T* r = __uninitialized_fill_n(items, 0u, x, __false_type());
    if (r != items || g_live != base) { std::printf("00a3fc00_TEST FAIL\n"); return 1; }
    r = __uninitialized_fill_n(items, 5u, x, __false_type());
    if (r != items + 5) { std::printf("00a3fc00_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (!Same(items[i], x)) { std::printf("00a3fc00_TEST FAIL\n"); return 1; }
    if (g_live != base + 5 * 2) { std::printf("00a3fc00_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        items[i].~T();
    if (g_live != base) { std::printf("00a3fc00_TEST FAIL\n"); return 1; }
    std::printf("00a3fc00_TEST PASS\n");
    return 0;
}