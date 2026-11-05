// std::__destroy_aux(P* first, P* last, const __false_type&) -- STLport _Destroy over pair<long, CCountedPointer<CNavConnectorNode> >:
//   only the counted pointer has a destructor (release: --RefCount, at zero DeleteFunc(Data) and delete Info; Data = Info = 0)
// Leaf: ??$__destroy_aux@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@@std@@YIXPAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@0@0ABU__false_type@0@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80E40, 80 bytes, __fastcall (first ecx, last edx, the tag by reference on the stack, never
//   read), `ret 4`; operator delete 0x00BFE9BC.
// Identity: catalogued "std::_Destroy_range" over pair<long, CCountedPointer<CRegionConnectionNode> > (Dinkumware spelling,
//   wrong pointee). It is the _M_clear/_Destroy callee of the landed vector<pair<long, CCountedPointer<CNavConnectorNode> > >
//   ::_M_insert_overflow 0x00A838F0, and its thirteen other callers are the nav quad-tree module (CNavQuadTree::OnNavMapUnload
//   0x00A79290 ...); the CRegionConnectionNode overflow 0x008294F0 calls its own destroy 0x00519050. Ego_r has
//   CVectorMap<long, CCountedPointer<CNavConnectorNode> >. Family pass 2 (agent951 G055) agrees.
extern "C" void* __cdecl malloc(unsigned int size);
inline void* operator new(unsigned int, void* p) { return p; }
#include "engine/CCountedPointer.h"
class CNavConnectorNode;
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
typedef pair<long, CCountedPointer<CNavConnectorNode> > _Pair;
template void __fastcall __destroy_aux<_Pair*>(_Pair*, _Pair*, const __false_type&);
}
