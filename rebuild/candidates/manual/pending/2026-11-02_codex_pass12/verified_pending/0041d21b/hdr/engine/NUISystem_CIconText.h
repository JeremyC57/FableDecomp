#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CICONTEXT_FACTORY_H
#define FABLE_ENGINE_CICONTEXT_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x542330.
// Layout: complete native 0x1b4 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CChangingStateComponent.h"
namespace NUISystem {
class CIconText:public CChangingStateComponent {
public:
    CIconText(long);
    virtual ~CIconText();
protected:
    unsigned char Text[4]; // CWideString; native+0x15c, donor+0x18c.
protected:
    unsigned char DisplayText[4]; // CWideString; native+0x160, donor+0x190.
protected:
    unsigned char PFont[8]; // CCountedPointer<CFontBank>; native+0x164, donor+0x194.
protected:
    unsigned char Unavailable[1]; // bool; native+0x16c, donor+0x19c.
    unsigned char _pad_16d[3];
protected:
    unsigned char TextHandle[8]; // CEnginePrimitiveHandle; native+0x170, donor+0x1a0.
protected:
    unsigned char DropShadowHandle[8]; // CEnginePrimitiveHandle; native+0x178, donor+0x1a8.
protected:
    unsigned char IconHandles[12]; // std::vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle> >; native+0x180, donor+0x1b0.
protected:
    unsigned char Icons[12]; // std::vector<NUISystem::CIconText::CEmbeddedIcon,std::allocator<NUISystem::CIconText::CEmbeddedIcon> >; native+0x18c, donor+0x1c0.
protected:
    unsigned char Window[16]; // C2DBoxF; native+0x198, donor+0x1d0.
protected:
    unsigned char PositionOffset[8]; // C2DVector; native+0x1a8, donor+0x1e0.
protected:
    unsigned char NoRenderNextFrame[1]; // unsigned char; native+0x1b0, donor+0x1e8.
    unsigned char _pad_1b1[3];
};
FABLE_STATIC_ASSERT(sizeof(CIconText)==0x1b4);
}
#endif

#else

#endif
