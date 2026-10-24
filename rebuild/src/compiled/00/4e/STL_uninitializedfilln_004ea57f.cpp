// _STL::uninitialized_fill_n(CEngineGraphic* first, unsigned int n, const CEngineGraphic& x) -- STLport uninitialized fill of n 14-byte engine graphics: placement-copy x into each slot, return the end
// Leaf: ??$uninitialized_fill_n
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EA57F, 34 bytes, `ret 4` (/Gr __fastcall: first in ecx, n in edx, x on the stack).
// Label correction: the catalog's `_Fill<CEngineGraphic*,CEngineGraphic>` is the propagated Dinkumware spelling; retail STL is
//   STLport 4.x, whose `uninitialized_fill_n(first, n, x)` dispatches to `__uninitialized_fill_n(first, n, x, _Is_POD())`
//   (inlined here). The element is CEngineGraphic (Ego_r.pdb, 14 bytes, 2-byte packed: `movsd x3 ; movsw`), the element of
//   CReplaceableMeshes' CArray<CEngineGraphic> (its vector's fill-insert / resize helpers live next to it).
// Shape: `for (cur = first; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` -- the unsigned size_type count
//   (`test edx,edx ; jbe`), the placement-new null test per element, the implicit 14-byte copy. Size TU without a frame
//   pointer: `#pragma optimize("s", on)`; the inline helpers (_Construct, __uninitialized_fill_n) are `__forceinline`.
#pragma optimize("s", on)
#include <stddef.h>

inline void* operator new(size_t, void* where) { return where; }

namespace _STL {
struct __false_type { };

#pragma pack(push, 2)
struct CEngineGraphic {                         // Ego_r.pdb, 14 bytes
    unsigned long BankIndex;                    // +0x00
    float         AnimStep;                     // +0x04
    float         RenderSizeX;                  // +0x08
    bool          AdditiveAlpha;                // +0x0c
    unsigned char Type;                         // +0x0d
};
#pragma pack(pop)

template <class T> struct __type_traits { typedef __false_type is_POD_type; };
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x, const __false_type&)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x)
{
    typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
    return __uninitialized_fill_n(first, n, x, _Is_POD());
}
template CEngineGraphic* __fastcall uninitialized_fill_n<CEngineGraphic*, unsigned int, CEngineGraphic>(CEngineGraphic* first, unsigned int n,
                                                                                                         const CEngineGraphic& x);
}