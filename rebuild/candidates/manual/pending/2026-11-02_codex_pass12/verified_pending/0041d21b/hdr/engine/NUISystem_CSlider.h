#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CSLIDER_FACTORY_H
#define FABLE_ENGINE_CSLIDER_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x54c050.
// Layout: complete native 0x1ec bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CSprite.h"
namespace NUISystem {
class CSlider:public CSprite {
public:
    CSlider(long);
    virtual ~CSlider();
protected:
    unsigned char Dimensions[8]; // C2DVector; native+0x184, donor+0x1b4.
protected:
    unsigned char Min[8]; // C2DVector; native+0x18c, donor+0x1bc.
protected:
    unsigned char Max[8]; // C2DVector; native+0x194, donor+0x1c4.
protected:
    unsigned char Step[8]; // C2DVector; native+0x19c, donor+0x1cc.
protected:
    unsigned char Value[8]; // C2DVector; native+0x1a4, donor+0x1d4.
protected:
    unsigned char Move[1]; // bool; native+0x1ac, donor+0x1dc.
protected:
    unsigned char OldValue[8]; // C2DVector; native+0x1ad, donor+0x1dd.
protected:
    unsigned char StartValue[8]; // C2DVector; native+0x1b5, donor+0x1e5.
    unsigned char _pad_1bd[3];
protected:
    unsigned char Actions[12]; // std::vector<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >; native+0x1c0, donor+0x1f0.
protected:
    unsigned char PActionsOnBack[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x1cc, donor+0x200.
protected:
    unsigned char PActionsOnSelect[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x1d0, donor+0x204.
protected:
    unsigned char PDefAction[4]; // NUISystem::EActionType; native+0x1d4, donor+0x208.
protected:
    unsigned char PLeftArrow[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x1d8, donor+0x20c.
protected:
    unsigned char PRightArrow[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x1dc, donor+0x210.
protected:
    unsigned char PreviousValue[8]; // C2DVector; native+0x1e0, donor+0x214.
protected:
    unsigned char PAcceptButton[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x1e8, donor+0x21c.
};
FABLE_STATIC_ASSERT(sizeof(CSlider)==0x1ec);
}
#endif

#else

#endif
