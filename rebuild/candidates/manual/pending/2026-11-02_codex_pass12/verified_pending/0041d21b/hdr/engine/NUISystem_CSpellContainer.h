#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CSPELLCONTAINER_FACTORY_H
#define FABLE_ENGINE_CSPELLCONTAINER_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x545720.
// Layout: complete native 0x17c bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CSpellContainer:public CChangingStateComponent {
public:
    CSpellContainer(long);
    virtual ~CSpellContainer();
protected:
    unsigned char BaseSpells[12]; // std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > >; native+0x15c, donor+0x18c.
protected:
    unsigned char NewSpell[8]; // std::pair<NUISystem::CComponent *,unsigned long>; native+0x168, donor+0x19c.
protected:
    unsigned char EmptySlot[4]; // long; native+0x170, donor+0x1a4.
protected:
    unsigned char PositionOffset[8]; // C2DVector; native+0x174, donor+0x1a8.
};
FABLE_STATIC_ASSERT(sizeof(CSpellContainer)==0x17c);
}
#endif

#else

#endif
