#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CREDEFINERLIST_FACTORY_H
#define FABLE_ENGINE_CREDEFINERLIST_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x556350.
// Layout: complete native 0x190 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CScrollableList.h"
namespace NUISystem {
class CRedefinerList:public CScrollableList {
public:
    CRedefinerList(long);
    virtual ~CRedefinerList();
protected:
    unsigned char IsActivated[1]; // bool; native+0x17c, donor+0x1ac.
protected:
    unsigned char RefreshAfterFade[1]; // bool; native+0x17d, donor+0x1ad.
    unsigned char _pad_17e[2];
protected:
    unsigned char FadeOutTime[4]; // float; native+0x180, donor+0x1b0.
protected:
    unsigned char HaveUndefinedControls[1]; // bool; native+0x184, donor+0x1b4.
    unsigned char _pad_185[3];
protected:
    unsigned char PUndefinedWarning[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x188, donor+0x1b8.
protected:
    unsigned char PAcceptButton[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x18c, donor+0x1bc.
};
FABLE_STATIC_ASSERT(sizeof(CRedefinerList)==0x190);
}
#endif

#else

#endif
