#ifndef FABLE_ENGINE_CCOMBATSEQUENCEBASE_H
#define FABLE_ENGINE_CCOMBATSEQUENCEBASE_H
#if defined(FABLE_RETAIL_COMBAT_SEQUENCE_SELECTION_PASS27) || defined(FABLE_RETAIL_CCOMBATSEQUENCEBASE) // NF325: + the class-model opt-in (NF322 Q1 ruling)
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
#ifdef FABLE_RETAIL_CCOMBATSEQUENCEBASE // NF325: retail access (RTTI vftable 0x01278FEC slots 16-18 + FableWin.pdb ?GetFramesToPredictTargetPosition@CCombatSequenceBase@@MBEJXZ, ?OnStart@..@@MAEXXZ, ?OnStop@..@@MAEXXZ)
protected:
#endif
    virtual long GetFramesToPredictTargetPosition() const;
    virtual void OnStart();
    virtual void OnStop();
#ifdef FABLE_RETAIL_CCOMBATSEQUENCEBASE
public:
#endif
    bool IsZoneValid() const;
private:
    bool Running;
    long NoAvailableActions, CurrentActionIndex;
    std::vector<CCombatAction> CombatActions;
    CThingCreatureBase& Creature;
    CThingCreatureBase& Target;
    bool CreateCalled;
};
FABLE_STATIC_ASSERT_AT(37, sizeof(CCombatSequenceBase)==0x28);
#endif
#endif
