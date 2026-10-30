// std::swap(_Tp& a, _Tp& b) -- STLport swap of two update-parameter map entries pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long>>: copy a to a temporary, assign b to a, the temporary to b
// Leaf: ??$swap@U?$pair@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00769CB0, 74 bytes (+ padding to 80), `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (label correction): the catalog's "pair<pair<long,long>,CEngineTextureDecal>" is propagated. The temporary's
//   vtable 0x0125FEE4 is RTTI .?AV?$CParticleUpdateParamDesc@KK@NParticleEngine@@ (base CBaseClass), so the 16-byte
//   element is pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > (key +0; the descriptor's
//   vptr +4, Callback +8, Index +0xC), the value of CParticleUpdateParameters<ul, ul>::UpdateParams (a CVectorMap; its
//   sort helpers live in this 0x0076Axxx range next to other 0x0125FEE4 users). No direct callers (an emitted COMDAT of
//   the map's sort).
// Callee: CBaseClass::~CBaseClass @ 0x0099A2E0 (the temporary's descriptor).
// Shape: STLport 4.6 `_Tp __tmp = __a; __a = __b; __b = __tmp;` -- the pair's and the descriptor's implicit copy
//   constructor and assignment inline (the vptr is set on the temporary, never copied); the temporary is destroyed last.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParamCallback;

template <class A, class B> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4
public:
    CParticleUpdateParamCallback<A, B>* Callback;       // +0x04
    unsigned long                       Index;          // +0x08
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap(pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >&,
                              pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >&);
}