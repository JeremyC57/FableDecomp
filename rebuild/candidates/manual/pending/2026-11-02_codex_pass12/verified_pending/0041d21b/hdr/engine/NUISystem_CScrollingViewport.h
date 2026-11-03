#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CSCROLLINGVIEWPORT_FACTORY_H
#define FABLE_ENGINE_CSCROLLINGVIEWPORT_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x53f120.
// Layout: complete native 0x19c bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CViewport.h"
namespace NUISystem {
class CScrollingViewport:public CViewport {
public:
    CScrollingViewport(long);
    virtual ~CScrollingViewport();
protected:
    unsigned char ScrollingSpeed[4]; // float; native+0x174, donor+0x1a8.
protected:
    unsigned char Window[16]; // C2DBoxF; native+0x178, donor+0x1ac.
protected:
    unsigned char PScrollBar[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x188, donor+0x1bc.
protected:
    unsigned char PScrollBarOutside[4]; // {"pointer_to":"NUISystem::CComponent","mode":"pointer","attributes":"0x800a"}; native+0x18c, donor+0x1c0.
protected:
    unsigned char Changed[1]; // bool; native+0x190, donor+0x1c4.
    unsigned char _pad_191[3];
protected:
    unsigned char OffsetToUseAbove[4]; // float; native+0x194, donor+0x1c8.
protected:
    unsigned char OffsetToUseUnder[4]; // float; native+0x198, donor+0x1cc.
};
FABLE_STATIC_ASSERT(sizeof(CScrollingViewport)==0x19c);
}
#endif

#else

#endif
