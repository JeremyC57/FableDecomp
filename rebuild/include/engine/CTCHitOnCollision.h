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
#ifdef FABLE_RETAIL_KNOCKDOWN_INITIALISE_PASS28
    // FableWin IsThrowThingsOutLaterally and the native byte test at +0x1b.
    bool IsThrowThingsOutLaterally() const { return PushSideways; }
#endif
private:
    std::vector<CThingGameObject*> CollidedList;
    bool DontHitWhenCombatAction;
    bool Blockable;
    bool PassThroughIfBlocked;
    bool PushSideways;
    float Damage;
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CTCHitOnCollision) == 0x20);
#endif
