// std::__destroy_aux(CCountedPointer<CParticleSimulation>* first, CCountedPointer<CParticleSimulation>* last, const __false_type&) -- STLport non-trivial destroy loop: run the inline counted-pointer destructor on every element of [first, last)
// Leaf: ??$__destroy_aux@PAV?$CCountedPointer@VCParticleSimulation@@@@@std@@YIXPAV?$CCountedPointer@VCParticleSimulation@@@@0ABU__false_type@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B833F0, length 76 bytes, `__fastcall` (first ecx, last edx, the
//   __false_type reference on the stack), `ret 4`.
// Identity: the landed CArray<CCountedPointer<CParticleSimulation> >::DeallocateMemory 0x00B83D30 (FableWin.pdb name) binds
//   this address as `std::__destroy_aux<...>` (vector::erase / ~vector -> _Destroy(first, last) -> this out-of-line
//   __destroy_aux(__false_type)); 16 call sites, nearly all in the cloth-simulation TU 0x00B81E60..0x00B84190 (incl. the
//   ~CEngineClothSimulationMeshBase bodies, ParticleSims = Ego_r CArray<CCountedPointer<CParticleSimulation> >). The
//   third argument is the STLport tag reference, not the by-value allocator of the Dinkumware `_Destroy_range` the catalog
//   name DestroyRange_CountedPointer and the manifest carried.
// Shape: STLport 4.6 `for (; __first != __last; ++__first) _Destroy(&*__first);` with the CCountedPointer destructor
//   inline: release the info block (RefCount at +0; at zero the delete function at +4 with Data (+8) in ecx, then operator
//   delete @ 0x00BFE9BC) and null both fields.
#include "engine/CCountedPointer.h"

class CParticleSimulation;
typedef CCountedPointer<CParticleSimulation> CParticleSimulationPtr;

struct __false_type { __false_type() {} };

namespace std {
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
}

template void __fastcall std::__destroy_aux<CParticleSimulationPtr*>(CParticleSimulationPtr*, CParticleSimulationPtr*,
    const __false_type&);
