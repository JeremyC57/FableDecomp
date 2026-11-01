// Behaviour test for std::vector<CTVertexLandscapeBackground>::_M_fill_insert @ 0x00B6CE00 (copy of the model).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
class CTVertexLandscapeBackground {
public:
    unsigned short X, Y; float Z, NormalX, NormalY, NormalZ; unsigned char UVPad1, V1, U1, UVPad2;
};
namespace std {
struct __false_type { };
struct random_access_iterator_tag { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{ _ForwardIter __cur = __result; for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first); return __cur; }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{ _ForwardIter __cur = __first; for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return __cur; }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{ return __uninitialized_fill_n(__first, __n, __x, __false_type()); }
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
inline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, const random_access_iterator_tag&, _Distance*)
{ for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result; }
template <class _Tp>
inline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{ return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0); }
template <class _ForwardIter, class _Tp>
inline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val) { for (; __first != __last; ++__first) *__first = __val; }
template <class _Tp> class allocator { public: };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base { public: _Tp* _M_start; _Tp* _M_finish; _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer; typedef _Tp* iterator; typedef unsigned int size_type;
    bool overflowed;
    void _M_insert_overflow(pointer, const _Tp&, const __false_type&, size_type, bool = false) { overflowed = true; }
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
    {
        if (__n != 0) {
            if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
                _Tp __x_copy = __x;
                const size_type __elems_after = this->_M_finish - __position;
                pointer __old_finish = this->_M_finish;
                if (__elems_after > __n) {
                    __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                    this->_M_finish += __n;
                    __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                    fill(__position, __position + __n, __x_copy);
                } else {
                    uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                    this->_M_finish += __n - __elems_after;
                    __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                    this->_M_finish += __elems_after;
                    fill(__position, __old_finish, __x_copy);
                }
            } else
                _M_insert_overflow(__position, __x, __false_type(), __n);
        }
    }
};
}
static CTVertexLandscapeBackground V(unsigned short x) { CTVertexLandscapeBackground v; std::memset(&v, 0, sizeof(v)); v.X = x; v.Z = (float)x; v.U1 = (unsigned char)x; return v; }
int main()
{
    CTVertexLandscapeBackground buf[16];
    std::vector<CTVertexLandscapeBackground> v;
    v.overflowed = false;
    v._M_start = buf; v._M_finish = buf + 5; v._M_end_of_storage._M_data = buf + 16;
    for (unsigned short i = 0; i < 5; ++i) buf[i] = V((unsigned short)(i + 1));
    // elems_after (3) > n (2): slide the tail
    v._M_fill_insert(buf + 2, 2, V(9));
    unsigned short want1[] = {1, 2, 9, 9, 3, 4, 5};
    if (v._M_finish != buf + 7) { std::printf("00b6ce00_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (buf[i].X != want1[i] || buf[i].U1 != (unsigned char)want1[i]) { std::printf("00b6ce00_TEST FAIL\n"); return 1; }
    // elems_after (1) <= n (3): fill past the end then move the tail
    v._M_fill_insert(buf + 6, 3, V(7));
    unsigned short want2[] = {1, 2, 9, 9, 3, 4, 7, 7, 7, 5};
    if (v._M_finish != buf + 10) { std::printf("00b6ce00_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 10; ++i) if (buf[i].X != want2[i]) { std::printf("00b6ce00_TEST FAIL\n"); return 1; }
    // no spare capacity: overflow path; n == 0: nothing
    v._M_fill_insert(buf + 10, 7, V(1));
    if (!v.overflowed || v._M_finish != buf + 10) { std::printf("00b6ce00_TEST FAIL\n"); return 1; }
    v.overflowed = false;
    v._M_fill_insert(buf, 0, V(1));
    if (v.overflowed || v._M_finish != buf + 10) { std::printf("00b6ce00_TEST FAIL\n"); return 1; }
    std::printf("00b6ce00_TEST PASS\n");
    return 0;
}