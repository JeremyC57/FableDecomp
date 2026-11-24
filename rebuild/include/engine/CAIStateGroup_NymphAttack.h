#ifndef FABLE_ENGINE_CAISTATEGROUP_NYMPHATTACK_H
#define FABLE_ENGINE_CAISTATEGROUP_NYMPHATTACK_H

#include "engine/CAIStateGroupBase.h"

class CThingCreatureBase;

// Ego_r class record, size 0x14, has no additional members.
class CAIStateGroup_NymphAttack : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();
    virtual void Update();
private:
    bool IsTargetInLineOfSight(CThingCreatureBase* target);
};
FABLE_STATIC_ASSERT_AT(13, sizeof(CAIStateGroup_NymphAttack) == 0x14);

#endif
