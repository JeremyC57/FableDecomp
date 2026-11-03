#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CEDITBOX_FACTORY_H
#define FABLE_ENGINE_CEDITBOX_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x5407b0.
// Layout: complete native 0x18c bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CEditBox:public CChangingStateComponent {
public:
    CEditBox(long);
    virtual ~CEditBox();
protected:
    unsigned char ActionsOnEnter[4]; // std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >; native+0x15c, donor+0x18c.
protected:
    unsigned char ActionsOnEscape[4]; // std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > >; native+0x160, donor+0x198.
protected:
    unsigned char Text[4]; // CWideString; native+0x164, donor+0x1a4.
protected:
    unsigned char CursorPos[4]; // long; native+0x168, donor+0x1a8.
protected:
    unsigned char LeftMostCharacter[4]; // long; native+0x16c, donor+0x1ac.
protected:
    unsigned char RightMostCharacter[4]; // long; native+0x170, donor+0x1b0.
protected:
    unsigned char Adjustment[4]; // long; native+0x174, donor+0x1b4.
protected:
    unsigned char Active[1]; // bool; native+0x178, donor+0x1b8.
protected:
    unsigned char IsPasswordBox[1]; // bool; native+0x179, donor+0x1b9.
protected:
    unsigned char UsesIME[1]; // bool; native+0x17a, donor+0x1ba.
protected:
    unsigned char DisallowSpaceAsFirstChar[1]; // bool; native+0x17b, donor+0x1bb.
protected:
    unsigned char DeleteAllOnKeyPress[1]; // bool; native+0x17c, donor+0x1bc.
    unsigned char _pad_17d[3];
protected:
    unsigned char MaxLength[4]; // long; native+0x180, donor+0x1c0.
protected:
    unsigned char AllowedCharacters[4]; // CWideString; native+0x184, donor+0x1c4.
protected:
    unsigned char ForbiddenCharacters[4]; // CWideString; native+0x188, donor+0x1c8.
};
FABLE_STATIC_ASSERT(sizeof(CEditBox)==0x18c);
}
#endif

#else

#endif
