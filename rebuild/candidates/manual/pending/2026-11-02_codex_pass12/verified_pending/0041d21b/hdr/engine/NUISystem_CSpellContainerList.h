#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CSPELLCONTAINERLIST_FACTORY_H
#define FABLE_ENGINE_CSPELLCONTAINERLIST_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x544b70.
// Layout: complete native 0x1a8 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CSpellContainerList:public CChangingStateComponent {
public:
    CSpellContainerList(long);
    virtual ~CSpellContainerList();
protected:
    unsigned char LastInputTime[4]; // float; native+0x15c, donor+0x18c.
protected:
    unsigned char SpellIndex[4]; // unsigned long; native+0x160, donor+0x190.
protected:
    unsigned char SelectedChild[4]; // unsigned long; native+0x164, donor+0x194.
protected:
    unsigned char SpellSelectionChild[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x168, donor+0x198.
protected:
    unsigned char SpellIndicationChild[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x16c, donor+0x19c.
protected:
    unsigned char SpellContainers[12]; // std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> >; native+0x170, donor+0x1a0.
protected:
    unsigned char SpellToAssign[4]; // long; native+0x17c, donor+0x1b0.
protected:
    unsigned char PositionOffset[8]; // C2DVector; native+0x180, donor+0x1b4.
protected:
    unsigned char AlphaOffset[4]; // float; native+0x188, donor+0x1bc.
protected:
    unsigned char NextSpellContainerAlpha[4]; // float; native+0x18c, donor+0x1c0.
protected:
    unsigned char NextSpellContainerPosition[8]; // C2DVector; native+0x190, donor+0x1c4.
protected:
    unsigned char PositionOffsetToUse[8]; // C2DVector; native+0x198, donor+0x1cc.
protected:
    unsigned char AlphaOffsetToUse[4]; // float; native+0x1a0, donor+0x1d4.
protected:
    unsigned char InputDelay[4]; // float; native+0x1a4, donor+0x1d8.
};
FABLE_STATIC_ASSERT(sizeof(CSpellContainerList)==0x1a8);
}
#endif

#else

#endif
