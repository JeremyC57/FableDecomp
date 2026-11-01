// std::uninitialized_fill_n(CCountedPointer<CEngineSceneGrid>* first, unsigned int n, const CCountedPointer<CEngineSceneGrid>& x) -- copy-construct x into n raw slots, return the end
// Leaf: ??$uninitialized_fill_n@PAV?$CCountedPointer@VCEngineSceneGrid@@@@IV1@@std@@YIPAV?$CCountedPointer@VCEngineSceneGrid@@@@PAV1@IABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B96150, 46 bytes, /Gr `__fastcall` (first ecx, n edx, x on the stack), `ret 4`.
// Identity (proven): the sole caller (call at 0x00B97F30) is the landed vector<CCountedPointer<CEngineSceneGrid> >::_M_fill_insert
//   0x00B97E90, which reloc_bindings binds from the landed vector<CCountedPointer<CEngineSceneGrid> >::resize 0x00B98170; that
//   resize is called by CEngineSceneMap::Initialise 0x00B949A0 on +0x18 = Ego_r.pdb CEngineSceneMap::GridLevels
//   `CArray<CCountedPointer<CEngineSceneGrid> >`. One stack argument (`ret 4`) = the three-argument front end
//   `uninitialized_fill_n`, not the tagged `__uninitialized_fill_n`. The manifest label
//   "_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > ...>" is a Dinkumware spelling
//   propagated across byte-identical bodies (an 8-byte stride here, not a 16-byte pair); the landed _M_fill_insert binds
//   this address as uninitialized_fill_n<CCountedPointer<CUnprovenPointee_00b97cb0>*, ...> (NAME_FIXES).
// Body: STLport 4.6 `return __uninitialized_fill_n(first, n, x, __false_type());` with the inner loop inline -- placement
//   new's null test, CCountedPointer's copy constructor (Data, Info, `if (Info) ++Info->RefCount`).
#include "engine/CCountedPointer.h"
class CEngineSceneGrid;
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
template CCountedPointer<CEngineSceneGrid>* __fastcall uninitialized_fill_n(CCountedPointer<CEngineSceneGrid>*, unsigned int, const CCountedPointer<CEngineSceneGrid>&);
}