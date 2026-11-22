#ifndef FABLE_ENGINE_CCOMBATSEQUENCEBASE_H
#define FABLE_ENGINE_CCOMBATSEQUENCEBASE_H
#ifdef FABLE_RETAIL_COMBAT_SEQUENCE_SELECTION_PASS27
#include "stlport/fable_containers.h"
class CCombatAction; class CThingCreatureBase; class CCharString;
enum ECombatSequenceType; enum ECombatSequenceInterruptionType;
class CCombatSequenceBase {
public:
    virtual ~CCombatSequenceBase();
    virtual void OnCreate();
    virtual CCharString GetName() const;
    virtual ECombatSequenceType GetCombatSequenceType() const;
    virtual ECombatSequenceInterruptionType GetCombatSequenceInterruptionType() const;
    virtual bool IsInterruptable() const;
    virtual bool IsInterruptableDueToZoneChange() const;
    virtual bool IsMeleeSequence() const;
    virtual bool IsBreakIntoMeleeSequence() const;
    virtual bool IsBoastSequence() const;
    virtual bool IsOuterRingSequence() const;
    virtual bool IsAllowedOnOuterRing() const;
    virtual bool IsValid() const;
    virtual bool IsToPreferSequenceWhenValid() const;
    virtual bool IsSequenceWithActions() const;
    virtual unsigned long GetZone() const;
    virtual long GetFramesToPredictTargetPosition() const;
    virtual void OnStart();
    virtual void OnStop();
    bool IsZoneValid() const;
private:
    bool Running;
    long NoAvailableActions, CurrentActionIndex;
    std::vector<CCombatAction> CombatActions;
    CThingCreatureBase& Creature;
    CThingCreatureBase& Target;
    bool CreateCalled;
};
FABLE_STATIC_ASSERT(sizeof(CCombatSequenceBase)==0x28);
#endif
#endif
