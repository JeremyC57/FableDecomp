#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CYESNO_FACTORY_H
#define FABLE_ENGINE_CYESNO_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x41cadc.
// Layout: complete native 0x164 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CYesNo:public CChangingStateComponent {
public:
    CYesNo(long);
    virtual ~CYesNo();
protected:
    unsigned char YesCallback[4]; // {"pointer_to":{"return":"void","owner":null,"this_type":null,"calling_convention":"fastcall","parameter_count":0,"parameters":[],"options":"0x0","this_adjust":null},"mode":"pointer","attributes":"0x800a"}; native+0x15c, donor+0x18c.
protected:
    unsigned char NoCallback[4]; // {"pointer_to":{"return":"void","owner":null,"this_type":null,"calling_convention":"fastcall","parameter_count":0,"parameters":[],"options":"0x0","this_adjust":null},"mode":"pointer","attributes":"0x800a"}; native+0x160, donor+0x190.
};
FABLE_STATIC_ASSERT(sizeof(CYesNo)==0x164);
}
#endif

#else

#endif
