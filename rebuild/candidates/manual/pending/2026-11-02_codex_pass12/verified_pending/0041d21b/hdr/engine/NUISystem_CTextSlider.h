#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CTEXTSLIDER_FACTORY_H
#define FABLE_ENGINE_CTEXTSLIDER_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x549f60.
// Layout: complete native 0x1a0 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CTextSlider:public CChangingStateComponent {
public:
    CTextSlider(long);
    virtual ~CTextSlider();
protected:
    unsigned char SelectedChild[4]; // unsigned long; native+0x15c, donor+0x18c.
protected:
    unsigned char PreviouslySelectedChild[4]; // unsigned long; native+0x160, donor+0x190.
protected:
    unsigned char StartSelectedChild[4]; // unsigned long; native+0x164, donor+0x194.
protected:
    unsigned char Move[1]; // bool; native+0x168, donor+0x198.
    unsigned char _pad_169[3];
protected:
    unsigned char Actions[12]; // std::vector<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >; native+0x16c, donor+0x19c.
protected:
    unsigned char PActionsOnBack[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x178, donor+0x1ac.
protected:
    unsigned char PActionsOnSelect[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x17c, donor+0x1b0.
protected:
    unsigned char LastInputTime[4]; // float; native+0x180, donor+0x1b4.
protected:
    unsigned char InputDelay[4]; // float; native+0x184, donor+0x1b8.
protected:
    unsigned char PDefAction[4]; // NUISystem::EActionType; native+0x188, donor+0x1bc.
protected:
    unsigned char PLeftArrow[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x18c, donor+0x1c0.
protected:
    unsigned char PRightArrow[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x190, donor+0x1c4.
protected:
    unsigned char OriginalSelectedChild[4]; // unsigned long; native+0x194, donor+0x1c8.
protected:
    unsigned char PAcceptButton[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x198, donor+0x1cc.
protected:
    unsigned char MaxChild[4]; // unsigned long; native+0x19c, donor+0x1d0.
};
FABLE_STATIC_ASSERT(sizeof(CTextSlider)==0x1a0);
}
#endif

#else

#endif
