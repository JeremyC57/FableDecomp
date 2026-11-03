#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CLISTARROW_FACTORY_H
#define FABLE_ENGINE_CLISTARROW_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x54c1d0.
// Layout: complete native 0x190 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CSprite.h"
namespace NUISystem {
class CListArrow:public CSprite {
public:
    CListArrow(long);
    virtual ~CListArrow();
protected:
    unsigned char MinToDraw[4]; // long; native+0x184, donor+0x1b4.
protected:
    unsigned char MaxToDraw[4]; // long; native+0x188, donor+0x1b8.
protected:
    unsigned char Internal[1]; // bool; native+0x18c, donor+0x1bc.
    unsigned char _pad_18d[3];
};
FABLE_STATIC_ASSERT(sizeof(CListArrow)==0x190);
}
#endif

#else

#endif
