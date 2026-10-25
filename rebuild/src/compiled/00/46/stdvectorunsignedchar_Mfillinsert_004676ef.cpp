// std::vector<unsigned char>::_M_fill_insert(unsigned char* position, unsigned int n, const unsigned char& x) -- STLport insert of n copies of a byte: memmove the tail inside the spare capacity and memset the gap, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@E
// Retail: Fable.exe (TLC, VC7.1) @ 0x004676EF, 285 bytes, thiscall `ret 0Ch`; size TU with a frame pointer.
// Identity: same TU as the landed vector<unsigned char>::_M_insert_overflow 0x00467831 (its only caller is this body);
//   the catalog label "Vector_FillInsert" is behavioural.
// Callees: __uninitialized_copy(first, last, result, const __true_type&) 0x0046780E (__fastcall, `ret 8`: the
//   __copy_trivial memmove), memmove 0x00BFEAE6 (the inlined __copy_trivial_backward), _M_insert_overflow 0x00467831.
// Shape: STLport 4.x `_M_fill_insert` with the POD (`__true_type`) helpers. x_copy is a byte homed in the dead `x`
//   slot ([ebp+13h]); both `fill` loops and the inlined uninitialized_fill_n become VC7.1's byte-replicating
//   `rep stosd ; rep stosb` (the two fills tail-merge); `__copy_backward_ptrs` is __copy_trivial_backward (`jle` on the
//   signed byte count). The two out-of-line __uninitialized_copy calls take their tag from `_IsPOD<T>::_Answer()`, a
//   by-value return: zero-stored then copied in place (`stosb ; mov al,[ebp+0Fh] ; mov [ebp+0Fh],al`); the
//   _M_insert_overflow tag is the plain `_IsPODType()` temporary (zero-stored only).
// Model: STLport's three vector words; `_IsPOD<T>` is STLport's type_traits helper (`static _Type _Answer()`).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);   // 0x00BFEAE6

namespace std {

struct __true_type { };

template <class _Tp> struct _IsPOD {
    typedef __true_type _Type;
    static __forceinline _Type _Answer() { return _Type(); }
};

template <class _Tp> _Tp* __fastcall __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result,
                                                         const __true_type&);                              // 0x0046780E

__forceinline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _Tp> __forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial_backward(__first, __last, __result);
}

template <class _Tp> __forceinline void fill(_Tp* __first, _Tp* __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> __forceinline _Tp* fill_n(_Tp* __first, unsigned int __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _Tp> __forceinline _Tp* __uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

template <class _Tp> __forceinline _Tp* uninitialized_fill_n(_Tp* __first, unsigned int __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    typedef __true_type _IsPODType;
    typedef __true_type _TrivialAss;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x00467831
    void _M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x);
};

template <class _Tp>
void vector<_Tp>::_M_fill_insert(pointer __position, unsigned int __n, const _Tp& __x)
{
    if (__n != 0) {
        if ((unsigned int)(_M_end_of_storage - _M_finish) >= __n) {
            _Tp __x_copy = __x;
            const unsigned int __elems_after = _M_finish - __position;
            pointer __old_finish = _M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, _IsPOD<_Tp>::_Answer());
                _M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, _M_finish, _IsPOD<_Tp>::_Answer());
                _M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<unsigned char>::_M_fill_insert(unsigned char*, unsigned int, const unsigned char&);

}