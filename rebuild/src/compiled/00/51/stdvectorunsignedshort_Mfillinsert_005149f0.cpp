// std::vector<unsigned short>::_M_fill_insert(unsigned short* pos, unsigned int n, const unsigned short& x) -- STLport vector insert(pos, n, x) for a POD element: memmove the tail in place when capacity allows, else grow through the __true_type _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005149F0, 269 bytes, thiscall `ret 0Ch`. Callers (COMDAT-folded across TUs):
//   0x00515BB0 (jmp: insert(pos, n, x) / resize), 0x0051794C, 0x006E3F06, 0x0099BAC6, 0x0099BBB7, 0x00A90A4C, 0x00BE2D6C.
//   The catalog label "vector_insert_erase" is wrong (no erase: this is the fill insert).
// Callees: memmove @ 0x00BFEAE6 (the __true_type uninitialized copies and __copy_backward_ptrs, via __copy_trivial /
//   __copy_trivial_backward), uninitialized_fill_n @ 0x005123A0 (tag-less, `ret 4`), fill @ 0x0050E500,
//   _M_insert_overflow(.., const __true_type&, ..) @ 0x005135D0.
// Shape: STLport 4.6 `_M_fill_insert` verbatim; unsigned short is POD so every `_IsPODType()` / `_TrivialAss()` tag is a
//   __true_type and the copies fold to memmove (`(last == first) ? result : memmove(..)` / `(_Num > 0) ? memmove(..)`).
//   The two helpers retail calls are same-TU noinline bodies under `auto_inline(off)`; VC7.1 then homes x_copy in the
//   dead `&x` argument slot and elems_after (later n * 2) in the dead `pos` slot, as retail does.
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                         const __true_type&)
{
    return (_ForwardIter)__copy_trivial(__first, __last, __result);
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __true_type&)
{
    return (_BidirectionalIter2)__copy_trivial_backward(__first, __last, __result);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

#pragma auto_inline(off)
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x005123A0
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x0050E500
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef __true_type _TrivialAss;
    typedef __true_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x005135D0
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<unsigned short>::_M_fill_insert(unsigned short*, unsigned int, const unsigned short&);
}