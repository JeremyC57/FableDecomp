#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_COK_FACTORY_H
#define FABLE_ENGINE_COK_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x41cb70.
// Layout: complete native 0x160 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class COk:public CChangingStateComponent {
public:
    COk(long);
    virtual ~COk();
protected:
    unsigned char OkCallback[4]; // {"pointer_to":{"return":"void","owner":null,"this_type":null,"calling_convention":"fastcall","parameter_count":0,"parameters":[],"options":"0x0","this_adjust":null},"mode":"pointer","attributes":"0x800a"}; native+0x15c, donor+0x18c.
};
FABLE_STATIC_ASSERT(sizeof(COk)==0x160);
}
#endif

#else

#endif
