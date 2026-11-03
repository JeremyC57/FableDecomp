#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CKEYREDEFINER_FACTORY_H
#define FABLE_ENGINE_CKEYREDEFINER_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x558540.
// Layout: complete native 0x1c0 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CKeyRedefiner:public CClickable {
public:
    CKeyRedefiner(long);
    virtual ~CKeyRedefiner();
private:
    unsigned char PActionText[4]; // {"pointer_to":"NUISystem::CText","mode":"pointer","attributes":"0x800a"}; native+0x194, donor+0x1c4.
private:
    unsigned char PKeyText[4]; // {"pointer_to":"NUISystem::CText","mode":"pointer","attributes":"0x800a"}; native+0x198, donor+0x1c8.
private:
    unsigned char Actions[4]; // std::list<enum EGameAction,std::allocator<enum EGameAction> >; native+0x19c, donor+0x1cc.
private:
    unsigned char SubType[4]; // NUISystem::CKeyRedefiner::EGameActionSubType; native+0x1a0, donor+0x1d8.
private:
    unsigned char Definition[28]; // CActionInputControl; native+0x1a4, donor+0x1dc.
};
FABLE_STATIC_ASSERT(sizeof(CKeyRedefiner)==0x1c0);
}
#endif

#else

#endif
