#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CSWAPPINGSTATECOMPONENT_FACTORY_H
#define FABLE_ENGINE_CSWAPPINGSTATECOMPONENT_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x547600.
// Layout: complete native 0x170 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CSwappingStateComponent:public CChangingStateComponent {
public:
    CSwappingStateComponent(long);
    virtual ~CSwappingStateComponent();
protected:
    unsigned char SwappingStates[12]; // std::vector<std::pair<unsigned long,float>,std::allocator<std::pair<unsigned long,float> > >; native+0x15c, donor+0x18c.
protected:
    unsigned char LastSwapTime[4]; // float; native+0x168, donor+0x19c.
protected:
    unsigned char Seed[4]; // unsigned long; native+0x16c, donor+0x1a0.
};
FABLE_STATIC_ASSERT(sizeof(CSwappingStateComponent)==0x170);
}
#endif

#else

#endif
