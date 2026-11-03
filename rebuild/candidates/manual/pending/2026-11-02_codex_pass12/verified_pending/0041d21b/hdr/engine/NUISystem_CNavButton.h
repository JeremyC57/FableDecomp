#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CNAVBUTTON_FACTORY_H
#define FABLE_ENGINE_CNAVBUTTON_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x558b90.
// Layout: complete native 0x194 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CNavButton:public CClickable {
public:
    CNavButton(long);
    virtual ~CNavButton();

};
FABLE_STATIC_ASSERT(sizeof(CNavButton)==0x194);
}
#endif

#else

#endif
