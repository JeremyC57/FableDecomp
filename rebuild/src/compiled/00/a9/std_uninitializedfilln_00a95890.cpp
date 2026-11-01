// std::uninitialized_fill_n(CCountedPointer<C3DGroup2>* first, unsigned int n, const CCountedPointer<C3DGroup2>& x) -- copy-construct x into n raw slots, return the end
// Leaf: ??$uninitialized_fill_n@PAV?$CCountedPointer@VC3DGroup2@@@@IV1@@std@@YIPAV?$CCountedPointer@VC3DGroup2@@@@PAV1@IABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A95890, 46 bytes, /Gr `__fastcall` (first ecx, n edx, x on the stack), `ret 4`.
// Identity (proven): the sole caller (call at 0x00A98010) is the landed vector<CCountedPointer<C3DGroup2> >::_M_fill_insert
//   0x00A97F70, which reloc_bindings binds from the landed vector<CCountedPointer<C3DGroup2> >::resize 0x00A985A0 (its
//   __destroy_aux callee 0x00A969D0 is bound for CCountedPointer<C3DGroup2>); the element is Ego_r.pdb
//   CDeviceIndependent::Groups `CArray<CCountedPointer<C3DGroup2> >`. One stack argument (`ret 4`) = the three-argument
//   front end `uninitialized_fill_n`, not the tagged `__uninitialized_fill_n`. The manifest label
//   "_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > ...>" is a Dinkumware spelling
//   propagated across byte-identical bodies (an 8-byte stride here, not a 16-byte pair); the landed _M_fill_insert binds
//   this address as the stand-in N00a97f70::__uninitialized_fill_n<N00a97f70::Element> (NAME_FIXES).
// Body: STLport 4.6 `return __uninitialized_fill_n(first, n, x, __false_type());` with the inner loop inline -- placement
//   new's null test, CCountedPointer's copy constructor (Data, Info, `if (Info) ++Info->RefCount`).
#include "engine/CCountedPointer.h"
class C3DGroup2;
#include <new>
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
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
template CCountedPointer<C3DGroup2>* __fastcall uninitialized_fill_n(CCountedPointer<C3DGroup2>*, unsigned int, const CCountedPointer<C3DGroup2>&);
}