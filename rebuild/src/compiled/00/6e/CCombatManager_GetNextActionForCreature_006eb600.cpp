// CCombatManager::GetNextActionForCreature(CThingCreatureBase&) -- reuse eligible sequence action or reset and advance the sequence
// Retail0x006EB600,247 bytes. PDB map16 -> retail tree12 shifts Combatants20->16; native node pair data+24.
// Creature PCurrentAction is native+0x100, donor+0x11c; UID inherited at+0x18. Local retail view documents that mismatch.
#include "fable_string.h"
#include "engine/CCreatureActionBase.h"
#define CThingCreatureBase CThingCreatureBase_PDB
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
class CCombatActionBase;
struct CreatureUID { unsigned long Low,High; };
class CThingCreatureBase { public:
    unsigned char _beforeUID[0x18];CreatureUID UID;unsigned char _beforeCurrentAction[0xe0];
    CCreatureActionBase* PCurrentAction_Data;
    bool IsCombatant() const; // 0x00662410: delegates via world combat manager to0x006E92C0
    bool IsPerformingAnyAction() const; //0x006622A0
};
class CCombatSequenceBase { public:
    virtual ~CCombatSequenceBase();
    virtual void OnCreate();
    virtual CCharString GetName() const;
    virtual long GetCombatSequenceType() const;
    virtual long GetCombatSequenceInterruptionType() const;
    virtual bool IsInterruptable() const;
    virtual bool IsInterruptableDueToZoneChange() const; //slot6 verified0x006F06F0
    bool Running;
    bool IsZoneValid() const; //0x008B0610
    CCombatActionBase* GetNextActionInSequence(); //0x008B0780
};
struct SequencePointer { CCombatSequenceBase* Data; void* Info; };
class CAttackingCombatantInfo { public:
    CThingCreatureBase* Attacked;long AttackerNumber,CombatWheelRingIndex;
    bool PreferredAttackDollySet;long PreferredAttackDollyTimer;float PreferredAttackDollyVector[2];bool UsingCombatWheel;
    long CurrentSequence;SequencePointer* Begin;SequencePointer* End;SequencePointer* Capacity;
};
struct CCombatantInfo { CThingCreatureBase* Parent;CAttackingCombatantInfo AttackerInfo; };
struct CombatNode { unsigned long Colour;CombatNode* Parent;CombatNode* Left;CombatNode* Right;CreatureUID Key;CCombatantInfo Value; };
struct CombatMap { CombatNode* Header;unsigned long Count;unsigned long Compare;CombatNode* FindLE(const CreatureUID& key); }; //0x006F0250
class CCombatManager { public:
    void* VTable;unsigned char RegisteredCombatSequences[12];CombatMap Combatants;
    void PerformSequenceReset(CThingCreatureBase&); //0x006EA2C0
    void SetupNextSequenceForAttacker(CCombatantInfo&); //0x006EB390
    CCombatActionBase* GetNextActionForCreature(CThingCreatureBase& creature);
};
CCombatActionBase* CCombatManager::GetNextActionForCreature(CThingCreatureBase& creature){
    if(!creature.IsCombatant())return 0;
    CreatureUID key=creature.UID;
    CCombatantInfo& info=Combatants.FindLE(key)->Value;
    bool canAct=true;
    if(creature.IsPerformingAnyAction() && !creature.PCurrentAction_Data->Finished)canAct=false;
    if(info.AttackerInfo.CurrentSequence>=0){
        CCombatSequenceBase* sequence=info.AttackerInfo.Begin[info.AttackerInfo.CurrentSequence].Data;
        if(sequence->Running && (sequence->IsZoneValid() || !sequence->IsInterruptableDueToZoneChange())){
            CCombatActionBase* action=sequence->GetNextActionInSequence();
            if(action)return canAct?action:0;
        }
        if(!canAct)return 0;
        PerformSequenceReset(creature);
    }
    SetupNextSequenceForAttacker(info);
    if(creature.IsCombatant() && info.AttackerInfo.CurrentSequence>=0)
        return info.AttackerInfo.Begin[info.AttackerInfo.CurrentSequence].Data->GetNextActionInSequence();
    return 0;
}