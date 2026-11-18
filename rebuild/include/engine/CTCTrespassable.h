#ifndef FABLE_ENGINE_CTCTRESPASSABLE_H
#define FABLE_ENGINE_CTCTRESPASSABLE_H
#ifdef FABLE_RETAIL_CTCTRESPASSABLE
#include "engine/CTCBase.h"
class CTCTrespassable : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_TRESPASSABLE; }
    // Ego_r class members at +0x0C/+0x0D, confirmed by native FrameUpdate.
    bool Trespassable;
    bool IsBeingTrespassedIn;
};
FABLE_STATIC_ASSERT(sizeof(CTCTrespassable) == 0x10);
#endif
#endif
