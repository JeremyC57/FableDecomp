// std::uninitialized_fill_n<CIntelligentPointer<NParticleEngine::CParticleEmitter>*,unsigned int,
// CIntelligentPointer<NParticleEngine::CParticleEmitter> >(first, n, x) -- STLport front end: dispatch to
// __uninitialized_fill_n with the element's __false_type
// Leaf: ??$uninitialized_fill_n@
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

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    CBaseObject* PItem;                               // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other);   // 0x00654DAF
};

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
