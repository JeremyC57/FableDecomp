// std::uninitialized_fill_n(ENavigatorType* first, unsigned int n, const ENavigatorType& x) -- STLport uninitialized_fill_n front end with the __false_type helper inline: placement-copy x into n slots, return first + n
// Leaf: ??$uninitialized_fill_n@PAW4ENavigatorType@@
//
// Retail 0x006727F0, 34 bytes (+ int3 padding to 48), `ret 4` (/Gr: first in ecx -> eax as `cur`, n in edx counted down,
//   &x the one stack argument).
// Caller: 0x00672762 in vector<ENavigatorType>::_M_fill_insert 0x006726B0 (reached from vector<ENavigatorType>::resize
//   0x00672660, which GFSerialiseVectorBinaryIn<ENavigatorType> 0x00672530 calls) -- the element type is the enum, not
//   the catalog's `unsigned long`.
// Shape: STLport _uninitialized.h -- `uninitialized_fill_n(first, n, x)` returns `__uninitialized_fill_n(first, n, x,
//   _IS_POD_ITER(first, _Tp*))`; STLport has no __type_traits specialization for an enum, so the __false_type arm runs:
//   `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` with placement new's null test (`test eax,eax ; je`) and x re-read
//   every pass.  The helper needs __forceinline (a plain inline helper stays a call).  The retail TU is /Gr, written as
//   an explicit __fastcall.
// Label: catalog "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" is a propagated Dinkumware spelling.
inline void* operator new(unsigned int, void* p) { return p; }

enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace std {
struct __false_type { };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
}

template ENavigatorType* __fastcall std::uninitialized_fill_n<ENavigatorType*, unsigned int, ENavigatorType>(
    ENavigatorType*, unsigned int, const ENavigatorType&);