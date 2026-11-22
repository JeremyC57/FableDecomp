#ifndef FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEUPDATEPARAMETERS_H
#define FABLE_ENGINE_NPARTICLEENGINE_CPARTICLEUPDATEPARAMETERS_H
// New header (chunk Q3, rows 0x008E20F0 / 0x008E24F0): NParticleEngine::CParticleUpdateParameters<K, V> (vtable 0x0125FFDC for
// <unsigned long, unsigned long>), the model of the landed 0x00767DA0 UpdateParticleAttachment and chunk 796's 0x008E18D0 draft,
// on the shared engine/CVectorMap.h. Ego_r.pdb: : CBaseClass, VectorParams +0x04, UpdateParams +0x14 (retail 0x10-byte CVectorMaps,
// sizeof 0x24 as engine/CParticleEmitter.h's UpdateParameters[0x24]); the nested compare classes are the PDB's
// CParticleUpdateCCompareVector<T> / CParticleUpdateCCompareParamDesc<T> (the std::sort instantiations 0x0076C000 / 0x0076C1A0).
// The destructor is out of line (0x006E2500). Needs FABLE_RETAIL_CVECTORMAP_INSERT (Insert / Update) and FABLE_RETAIL_STLP_SORT_FASTCALL.
#include "engine/CBaseClass.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_VECTORKEY_PASS26
#include <string.h>
#include "engine/CCRC.h"
#endif
namespace NParticleEngine {
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_VECTORKEY_PASS26
// Original FableWin 0x025F1360 calls strlen, then CCRC::Calc; retail Activate inlines this wrapper.
inline unsigned long FABLE_FASTCALL MAKE_VECTOR_PARAM_FROM_BASE(unsigned long base, const char* parameter) {
    return CCRC::Calc(base, parameter, (unsigned long)strlen(parameter));
}
#endif
template <class K, class V> class CParticleUpdateParamCallback;
template <class K, class V> class CParticleUpdateParamDesc : public CBaseClass {
public:
    CParticleUpdateParamCallback<K, V>* Callback;            // +0x04
    unsigned long Index;                                     // +0x08
};
template <class K, class V> class CParticleUpdateParameters : public CBaseClass {
public:
    template <class T> class CParticleUpdateCCompareVector { public:
        bool operator()(const std::pair<T, C3DVector>& a, const std::pair<T, C3DVector>& b) const { return a.first < b.first; }
    };
    template <class T> class CParticleUpdateCCompareParamDesc { public:
        bool operator()(const std::pair<T, CParticleUpdateParamDesc<K, V> >& a, const std::pair<T, CParticleUpdateParamDesc<K, V> >& b) const { return a.first < b.first; }
    };
    CVectorMap<K, C3DVector, CParticleUpdateCCompareVector<K> > VectorParams;                                 // +0x04
    CVectorMap<K, CParticleUpdateParamDesc<K, V>, CParticleUpdateCCompareParamDesc<K> > UpdateParams;        // +0x14
#if defined(FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_NOINLINE) && defined(_MSC_VER)
    // Per-TU opt-in (row 0x008548B0): the same inline body, kept out of line as that TU calls the COMDAT 0x006E24D0; the visible
    // body tells VC7.1 the call writes nothing but the object (retail keeps the LIGHTNING_START key in edi across it, 0x00854AD5).
    __declspec(noinline) CParticleUpdateParameters() { }
#elif defined(FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_OUTOFLINE)
    // Per-TU opt-in (row 0x008548B0 CCreatureAction_SpiritAttack::CreateAttackEffect): that TU calls the constructor COMDAT
    // ??0?$CParticleUpdateParameters@KK@NParticleEngine@@QAE@XZ 0x006E24D0 (`lea ecx,[params] ; call 0x006E24D0`, 0x00854AD5).
    CParticleUpdateParameters();                             // 0x006E24D0
#elif defined(FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_FORCEINLINE)
    // NF55 opt-in (2026-11-17, row 0x007C25A0 CTCShotDefinitionBase::InitAttachedEffects): the same inline body, forced inline: that large
    // TU expands it (CBaseClass ctor 0x0099A2D0, vtable 0x0125FFDC, the two maps' words and Dirty bytes zeroed in place), where VC7.1 calls
    // a plain-inline copy.
    FABLE_FORCEINLINE CParticleUpdateParameters() { }
#else
    CParticleUpdateParameters() { }
#endif
#ifndef FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_IMPLICIT_DTOR
    virtual ~CParticleUpdateParameters();                    // 0x006E2500
#else
    // Opt-in (p25N / AUTH10, row 0x008E18D0; evidence AUTH15 2026-11-22): no user-declared destructor. Ego_d.pdb (field list 0x232B4),
    // FableWin.pdb (0x232B4) and Ego_r.pdb (0x213ED) all list ~CParticleUpdateParameters<unsigned long,unsigned long> as
    // `public virtual compiler-generated` (virtual through CBaseClass). The implicit one is expanded in the row's sky block and
    // called at 0x008E1C3A, and its out-of-line COMDAT compares SAME with retail 0x006E2500 (??1?$CParticleUpdateParameters@KK@...).
#endif
#ifdef FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_UPDATE_DECL_PASS26
    // Original named COMDAT at native 0x0076C750, called by Activate.
    void Update();
#else
    void Update() { VectorParams.Update(); UpdateParams.Update(); }
#endif
};
}
#endif
