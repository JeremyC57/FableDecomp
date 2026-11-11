#ifndef FABLE_ENGINE_CCREATUREACTION_KRAKENROAR_H
#define FABLE_ENGINE_CCREATUREACTION_KRAKENROAR_H
#include "engine/CCombatActionBase.h"
namespace {
// Layout: original 0x54c3a, complete member set; retail base276.
class CCreatureAction_KrakenRoarLoop:public CCombatActionBase {
public:
 virtual CCreatureActionBase * Clone() const;
 CCreatureAction_KrakenRoarLoop(const CCreatureAction_KrakenRoarLoop & p0);
 CCreatureAction_KrakenRoarLoop(CThingCreatureBase & p0, long p1);
 virtual void FinishAction();
 virtual bool IsUsingAnimationMovement() const;
private:
 long LoopNum;
public:
 CCreatureAction_KrakenRoarLoop & operator=(const CCreatureAction_KrakenRoarLoop & p0);
};
// Layout: original 0x54c4b, complete member set; retail base276.
class CCreatureAction_KrakenRoarOutOf:public CCombatActionBase {
public:
 virtual CCreatureActionBase * Clone() const;
 CCreatureAction_KrakenRoarOutOf(const CCreatureAction_KrakenRoarOutOf & p0);
 CCreatureAction_KrakenRoarOutOf(CThingCreatureBase & p0);
 virtual bool IsUsingAnimationMovement() const;
 CCreatureAction_KrakenRoarOutOf & operator=(const CCreatureAction_KrakenRoarOutOf & p0);
};
}
#endif
