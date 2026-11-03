#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CSCROLLBAR_FACTORY_H
#define FABLE_ENGINE_CSCROLLBAR_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x559830.
// Layout: complete native 0x1dc bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CDraggable.h"
namespace NUISystem {
class CScrollBar:public CDraggable {
public:
    CScrollBar(long);
    virtual ~CScrollBar();
protected:
    unsigned char PActionsOnDraggedUp[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x1ac, donor+0x1dc.
protected:
    unsigned char PActionsOnDraggedDown[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x1b0, donor+0x1e0.
protected:
    unsigned char DeltaBeforeAction[8]; // C2DVector; native+0x1b4, donor+0x1e4.
protected:
    unsigned char DeltaForActionUp[8]; // C2DVector; native+0x1bc, donor+0x1ec.
protected:
    unsigned char DeltaForActionDown[8]; // C2DVector; native+0x1c4, donor+0x1f4.
protected:
    unsigned char AmountActionsUp[4]; // unsigned long; native+0x1cc, donor+0x1fc.
protected:
    unsigned char AmountActionsDown[4]; // unsigned long; native+0x1d0, donor+0x200.
protected:
    unsigned char MaxAmountActionsUp[4]; // unsigned long; native+0x1d4, donor+0x204.
protected:
    unsigned char MaxAmountActionsDown[4]; // unsigned long; native+0x1d8, donor+0x208.
};
FABLE_STATIC_ASSERT(sizeof(CScrollBar)==0x1dc);
}
#endif

#else

#endif
