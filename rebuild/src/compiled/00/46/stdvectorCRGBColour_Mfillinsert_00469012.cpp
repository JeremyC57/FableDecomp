// std::vector<CRGBColour>::_M_fill_insert(CRGBColour* position, unsigned int n, const CRGBColour& x) -- STLport insert of n copies of a colour: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@UCRGBColour@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00469012, 311 bytes, thiscall `ret 0Ch`; size TU with a frame pointer.
// Callees: _Construct<CRGBColour>(p, value) 0x0045BB8F (__fastcall placement copy, landed), uninitialized_fill_n(first, n,
//   x) 0x00469149 (__fastcall, landed), _M_insert_overflow(position, x, const __false_type&, n, atend) 0x0046916B (landed).
// Shape: STLport 4.x `_M_fill_insert` verbatim for a non-POD element (CRGBColour has a user copy constructor, so
//   `_IsPODType` and `_TrivialAss` are __false_type): `x_copy` lives in the x slot (copied byte by byte, R G B then A), the
//   inlined __uninitialized_copy loops keep their cursors in the dead n / position slots and one frame word, the
//   __copy_backward and both `fill` loops are inlined with the colour's implicit (dword) assignment, the front-end
//   uninitialized_fill_n result is dropped (`_M_finish += n - elems_after` follows), and only the _M_insert_overflow call
//   materialises its tag: `__false_type()` with NO user constructor is value-initialised (`xor eax,eax ; stosb` into the
//   high byte of the n slot, eax shared with the `atend = false` push).
// Model: the three STLport vector words; CRGBColour is the PDB's 4-byte B, G, R, A record with a user copy constructor
//   (`__forceinline`: the size TU otherwise calls it out of line for x_copy).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CRGBColour {                                      // D3DCOLOR byte order
    unsigned char B, G, R, A;
    __forceinline CRGBColour(const CRGBColour& other) { R = other.R; G = other.G; B = other.B; A = other.A; }
};

namespace std {

struct __false_type { };
struct random_access_iterator_tag { };

void __fastcall _Construct(CRGBColour* p, const CRGBColour& value);                                  // 0x0045BB8F
CRGBColour* __fastcall uninitialized_fill_n(CRGBColour* first, unsigned int n, const CRGBColour& x); // 0x00469149

template <class _Tp> __forceinline _Tp* __uninitialized_copy(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    _Tp* __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(__cur, *__first);
    return __cur;
}

template <class _Tp> __forceinline _Tp* __copy_backward(_Tp* __first, _Tp* __last, _Tp* __result,
                                                        const random_access_iterator_tag&, int*)
{
    for (int __n = int(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _Tp> __forceinline _Tp* __copy_backward_ptrs(_Tp* __first, _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> __forceinline void fill(_Tp* __first, _Tp* __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp> class vector {
public:
    typedef _Tp* pointer;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend = false);                                                    // 0x0046916B
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
                __uninitialized_copy(_M_finish - __n, _M_finish, _M_finish, __false_type());
                _M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, _M_finish, __false_type());
                _M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template void vector<CRGBColour>::_M_fill_insert(CRGBColour*, unsigned int, const CRGBColour&);

}