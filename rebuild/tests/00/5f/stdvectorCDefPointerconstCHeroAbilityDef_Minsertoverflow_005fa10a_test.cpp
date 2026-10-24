// Behaviour test for 0x005FA10A std::vector<CDefPointer<const CHeroAbilityDef> >::_M_insert_overflow(pos, x, __false_type,
// fill_len, atend): grow to old + max(old, fill_len), add-ref copies of the head, of x and of the tail, then _M_clear
// (0x005F9F37: release the old elements, free the block).
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
    ~CDefPointer() { if (Object && --Object->m_refCount == 0) Object->Destroy(); }
};
typedef CDefPointer<const CHeroAbilityDef> CAbilityDefPointer;

namespace stl {
struct __false_type { __false_type() { } };

CAbilityDefPointer* __uninitialized_copy(const CAbilityDefPointer* __first, const CAbilityDefPointer* __last,
                                         CAbilityDefPointer* __result, const __false_type&)
{
    CAbilityDefPointer* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        new (__cur) CAbilityDefPointer(*__first);
    return __cur;
}
CAbilityDefPointer* __uninitialized_fill_n(CAbilityDefPointer* __first, size_t __n, const CAbilityDefPointer& __x,
                                           const __false_type&)
{
    for (; __n > 0; --__n, ++__first)
        new (__first) CAbilityDefPointer(__x);
    return __first;
}
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct allocator {
    _Tp* allocate(size_t __n, const void* = 0) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};
template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;
    size_t size() const { return size_t(_M_finish - _M_start); }
    void _M_clear()
    {
        for (pointer p = _M_start; p != _M_finish; ++p) p->~_Tp();
        std::free(_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_t __fill_len,
                            bool __atend = false)
    {
        const size_t __old_size = size();
        const size_t __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = _M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Copy_Construct(__new_finish, __x);
            ++__new_finish;
        }
        else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};
}

typedef stl::vector<CAbilityDefPointer> V;

int main()
{
    int ok = 1;
    CHeroAbilityDef d[3], x;
    V v;
    v._M_start = (CAbilityDefPointer*)std::malloc(3 * sizeof(CAbilityDefPointer));
    for (int i = 0; i < 3; ++i) new (v._M_start + i) CAbilityDefPointer(d + i);
    v._M_finish = v._M_start + 3;
    v._M_end_of_storage._M_data = v._M_finish;
    {
        CAbilityDefPointer px(&x);
        // insert two copies of x before element 1 (not at the end): head, fill, tail
        v._M_insert_overflow(v._M_start + 1, px, stl::__false_type(), 2, false);
        if (v.size() != 5 || v._M_end_of_storage._M_data - v._M_start != 6) ok = 0;
        if (v._M_start[0].Object != &d[0] || v._M_start[1].Object != &x || v._M_start[2].Object != &x ||
            v._M_start[3].Object != &d[1] || v._M_start[4].Object != &d[2]) ok = 0;
        // old copies released, new copies added: each d[i] still holds one extra reference, x two (plus px)
        if (d[0].m_refCount != 2 || d[2].m_refCount != 2 || x.m_refCount != 4) ok = 0;
        // push_back shape: one element at the end
        v._M_insert_overflow(v._M_finish, px, stl::__false_type(), 1, true);
        if (v.size() != 6 || v._M_start[5].Object != &x || x.m_refCount != 5 ||
            v._M_end_of_storage._M_data - v._M_start != 10) ok = 0;   // 5 + max(5, 1)
    }
    v._M_clear();
    if (d[1].m_refCount != 1 || x.m_refCount != 1) ok = 0;
    if (!ok) { std::printf("005fa10a_TEST FAIL\n"); return 1; }
    std::printf("005fa10a_TEST PASS\n");
    return 0;
}