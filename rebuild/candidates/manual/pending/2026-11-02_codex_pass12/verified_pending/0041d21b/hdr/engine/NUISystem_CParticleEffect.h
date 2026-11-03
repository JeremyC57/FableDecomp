#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CPARTICLEEFFECT_FACTORY_H
#define FABLE_ENGINE_CPARTICLEEFFECT_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x544010.
// Layout: complete native 0x164 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CParticleEffect:public CChangingStateComponent {
public:
    CParticleEffect(long);
    virtual ~CParticleEffect();
protected:
    unsigned char PHandle[4]; // {"pointer_to":"NParticleEngine::CParticleEmitter","mode":"pointer","attributes":"0x800a"}; native+0x15c, donor+0x18c.
protected:
    unsigned char EffectIndex[4]; // unsigned long; native+0x160, donor+0x190.
};
FABLE_STATIC_ASSERT(sizeof(CParticleEffect)==0x164);
}
#endif

#else

#endif
