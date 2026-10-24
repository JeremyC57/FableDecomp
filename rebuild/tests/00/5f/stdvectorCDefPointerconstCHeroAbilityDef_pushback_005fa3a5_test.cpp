// Behaviour test for 0x005FA3A5 std::vector<CDefPointer<const CHeroAbilityDef> >::push_back(x): add-ref copy into the spare
// slot, else _M_insert_overflow(finish, x, __false_type(), 1, true) (0x005FA10A, modelled here as a recording grow).
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <new>

struct CDefPointeeBase {
    virtual void v00() { }
    virtual void Destroy() const { }
    mutable long m_refCount;
    CDefPointeeBase() : m_refCount(1) { }
    virtual ~CDefPointeeBase() { }
};
struct CHeroAbilityDef : CDefPointeeBase { };

template <class T> class CDefPointer {
public:
    T* Object;
    explicit CDefPointer(T* p) : Object(p) { if (Object) ++Object->m_refCount; }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object) --Object->m_refCount; }
};
typedef CDefPointer<const CHeroAbilityDef> CAbilityDefPointer;

namespace stl {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

static int g_overflows = 0;
static size_t g_lastFill = 0;
static bool g_lastAtEnd = false;

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len, bool __atend = false)
    {
        ++g_overflows; g_lastFill = __fill_len; g_lastAtEnd = __atend;
        size_t old = size_t(_M_finish - _M_start), len = old ? 2 * old : 1;
        pointer s = (pointer)std::malloc(len * sizeof(_Tp)), f = s;
        for (pointer p = _M_start; p != __position; ++p, ++f) { new (f) _Tp(*p); p->~_Tp(); }
        new (f) _Tp(__x); ++f;
        std::free(_M_start);
        _M_start = s; _M_finish = f; _M_end_of_storage._M_data = s + len;
    }
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

typedef stl::vector<CAbilityDefPointer> V;

int main()
{
    int ok = 1;
    CHeroAbilityDef a, b;
    V v;
    v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    {
        CAbilityDefPointer pa(&a), pb(&b), pn(0);
        v.push_back(pa);                      // empty: grows through the overflow path
        if (stl::g_overflows != 1 || stl::g_lastFill != 1 || !stl::g_lastAtEnd || v._M_finish - v._M_start != 1) ok = 0;
        v.push_back(pb);                      // full again (capacity 1): overflow, capacity 2
        v._M_end_of_storage._M_data = v._M_finish;   // force a later in-place append to need room
        if (stl::g_overflows != 2) ok = 0;
        // give it room by hand and append in place: no overflow call, add-ref copy into the slot
        CAbilityDefPointer* big = (CAbilityDefPointer*)std::malloc(4 * sizeof(CAbilityDefPointer));
        new (big) CAbilityDefPointer(v._M_start[0]); new (big + 1) CAbilityDefPointer(v._M_start[1]);
        v._M_start[0].~CAbilityDefPointer(); v._M_start[1].~CAbilityDefPointer(); std::free(v._M_start);
        v._M_start = big; v._M_finish = big + 2; v._M_end_of_storage._M_data = big + 4;
        long before = a.m_refCount;
        v.push_back(pa);
        v.push_back(pn);
        if (stl::g_overflows != 2 || v._M_finish != big + 4 || big[2].Object != &a || big[3].Object != 0 ||
            a.m_refCount != before + 1 || big[1].Object != &b) ok = 0;
    }
    for (CAbilityDefPointer* p = v._M_start; p != v._M_finish; ++p) p->~CAbilityDefPointer();
    std::free(v._M_start);
    if (a.m_refCount != 1 || b.m_refCount != 1) ok = 0;
    if (!ok) { std::printf("005fa3a5_TEST FAIL\n"); return 1; }
    std::printf("005fa3a5_TEST PASS\n");
    return 0;
}