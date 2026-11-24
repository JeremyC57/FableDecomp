#ifndef FABLE_ENGINE_CCREATUREACTION_SUMMONCREATURES_H
#define FABLE_ENGINE_CCREATUREACTION_SUMMONCREATURES_H

#include "engine/CCombatActionBase.h"
class CThingCreatureBase;

// Original class hierarchy and constructor types from Ego_r and FableWin.
class CCreatureAction_SummonCreatures : public CCombatActionBase {
public:
    CCreatureAction_SummonCreatures(CThingCreatureBase& creature, CThingCreatureBase& target, bool spawn_on_target);
private:
    bool SpawnOnTarget;
};
FABLE_STATIC_ASSERT_AT(33, sizeof(CCreatureAction_SummonCreatures) == 0x118);

#endif
