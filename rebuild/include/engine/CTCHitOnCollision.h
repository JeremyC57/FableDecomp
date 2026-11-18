#ifndef FABLE_ENGINE_CTCHITONCOLLISION_H
#define FABLE_ENGINE_CTCHITONCOLLISION_H
#include "engine/CTCBase.h"
#include "stlport/fable_containers.h"
class CThing;
class CThingGameObject;
class C3DVector;

// Ego_d and Ego_r type records; retail vector ends at +0x18.
class CTCHitOnCollision : public CTCBase
{
public:
    void OnCollision(const C3DVector&, const C3DVector&, CThing&);
private:
    std::vector<CThingGameObject*> CollidedList;
    bool DontHitWhenCombatAction;
    bool Blockable;
    bool PassThroughIfBlocked;
    bool PushSideways;
    float Damage;
};
FABLE_STATIC_ASSERT(sizeof(CTCHitOnCollision) == 0x20);
#endif
