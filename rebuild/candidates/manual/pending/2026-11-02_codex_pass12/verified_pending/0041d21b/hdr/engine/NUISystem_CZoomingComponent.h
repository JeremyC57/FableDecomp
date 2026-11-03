#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CZOOMINGCOMPONENT_FACTORY_H
#define FABLE_ENGINE_CZOOMINGCOMPONENT_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x546b00.
// Layout: complete native 0x184 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CZoomingComponent:public CChangingStateComponent {
public:
    CZoomingComponent(long);
    virtual ~CZoomingComponent();
protected:
    unsigned char LastInputTime[4]; // float; native+0x15c, donor+0x18c.
protected:
    unsigned char ZoomCenter[8]; // C2DVector; native+0x160, donor+0x190.
protected:
    unsigned char MinZoom[8]; // C2DVector; native+0x168, donor+0x198.
protected:
    unsigned char MaxZoom[8]; // C2DVector; native+0x170, donor+0x1a0.
protected:
    unsigned char Panning[1]; // bool; native+0x178, donor+0x1a8.
protected:
    unsigned char Zooming[1]; // bool; native+0x179, donor+0x1a9.
    unsigned char _pad_17a[2];
protected:
    unsigned char InputDelay[4]; // float; native+0x17c, donor+0x1ac.
protected:
    unsigned char ScrollingSpeed[4]; // float; native+0x180, donor+0x1b0.
};
FABLE_STATIC_ASSERT(sizeof(CZoomingComponent)==0x184);
}
#endif

#else

#endif
