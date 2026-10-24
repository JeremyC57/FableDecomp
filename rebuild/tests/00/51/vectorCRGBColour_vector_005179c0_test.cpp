// Behaviour test for 0x005179C0 vector<CRGBColour>(n): n == 0 leaves all three pointers null; otherwise one malloc of
// n colours, end_of_storage = start + n, and every slot copy-constructed from the default colour (finish = start + n).
#include <cstdio>
#include <cstdlib>
#include <new>
using std::malloc;
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_copies;
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(const CRGBColour& o) { ++g_copies; R = o.R; G = o.G; B = o.B; A = o.A; }
};

struct __false_type { __false_type() {} };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x00513B00 (same-TU body)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base(unsigned int n, const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};
template <class T> class vector : public _Vector_base<T> {
public:
    explicit vector(unsigned int n) : _Vector_base<T>(n, allocator<T>())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, T());
    }
};


int main()
{
    int ok = 1;
    {
        vector<CRGBColour> v(0);
        ok &= v._M_start == 0 && v._M_finish == 0 && v._M_end_of_storage._M_data == 0 && g_copies == 0;
    }
    {
        vector<CRGBColour> v(5);
        ok &= v._M_start != 0 && v._M_finish == v._M_start + 5 && v._M_end_of_storage._M_data == v._M_start + 5;
        ok &= g_copies == 5;
        std::free(v._M_start);
    }
    if (!ok) { std::printf("005179c0_TEST FAIL\n"); return 1; }
    std::printf("005179c0_TEST PASS\n");
    return 0;
}