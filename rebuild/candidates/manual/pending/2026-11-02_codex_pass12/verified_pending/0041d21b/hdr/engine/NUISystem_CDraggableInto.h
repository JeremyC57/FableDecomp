#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CDRAGGABLEINTO_FACTORY_H
#define FABLE_ENGINE_CDRAGGABLEINTO_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x558ec0.
// Layout: complete native 0x170 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CDraggableInto:public CHoverable {
public:
    CDraggableInto(long);
    virtual ~CDraggableInto();
protected:
    unsigned char PActions[4]; // {"pointer_to":"std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >","mode":"pointer","attributes":"0x800a"}; native+0x16c, donor+0x19c.
};
FABLE_STATIC_ASSERT(sizeof(CDraggableInto)==0x170);
}
#endif

#else

#endif
