// Behaviour test for _STL::vector<long>::vector<int>(int first, int last, const allocator&) (0x00453659): with two ints
// the range constructor takes the integral path -- `first` elements, each equal to `last`; zero gives an empty vector.
#include <cstdio>
#include <cstdlib>

namespace _STL {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct _Is_integer { typedef __false_type _Integral; };
template <> struct _Is_integer<int> { typedef __true_type _Integral; };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc&) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~_Vector_base() { std::free(_M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    template <class _Integer>
    void _M_initialize_aux(_Integer __n, _Integer __value, const __true_type&)       // 0x00451C3E
    {
        this->_M_start = __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0;
        this->_M_end_of_storage = this->_M_start + __n;
        _Tp* __cur = this->_M_start;
        for (; __n > 0; --__n, ++__cur)
            *__cur = __value;
        this->_M_finish = __cur;
    }
    template <class _InputIterator>
    vector(_InputIterator __first, _InputIterator __last, const allocator_type& __a = allocator_type())
        : _Vector_base<_Tp, _Alloc>(__a)
    {
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_initialize_aux(__first, __last, _Integral());
    }
};
}

int main()
{
    int fails = 0;
    {
        _STL::vector<long> v(4, -7);
        if (v._M_finish - v._M_start != 4 || v._M_end_of_storage != v._M_finish) ++fails;
        for (long* p = v._M_start; p != v._M_finish; ++p)
            if (*p != -7) ++fails;
        _STL::vector<long> e(0, 5);
        if (e._M_start != 0 || e._M_finish != 0) ++fails;
    }
    if (fails) { std::printf("00453659_TEST FAIL\n"); return 1; }
    std::printf("00453659_TEST PASS\n");
    return 0;
}