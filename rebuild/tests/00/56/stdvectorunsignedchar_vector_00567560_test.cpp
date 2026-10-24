// Behaviour test for 0x00567560 std::vector<unsigned char>::vector(n, val, a).
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace stl {
struct __true_type { };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
inline void fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val)
{
    unsigned char __tmp = __val;
    std::memset(__first, __tmp, __last - __first);
}
template <class _Size>
inline unsigned char* fill_n(unsigned char* __first, _Size __n, const unsigned char& __val)
{
    fill(__first, __first + __n, __val);
    return __first + __n;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector(unsigned int __n, const _Tp& __val, const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__n, __a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
    }
};
}

int main()
{
    int ok = 1;
    unsigned int sizes[] = { 1, 3, 4, 7, 64 };
    for (int k = 0; k < 5; ++k) {
        unsigned int n = sizes[k];
        stl::vector<unsigned char> v(n, (unsigned char)0xA5);
        if (v._M_start == 0 || v._M_finish != v._M_start + n || v._M_end_of_storage._M_data != v._M_start + n) ok = 0;
        for (unsigned int i = 0; i < n; ++i)
            if (v._M_start[i] != 0xA5) ok = 0;
        std::free(v._M_start);
    }
    stl::vector<unsigned char> e(0, (unsigned char)1);
    if (e._M_start != 0 || e._M_finish != 0 || e._M_end_of_storage._M_data != 0) ok = 0;
    if (!ok) { std::printf("00567560_TEST FAIL\n"); return 1; }
    std::printf("00567560_TEST PASS\n");
    return 0;
}