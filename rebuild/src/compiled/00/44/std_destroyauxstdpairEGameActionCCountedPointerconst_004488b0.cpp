// std::__destroy_aux<std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*>(std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >* first, std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >* last, const __false_type&) -- destroy a range of counted-pointer elements
// Leaf: ??$__destroy_aux@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@@std@@YIXPAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@0@0ABU__false_type@0@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x004488B0, 77 bytes, __fastcall (first ecx, last edx, the __false_type tag on the stack, `ret 4`);
// callers include the CGamePlayerInterface destructor (0x00446C9A on the Controls CVectorMap<EGameAction, CCountedPointer<const
// CInputTypeBase>> at +0x7a8); 11 callers in all. STLport _Destroy(first, last) for a 12-byte element whose only non-trivial member is a
// CCountedPointer (inline release: if Info, --RefCount, at zero DeleteFunc(Data) __fastcall + operator delete(Info)
// 0x00BFE9BC; Data = Info = 0). The catalog label pair<long, CCountedPointer<CRegionConnectionNode>> names a byte-identical (folded) twin; the element type here follows the CGamePlayerInterface destructor.
// Layout: pair {EGameAction first +0, CCountedPointer second +4/+8}.
// Shape: /O2 (16-aligned entry, loop-head pad).
#include "engine/CCountedPointer.h"
class CInputTypeBase;
enum EGameAction { GAME_ACTION_NONE };
namespace std {
struct __false_type {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template void __fastcall __destroy_aux<std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*>(std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, std::pair<EGameAction, CCountedPointer<const CInputTypeBase> >*, const __false_type&);
}