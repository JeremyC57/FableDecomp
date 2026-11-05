// std::_Destroy(CDefendingCombatantInfo::CProng* first, CDefendingCombatantInfo::CProng* last) -- STLport range destroy: run each element's destructor
// Leaf: ??$_Destroy@VCProng@CDefendingCombatantInfo@@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F2B00, 34 bytes, `__fastcall` (/Gr: first in ecx, last in edx), `ret`.
// Identity (probable): its only caller is the landed vector<CDefendingCombatantInfo::CProng>::operator= 0x006F3C20 (the K104 0x006F3450 element; stand-in ?_Destroy@@YIXPAUSSlot@@0@Z).
// Callees: the element destructor's one call -- ~CBaseIntelligentPointer 0x00A01C10 on POccupier +0x14.
// Shape (family K118, representative 0x004D0EE0): STLport 4.6 `_Destroy(first, last)` -> `__destroy_aux(..., __false_type)` loop,
//   emitted out of line (the vector operator= calls it); the element's implicit destructor inlines to the member's call.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThingCreatureBase;
class CDefendingCombatantInfo {
public:
    class CProng {                                       // Ego_r.pdb 28 bytes
    public:
        unsigned char _head[0x14];                       // +0x00 Blocked, Prong (C3DVector), ProngBlockageTimer (not touched here)
        CIntelligentPointer<CThingCreatureBase> POccupier;   // +0x14
    };
};
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void __fastcall _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
}
template void __fastcall std::_Destroy<CDefendingCombatantInfo::CProng >(CDefendingCombatantInfo::CProng*, CDefendingCombatantInfo::CProng*);