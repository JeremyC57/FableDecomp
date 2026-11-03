#ifndef FABLE_ENGINE_CTHINGBUILDING_H
#define FABLE_ENGINE_CTHINGBUILDING_H
#include "engine/CThingGameObject.h"
// Ego_r TPI inheritance, members and access; native IsPointInside 0082d000.
class CThingBuilding : public CThingGameObject {
protected:
    long ForceOpenCount;
    bool OverrideAutomaticLocking;
public:
    static long __fastcall StatGetType() { return 3; }
    bool IsPointInside(const C3DVector&) const;
};
FABLE_STATIC_ASSERT(sizeof(CThingBuilding)==0xd8);
#endif
