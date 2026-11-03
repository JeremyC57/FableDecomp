#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CSCROLLABLELIST_FACTORY_H
#define FABLE_ENGINE_CSCROLLABLELIST_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x555180.
// Layout: complete native 0x17c bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CScrollableList:public CChangingStateComponent {
public:
    CScrollableList(long);
    virtual ~CScrollableList();
protected:
    unsigned char TopOfList[4]; // unsigned long; native+0x15c, donor+0x18c.
protected:
    unsigned char VisibleListItems[4]; // unsigned long; native+0x160, donor+0x190.
protected:
    unsigned char PositionOffset[8]; // C2DVector; native+0x164, donor+0x194.
protected:
    unsigned char ScrollTimer[4]; // float; native+0x16c, donor+0x19c.
protected:
    unsigned char PUpArrow[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x170, donor+0x1a0.
protected:
    unsigned char PDownArrow[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x174, donor+0x1a4.
protected:
    unsigned char UpArrowActive[1]; // bool; native+0x178, donor+0x1a8.
protected:
    unsigned char DownArrowActive[1]; // bool; native+0x179, donor+0x1a9.
    unsigned char _pad_17a[2];
};
FABLE_STATIC_ASSERT(sizeof(CScrollableList)==0x17c);
}
#endif

#else

#endif
