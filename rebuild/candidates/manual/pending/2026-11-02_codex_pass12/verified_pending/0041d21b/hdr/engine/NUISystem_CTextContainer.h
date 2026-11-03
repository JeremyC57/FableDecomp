#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CTEXTCONTAINER_FACTORY_H
#define FABLE_ENGINE_CTEXTCONTAINER_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x546d30.
// Layout: complete native 0x16c bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CTextContainer:public CChangingStateComponent {
public:
    CTextContainer(long);
    virtual ~CTextContainer();
protected:
    unsigned char Window[16]; // C2DBoxF; native+0x15c, donor+0x18c.
};
FABLE_STATIC_ASSERT(sizeof(CTextContainer)==0x16c);
}
#endif

#else

#endif
