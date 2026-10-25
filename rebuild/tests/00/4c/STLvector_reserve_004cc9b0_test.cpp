// Behaviour test for 0x004CC9B0: STLport vector<pair<ETCInterfaceType, CTCBase*> >::reserve(n) -- grow the capacity to
// n (allocate when empty, else allocate-and-copy and free the old block); never shrink.
#include <cstdio>
#include <cstdlib>
#include <new>

static int gMallocs = 0;
static int gFrees = 0;
enum ETCInterfaceType { TC_NONE = 0, TC_TEST_MIN = -128, TC_TEST_MAX = 127 };   // test range widened
class CTCBase;
static char gThings[64];                                // fake component addresses

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { if (__n != 0) ++gMallocs; return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) { ++gFrees; std::free(__p); } }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage._M_data - _M_start); }
    template <class _ForwardIterator>
    _Tp* _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e) { _M_start = __s; _M_finish = __f; _M_end_of_storage._M_data = __e; }
    void reserve(size_type __n)
    {
        if (capacity() < __n) {
            const size_type __old_size = size();
            _Tp* __tmp;
            if (_M_start) {
                __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
                _M_clear();
            } else {
                __tmp = _M_end_of_storage.allocate(__n);
            }
            _M_set(__tmp, __tmp + __old_size, __tmp + __n);
        }
    }
};
}
typedef std::pair<ETCInterfaceType, CTCBase*> TElem;
typedef std::vector<TElem> TVec;
static TElem Mk(int i) { return TElem((ETCInterfaceType)(i * 3 - 7), (CTCBase*)(gThings + i)); }
static bool Is(const TElem& e, int i) { return (int)e.first == i * 3 - 7 && (char*)e.second == gThings + i; }

int main()
{
    TVec v;
    v._M_set(0, 0, 0);
    v.reserve(4);                                       // empty: plain allocate
    bool ok = v._M_start != 0 && v.size() == 0 && v.capacity() == 4 && gMallocs == 1 && gFrees == 0;
    for (int i = 0; i < 3; ++i) {
        new (v._M_finish) TElem(Mk(i));
        ++v._M_finish;
    }
    TElem* old = v._M_start;
    v.reserve(10);                                      // grow: copy three, free the old block
    ok = ok && v._M_start != old && v.size() == 3 && v.capacity() == 10 && gMallocs == 2 && gFrees == 1;
    for (int i = 0; ok && i < 3; ++i)
        if (!Is(v._M_start[i], i))
            ok = false;
    TElem* cur = v._M_start;
    v.reserve(5);                                       // smaller: no-op
    ok = ok && v._M_start == cur && v.size() == 3 && v.capacity() == 10 && gMallocs == 2 && gFrees == 1;
    v._M_clear();
    if (!ok) {
        std::printf("004cc9b0_TEST FAIL\n");
        return 1;
    }
    std::printf("004cc9b0_TEST PASS\n");
    return 0;
}