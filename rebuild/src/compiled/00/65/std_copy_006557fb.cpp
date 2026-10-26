// std::__copy(CIntelligentPointer<NParticleEngine::CParticleEmitter>* first, CIntelligentPointer<NParticleEngine::CParticleEmitter>* last, CIntelligentPointer<NParticleEngine::CParticleEmitter>* result, const random_access_iterator_tag&, int*) -- STLport counted forward assignment of CIntelligentPointer<NParticleEngine::CParticleEmitter> elements; returns the advanced result
// Leaf: ??$__copy@PAV?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@PAV1@H@std@@YIPAV?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006557FB, 45 bytes, __fastcall (first ecx, last edx; result, the tag reference and the distance pointer on the stack), `ret 0Ch`; size-optimised TU (result advanced in place, `add dword ptr [esp+0Ch],8`).
// Identity (proven): std::__copy<CIntelligentPointer<NParticleEngine::CParticleEmitter>*, CIntelligentPointer<NParticleEngine::CParticleEmitter>*, int>; evidence: the 8-byte element's assignment 0x0065533C is called from the player-GUI effect code (CDrawStamina::Draw, CDrawHealth::Draw, CDrawAbilityButtons::DrawEffects, CDrawQuickAccessIcons::DrawEffectAtPos, CDrawMeleeActionButtonEffects::Draw), whose Ego_r.pdb Effects / IncreaseEffects members are all vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >; the live caller 0x00656202 is that vector's erase(first, last) (CDrawStamina::Draw calls it): mutable iterators.
// Label: the manifest's "_Copy_impl<CArray<CRandomAppearanceMorph::CBodyPartMesh>*, ...>" is propagated (family H060: six out-of-line __copy loops, one per element).
// Callee: CIntelligentPointer<NParticleEngine::CParticleEmitter>::operator= 0x0065533C (the element assignment).
// Shape: STLport 4.6 _algobase.h `__copy(first, last, result, random_access_iterator_tag, Distance*)` as written (the landed CWideString
//   0x00433CEC / CArray<unsigned short> 0x00F3B8A0 bodies), `__fastcall` on the template definition.
#pragma optimize("s", on)
namespace NParticleEngine { class CParticleEmitter; }
template <class T> class CIntelligentPointer {               // 8 bytes
public:
    void* _body[2];
    CIntelligentPointer& operator=(const CIntelligentPointer& other);
};

namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template CIntelligentPointer<NParticleEngine::CParticleEmitter>* __fastcall __copy<CIntelligentPointer<NParticleEngine::CParticleEmitter>*, CIntelligentPointer<NParticleEngine::CParticleEmitter>*, int>(CIntelligentPointer<NParticleEngine::CParticleEmitter>*, CIntelligentPointer<NParticleEngine::CParticleEmitter>*, CIntelligentPointer<NParticleEngine::CParticleEmitter>*, const random_access_iterator_tag&, int*);
}