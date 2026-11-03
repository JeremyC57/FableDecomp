#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CCONTROLLERDISCONNECT_FACTORY_H
#define FABLE_ENGINE_CCONTROLLERDISCONNECT_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x41cbe4.
// Layout: complete native 0x160 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CControllerDisconnect:public CChangingStateComponent {
public:
    CControllerDisconnect(long);
    virtual ~CControllerDisconnect();
private:
    unsigned char m_pPrevExclusiveObserver[4]; // {"pointer_to":"NUISystem::CObserver","mode":"pointer","attributes":"0x800a"}; native+0x15c, donor+0x18c.
};
FABLE_STATIC_ASSERT(sizeof(CControllerDisconnect)==0x160);
}
#endif

#else

#endif
