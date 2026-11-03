#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CCOMPONENTCONTAINER_FACTORY_H
#define FABLE_ENGINE_CCOMPONENTCONTAINER_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x5460c0.
// Layout: complete native 0x15c bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CComponentContainer:public CChangingStateComponent {
public:
    CComponentContainer(long);
    virtual ~CComponentContainer();

};
FABLE_STATIC_ASSERT(sizeof(CComponentContainer)==0x15c);
}
#endif

#else

#endif
