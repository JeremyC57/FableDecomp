// _STL::__linear_insert(ENavigatorType* first, ENavigatorType* last, ENavigatorType val, less<ENavigatorType> comp) -- STLport insertion-sort step for the creature def's navigator types: a value below the first element shifts the run up one slot and goes first, anything else walks down unguarded
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006721C0, length 72 bytes (+ int3 padding), `ret 8`.
// Callers: __final_insertion_sort 0x00672100 (landed; 0x0067212E, 0x00672168), part of sort() over CThingCreatureDef's
//   vector<ENavigatorType> (CThingCreatureDef::Transfer sorts then uniques it).
// Callees: __copy_backward_ptrs(first, last, result, const __false_type&) @ 0x00672210 (landed).
// Label: catalog "std::InsertSorted_Pair" is descriptive; the body is STLport 4.x `__linear_insert` verbatim:
//   `if (__comp(__val, *__first)) { copy_backward(__first, __last, __last + 1); *__first = __val; } else
//   __unguarded_linear_insert(__last, __val, __comp);` with copy_backward, __copy_backward_aux and
//   __unguarded_linear_insert inline (`__next = __last; --__next; while (__comp(__val, *__next)) { *__last = *__next;
//   __last = __next; --__next; } *__last = __val;`); less<> on the enum is the signed `jge`/`jl`.
// Shape: /Gr algorithm TU (`__fastcall` template: first in ecx, last in edx, val and the empty comparator on the stack).
//   The memcpy-safety tag handed to the out-of-line __copy_backward_ptrs is `_IsOKToMemCpy(...)._Ret()`: a POD
//   __false_type returned by value in al (`xor al,al`) and stored into the dead `val` slot (`mov [esp+0Ch],al`).
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum (signed compares)

namespace _STL {
struct __true_type { };
struct __false_type { };

template <class _Tp> struct less {
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp1, class _Tp2> struct _BothPtrType {
    static __false_type _Ret() { return __false_type(); }
};
template <class _Tp1, class _Tp2> struct _BothPtrType<_Tp1*, _Tp2*> {
    static __true_type _Ret() { return __true_type(); }
};
template <class _Tp1, class _Tp2> struct _OKToMemCpy {
    typedef __false_type _Type;                            // __type_traits<enum>: no trivial assignment
    static _Type _Ret() { return _Type(); }
};
template <class _Tp1, class _Tp2> inline _OKToMemCpy<_Tp1, _Tp2> _IsOKToMemCpy(_Tp1*, _Tp2*) { return _OKToMemCpy<_Tp1, _Tp2>(); }
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, const __false_type&);   // 0x00672210

template <class _BI1, class _BI2>
inline _BI2 __copy_backward_aux(_BI1 __first, _BI1 __last, _BI2 __result, const __true_type&)
{
    return __copy_backward_ptrs(__first, __last, __result, _IsOKToMemCpy(__value_type(__first), __value_type(__result))._Ret());
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward_aux(__first, __last, __result, _BothPtrType<_BidirectionalIter1, _BidirectionalIter2>::_Ret());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    } else
        __unguarded_linear_insert(__last, __val, __comp);
}
}

template void __fastcall _STL::__linear_insert<ENavigatorType*, ENavigatorType, _STL::less<ENavigatorType> >(ENavigatorType*, ENavigatorType*, ENavigatorType, _STL::less<ENavigatorType>);