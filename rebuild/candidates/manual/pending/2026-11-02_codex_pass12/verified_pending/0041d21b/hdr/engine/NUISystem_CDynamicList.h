#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CDYNAMICLIST_FACTORY_H
#define FABLE_ENGINE_CDYNAMICLIST_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x5415f0.
// Layout: complete native 0x180 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CDynamicList:public CChangingStateComponent {
public:
    CDynamicList(long);
    virtual ~CDynamicList();
protected:
    unsigned char BInitialised[4]; // int; native+0x15c, donor+0x18c.
protected:
    unsigned char PositionOffset[8]; // C2DVector; native+0x160, donor+0x190.
protected:
    unsigned char TopIndex[4]; // long; native+0x168, donor+0x198.
protected:
    unsigned char SelectionIndex[4]; // long; native+0x16c, donor+0x19c.
protected:
    unsigned char NumVisible[4]; // long; native+0x170, donor+0x1a0.
protected:
    unsigned char ScrollingChildren[12]; // std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >; native+0x174, donor+0x1a4.
};
FABLE_STATIC_ASSERT(sizeof(CDynamicList)==0x180);
}
#endif

#else

#endif
