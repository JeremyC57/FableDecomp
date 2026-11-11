#ifndef FABLE_ENGINE_CTCTARGETINGAI_FILTERS_H
#define FABLE_ENGINE_CTCTARGETINGAI_FILTERS_H
// Original complete PDB records in both Ego_r and FableWin; native007f6200 layout/calls.
#include "engine/CThingFilter_IsAICreatureWillingToAttack.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsThingAbleToBeEngagedInCombat.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CThingFilter_IsNotList.h"
namespace {
class CThingFilter_IsAICreatureAwareOfMoreGenerous : public std::unary_function<CThingCreatureBase,bool> {
public:
 CThingFilter_IsAICreatureAwareOfMoreGenerous(CThingAICreature const * p0);
 bool operator()(CThingCreatureBase const & p0) const;
private:
 CThingAICreature const * PCreature;
};
class CThingFilter_IsValidAttackTargetForCreature {
public:
 CThingFilter_IsValidAttackTargetForCreature(CThingAICreature const & p0, std::vector<CThing const *,std::allocator<CThing const *> > const & p1, bool p2);
 bool operator()(CThingCreatureBase const & p0) const;
private:
 CThingFilter_IsAICreatureWillingToAttack FilterWilling;
 CThingFilter_IsAICreatureAwareOfMoreGenerous FilterAware;
 CThingFilter_IsNot FilterNot;
 CThingFilter_IsThingAbleToBeEngagedInCombat FilterAbleToBeEngaged;
 CThingFilter_EnemyCreature FilterEnemy;
 CThingFilter_IsNotList FilterNotInList;
 bool CheckAwareness;
 bool IAmAGhostExclaim;
};
}
#endif
