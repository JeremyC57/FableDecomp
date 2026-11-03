#ifdef FABLE_RETAIL_UI_FACTORY
#ifndef FABLE_ENGINE_CMOUSECURSOR_FACTORY_H
#define FABLE_ENGINE_CMOUSECURSOR_FACTORY_H
// Retail opt-in (Codex pass 12): original Ego_r class and member order; RTTI constructor 0x55c650.
// Layout: complete native 0x184 bytes, allocation in0041d21b; inherited native bases and STLport4.6 member sizes.
#include "engine/NUISystem_CSprite.h"
namespace NUISystem {
class CMouseCursor:public CSprite {
public:
    CMouseCursor(long);
    virtual ~CMouseCursor();

};
FABLE_STATIC_ASSERT(sizeof(CMouseCursor)==0x184);
}
#endif

#else

#endif
