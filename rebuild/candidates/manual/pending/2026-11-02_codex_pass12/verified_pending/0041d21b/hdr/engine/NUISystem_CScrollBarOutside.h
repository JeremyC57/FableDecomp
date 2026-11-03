#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CSCROLLBAROUTSIDE_FACTORY_H
#define FABLE_ENGINE_CSCROLLBAROUTSIDE_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x559360.
// Layout: complete native 0x1a0 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CScrollBarOutside:public CClickable {
public:
    CScrollBarOutside(long);
    virtual ~CScrollBarOutside();
protected:
    unsigned char PScrollBar[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x194, donor+0x1c4.
protected:
    unsigned char PActionsOnLeftClickedAbove[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x198, donor+0x1c8.
protected:
    unsigned char PActionsOnLeftClickedUnder[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x19c, donor+0x1cc.
};
FABLE_STATIC_ASSERT(sizeof(CScrollBarOutside)==0x1a0);
}
#endif

#else

#endif
