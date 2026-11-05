// std::uninitialized_fill_n<CIntelligentPointer<NParticleEngine::CParticleEmitter>*,unsigned int,
// CIntelligentPointer<NParticleEngine::CParticleEmitter> >(first, n, x) -- STLport front end: dispatch to
// __uninitialized_fill_n with the element's __false_type
// Leaf: ??$uninitialized_fill_n@
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006556F2, 19 bytes, `__fastcall` (first in ecx and n in edx passed straight
//   through, the value reference on the stack, `ret 4`); the empty tag is homed in the high byte of the value slot
//   (`lea eax,[ebp+0Bh]`). No callers (an out-of-line copy of the inline front end).
// Identity: its only callee is 0x00654E90, std::__uninitialized_fill_n<CIntelligentPointer<NParticleEngine::
//   CParticleEmitter>*,unsigned int,..>(first, n, x, const __false_type&) (renamed in this pass: callers are the landed
//   vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > _M_insert_overflow 0x006566C3 / _M_fill_insert
//   0x006568F2), and the body is exactly STLport's `return __uninitialized_fill_n(first, n, x, _Is_POD(..)._Answer())`
//   forwarding. Was filed as vector<EWeaponClass>::_Umove<EWeaponClass*> (a propagated Dinkumware name) with a
//   __stdcall stand-in body.
#pragma optimize("s", on)

#include <new>

class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


namespace std {
struct __false_type { __false_type() { } };


template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);   // 0x00654E90

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template CIntelligentPointer<NParticleEngine::CParticleEmitter>* __fastcall uninitialized_fill_n(
    CIntelligentPointer<NParticleEngine::CParticleEmitter>*, unsigned int, const CIntelligentPointer<NParticleEngine::CParticleEmitter>&);
}
