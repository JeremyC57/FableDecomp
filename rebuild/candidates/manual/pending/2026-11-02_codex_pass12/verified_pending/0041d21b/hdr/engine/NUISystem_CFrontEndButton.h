#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CFRONTENDBUTTON_FACTORY_H
#define FABLE_ENGINE_CFRONTENDBUTTON_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x54e0b0.
// Layout: complete native 0x1b4 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CClickable.h"
namespace NUISystem {
class CFrontEndButton:public CClickable {
public:
    CFrontEndButton(long);
    virtual ~CFrontEndButton();
protected:
    unsigned char m_fLastInputTime[4]; // float; native+0x194, donor+0x1c4.
protected:
    unsigned char Actions[12]; // std::vector<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >; native+0x198, donor+0x1c8.
protected:
    unsigned char ActionsOnDelete[12]; // std::vector<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >; native+0x1a4, donor+0x1d8.
protected:
    unsigned char UIActionParams[4]; // std::list<NUISystem::CActionParam *,std::allocator<NUISystem::CActionParam *> >; native+0x1b0, donor+0x1e8.
};
FABLE_STATIC_ASSERT(sizeof(CFrontEndButton)==0x1b4);
}
#endif

#else

#endif
