// std::__copy_backward(CCountedPointer<T>* first, CCountedPointer<T>* last, CCountedPointer<T>* result, const random_access_iterator_tag&, int*) -- (CCountedPointer<NUISystem::CComponent>) assign last-first counted pointers back to front into the range ending at result; returns the new result
// Leaf: ??$__copy_backward@PAV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCComponent@NUISystem@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00534F80, 119 bytes (+ int3 padding), `__fastcall` (first in ecx, last in edx; result,
//   the tag reference and the null distance pointer on the stack, `ret 0Ch`).
// Identity (proven): std::__copy_backward<CCountedPointer<NUISystem::CComponent>*, CCountedPointer<NUISystem::CComponent>*, int> (STLport 4.6 _algobase.h);
//   evidence: its caller 0x00535B10 is vector<CCountedPointer<NUISystem::CComponent> >::insert(iterator, const T&), which the landed CComponent::AddChildFront 0x00533BA0 runs on Children (+0xB0) and CSpellContainer's slot 142 0x00545F00 also calls; the other caller 0x00535360 is the same vector's small forwarder next to it.
// Label: the manifest's "_Copy_backward<CCountedPointer<C3DGroup2>*,CCountedPointer<class_C3DGroup2>*>" is propagated and names the wrong element (family J094: five byte-identical bodies,
//   one COMDAT per pointee; retail has no identical-code folding).
// Callees: the count block's DeleteFunc (`call [Info+4]`, Data in ecx) and operator delete 0x00BFE9BC, from the inlined
//   CCountedPointer::operator= (ShareData: nothing when both share one block, else release the old block, then share).
// Body: `for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;`.
// Layout: CCountedPointer<T> is 8 bytes {Data +0, Info +4}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8} (Ego_r.pdb).
// Model: J051's counted-pointer assignment (agent953) on the real STLport signature.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
#include "engine/CCountedPointer.h"
namespace NUISystem { class CComponent; }
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
typedef CCountedPointer<NUISystem::CComponent > _Elem_00534f80;
template _Elem_00534f80* __fastcall __copy_backward<_Elem_00534f80*, _Elem_00534f80*, int>(_Elem_00534f80*, _Elem_00534f80*, _Elem_00534f80*,
                                                                    const random_access_iterator_tag&, int*);
}   // namespace std