// std::vector<CReverbEnvironmentEntry>::_M_fill_insert(CReverbEnvironmentEntry* position, unsigned int n, const CReverbEnvironmentEntry& x) -- STLport insert of n copies of a reverb environment: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCReverbEnvironmentEntry@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046C022, 326 bytes, thiscall `ret 0Ch`; size TU with a frame pointer.
// Identity: the only caller is vector<CReverbEnvironmentEntry>::resize(n, x) 0x0046BF98 (insert(end(), n - size(), x)),
//   itself called only by resize(n) 0x0046BF72 (landed: GFSerialiseVectorBinaryIn<CReverbEnvironmentEntry>). The forge
//   label "Vector_Insert" (described as CAppearanceModifierGraphics::CEntry) is wrong about the element.
// Callees: uninitialized_fill_n(first, n, x) 0x0046C16A (__fastcall), _M_insert_overflow(position, x, const __false_type&,
//   n, atend) 0x0046C18C.
// Shape: STLport 4.x `_M_fill_insert` verbatim, as 0x0046AD59: `x_copy` is an 84-byte frame copy (`rep movsd` x0x15), the
//   element count is a signed `idiv` by 0x54, the inlined `_Construct` is the null-checked placement copy, __copy_backward
//   and both `fill` loops copy with `rep movsd`; only the _M_insert_overflow call materialises its tag (`xor eax,eax ; stosb`).
// Model: CReverbEnvironmentEntry is the PDB's 21-float POD (as the landed resize(n) 0x0046BF72); three STLport vector words.
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* where) { return where; }

class CReverbEnvironmentEntry { public:
    float Params[21];                                    // +0x00
};

namespace std {

struct __false_type { };
struct random_access_iterator_tag { };

CReverbEnvironmentEntry* __fastcall uninitialized_fill_n(CReverbEnvironmentEntry* first, unsigned int n, const CReverbEnvironmentEntry& x);  // 0x0046C16A

template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}

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
                            bool __atend = false);                                                    // 0x0046C18C
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

template void vector<CReverbEnvironmentEntry>::_M_fill_insert(CReverbEnvironmentEntry*, unsigned int, const CReverbEnvironmentEntry&);

}