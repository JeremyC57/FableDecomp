#ifndef FABLE_ENGINE_CAISTATEGROUP_VERSUSROCKTROLL_H
#define FABLE_ENGINE_CAISTATEGROUP_VERSUSROCKTROLL_H
#include "engine/CAIStateGroup_VersusEnemyBase.h"
class CAIStateGroup_VersusRockTroll : public CAIStateGroup_VersusEnemyBase {
protected:
    virtual void Update();
private:
    bool GoClockwise;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_VersusRockTroll)==0x20);
#endif
