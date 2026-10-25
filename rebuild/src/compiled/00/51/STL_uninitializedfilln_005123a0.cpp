// _STL::uninitialized_fill_n(unsigned short* first, unsigned int n, const unsigned short& x) -- STLport: fill n 16-bit slots with x (the POD path is fill_n) and return the end
// Leaf: ??$uninitialized_fill_n@PAGIG@_STL@@YIPAGPAGIABG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005123A0, 32 bytes, `__fastcall` (first in ecx, n in edx, &x pushed), `ret 4`,
//   returns first + n.
// Identity: the callers are STLport basic_string<unsigned short> (CWideString) members -- landed push_back 0x0099CC80,
//   append 0x0099CB00 and CWideString::operator 0x0099B9A0 call it as `uninitialized_fill_n(_M_finish + 1, n - 1, c)` --
//   plus 0x00514A96. Label correction: the catalog's "vector<unsigned short>::_Ufill" is a propagated Dinkumware
//   spelling; retail STL is STLport 4.x.
// Callees: none.
// Shape: STLport 4.6 verbatim: uninitialized_fill_n -> __uninitialized_fill_n(.., _STLP_VALUE_TYPE) ->
//   __uninitialized_fill_n_aux(.., __true_type) -> fill_n, all inline; fill_n's `for (; __n > 0; --__n, ++__first)
//   *__first = __val;` re-reads the value through its reference every pass (`mov si,[edx]` inside the loop: the store
//   may alias it), the unsigned `__n > 0` guard is `test ; jbe`. The /Gr TU makes the template `__fastcall`.
namespace _STL {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

template <class _Tp> struct __type_traits { typedef __true_type is_POD_type; };

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter __fastcall fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n_aux(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, _Tp1*)
{
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, (_Tp*)0);
}

template unsigned short* __fastcall uninitialized_fill_n<unsigned short*, unsigned int, unsigned short>(unsigned short*, unsigned int, const unsigned short&);

}