// behaviour test for 0x00440F00 std::vector<long>::_M_fill_insert: in-place insert with a long tail (elems_after > n), with a short tail (fill past the end, then move the tail), n == 0 no-op, and the growth path through _M_insert_overflow
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stddef.h>

namespace stlp_test {
struct __true_type { };
static int gOverflows = 0;
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? std::memmove((char*)__result - _Num, __first, _Num) : __result;
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first) *__first = __val;
    return __first;
}
template <class _ForwardIter, class _Tp>
void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first) *__first = __val;
}
template <class _Tp> class vector {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend = false)
    {
        ++gOverflows;
        size_type __old = size_type(_M_finish - _M_start);
        size_type __len = __old + (__old > __fill_len ? __old : __fill_len);
        pointer __s = (pointer)std::malloc(__len * sizeof(_Tp));
        pointer __f = (pointer)__copy_trivial(_M_start, __position, __s);
        __f = fill_n(__f, __fill_len, __x);
        if (!__atend) __f = (pointer)__copy_trivial(__position, _M_finish, __f);
        std::free(_M_start);
        _M_start = __s; _M_finish = __f; _M_end_of_storage = __s + __len;
    }
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
    {
        if (__n != 0) {
            if (size_type(_M_end_of_storage - _M_finish) >= __n) {
                _Tp __x_copy = __x;
                const size_type __elems_after = _M_finish - __position;
                pointer __old_finish = _M_finish;
                if (__elems_after > __n) {
                    __copy_trivial(_M_finish - __n, _M_finish, _M_finish);
                    _M_finish += __n;
                    __copy_trivial_backward(__position, __old_finish - __n, __old_finish);
                    fill(__position, __position + __n, __x_copy);
                } else {
                    fill_n(_M_finish, __n - __elems_after, __x_copy);
                    _M_finish += __n - __elems_after;
                    __copy_trivial(__position, __old_finish, _M_finish);
                    _M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            } else
                _M_insert_overflow(__position, __x, __true_type(), __n);
        }
    }
};
}
typedef long T;
static T mk(unsigned int v) { return (T)(size_t)v; }
static int Same(const stlp_test::vector<T>& v, const unsigned int* want, unsigned int n)
{
    if ((unsigned int)(v._M_finish - v._M_start) != n) return 0;
    for (unsigned int i = 0; i < n; ++i) if (v._M_start[i] != mk(want[i])) return 0;
    return 1;
}
static int Check()
{
    stlp_test::vector<T> v;
    v._M_start = (T*)std::malloc(16 * sizeof(T));
    v._M_finish = v._M_start; v._M_end_of_storage = v._M_start + 16;
    for (unsigned int i = 1; i <= 5; ++i) *v._M_finish++ = mk(i);
    v._M_fill_insert(v._M_start + 1, 2, mk(9));                  // elems_after 4 > 2: shift the tail, fill the gap
    unsigned int w1[7] = { 1, 9, 9, 2, 3, 4, 5 };
    if (!Same(v, w1, 7) || stlp_test::gOverflows != 0) return 1;
    v._M_fill_insert(v._M_start + 5, 4, mk(7));                  // elems_after 2 <= 4: fill past the end, then move the tail
    unsigned int w2[11] = { 1, 9, 9, 2, 3, 7, 7, 7, 7, 4, 5 };
    if (!Same(v, w2, 11) || stlp_test::gOverflows != 0) return 2;
    v._M_fill_insert(v._M_start, 0, mk(3));                      // n == 0: nothing
    if (!Same(v, w2, 11)) return 3;
    v._M_fill_insert(v._M_start + 11, 6, mk(8));                 // only 5 spare: grow through _M_insert_overflow
    unsigned int w3[17] = { 1, 9, 9, 2, 3, 7, 7, 7, 7, 4, 5, 8, 8, 8, 8, 8, 8 };
    if (!Same(v, w3, 17) || stlp_test::gOverflows != 1) return 4;
    std::free(v._M_start);
    return 0;
}
int main()
{
    if (Check() != 0) { std::printf("00440f00_TEST FAIL\n"); return 1; }
    std::printf("00440f00_TEST PASS\n");
    return 0;
}