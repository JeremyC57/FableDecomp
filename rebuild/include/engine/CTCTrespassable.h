#ifndef FABLE_ENGINE_CTCTRESPASSABLE_H
#define FABLE_ENGINE_CTCTRESPASSABLE_H
#ifdef FABLE_RETAIL_CTCTRESPASSABLE
#include "engine/CTCBase.h"
class CTCTrespassable : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_TRESPASSABLE; }
    // Ego_r class members at +0x0C/+0x0D, confirmed by native FrameUpdate.
#ifdef FABLE_RETAIL_UPDATE_CURFEW_PASS29
    bool GetTrespassable()const{return Trespassable;}
    void SetTrespassable(bool value){Trespassable=value;}
#endif
    bool Trespassable;
    bool IsBeingTrespassedIn;
};
FABLE_STATIC_ASSERT_AT(12, sizeof(CTCTrespassable) == 0x10);
#endif
#endif
