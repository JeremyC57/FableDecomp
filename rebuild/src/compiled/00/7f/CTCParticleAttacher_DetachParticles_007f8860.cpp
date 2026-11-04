// CTCParticleAttacher::DetachParticles() -- kill every attached particle thing (CKillParticle over ParticleList), then empty the list
// Leaf: ?DetachParticles@CTCParticleAttacher@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F8860, built /Gr.
// Identity: Ego_r.pdb ?DetachParticles@CTCParticleAttacher@@QAEXXZ; ParticleList vector<CIntelligentPointer<CThing>> +0x10.
// Callees: std::for_each<..., CKillParticle> 0x007F8760 (__fastcall under /Gr), vector::erase 0x004AC0F0.
// Shape: `std::for_each(begin, end, CKillParticle()); ParticleList.clear();` (CKillParticle has a user ctor: hidden return slot).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"

namespace std {
// STLport 4.6 _algo.h
template <class _InputIter, class _Function>
_Function for_each(_InputIter __first, _InputIter __last, _Function __f)
{
    for ( ; __first != __last; ++__first)
        __f(*__first);
    return __f;
}
}

class CKillParticle {                                        // FableWin ??RCKillParticle@@QBEXAAV?$CIntelligentPointer@VCThing@@@@@Z
public:
    CKillParticle() { }
    void operator()(CIntelligentPointer<CThing>& particle) const
    {
        if (particle.Get())
            particle->Kill(false);
    }
};

class CParticleAttacherDef;

class CTCParticleAttacher : public CTCBase {                 // Ego_r.pdb CTCParticleAttacher (retail 0x1C with the 12-byte vector)
public:
    CTCParticleAttacher(CThing&);
    virtual ~CTCParticleAttacher();
    void AttachParticles();
    void DetachParticles();
    bool AreParticlesAttached() const;
    CDefPointer<const CParticleAttacherDef>    PDef;         // +0x0c
    std::vector<CIntelligentPointer<CThing> >  ParticleList; // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CTCParticleAttacher) == 0x1c);

void CTCParticleAttacher::DetachParticles()
{
    std::for_each(ParticleList.begin(), ParticleList.end(), CKillParticle());
    ParticleList.clear();
}