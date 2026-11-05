// std::uninitialized_fill_n(CCountedPointer<CEngineSimulationBase>* first, unsigned int n, const CCountedPointer<CEngineSimulationBase>& x) -- copy-construct x into n raw slots, return the end
// Leaf: ??$uninitialized_fill_n@PAV?$CCountedPointer@VCEngineSimulationBase@@@@IV1@@std@@YIPAV?$CCountedPointer@VCEngineSimulationBase@@@@PAV1@IABV1@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b82e90, 46 bytes, /Gr `__fastcall` (first ecx, n edx, x on the stack), `ret 4`.
// Identity (proven): its caller is vector<CCountedPointer<CEngineSimulationBase> >::_M_fill_insert 0x00B83E70 (bound in reloc_bindings; its __uninitialized_copy 0x00B82DF0 is bound only with a descriptive pointee).
// Body: STLport 4.6 `return __uninitialized_fill_n(first, n, x, __false_type());` with the inner loop inline -- placement new's null
//   test, CCountedPointer's copy constructor (Data, Info, `if (Info) ++Info->RefCount`).
class CEngineSimulationBase;
#include "engine/CCountedPointer.h"
inline void* operator new(unsigned int, void* __p) { return __p; }
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
template CCountedPointer<CEngineSimulationBase>* __fastcall uninitialized_fill_n(CCountedPointer<CEngineSimulationBase>*, unsigned int, const CCountedPointer<CEngineSimulationBase>&);
}