// std::_Destroy(std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >* first, std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >* last) -- STLport range destroy: run each element's destructor
// Leaf: ??$_Destroy@U?$pair@KV?$CParticleUpdateParamDesc@KK@NParticleEngine@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E2200, 34 bytes, `__fastcall` (/Gr: first in ecx, last in edx), `ret`.
// Identity (probable): its only caller is the landed vector<pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > >::operator= 0x006E22F0 (the K104 0x006E2230 element).
// Callees: the element destructor's one call -- ~CBaseClass 0x0099A2E0 on the desc at +4.
// Shape (family K118, representative 0x004D0EE0): STLport 4.6 `_Destroy(first, last)` -> `__destroy_aux(..., __false_type)` loop,
//   emitted out of line (the vector operator= calls it); the element's implicit destructor inlines to the member's call.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
namespace NParticleEngine {
template <class A, class B> class CParticleUpdateParamCallback;
template <class A, class B> class CParticleUpdateParamDesc : public CBaseClass {   // Ego_r.pdb 12 bytes
public:
    CParticleUpdateParamCallback<A, B>* Callback;          // +0x04
    unsigned long Index;                                 // +0x08
};
}
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
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
template void __fastcall std::_Destroy<std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > >(std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >*, std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >*);