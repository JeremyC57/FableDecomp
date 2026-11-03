#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CDRAGGABLE_FACTORY_H
#define FABLE_ENGINE_CDRAGGABLE_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x55a9c0.
// Layout: complete native 0x1ac bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CDraggable:public CClickable {
public:
    CDraggable(long);
    virtual ~CDraggable();
protected:
    unsigned char PositionBeforeDrag[8]; // C2DVector; native+0x194, donor+0x1c4.
protected:
    unsigned char WasDragged[1]; // bool; native+0x19c, donor+0x1cc.
    unsigned char _pad_19d[3];
protected:
    unsigned char PActionsOnDropped[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x1a0, donor+0x1d0.
protected:
    unsigned char PActionsOnDroppedNowhere[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x1a4, donor+0x1d4.
protected:
    unsigned char SnapBackIfDroppedNowhere[1]; // bool; native+0x1a8, donor+0x1d8.
protected:
    unsigned char ChangeLayer[1]; // bool; native+0x1a9, donor+0x1d9.
    unsigned char _pad_1aa[2];
};
FABLE_STATIC_ASSERT(sizeof(CDraggable)==0x1ac);
}
#endif

#else

#endif
