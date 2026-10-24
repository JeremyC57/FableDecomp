// Behaviour test for 0x00A5E4D0 _STL::vector<SElement8>::operator=(const vector&)
#include <cstdio>
#include <cstdlib>
#include <new>

struct SElement8 {
    long m_00;
    long m_04;
};

namespace _STL {
struct __false_type { __false_type() {} };
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }
// stand-ins for the out-of-line helpers 0x00A5DF30 / 0x00A5DDE0 / 0x00A5DEE0
template <class _InputIter, class _OutputIter>
_OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    for (; __first != __last; ++__first, ++__result) *__result = *__first;
    return __result;
}
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for (; __first != __last; ++__first, ++__result) new (__result) SElement8(*__first);
    return __result;
}
template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    typedef __false_type _TrivialAss;
    typedef __false_type _IsPODType;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector(size_type n, size_type cap, long seed)
    {
        _M_start = (_Tp*)std::malloc(cap * sizeof(_Tp));
        _M_finish = _M_start + n;
        _M_end_of_storage = _M_start + cap;
        for (size_type i = 0; i < n; ++i) { _M_start[i].m_00 = seed + (long)i; _M_start[i].m_04 = -(seed + (long)i); }
    }
    ~vector() { std::free(_M_start); }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage - this->_M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = (pointer)std::malloc(__n * sizeof(_Tp));
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        if (this->_M_start) std::free(this->_M_start);
    }
    vector<_Tp>& operator=(const vector<_Tp>& __x)
    {
        if (&__x != this) {
            const size_type __xlen = __x.size();
            if (__xlen > capacity()) {
                pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
                _M_clear();
                this->_M_start = __tmp;
                this->_M_end_of_storage = this->_M_start + __xlen;
            }
            else if (size() >= __xlen) {
                pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0,
                                          (pointer)this->_M_start, _TrivialAss());
                _Destroy(__i, this->_M_finish);
            }
            else {
                __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start,
                            _TrivialAss());
                __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0,
                                     this->_M_finish, _IsPODType());
            }
            this->_M_finish = this->_M_start + __xlen;
        }
        return *this;
    }
};
}   // namespace _STL

typedef _STL::vector<SElement8> CVec;

static bool Holds(const CVec& v, unsigned int n, long seed)
{
    if (v.size() != n) return false;
    for (unsigned int i = 0; i < n; ++i)
        if (v._M_start[i].m_00 != seed + (long)i || v._M_start[i].m_04 != -(seed + (long)i)) return false;
    return true;
}

int main()
{
    CVec src(5, 5, 100);
    CVec grow(1, 2, 0);                     // capacity 2 < 5: reallocate and copy
    CVec shrink(7, 8, 0);                   // size 7 >= 5: copy over, finish moves down
    CVec partial(2, 6, 0);                  // size 2 < 5 <= capacity 6: copy 2, construct 3
    bool ok = &(grow = src) == &grow && Holds(grow, 5, 100) && grow.capacity() == 5;
    ok = ok && Holds(shrink = src, 5, 100) && shrink.capacity() == 8;
    ok = ok && Holds(partial = src, 5, 100) && partial.capacity() == 6;
    ok = ok && Holds(src = src, 5, 100);    // self-assignment is a no-op
    if (!ok) { std::printf("00a5e4d0_TEST FAIL\n"); return 1; }
    std::printf("00a5e4d0_TEST PASS\n");
    return 0;
}